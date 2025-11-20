#include "../include/BudgetSettings.h"

const QString BudgetSettings::ORGANIZATION_NAME = "HomeAccounting";
const QString BudgetSettings::APPLICATION_NAME = "HomeAccounting";
const QString BudgetSettings::SALARY_KEY = "budget/monthlySalary";
const QString BudgetSettings::SAVINGS_KEY = "budget/monthlySavings";
const QString BudgetSettings::WARNING_THRESHOLD_KEY = "budget/warningThreshold";

BudgetSettings::BudgetSettings() {
    settings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME);
}

BudgetSettings::~BudgetSettings() {
    delete settings;
}

BudgetSettings::BudgetSettings(const BudgetSettings& other) {
    (void)other;
    settings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME);
}

BudgetSettings& BudgetSettings::operator=(const BudgetSettings& other) {
    if (this != &other) {
        (void)other;
        delete settings;
        settings = new QSettings(ORGANIZATION_NAME, APPLICATION_NAME);
    }
    return *this;
}

BudgetSettings::BudgetSettings(BudgetSettings&& other) noexcept {
    settings = other.settings;
    other.settings = nullptr;
}

BudgetSettings& BudgetSettings::operator=(BudgetSettings&& other) noexcept {
    if (this != &other) {
        delete settings;
        settings = other.settings;
        other.settings = nullptr;
    }
    return *this;
}

double BudgetSettings::getMonthlySalary() const {
    return settings->value(SALARY_KEY, 0.0).toDouble();
}

void BudgetSettings::setMonthlySalary(double salary) {
    settings->setValue(SALARY_KEY, salary);
    settings->sync();
}

double BudgetSettings::getMonthlySavings() const {
    return settings->value(SAVINGS_KEY, 0.0).toDouble();
}

void BudgetSettings::setMonthlySavings(double savings) {
    settings->setValue(SAVINGS_KEY, savings);
    settings->sync();
}

double BudgetSettings::getMonthlyLimit() const {
    double salary = getMonthlySalary();
    double savings = getMonthlySavings();
    return salary - savings;
}

double BudgetSettings::getWarningThreshold() const {

    return settings->value(WARNING_THRESHOLD_KEY, 80.0).toDouble();
}

void BudgetSettings::setWarningThreshold(double threshold) {
    settings->setValue(WARNING_THRESHOLD_KEY, threshold);
    settings->sync();
}

bool BudgetSettings::shouldShowWarning(double currentExpenses) const {
    double limit = getMonthlyLimit();
    if (limit <= 0) return false;
    
    double threshold = getWarningThreshold() / 100.0;
    double warningLevel = limit * threshold;
    
    return currentExpenses >= warningLevel;
}


