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
#include "StatsDialog.h"
#include <memory>
#include "TransactionList.h"
#include <QAction>

class MainWindow : public QMainWindow {
    Q_OBJECT

private:

    TransactionList transactionList;
    

    QWidget* centralWidget;
    QTableWidget* transactionTable;
    QPushButton* addIncomeButton;
    QPushButton* addExpenseButton;
    // bottom delete button removed; deletion handled per-row
    QLabel* balanceLabel;
    

    // Right-side search panel widgets
    QWidget* searchPanel;
    QLineEdit* nameSearchEdit;
    QComboBox* categoryCombo;
    QTreeView* categoryView;
    QStandardItemModel* categoryModel;
    QDateEdit* dateFromEdit;
    QDateEdit* dateToEdit;
    QPushButton* resetFiltersButton;
    QPushButton* analyticsButton;
    
    void setupUI();
    void createMenuBar();
    void connectSignals();
    void updateTable();
    void updateBalance();
    void buildCategoryModel();
    void applyFiltersAndUpdateTable();
    void updateBalanceFor(const std::list<std::shared_ptr<Transaction>>& list);
    void deleteTransactionById(size_t id);
    void deleteSelectedRow();
    void applyTheme(bool dark);
    
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

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private:
    bool dateSortAscending = true;
    bool amountSortAscending = true;
};
