// Copyright (C) 2016 The Qt Company Ltd.
// Copyright (C) 2026 Masahiro1968.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "AnalogClock.h"

AnalogClock::AnalogClock(QWidget *parent)
    : QWidget(parent)
    , m_colorPattern(0)
    , m_hourColor(palette().color(QPalette::Text))
    , m_minuteColor(palette().color(QPalette::Text))
    , m_secondsColor(palette().color(QPalette::Accent))
    , m_hourHandOnly(false)
    , m_showMinuteMarkers(true)
    , m_showHourMarkers(true)
    , m_showHourNumbers(true)
    , m_showOuterCircle(true)
    , m_showSubDial(true)
    , m_showDigital(true)
    , m_showDate(true)
{
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&AnalogClock::update));
    timer->start(40);

    setWindowTitle(tr("Analog Clock"));
    resize(m_startW, m_startH);
    loadPreference();

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
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
    painter.fillRect(rect(), QColor(0, 0, 0, 1));
    painter.translate(w / 2, h / 2);
    painter.scale(side / m_startW, side / m_startH);

    if (m_showSubDial)
        drawSubDial1(painter);

    int currentRadius = 99;

    if (m_showOuterCircle) {
        drawOuterCircle(painter, currentRadius);
        currentRadius -= 2;
    }

    if (m_showMinuteMarkers) {
        drawMinuteMarker(painter, currentRadius);
        currentRadius -= 6;
    }

    if (m_showHourMarkers) {
        drawHourMarker(painter, currentRadius);
        currentRadius -= 10;
    }

    if (m_showDigital)
        drawDigital(painter);

    if (m_showDate)
        drawDateWeek(painter);

    if (m_showHourNumbers)
        drawHourCharacter(painter, currentRadius);

    drawHourHand(painter);

    if (!m_hourHandOnly) {
        drawMinuteHand(painter);
        drawSecondHand(painter);
    }
}

void AnalogClock::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (auto handle = windowHandle()) {
            handle->startSystemMove();
            event->accept();
        }
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
    event->accept();
}

void AnalogClock::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);

    if (m_isStopwatchMode) {
        if (m_isRunning) {
            menu.addAction("Stop", this, &AnalogClock::stopStopwatch);
        } else {
            menu.addAction("Start", this, &AnalogClock::startStopwatch);
        }
        menu.addAction("Reset", this, &AnalogClock::resetStopwatch);
        menu.addSeparator();
        menu.addAction("Return to Clock", this, &AnalogClock::switchToClockMode);
    } else {
        menu.addAction("Stopwatch Mode", this, &AnalogClock::startStopwatch);
    }

    menu.addSeparator();

    // ★ オプションを細かく管理できるサブメニューを定義
    QMenu *optionsMenu = menu.addMenu("Customization Options");

    QAction *actHourOnly = optionsMenu->addAction("Hour Hand Only (短針のみ表示)", this, [this]() {
        m_hourHandOnly = !m_hourHandOnly;
        update();
    });
    actHourOnly->setCheckable(true);
    actHourOnly->setChecked(m_hourHandOnly);

    QAction *actMinMarkers = optionsMenu->addAction("Minute Ticks (分目盛り表示)", this, [this]() {
        m_showMinuteMarkers = !m_showMinuteMarkers;
        update();
    });
    actMinMarkers->setCheckable(true);
    actMinMarkers->setChecked(m_showMinuteMarkers);

    QAction *actHourMarkers = optionsMenu->addAction("Hour Markers (時目盛り表示)", this, [this]() {
        m_showHourMarkers = !m_showHourMarkers;
        update();
    });
    actHourMarkers->setCheckable(true);
    actHourMarkers->setChecked(m_showHourMarkers);

    QAction *actHourNumbers = optionsMenu->addAction("Hour Numbers (数字表示)", this, [this]() {
        m_showHourNumbers = !m_showHourNumbers;
        update();
    });
    actHourNumbers->setCheckable(true);
    actHourNumbers->setChecked(m_showHourNumbers);

    QAction *actOuterCircle = optionsMenu->addAction("Outer Border (外周枠表示)", this, [this]() {
        m_showOuterCircle = !m_showOuterCircle;
        update();
    });
    actOuterCircle->setCheckable(true);
    actOuterCircle->setChecked(m_showOuterCircle);

    QAction *actSubdial = optionsMenu->addAction("Subdial (サブダイヤル表示)", this, [this]() {
        m_showSubDial = !m_showSubDial;
        update();
    });
    actSubdial->setCheckable(true);
    actSubdial->setChecked(m_showSubDial);

    QAction *actDigital = optionsMenu->addAction("Digital Panel (デジタル時刻表示)", this, [this]() {
        m_showDigital = !m_showDigital;
        update();
    });
    actDigital->setCheckable(true);
    actDigital->setChecked(m_showDigital);

    QAction *actDate = optionsMenu->addAction("Date Display (日付表示)", this, [this]() {
        m_showDate = !m_showDate;
        update();
    });
    actDate->setCheckable(true);
    actDate->setChecked(m_showDate);

    menu.addSeparator();

    QMenu *themeOptionsMenu = menu.addMenu("Theme Options");

    QAction *lightTheme = themeOptionsMenu->addAction("Light Theme", this, [this]() {
        changeColor(1);
    });
    lightTheme->setCheckable(true);
    lightTheme->setChecked(m_colorPattern == 1 ? true : false);

    QAction *darkTheme = themeOptionsMenu->addAction("Dark Theme", this, [this]() {
        changeColor(2);
    });
    darkTheme->setCheckable(true);
    darkTheme->setChecked(m_colorPattern == 2 ? true : false);

    QAction *blackTheme = themeOptionsMenu->addAction("Black Theme", this, [this]() {
        changeColor(0);
    });
    blackTheme->setCheckable(true);
    blackTheme->setChecked(m_colorPattern == 0 ? true : false);

    menu.addSeparator();

    menu.addAction("Quit", qApp, &QCoreApplication::quit);
    menu.exec(event->globalPos());
}

void AnalogClock::closeEvent(QCloseEvent *event)
{
    savePreference();
    event->accept();
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
    m_stopwatchElapsed = QTime(0, 0);
    update();
}

void AnalogClock::switchToClockMode()
{
    m_isRunning = false;
    m_isStopwatchMode = false;
    update();
}

void AnalogClock::changeColor(int pattern)
{
    m_colorPattern = pattern;

    if (m_colorPattern == 1) {
        // Light theme
        m_hourColor = palette().color(QPalette::Light);
        m_minuteColor = palette().color(QPalette::Light);
        m_secondsColor = palette().color(QPalette::Accent);
    } else if (m_colorPattern == 2) {
        // Dark theme
        m_hourColor = palette().color(QPalette::Dark);
        m_minuteColor = palette().color(QPalette::Dark);
        m_secondsColor = palette().color(QPalette::Accent);
    } else {
        // Black theme
        m_hourColor = palette().color(QPalette::Text);
        m_minuteColor = palette().color(QPalette::Text);
        m_secondsColor = palette().color(QPalette::Accent);
    }
    update();
}

void AnalogClock::loadPreference()
{
#ifdef Q_OS_LINUX
    QSettings settings(OWNER_NAME, APP_NAME);
#else
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, OWNER_NAME, APP_NAME);
#endif

    if (settings.contains("windowPos")) {
        move(settings.value("windowPos").toPoint());
        resize(settings.value("windowSize").toSize());
    }

    int savedPattern = settings.value("colorPattern", 0).toInt();
    changeColor(savedPattern);

    // ★ オプション値の読み込み (デフォルト設定を考慮してフォールバックを用意)
    m_hourHandOnly      = settings.value("hourHandOnly", false).toBool();
    m_showMinuteMarkers = settings.value("showMinuteMarkers", true).toBool();
    m_showHourMarkers   = settings.value("showHourMarkers", true).toBool();
    m_showHourNumbers   = settings.value("showHourNumbers", true).toBool();
    m_showOuterCircle   = settings.value("showOuterCircle", true).toBool();
    m_showSubDial       = settings.value("showSubDial", true).toBool();
    m_showDigital       = settings.value("showDigital", true).toBool();
    m_showDate          = settings.value("showDate", true).toBool();
}

void AnalogClock::savePreference()
{
#ifdef Q_OS_LINUX
    QSettings settings(OWNER_NAME, APP_NAME);
#else
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, OWNER_NAME, APP_NAME);
#endif

    settings.setValue("windowPos", pos());
    settings.setValue("windowSize", size());
    settings.setValue("colorPattern", m_colorPattern);

    // ★ オプション値の永続化
    settings.setValue("hourHandOnly", m_hourHandOnly);
    settings.setValue("showMinuteMarkers", m_showMinuteMarkers);
    settings.setValue("showHourMarkers", m_showHourMarkers);
    settings.setValue("showHourNumbers", m_showHourNumbers);
    settings.setValue("showOuterCircle", m_showOuterCircle);
    settings.setValue("showSubDial", m_showSubDial);
    settings.setValue("showDigital", m_showDigital);
    settings.setValue("showDate", m_showDate);

    settings.sync();
}

void AnalogClock::drawSubDial1(QPainter &painter)
{
    painter.save();

    painter.translate(36, -36);
    painter.scale(0.25, 0.25);

    painter.setPen(QPen(m_minuteColor, 2));
    painter.setBrush(Qt::NoBrush);
    QPointF center = QPointF(0.0, 0.0);
    int r1 = (m_startH / 2) - 2;
    painter.drawEllipse(center, r1, r1);

    painter.setPen(QPen(QColor(180, 180, 180), 2));
    int r2 = (m_startH / 2) - 3;
    painter.drawEllipse(center, r2, r2);

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

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_minuteColor);
    for (int i = 0; i < 12; ++i) {
        painter.drawRect(80, -3, 15, 6);
        painter.rotate(30.0);
    }

    QTime currentTime = QTime::currentTime();

    {
        painter.save();
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_hourColor);
        painter.rotate(30.0 * (currentTime.hour() % 12 + currentTime.minute() / 60.0));
        painter.drawConvexPolygon(m_hourHand, 4);
        painter.restore();
    }

    {
        painter.save();
        painter.setPen(QPen(m_minuteColor, 2));
        painter.rotate(6.0 * currentTime.minute());
        painter.drawConvexPolygon(m_minuteHand, 4);
        painter.restore();
    }

    painter.restore();
}

void AnalogClock::drawOuterCircle(QPainter &painter, int radius)
{
    painter.save();
    painter.setPen(QPen(m_hourColor, 1));
    painter.drawEllipse(QPointF(0.0, 0.0), radius, radius);
    painter.restore();
}

void AnalogClock::drawMinuteMarker(QPainter &painter, int radius)
{
    painter.save();

    int outer = radius;
    int inner = radius - 6;

    for (int j = 0; j < 60; ++j) {
        if (j % 5 == 0)
            painter.setPen(QPen(m_minuteColor, 2));
        else
            painter.setPen(QPen(m_minuteColor, 1));

        painter.drawLine(inner, 0, outer, 0);
        painter.rotate(6.0);
    }

    painter.restore();
}

void AnalogClock::drawDigital(QPainter &painter)
{
    painter.save();

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

    painter.restore();
}

void AnalogClock::drawDateWeek(QPainter &painter)
{
    painter.save();

    QFont digitalFont("Monospace");
    digitalFont.setPixelSize(11);
    digitalFont.setBold(true);
    painter.setFont(digitalFont);

    QString dateText = QDate::currentDate().toString("dd ddd");
    int textWidth = painter.fontMetrics().horizontalAdvance(dateText);
    int textHeight = 18;

    painter.translate(38, 0);

    painter.setPen(QPen(m_hourColor, 1));
    painter.setBrush(QColor(180, 180, 180, 200));

    QRect digitalRect(-(textWidth / 2 + 3), -(textHeight / 2), textWidth + 6, textHeight);
    painter.drawRect(digitalRect);

    painter.setPen(m_hourColor);
    painter.drawText(digitalRect, Qt::AlignCenter, dateText);

    painter.restore();
}

void AnalogClock::drawHourCharacter(QPainter &painter, int radius)
{
    painter.save();
    painter.setPen(m_hourColor);

    // ★ 半径（残されたスペース）に応じて、フォントサイズと配置位置を動的に決定
    int fontSize = 12;
    int textMargin = 12; // 目盛りからどれくらい内側に離すか

    if (radius >= 95) {
        // 外枠や目盛りがほとんどない（大画面・超ミニマルモード）
        fontSize = 18;
        textMargin = 16;
    } else if (radius >= 87) {
        // 目盛りが1種類だけないなど、少しスペースがある
        fontSize = 15;
        textMargin = 14;
    } else {
        // すべて表示されている（標準モード）
        fontSize = 12;
        textMargin = 12;
    }

    // 動的に決まったサイズでフォントを設定
    painter.setFont(QFont("Arial", fontSize, QFont::Bold));

    // フォントが大きくなる可能性を考慮して、描画エリア（幅と高さ）も動的に調整
    int w = fontSize + 12;
    int h = fontSize + 12;
    int halfW = w / 2;
    int halfH = h / 2;

    // 割り当てられた半径から、文字の描画中心位置（オフセット）を計算
    int offset = radius - textMargin;

    // 各方向の文字位置を計算
    QRect rect00 = QRect(-halfW, offset - halfH, w, h);  // 下 (00 / 30)
    QRect rect12 = QRect(-halfW, -offset - halfH, w, h); // 上 (12 / 00)
    QRect rect18 = QRect(offset - halfW, -halfH, w, h);  // 右 (18 / 15)
    QRect rect06 = QRect(-offset - halfW, -halfH, w, h); // 左 (06 / 45)

    painter.drawText(rect00, Qt::AlignCenter, m_isStopwatchMode ? "30" : "00");
    painter.drawText(rect12, Qt::AlignCenter, m_isStopwatchMode ? "00" : "12");
    painter.drawText(rect18, Qt::AlignCenter, m_isStopwatchMode ? "15" : "18");
    painter.drawText(rect06, Qt::AlignCenter, m_isStopwatchMode ? "45" : "06");

    painter.restore();
}

void AnalogClock::drawHourMarker(QPainter &painter, int radius)
{
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_hourColor);

    int startX = radius - 10;

    if (m_isStopwatchMode) {
        for (int i = 0; i < 12; ++i) {
            painter.drawRect(startX, -2, 10, 4);
            painter.rotate(30.0);
        }
    } else {
        for (int i = 0; i < 24; ++i) {
            painter.drawRect(startX, -2, 10, 4);
            painter.rotate(15.0);
        }
    }
    painter.restore();
}

void AnalogClock::drawHourHand(QPainter &painter)
{
    painter.save();

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

    painter.restore();
}

void AnalogClock::drawMinuteHand(QPainter &painter)
{
    painter.save();

    painter.setPen(QPen(m_minuteColor, 2));
    painter.setBrush(Qt::NoBrush);
    double preciseMinutes = m_currentTime.minute() + (m_currentTime.second() / 60.0);
    painter.rotate(6.0 * preciseMinutes);
    painter.drawConvexPolygon(m_minuteHand, 4);

    painter.restore();
}

void AnalogClock::drawSecondHand(QPainter &painter)
{
    painter.save();

    painter.setPen(Qt::NoPen);
    painter.setBrush(m_secondsColor);
    double preciseSeconds = m_currentTime.second() + (m_currentTime.msec() / 1000.0);
    painter.rotate(6.0 * preciseSeconds);
    painter.drawConvexPolygon(m_secondsHand, 4);
    painter.drawEllipse(-3, -3, 6, 6);
    painter.drawEllipse(-5, -68, 10, 10);

    painter.restore();
}
