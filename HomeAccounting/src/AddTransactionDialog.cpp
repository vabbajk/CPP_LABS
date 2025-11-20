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
    QVBoxLayout* mainLayout = nullptr;
    QPushButton* deleteButton = nullptr;

    setupTransactionDialogUI(
        this,
        isIncome,
        /* isNewTransaction */ true,
        QString::fromUtf8("✓ Добавить"),
        /* withDeleteButton */ false,
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
    return createTransactionFromInputs(
        isIncome,
        nameEdit,
        categoryCombo,
        dateEdit,
        amountSpinBox,
        additionalInfoEdit,
        /* preserveId */ false,
        /* existingId */ 0
    );
}
