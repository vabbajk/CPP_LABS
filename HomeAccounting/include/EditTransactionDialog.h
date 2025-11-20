#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <memory>
#include "Transaction.h"

class EditTransactionDialog : public QDialog {
    Q_OBJECT

private:
    std::shared_ptr<Transaction> transaction;
    bool isIncome;
    
    QLineEdit* nameEdit;
    QComboBox* categoryCombo;
    QDateEdit* dateEdit;
    QDoubleSpinBox* amountSpinBox;
    QLineEdit* additionalInfoEdit;
    
    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* deleteButton;
    
    void setupUI();
    void loadTransactionData();
    bool validateInput();
    bool deleteRequested = false;

public:
    explicit EditTransactionDialog(std::shared_ptr<Transaction> trans, QWidget *parent = nullptr);
    ~EditTransactionDialog() override = default;
    
    std::shared_ptr<Transaction> getUpdatedTransaction(size_t originalId) const;
    bool isDeleteRequested() const { return deleteRequested; }

private slots:
    void onAccept();
    void onDeleteClicked();
};
