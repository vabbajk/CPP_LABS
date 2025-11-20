#include "../include/TransactionTableManager.h"
#include "../include/TransactionList.h"
#include "../include/EditTransactionDialog.h"
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QMenu>
#include <QMessageBox>
#include <QVariant>
#include <QHBoxLayout>
#include <QWidget>

TransactionTableManager::TransactionTableManager(QTableWidget* table, QWidget* parent)
    : QObject(parent), table_(table), parent_(parent) {
    
    setupTable();
    
    connect(table_, &QTableWidget::cellDoubleClicked, this, &TransactionTableManager::onTableDoubleClicked);
    table_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(table_, &QWidget::customContextMenuRequested, this, &TransactionTableManager::onTableContextMenu);
}

void TransactionTableManager::setupTable() {
    setupTableHeaders();
    setupTableStyle();
}

void TransactionTableManager::setupTableHeaders() {
    table_->setColumnCount(9);
    QStringList headers;
    headers << "" << "ID" << QString::fromUtf8("Название") << QString::fromUtf8("Категория") 
            << QString::fromUtf8("Дата") << QString::fromUtf8("Сумма") 
            << QString::fromUtf8("Тип") << QString::fromUtf8("Доп. инфо") << QString::fromUtf8("Действия");
    table_->setHorizontalHeaderLabels(headers);
    
    table_->setCornerButtonEnabled(false);
    table_->verticalHeader()->setVisible(false);
    table_->horizontalHeader()->setStretchLastSection(false);
    table_->horizontalHeader()->setSortIndicatorShown(true);
    table_->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    
    table_->setColumnWidth(0, 48);
    table_->setColumnWidth(1, 60);
    table_->setColumnWidth(2, 150);
    table_->setColumnWidth(3, 130);
    table_->setColumnWidth(4, 100);
    table_->setColumnWidth(5, 90);
    table_->setColumnWidth(6, 80);
    table_->setColumnWidth(7, 120);
    table_->setColumnWidth(8, 100);
    
    table_->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_->setSelectionMode(QAbstractItemView::SingleSelection);
    table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table_->setAlternatingRowColors(true);
    table_->setShowGrid(false);
    table_->verticalHeader()->setDefaultSectionSize(36);
}

void TransactionTableManager::setupTableStyle() {
    table_->setStyleSheet(
        "QTableWidget {"
        "  background-color: #0d1117;"
        "  color: #e6edf3;"
        "  gridline-color: #30363d;"
        "  border: 1px solid #21262d;"
        "  border-radius: 8px;"
        "}"
        "QHeaderView::section {"
        "  background-color: #161b22;"
        "  color: #c9d1d9;"
        "  font-weight: 600;"
        "  border: none;"
        "  padding: 6px;"
        "}"
        "QTableCornerButton::section { background: #161b22; border: none; }"
    );
    table_->verticalHeader()->setDefaultSectionSize(52);
    table_->horizontalHeader()->setStretchLastSection(true);
}

void TransactionTableManager::rebuildTable(const std::vector<std::shared_ptr<Transaction>>& transactions) {
    table_->setRowCount(0);
    int row = 0;
    for (const auto& transaction : transactions) {
        table_->insertRow(row);
        createTableRow(row, transaction);
        row++;
    }
}

void TransactionTableManager::createTableRow(int row, const std::shared_ptr<Transaction>& transaction) {
    // Icon column
    auto* iconItem = new QTableWidgetItem();
    if (transaction->getType() == 1) {
        iconItem->setText("+");
        iconItem->setForeground(QBrush(QColor("#2ecc71")));
    } else {
        iconItem->setText("-");
        iconItem->setForeground(QBrush(QColor("#ff6b6b")));
    }
    QFont iconFont = iconItem->font();
    iconFont.setPointSize(iconFont.pointSize() + 6);
    iconFont.setBold(true);
    iconItem->setFont(iconFont);
    iconItem->setTextAlignment(Qt::AlignCenter);
    table_->setItem(row, 0, iconItem);
    
    // ID
    auto* idItem = new QTableWidgetItem(QString::number(transaction->getID()));
    idItem->setTextAlignment(Qt::AlignCenter);
    table_->setItem(row, 1, idItem);
    
    // Name
    table_->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(transaction->getName().c_str())));
    
    // Category
    table_->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(transaction->getCategory().c_str())));
    
    // Date
    auto* dateItem = new QTableWidgetItem(QString::fromStdString(transaction->getDate().getDate()));
    dateItem->setTextAlignment(Qt::AlignCenter);
    table_->setItem(row, 4, dateItem);
    
    // Amount
    auto* amountItem = new QTableWidgetItem(QString::number(transaction->getAmount(), 'f', 2));
    amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    table_->setItem(row, 5, amountItem);
    
    // Type
    QString type = (transaction->getType() == 1) ? QString::fromUtf8("Доход") : QString::fromUtf8("Расход");
    auto* typeItem = new QTableWidgetItem(type);
    typeItem->setTextAlignment(Qt::AlignCenter);
    table_->setItem(row, 6, typeItem);
    
    // Additional info
    QString additionalInfo;
    if (transaction->getType() == 1) {
        auto income = std::dynamic_pointer_cast<IncomeTransaction>(transaction);
        if (income) additionalInfo = QString::fromUtf8(income->getIncomeSource().c_str());
    } else {
        auto expense = std::dynamic_pointer_cast<Expense>(transaction);
        if (expense) additionalInfo = QString::fromUtf8(expense->getWhere().c_str());
    }
    table_->setItem(row, 7, new QTableWidgetItem(additionalInfo));
    
    // Actions
    auto* actionWidget = new QWidget(parent_);
    auto* actionLayout = new QHBoxLayout(actionWidget);
    actionLayout->setContentsMargins(8, 2, 8, 2);
    actionLayout->setSpacing(0);
    actionLayout->setAlignment(Qt::AlignCenter);
    
    auto* editButton = createEditButton(transaction->getID());
    actionLayout->addWidget(editButton);
    table_->setCellWidget(row, 8, actionWidget);
}

QPushButton* TransactionTableManager::createEditButton(size_t transactionId) {
    auto* editButton = new QPushButton(QString::fromUtf8("Редакт."), parent_);
    editButton->setFixedSize(85, 28);
    editButton->setToolTip(QString::fromUtf8("Редактировать транзакцию"));
    editButton->setStyleSheet(
        "QPushButton {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1f6feb, stop:1 #388bfd);"
        "  color: #ffffff;"
        "  border: none;"
        "  border-radius: 7px;"
        "  font-size: 9pt;"
        "  font-weight: 700;"
        "  padding: 4px 8px;"
        "}"
        "QPushButton:hover {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #388bfd, stop:1 #58a6ff);"
        "  box-shadow: 0 2px 8px rgba(56, 139, 253, 0.4);"
        "}"
        "QPushButton:pressed { background: #0969da; }"
    );
    editButton->setCursor(Qt::PointingHandCursor);
    editButton->setProperty("transactionId", QVariant::fromValue<qulonglong>(transactionId));
    connect(editButton, &QPushButton::clicked, this, &TransactionTableManager::onEditButtonClicked);
    return editButton;
}

void TransactionTableManager::onEditButtonClicked() {
    const auto* button = qobject_cast<const QPushButton*>(sender());
    if (!button) return;
    auto id = button->property("transactionId").toULongLong();
    emit editRequested(id);
}

void TransactionTableManager::onTableDoubleClicked(int row, int column) {
    (void)column;
    if (row < 0 || row >= table_->rowCount()) return;
    const auto* idItem = table_->item(row, 1);
    if (!idItem) return;
    size_t id = idItem->text().toULongLong();
    emit editRequested(id);
}

void TransactionTableManager::onTableContextMenu(const QPoint& pos) {
    QModelIndex index = table_->indexAt(pos);
    QMenu menu(parent_);
    const auto* editAct = menu.addAction(QString::fromUtf8("Редактировать"));
    const auto* delAct = menu.addAction(QString::fromUtf8("Удалить"));
    const auto* chosen = menu.exec(table_->viewport()->mapToGlobal(pos));
    if (!chosen) return;
    
    int row = index.isValid() ? index.row() : table_->currentRow();
    if (row < 0) return;
    const auto* idItem = table_->item(row, 1);
    if (!idItem) return;
    size_t id = idItem->text().toULongLong();
    
    if (chosen == editAct) {
        emit editRequested(id);
    } else if (chosen == delAct) {
        emit deleteRequested(id);
    }
}

void TransactionTableManager::handleHeaderClick(int logicalIndex, TransactionList& transactionList, // NOSONAR - modifies external table_ and reference parameters
                                                 int& currentSortColumn, bool& dateSortAscending, bool& amountSortAscending) { // NOSONAR - modifies external table_ and reference parameters
    if (logicalIndex == 4) {
        currentSortColumn = 4;
        transactionList.sortByDate();
        if (!dateSortAscending) transactionList.reverseOrder();
        dateSortAscending = !dateSortAscending;
        table_->horizontalHeader()->setSortIndicator(4, dateSortAscending ? Qt::AscendingOrder : Qt::DescendingOrder);
    } else if (logicalIndex == 5) {
        currentSortColumn = 5;
        transactionList.sortByAmount();
        if (!amountSortAscending) transactionList.reverseOrder();
        amountSortAscending = !amountSortAscending;
        table_->horizontalHeader()->setSortIndicator(5, amountSortAscending ? Qt::AscendingOrder : Qt::DescendingOrder);
    } else {
        currentSortColumn = -1;
        table_->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
    }
}

