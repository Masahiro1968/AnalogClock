// Copyright (C) 2016 The Qt Company Ltd.
// Copyright (C) 2026 Masahiro1968.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#pragma once

#include <QElapsedTimer>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QSettings>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <QWindow>

#define OWNER_NAME "Masahiro1968"
#define APP_NAME "AnalogClock"

class AnalogClock : public QWidget
{
    Q_OBJECT

public:
    AnalogClock(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    QTime displayTime() const;
    void startStopwatch();
    void stopStopwatch();
    void resetStopwatch();
    void switchToClockMode();
    void reverseColor(int pattern = -1);
    void loadPreference();
    void savePreference();

    void drawSubDial1(QPainter &painter);
    void drawOuterCircle(QPainter &painter);
    void drawMinuteMarker(QPainter &painter);
    void drawDigital(QPainter &painter);
    void drawDateWeek(QPainter &painter);
    void drawHourCharacter(QPainter &painter);
    void drawHourMarker(QPainter &painter);
    void drawHourHand(QPainter &painter);
    void drawMinuteHand(QPainter &painter);
    void drawSecondHand(QPainter &painter);

private:
    const QPoint m_hourHand[4] = {QPoint(5, 14), QPoint(-5, 14), QPoint(-4, -71), QPoint(4, -71)};
    const QPoint m_minuteHand[4] = {QPoint(4, 14), QPoint(-4, 14), QPoint(-3, -89), QPoint(3, -89)};
    const QPoint m_secondsHand[4] = {QPoint(1, 14), QPoint(-1, 14), QPoint(-1, -89), QPoint(1, -89)};

    QColor m_hourColor;
    QColor m_minuteColor;
    QColor m_secondsColor;
    int m_colorPattern;

    const double m_startW = 200.0;
    const double m_startH = 200.0;

    QPoint m_dragPosition;
    QTime m_currentTime;
    QTime m_stopwatchElapsed;
    QElapsedTimer m_elapsedTimer;
    bool m_isStopwatchMode = false;
    bool m_isRunning = false;
};
