#include "../include/BudgetManager.h"

BudgetManager::BudgetManager(BudgetSettings* settings, TransactionList* transactionList, QObject* parent)
    : QObject(parent), budgetSettings_(settings), transactionList_(transactionList) {
}

void BudgetManager::setLabels(QLabel* infoLabel, QLabel* warningLabel) {
    budgetInfoLabel_ = infoLabel;
    budgetWarningLabel_ = warningLabel;
}

void BudgetManager::updateBudgetInfo() {
    if (!budgetInfoLabel_) return;
    
    double monthlyLimit = budgetSettings_->getMonthlyLimit();
    double currentExpenses = transactionList_->getCurrentMonthExpenses();
    
    if (monthlyLimit <= 0) {
        budgetInfoLabel_->setText(QString::fromUtf8("<span style='color: #666;'>Бюджет не настроен</span>"));
        budgetInfoLabel_->setVisible(true);
        return;
    }
    
    double remaining = monthlyLimit - currentExpenses;
    double percentage = (currentExpenses / monthlyLimit) * 100.0;
    
    QString infoText;
    QString color;
    
    if (currentExpenses >= monthlyLimit) {
        color = "#d32f2f";
        infoText = QString::fromUtf8("Бюджет: <b>%1</b> / %2 руб. (<span style='color: %3;'>%4%</span>) | Осталось: <span style='color: %3;'>%5 руб.</span>")
                   .arg(currentExpenses, 0, 'f', 2)
                   .arg(monthlyLimit, 0, 'f', 2)
                   .arg(color)
                   .arg(percentage, 0, 'f', 1)
                   .arg(remaining, 0, 'f', 2);
    } else if (percentage >= 90) {
        color = "#f57c00";
        infoText = QString::fromUtf8("Бюджет: <b>%1</b> / %2 руб. (<span style='color: %3;'>%4%</span>) | Осталось: %5 руб.")
                   .arg(currentExpenses, 0, 'f', 2)
                   .arg(monthlyLimit, 0, 'f', 2)
                   .arg(color)
                   .arg(percentage, 0, 'f', 1)
                   .arg(remaining, 0, 'f', 2);
    } else if (percentage >= 80) {
        color = "#f9a825";
        infoText = QString::fromUtf8("Бюджет: <b>%1</b> / %2 руб. (<span style='color: %3;'>%4%</span>) | Осталось: %5 руб.")
                   .arg(currentExpenses, 0, 'f', 2)
                   .arg(monthlyLimit, 0, 'f', 2)
                   .arg(color)
                   .arg(percentage, 0, 'f', 1)
                   .arg(remaining, 0, 'f', 2);
    } else {
        color = "#4caf50";
        infoText = QString::fromUtf8("Бюджет: <b>%1</b> / %2 руб. (<span style='color: %3;'>%4%</span>) | Осталось: %5 руб.")
                   .arg(currentExpenses, 0, 'f', 2)
                   .arg(monthlyLimit, 0, 'f', 2)
                   .arg(color)
                   .arg(percentage, 0, 'f', 1)
                   .arg(remaining, 0, 'f', 2);
    }
    
    budgetInfoLabel_->setText(infoText);
    budgetInfoLabel_->setVisible(true);
}

void BudgetManager::checkBudgetLimit() {
    updateBudgetInfo();
    
    if (!budgetWarningLabel_) return;
    
    double monthlyLimit = budgetSettings_->getMonthlyLimit();
    if (monthlyLimit <= 0) {
        budgetWarningLabel_->setVisible(false);
        return;
    }
    
    double currentExpenses = transactionList_->getCurrentMonthExpenses();
    if (budgetSettings_->shouldShowWarning(currentExpenses) || currentExpenses >= monthlyLimit) {
        updateBudgetWarning();
    } else {
        budgetWarningLabel_->setVisible(false);
    }
}

void BudgetManager::updateBudgetWarning() {
    if (!budgetWarningLabel_) return;
    
    double monthlyLimit = budgetSettings_->getMonthlyLimit();
    double currentExpenses = transactionList_->getCurrentMonthExpenses();
    double remaining = monthlyLimit - currentExpenses;
    double percentage = (currentExpenses / monthlyLimit) * 100.0;
    
    QString warningText;
    QString color;
    
    if (currentExpenses >= monthlyLimit) {
        warningText = QString::fromUtf8("⚠ Лимит превышен! Потрачено: %1 руб. из %2 руб. (%3%)")
                      .arg(currentExpenses, 0, 'f', 2)
                      .arg(monthlyLimit, 0, 'f', 2)
                      .arg(percentage, 0, 'f', 1);
        color = "#d32f2f";
    } else if (percentage >= 90) {
        warningText = QString::fromUtf8("⚠ Почти превышен лимит! Потрачено: %1 руб. из %2 руб. (%3%). Осталось: %4 руб.")
                      .arg(currentExpenses, 0, 'f', 2)
                      .arg(monthlyLimit, 0, 'f', 2)
                      .arg(percentage, 0, 'f', 1)
                      .arg(remaining, 0, 'f', 2);
        color = "#f57c00";
    } else {
        warningText = QString::fromUtf8("⚠ Приближение к лимиту! Потрачено: %1 руб. из %2 руб. (%3%). Осталось: %4 руб.")
                      .arg(currentExpenses, 0, 'f', 2)
                      .arg(monthlyLimit, 0, 'f', 2)
                      .arg(percentage, 0, 'f', 1)
                      .arg(remaining, 0, 'f', 2);
        color = "#f9a825";
    }
    
    budgetWarningLabel_->setText(QString("<span style='color: %1; font-weight: bold;'>%2</span>").arg(color, warningText));
    budgetWarningLabel_->setVisible(true);
}

