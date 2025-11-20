#pragma once

#include <QDialog>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPainter>
#include <QMap>
#include <QPushButton>
#include "TransactionList.h"

#include <algorithm>

class PieChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit PieChartWidget(QWidget* parent = nullptr) : QWidget(parent) {}

    void setData(const QMap<QString, double>& categoryToValue, const QString& title) {
        data = categoryToValue;
        chartTitle = title;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QWidget::paintEvent(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);

        QRect rect = this->rect().adjusted(20, 20, -20, -20);

        QFont f = p.font();
        f.setBold(true);
        f.setPointSize(13);
        p.setFont(f);
        p.setPen(QColor(88,166,255));
        p.drawText(rect.left(), rect.top(), rect.width(), 30, Qt::AlignLeft | Qt::AlignVCenter, chartTitle);

        auto pieRect = QRect(rect.left(), rect.top() + 45, rect.height() - 50, rect.height() - 50);
        if (pieRect.width() > rect.width()/2) {
            int size = qMin(rect.width()/2 - 20, pieRect.height());
            pieRect.setWidth(size);
            pieRect.setHeight(size);
        }

        double total = 0.0;
        for (auto it = data.constBegin(); it != data.constEnd(); ++it) total += it.value();
        if (total <= 0.0) {
            p.setPen(QColor(139,148,158));
            QFont emptyFont = p.font();
            emptyFont.setPointSize(12);
            emptyFont.setBold(false);
            p.setFont(emptyFont);
            p.drawText(rect, Qt::AlignCenter, QString::fromUtf8("📭 Нет данных за выбранный период"));
            return;
        }


        QList colors = {
            QColor(88,166,255),
            QColor(121,192,255),
            QColor(248,81,73),
            QColor(255,123,114),
            QColor(63,185,80),
            QColor(87,242,135),
            QColor(255,171,64),
            QColor(255,203,107),
            QColor(187,128,255),
            QColor(214,175,255)
        };

        int colorIdx = 0;
        int startAngle = 0;
        QList<QPair<QString,double>> items;
        for (auto it = data.constBegin(); it != data.constEnd(); ++it) items.append({it.key(), it.value()});
        std::sort(items.begin(), items.end(), [](const auto& a, const auto& b){ return a.second > b.second; });

        for (const auto& [category, value] : items) {
            auto span = static_cast<int>((value / total) * 360.0 * 16);
            QColor c = colors[colorIdx % colors.size()];
            p.setBrush(c);
            p.setPen(QPen(QColor(33,38,45), 2));
            p.drawPie(pieRect, startAngle, span);
            startAngle += span;
            colorIdx++;
        }


        int legendX = pieRect.right() + 30;
        int legendY = pieRect.top();
        colorIdx = 0;
        QFont legendFont = p.font();
        legendFont.setPointSize(10);
        legendFont.setBold(false);
        p.setFont(legendFont);
        
        for (const auto& [category, value] : items) {
            QColor c = colors[colorIdx % colors.size()];
            p.setBrush(c);
            p.setPen(Qt::NoPen);
            QRect box(legendX, legendY, 18, 18);
            p.drawRoundedRect(box, 5, 5);
            
            p.setPen(QColor(230,237,243));
            auto percent = (value / total) * 100.0;
            QString text = QString("%1 — %2 руб. (%3%)").arg(category)
                .arg(QString::number(value, 'f', 2))
                .arg(QString::number(percent, 'f', 1));
            p.drawText(legendX + 28, legendY + 14, text);
            legendY += 28;
            colorIdx++;
        }
    }

private:
    QMap<QString, double> data;
    QString chartTitle;
};

class StatsDialog : public QDialog {
    Q_OBJECT
public:
    explicit StatsDialog(const TransactionList* list, QWidget* parent = nullptr);
    ~StatsDialog() override = default;

private:
    const TransactionList* transactionList;
    QDateEdit* dateFrom;
    QDateEdit* dateTo;
    PieChartWidget* incomeChart;
    PieChartWidget* expenseChart;

    void recalc() const;
};





