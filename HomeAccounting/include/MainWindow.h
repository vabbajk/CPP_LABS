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
#include "TransactionList.h"
#include <QAction>
#include "BudgetSettings.h"
#include "BudgetSettingsDialog.h"

class FiltersPanel;

class MainWindow : public QMainWindow {
    Q_OBJECT

private:

    TransactionList transactionList;
    

    QWidget* centralWidget;
    QTableWidget* transactionTable;
    QPushButton* addIncomeButton;
    QPushButton* addExpenseButton;

    QLabel* balanceLabel;
    QLabel* budgetInfoLabel;
    QLabel* budgetWarningLabel;
    QProgressBar* savingsProgressBar;
    QLabel* savingsStatusLabel;
    QLabel* savingsForecastLabel;
    QLabel* totalSavingsLabel;
    
    BudgetSettings* budgetSettings;

    FiltersPanel* filtersPanel;
    
    void setupUI();
    void createMenuBar();
    void connectSignals();
    void updateTable();
    void updateBalance();
    void applyFiltersAndUpdateTable();
    void updateBalanceFor(const std::list<std::shared_ptr<Transaction>>& list);
    void deleteTransactionById(size_t id);
    void deleteSelectedRow();
    void editTransactionById(size_t id);
    void applyTheme(bool dark);
    void checkBudgetLimit();
    void updateBudgetInfo();
    void updateBudgetWarning();
    void updateSavingsRadar();
    void updateSavingsCounter();
    void addTransaction(bool isIncome);
    
private slots:
    void onAddIncome();
    void onAddExpense();
    void onEditTransaction();
    void onHeaderClicked(int logicalIndex);
    void onApplyFilter();
    void onClearFilter();
    void onAbout();
    void onTableDoubleClicked(int row, int column);
    void onExportTxt();
    void onTableContextMenu(const QPoint& pos);
    void onBudgetSettings();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    bool dateSortAscending = true;
    bool amountSortAscending = true;
    int currentSortColumn = -1;
};
