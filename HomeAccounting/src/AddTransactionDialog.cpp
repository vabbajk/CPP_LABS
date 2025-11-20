#include "../include/AddTransactionDialog.h"
#include "../include/TransactionDialogCommon.h"
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
    categoryCombo->addItems(isIncome ? getIncomeCategories() : getExpenseCategories());
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

    setStyleSheet(getCommonTransactionDialogStyleSheet());

    connect(okButton, &QPushButton::clicked, this, &AddTransactionDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

bool AddTransactionDialog::validateInput() {
    return validateTransactionInputs(this, nameEdit, categoryCombo, additionalInfoEdit, isIncome);
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
