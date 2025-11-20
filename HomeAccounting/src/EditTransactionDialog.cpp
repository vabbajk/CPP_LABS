#include "../include/EditTransactionDialog.h"
#include "../include/TransactionDialogCommon.h"
#include <QMessageBox>
#include <QDate>

EditTransactionDialog::EditTransactionDialog(std::shared_ptr<Transaction> trans, QWidget *parent)
    : QDialog(parent), transaction(trans) {
    
    isIncome = (transaction->getType() == 1);
    
    if (isIncome) {
        setWindowTitle(QString::fromUtf8("Редактировать доход"));
    } else {
        setWindowTitle(QString::fromUtf8("Редактировать расход"));
    }
    
    setMinimumWidth(400);
    setupUI();
    loadTransactionData();
}

void EditTransactionDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    
    nameEdit = new QLineEdit(this);
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
    formLayout->addRow(QString::fromUtf8("Сумма:"), amountSpinBox);
    
    additionalInfoEdit = new QLineEdit(this);
    if (isIncome) {
        formLayout->addRow(QString::fromUtf8("Источник дохода:"), additionalInfoEdit);
    } else {
        formLayout->addRow(QString::fromUtf8("Место расхода:"), additionalInfoEdit);
    }
    
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(8);
    

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    
    okButton = new QPushButton(QString::fromUtf8("✓ Сохранить"), this);
    okButton->setProperty("btnRole", "primary");
    okButton->setMinimumHeight(40);
    okButton->setCursor(Qt::PointingHandCursor);
    
    deleteButton = new QPushButton(QString::fromUtf8("🗑 Удалить"), this);
    deleteButton->setProperty("btnRole", "danger");
    deleteButton->setMinimumHeight(40);
    deleteButton->setCursor(Qt::PointingHandCursor);
    
    cancelButton = new QPushButton(QString::fromUtf8("✕ Отмена"), this);
    cancelButton->setMinimumHeight(40);
    cancelButton->setCursor(Qt::PointingHandCursor);
    
    okButton->setDefault(true);
    
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);
    
    mainLayout->addLayout(buttonLayout);

    setStyleSheet(getCommonTransactionDialogStyleSheet());

    connect(okButton, &QPushButton::clicked, this, &EditTransactionDialog::onAccept);
    connect(deleteButton, &QPushButton::clicked, this, &EditTransactionDialog::onDeleteClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void EditTransactionDialog::loadTransactionData() {
    nameEdit->setText(QString::fromUtf8(transaction->getName().c_str()));
    {
        QString current = QString::fromUtf8(transaction->getCategory().c_str());
        int idx = categoryCombo->findText(current);
        if (idx >= 0) categoryCombo->setCurrentIndex(idx);
        else {

            categoryCombo->addItem(current);
            categoryCombo->setCurrentText(current);
        }
    }
    
    Date date = transaction->getDate();
    dateEdit->setDate(QDate(date.getYear(), date.getMonth(), date.getDay()));
    
    amountSpinBox->setValue(transaction->getAmount());
    
    if (isIncome) {
        auto income = std::dynamic_pointer_cast<IncomeTransaction>(transaction);
        if (income) {
            additionalInfoEdit->setText(QString::fromUtf8(income->getIncomeSource().c_str()));
        }
    } else {
        auto expense = std::dynamic_pointer_cast<Expense>(transaction);
        if (expense) {
            additionalInfoEdit->setText(QString::fromUtf8(expense->getWhere().c_str()));
        }
    }
}

bool EditTransactionDialog::validateInput() {
    return validateTransactionInputs(this, nameEdit, categoryCombo, additionalInfoEdit, isIncome);
}

void EditTransactionDialog::onAccept() {
    if (validateInput()) {
        accept();
    }
}

void EditTransactionDialog::onDeleteClicked() {
    deleteRequested = true;
    accept();
}

std::shared_ptr<Transaction> EditTransactionDialog::getUpdatedTransaction() const {
    std::string name = nameEdit->text().toUtf8().constData();
    std::string category = categoryCombo->currentText().toUtf8().constData();
    
    QDate qdate = dateEdit->date();
    Date date(qdate.day(), qdate.month(), qdate.year());
    
    double amount = amountSpinBox->value();
    std::string additionalInfo = additionalInfoEdit->text().toUtf8().constData();
    
    size_t existingId = transaction ? transaction->getID() : 0;
    if (isIncome) {
        return std::make_shared<IncomeTransaction>(existingId, name, category, date, amount, additionalInfo);
    } else {
        return std::make_shared<Expense>(existingId, name, category, date, amount, additionalInfo);
    }
}
