#include "../include/AddTransactionDialog.h"
#include <QMessageBox>
#include <QDate>

AddTransactionDialog::AddTransactionDialog(bool isIncome, QWidget *parent)
    : QDialog(parent), isIncome(isIncome) {
    
    if (isIncome) {
        setWindowTitle(QString::fromUtf8("Добавить доход"));
    } else {
        setWindowTitle(QString::fromUtf8("Добавить расход"));
    }
    
    setMinimumWidth(400);
    setupUI();
}

void AddTransactionDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);


    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText(QString::fromUtf8("Например: Зарплата"));
    formLayout->addRow(QString::fromUtf8("Название:"), nameEdit);
    
    categoryCombo = new QComboBox(this);

    if (isIncome) {
        categoryCombo->addItems({
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
        });
    } else {
        categoryCombo->addItems({
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
        });
    }
    formLayout->addRow(QString::fromUtf8("Категория:"), categoryCombo);
    
    dateEdit = new QDateEdit(QDate::currentDate(), this);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd.MM.yyyy");
    formLayout->addRow(QString::fromUtf8("Дата:"), dateEdit);
    
    amountSpinBox = new QDoubleSpinBox(this);
    amountSpinBox->setRange(0.01, 1000000.00);
    amountSpinBox->setDecimals(2);
    amountSpinBox->setSuffix(QString::fromUtf8(" руб."));
    amountSpinBox->setValue(100.00);
    formLayout->addRow(QString::fromUtf8("Сумма:"), amountSpinBox);
    
    additionalInfoEdit = new QLineEdit(this);
    if (isIncome) {
        additionalInfoEdit->setPlaceholderText(QString::fromUtf8("Например: Основная работа"));
        formLayout->addRow(QString::fromUtf8("Источник дохода:"), additionalInfoEdit);
    } else {
        additionalInfoEdit->setPlaceholderText(QString::fromUtf8("Например: Магазин"));
        formLayout->addRow(QString::fromUtf8("Место расхода:"), additionalInfoEdit);
    }
    
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(8);
    

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    okButton = new QPushButton(QString::fromUtf8("✓ Добавить"), this);
    okButton->setProperty("btnRole", "primary");
    okButton->setMinimumHeight(40);
    okButton->setCursor(Qt::PointingHandCursor);
    
    cancelButton = new QPushButton(QString::fromUtf8("✕ Отмена"), this);
    cancelButton->setMinimumHeight(40);
    cancelButton->setCursor(Qt::PointingHandCursor);
    
    okButton->setDefault(true);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);
    
    mainLayout->addLayout(buttonLayout);
    

    setStyleSheet(R"(
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
    

    connect(okButton, &QPushButton::clicked, this, &AddTransactionDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

bool AddTransactionDialog::validateInput() {
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Ошибка"), 
                           QString::fromUtf8("Введите название транзакции"));
        return false;
    }
    
    if (categoryCombo->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(this, QString::fromUtf8("Ошибка"), 
                           QString::fromUtf8("Введите категорию"));
        return false;
    }
    
    if (additionalInfoEdit->text().trimmed().isEmpty()) {
        QString field = isIncome ? QString::fromUtf8("источник дохода") 
                                 : QString::fromUtf8("место расхода");
        QMessageBox::warning(this, QString::fromUtf8("Ошибка"), 
                           QString::fromUtf8("Введите ") + field);
        return false;
    }
    
    return true;
}

void AddTransactionDialog::onAccept() {
    if (validateInput()) {
        accept();
    }
}

std::shared_ptr<Transaction> AddTransactionDialog::getTransaction() const {
    std::string name = nameEdit->text().toUtf8().constData();
    std::string category = categoryCombo->currentText().toUtf8().constData();
    
    QDate qdate = dateEdit->date();
    Date date(qdate.day(), qdate.month(), qdate.year());
    
    double amount = amountSpinBox->value();
    std::string additionalInfo = additionalInfoEdit->text().toUtf8().constData();
    
    if (isIncome) {
        return std::make_shared<IncomeTransaction>(name, category, date, amount, additionalInfo);
    } else {
        return std::make_shared<Expense>(name, category, date, amount, additionalInfo);
    }
}
