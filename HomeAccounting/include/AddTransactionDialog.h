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

class AddTransactionDialog : public QDialog {
    Q_OBJECT

private:
    bool isIncome;
    
    QLineEdit* nameEdit;
    QComboBox* categoryCombo;
    QDateEdit* dateEdit;
    QDoubleSpinBox* amountSpinBox;
    QLineEdit* additionalInfoEdit;
    
    QPushButton* okButton;
    QPushButton* cancelButton;
    
    void setupUI();
    bool validateInput();

public:
    explicit AddTransactionDialog(bool isIncome, QWidget *parent = nullptr);
    ~AddTransactionDialog() override = default;
    
    std::shared_ptr<Transaction> getTransaction() const;

private slots:
    void onAccept();
};
