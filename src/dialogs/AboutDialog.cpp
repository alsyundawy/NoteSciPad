/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AboutDialog.h"
#include "../features/ThemeManager.h"

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

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &AboutDialog::updateThemeColors);
    updateThemeColors();
}

void AboutDialog::setupUi()
{
    setWindowTitle(tr("About NoteSciPad"));
    resize(720, 560);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(14);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createAboutTab(), tr("&About"));
    m_tabWidget->addTab(createMaintainerTab(), tr("&Maintainer"));
    m_tabWidget->addTab(createCreditsTab(), tr("&Credits && Attribution"));
    m_tabWidget->addTab(createEnvironmentTab(), tr("&Diagnostics"));
    m_tabWidget->addTab(createShortcutsTab(), tr("&Shortcuts"));
    m_tabWidget->addTab(createLicenseTab(), tr("&License"));

    mainLayout->addWidget(m_tabWidget);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::accept);
    mainLayout->addWidget(buttonBox);
}

void AboutDialog::updateThemeColors()
{
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();

    QString cardBg = pal ? QString::fromLatin1(pal->card) : (dark ? QStringLiteral("#25282c") : QStringLiteral("#ffffff"));
    QString borderCol = pal ? QString::fromLatin1(pal->border) : (dark ? QStringLiteral("#33373e") : QStringLiteral("#d0d7de"));
    QString accentCol = pal ? QString::fromLatin1(pal->blue) : (dark ? QStringLiteral("#58a6ff") : QStringLiteral("#0969da"));
    QString txtPri = pal ? QString::fromLatin1(pal->txtPri) : (dark ? QStringLiteral("#e0e0e0") : QStringLiteral("#24292f"));

    QString cardStyle = QString(
        "padding: 14px;"
        "background-color: %1;"
        "border: 1px solid %2;"
        "border-left: 4px solid %3;"
        "border-radius: 8px;"
        "color: %4;"
        "font-size: 13px;"
    ).arg(cardBg, borderCol, accentCol, txtPri);

    if (m_noticeBox) {
        m_noticeBox->setStyleSheet(cardStyle);
    }
    if (m_diagnosticsCard) {
        m_diagnosticsCard->setStyleSheet(cardStyle);
    }
}

QWidget *AboutDialog::createAboutTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(14);

    // Header layout with icon and title
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(16);

    QLabel *iconLabel = new QLabel(this);
    QPixmap iconPixmap(":/icons/NoteSciPad.png");
    if (!iconPixmap.isNull()) {
        iconLabel->setPixmap(iconPixmap.scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    iconLabel->setFixedSize(80, 80);
    headerLayout->addWidget(iconLabel);

    QVBoxLayout *titleLayout = new QVBoxLayout();
    titleLayout->setSpacing(4);

    QLabel *titleLabel = new QLabel(this);
    titleLabel->setText(QString(
        "<h2>NoteSciPad v%1 <span style='font-size: 11px; padding: 2px 8px; background-color: #0969da; color: #ffffff; border-radius: 4px;'>STABLE</span></h2>"
        "<p style='margin: 0; font-weight: bold; font-size: 13px;'>"
        "Independent, Ultra-Lightweight &amp; High-Performance Text Editor</p>"
        "<p style='margin: 0; font-style: italic; font-size: 12px;'>"
        "Powered natively by Vendored Scintilla 5.6.x &amp; Lexilla 5.5.x with Modern Qt 6</p>"
    ).arg(APP_VERSION));
    titleLayout->addWidget(titleLabel);

    // Badges pill row
    QLabel *badgesLabel = new QLabel(this);
    badgesLabel->setText(
        "<p style='margin-top: 6px; font-size: 11px;'>"
        "<span style='background: rgba(0,122,255,0.12); color: #007aff; border: 1px solid rgba(0,122,255,0.3); border-radius: 10px; padding: 2px 8px;'>🎨 75 Themes</span> "
        "<span style='background: rgba(16,185,129,0.12); color: #10b981; border: 1px solid rgba(16,185,129,0.3); border-radius: 10px; padding: 2px 8px;'>📐 Math Eval</span> "
        "<span style='background: rgba(139,92,246,0.12); color: #8b5cf6; border: 1px solid rgba(139,92,246,0.3); border-radius: 10px; padding: 2px 8px;'>🔤 100+ Lexers</span> "
        "<span style='background: rgba(245,158,11,0.12); color: #f59e0b; border: 1px solid rgba(245,158,11,0.3); border-radius: 10px; padding: 2px 8px;'>📄 Export PDF</span> "
        "<span style='background: rgba(236,72,153,0.12); color: #ec4899; border: 1px solid rgba(236,72,153,0.3); border-radius: 10px; padding: 2px 8px;'>⚡ Scintilla 5.6.x</span>"
        "</p>"
    );
    titleLayout->addWidget(badgesLabel);

    headerLayout->addLayout(titleLayout);
    headerLayout->addStretch();
    layout->addLayout(headerLayout);

    // Legal notice box with dynamic theme card
    m_noticeBox = new QLabel(this);
    m_noticeBox->setWordWrap(true);
    m_noticeBox->setText(
        tr("<b>Important Legal &amp; Non-Affiliation Statement:</b><br/>"
           "<i>NoteSciPad is an independent open-source project. It is not affiliated with, "
           "endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.</i>")
    );
    layout->addWidget(m_noticeBox);

    QLabel *metaLabel = new QLabel(this);
    metaLabel->setOpenExternalLinks(true);
    metaLabel->setText(
        QString("<p><b>Distribution Channel:</b> %1<br/>"
                "<b>Official Repository:</b> <a href='https://github.com/alsyundawy/NoteSciPad'>https://github.com/alsyundawy/NoteSciPad</a><br/>"
                "<b>License:</b> GNU General Public License v3 (GPL-3.0-or-later)<br/>"
                "<b>Engine Architecture:</b> Vendored Scintilla 5.6.x + Lexilla 5.5.x C++ Core</p>")
        .arg(APP_DISTRIBUTION)
    );
    layout->addWidget(metaLabel);

    layout->addStretch();
    return tab;
}

QWidget *AboutDialog::createMaintainerTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    m_maintainerBrowser = new QTextBrowser(this);
    m_maintainerBrowser->setOpenExternalLinks(true);
    m_maintainerBrowser->setHtml(
        tr("<h3>Maintainer &amp; Official Support Channels</h3>"
           "<p><b>Lead Architect &amp; Maintainer:</b> HARRY DERTIN SUTISNA ALSYUNDAWY (<code>@alsyundawy</code>)<br/>"
           "<b>Organization:</b> ALSYUNDAWY IT SOLUTION<br/>"
           "<b>Official Website:</b> <a href='https://alsyundawy.com'>https://alsyundawy.com</a><br/>"
           "<b>Official Email:</b> <a href='mailto:alsyundawy@gmail.com'>alsyundawy@gmail.com</a><br/>"
           "<b>GitHub Profile:</b> <a href='https://github.com/alsyundawy'>@alsyundawy</a><br/>"
           "<b>Telegram Channel:</b> <a href='https://t.me/alsyundawy'>@alsyundawy</a></p>"
           "<hr/>"
           "<h4>Sponsorship &amp; Donation Channels</h4>"
           "<p>If you find NoteSciPad useful for your development and daily workflow, consider supporting our ongoing development:</p>"
           "<ul>"
           "<li>💳 <b>PayPal:</b> <a href='https://www.paypal.me/alsyundawy'>paypal.me/alsyundawy</a></li>"
           "<li>🇮🇩 <b>QRIS (NMID: ID1020021153676):</b> <a href='https://github.com/user-attachments/assets/a0126f28-6dde-43da-ba14-d7c9a27de0df'>View Official QRIS Barcode</a></li>"
           "</ul>")
    );
    layout->addWidget(m_maintainerBrowser);
    return tab;
}

QWidget *AboutDialog::createCreditsTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);

    m_creditsBrowser = new QTextBrowser(this);
    m_creditsBrowser->setOpenExternalLinks(true);
    m_creditsBrowser->setHtml(
        tr("<h3>Credits &amp; Upstream Attribution</h3>"
           "<p>NoteSciPad stands on the shoulders of giants. We express our deepest gratitude to the following projects and authors:</p>"
           "<ul>"
           "<li><b>Scintilla:</b> Created by Neil Hodgson and contributors. The legendary core editing component providing high-performance text rendering, multi-caret editing, and fold margins. (<a href='https://www.scintilla.org'>scintilla.org</a>)</li>"
           "<li><b>Lexilla:</b> Created by Neil Hodgson and contributors. Provides syntax highlighting and styling lexers for over 100 programming languages. (<a href='https://www.scintilla.org'>scintilla.org/Lexilla.html</a>)</li>"
           "<li><b>NotepadNext:</b> Created by Justin Dailey (dail8859). Provided the cross-platform Qt scaffolding and docking framework foundation. (<a href='https://github.com/dail8859/NotepadNext'>github.com/dail8859/NotepadNext</a>)</li>"
           "<li><b>Notepad4:</b> Created by Zufu Liu (zufuliu). Provided invaluable architectural ideas, lexer updates, and utility concepts. (<a href='https://github.com/zufuliu/notepad4'>github.com/zufuliu/notepad4</a>)</li>"
           "<li><b>Notepad3:</b> Created by Rizonesoft and contributors. Inspired Scintilla power-user text transformation patterns and UI polish. (<a href='https://www.rizonesoft.com'>rizonesoft.com</a>)</li>"
           "<li><b>SubnetCalc for macOS:</b> Inspired UI elegance, theme architecture, and release documentation standards. (<a href='https://github.com/alsyundawy/SubnetCalc-MacOS'>github.com/alsyundawy/SubnetCalc-MacOS</a>)</li>"
           "<li><b>Lon McGregor (NotepadPlusPlusIcons):</b> Designer of the iconic open-source editor and chameleon artwork series, adapted as NoteSciPad's official application icon. (<a href='https://github.com/LonMcGregor/NotepadPlusPlusIcons'>github.com/LonMcGregor/NotepadPlusPlusIcons</a>)</li>"
           "<li><b>Qt Project:</b> Developed by The Qt Company and the Qt Open Source Community. (<a href='https://www.qt.io'>qt.io</a>)</li>"
           "<li><b>uchardet:</b> Maintained by Freedesktop. Enables fast universal character encoding detection.</li>"
           "<li><b>Qt Advanced Docking System (ADS):</b> Advanced dock widget management.</li>"
           "</ul>")
    );
    layout->addWidget(m_creditsBrowser);
    return tab;
}

QWidget *AboutDialog::createEnvironmentTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(14);

    m_diagnosticsCard = new QLabel(this);
    m_diagnosticsCard->setTextFormat(Qt::RichText);

    QScreen *primary = QGuiApplication::primaryScreen();
    qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

    QString infoText = QString(
        "<b>Operating System:</b> %1 (%2)<br/>"
        "<b>Architecture:</b> %3 (Build Target: %4)<br/>"
        "<b>Qt Version:</b> %5 (Built with %6)<br/>"
        "<b>Display Scale Ratio:</b> %7x (Retina / High-DPI)<br/>"
        "<b>NoteSciPad Version:</b> v%8<br/>"
        "<b>Kernel Type:</b> %9"
    ).arg(QSysInfo::prettyProductName())
     .arg(QSysInfo::kernelVersion())
     .arg(QSysInfo::currentCpuArchitecture())
     .arg(QSysInfo::buildCpuArchitecture())
     .arg(qVersion())
     .arg(QT_VERSION_STR)
     .arg(QString::number(dpr, 'f', 1))
     .arg(APP_VERSION)
     .arg(QSysInfo::kernelType());

    m_diagnosticsCard->setText(infoText);
    layout->addWidget(m_diagnosticsCard);

    QPushButton *copyBtn = new QPushButton(tr("Copy Diagnostics to Clipboard"), this);
    copyBtn->setCursor(Qt::PointingHandCursor);
    connect(copyBtn, &QPushButton::clicked, this, &AboutDialog::copySystemInfoToClipboard);
    layout->addWidget(copyBtn);

    layout->addStretch();
    return tab;
}

QWidget *AboutDialog::createShortcutsTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);

    m_shortcutsBrowser = new QTextBrowser(this);
    m_shortcutsBrowser->setOpenExternalLinks(false);
    m_shortcutsBrowser->setHtml(
        tr("<h3>Key Productivity Shortcuts</h3>"
           "<table border='0' cellspacing='6' cellpadding='4' width='100%'>"
           "<tr><td><b>Ctrl / Cmd + N</b></td><td>Create New File</td></tr>"
           "<tr><td><b>Ctrl / Cmd + Shift + N</b></td><td>Open New Window (Independent Instance)</td></tr>"
           "<tr><td><b>Ctrl / Cmd + Shift + E</b></td><td>Evaluate Inline Math Expression</td></tr>"
           "<tr><td><b>Ctrl / Cmd + Shift + T</b></td><td>Interactive Theme Switcher (75 Palettes)</td></tr>"
           "<tr><td><b>Ctrl / Cmd + Shift + F</b></td><td>Find in Files / Workspace</td></tr>"
           "<tr><td><b>Ctrl / Cmd + P</b></td><td>Print Document / Export to PDF</td></tr>"
           "<tr><td><b>Ctrl / Cmd + D</b></td><td>Duplicate Current Line or Selection</td></tr>"
           "<tr><td><b>Ctrl / Cmd + /</b></td><td>Toggle Single-Line Comment</td></tr>"
           "<tr><td><b>Ctrl / Cmd + Shift + /</b></td><td>Toggle Block / Stream Comment</td></tr>"
           "<tr><td><b>Ctrl / Cmd + Shift + U</b></td><td>Convert Selection to UPPERCASE</td></tr>"
           "<tr><td><b>Ctrl / Cmd + U</b></td><td>Convert Selection to lowercase</td></tr>"
           "<tr><td><b>Alt + Click &amp; Drag</b></td><td>Column / Rectangular Box Selection</td></tr>"
           "</table>")
    );
    layout->addWidget(m_shortcutsBrowser);
    return tab;
}

QWidget *AboutDialog::createLicenseTab()
{
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);

    m_licenseBrowser = new QTextBrowser(this);
    m_licenseBrowser->setOpenExternalLinks(true);
    m_licenseBrowser->setHtml(
        tr("<h3>GNU General Public License v3.0</h3>"
           "<p>NoteSciPad is free software: you can redistribute it and/or modify "
           "it under the terms of the GNU General Public License as published by "
           "the Free Software Foundation, either version 3 of the License, or "
           "(at your option) any later version.</p>"
           "<p>This program is distributed in the hope that it will be useful, "
           "but WITHOUT ANY WARRANTY; without even the implied warranty of "
           "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
           "GNU General Public License for more details.</p>"
           "<p>For complete license details, visit: <a href='https://www.gnu.org/licenses/'>https://www.gnu.org/licenses/</a></p>")
    );
    layout->addWidget(m_licenseBrowser);
    return tab;
}

void AboutDialog::copySystemInfoToClipboard()
{
    QScreen *primary = QGuiApplication::primaryScreen();
    qreal dpr = primary ? primary->devicePixelRatio() : 1.0;

    QString plainInfo = QString(
        "NoteSciPad Version: v%1\n"
        "OS: %2 (%3)\n"
        "Kernel: %4\n"
        "CPU: %5 (Build Target: %6)\n"
        "Qt: %7 (Built with %8)\n"
        "Display Scale: %9x\n"
    ).arg(APP_VERSION)
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
