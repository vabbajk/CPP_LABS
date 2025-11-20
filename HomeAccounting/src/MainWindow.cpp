#include "../include/MainWindow.h"
#include "../include/AddTransactionDialog.h"
#include "../include/EditTransactionDialog.h"
#include "../include/BudgetSettings.h"
#include "../include/BudgetSettingsDialog.h"
#include "../include/TransactionFilter.h"
#include "../include/FiltersPanel.h"
#include "../include/TransactionTableManager.h"
#include "../include/BudgetManager.h"
#include "../include/SavingsTracker.h"
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QGroupBox>
#include <QFrame>
#include <QString>
#include <QFileDialog>
#include <QCalendarWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <algorithm>
#include <cmath>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    
    setWindowTitle(QString::fromUtf8("Домашняя бухгалтерия"));
    setMinimumSize(1100, 600);
    resize(1300, 650);
    
    // Initialize manager classes
    budgetManager = new BudgetManager(budgetSettings, &transactionList, this);
    savingsTracker = new SavingsTracker(budgetSettings, &transactionList, this);
    
    setupUI();
    createMenuBar();
    connectSignals();

    transactionList.loadFromDatabase();
    applyFiltersAndUpdateTable();
    budgetManager->checkBudgetLimit();
    savingsTracker->updateSavingsRadar();
    savingsTracker->updateSavingsCounter();
    
    applyTheme(true);
}

void MainWindow::applyFiltersAndUpdateTable() {
    FilterCriteria criteria = filtersPanel->currentCriteria();

    auto all = transactionList.getAllTransactions();
    std::vector<std::shared_ptr<Transaction>> ordered = TransactionFilter::filterAndSort(all, criteria);

    updateBalanceFor(ordered);
    tableManager->rebuildTable(ordered);
    budgetManager->checkBudgetLimit();
    savingsTracker->updateSavingsRadar();
    savingsTracker->updateSavingsCounter();
}

void MainWindow::updateBalanceFor(const std::vector<std::shared_ptr<Transaction>>& ordered) {
    double total = 0.0;
    for (const auto& t : ordered) {
        if (t->getType()) {
            total += t->getAmount();
        } else {
            total -= t->getAmount();
        }
    }
    QString balanceText = QString::fromUtf8("Баланс: %1 руб.").arg(total, 0, 'f', 2);
    balanceLabel->setText(balanceText);

    QString balanceColor = total >= 0 ? "#57f287" : "#ff7b72";
    balanceLabel->setStyleSheet(QString(
        "QLabel#balanceLabel { "
        "  color: %1; "
        "  font-size: 16pt; "
        "  font-weight: 700; "
        "  padding: 8px 12px; "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(56, 139, 253, 0.1), stop:1 rgba(56, 139, 253, 0.05)); "
        "  border-radius: 12px; "
        "  border-left: 4px solid #388bfd; "
        "}"
    ).arg(balanceColor));
}



MainWindow::~MainWindow() {
    delete budgetSettings;
    transactionList.saveToDatabase();
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    auto* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);
    

    auto* topLayout = new QHBoxLayout();
    balanceLabel = new QLabel(QString::fromUtf8("Баланс: 0.00 руб."), this);
    balanceLabel->setObjectName("balanceLabel");
    QFont balanceFont = balanceLabel->font();
    balanceFont.setPointSize(14);
    balanceFont.setBold(true);
    balanceLabel->setFont(balanceFont);
    topLayout->addWidget(balanceLabel);
    

    auto* budgetInfoLabel = new QLabel(this);
    budgetInfoLabel->setTextFormat(Qt::RichText);
    QFont budgetFont = budgetInfoLabel->font();
    budgetFont.setPointSize(10);
    budgetInfoLabel->setFont(budgetFont);
    budgetInfoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    topLayout->addWidget(budgetInfoLabel, 1);
    

    auto* budgetWarningLabel = new QLabel(this);
    budgetWarningLabel->setVisible(false);
    budgetWarningLabel->setWordWrap(true);
    budgetWarningLabel->setTextFormat(Qt::RichText);
    QFont warningFont = budgetWarningLabel->font();
    warningFont.setPointSize(10);
    budgetWarningLabel->setFont(warningFont);
    
    budgetManager->setLabels(budgetInfoLabel, budgetWarningLabel);
    
    mainLayout->addLayout(topLayout);
    

    auto* warningLayout = new QHBoxLayout();
    warningLayout->addWidget(budgetWarningLabel);
    mainLayout->addLayout(warningLayout);
    
    auto* savingsCard = new QFrame(this);
    savingsCard->setObjectName("savingsCard");
    auto* savingsLayout = new QVBoxLayout(savingsCard);
    savingsLayout->setContentsMargins(16, 16, 16, 16);
    savingsLayout->setSpacing(10);

    auto* savingsTitleLabel = new QLabel(QString::fromUtf8("Радар накоплений"), savingsCard);
    QFont savingsTitleFont = savingsTitleLabel->font();
    savingsTitleFont.setPointSize(12);
    savingsTitleFont.setBold(true);
    savingsTitleLabel->setFont(savingsTitleFont);
    savingsLayout->addWidget(savingsTitleLabel);

    auto* savingsStatusLabel = new QLabel(savingsCard);
    savingsStatusLabel->setTextFormat(Qt::RichText);
    savingsLayout->addWidget(savingsStatusLabel);

    auto* savingsProgressBar = new QProgressBar(savingsCard);
    savingsProgressBar->setRange(0, 100);
    savingsProgressBar->setTextVisible(false);
    savingsProgressBar->setFixedHeight(14);
    savingsLayout->addWidget(savingsProgressBar);

    auto* savingsForecastLabel = new QLabel(savingsCard);
    savingsForecastLabel->setTextFormat(Qt::RichText);
    QFont forecastFont = savingsForecastLabel->font();
    forecastFont.setPointSize(9);
    savingsForecastLabel->setFont(forecastFont);
    savingsLayout->addWidget(savingsForecastLabel);

    mainLayout->addWidget(savingsCard);
    

    auto* savingsCounterCard = new QFrame(this);
    savingsCounterCard->setObjectName("savingsCard");
    auto* counterLayout = new QVBoxLayout(savingsCounterCard);
    counterLayout->setContentsMargins(16, 16, 16, 16);
    counterLayout->setSpacing(10);
    
    auto* counterTitleLabel = new QLabel(QString::fromUtf8("Счетчик накоплений"), savingsCounterCard);
    QFont counterTitleFont = counterTitleLabel->font();
    counterTitleFont.setPointSize(12);
    counterTitleFont.setBold(true);
    counterTitleLabel->setFont(counterTitleFont);
    counterLayout->addWidget(counterTitleLabel);
    
    auto* totalSavingsLabel = new QLabel(savingsCounterCard);
    totalSavingsLabel->setTextFormat(Qt::RichText);
    QFont totalSavingsFont = totalSavingsLabel->font();
    totalSavingsFont.setPointSize(11);
    totalSavingsLabel->setFont(totalSavingsFont);
    counterLayout->addWidget(totalSavingsLabel);
    
    savingsTracker->setWidgets(savingsProgressBar, savingsStatusLabel, savingsForecastLabel, totalSavingsLabel);
    
    mainLayout->addWidget(savingsCounterCard);
    
    mainLayout->setContentsMargins(12, 8, 12, 8);
    
    
    
    auto* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(16);
        
    auto* leftWidget = new QWidget(this);
    auto* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(12);
    

    transactionTable = new QTableWidget(this);
    tableManager = new TransactionTableManager(transactionTable, this);
    connect(tableManager, &TransactionTableManager::editRequested, this, &MainWindow::onEditRequested);
    connect(tableManager, &TransactionTableManager::deleteRequested, this, &MainWindow::onDeleteRequested);
    
    leftLayout->addWidget(transactionTable);
    
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(12);
    
    addIncomeButton = new QPushButton(QString::fromUtf8("+ Добавить доход"), this);
    addIncomeButton->setProperty("btnRole", "primary");
    addIncomeButton->setMinimumHeight(40);
    addIncomeButton->setCursor(Qt::PointingHandCursor);
    
    addExpenseButton = new QPushButton(QString::fromUtf8("- Добавить расход"), this);
    addExpenseButton->setProperty("btnRole", "danger");
    addExpenseButton->setMinimumHeight(40);
    addExpenseButton->setCursor(Qt::PointingHandCursor);
    
    buttonLayout->addWidget(addIncomeButton);
    buttonLayout->addWidget(addExpenseButton);
    buttonLayout->addStretch();
    
    leftLayout->addLayout(buttonLayout);
    contentLayout->addWidget(leftWidget, 1);
    

    filtersPanel = new FiltersPanel(this);
    contentLayout->addWidget(filtersPanel);
    
    mainLayout->addLayout(contentLayout);
}

void MainWindow::createMenuBar() {
    auto* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    auto* fileMenu = menuBar->addMenu(QString::fromUtf8("Файл"));
    fileMenu->addSeparator();
    const auto* exitAction = fileMenu->addAction(QString::fromUtf8("Выход"));
    
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    auto* helpMenu = menuBar->addMenu(QString::fromUtf8("Справка"));
    const auto* aboutAction = helpMenu->addAction(QString::fromUtf8("О программе"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    auto* settingsMenu = menuBar->addMenu(QString::fromUtf8("Настройки"));
    const auto* budgetSettingsAction = settingsMenu->addAction(QString::fromUtf8("Настройка бюджета..."));
    connect(budgetSettingsAction, &QAction::triggered, this, &MainWindow::onBudgetSettings);
    settingsMenu->addSeparator();
    const auto* exportTxtAction = settingsMenu->addAction(QString::fromUtf8("Выгрузить все транзакции в TXT..."));
    connect(exportTxtAction, &QAction::triggered, this, &MainWindow::onExportTxt);
}

void MainWindow::connectSignals() {
    connect(addIncomeButton, &QPushButton::clicked, this, &MainWindow::onAddIncome);
    connect(addExpenseButton, &QPushButton::clicked, this, &MainWindow::onAddExpense);

    transactionTable->horizontalHeader()->setSectionsClickable(true);
    connect(transactionTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderClicked);

    connect(filtersPanel->nameSearchEdit(), &QLineEdit::textChanged, this, [this]{ applyFiltersAndUpdateTable(); });
    connect(filtersPanel->categoryCombo(), qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int){ applyFiltersAndUpdateTable(); });
    connect(filtersPanel->dateFromEdit(), &QDateEdit::dateChanged, this, [this](const QDate&){ applyFiltersAndUpdateTable(); });
    connect(filtersPanel->dateToEdit(), &QDateEdit::dateChanged, this, [this](const QDate&){ applyFiltersAndUpdateTable(); });
    connect(filtersPanel->resetFiltersButton(), &QPushButton::clicked, this, [this]{
        filtersPanel->resetFiltersToDefault();
        applyFiltersAndUpdateTable();
    });
    connect(filtersPanel->analyticsButton(), &QPushButton::clicked, this, [this]{
        StatsDialog dlg(&transactionList, this);
        dlg.exec();
    });
}

void MainWindow::applyTheme(bool dark) const {

    QApplication::setStyle("Fusion");
    QFont font("Segoe UI", 10);
    QApplication::setFont(font);

    const QString lightQss = R"(
        QWidget { font-family: 'Segoe UI'; font-size: 10pt; }
        QMainWindow { background: #f6f7fb; }
        QLabel#balanceLabel { color: #222; }

        QPushButton { background: #ffffff; color: #222; border: 1px solid #d9d9e3; border-radius: 8px; padding: 8px 14px; }
        QPushButton:hover { background: #f0f2f5; }
        QPushButton:pressed { background: #e6e8ee; }
        QPushButton[btnRole="primary"] { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2196f3, stop:1 #1976d2); color: #fff; border: none; font-weight: 600; }
        QPushButton[btnRole="primary"]:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1e88e5, stop:1 #1565c0); }
        QPushButton[btnRole="primary"]:pressed { background: #0d47a1; }
        QPushButton[btnRole="danger"] { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f44336, stop:1 #d32f2f); color: #fff; border: none; font-weight: 600; }
        QPushButton[btnRole="danger"]:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #e53935, stop:1 #c62828); }
        QPushButton[btnRole="danger"]:pressed { background: #b71c1c; }

        QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox { background: #ffffff; color: #222; border: 1px solid #d9d9e3; border-radius: 8px; padding: 6px 8px; }
        QComboBox::drop-down { border: 0; width: 28px; }
        QComboBox QAbstractItemView { background: #ffffff; selection-background-color: #e6f0ff; }

        QHeaderView::section { background: #f6f7fb; color: #444; border: 0; border-bottom: 1px solid #e6e8ee; padding: 8px 6px; font-weight: 600; }
        QTableView { selection-background-color: #e6f0ff; selection-color: #111; alternate-background-color: #fafbfe; }
        QTableWidget { background: #ffffff; gridline-color: #eeeeee; }

        QCalendarWidget QWidget { color: #222; background: #ffffff; }
        QCalendarWidget QAbstractItemView:enabled { color: #222; selection-background-color: #e6f0ff; selection-color: #111; }
        QCalendarWidget QToolButton { background: transparent; color: #222; border: none; padding: 4px; }
        QCalendarWidget QToolButton:hover { background: #f0f2f5; border-radius: 6px; }

        QScrollBar:vertical { background: transparent; width: 10px; }
        QScrollBar::handle:vertical { background: #c9ccda; border-radius: 4px; }
        QScrollBar::handle:vertical:hover { background: #b5b9c9; }
        QScrollBar::add-line, QScrollBar::sub-line { height: 0; }

        #searchPanel { background: #ffffff; border: 1px solid #e6e8ee; border-radius: 12px; padding: 12px; }
        #savingsCard { background: #ffffff; border: 1px solid #e6e8ee; border-radius: 12px; }
        #savingsCard QLabel { color: #222; }
        #savingsCard QProgressBar { background: #ecedf4; border: none; border-radius: 7px; height: 14px; }
        #savingsCard QProgressBar::chunk { background: #1976d2; border-radius: 7px; }
    )";

    const QString darkQss = R"(

        QWidget { 
            font-family: 'Segoe UI', -apple-system, BlinkMacSystemFont, system-ui, sans-serif; 
            font-size: 10pt; 
            color: #e6edf3; 
        }
        

        QMainWindow { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #0d1117, 
                stop:1 #010409);
        }
        

        QLabel#balanceLabel { 
            color: #e6edf3; 
            font-size: 16pt;
            font-weight: 700;
            padding: 8px 12px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 rgba(56, 139, 253, 0.1), 
                stop:1 rgba(56, 139, 253, 0.05));
            border-radius: 12px;
            border-left: 4px solid #388bfd;
        }
        

        QPushButton { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #21262d, 
                stop:1 #161b22); 
            color: #e6edf3; 
            border: 1px solid #30363d;
            border-radius: 10px; 
            padding: 10px 18px;
            font-weight: 600;
            min-height: 24px;
        }
        QPushButton:hover { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #30363d, 
                stop:1 #21262d);
            border: 1px solid #388bfd;
            box-shadow: 0 0 8px rgba(56, 139, 253, 0.3);
        }
        QPushButton:pressed { 
            background: #0d1117;
            border: 1px solid #58a6ff;
        }
        

        QPushButton[btnRole="primary"] { 
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #1f6feb, 
                stop:0.5 #388bfd, 
                stop:1 #58a6ff); 
            color: #ffffff; 
            border: none;
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        QPushButton[btnRole="primary"]:hover { 
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #388bfd, 
                stop:0.5 #58a6ff, 
                stop:1 #79c0ff);
            box-shadow: 0 0 20px rgba(88, 166, 255, 0.5);
        }
        QPushButton[btnRole="primary"]:pressed { 
            background: #0969da;
        }
        

        QPushButton[btnRole="danger"] { 
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #da3633, 
                stop:0.5 #f85149, 
                stop:1 #ff7b72); 
            color: #ffffff; 
            border: none;
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        QPushButton[btnRole="danger"]:hover { 
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #f85149, 
                stop:0.5 #ff7b72, 
                stop:1 #ffa198);
            box-shadow: 0 0 20px rgba(248, 81, 73, 0.5);
        }
        QPushButton[btnRole="danger"]:pressed { 
            background: #b62324;
        }
        

        QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox { 
            background: #0d1117; 
            color: #e6edf3; 
            border: 2px solid #21262d;
            border-radius: 10px; 
            padding: 8px 12px;
            selection-background-color: #1f6feb;
            selection-color: #ffffff;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QDoubleSpinBox:focus {
            border: 2px solid #388bfd;
            background: #161b22;
            box-shadow: 0 0 10px rgba(56, 139, 253, 0.3);
        }
        QLineEdit:hover, QComboBox:hover, QDateEdit:hover, QDoubleSpinBox:hover {
            border: 2px solid #30363d;
        }
        

        QComboBox::drop-down { 
            border: 0; 
            width: 32px;
            padding-right: 4px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 4px solid transparent;
            border-right: 4px solid transparent;
            border-top: 5px solid #7d8590;
            width: 0;
            height: 0;
        }
        QComboBox QAbstractItemView { 
            background: #161b22; 
            border: 2px solid #30363d;
            border-radius: 8px;
            selection-background-color: #1f6feb;
            selection-color: #ffffff;
            padding: 4px;
        }
        

        QHeaderView::section { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #161b22, 
                stop:1 #0d1117); 
            color: #8b949e; 
            border: 0; 
            border-bottom: 2px solid #21262d; 
            border-right: 1px solid #21262d;
            padding: 10px 8px; 
            font-weight: 700;
            text-transform: uppercase;
            letter-spacing: 0.5px;
            font-size: 9pt;
        }
        QHeaderView::section:hover {
            background: #21262d;
            color: #58a6ff;
        }
        
        QTableView { 
            background: #0d1117; 
            selection-background-color: #1f6feb; 
            selection-color: #ffffff; 
            alternate-background-color: #161b22;
            gridline-color: #21262d;
        }
        QTableWidget { 
            background: #0d1117; 
            gridline-color: #21262d;
            border: 1px solid #21262d;
            border-radius: 12px;
        }
        QTableWidget::item {
            padding: 8px;
            border-bottom: 1px solid #21262d;
        }
        QTableWidget::item:selected {
            background: rgba(31, 111, 235, 0.2);
            color: #58a6ff;
        }
        QTableWidget::item:hover {
            background: rgba(48, 54, 61, 0.5);
        }
        

        QCalendarWidget QWidget { 
            color: #e6edf3; 
            background: #0d1117;
        }
        QCalendarWidget QAbstractItemView { 
            background: #161b22;
            selection-background-color: #1f6feb;
            selection-color: #ffffff;
            border: 1px solid #30363d;
            border-radius: 8px;
        }
        QCalendarWidget QAbstractItemView:enabled { 
            color: #e6edf3; 
        }
        QCalendarWidget QToolButton { 
            background: transparent; 
            color: #e6edf3; 
            border: none; 
            padding: 6px;
            border-radius: 6px;
        }
        QCalendarWidget QToolButton:hover { 
            background: #21262d; 
        }
        QCalendarWidget QToolButton:pressed {
            background: #1f6feb;
        }
        QCalendarWidget QMenu {
            background: #161b22;
            border: 1px solid #30363d;
        }
        

        QScrollBar:vertical { 
            background: transparent; 
            width: 12px;
            margin: 0;
        }
        QScrollBar::handle:vertical { 
            background: #30363d; 
            border-radius: 6px;
            min-height: 30px;
        }
        QScrollBar::handle:vertical:hover { 
            background: #484f58;
        }
        QScrollBar::handle:vertical:pressed {
            background: #58a6ff;
        }
        QScrollBar::add-line, QScrollBar::sub-line { 
            height: 0; 
        }
        QScrollBar:horizontal {
            background: transparent;
            height: 12px;
        }
        QScrollBar::handle:horizontal {
            background: #30363d;
            border-radius: 6px;
            min-width: 30px;
        }
        QScrollBar::handle:horizontal:hover {
            background: #484f58;
        }
        

        #searchPanel { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #161b22, 
                stop:1 #0d1117); 
            border: 2px solid #21262d;
            border-radius: 16px; 
            padding: 16px;
        }
        #savingsCard {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #161b22,
                stop:1 #0d1117);
            border: 2px solid #21262d;
            border-radius: 16px;
        }
        #savingsCard QLabel { color: #e6edf3; }
        #savingsCard QProgressBar {
            background: #1a1f24;
            border: 1px solid #2c3136;
            border-radius: 7px;
            height: 14px;
        }
        #savingsCard QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1f6feb,
                stop:1 #58a6ff);
            border-radius: 7px;
        }
        

        QMenuBar {
            background: #0d1117;
            color: #e6edf3;
            border-bottom: 1px solid #21262d;
            padding: 4px;
        }
        QMenuBar::item {
            background: transparent;
            padding: 6px 12px;
            border-radius: 6px;
        }
        QMenuBar::item:selected {
            background: #21262d;
            color: #58a6ff;
        }
        QMenuBar::item:pressed {
            background: #1f6feb;
        }
        

        QMenu {
            background: #161b22;
            border: 2px solid #30363d;
            border-radius: 8px;
            padding: 4px;
        }
        QMenu::item {
            padding: 8px 24px;
            border-radius: 6px;
        }
        QMenu::item:selected {
            background: #1f6feb;
            color: #ffffff;
        }
        

        QMessageBox {
            background: #0d1117;
        }
        QMessageBox QLabel {
            color: #e6edf3;
        }
        QMessageBox QPushButton {
            min-width: 80px;
        }
        

        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #161b22, 
                stop:1 #0d1117);
        }
    )";

    qApp->setStyleSheet(dark ? darkQss : lightQss);
}



void MainWindow::addTransaction(bool isIncome) {
    AddTransactionDialog dialog(isIncome, this);
    if (dialog.exec() == QDialog::Accepted) {
        auto transaction = dialog.getTransaction();
        transactionList.addTransaction(transaction);
        transactionList.saveToDatabase();
        applyFiltersAndUpdateTable();
    }
}

void MainWindow::onAddIncome() {
    addTransaction(true);
}

void MainWindow::onAddExpense() {
    addTransaction(false);
}

void MainWindow::onEditTransaction() {
    int currentRow = transactionTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Предупреждение"), 
                           QString::fromUtf8("Выберите транзакцию для редактирования"));
        return;
    }
    
    const auto* idItem = transactionTable->item(currentRow, 1);
    if (!idItem) return;
        size_t id = idItem->text().toULongLong();
    onEditRequested(id);
}

void MainWindow::onHeaderClicked(int logicalIndex) {
    tableManager->handleHeaderClick(logicalIndex, transactionList, currentSortColumn, dateSortAscending, amountSortAscending);
        applyFiltersAndUpdateTable();
}

void MainWindow::onExportTxt() {
    QString path = QFileDialog::getSaveFileName(this, QString::fromUtf8("Сохранить как"),
                                               QDir::homePath() + "/transactions.txt",
                                               QString::fromUtf8("Текстовые файлы (*.txt)"));
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, QString::fromUtf8("Ошибка"), QString::fromUtf8("Не удалось открыть файл для записи"));
        return;
    }
    QTextStream out(&f);
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    out.setEncoding(QStringConverter::Utf8);
#endif
    auto all = transactionList.getAllTransactions();
    for (const auto& t : all) {
        QString type = t->getType() ? QString::fromUtf8("Доход") : QString::fromUtf8("Расход");
        QString extra;
        if (t->getType()) {
            auto income = std::dynamic_pointer_cast<IncomeTransaction>(t);
            if (income) extra = QString::fromUtf8(income->getIncomeSource().c_str());
        } else {
            auto expense = std::dynamic_pointer_cast<Expense>(t);
            if (expense) extra = QString::fromUtf8(expense->getWhere().c_str());
        }
        out << t->getID() << "\t"
            << QString::fromUtf8(t->getName().c_str()) << "\t"
            << QString::fromUtf8(t->getCategory().c_str()) << "\t"
            << QString::fromStdString(t->getDate().getDate()) << "\t"
            << QString::number(t->getAmount(), 'f', 2) << "\t"
            << type << "\t"
            << extra << "\n";
    }
    f.close();
    QMessageBox::information(this, QString::fromUtf8("Готово"), QString::fromUtf8("Транзакции выгружены в файл"));
}


void MainWindow::deleteTransactionById(size_t id) {
    if (const auto reply = QMessageBox::question(this,
        QString::fromUtf8("Подтверждение"),
        QString::fromUtf8("Вы уверены, что хотите удалить эту транзакцию?"),
        QMessageBox::Yes | QMessageBox::No); reply != QMessageBox::Yes) {
        return;
    }
    transactionList.deleteTransaction(id);
    transactionList.saveToDatabase();
    applyFiltersAndUpdateTable();
}

void MainWindow::onEditRequested(size_t transactionId) {
    auto transactionsLocal = transactionList.getAllTransactions();
    auto it = std::find_if(transactionsLocal.begin(), transactionsLocal.end(),
        [transactionId](const std::shared_ptr<Transaction>& t) {
            return t->getID() == transactionId;
        });
    if (it == transactionsLocal.end()) return;

    EditTransactionDialog dialog(*it, this);
    if (dialog.exec() != QDialog::Accepted) return;

    if (dialog.isDeleteRequested()) {
        onDeleteRequested(transactionId);
        return;
    }

    auto updatedTransaction = dialog.getUpdatedTransaction(transactionId);
    transactionList.updateTransaction(transactionId, updatedTransaction);
                        transactionList.saveToDatabase();
                        applyFiltersAndUpdateTable();
                    }

void MainWindow::onDeleteRequested(size_t transactionId) {
    deleteTransactionById(transactionId);
}

void MainWindow::onAbout() {
    QMessageBox::about(this, QString::fromUtf8("О программе"), 
                      QString::fromUtf8("Домашняя бухгалтерия v1.0\n\n"
                                           "Программа для учета личных финансов\n"
                                           "Создано с использованием Qt Framework"));
}

void MainWindow::onBudgetSettings() {
    BudgetSettingsDialog dialog(budgetSettings, this);
    if (dialog.exec() == QDialog::Accepted) {
        budgetManager->checkBudgetLimit();
    }
}
