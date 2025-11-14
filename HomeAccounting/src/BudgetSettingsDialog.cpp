#include "../include/BudgetSettingsDialog.h"
#include <QMessageBox>

BudgetSettingsDialog::BudgetSettingsDialog(BudgetSettings* settings, QWidget* parent)
    : QDialog(parent), budgetSettings(settings) {
    
    setWindowTitle(QString::fromUtf8("Настройка бюджета"));
    setMinimumWidth(400);
    resize(450, 250);
    
    setupUI();
    updateLimitLabel();
    

    salarySpinBox->setValue(budgetSettings->getMonthlySalary());
    savingsSpinBox->setValue(budgetSettings->getMonthlySavings());
    thresholdSpinBox->setValue(budgetSettings->getWarningThreshold());
}

void BudgetSettingsDialog::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QFormLayout* formLayout = new QFormLayout();
    

    salarySpinBox = new QDoubleSpinBox(this);
    salarySpinBox->setRange(0.0, 10000000.0);
    salarySpinBox->setDecimals(2);
    salarySpinBox->setSuffix(QString::fromUtf8(" руб."));
    salarySpinBox->setSingleStep(1000.0);
    formLayout->addRow(QString::fromUtf8("Ожидаемая зарплата в месяц:"), salarySpinBox);
    

    savingsSpinBox = new QDoubleSpinBox(this);
    savingsSpinBox->setRange(0.0, 10000000.0);
    savingsSpinBox->setDecimals(2);
    savingsSpinBox->setSuffix(QString::fromUtf8(" руб."));
    savingsSpinBox->setSingleStep(1000.0);
    formLayout->addRow(QString::fromUtf8("Сумма для откладывания:"), savingsSpinBox);
    

    thresholdSpinBox = new QDoubleSpinBox(this);
    thresholdSpinBox->setRange(50.0, 100.0);
    thresholdSpinBox->setDecimals(0);
    thresholdSpinBox->setSuffix(QString::fromUtf8("%"));
    thresholdSpinBox->setSingleStep(5.0);
    formLayout->addRow(QString::fromUtf8("Порог предупреждения:"), thresholdSpinBox);
    
    mainLayout->addLayout(formLayout);
    

    limitLabel = new QLabel(this);
    limitLabel->setTextFormat(Qt::RichText);
    limitLabel->setStyleSheet("QLabel { font-weight: bold; padding: 8px; background: #f0f0f0; border-radius: 4px; }");
    updateLimitLabel();
    mainLayout->addWidget(limitLabel);
    
    mainLayout->addStretch();
    

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    okButton = new QPushButton(QString::fromUtf8("ОК"), this);
    okButton->setProperty("btnRole", "primary");
    cancelButton = new QPushButton(QString::fromUtf8("Отмена"), this);
    
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    

    connect(salarySpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BudgetSettingsDialog::onValuesChanged);
    connect(savingsSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BudgetSettingsDialog::onValuesChanged);
    connect(thresholdSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &BudgetSettingsDialog::onValuesChanged);
    
    connect(okButton, &QPushButton::clicked, this, [this]() {

        if (savingsSpinBox->value() > salarySpinBox->value()) {
            QMessageBox::warning(this, QString::fromUtf8("Ошибка"),
                               QString::fromUtf8("Сумма для откладывания не может быть больше зарплаты!"));
            return;
        }
        

        budgetSettings->setMonthlySalary(salarySpinBox->value());
        budgetSettings->setMonthlySavings(savingsSpinBox->value());
        budgetSettings->setWarningThreshold(thresholdSpinBox->value());
        
        accept();
    });
    
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void BudgetSettingsDialog::onValuesChanged() {
    updateLimitLabel();
}

void BudgetSettingsDialog::updateLimitLabel() {
    double salary = salarySpinBox->value();
    double savings = savingsSpinBox->value();
    double limit = salary - savings;
    
    if (limit < 0) {
        limitLabel->setText(QString::fromUtf8("⚠ Доступный лимит: <span style='color: red;'>некорректные значения</span>"));
    } else if (limit == 0) {
        limitLabel->setText(QString::fromUtf8("ℹ Доступный лимит: 0 руб. (вся зарплата откладывается)"));
    } else {
        QString limitText = QString::fromUtf8("ℹ Доступный лимит на месяц: <b>%1 руб.</b>").arg(limit, 0, 'f', 2);
        double threshold = thresholdSpinBox->value();
        double warningAmount = limit * (threshold / 100.0);
        limitText += QString::fromUtf8("<br>Предупреждение при достижении: <b>%1 руб.</b> (%2%)")
                     .arg(warningAmount, 0, 'f', 2)
                     .arg(threshold, 0, 'f', 0);
        limitLabel->setText(limitText);
    }
}

