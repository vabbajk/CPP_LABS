#pragma once

#include <list>
#include <vector>
#include <memory>

#include <QString>
#include <QDate>

#include "Transaction.h"

struct FilterCriteria {
    QString nameQuery;
    int typeSel;           // -1 = any
    QString category;
    QDate fromDate;
    QDate toDate;
};

class TransactionFilter {
public:
    static std::vector<std::shared_ptr<Transaction>> filterAndSort(
        const std::list<std::shared_ptr<Transaction>>& all,
        const FilterCriteria& criteria
    ) {
        std::list<std::shared_ptr<Transaction>> filtered;

        if (criteria.fromDate > criteria.toDate) {
            // Некорректный диапазон дат — вернём пустой список, как и раньше
            return {};
        }

        for (const auto& t : all) {
            bool ok = true;

            if (!criteria.nameQuery.isEmpty()) {
                ok &= QString::fromUtf8(t->getName().c_str())
                          .contains(criteria.nameQuery, Qt::CaseInsensitive);
            }

            if (criteria.typeSel != -1 && !criteria.category.isEmpty()) {
                ok &= (t->getType() == criteria.typeSel) &&
                      (QString::fromUtf8(t->getCategory().c_str()) == criteria.category);
            }

            const Date td = t->getDate();
            QDate qtd(td.getYear(), td.getMonth(), td.getDay());
            ok &= !(qtd < criteria.fromDate) && !(criteria.toDate < qtd);

            if (ok) {
                filtered.push_back(t);
            }
        }

        // Преобразуем в вектор и применим сортировку по умолчанию
        std::vector<std::shared_ptr<Transaction>> ordered(filtered.begin(), filtered.end());

        // Логика сортировки повторяет текущую ветку "по умолчанию" из MainWindow::applyFiltersAndUpdateTable:
        // сортировка по дате (новые сверху)
        std::sort(ordered.begin(), ordered.end(), [](const std::shared_ptr<Transaction>& a,
                                                     const std::shared_ptr<Transaction>& b) {
            const Date& da = a->getDate();
            const Date& db = b->getDate();
            if (da.getYear() != db.getYear()) return da.getYear() > db.getYear();
            if (da.getMonth() != db.getMonth()) return da.getMonth() > db.getMonth();
            if (da.getDay() != db.getDay()) return da.getDay() > db.getDay();
            return a->getID() > b->getID();
        });

        return ordered;
    }
};
