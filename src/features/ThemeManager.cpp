/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ThemeManager.h"
#include "ApplicationSettings.h"
#include "dialogs/MainWindow.h"
#include "ScintillaNext.h"

#include <QGuiApplication>
#include <QStyleHints>
#include <QPalette>
#include <QColor>

ThemeManager *ThemeManager::instance()
{
    static ThemeManager s_instance;
    return &s_instance;
}

ThemeManager::ThemeManager(QObject *parent)
    : QObject(parent)
{
    if (QGuiApplication::styleHints()) {
        connect(QGuiApplication::styleHints(), &QStyleHints::colorSchemeChanged,
                this, &ThemeManager::onSystemColorSchemeChanged);
    }
}

ThemeManager::~ThemeManager()
{
}

void ThemeManager::initialize(ApplicationSettings *settings)
{
    m_settings = settings;
    if (m_settings) {
        QString savedMode = m_settings->value(QStringLiteral("App/ThemeMode"), QStringLiteral("Light")).toString().toLower();
        if (savedMode == QStringLiteral("dark")) {
            m_themeMode = ThemeMode::Dark;
        } else if (savedMode == QStringLiteral("auto")) {
            m_themeMode = ThemeMode::Auto;
        } else {
            m_themeMode = ThemeMode::Light;
        }

        QString savedPalette = m_settings->value(QStringLiteral("App/ThemePalette"), QString()).toString();
        if (!savedPalette.isEmpty() && findThemePaletteById(savedPalette)) {
            m_currentPaletteId = savedPalette;
        } else {
            m_currentPaletteId = (m_themeMode == ThemeMode::Dark) ? QStringLiteral("catppuccinMocha") : QStringLiteral("defaultLight");
        }
    }
}

bool ThemeManager::isDarkActive() const
{
    if (m_themeMode == ThemeMode::Dark)
        return true;
    if (m_themeMode == ThemeMode::Light)
        return false;

    // Auto: Follow system color scheme
    if (QGuiApplication::styleHints()) {
        return QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
    }
    return QGuiApplication::palette().color(QPalette::Window).lightness() < 128;
}

const ThemePalette *ThemeManager::currentPalette() const
{
    if (!m_currentPaletteId.isEmpty()) {
        const ThemePalette *p = findThemePaletteById(m_currentPaletteId);
        if (p)
            return p;
    }
    return isDarkActive() ? findThemePaletteById(QStringLiteral("catppuccinMocha"))
                          : findThemePaletteById(QStringLiteral("githubLight"));
}

void ThemeManager::setThemeMode(ThemeMode mode, MainWindow *window)
{
    m_themeMode = mode;
    if (window) {
        m_activeWindow = window;
    }

    if (m_settings) {
        QString modeStr = QStringLiteral("Auto");
        if (mode == ThemeMode::Dark) modeStr = QStringLiteral("Dark");
        else if (mode == ThemeMode::Light) modeStr = QStringLiteral("Light");
        m_settings->setValue(QStringLiteral("App/ThemeMode"), modeStr);
    }

    if (m_activeWindow) {
        applyTheme(m_activeWindow);
    }

    emit themeChanged(isDarkActive());
}

void ThemeManager::setPalette(const QString &paletteId, MainWindow *window)
{
    const ThemePalette *p = findThemePaletteById(paletteId);
    if (!p)
        return;

    m_currentPaletteId = paletteId;
    if (p->isDark) {
        m_themeMode = ThemeMode::Dark;
    } else {
        m_themeMode = ThemeMode::Light;
    }

    if (window) {
        m_activeWindow = window;
    }

    if (m_settings) {
        m_settings->setValue(QStringLiteral("App/ThemePalette"), m_currentPaletteId);
        m_settings->setValue(QStringLiteral("App/ThemeMode"), p->isDark ? QStringLiteral("Dark") : QStringLiteral("Light"));
    }

    if (m_activeWindow) {
        applyTheme(m_activeWindow);
    }

    emit themeChanged(isDarkActive());
    emit paletteChanged(m_currentPaletteId);
}

void ThemeManager::toggleTheme(MainWindow *window)
{
    bool currentlyDark = isDarkActive();
    setThemeMode(currentlyDark ? ThemeMode::Light : ThemeMode::Dark, window);
}

void ThemeManager::onSystemColorSchemeChanged()
{
    if (m_themeMode == ThemeMode::Auto && m_activeWindow) {
        applyTheme(m_activeWindow);
        emit themeChanged(isDarkActive());
    }
}

void ThemeManager::applyTheme(MainWindow *window)
{
    m_activeWindow = window;
    if (!window)
        return;

    window->applyStyleSheet();

    for (ScintillaNext *editor : window->editors()) {
        applyThemeToEditor(editor);
    }
}

void ThemeManager::applyThemeToEditor(ScintillaNext *editor)
{
    if (!editor)
        return;

    const ThemePalette *pal = currentPalette();
    if (!pal)
        return;

    QColor card(pal->card);
    QColor border(pal->border);
    QColor txtPri(pal->txtPri);
    QColor txtSec(pal->txtSec);
    QColor cyan(pal->cyan);
    QColor blue(pal->blue);
    QColor net(pal->net);
    QColor sub(pal->sub);
    QColor host(pal->host);
    QColor sep(pal->sep);

    // Canvas background and primary text
    editor->styleSetFore(STYLE_DEFAULT, scFromRgb(txtPri.rgb()));
    editor->styleSetBack(STYLE_DEFAULT, scFromRgb(card.rgb()));
    editor->styleClearAll();

    // Line number margin
    editor->styleSetFore(STYLE_LINENUMBER, scFromRgb(txtSec.rgb()));
    editor->styleSetBack(STYLE_LINENUMBER, scFromRgb(border.rgb()));

    // Caret color
    editor->setCaretFore(scFromRgb(txtPri.rgb()));

    // Caret line highlight (slight blend)
    quint32 caretLineColor = 0xFF000000 | (pal->isDark ? scColor(qMin(card.red() + 18, 255), qMin(card.green() + 18, 255), qMin(card.blue() + 22, 255))
                                                       : scColor(qMax(card.red() - 12, 0), qMax(card.green() - 12, 0), qMax(card.blue() - 6, 0)));
    editor->setElementColour(SC_ELEMENT_CARET_LINE_BACK, caretLineColor);

    // Selection highlight
    quint32 selColor = 0xFF000000 | scFromRgb(border.rgb());
    editor->setElementColour(SC_ELEMENT_SELECTION_BACK, selColor);
    editor->setElementColour(SC_ELEMENT_SELECTION_INACTIVE_BACK, selColor);

    // Fold margin colors
    editor->setFoldMarginColour(true, scFromRgb(card.rgb()));
    editor->setFoldMarginHiColour(true, scFromRgb(border.rgb()));
    editor->setElementColour(SC_ELEMENT_FOLD_LINE, 0xFF000000 | scFromRgb(border.rgb()));

    // Matching and mismatching braces
    editor->styleSetFore(STYLE_BRACELIGHT, scFromRgb(cyan.rgb()));
    editor->styleSetBack(STYLE_BRACELIGHT, scFromRgb(border.rgb()));
    editor->styleSetFore(STYLE_BRACEBAD, scColor(207, 34, 46));
    editor->styleSetBack(STYLE_BRACEBAD, scFromRgb(border.rgb()));

    // Indentation guides and white space
    editor->styleSetFore(STYLE_INDENTGUIDE, scFromRgb(border.rgb()));
    editor->styleSetBack(STYLE_INDENTGUIDE, scFromRgb(card.rgb()));
    editor->setElementColour(SC_ELEMENT_WHITE_SPACE, 0xFF000000 | scFromRgb(border.rgb()));

    // Common Lexilla tokens styling (styles 1 through 15 across Lexilla standard lexers)
    // 1: Comment, 2: Comment line, 3: Comment doc
    editor->styleSetFore(1, scFromRgb(txtSec.rgb()));
    editor->styleSetItalic(1, true);
    editor->styleSetFore(2, scFromRgb(txtSec.rgb()));
    editor->styleSetItalic(2, true);
    editor->styleSetFore(3, scFromRgb(txtSec.rgb()));
    editor->styleSetItalic(3, true);

    // 4: Number
    editor->styleSetFore(4, scFromRgb(sub.rgb()));

    // 5: Keyword / Word
    editor->styleSetFore(5, scFromRgb(cyan.rgb()));
    editor->styleSetBold(5, true);

    // 6: String, 7: Character
    editor->styleSetFore(6, scFromRgb(host.rgb()));
    editor->styleSetFore(7, scFromRgb(host.rgb()));

    // 9: Preprocessor
    editor->styleSetFore(9, scFromRgb(net.rgb()));

    // 10: Operator
    editor->styleSetFore(10, scFromRgb(sep.rgb()));

    // 11: Identifier / Function
    editor->styleSetFore(11, scFromRgb(blue.rgb()));
}

QString ThemeManager::getThemeStyleSheet() const
{
    const ThemePalette *pal = currentPalette();
    if (!pal) {
        pal = isDarkActive() ? findThemePaletteById(QStringLiteral("catppuccinMocha"))
                             : findThemePaletteById(QStringLiteral("githubLight"));
    }

    QString win = pal ? QString::fromLatin1(pal->win) : (isDarkActive() ? QStringLiteral("#1e1e1e") : QStringLiteral("#f6f8fa"));
    QString card = pal ? QString::fromLatin1(pal->card) : (isDarkActive() ? QStringLiteral("#25282c") : QStringLiteral("#ffffff"));
    QString border = pal ? QString::fromLatin1(pal->border) : (isDarkActive() ? QStringLiteral("#33373e") : QStringLiteral("#d0d7de"));
    QString txtPri = pal ? QString::fromLatin1(pal->txtPri) : (isDarkActive() ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));
    QString txtSec = pal ? QString::fromLatin1(pal->txtSec) : (isDarkActive() ? QStringLiteral("#9aa0a6") : QStringLiteral("#57606a"));
    QString accent = pal ? QString::fromLatin1(pal->blue) : (isDarkActive() ? QStringLiteral("#58a6ff") : QStringLiteral("#0969da"));

    const QString templateStr = QString::fromUtf8(R"(
QWidget {
    font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", "Inter", "Ubuntu", "Roboto", "Segoe UI", "DejaVu Sans", "Liberation Sans", "Helvetica Neue", Arial, sans-serif;
}
QStatusBar {
    background-color: %1;
    color: %2;
    border-top: 1px solid %3;
}
QStatusBar::item {
    border: none;
}
ads--CDockWidgetTab ads--CElidingLabel {
    color: %4;
}
ads--CDockWidgetTab {
    background: %1;
    margin-top: 2px;
    border-left: 1px solid %3;
    border-top: 2px solid %3;
    border-right: 1px solid %3;
    padding: 3px 6px 1px 6px;
    border-top-left-radius: 4px;
    border-top-right-radius: 4px;
}
ads--CDockWidgetTab:hover[activeTab="false"] {
    background: %3;
    color: %2;
}
ads--CDockWidgetTab[activeTab="true"] {
    background: %5;
    border-top: 3px solid %6;
    padding-bottom: 3px;
    margin-top: 0px;
}
ads--CDockWidgetTab[activeTab="true"] ads--CElidingLabel {
    color: %2;
    font-weight: bold;
}
ads--CDockWidgetTab[focused="true"] {
    background: %5;
    border-top: 3px solid %6;
}
ads--CDockAreaTitleBar {
    background: %1;
    border-bottom: 1px solid %3;
    padding-bottom: 0px;
}
#tabCloseButton {
    background: transparent;
    margin-bottom: 1px;
    margin-left: 4px;
    border: none;
    padding: 0px 0px;
    border-radius: 3px;
}
#tabCloseButton:hover {
    background-color: rgba(235, 87, 87, 0.35);
    color: #eb5757;
}
#tabCloseButton:pressed {
    background-color: #eb5757;
    color: #ffffff;
}
#QuickFindWidget {
    border-left: 1px solid %3;
    border-right: 1px solid %3;
    border-bottom: 3px solid %6;
    background: %1;
    color: %2;
}
QDialog, QMessageBox {
    background-color: %1;
    color: %2;
}
QLabel {
    color: %2;
}
QTabWidget::pane {
    border: 1px solid %3;
    background-color: %1;
    border-radius: 8px;
}
QTabBar::tab {
    background-color: %1;
    color: %4;
    padding: 7px 16px;
    border: 1px solid %3;
    border-bottom: none;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    margin-right: 2px;
}
QTabBar::tab:selected {
    background-color: %5;
    color: %2;
    font-weight: bold;
    border-top: 2px solid %6;
}
QTabBar::tab:hover:!selected {
    background-color: %3;
    color: %2;
}
QPushButton {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 6px;
    padding: 6px 16px;
    font-weight: 500;
}
QPushButton:hover {
    border-color: %6;
}
QPushButton:pressed {
    background-color: %6;
    color: #ffffff;
}
QPushButton:default {
    background-color: %6;
    color: #ffffff;
    border: 1px solid %6;
}
QTextBrowser {
    background-color: %5;
    color: %2;
    border: 1px solid %3;
    border-radius: 8px;
}
)");

    return templateStr.arg(win, txtPri, border, txtSec, card, accent);
}
