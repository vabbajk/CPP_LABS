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
    QVBoxLayout* mainLayout = nullptr;

    setupTransactionDialogUI(
        this,
        isIncome,
        /* isNewTransaction */ false,
        QString::fromUtf8("✓ Сохранить"),
        /* withDeleteButton */ true,
        mainLayout,
        nameEdit,
        categoryCombo,
        dateEdit,
        amountSpinBox,
        additionalInfoEdit,
        okButton,
        cancelButton,
        deleteButton
    );

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
    size_t existingId = transaction ? transaction->getID() : 0;
    return createTransactionFromInputs(
        isIncome,
        nameEdit,
        categoryCombo,
        dateEdit,
        amountSpinBox,
        additionalInfoEdit,
        /* preserveId */ true,
        existingId
    );
}
