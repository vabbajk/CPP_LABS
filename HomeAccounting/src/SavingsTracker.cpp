#include "../include/SavingsTracker.h"
#include <QDate>
#include <cmath>

SavingsTracker::SavingsTracker(BudgetSettings* settings, TransactionList* transactionList, QObject* parent)
    : QObject(parent), budgetSettings_(settings), transactionList_(transactionList) {
}

void SavingsTracker::setWidgets(QProgressBar* progressBar, QLabel* statusLabel, 
                                 QLabel* forecastLabel, QLabel* counterLabel) {
    savingsProgressBar_ = progressBar;
    savingsStatusLabel_ = statusLabel;
    savingsForecastLabel_ = forecastLabel;
    totalSavingsLabel_ = counterLabel;
}

void SavingsTracker::updateSavingsRadar() {
    if (!savingsProgressBar_ || !savingsStatusLabel_ || !savingsForecastLabel_) return;
    
    double plannedSavings = budgetSettings_->getMonthlySavings();
    double currentIncome = transactionList_->getCurrentMonthIncome();
    double currentExpenses = transactionList_->getCurrentMonthExpenses();
    double actualSavings = currentIncome - currentExpenses;

    if (plannedSavings <= 0.0) {
        savingsProgressBar_->setValue(0);
        savingsStatusLabel_->setText(QString::fromUtf8("<span style='color:#666;'>Задайте цель накоплений в настройках бюджета.</span>"));
        savingsForecastLabel_->setText(QString::fromUtf8("Переход: Настройки → Настройка бюджета..."));
        return;
    }

    double progress = (plannedSavings > 0.0) ? (actualSavings / plannedSavings) * 100.0 : 0.0;
    auto progressValue = static_cast<int>(std::round(std::clamp(progress, 0.0, 200.0)));
    savingsProgressBar_->setValue(std::min(progressValue, 100));

    QString color = "#4caf50";
    QString statusText;

    if (actualSavings >= plannedSavings) {
        color = "#2ecc71";
        statusText = QString::fromUtf8("🎉 <span style='color:%1;'>Цель достигнута!</span> Накоплено <b>%2 руб.</b> из %3 руб.")
            .arg(color)
            .arg(actualSavings, 0, 'f', 2)
            .arg(plannedSavings, 0, 'f', 2);
    } else if (progress >= 90.0) {
        color = "#f57c00";
        statusText = QString::fromUtf8("🔥 <span style='color:%1;'>Почти у цели!</span> Накоплено <b>%2 руб.</b> из %3 руб.")
            .arg(color)
            .arg(actualSavings, 0, 'f', 2)
            .arg(plannedSavings, 0, 'f', 2);
    } else if (progress >= 60.0) {
        color = "#7986cb";
        statusText = QString::fromUtf8("📈 <span style='color:%1;'>Хороший прогресс.</span> Накоплено <b>%2 руб.</b> из %3 руб.")
            .arg(color)
            .arg(actualSavings, 0, 'f', 2)
            .arg(plannedSavings, 0, 'f', 2);
    } else if (actualSavings > 0.0) {
        color = "#7986cb";
        statusText = QString::fromUtf8("🌱 <span style='color:%1;'>Начало положено.</span> Накоплено <b>%2 руб.</b> из %3 руб.")
            .arg(color)
            .arg(actualSavings, 0, 'f', 2)
            .arg(plannedSavings, 0, 'f', 2);
    } else {
        color = "#ef5350";
        statusText = QString::fromUtf8("❗ <span style='color:%1;'>Накоплений пока нет.</span> Цель: %2 руб.")
            .arg(color)
            .arg(plannedSavings, 0, 'f', 2);
    }

    savingsStatusLabel_->setText(statusText);

    QDate today = QDate::currentDate();
    int dayOfMonth = today.day();
    int daysInMonth = today.daysInMonth();
    double dailyAverage = (dayOfMonth > 0) ? (actualSavings / dayOfMonth) : 0.0;
    double projectedSavings = dailyAverage * daysInMonth;
    double projectedDifference = projectedSavings - plannedSavings;

    QString forecastText;
    if (dailyAverage <= 0.0) {
        forecastText = QString::fromUtf8("Совет: пока расход больше дохода...");
    } else if (projectedSavings >= plannedSavings) {
        forecastText = QString::fromUtf8("Прогноз: ~<b>%1 руб.</b> к концу месяца. Цель будет выполнена на %2 руб. больше!")
            .arg(projectedSavings, 0, 'f', 2)
            .arg(projectedDifference, 0, 'f', 2);
    } else {
        forecastText = QString::fromUtf8("Прогноз: ~<b>%1 руб.</b> к концу месяца. Не хватает %2 руб. — ускорьтесь!")
            .arg(projectedSavings, 0, 'f', 2)
            .arg(plannedSavings - projectedSavings, 0, 'f', 2);
    }

    savingsForecastLabel_->setText(forecastText);
}

void SavingsTracker::updateSavingsCounter() {
    if (!totalSavingsLabel_) return;
    
    double currentMonthSavings = transactionList_->getCurrentMonthNetSavings();
    double totalSavings = transactionList_->getTotalSavings();
    
    QString monthColor = currentMonthSavings >= 0 ? "#2ecc71" : "#ff6b6b";
    QString totalColor = totalSavings >= 0 ? "#4caf50" : "#e74c3c";
    
    QString counterText = QString::fromUtf8(
        "📅 <b>За этот месяц:</b> <span style='color:%1; font-size:13pt;'>%2 руб.</span><br>"
        "💎 <b>Всего накоплено:</b> <span style='color:%3; font-size:13pt;'>%4 руб.</span>"
    )
    .arg(monthColor)
    .arg(currentMonthSavings, 0, 'f', 2)
    .arg(totalColor)
    .arg(totalSavings, 0, 'f', 2);
    
    totalSavingsLabel_->setText(counterText);
}

