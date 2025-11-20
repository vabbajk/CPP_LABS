#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include "BudgetSettings.h"

class BudgetSettingsDialog : public QDialog {
    Q_OBJECT

private:
    BudgetSettings* budgetSettings;
    
    QDoubleSpinBox* salarySpinBox;
    QDoubleSpinBox* savingsSpinBox;
    QDoubleSpinBox* thresholdSpinBox;
    QLabel* limitLabel;
    QPushButton* okButton;
    QPushButton* cancelButton;

    void setupUI();
    void updateLimitLabel();
    
private slots:
    void onValuesChanged();

public:
    explicit BudgetSettingsDialog(BudgetSettings* settings, QWidget* parent = nullptr);
    ~BudgetSettingsDialog() override = default;
};


