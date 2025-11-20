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
    TransactionDialogControls controls{};

    setupTransactionDialogUI(
        this,
        isIncome,
        /* isNewTransaction */ true,
        QString::fromUtf8("✓ Добавить"),
        /* withDeleteButton */ false,
        controls
    );

    nameEdit = controls.nameEdit;
    categoryCombo = controls.categoryCombo;
    dateEdit = controls.dateEdit;
    amountSpinBox = controls.amountSpinBox;
    additionalInfoEdit = controls.additionalInfoEdit;
    okButton = controls.okButton;
    cancelButton = controls.cancelButton;

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
    TransactionDialogControls controls{};
    controls.nameEdit = nameEdit;
    controls.categoryCombo = categoryCombo;
    controls.dateEdit = dateEdit;
    controls.amountSpinBox = amountSpinBox;
    controls.additionalInfoEdit = additionalInfoEdit;

    return createTransactionFromInputs(
        isIncome,
        controls,
        /* preserveId */ false,
        /* existingId */ 0
    );
}
