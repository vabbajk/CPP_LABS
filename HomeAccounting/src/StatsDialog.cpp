#include "../include/StatsDialog.h"
#include <QDate>
#include <QGridLayout>
#include <QDialogButtonBox>

StatsDialog::StatsDialog(const TransactionList* list, QWidget* parent)
    : QDialog(parent), transactionList(list) {
    setWindowTitle(QString::fromUtf8("Аналитика: диаграммы"));
    resize(1200, 750);
    setMinimumSize(1000, 650);

    auto* main = new QVBoxLayout(this);
    main->setSpacing(20);
    main->setContentsMargins(32, 32, 32, 32);
    
    auto* headerLayout = new QHBoxLayout();
    auto* titleLabel = new QLabel(QString::fromUtf8("📊 Финансовая аналитика"), this);
    titleLabel->setObjectName("titleLabel");
    QFont titleFont;
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet(
        "QLabel#titleLabel { "
        "  color: #58a6ff; "
        "  padding: 12px 16px; "
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 rgba(88, 166, 255, 0.15), stop:1 rgba(88, 166, 255, 0.05)); "
        "  border-radius: 12px; "
        "  border-left: 4px solid #388bfd; "
        "}"
    );
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();
    main->addLayout(headerLayout);


    auto* periodCard = new QWidget(this);
    periodCard->setObjectName("periodCard");
    periodCard->setStyleSheet(
        "QWidget#periodCard { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #161b22, stop:1 #0d1117); "
        "  border: 2px solid #21262d; "
        "  border-radius: 12px; "
        "  padding: 16px; "
        "}"
    );
    
    auto* periodLayout = new QHBoxLayout(periodCard);
    periodLayout->setContentsMargins(16, 12, 16, 12);
    periodLayout->setSpacing(12);
    
    auto* periodIcon = new QLabel(QString::fromUtf8("📅"), periodCard);
    QFont iconFont;
    iconFont.setPointSize(14);
    periodIcon->setFont(iconFont);
    
    auto* periodLabel = new QLabel(QString::fromUtf8("Период анализа:"), periodCard);
    QFont labelFont;
    labelFont.setPointSize(11);
    labelFont.setBold(true);
    periodLabel->setFont(labelFont);
    
    dateFrom = new QDateEdit(QDate::currentDate().addMonths(-1), periodCard);
    dateFrom->setCalendarPopup(true);
    dateFrom->setDisplayFormat("dd.MM.yyyy");
    dateFrom->setMinimumWidth(140);
    
    auto* separator = new QLabel(QString::fromUtf8("—"), periodCard);
    separator->setStyleSheet("color: #7d8590; font-size: 12pt;");
    
    dateTo = new QDateEdit(QDate::currentDate(), periodCard);
    dateTo->setCalendarPopup(true);
    dateTo->setDisplayFormat("dd.MM.yyyy");
    dateTo->setMinimumWidth(140);
    
    periodLayout->addWidget(periodIcon);
    periodLayout->addWidget(periodLabel);
    periodLayout->addWidget(dateFrom);
    periodLayout->addWidget(separator);
    periodLayout->addWidget(dateTo);
    periodLayout->addStretch();
    
    main->addWidget(periodCard);
    
    auto* charts = new QHBoxLayout();
    charts->setSpacing(20);
    
    incomeChart = new PieChartWidget(this);
    expenseChart = new PieChartWidget(this);
    incomeChart->setMinimumSize(500, 480);
    expenseChart->setMinimumSize(500, 480);
    

    QString chartStyle = 
        "PieChartWidget { "
        "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #161b22, stop:1 #0d1117); "
        "  border: 2px solid #30363d; "
        "  border-radius: 16px; "
        "  padding: 16px; "
        "}";
    
    incomeChart->setStyleSheet(chartStyle);
    expenseChart->setStyleSheet(chartStyle);
    
    charts->addWidget(incomeChart);
    charts->addWidget(expenseChart);
    main->addLayout(charts);
    
    auto* closeBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    auto* closeBtn = closeBox->button(QDialogButtonBox::Close);
    if (closeBtn) {
        closeBtn->setText(QString::fromUtf8("Закрыть"));
        closeBtn->setMinimumHeight(40);
        closeBtn->setMinimumWidth(120);
        closeBtn->setCursor(Qt::PointingHandCursor);
    }
    main->addWidget(closeBox);
    connect(closeBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(closeBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

    connect(dateFrom, &QDateEdit::dateChanged, this, [this](const QDate&){ recalc(); });
    connect(dateTo, &QDateEdit::dateChanged, this, [this](const QDate&){ recalc(); });


    setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, 
                stop:0 #161b22, 
                stop:1 #0d1117);
        }
        QLabel {
            color: #e6edf3;
            font-weight: 600;
            font-size: 10pt;
        }
        QDateEdit {
            background: #0d1117;
            color: #e6edf3;
            border: 2px solid #30363d;
            border-radius: 10px;
            padding: 10px 14px;
            font-size: 11pt;
            font-weight: 600;
            min-height: 24px;
        }
        QDateEdit:focus {
            border: 2px solid #388bfd;
            background: #161b22;
            box-shadow: 0 0 10px rgba(56, 139, 253, 0.3);
        }
        QDateEdit:hover {
            border: 2px solid #388bfd;
            background: #161b22;
        }
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #1f6feb, 
                stop:1 #388bfd);
            color: #ffffff;
            border: none;
            border-radius: 10px;
            padding: 12px 32px;
            font-weight: 700;
            font-size: 11pt;
            min-height: 36px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, 
                stop:0 #388bfd, 
                stop:1 #58a6ff);
            box-shadow: 0 0 20px rgba(88, 166, 255, 0.4);
        }
        QPushButton:pressed {
            background: #0969da;
        }
        QCalendarWidget QWidget {
            color: #e6edf3;
            background: #0d1117;
        }
        QCalendarWidget QAbstractItemView {
            background: #161b22;
            selection-background-color: #1f6feb;
            border: 1px solid #30363d;
        }
        QCalendarWidget QToolButton {
            background: transparent;
            color: #e6edf3;
            border: none;
            border-radius: 6px;
        }
        QCalendarWidget QToolButton:hover {
            background: #21262d;
        }
    )");

    recalc();
}

void StatsDialog::recalc() const {
    if (!transactionList) return;
    auto qFrom = dateFrom->date();
    auto qTo = dateTo->date();

    QMap<QString, double> incomeAgg;
    QMap<QString, double> expenseAgg;
    

    if (qFrom <= qTo) {
        auto all = transactionList->getAllTransactions();
        for (const auto& t : all) {
            Date d = t->getDate();
            QDate qd(d.getYear(), d.getMonth(), d.getDay());
            if (qd < qFrom || qTo < qd) continue;
            QString cat = QString::fromUtf8(t->getCategory().c_str());
            if (t->getType() == 1) {
                incomeAgg[cat] += t->getAmount();
            } else {
                expenseAgg[cat] += t->getAmount();
            }
        }
    }

    incomeChart->setData(incomeAgg, QString::fromUtf8("Доходы по категориям"));
    expenseChart->setData(expenseAgg, QString::fromUtf8("Расходы по категориям"));
}





