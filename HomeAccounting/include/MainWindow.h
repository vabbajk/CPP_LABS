#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QMessageBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <QProgressBar>
#include "StatsDialog.h"
#include <memory>
#include <vector>
#include "TransactionList.h"
#include <QAction>
#include "BudgetSettings.h"
#include "BudgetSettingsDialog.h"
#include "TransactionTableManager.h"
#include "BudgetManager.h"
#include "SavingsTracker.h"

class FiltersPanel;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    TransactionList transactionList{"homeaccounting.dat"};
    
    QWidget* centralWidget;
    QTableWidget* transactionTable;
    QPushButton* addIncomeButton;
    QPushButton* addExpenseButton;
    QLabel* balanceLabel;
    
    BudgetSettings* budgetSettings = new BudgetSettings();
    FiltersPanel* filtersPanel;
    
    // Manager classes
    TransactionTableManager* tableManager;
    BudgetManager* budgetManager;
    SavingsTracker* savingsTracker;
    
    void setupUI();
    void createMenuBar();
    void connectSignals();
    void applyFiltersAndUpdateTable();
    void updateBalanceFor(const std::vector<std::shared_ptr<Transaction>>& list);
    void deleteTransactionById(size_t id);
    void applyTheme(bool dark) const;
    void addTransaction(bool isIncome);
    
private slots:
    void onAddIncome();
    void onAddExpense();
    void onEditTransaction();
    void onHeaderClicked(int logicalIndex);
    void onAbout();
    void onExportTxt();
    void onBudgetSettings();
    void onEditRequested(size_t transactionId);
    void onDeleteRequested(size_t transactionId);

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    bool dateSortAscending = true;
    bool amountSortAscending = true;
    int currentSortColumn = -1;
};
