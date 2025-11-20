#pragma once

#include <QStringList>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>

inline QStringList getIncomeCategories() {
    return {
        QString::fromUtf8("Зарплата"),
        QString::fromUtf8("Премия"),
        QString::fromUtf8("Бонусы"),
        QString::fromUtf8("Фриланс"),
        QString::fromUtf8("Подарки"),
        QString::fromUtf8("Пассивный доход"),
        QString::fromUtf8("Дивиденды"),
        QString::fromUtf8("Проценты по вкладам"),
        QString::fromUtf8("Аренда имущества"),
        QString::fromUtf8("Продажа вещей"),
        QString::fromUtf8("Прочее")
    };
}

inline QStringList getExpenseCategories() {
    return {
        QString::fromUtf8("Продукты"),
        QString::fromUtf8("Кафе и рестораны"),
        QString::fromUtf8("Транспорт"),
        QString::fromUtf8("Такси"),
        QString::fromUtf8("ЖКХ"),
        QString::fromUtf8("Связь и интернет"),
        QString::fromUtf8("Одежда и обувь"),
        QString::fromUtf8("Здоровье"),
        QString::fromUtf8("Аптека"),
        QString::fromUtf8("Образование"),
        QString::fromUtf8("Подписки"),
        QString::fromUtf8("Развлечения"),
        QString::fromUtf8("Путешествия"),
        QString::fromUtf8("Авто: топливо"),
        QString::fromUtf8("Авто: обслуживание"),
        QString::fromUtf8("Дом и ремонт"),
        QString::fromUtf8("Дети"),
        QString::fromUtf8("Подарки"),
        QString::fromUtf8("Налоги и сборы"),
        QString::fromUtf8("Благотворительность"),
        QString::fromUtf8("Прочее")
    };
}

inline QString getCommonTransactionDialogStyleSheet() {
    return QString::fromUtf8(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #161b22,
                stop:1 #0d1117);
        }
        QLabel {
            color: #e6edf3;
            font-weight: 600;
            font-size: 10pt;
        }
        QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox {
            background: #0d1117;
            color: #e6edf3;
            border: 2px solid #21262d;
            border-radius: 10px;
            padding: 10px 12px;
            font-size: 10pt;
            min-height: 20px;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QDoubleSpinBox:focus {
            border: 2px solid #388bfd;
            background: #161b22;
        }
        QComboBox::drop-down {
            border: 0;
            width: 30px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 5px solid #7d8590;
        }
        QComboBox QAbstractItemView {
            background: #161b22;
            border: 2px solid #30363d;
            border-radius: 8px;
            selection-background-color: #1f6feb;
            selection-color: #ffffff;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #21262d,
                stop:1 #161b22);
            color: #e6edf3;
            border: 1px solid #30363d;
            border-radius: 10px;
            padding: 10px 24px;
            font-weight: 600;
            font-size: 10pt;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #30363d,
                stop:1 #21262d);
            border: 1px solid #388bfd;
        }
        QPushButton[btnRole="primary"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1f6feb,
                stop:0.5 #388bfd,
                stop:1 #58a6ff);
            color: #ffffff;
            border: none;
            font-weight: 700;
        }
        QPushButton[btnRole="primary"]:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #388bfd,
                stop:0.5 #58a6ff,
                stop:1 #79c0ff);
        }
        QPushButton[btnRole="danger"] {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #da3633,
                stop:0.5 #f85149,
                stop:1 #ff7b72);
            color: #ffffff;
            border: none;
            font-weight: 700;
        }
        QPushButton[btnRole="danger"]:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #f85149,
                stop:0.5 #ff7b72,
                stop:1 #ffa198);
        }
        QCalendarWidget QWidget {
            color: #e6edf3;
            background: #0d1117;
        }
        QCalendarWidget QAbstractItemView {
            background: #161b22;
            selection-background-color: #1f6feb;
            border: 1px solid #30363d;
        }
        QCalendarWidget QToolButton {
            background: transparent;
            color: #e6edf3;
            border: none;
            border-radius: 6px;
        }
        QCalendarWidget QToolButton:hover {
            background: #21262d;
        }
    )");
}

inline bool validateTransactionInputs(QWidget* parent,
                                      QLineEdit* nameEdit,
                                      QComboBox* categoryCombo,
                                      QLineEdit* additionalInfoEdit,
                                      bool isIncome) {
    if (!nameEdit || nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(parent,
                             QString::fromUtf8("Ошибка"),
                             QString::fromUtf8("Введите название транзакции"));
        return false;
    }

    if (!categoryCombo || categoryCombo->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(parent,
                             QString::fromUtf8("Ошибка"),
                             QString::fromUtf8("Введите категорию"));
        return false;
    }

    if (!additionalInfoEdit || additionalInfoEdit->text().trimmed().isEmpty()) {
        QString field = isIncome
            ? QString::fromUtf8("источник дохода")
            : QString::fromUtf8("место расхода");
        QMessageBox::warning(parent,
                             QString::fromUtf8("Ошибка"),
                             QString::fromUtf8("Введите ") + field);
        return false;
    }

    return true;
}
