#pragma once

#include <QTableWidget>
#include <QWidget>
#include <QPushButton>
#include <memory>
#include <vector>
#include "Transaction.h"

class TransactionList;

class TransactionTableManager : public QObject {
    Q_OBJECT

public:
    explicit TransactionTableManager(QTableWidget* table, QWidget* parent = nullptr);
    
    void rebuildTable(const std::vector<std::shared_ptr<Transaction>>& transactions);
    void setupTable();
    void handleHeaderClick(int logicalIndex, TransactionList& transactionList, int& currentSortColumn, bool& dateSortAscending, bool& amountSortAscending);
    
signals:
    void editRequested(size_t transactionId);
    void deleteRequested(size_t transactionId);
    
private slots:
    void onEditButtonClicked();
    void onTableDoubleClicked(int row, int column);
    void onTableContextMenu(const QPoint& pos);
    
private:
    QTableWidget* table_;
    QWidget* parent_;
    
    void setupTableHeaders();
    void setupTableStyle();
    void createTableRow(int row, const std::shared_ptr<Transaction>& transaction);
    QPushButton* createEditButton(size_t transactionId);
};

