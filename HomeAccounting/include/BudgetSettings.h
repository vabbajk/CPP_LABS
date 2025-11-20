#pragma once

#include <QSettings>
#include <QString>

class BudgetSettings {
private:
    QSettings* settings = nullptr;
    static const QString ORGANIZATION_NAME;
    static const QString APPLICATION_NAME;
    static const QString SALARY_KEY;
    static const QString SAVINGS_KEY;
    static const QString WARNING_THRESHOLD_KEY;

public:
    BudgetSettings();
    ~BudgetSettings();

    // Копирование создаёт собственный QSettings с теми же параметрами
    BudgetSettings(const BudgetSettings& other);
    BudgetSettings& operator=(const BudgetSettings& other);

    // Перемещение передаёт владение указателем settings
    BudgetSettings(BudgetSettings&& other) noexcept;
    BudgetSettings& operator=(BudgetSettings&& other) noexcept;


    double getMonthlySalary() const;
    void setMonthlySalary(double salary);

    double getMonthlySavings() const;
    void setMonthlySavings(double savings);


    double getMonthlyLimit() const;


    double getWarningThreshold() const;
    void setWarningThreshold(double threshold);


    bool shouldShowWarning(double currentExpenses) const;
};


