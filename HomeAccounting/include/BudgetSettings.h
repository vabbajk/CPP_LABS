#pragma once

#include <QSettings>
#include <QString>

class BudgetSettings {
private:
    QSettings* settings;
    static const QString ORGANIZATION_NAME;
    static const QString APPLICATION_NAME;
    static const QString SALARY_KEY;
    static const QString SAVINGS_KEY;
    static const QString WARNING_THRESHOLD_KEY;

public:
    BudgetSettings();
    ~BudgetSettings();


    double getMonthlySalary() const;
    void setMonthlySalary(double salary);

    double getMonthlySavings() const;
    void setMonthlySavings(double savings);


    double getMonthlyLimit() const;


    double getWarningThreshold() const;
    void setWarningThreshold(double threshold);


    bool shouldShowWarning(double currentExpenses) const;
};


