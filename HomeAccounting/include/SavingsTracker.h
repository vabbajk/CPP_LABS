#pragma once

#include <QLabel>
#include <QProgressBar>
#include "BudgetSettings.h"
#include "TransactionList.h"

class SavingsTracker : public QObject {
    Q_OBJECT

public:
    explicit SavingsTracker(BudgetSettings* settings, TransactionList* transactionList, QObject* parent = nullptr);
    
    void setWidgets(QProgressBar* progressBar, QLabel* statusLabel, QLabel* forecastLabel, QLabel* counterLabel);
    void updateSavingsRadar();
    void updateSavingsCounter();
    
private:
    BudgetSettings* budgetSettings_;
    TransactionList* transactionList_;
    QProgressBar* savingsProgressBar_;
    QLabel* savingsStatusLabel_;
    QLabel* savingsForecastLabel_;
    QLabel* totalSavingsLabel_;
};

