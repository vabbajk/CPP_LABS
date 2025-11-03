#include "../include/MainWindow.h"
#include "../include/AddTransactionDialog.h"
#include "../include/EditTransactionDialog.h"
#include <QApplication>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QGroupBox>
#include <QString>
#include <QFileDialog>
#include <QCalendarWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), transactionList("homeaccounting.dat") {
    
    setWindowTitle(QString::fromUtf8("Домашняя бухгалтерия"));
    setMinimumSize(1100, 600);
    resize(1300, 650);  // Оптимальный размер для комфортного просмотра
    
    setupUI();
    createMenuBar();
    connectSignals();
    

    transactionList.loadFromDatabase();
    applyFiltersAndUpdateTable();

    // Apply dark theme (permanently enabled)
    applyTheme(true);
}

void MainWindow::buildCategoryModel() {
    categoryModel->clear();
    categoryModel->setHorizontalHeaderLabels({QString::fromUtf8("Категория")});

    auto makeParent = [&](const QString& title) {
        auto* parent = new QStandardItem(title);
        parent->setFlags(Qt::ItemIsEnabled); // not selectable
        return parent;
    };

    QStandardItem* incomeRoot = makeParent(QString::fromUtf8("Доходы"));
    QStringList incomeCats = {
        QString::fromUtf8("Зарплата"), QString::fromUtf8("Премия"), QString::fromUtf8("Бонусы"),
        QString::fromUtf8("Фриланс"), QString::fromUtf8("Подарки"), QString::fromUtf8("Пассивный доход"),
        QString::fromUtf8("Дивиденды"), QString::fromUtf8("Проценты по вкладам"), QString::fromUtf8("Аренда имущества"),
        QString::fromUtf8("Продажа вещей"), QString::fromUtf8("Прочее")
    };
    for (const auto& c : incomeCats) {
        auto* item = new QStandardItem(c);
        item->setData(1, Qt::UserRole + 1); // type: income
        item->setData(c, Qt::UserRole + 2); // category
        incomeRoot->appendRow(item);
    }

    QStandardItem* expenseRoot = makeParent(QString::fromUtf8("Расходы"));
    QStringList expenseCats = {
        QString::fromUtf8("Продукты"), QString::fromUtf8("Кафе и рестораны"), QString::fromUtf8("Транспорт"),
        QString::fromUtf8("Такси"), QString::fromUtf8("ЖКХ"), QString::fromUtf8("Связь и интернет"),
        QString::fromUtf8("Одежда и обувь"), QString::fromUtf8("Здоровье"), QString::fromUtf8("Аптека"),
        QString::fromUtf8("Образование"), QString::fromUtf8("Подписки"), QString::fromUtf8("Развлечения"),
        QString::fromUtf8("Путешествия"), QString::fromUtf8("Авто: топливо"), QString::fromUtf8("Авто: обслуживание"),
        QString::fromUtf8("Дом и ремонт"), QString::fromUtf8("Дети"), QString::fromUtf8("Подарки"),
        QString::fromUtf8("Налоги и сборы"), QString::fromUtf8("Благотворительность"), QString::fromUtf8("Прочее")
    };
    for (const auto& c : expenseCats) {
        auto* item = new QStandardItem(c);
        item->setData(0, Qt::UserRole + 1); // type: expense
        item->setData(c, Qt::UserRole + 2); // category
        expenseRoot->appendRow(item);
    }

    // Add an "Любая категория" option as a flat root row for clearing selection
    auto* anyItem = new QStandardItem(QString::fromUtf8("Любая категория"));
    anyItem->setData(-1, Qt::UserRole + 1);
    anyItem->setData(QString(), Qt::UserRole + 2);
    categoryModel->appendRow(anyItem);

    categoryModel->appendRow(incomeRoot);
    categoryModel->appendRow(expenseRoot);

    categoryView->expandAll();
    categoryCombo->setCurrentIndex(0);
}

void MainWindow::applyFiltersAndUpdateTable() {
    QString nameQuery = nameSearchEdit->text().trimmed();
    QModelIndex mi = categoryView->currentIndex();
    int typeSel = mi.data(Qt::UserRole + 1).isValid() ? mi.data(Qt::UserRole + 1).toInt() : -1;
    QString catSel = mi.data(Qt::UserRole + 2).toString();
    QDate qFrom = dateFromEdit->date();
    QDate qTo = dateToEdit->date();

    auto all = transactionList.getAllTransactions();
    std::list<std::shared_ptr<Transaction>> filtered;
    
    // If date range is invalid (from > to), show empty list
    if (qFrom > qTo) {
        // Skip filtering, leave filtered list empty
    } else {
        for (const auto& t : all) {
        bool ok = true;
        if (!nameQuery.isEmpty()) {
            ok &= QString::fromUtf8(t->getName().c_str()).contains(nameQuery, Qt::CaseInsensitive);
        }
        if (typeSel != -1 && !catSel.isEmpty()) {
            ok &= (t->getType() == typeSel) && (QString::fromUtf8(t->getCategory().c_str()) == catSel);
        }
        // date range inclusive using QDate for robustness
        Date td = t->getDate();
        QDate qtd(td.getYear(), td.getMonth(), td.getDay());
            ok &= !(qtd < qFrom) && !(qTo < qtd);
            if (ok) filtered.push_back(t);
        }
    }

    // Update balance for filtered list
    double total = 0.0;
    for (const auto& t : filtered) {
        if (t->getType()) total += t->getAmount();
        else total -= t->getAmount();
    }
    QString balanceText = QString::fromUtf8("Баланс: %1 руб.").arg(total, 0, 'f', 2);
    balanceLabel->setText(balanceText);
    
    // Сохраняем стиль темной темы с динамическим цветом
    QString balanceColor = total >= 0 ? "#57f287" : "#ff7b72";  // Зеленый или красный для темной темы
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

    // Render filtered list (same as in updateTable loop body)
    transactionTable->setRowCount(0);
    int row = 0;
    for (const auto& transaction : filtered) {
        transactionTable->insertRow(row);
        QTableWidgetItem* iconItem = new QTableWidgetItem();
        if (transaction->getType() == 1) {
            iconItem->setText("+");
            iconItem->setForeground(QBrush(Qt::green));
        } else {
            iconItem->setText("-");
            iconItem->setForeground(QBrush(Qt::red));
        }
        iconItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 0, iconItem);

        // ID - выравнивание по центру
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(transaction->getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 1, idItem);
        
        // Название - выравнивание по левому краю (по умолчанию)
        transactionTable->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(transaction->getName().c_str())));
        
        // Категория - выравнивание по левому краю
        transactionTable->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(transaction->getCategory().c_str())));
        
        // Дата - выравнивание по центру
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(transaction->getDate().getDate()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 4, dateItem);
        
        // Сумма - выравнивание по правому краю
        QTableWidgetItem* amountItem = new QTableWidgetItem(QString::number(transaction->getAmount(), 'f', 2));
        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        transactionTable->setItem(row, 5, amountItem);

        QString type;
        QString additionalInfo;
        if (transaction->getType() == 1) {
            type = QString::fromUtf8("Доход");
            auto income = std::dynamic_pointer_cast<IncomeTransaction>(transaction);
            if (income) additionalInfo = QString::fromUtf8(income->getIncomeSource().c_str());
        } else {
            type = QString::fromUtf8("Расход");
            auto expense = std::dynamic_pointer_cast<Expense>(transaction);
            if (expense) additionalInfo = QString::fromUtf8(expense->getWhere().c_str());
        }
        
        // Тип - выравнивание по центру
        QTableWidgetItem* typeItem = new QTableWidgetItem(type);
        typeItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 6, typeItem);
        
        // Доп. инфо - выравнивание по левому краю
        transactionTable->setItem(row, 7, new QTableWidgetItem(additionalInfo));

        QWidget* actionWidget = new QWidget(this);
        QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(8, 2, 8, 2);
        actionLayout->setSpacing(0);
        actionLayout->setAlignment(Qt::AlignCenter);
        
        QPushButton* editRowButton = new QPushButton(QString::fromUtf8("Редакт."), actionWidget);
        editRowButton->setFixedSize(85, 28);
        editRowButton->setToolTip(QString::fromUtf8("Редактировать транзакцию"));
        editRowButton->setStyleSheet(
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
        editRowButton->setCursor(Qt::PointingHandCursor);
        editRowButton->setProperty("transactionId", QVariant::fromValue<qulonglong>(transaction->getID()));
        actionLayout->addWidget(editRowButton);
        transactionTable->setCellWidget(row, 8, actionWidget);
        connect(editRowButton, &QPushButton::clicked, this, [this, editRowButton]() {
            size_t id = static_cast<size_t>(editRowButton->property("transactionId").toULongLong());
            auto transactionsLocal = transactionList.getAllTransactions();
            for (const auto& trans : transactionsLocal) {
                if (trans->getID() == id) {
                    EditTransactionDialog dialog(trans, this);
                    if (dialog.exec() == QDialog::Accepted) {
                        if (dialog.isDeleteRequested()) {
                            deleteTransactionById(id);
                        } else {
                            auto updatedTransaction = dialog.getUpdatedTransaction();
                            transactionList.updateTransaction(id, updatedTransaction);
                            transactionList.saveToDatabase();
                            applyFiltersAndUpdateTable();
                        }
                    }
                    break;
                }
            }
        });
        row++;
    }

    transactionTable->setColumnWidth(0, 30);
    transactionTable->setColumnWidth(1, 60);
    transactionTable->setColumnWidth(2, 150);
    transactionTable->setColumnWidth(3, 130);
    transactionTable->setColumnWidth(4, 100);
    transactionTable->setColumnWidth(5, 90);
    transactionTable->setColumnWidth(6, 80);
    transactionTable->setColumnWidth(7, 120);
    transactionTable->setColumnWidth(8, 100);

    transactionTable->setStyleSheet(
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


    // Подгоняем высоту строк
    transactionTable->verticalHeader()->setDefaultSectionSize(52);
    //transactionTable->resizeRowsToContents();

    transactionTable->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow() {

    transactionList.saveToDatabase();
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    // === БАЛАНС ===
    balanceLabel = new QLabel(QString::fromUtf8("Баланс: 0.00 руб."), this);
    balanceLabel->setObjectName("balanceLabel");
    QFont balanceFont = balanceLabel->font();
    balanceFont.setPointSize(14);
    balanceFont.setBold(true);
    balanceLabel->setFont(balanceFont);
    mainLayout->addWidget(balanceLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    mainLayout->setContentsMargins(12, 8, 12, 8);



    // === ОСНОВНОЙ КОНТЕЙНЕР (таблица + панель поиска) ===
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(16);
    
    // === ЛЕВАЯ ЧАСТЬ: Таблица + кнопки ===
    QWidget* leftWidget = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(12);
    
    // Настройка таблицы
    transactionTable = new QTableWidget(this);
    transactionTable->setColumnCount(9);
    
    QStringList headers;
    headers << "" << "ID" << QString::fromUtf8("Название") << QString::fromUtf8("Категория") 
            << QString::fromUtf8("Дата") << QString::fromUtf8("Сумма") 
            << QString::fromUtf8("Тип") << QString::fromUtf8("Доп. инфо") << QString::fromUtf8("Действия");
    transactionTable->setHorizontalHeaderLabels(headers);
    
    // Убираем белый угол ПОЛНОСТЬЮ
    transactionTable->setCornerButtonEnabled(false);
    transactionTable->verticalHeader()->setVisible(false);
    
    // Настройки заголовков
    transactionTable->horizontalHeader()->setStretchLastSection(false);
    transactionTable->horizontalHeader()->setSortIndicatorShown(true);
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    
    // Фиксированные ширины столбцов
    transactionTable->setColumnWidth(0, 30);   // +/-
    transactionTable->setColumnWidth(1, 60);   // ID
    transactionTable->setColumnWidth(2, 150);  // Название
    transactionTable->setColumnWidth(3, 130);  // Категория
    transactionTable->setColumnWidth(4, 100);  // Дата
    transactionTable->setColumnWidth(5, 90);   // Сумма
    transactionTable->setColumnWidth(6, 80);   // Тип
    transactionTable->setColumnWidth(7, 120);  // Доп. инфо
    transactionTable->setColumnWidth(8, 100);  // Действия
    
    // Внешний вид таблицы
    transactionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    transactionTable->setSelectionMode(QAbstractItemView::SingleSelection);
    transactionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    transactionTable->setAlternatingRowColors(true);
    transactionTable->setShowGrid(false);
    transactionTable->verticalHeader()->setDefaultSectionSize(36);
    
    leftLayout->addWidget(transactionTable);
    
    // Кнопки добавления
    QHBoxLayout* buttonLayout = new QHBoxLayout();
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
    
    // === ПРАВАЯ ЧАСТЬ: Панель поиска ===
    searchPanel = new QWidget(this);
    searchPanel->setObjectName("searchPanel");
    searchPanel->setFixedWidth(280);
    
    QVBoxLayout* rightLayout = new QVBoxLayout(searchPanel);
    rightLayout->setContentsMargins(16, 16, 16, 16);
    rightLayout->setSpacing(12);
    
    // Заголовок панели
    QHBoxLayout* searchHeader = new QHBoxLayout();
    QLabel* searchTitle = new QLabel(QString::fromUtf8("Поиск"), searchPanel);
    QFont titleFont = searchTitle->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    searchTitle->setFont(titleFont);
    
    analyticsButton = new QPushButton(QString::fromUtf8("Аналитика"), searchPanel);
    analyticsButton->setCursor(Qt::PointingHandCursor);
    
    searchHeader->addWidget(searchTitle);
    searchHeader->addStretch();
    searchHeader->addWidget(analyticsButton);
    rightLayout->addLayout(searchHeader);
    
    // Поле поиска
    nameSearchEdit = new QLineEdit(searchPanel);
    nameSearchEdit->setPlaceholderText(QString::fromUtf8("Поиск по названию"));
    nameSearchEdit->setMinimumHeight(36);
    rightLayout->addWidget(nameSearchEdit);
    
    // Категории
    categoryCombo = new QComboBox(searchPanel);
    categoryView = new QTreeView(categoryCombo);
    categoryModel = new QStandardItemModel(categoryCombo);
    categoryCombo->setModel(categoryModel);
    categoryCombo->setView(categoryView);
    categoryCombo->setEditable(false);
    categoryCombo->setMinimumHeight(36);
    buildCategoryModel();
    rightLayout->addWidget(categoryCombo);
    
    // Даты
    QHBoxLayout* datesLayout = new QHBoxLayout();
    datesLayout->setSpacing(8);
    
    dateFromEdit = new QDateEdit(QDate::currentDate().addDays(-30), searchPanel);
    dateFromEdit->setCalendarPopup(true);
    dateFromEdit->setDisplayFormat("dd.MM.yyyy");
    dateFromEdit->setMinimumHeight(36);
    
    dateToEdit = new QDateEdit(QDate::currentDate(), searchPanel);
    dateToEdit->setCalendarPopup(true);
    dateToEdit->setDisplayFormat("dd.MM.yyyy");
    dateToEdit->setMinimumHeight(36);
    
    datesLayout->addWidget(dateFromEdit);
    datesLayout->addWidget(new QLabel("-", searchPanel));
    datesLayout->addWidget(dateToEdit);
    rightLayout->addLayout(datesLayout);
    
    // Кнопка сброса
    resetFiltersButton = new QPushButton(QString::fromUtf8("Сбросить фильтры"), searchPanel);
    resetFiltersButton->setMinimumHeight(36);
    resetFiltersButton->setCursor(Qt::PointingHandCursor);
    rightLayout->addWidget(resetFiltersButton);
    
    rightLayout->addStretch();
    contentLayout->addWidget(searchPanel);
    
    mainLayout->addLayout(contentLayout);
}

void MainWindow::createMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);
    
    QMenu* fileMenu = menuBar->addMenu(QString::fromUtf8("Файл"));
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction(QString::fromUtf8("Выход"));
    
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    
    QMenu* helpMenu = menuBar->addMenu(QString::fromUtf8("Справка"));
    QAction* aboutAction = helpMenu->addAction(QString::fromUtf8("О программе"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::onAbout);

    QMenu* settingsMenu = menuBar->addMenu(QString::fromUtf8("Настройки"));
    QAction* exportTxtAction = settingsMenu->addAction(QString::fromUtf8("Выгрузить все транзакции в TXT..."));
    connect(exportTxtAction, &QAction::triggered, this, &MainWindow::onExportTxt);

    // Тёмная тема включена постоянно (опция переключения удалена)
}

void MainWindow::connectSignals() {
    connect(addIncomeButton, &QPushButton::clicked, this, &MainWindow::onAddIncome);
    connect(addExpenseButton, &QPushButton::clicked, this, &MainWindow::onAddExpense);
    // bottom delete disabled; deletion handled per row
    transactionTable->horizontalHeader()->setSectionsClickable(true);
    connect(transactionTable->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderClicked);
    connect(nameSearchEdit, &QLineEdit::textChanged, this, [this]{ applyFiltersAndUpdateTable(); });
    connect(categoryCombo, qOverload<int>(&QComboBox::currentIndexChanged), this, [this](int){ applyFiltersAndUpdateTable(); });
    connect(dateFromEdit, &QDateEdit::dateChanged, this, [this](const QDate&){ applyFiltersAndUpdateTable(); });
    connect(dateToEdit, &QDateEdit::dateChanged, this, [this](const QDate&){ applyFiltersAndUpdateTable(); });
    connect(resetFiltersButton, &QPushButton::clicked, this, [this]{
        nameSearchEdit->clear();
        categoryCombo->setCurrentIndex(0);
        dateFromEdit->setDate(QDate::currentDate().addMonths(-1));
        dateToEdit->setDate(QDate::currentDate());
        applyFiltersAndUpdateTable();
    });
    connect(analyticsButton, &QPushButton::clicked, this, [this]{
        StatsDialog dlg(&transactionList, this);
        dlg.exec();
    });
    connect(transactionTable, &QTableWidget::cellDoubleClicked, this, &MainWindow::onTableDoubleClicked);
    transactionTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(transactionTable, &QWidget::customContextMenuRequested, this, &MainWindow::onTableContextMenu);
    transactionTable->setColumnWidth(0, 30);
}

void MainWindow::applyTheme(bool dark) {
    // Base style and font
    qApp->setStyle("Fusion");
    QFont font("Segoe UI", 10);
    qApp->setFont(font);

    const QString lightQss = R"(
        QWidget { font-family: 'Segoe UI'; font-size: 10pt; }
        QMainWindow { background: #f6f7fb; }
        QLabel#balanceLabel { color: #222; }
        /* Buttons */
        QPushButton { background: #ffffff; color: #222; border: 1px solid #d9d9e3; border-radius: 8px; padding: 8px 14px; }
        QPushButton:hover { background: #f0f2f5; }
        QPushButton:pressed { background: #e6e8ee; }
        QPushButton[btnRole="primary"] { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #2196f3, stop:1 #1976d2); color: #fff; border: none; font-weight: 600; }
        QPushButton[btnRole="primary"]:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #1e88e5, stop:1 #1565c0); }
        QPushButton[btnRole="primary"]:pressed { background: #0d47a1; }
        QPushButton[btnRole="danger"] { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #f44336, stop:1 #d32f2f); color: #fff; border: none; font-weight: 600; }
        QPushButton[btnRole="danger"]:hover { background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #e53935, stop:1 #c62828); }
        QPushButton[btnRole="danger"]:pressed { background: #b71c1c; }
        /* Inputs */
        QLineEdit, QComboBox, QDateEdit, QDoubleSpinBox { background: #ffffff; color: #222; border: 1px solid #d9d9e3; border-radius: 8px; padding: 6px 8px; }
        QComboBox::drop-down { border: 0; width: 28px; }
        QComboBox QAbstractItemView { background: #ffffff; selection-background-color: #e6f0ff; }
        /* Table */
        QHeaderView::section { background: #f6f7fb; color: #444; border: 0; border-bottom: 1px solid #e6e8ee; padding: 8px 6px; font-weight: 600; }
        QTableView { selection-background-color: #e6f0ff; selection-color: #111; alternate-background-color: #fafbfe; }
        QTableWidget { background: #ffffff; gridline-color: #eeeeee; }
        /* Calendar */
        QCalendarWidget QWidget { color: #222; background: #ffffff; }
        QCalendarWidget QAbstractItemView:enabled { color: #222; selection-background-color: #e6f0ff; selection-color: #111; }
        QCalendarWidget QToolButton { background: transparent; color: #222; border: none; padding: 4px; }
        QCalendarWidget QToolButton:hover { background: #f0f2f5; border-radius: 6px; }
        /* Scrollbars */
        QScrollBar:vertical { background: transparent; width: 10px; }
        QScrollBar::handle:vertical { background: #c9ccda; border-radius: 4px; }
        QScrollBar::handle:vertical:hover { background: #b5b9c9; }
        QScrollBar::add-line, QScrollBar::sub-line { height: 0; }
        /* Card-like search panel */
        #searchPanel { background: #ffffff; border: 1px solid #e6e8ee; border-radius: 12px; padding: 12px; }
    )";

    const QString darkQss = R"(
        /* ===== Modern Dark Theme ===== */
        QWidget { 
            font-family: 'Segoe UI', -apple-system, BlinkMacSystemFont, system-ui, sans-serif; 
            font-size: 10pt; 
            color: #e6edf3; 
        }
        
        /* Main Window Background - Deep Dark */
        QMainWindow { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #0d1117, 
                stop:1 #010409);
        }
        
        /* Balance Label - Dynamic Color */
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
        
        /* Standard Buttons */
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
        
        /* Primary Button - Vibrant Blue Gradient */
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
        
        /* Danger Button - Red Gradient */
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
        
        /* Input Fields - Dark with Glow */
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
        
        /* ComboBox Dropdown */
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
        
        /* Table - Sleek Dark */
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
        
        /* Calendar Widget - Modern Dark */
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
        
        /* Scrollbars - Sleek */
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
        
        /* Search Panel - Card Style with Glow */
        #searchPanel { 
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #161b22, 
                stop:1 #0d1117); 
            border: 2px solid #21262d;
            border-radius: 16px; 
            padding: 16px;
        }
        
        /* Menu Bar */
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
        
        /* Menu Dropdown */
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
        
        /* Message Box */
        QMessageBox {
            background: #0d1117;
        }
        QMessageBox QLabel {
            color: #e6edf3;
        }
        QMessageBox QPushButton {
            min-width: 80px;
        }
        
        /* Dialog */
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #161b22, 
                stop:1 #0d1117);
        }
    )";

    qApp->setStyleSheet(dark ? darkQss : lightQss);
}

void MainWindow::updateTable() {
    transactionTable->setRowCount(0);

    auto transactions = transactionList.getAllTransactions();
    int row = 0;

    for (const auto& transaction : transactions) {
        transactionTable->insertRow(row);

        // Иконка +/- (по центру)
        QTableWidgetItem* iconItem = new QTableWidgetItem();
        if (transaction->getType() == 1) {
            iconItem->setText("+");
            iconItem->setForeground(QBrush(Qt::green));
        } else {
            iconItem->setText("-");
            iconItem->setForeground(QBrush(Qt::red));
        }
        iconItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 0, iconItem);

        // ID
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(transaction->getID()));
        idItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 1, idItem);

        // Название
        transactionTable->setItem(row, 2, new QTableWidgetItem(QString::fromUtf8(transaction->getName().c_str())));

        // Категория
        transactionTable->setItem(row, 3, new QTableWidgetItem(QString::fromUtf8(transaction->getCategory().c_str())));

        // Дата
        QTableWidgetItem* dateItem = new QTableWidgetItem(QString::fromStdString(transaction->getDate().getDate()));
        dateItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 4, dateItem);

        // Сумма (по правому краю)
        QTableWidgetItem* amountItem = new QTableWidgetItem(QString::number(transaction->getAmount(), 'f', 2));
        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        transactionTable->setItem(row, 5, amountItem);

        // Тип и доп. информация
        QString type;
        QString additionalInfo;

        if (transaction->getType() == 1) {
            type = QString::fromUtf8("Доход");
            auto income = std::dynamic_pointer_cast<IncomeTransaction>(transaction);
            if (income)
                additionalInfo = QString::fromUtf8(income->getIncomeSource().c_str());
        } else {
            type = QString::fromUtf8("Расход");
            auto expense = std::dynamic_pointer_cast<Expense>(transaction);
            if (expense)
                additionalInfo = QString::fromUtf8(expense->getWhere().c_str());
        }

        QTableWidgetItem* typeItem = new QTableWidgetItem(type);
        typeItem->setTextAlignment(Qt::AlignCenter);
        transactionTable->setItem(row, 6, typeItem);

        transactionTable->setItem(row, 7, new QTableWidgetItem(additionalInfo));

        // === Кнопка "Редакт." ===
        QWidget* actionWidget = new QWidget(this);
        QHBoxLayout* actionLayout = new QHBoxLayout(actionWidget);
        actionLayout->setContentsMargins(4, 0, 4, 0);
        actionLayout->setSpacing(0);
        actionLayout->setAlignment(Qt::AlignCenter);

        // Политика размера — чтобы ячейка не сжимала кнопку
        actionWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        QPushButton* editRowButton = new QPushButton(QString::fromUtf8("Редакт."), actionWidget);
        editRowButton->setFixedHeight(30);
        editRowButton->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
        editRowButton->setCursor(Qt::PointingHandCursor);
        editRowButton->setToolTip(QString::fromUtf8("Редактировать транзакцию"));

        // Стиль кнопки
        editRowButton->setStyleSheet(
            "QPushButton {"
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1f6feb, stop:1 #388bfd);"
            "  color: #ffffff;"
            "  border: none;"
            "  border-radius: 7px;"
            "  font-size: 9pt;"
            "  font-weight: 700;"
            "  padding: 4px 10px;"
            "}"
            "QPushButton:hover {"
            "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #388bfd, stop:1 #58a6ff);"
            "}"
            "QPushButton:pressed { background: #0969da; }"
        );

        editRowButton->setProperty("transactionId", QVariant::fromValue<qulonglong>(transaction->getID()));

        actionLayout->addWidget(editRowButton);
        transactionTable->setCellWidget(row, 8, actionWidget);

        // Сигнал редактирования
        connect(editRowButton, &QPushButton::clicked, this, [this, editRowButton]() {
            size_t id = static_cast<size_t>(editRowButton->property("transactionId").toULongLong());
            auto transactionsLocal = transactionList.getAllTransactions();

            for (const auto& trans : transactionsLocal) {
                if (trans->getID() == id) {
                    EditTransactionDialog dialog(trans, this);
                    if (dialog.exec() == QDialog::Accepted) {
                        if (dialog.isDeleteRequested()) {
                            deleteTransactionById(id);
                        } else {
                            auto updatedTransaction = dialog.getUpdatedTransaction();
                            transactionList.updateTransaction(id, updatedTransaction);
                            transactionList.saveToDatabase();
                            applyFiltersAndUpdateTable();
                        }
                    }
                    break;
                }
            }
        });

        row++;
    }

    // Настройка ширины и поведения колонок
    transactionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    transactionTable->horizontalHeader()->setStretchLastSection(false);

    transactionTable->setColumnWidth(0, 30);   // +/-
    transactionTable->setColumnWidth(1, 60);   // ID
    transactionTable->setColumnWidth(2, 150);  // Название
    transactionTable->setColumnWidth(3, 130);  // Категория
    transactionTable->setColumnWidth(4, 100);  // Дата
    transactionTable->setColumnWidth(5, 90);   // Сумма
    transactionTable->setColumnWidth(6, 80);   // Тип
    transactionTable->setColumnWidth(7, 120);  // Доп. инфо
    transactionTable->setColumnWidth(8, 130);  // Действия

    // Чуть выше строки — чтобы кнопка не упиралась
    transactionTable->verticalHeader()->setDefaultSectionSize(44);

    // Гарантия корректной прорисовки
    transactionTable->setContentsMargins(0, 0, 0, 0);
    transactionTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
}


void MainWindow::updateBalance() {
    double balance = transactionList.getTotalBalance();
    QString balanceText = QString::fromUtf8("Баланс: %1 руб.").arg(balance, 0, 'f', 2);
    balanceLabel->setText(balanceText);
    
    // Сохраняем стиль темной темы с динамическим цветом
    QString balanceColor = balance >= 0 ? "#57f287" : "#ff7b72";  // Зеленый или красный для темной темы
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

    balanceLabel->setObjectName("balanceLabel");

}

void MainWindow::onAddIncome() {
    AddTransactionDialog dialog(true, this);
    if (dialog.exec() == QDialog::Accepted) {
        auto transaction = dialog.getTransaction();
        transactionList.addTransaction(transaction);
        transactionList.saveToDatabase();
        applyFiltersAndUpdateTable();
    }
}

void MainWindow::onAddExpense() {
    AddTransactionDialog dialog(false, this);
    if (dialog.exec() == QDialog::Accepted) {
        auto transaction = dialog.getTransaction();
        transactionList.addTransaction(transaction);
        transactionList.saveToDatabase();
        applyFiltersAndUpdateTable();
    }
}

void MainWindow::onEditTransaction() {
    int currentRow = transactionTable->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Предупреждение"), 
                           QString::fromUtf8("Выберите транзакцию для редактирования"));
        return;
    }
    
    QTableWidgetItem* idItem = transactionTable->item(currentRow, 0);
    if (idItem) {
        size_t id = idItem->text().toULongLong();
        

        auto transactions = transactionList.getAllTransactions();
        for (const auto& trans : transactions) {
            if (trans->getID() == id) {
                EditTransactionDialog dialog(trans, this);
                if (dialog.exec() == QDialog::Accepted) {
                    auto updatedTransaction = dialog.getUpdatedTransaction();
                    transactionList.updateTransaction(id, updatedTransaction);
                    transactionList.saveToDatabase();
                    updateTable();
                    updateBalance();
                }
                break;
            }
        }
    }
}

void MainWindow::onTableDoubleClicked(int row, int column) {
    Q_UNUSED(column);
    transactionTable->selectRow(row);
    onEditTransaction();
}

void MainWindow::onHeaderClicked(int logicalIndex) {
    // Columns: 0 icon, 1 ID, 2 Name, 3 Category, 4 Date, 5 Amount, 6 Type, 7 Additional, 8 Actions
    if (logicalIndex == 4) {
    transactionList.sortByDate();
        if (!dateSortAscending) transactionList.reverseOrder();
        dateSortAscending = !dateSortAscending;
        transactionTable->horizontalHeader()->setSortIndicator(4, dateSortAscending ? Qt::AscendingOrder : Qt::DescendingOrder);
        applyFiltersAndUpdateTable();
    } else if (logicalIndex == 5) {
        transactionList.sortByAmount();
        if (!amountSortAscending) transactionList.reverseOrder();
        amountSortAscending = !amountSortAscending;
        transactionTable->horizontalHeader()->setSortIndicator(5, amountSortAscending ? Qt::AscendingOrder : Qt::DescendingOrder);
        applyFiltersAndUpdateTable();
    }
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

void MainWindow::onApplyFilter() {
    // legacy filter action removed; using right search panel live filtering
}

void MainWindow::onClearFilter() {
    // legacy clear removed; handled by reset button in right panel
}

void MainWindow::deleteTransactionById(size_t id) {
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        QString::fromUtf8("Подтверждение"),
        QString::fromUtf8("Вы уверены, что хотите удалить эту транзакцию?"),
        QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) return;
    transactionList.deleteTransaction(id);
    transactionList.saveToDatabase();
    applyFiltersAndUpdateTable();
}

void MainWindow::deleteSelectedRow() {
    int row = transactionTable->currentRow();
    if (row < 0) return;
    QTableWidgetItem* idItem = transactionTable->item(row, 1);
    if (!idItem) return;
    size_t id = idItem->text().toULongLong();
    deleteTransactionById(id);
}

void MainWindow::onTableContextMenu(const QPoint& pos) {
    QModelIndex index = transactionTable->indexAt(pos);
    QMenu menu(this);
    QAction* editAct = menu.addAction(QString::fromUtf8("Редактировать"));
    QAction* delAct = menu.addAction(QString::fromUtf8("Удалить"));
    QAction* chosen = menu.exec(transactionTable->viewport()->mapToGlobal(pos));
    if (!chosen) return;
    int row = index.isValid() ? index.row() : transactionTable->currentRow();
    if (row < 0) return;
    QTableWidgetItem* idItem = transactionTable->item(row, 1);
    if (!idItem) return;
    size_t id = idItem->text().toULongLong();
    if (chosen == editAct) {
        auto transactionsLocal = transactionList.getAllTransactions();
        for (const auto& trans : transactionsLocal) {
            if (trans->getID() == id) {
                EditTransactionDialog dialog(trans, this);
                if (dialog.exec() == QDialog::Accepted) {
                    if (dialog.isDeleteRequested()) {
                        deleteTransactionById(id);
                    } else {
                        auto updatedTransaction = dialog.getUpdatedTransaction();
                        transactionList.updateTransaction(id, updatedTransaction);
                        transactionList.saveToDatabase();
                        applyFiltersAndUpdateTable();
                    }
                }
                break;
            }
        }
    } else if (chosen == delAct) {
        deleteTransactionById(id);
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, QString::fromUtf8("О программе"), 
                      QString::fromUtf8("Домашняя бухгалтерия v1.0\n\n"
                                           "Программа для учета личных финансов\n"
                                           "Создано с использованием Qt Framework"));
}
