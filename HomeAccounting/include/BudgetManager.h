#pragma once

#include <QLabel>
#include <QProgressBar>
#include "BudgetSettings.h"
#include "TransactionList.h"

class BudgetManager : public QObject {
    Q_OBJECT

public:
    explicit BudgetManager(BudgetSettings* settings, TransactionList* transactionList, QObject* parent = nullptr);
    
    void setLabels(QLabel* infoLabel, QLabel* warningLabel);
    void updateBudgetInfo();
    void checkBudgetLimit();
    
private:
    BudgetSettings* budgetSettings_;
    TransactionList* transactionList_;
    QLabel* budgetInfoLabel_;
    QLabel* budgetWarningLabel_;
    
    void updateBudgetWarning();
};

