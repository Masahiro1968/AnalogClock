// Copyright (C) 2016 The Qt Company Ltd.
// Copyright (C) 2026 Masahiro1968.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#pragma once

#include <QElapsedTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterStateGuard>
#include <QTime>
#include <QTimer>
#include <QWidget>

class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    AnalogClock(QWidget *parent = nullptr);

protected:
    QTime displayTime() const;
    void stopStopwatch();
    void startStopwatch();
    void resetStopwatch();

    void drawSubDial1(QPainter &painter);
    void drawOuterCircle(QPainter &painter);
    void drawHourHand(QPainter &painter);
    void drawHourMarker(QPainter &painter);
    void drawHourCharacter(QPainter &painter);
    void drawMinuteHand(QPainter &painter);
    void drawSecondHand(QPainter &painter);
    void drawMinuteMarker(QPainter &painter);
    void drawDigital(QPainter &painter);
    void drawDateWeek(QPainter &painter);

    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    const QPoint m_hourHand[4] = {QPoint(5, 14), QPoint(-5, 14), QPoint(-4, -71), QPoint(4, -71)};
    const QPoint m_minuteHand[4] = {QPoint(4, 14), QPoint(-4, 14), QPoint(-3, -89), QPoint(3, -89)};
    const QPoint m_secondsHand[4] = {QPoint(1, 14), QPoint(-1, 14), QPoint(-1, -89), QPoint(1, -89)};

    const QColor m_hourColor = palette().color(QPalette::Text);
    const QColor m_minuteColor = palette().color(QPalette::Text);
    const QColor m_secondsColor = palette().color(QPalette::Accent);

    const double m_startW = 200.0;
    const double m_startH = 200.0;

    QPoint m_dragPosition;
    QTime m_currentTime;
    QTime m_stopwatchElapsed;
    QElapsedTimer m_elapsedTimer;
    bool m_isStopwatchMode = false;
    bool m_isRunning = false;
};
