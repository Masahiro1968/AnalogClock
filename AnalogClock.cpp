// Copyright (C) 2016 The Qt Company Ltd.
// Copyright (C) 2026 Masahiro1968.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "AnalogClock.h"

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update));
    timer->start(40);

    setWindowTitle(tr("Analog Clock"));
    resize(m_startW, m_startH);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void AnalogClock::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void AnalogClock::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - m_dragPosition);
        event->accept();
    }
}

void AnalogClock::wheelEvent(QWheelEvent *event)
{
    int delta = event->angleDelta().y() > 0 ? 10 : -10;
    int newSide = qMax(100, width() + delta);
    resize(newSide, newSide);
}

void AnalogClock::mouseDoubleClickEvent(QMouseEvent *event)
{
    // future implement.
}

void AnalogClock::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    if (m_isStopwatchMode && m_isRunning) {
        menu.addAction("Stop", this, &AnalogClock::stopStopwatch);
    } else {
        menu.addAction("Start", this, &AnalogClock::startStopwatch);
    }
    menu.addAction("Reset", this, &AnalogClock::resetStopwatch);
    menu.addSeparator();
    menu.addAction("Quit", qApp, &QCoreApplication::quit);
    menu.exec(event->globalPos());
}

QTime AnalogClock::displayTime() const
{
    if (m_isStopwatchMode) {
        return m_stopwatchElapsed;
    }
    return QTime::currentTime();
}

void AnalogClock::startStopwatch()
{
    if (!m_isStopwatchMode)
        m_stopwatchElapsed = QTime(0, 0);

    m_isStopwatchMode = true;
    m_isRunning = true;
    m_elapsedTimer.start();

    update();
}

void AnalogClock::stopStopwatch()
{
    m_isRunning = false;
}

void AnalogClock::resetStopwatch()
{
    m_isRunning = false;
    m_isStopwatchMode = false;
    m_stopwatchElapsed = QTime(0, 0);
    update();
}

void AnalogClock::paintEvent(QPaintEvent *)
{
    if (m_isStopwatchMode && m_isRunning) {
        qint64 ms = m_elapsedTimer.restart();
        m_stopwatchElapsed = m_stopwatchElapsed.addMSecs(ms);
    }

    m_currentTime = displayTime();

    int w = width();
    int h = height();
    int side = qMin(w, h);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(w / 2, h / 2);
    painter.scale(side / m_startW, side / m_startH);

    drawSubDial1(painter);
    drawOuterCircle(painter);
    drawMinuteMarker(painter);
    drawDigital(painter);
    drawDateWeek(painter);
    drawHourCharacter(painter);
    drawHourMarker(painter);
    drawHourHand(painter);
    drawMinuteHand(painter);
    drawSecondHand(painter);
}

void AnalogClock::drawSubDial1(QPainter &painter)
{
    QPainterStateGuard subGuard(&painter);

    // --- Move origin and set scale ---
    painter.translate(36, -36);
    painter.scale(0.25, 0.25);

    // --- outer circle ---
    painter.setPen(QPen(m_minuteColor, 2));
    painter.setBrush(Qt::NoBrush);
    QPointF center = QPointF(0.0, 0.0);
    int r1 = (m_startH / 2) - 2;
    painter.drawEllipse(center, r1, r1);

    // --- outer circle shadow ---
    painter.setPen(QPen(QColor(180, 180, 180), 2));
    int r2 = (m_startH / 2) - 3;
    painter.drawEllipse(center, r2, r2);

    // --- subdial's number ---
    int w = 40;
    int h = 40;
    painter.setPen(m_minuteColor);
    painter.setFont(QFont("Arial", 18, QFont::Bold));

    QRect rect12(-20, -88, w, h);
    painter.drawText(rect12, Qt::AlignCenter, "12");

    QRect rect06(-20, 48, w, h);
    painter.drawText(rect06, Qt::AlignCenter, "06");

    QRect rect03(42, -20, w, h);
    painter.drawText(rect03, Qt::AlignCenter, "03");

    QRect rect09(-84, -20, w, h);
    painter.drawText(rect09, Qt::AlignCenter, "09");

    // --- debug ---
    // painter.setPen(QPen(m_minuteColor, 1, Qt::DashLine));
    // painter.setBrush(Qt::NoBrush);
    // painter.drawRect(rect12);
    // painter.drawRect(rect06);
    // painter.drawRect(rect03);
    // painter.drawRect(rect09);

    // --- scale ---
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_minuteColor);
    for (int i = 0; i < 12; ++i) {
        painter.drawRect(80, -3, 15, 6);
        painter.rotate(30.0);
    }

    // Subdial#1 always shows current time.
    QTime currentTime = QTime::currentTime();

    // --- hour hand ---
    {
        QPainterStateGuard handGuard(&painter);
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_hourColor);
        painter.rotate(30.0 * (currentTime.hour() % 12 + currentTime.minute() / 60.0));
        painter.drawConvexPolygon(m_hourHand, 4);
    }

    // ---  minute hand ---
    if (m_isStopwatchMode) {
        QPainterStateGuard handGuard(&painter);
        painter.setPen(QPen(m_minuteColor, 2));
        painter.rotate(6.0 * currentTime.minute());
        painter.drawConvexPolygon(m_minuteHand, 4);
    }
}

void AnalogClock::drawOuterCircle(QPainter &painter)
{
    painter.setPen(QPen(m_hourColor, 1));
    QPointF center = QPointF(0.0, 0.0);
    int r2 = (m_startH / 2) - 1;
    painter.drawEllipse(center, r2, r2);
}

void AnalogClock::drawHourCharacter(QPainter &painter)
{
    QPainterStateGuard numGuard(&painter);

    painter.setPen(m_hourColor);
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    int w = 22;
    int h = 22;

    QRect rect00 = QRect(-11, 62, w, h);
    painter.drawText(rect00, Qt::AlignCenter, m_isStopwatchMode ? "30" : "00");

    QRect rect12 = QRect(-11, -84, w, h);
    painter.drawText(rect12, Qt::AlignCenter, m_isStopwatchMode ? "00" : "12");

    QRect rect18 = QRect(60, -11, w, h);
    painter.drawText(rect18, Qt::AlignCenter, m_isStopwatchMode ? "15" : "18");

    QRect rect06 = QRect(-82, -11, w, h);
    painter.drawText(rect06, Qt::AlignCenter, m_isStopwatchMode ? "45" : "06");

    // --- debug ---
    // painter.setPen(QPen(m_hourColor, 1, Qt::DashLine));
    // painter.setBrush(Qt::NoBrush);
    // painter.drawRect(rect00);
    // painter.drawRect(rect12);
    // painter.drawRect(rect18);
    // painter.drawRect(rect06);
}

void AnalogClock::drawHourMarker(QPainter &painter)
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_hourColor);

    if (m_isStopwatchMode) {
        for (int i = 0; i < 12; ++i) {
            painter.drawRect(80, -2, 10, 4);
            painter.rotate(30.0);
        }
    } else {
        for (int i = 0; i < 24; ++i) {
            painter.drawRect(80, -2, 10, 4);
            painter.rotate(15.0);
        }
    }
}

void AnalogClock::drawHourHand(QPainter &painter)
{
    QPainterStateGuard guard(&painter);

    painter.setPen(QPen(m_hourColor, 2));
    painter.setBrush(Qt::NoBrush);

    if (m_isStopwatchMode) {
        double preciseHours = (m_currentTime.hour() % 12) + (m_currentTime.minute() / 60.0)
                              + (m_currentTime.second() / 3600.0);
        painter.rotate(30.0 * preciseHours);
        painter.drawConvexPolygon(m_hourHand, 4);
    } else {
        painter.rotate(180.0);
        double preciseHours = m_currentTime.hour() + (m_currentTime.minute() / 60.0)
                              + (m_currentTime.second() / 3600.0);
        painter.rotate(15.0 * preciseHours);
        painter.drawConvexPolygon(m_hourHand, 4);
    }
}

void AnalogClock::drawMinuteHand(QPainter &painter)
{
    QPainterStateGuard guard(&painter);

    painter.setPen(QPen(m_minuteColor, 2));
    painter.setBrush(Qt::NoBrush);
    double preciseMinutes = m_currentTime.minute() + (m_currentTime.second() / 60.0);
    painter.rotate(6.0 * preciseMinutes);
    painter.drawConvexPolygon(m_minuteHand, 4);
}

void AnalogClock::drawSecondHand(QPainter &painter)
{
    QPainterStateGuard guard(&painter);

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_secondsColor);
    double preciseSeconds = m_currentTime.second() + (m_currentTime.msec() / 1000.0);
    painter.rotate(6.0 * preciseSeconds);
    painter.drawConvexPolygon(m_secondsHand, 4);
    painter.drawEllipse(-3, -3, 6, 6);
    painter.drawEllipse(-5, -68, 10, 10);
}

void AnalogClock::drawMinuteMarker(QPainter &painter)
{
    for (int j = 0; j < 60; ++j) {
        if (j % 5 == 0)
            painter.setPen(QPen(m_minuteColor, 2));
        else
            painter.setPen(QPen(m_minuteColor, 1));
        painter.drawLine(92, 0, 98, 0);
        painter.rotate(6.0);
    }
}

void AnalogClock::drawDigital(QPainter &painter)
{
    QPainterStateGuard digitalGuard(&painter);

    painter.translate(0, 45);
    painter.setPen(m_secondsColor);

    QRect digitalRect(-30, -10, 60, 20);
    QFont digitalFont("Monospace");
    digitalFont.setPixelSize(12);
    digitalFont.setBold(true);
    painter.setFont(digitalFont);

    int second = m_currentTime.second();
    QString timeText = second % 2 == 0 ? m_currentTime.toString("HH:mm")
                                       : m_currentTime.toString("HH mm");
    painter.drawText(digitalRect, Qt::AlignCenter, timeText);
    // painter.drawRect(digitalRect);
}

void AnalogClock::drawDateWeek(QPainter &painter)
{
    QPainterStateGuard digitalGuard(&painter);

    painter.translate(42, 0);
    painter.setPen(QPen(m_hourColor, 1));
    painter.setBrush(QColor(180, 180, 180));

    QRect digitalRect(-20, -10, 40, 18);
    painter.drawRect(digitalRect);
    QFont digitalFont("Monospace");
    digitalFont.setPixelSize(12);
    digitalFont.setBold(true);
    painter.setFont(digitalFont);

    QString dateText = QDate::currentDate().toString("dd ddd");
    painter.drawText(digitalRect, Qt::AlignCenter, dateText);
}
