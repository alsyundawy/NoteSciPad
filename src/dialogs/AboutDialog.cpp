/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AboutDialog.h"
#include "../features/ThemeManager.h"
#include "../features/ThemePaletteData.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QTextBrowser>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGuiApplication>
#include <QClipboard>
#include <QSysInfo>
#include <QScreen>
#include <QPixmap>
#include <QScrollArea>
#include <QTabBar>

AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent) {
    setupUi();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &AboutDialog::updateThemeColors);
    connect(ThemeManager::instance(), &ThemeManager::paletteChanged, this, &AboutDialog::updateThemeColors);
    updateThemeColors();
}

void AboutDialog::setupUi() {
    setWindowTitle(tr("About SparkNotePad"));
    // Fix: minimum size prevents tab truncation on macOS Retina and Linux HiDPI
    setMinimumSize(620, 480);
    resize(660, 520);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(8);

    m_tabWidget = new QTabWidget(this);
    // Fix: prevent tab label eliding and scroll arrows on narrow dialogs
    m_tabWidget->tabBar()->setElideMode(Qt::ElideNone);
    m_tabWidget->setUsesScrollButtons(false);

    m_tabWidget->addTab(createAboutTab(), tr("&About"));
    m_tabWidget->addTab(createMaintainerTab(), tr("&Maintainer"));
    // Fix: shortened from tr("&Credits && Attribution") to prevent tab text clipping
    // Full text preserved in tooltip per UX spec (Section 3.3)
    m_tabWidget->addTab(createCreditsTab(), tr("&Credits"));
    m_tabWidget->setTabToolTip(2, tr("Credits & Attribution"));
    m_tabWidget->addTab(createEnvironmentTab(), tr("&Diagnostics"));
    m_tabWidget->setTabToolTip(3, tr("System & Engine Diagnostics"));
    m_tabWidget->addTab(createShortcutsTab(), tr("&Shortcuts"));
    m_tabWidget->addTab(createLicenseTab(), tr("&License"));

    mainLayout->addWidget(m_tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

void AboutDialog::updateThemeColors() {
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();

    // World standard accessible color metrics (WCAG 2.1 AAA: high contrast, zero blur)
    QString winBg =
        pal ? QString::fromLatin1(pal->win) : (dark ? QStringLiteral("#0f172a") : QStringLiteral("#f8fafc"));
    QString cardBg =
        pal ? QString::fromLatin1(pal->card) : (dark ? QStringLiteral("#1e293b") : QStringLiteral("#ffffff"));
    QString borderCol =
        pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#334155") : QStringLiteral("#cbd5e1"));
    QString accentCol =
        pal ? QString::fromLatin1(pal->blue) : (dark ? QStringLiteral("#38bdf8") : QStringLiteral("#2563eb"));
    QString txtPri =
        pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#f8fafc") : QStringLiteral("#0f172a"));
    QString txtSec =
        pal ? QString::fromLatin1(pal->txtSec) : (dark ? QStringLiteral("#94a3b8") : QStringLiteral("#475569"));

    QString badgeBg = dark ? QStringLiteral("rgba(255,255,255,0.08)") : QStringLiteral("rgba(0,0,0,0.05)");
    QString cardStyle =
        QString(
            "QLabel {"
            "  padding: 8px 10px;"
            "  background-color: %1;"
            "  border: 1px solid %2;"
            "  border-left: 4px solid %3;"
            "  border-radius: 6px;"
            "  color: %4;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 11px;"
            "  line-height: 1.4;"
            "}")
            .arg(cardBg, borderCol, accentCol, txtPri);

    QString dialogStyle =
        QString(
            "QDialog {"
            "  background-color: %1;"
            "  color: %4;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 12px;"
            "}"
            "QTabWidget::pane {"
            "  background-color: %2;"
            "  border: 1px solid %3;"
            "  border-radius: 6px;"
            "}"
            "QTabBar {"
            "  background: transparent;"
            "  border: none;"
            "  qproperty-drawBase: 0;"
            "}"
            "QTabBar::tab {"
            "  background-color: %1;"
            "  color: %5;"
            "  padding: 5px 12px;"
            "  border: 1px solid %3;"
            "  border-bottom: none;"
            "  border-top-left-radius: 5px;"
            "  border-top-right-radius: 5px;"
            "  margin-right: 2px;"
            "  font-size: 11px;"
            "  font-weight: 500;"
            "}"
            "QTabBar::tab:selected {"
            "  background-color: %2;"
            "  color: %4;"
            "  font-weight: bold;"
            "  border-top: 2px solid %6;"
            "}"
            "QTabBar::tab:hover:!selected {"
            "  background-color: %3;"
            "  color: %4;"
            "}"
            "QLabel {"
            "  color: %4;"
            "}"
            "QTextBrowser {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: 1px solid %3;"
            "  border-radius: 6px;"
            "  padding: 8px 10px;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 11px;"
            "  line-height: 1.4;"
            "}"
            "QPushButton {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: 1px solid %3;"
            "  border-radius: 5px;"
            "  padding: 5px 14px;"
            "  font-size: 11px;"
            "  font-weight: 600;"
            "}"
            "QPushButton:hover {"
            "  border-color: %6;"
            "  color: %6;"
            "}"
            "QPushButton:pressed {"
            "  background-color: %6;"
            "  color: #ffffff;"
            "}")
            .arg(winBg, cardBg, borderCol, txtPri, txtSec, accentCol);

    setStyleSheet(dialogStyle);

    if (m_titleLabel) {
        m_titleLabel->setText(
            QString(
                "<h3 style='color: %1; margin: 0 0 2px 0; font-size: 15px;'>SparkNotePad v%2 <span style='font-size: "
                "10px; padding: 1px "
                "6px; background-color: %3; color: #ffffff; border-radius: 4px; font-weight: bold;'>STABLE</span></h3>"
                "<p style='margin: 0; font-weight: 600; font-size: 11px; color: %1;'>"
                "Independent, Ultra-Lightweight &amp; High-Performance Text Editor</p>"
                "<p style='margin: 1px 0 0 0; font-style: italic; font-size: 10px; color: %4;'>"
                "Powered natively by Vendored Scintilla 5.6.x &amp; Lexilla 5.5.x with Modern Qt 6</p>")
                .arg(txtPri, APP_VERSION, accentCol, txtSec));
    }

    if (m_badgesLabel) {
        m_badgesLabel->setText(
            QString("<p style='margin-top: 4px; margin-bottom: 0; font-size: 10px; line-height: 1.5;'>"
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>75 Themes</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>📐 Math Eval</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>🔤 100+ Lexers</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>📄 Export PDF</span> "
                    "<span style='background: %1; color: %2; border: 1px solid %3; border-radius: "
                    "8px; padding: 2px 7px; font-weight: 500;'>⚡ Scintilla 5.6.x</span>"
                    "</p>")
                .arg(badgeBg, txtPri, borderCol));
    }

    if (m_noticeBox) {
        m_noticeBox->setStyleSheet(cardStyle);
        m_noticeBox->setText(tr("<b style='color: %1;'>Important Legal &amp; Non-Affiliation Statement:</b><br/>"
                                "<span style='color: %1;'>SparkNotePad is an independent open-source project. It is "
                                "not affiliated with, "
                                "endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.</span>")
                                 .arg(txtPri));
    }

    if (m_metaLabel) {
        m_metaLabel->setStyleSheet(cardStyle);
        m_metaLabel->setText(
            QString(
                "<div style='color: %1; line-height: 1.4;'>"
                "<table border='0' cellspacing='3' cellpadding='1' style='color: %1; font-size: 11px; width: 100%;'>"
                "<tr>"
                "  <td style='width: 130px; font-weight: bold;'>GitHub Repository:</td>"
                "  <td><a style='color: %2; text-decoration: none; font-weight: 600;' "
                "href='https://github.com/alsyundawy/SparkNotePad'>"
                "📦 https://github.com/alsyundawy/SparkNotePad</a></td>"
                "</tr>"
                "<tr>"
                "  <td style='font-weight: bold;'>Release Tag:</td>"
                "  <td><a style='text-decoration: none;' "
                "href='https://github.com/alsyundawy/SparkNotePad/releases/tag/v1.3.5'>"
                "<span style='background-color: #238636; color: #ffffff; padding: 1px 8px; border-radius: 10px; "
                "font-weight: bold; font-size: 10px;'>🏷️ v1.3.5 Official Release</span></a></td>"
                "</tr>"
                "<tr>"
                "  <td style='font-weight: bold;'>Lead Maintainer:</td>"
                "  <td><b>Harry Dertin Sutisna Alsyundawy</b> (<a style='color: %2; text-decoration: none;' "
                "href='https://github.com/alsyundawy'>@alsyundawy</a>) — <b>ALSYUNDAWY IT SOLUTION</b></td>"
                "</tr>"
                "<tr>"
                "  <td style='font-weight: bold;'>License &amp; Engine:</td>"
                "  <td>GNU General Public License v3.0 &bull; Scintilla 5.6.7 + Lexilla 5.5.4 &bull; Qt 6</td>"
                "</tr>"
                "</table>"
                "</div>")
                .arg(txtPri, accentCol));
    }

    if (m_diagnosticsCard) {
        m_diagnosticsCard->setStyleSheet(cardStyle);
        QScreen *primary = QGuiApplication::primaryScreen();
        qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

        QString infoText = QString(
                               "<div style='color: %1; line-height: 1.5;'>"
                               "<b>Operating System:</b> %2 (%3)<br/>"
                               "<b>Architecture:</b> %4 (Build Target: %5)<br/>"
                               "<b>Qt Version:</b> %6 (Built with %7)<br/>"
                               "<b>Display Scale Ratio:</b> %8x (Retina / High-DPI)<br/>"
                               "<b>SparkNotePad Version:</b> v%9<br/>"
                               "<b>Kernel Type:</b> %10"
                               "</div>")
                               .arg(txtPri)
                               .arg(QSysInfo::prettyProductName())
                               .arg(QSysInfo::kernelVersion())
                               .arg(QSysInfo::currentCpuArchitecture())
                               .arg(QSysInfo::buildCpuArchitecture())
                               .arg(qVersion())
                               .arg(QT_VERSION_STR)
                               .arg(QString::number(dpr, 'f', 1))
                               .arg(APP_VERSION)
                               .arg(QSysInfo::kernelType());

        m_diagnosticsCard->setText(infoText);
    }

    if (m_maintainerBrowser) {
        m_maintainerBrowser->setHtml(
            tr("<div style='color: %1; line-height: 1.4;'>"
               "<h3 style='color: %1; margin-top: 0; font-size: 13px;'>Maintainer &amp; Official Support Channels</h3>"
               "<p><b>Lead Architect &amp; Maintainer:</b> HARRY DERTIN SUTISNA ALSYUNDAWY "
               "(<code>@alsyundawy</code>)<br/>"
               "<b>Organization:</b> ALSYUNDAWY IT SOLUTION<br/>"
               "<b>Official Website:</b> <a style='color: %2;' "
               "href='https://alsyundawy.com'>https://alsyundawy.com</a><br/>"
               "<b>Official Email:</b> <a style='color: %2;' "
               "href='mailto:alsyundawy@gmail.com'>alsyundawy@gmail.com</a><br/>"
               "<b>GitHub Profile:</b> <a style='color: %2;' href='https://github.com/alsyundawy'>@alsyundawy</a><br/>"
               "<b>Telegram Channel:</b> <a style='color: %2;' href='https://t.me/alsyundawy'>@alsyundawy</a></p>"
               "<hr style='border: 0; border-top: 1px solid %3; margin: 10px 0;'/>"
               "<h4 style='color: %1; font-size: 12px; margin: 6px 0;'>Sponsorship &amp; Donation Channels</h4>"
               "<p>If you find SparkNotePad useful for your development and daily workflow, consider supporting our "
               "ongoing development:</p>"
               "<ul>"
               "<li>💳 <b>PayPal:</b> <a style='color: %2;' "
               "href='https://www.paypal.me/alsyundawy'>paypal.me/alsyundawy</a></li>"
               "<li>🇮🇩 <b>QRIS (NMID: ID1020021153676):</b> <a style='color: %2;' "
               "href='https://github.com/user-attachments/assets/a0126f28-6dde-43da-ba14-d7c9a27de0df'>View Official "
               "QRIS Barcode</a></li>"
               "</ul>"
               "</div>")
                .arg(txtPri, accentCol, borderCol));
    }

    if (m_creditsBrowser) {
        m_creditsBrowser->setHtml(
            tr("<div style='color: %1; line-height: 1.4;'>"
               "<h3 style='color: %1; margin-top: 0; font-size: 13px;'>Credits &amp; Upstream Attribution</h3>"
               "<p>SparkNotePad stands on the shoulders of giants. We express our deepest gratitude to the following "
               "projects and authors:</p>"
               "<ul>"
               "<li><b>Scintilla:</b> Created by Neil Hodgson and contributors. Core editing component providing "
               "high-performance text rendering, multi-caret editing, and fold margins. (<a style='color: %2;' "
               "href='https://www.scintilla.org'>scintilla.org</a>)</li>"
               "<li><b>Lexilla:</b> Created by Neil Hodgson and contributors. Syntax highlighting and styling "
               "lexers for over 100 programming languages. (<a style='color: %2;' "
               "href='https://www.scintilla.org'>scintilla.org/Lexilla.html</a>)</li>"
               "<li><b>NotepadNext:</b> Created by Justin Dailey (dail8859). Provided the cross-platform Qt "
               "scaffolding and docking framework foundation. (<a style='color: %2;' "
               "href='https://github.com/dail8859/NotepadNext'>github.com/dail8859/NotepadNext</a>)</li>"
               "<li><b>Notepad4:</b> Created by Zufu Liu (zufuliu). Architectural ideas, lexer "
               "updates, and utility concepts. (<a style='color: %2;' "
               "href='https://github.com/zufuliu/notepad4'>github.com/zufuliu/notepad4</a>)</li>"
               "<li><b>Notepad3:</b> Created by Rizonesoft and contributors. Scintilla power-user text "
               "transformation patterns and UI polish. (<a style='color: %2;' "
               "href='https://www.rizonesoft.com'>rizonesoft.com</a>)</li>"
               "<li><b>SubnetCalc for macOS:</b> Inspired UI elegance, theme architecture, and release documentation "
               "standards. (<a style='color: %2;' "
               "href='https://github.com/alsyundawy/SubnetCalc-MacOS'>github.com/alsyundawy/SubnetCalc-MacOS</a>)</li>"
               "<li><b>Lon McGregor (NotepadPlusPlusIcons):</b> Designer of the open-source editor and "
               "chameleon artwork series, adapted as SparkNotePad's official application icon. (<a style='color: %2;' "
               "href='https://github.com/LonMcGregor/NotepadPlusPlusIcons'>github.com/LonMcGregor/"
               "NotepadPlusPlusIcons</a>)</li>"
               "<li><b>Qt Project:</b> Developed by The Qt Company and the Qt Open Source Community. (<a style='color: "
               "%2;' href='https://www.qt.io'>qt.io</a>)</li>"
               "<li><b>uchardet:</b> Maintained by Freedesktop. Enables fast universal character encoding "
               "detection.</li>"
               "<li><b>Qt Advanced Docking System (ADS):</b> Advanced dock widget management.</li>"
               "</ul>"
               "</div>")
                .arg(txtPri, accentCol));
    }

    if (m_shortcutsBrowser) {
        m_shortcutsBrowser->setHtml(
            tr("<div style='color: %1; line-height: 1.4;'>"
               "<h3 style='color: %1; margin-top: 0; font-size: 13px;'>Key Productivity Shortcuts</h3>"
               "<table border='0' cellspacing='4' cellpadding='2' width='100%' style='color: %1; font-size: 11px;'>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + N</td><td>Create New File</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + N</td><td>Open New Window "
               "(Independent Instance)</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + E</td><td>Evaluate Inline Math "
               "Expression</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + T</td><td>Interactive Theme "
               "Switcher (75 Palettes)</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + F</td><td>Find in Files / "
               "Workspace</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + P</td><td>Print Document / Export to "
               "PDF</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + D</td><td>Duplicate Current Line or "
               "Selection</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + /</td><td>Toggle Single-Line "
               "Comment</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + /</td><td>Toggle Block / Stream "
               "Comment</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + Shift + U</td><td>Convert Selection to "
               "UPPERCASE</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Ctrl / Cmd + U</td><td>Convert Selection to "
               "lowercase</td></tr>"
               "<tr><td style='color: %2; font-weight: bold;'>Alt + Click &amp; Drag</td><td>Column / Rectangular Box "
               "Selection</td></tr>"
               "</table>"
               "</div>")
                .arg(txtPri, accentCol));
    }

    if (m_licenseBrowser) {
        m_licenseBrowser->setHtml(
            tr("<div style='color: %1; line-height: 1.4;'>"
               "<h3 style='color: %1; margin-top: 0; font-size: 13px;'>GNU General Public License v3.0</h3>"
               "<p>SparkNotePad is free software: you can redistribute it and/or modify "
               "it under the terms of the GNU General Public License as published by "
               "the Free Software Foundation, either version 3 of the License, or "
               "(at your option) any later version.</p>"
               "<p>This program is distributed in the hope that it will be useful, "
               "but WITHOUT ANY WARRANTY; without even the implied warranty of "
               "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
               "GNU General Public License for more details.</p>"
               "<p>For complete license details, visit: <a style='color: %2;' "
               "href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a></p>"
               "</div>")
                .arg(txtPri, accentCol));
    }
}

QWidget *AboutDialog::createAboutTab() {
    // Wrap content in QScrollArea to prevent clipping on small screens/HiDPI
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    // Header layout with icon and title
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(12);

    QLabel *iconLabel = new QLabel(tab);
    QPixmap iconPixmap(":/icons/SparkNotePad.png");
    if (!iconPixmap.isNull()) {
        iconLabel->setPixmap(iconPixmap.scaled(52, 52, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    iconLabel->setFixedSize(52, 52);
    headerLayout->addWidget(iconLabel);

    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(2);

    m_titleLabel = new QLabel(tab);
    m_titleLabel->setWordWrap(true);  // Fix: prevent horizontal clipping on Retina
    titleLayout->addWidget(m_titleLabel);

    m_badgesLabel = new QLabel(tab);
    m_badgesLabel->setWordWrap(true);  // Fix: allow badge line wrapping
    titleLayout->addWidget(m_badgesLabel);

    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    layout->addLayout(headerLayout);

    // Legal notice box with dynamic theme card
    m_noticeBox = new QLabel(tab);
    m_noticeBox->setWordWrap(true);
    layout->addWidget(m_noticeBox);

    m_metaLabel = new QLabel(tab);
    m_metaLabel->setOpenExternalLinks(true);
    m_metaLabel->setWordWrap(true);  // Fix: wrap long URLs on small dialogs
    layout->addWidget(m_metaLabel);

    layout->addStretch();
    scroll->setWidget(tab);
    return scroll;
}

QWidget *AboutDialog::createMaintainerTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    m_maintainerBrowser = new QTextBrowser(this);
    m_maintainerBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_maintainerBrowser);
    return tab;
}

QWidget *AboutDialog::createCreditsTab() {
    // Wrap in QScrollArea for HiDPI/small screen safety
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setFrameShape(QFrame::NoFrame);

    QWidget *tab = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);

    m_creditsBrowser = new QTextBrowser(tab);
    m_creditsBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_creditsBrowser);

    scroll->setWidget(tab);
    return scroll;
}

QWidget *AboutDialog::createEnvironmentTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->setSpacing(8);

    m_diagnosticsCard = new QLabel(this);
    m_diagnosticsCard->setTextFormat(Qt::RichText);
    layout->addWidget(m_diagnosticsCard);

    QPushButton *copyBtn = new QPushButton(tr("Copy Diagnostics to Clipboard"), this);
    copyBtn->setCursor(Qt::PointingHandCursor);
    connect(copyBtn, &QPushButton::clicked, this, &AboutDialog::copySystemInfoToClipboard);
    layout->addWidget(copyBtn);

    layout->addStretch();
    return tab;
}

QWidget *AboutDialog::createShortcutsTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);

    m_shortcutsBrowser = new QTextBrowser(this);
    m_shortcutsBrowser->setOpenExternalLinks(false);
    layout->addWidget(m_shortcutsBrowser);
    return tab;
}

QWidget *AboutDialog::createLicenseTab() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(10, 10, 10, 10);

    m_licenseBrowser = new QTextBrowser(this);
    m_licenseBrowser->setOpenExternalLinks(true);
    layout->addWidget(m_licenseBrowser);
    return tab;
}

void AboutDialog::copySystemInfoToClipboard() {
    QScreen *primary = QGuiApplication::primaryScreen();
    qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

    QString plainInfo = QString(
                            "SparkNotePad Version: v%1\n"
                            "OS: %2 (%3)\n"
                            "Kernel: %4\n"
                            "CPU: %5 (Build Target: %6)\n"
                            "Qt: %7 (Built with %8)\n"
                            "Display Scale: %9x\n")
                            .arg(APP_VERSION)
                            .arg(QSysInfo::prettyProductName())
                            .arg(QSysInfo::kernelType())
                            .arg(QSysInfo::kernelVersion())
                            .arg(QSysInfo::currentCpuArchitecture())
                            .arg(QSysInfo::buildCpuArchitecture())
                            .arg(qVersion())
                            .arg(QT_VERSION_STR)
                            .arg(QString::number(dpr, 'f', 1));

    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard) {
        clipboard->setText(plainInfo);
    }
}
