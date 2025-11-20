#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QDateEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDate>

#include "TransactionFilter.h"

class FiltersPanel : public QWidget {
public:
    explicit FiltersPanel(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        setObjectName("searchPanel");
        setFixedWidth(280);

        QVBoxLayout* rightLayout = new QVBoxLayout(this);
        rightLayout->setContentsMargins(16, 16, 16, 16);
        rightLayout->setSpacing(12);

        QHBoxLayout* searchHeader = new QHBoxLayout();
        QLabel* searchTitle = new QLabel(QString::fromUtf8("Поиск"), this);
        QFont titleFont = searchTitle->font();
        titleFont.setPointSize(12);
        titleFont.setBold(true);
        searchTitle->setFont(titleFont);

        analyticsButton_ = new QPushButton(QString::fromUtf8("Аналитика"), this);
        analyticsButton_->setCursor(Qt::PointingHandCursor);

        searchHeader->addWidget(searchTitle);
        searchHeader->addStretch();
        searchHeader->addWidget(analyticsButton_);
        rightLayout->addLayout(searchHeader);

        nameSearchEdit_ = new QLineEdit(this);
        nameSearchEdit_->setPlaceholderText(QString::fromUtf8("Поиск по названию"));
        nameSearchEdit_->setMinimumHeight(36);
        rightLayout->addWidget(nameSearchEdit_);

        categoryCombo_ = new QComboBox(this);
        categoryView_ = new QTreeView(categoryCombo_);
        categoryModel_ = new QStandardItemModel(categoryCombo_);
        categoryCombo_->setModel(categoryModel_);
        categoryCombo_->setView(categoryView_);
        categoryCombo_->setEditable(false);
        categoryCombo_->setMinimumHeight(36);
        buildCategoryModel();
        rightLayout->addWidget(categoryCombo_);

        QHBoxLayout* datesLayout = new QHBoxLayout();
        datesLayout->setSpacing(8);

        dateFromEdit_ = new QDateEdit(QDate::currentDate().addDays(-30), this);
        dateFromEdit_->setCalendarPopup(true);
        dateFromEdit_->setDisplayFormat("dd.MM.yyyy");
        dateFromEdit_->setMinimumHeight(36);

        dateToEdit_ = new QDateEdit(QDate::currentDate(), this);
        dateToEdit_->setCalendarPopup(true);
        dateToEdit_->setDisplayFormat("dd.MM.yyyy");
        dateToEdit_->setMinimumHeight(36);

        datesLayout->addWidget(dateFromEdit_);
        datesLayout->addWidget(new QLabel("-", this));
        datesLayout->addWidget(dateToEdit_);
        rightLayout->addLayout(datesLayout);

        resetFiltersButton_ = new QPushButton(QString::fromUtf8("Сбросить фильтры"), this);
        resetFiltersButton_->setMinimumHeight(36);
        resetFiltersButton_->setCursor(Qt::PointingHandCursor);
        rightLayout->addWidget(resetFiltersButton_);

        rightLayout->addStretch();
    }

    FilterCriteria currentCriteria() const {
        FilterCriteria c;
        c.nameQuery = nameSearchEdit_->text().trimmed();

        QModelIndex mi = categoryView_->currentIndex();
        c.typeSel = mi.data(Qt::UserRole + 1).isValid()
            ? mi.data(Qt::UserRole + 1).toInt()
            : -1;
        c.category = mi.data(Qt::UserRole + 2).toString();

        c.fromDate = dateFromEdit_->date();
        c.toDate = dateToEdit_->date();
        return c;
    }

    void resetFiltersToDefault() {
        nameSearchEdit_->clear();
        categoryCombo_->setCurrentIndex(0);
        dateFromEdit_->setDate(QDate::currentDate().addMonths(-1));
        dateToEdit_->setDate(QDate::currentDate());
    }

    QLineEdit* nameSearchEdit() const { return nameSearchEdit_; }
    QComboBox* categoryCombo() const { return categoryCombo_; }
    QTreeView* categoryView() const { return categoryView_; }
    QDateEdit* dateFromEdit() const { return dateFromEdit_; }
    QDateEdit* dateToEdit() const { return dateToEdit_; }
    QPushButton* resetFiltersButton() const { return resetFiltersButton_; }
    QPushButton* analyticsButton() const { return analyticsButton_; }

private:
    void buildCategoryModel() {
        categoryModel_->clear();
        categoryModel_->setHorizontalHeaderLabels({QString::fromUtf8("Категория")});

        auto makeParent = [&](const QString& title) {
            auto* parent = new QStandardItem(title);
            parent->setFlags(Qt::ItemIsEnabled);
            return parent;
        };

        QStandardItem* incomeRoot = makeParent(QString::fromUtf8("Доходы"));
        QStringList incomeCats = {
            QString::fromUtf8("Зарплата"), QString::fromUtf8("Премия"), QString::fromUtf8("Бонусы"),
            QString::fromUtf8("Фриланс"), QString::fromUtf8("Подарки"), QString::fromUtf8("Пассивный доход"),
            QString::fromUtf8("Дивиденды"), QString::fromUtf8("Проценты по вкладам"), QString::fromUtf8("Аренда имущества"),
            QString::fromUtf8("Продажа вещей"), QString::fromUtf8("Прочее")
        };
        for (const auto& c : incomeCats) {
            auto* item = new QStandardItem(c);
            item->setData(1, Qt::UserRole + 1);
            item->setData(c, Qt::UserRole + 2);
            incomeRoot->appendRow(item);
        }

        QStandardItem* expenseRoot = makeParent(QString::fromUtf8("Расходы"));
        QStringList expenseCats = {
            QString::fromUtf8("Продукты"), QString::fromUtf8("Кафе и рестораны"), QString::fromUtf8("Транспорт"),
            QString::fromUtf8("Такси"), QString::fromUtf8("ЖКХ"), QString::fromUtf8("Связь и интернет"),
            QString::fromUtf8("Одежда и обувь"), QString::fromUtf8("Здоровье"), QString::fromUtf8("Аптека"),
            QString::fromUtf8("Образование"), QString::fromUtf8("Подписки"), QString::fromUtf8("Развлечения"),
            QString::fromUtf8("Путешествия"), QString::fromUtf8("Авто: топливо"), QString::fromUtf8("Авто: обслуживание"),
            QString::fromUtf8("Дом и ремонт"), QString::fromUtf8("Дети"), QString::fromUtf8("Подарки"),
            QString::fromUtf8("Налоги и сборы"), QString::fromUtf8("Благотворительность"), QString::fromUtf8("Прочее")
        };
        for (const auto& c : expenseCats) {
            auto* item = new QStandardItem(c);
            item->setData(0, Qt::UserRole + 1);
            item->setData(c, Qt::UserRole + 2);
            expenseRoot->appendRow(item);
        }

        auto* anyItem = new QStandardItem(QString::fromUtf8("Любая категория"));
        anyItem->setData(-1, Qt::UserRole + 1);
        anyItem->setData(QString(), Qt::UserRole + 2);
        categoryModel_->appendRow(anyItem);

        categoryModel_->appendRow(incomeRoot);
        categoryModel_->appendRow(expenseRoot);

        categoryView_->expandAll();
        categoryCombo_->setCurrentIndex(0);
    }

    QLineEdit* nameSearchEdit_ = nullptr;
    QComboBox* categoryCombo_ = nullptr;
    QTreeView* categoryView_ = nullptr;
    QStandardItemModel* categoryModel_ = nullptr;
    QDateEdit* dateFromEdit_ = nullptr;
    QDateEdit* dateToEdit_ = nullptr;
    QPushButton* resetFiltersButton_ = nullptr;
    QPushButton* analyticsButton_ = nullptr;
};
