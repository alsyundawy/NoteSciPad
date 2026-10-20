/*
 * This file is part of Notepad Next.
 * Copyright 2019 Justin Dailey
 *
 * Notepad Next is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Notepad Next is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Notepad Next.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "PreferencesDialog.h"
#include "SparkNotePadApplication.h"
#include "TranslationManager.h"
#include "ui_PreferencesDialog.h"
#include "ScintillaNext.h"
#include "ThemeManager.h"
#include "features/ThemePaletteData.h"
#include "features/TransparentWindowManager.h"

#include <QButtonGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QFormLayout>
#include <QSlider>
#include <QSpinBox>
#include <QPushButton>

PreferencesDialog::PreferencesDialog(ApplicationSettings *settings, QWidget *parent)
    : QDialog(parent, Qt::Tool), ui(new Ui::PreferencesDialog), settings(settings) {
    ui->setupUi(this);

    setupThemeAndFeatureSettings();
    connect(ThemeManager::instance(), &ThemeManager::themeChanged, this, &PreferencesDialog::updateThemeColors);
    updateThemeColors();

    QIcon icon = style()->standardIcon(QStyle::SP_MessageBoxInformation);
    QPixmap pixmap = icon.pixmap(QSize(16, 16));
    ui->labelAppRestartIcon->setPixmap(pixmap);
    ui->labelAppRestartIcon->hide();
    ui->labelAppRestart->hide();

    MapSettingToCheckBox(ui->checkBoxMenuBar, &ApplicationSettings::showMenuBar, &ApplicationSettings::setShowMenuBar,
                         &ApplicationSettings::showMenuBarChanged);
    MapSettingToCheckBox(ui->checkBoxToolBar, &ApplicationSettings::showToolBar, &ApplicationSettings::setShowToolBar,
                         &ApplicationSettings::showToolBarChanged);
    MapSettingToCheckBox(ui->checkBoxStatusBar, &ApplicationSettings::showStatusBar,
                         &ApplicationSettings::setShowStatusBar, &ApplicationSettings::showStatusBarChanged);
    MapSettingToCheckBox(ui->checkBoxRecenterSearchDialog, &ApplicationSettings::centerSearchDialog,
                         &ApplicationSettings::setCenterSearchDialog, &ApplicationSettings::centerSearchDialogChanged);

    MapSettingToGroupBox(ui->gbxRestorePreviousSession, &ApplicationSettings::restorePreviousSession,
                         &ApplicationSettings::setRestorePreviousSession,
                         &ApplicationSettings::restorePreviousSessionChanged);
    connect(ui->gbxRestorePreviousSession, &QGroupBox::toggled, this, [=, this](bool checked) {
        if (!checked) {
            ui->checkBoxUnsavedFiles->setChecked(false);
            ui->checkBoxRestoreTempFiles->setChecked(false);
        } else {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("This feature is experimental and it should not be considered safe for critically "
                                    "important work. It may lead to possible data loss. Use at your own risk."));
        }
    });

    MapSettingToCheckBox(ui->checkBoxUnsavedFiles, &ApplicationSettings::restoreUnsavedFiles,
                         &ApplicationSettings::setRestoreUnsavedFiles,
                         &ApplicationSettings::restoreUnsavedFilesChanged);
    MapSettingToCheckBox(ui->checkBoxRestoreTempFiles, &ApplicationSettings::restoreTempFiles,
                         &ApplicationSettings::setRestoreTempFiles, &ApplicationSettings::restoreTempFilesChanged);

    MapSettingToCheckBox(ui->checkBoxCombineSearchResults, &ApplicationSettings::combineSearchResults,
                         &ApplicationSettings::setCombineSearchResults,
                         &ApplicationSettings::combineSearchResultsChanged);

    populateTranslationComboBox();
    connect(ui->comboBoxTranslation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=, this](int index) {
        settings->setTranslation(ui->comboBoxTranslation->itemData(index).toString());
        showApplicationRestartRequired();
    });

    MapSettingToCheckBox(ui->checkBoxExitOnLastTabClosed, &ApplicationSettings::exitOnLastTabClosed,
                         &ApplicationSettings::setExitOnLastTabClosed,
                         &ApplicationSettings::exitOnLastTabClosedChanged);

    ui->fcbDefaultFont->setCurrentFont(QFont(settings->fontName()));
    connect(ui->fcbDefaultFont, &QFontComboBox::currentFontChanged, this,
            [=, this](const QFont &f) { settings->setFontName(f.family()); });
    connect(settings, &ApplicationSettings::fontNameChanged, this,
            [=, this](QString fontName) { ui->fcbDefaultFont->setCurrentFont(QFont(fontName)); });

    ui->spbDefaultFontSize->setValue(settings->fontSize());
    connect(ui->spbDefaultFontSize, QOverload<int>::of(&QSpinBox::valueChanged), settings,
            &ApplicationSettings::setFontSize);
    connect(settings, &ApplicationSettings::fontSizeChanged, ui->spbDefaultFontSize, &QSpinBox::setValue);

    ui->comboBoxLineEndings->addItem(tr("System Default"), QString(""));
    ui->comboBoxLineEndings->addItem(tr("Windows (CR LF)"), ScintillaNext::eolModeToString(SC_EOL_CRLF));
    ui->comboBoxLineEndings->addItem(tr("Linux (LF)"), ScintillaNext::eolModeToString(SC_EOL_LF));
    ui->comboBoxLineEndings->addItem(tr("Macintosh (CR)"), ScintillaNext::eolModeToString(SC_EOL_CR));

    // Select the current one
    int index = ui->comboBoxLineEndings->findData(settings->defaultEOLMode());
    ui->comboBoxLineEndings->setCurrentIndex(index == -1 ? 0 : index);

    connect(ui->comboBoxLineEndings, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [=, this](int index) { settings->setDefaultEOLMode(ui->comboBoxLineEndings->itemData(index).toString()); });
    connect(settings, &ApplicationSettings::defaultEOLModeChanged, this, [=, this](QString defaultEOLMode) {
        int index = ui->comboBoxLineEndings->findData(defaultEOLMode);
        ui->comboBoxLineEndings->setCurrentIndex(index == -1 ? 0 : index);
    });

    MapSettingToCheckBox(ui->checkBoxHighlightURLs, &ApplicationSettings::urlHighlighting,
                         &ApplicationSettings::setURLHighlighting, &ApplicationSettings::urlHighlightingChanged);
    ui->spinBoxUrlThreshold->setValue(settings->urlThreshold());
    connect(ui->spinBoxUrlThreshold, &QSpinBox::valueChanged, settings, &ApplicationSettings::setUrlThreshold);
    connect(settings, &ApplicationSettings::urlThresholdChanged, ui->spinBoxUrlThreshold, &QSpinBox::setValue);
    MapSettingToCheckBox(ui->checkBoxShowLineNumbers, &ApplicationSettings::showLineNumbers,
                         &ApplicationSettings::setShowLineNumbers, &ApplicationSettings::showLineNumbersChanged);
    MapSettingToCheckBox(ui->checkBoxShowChangeHistory, &ApplicationSettings::showChangeHistory,
                         &ApplicationSettings::setShowChangeHistory, &ApplicationSettings::showChangeHistoryChanged);
    MapSettingToCheckBox(ui->checkBoxAutoCompletion, &ApplicationSettings::autoCompletion,
                         &ApplicationSettings::setAutoCompletion, &ApplicationSettings::autoCompletionChanged);
    MapSettingToCheckBox(ui->checkBoxAutoCloseBrackets, &ApplicationSettings::autoCloseBrackets,
                         &ApplicationSettings::setAutoCloseBrackets, &ApplicationSettings::autoCloseBracketsChanged);
    MapSettingToCheckBox(ui->checkBoxAutoCloseQuotes, &ApplicationSettings::autoCloseQuotes,
                         &ApplicationSettings::setAutoCloseQuotes, &ApplicationSettings::autoCloseQuotesChanged);
    MapSettingToCheckBox(ui->checkBoxSmartAutoIndent, &ApplicationSettings::smartAutoIndent,
                         &ApplicationSettings::setSmartAutoIndent, &ApplicationSettings::smartAutoIndentChanged);
    MapSettingToCheckBox(ui->checkBoxColorPreviewCallTip, &ApplicationSettings::colorPreviewCallTip,
                         &ApplicationSettings::setColorPreviewCallTip,
                         &ApplicationSettings::colorPreviewCallTipChanged);
    MapSettingToCheckBox(ui->checkBoxPreserveFileTime, &ApplicationSettings::preserveFileTime,
                         &ApplicationSettings::setPreserveFileTime, &ApplicationSettings::preserveFileTimeChanged);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->radioFollowCurrentDirectory, ApplicationSettings::FollowCurrentDocument);
    buttonGroup->addButton(ui->radioLastUsedDirectory, ApplicationSettings::RememberLastUsed);
    buttonGroup->addButton(ui->radioHardCoded, ApplicationSettings::HardCoded);

    connect(buttonGroup, &QButtonGroup::idClicked, this, [=, this](int id) {
        ApplicationSettings::DefaultDirectoryBehaviorEnum e =
            static_cast<ApplicationSettings::DefaultDirectoryBehaviorEnum>(id);
        settings->setDefaultDirectoryBehavior(e);
    });

    connect(ui->radioHardCoded, &QRadioButton::toggled, this, [=, this](bool checked) {
        ui->btnSelectHardCodedPath->setEnabled(checked);
        ui->txtHardCodedPath->setEnabled(checked);
    });

    connect(ui->btnSelectHardCodedPath, &QToolButton::clicked, this, [=, this]() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Default Directory"));
        if (dir.isEmpty())
            return;  // user cancelled

        settings->setDefaultDirectory(QDir::fromNativeSeparators(dir));
        ui->txtHardCodedPath->setText(QDir::toNativeSeparators(dir));
    });

    connect(ui->txtHardCodedPath, &QLineEdit::editingFinished, this, [=, this]() {
        QString dir = ui->txtHardCodedPath->text();
        settings->setDefaultDirectory(QDir::fromNativeSeparators(dir));
        ui->txtHardCodedPath->setText(QDir::toNativeSeparators(dir));
    });

    if (auto b = buttonGroup->button(settings->defaultDirectoryBehavior())) {
        b->setChecked(true);
    }

    if (settings->defaultDirectoryBehavior() == ApplicationSettings::HardCoded) {
        ui->txtHardCodedPath->setText((QDir::toNativeSeparators(settings->defaultDirectory())));
    } else {
        ui->txtHardCodedPath->setText(QString());
    }
}

PreferencesDialog::~PreferencesDialog() {
    delete ui;
}

void PreferencesDialog::showApplicationRestartRequired() const {
    ui->labelAppRestartIcon->show();
    ui->labelAppRestart->show();
}

template <typename Func1, typename Func2, typename Func3>
void PreferencesDialog::MapSettingToCheckBox(QCheckBox *checkBox, Func1 getter, Func2 setter, Func3 notifier) const {
    // Get the value and set the checkbox state
    checkBox->setChecked(std::bind(getter, settings)());

    // Set up two way connection
    connect(settings, notifier, checkBox, &QCheckBox::setChecked);
    connect(checkBox, &QCheckBox::toggled, settings, setter);
}

template <typename Func1, typename Func2, typename Func3>
void PreferencesDialog::MapSettingToGroupBox(QGroupBox *groupBox, Func1 getter, Func2 setter, Func3 notifier) const {
    // Get the value and set the checkbox state
    groupBox->setChecked(std::bind(getter, settings)());

    // Set up two way connection
    connect(settings, notifier, groupBox, &QGroupBox::setChecked);
    connect(groupBox, &QGroupBox::toggled, settings, setter);
}

void PreferencesDialog::populateTranslationComboBox() {
    SparkNotePadApplication *app = qobject_cast<SparkNotePadApplication *>(qApp);

    // Add the system default at the top
    ui->comboBoxTranslation->addItem(tr("<System Default>"), QStringLiteral(""));

    // TODO: sort this list and keep the system default at the top
    for (const auto &localeName : app->getTranslationManager()->availableTranslations()) {
        QLocale locale(localeName);
        const QString localeDisplay = TranslationManager::FormatLocaleTerritoryAndLanguage(locale);
        ui->comboBoxTranslation->addItem(localeDisplay, localeName);
    }

    // Select the current one
    int index = ui->comboBoxTranslation->findData(settings->translation());
    if (index != -1) {
        ui->comboBoxTranslation->setCurrentIndex(index);
    }
}

void PreferencesDialog::setupThemeAndFeatureSettings() {
    QGroupBox *gbxThemes =
        new QGroupBox(tr("🎨 Themes, Visual Styling & Modern Features"), ui->scrollAreaWidgetContents);
    QFormLayout *themeLayout = new QFormLayout(gbxThemes);
    themeLayout->setSpacing(10);
    themeLayout->setContentsMargins(14, 16, 14, 14);

    // 1. Theme Palette Selector
    comboBoxTheme = new QComboBox(gbxThemes);
    int selectedIdx = 0;
    int idx = 0;
    QString currentId = ThemeManager::instance()->currentPaletteId();
    for (const auto &pal : kAllThemePalettes) {
        QString label = QStringLiteral("[%1] %2 (%3)")
                            .arg(QString::fromUtf8(pal.family), QString::fromUtf8(pal.displayName),
                                 pal.isDark ? tr("Dark") : tr("Light"));
        comboBoxTheme->addItem(label, QString::fromLatin1(pal.id));
        if (pal.id == currentId) {
            selectedIdx = idx;
        }
        idx++;
    }
    comboBoxTheme->setCurrentIndex(selectedIdx);
    connect(comboBoxTheme, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        QString themeId = comboBoxTheme->itemData(index).toString();
        ThemeManager::instance()->setPalette(themeId, this);
    });

    QHBoxLayout *themeRowLayout = new QHBoxLayout();
    themeRowLayout->addWidget(comboBoxTheme, 1);

    btnToggleDarkLight = new QPushButton(tr("🔄 Toggle Dark/Light"), gbxThemes);
    btnToggleDarkLight->setToolTip(tr("Quickly switch between active dark and light modes"));
    connect(btnToggleDarkLight, &QPushButton::clicked, this, [this]() {
        ThemeManager::instance()->toggleTheme(this);
        QString currentId = ThemeManager::instance()->currentPaletteId();
        int found = comboBoxTheme->findData(currentId);
        if (found >= 0) {
            QSignalBlocker blocker(comboBoxTheme);
            comboBoxTheme->setCurrentIndex(found);
        }
    });
    themeRowLayout->addWidget(btnToggleDarkLight);
    themeLayout->addRow(tr("Active Theme:"), themeRowLayout);

    // 2. Window Opacity / Transparency
    QHBoxLayout *opacityLayout = new QHBoxLayout();
    sliderWindowOpacity = new QSlider(Qt::Horizontal, gbxThemes);
    sliderWindowOpacity->setRange(20, 100);
    spinBoxWindowOpacity = new QSpinBox(gbxThemes);
    spinBoxWindowOpacity->setRange(20, 100);
    spinBoxWindowOpacity->setSuffix(QStringLiteral("%"));

    int currentOpacity = settings->value("Window/Opacity", 100).toInt();
    sliderWindowOpacity->setValue(currentOpacity);
    spinBoxWindowOpacity->setValue(currentOpacity);

    connect(sliderWindowOpacity, &QSlider::valueChanged, spinBoxWindowOpacity, &QSpinBox::setValue);
    connect(spinBoxWindowOpacity, QOverload<int>::of(&QSpinBox::valueChanged), sliderWindowOpacity, &QSlider::setValue);
    connect(sliderWindowOpacity, &QSlider::valueChanged, this, [this](int val) {
        settings->setValue("Window/Opacity", val);
        if (QWidget *topWin = this->window()) {
            TransparentWindowManager::instance().setWindowTransparency(topWin, val);
        }
    });

    opacityLayout->addWidget(sliderWindowOpacity, 1);
    opacityLayout->addWidget(spinBoxWindowOpacity);
    themeLayout->addRow(tr("Window Opacity:"), opacityLayout);

    // 3. Tail -f Monitoring Debounce Interval
    spinBoxTailDebounce = new QSpinBox(gbxThemes);
    spinBoxTailDebounce->setRange(100, 5000);
    spinBoxTailDebounce->setSingleStep(100);
    spinBoxTailDebounce->setSuffix(tr(" ms"));
    int currentDebounce = settings->value("TailMonitoring/DebounceMs", 500).toInt();
    spinBoxTailDebounce->setValue(currentDebounce);
    connect(spinBoxTailDebounce, QOverload<int>::of(&QSpinBox::valueChanged), this,
            [this](int val) { settings->setValue("TailMonitoring/DebounceMs", val); });
    themeLayout->addRow(tr("Tail -f Reload Interval:"), spinBoxTailDebounce);

    // 4. Multi-Monitor Boundary Safety
    checkBoxMultiMonitorSafety =
        new QCheckBox(tr("Guard windows against offscreen placement across multi-DPI displays"), gbxThemes);
    bool currentSafety = settings->value("Screen/SafetyGuards", true).toBool();
    checkBoxMultiMonitorSafety->setChecked(currentSafety);
    connect(checkBoxMultiMonitorSafety, &QCheckBox::toggled, this,
            [this](bool checked) { settings->setValue("Screen/SafetyGuards", checked); });
    themeLayout->addRow(tr("Display Safety:"), checkBoxMultiMonitorSafety);

    // Insert at the top of scrollAreaWidgetContents layout
    ui->verticalLayout_4->insertWidget(0, gbxThemes);
}

void PreferencesDialog::updateThemeColors() {
    const ThemePalette *pal = ThemeManager::instance()->currentPalette();
    bool dark = ThemeManager::instance()->isDarkActive();

    QString winBg, cardBg, borderCol, accentCol, txtPri, txtSec;
    if (dark) {
        winBg = pal ? QString::fromLatin1(pal->win) : QStringLiteral("#0f172a");
        cardBg = pal ? QString::fromLatin1(pal->card) : QStringLiteral("#1e293b");
        borderCol = pal ? QString::fromLatin1(pal->border) : QStringLiteral("#334155");
        accentCol = pal ? QString::fromLatin1(pal->blue) : QStringLiteral("#38bdf8");
        txtPri = pal ? QString::fromLatin1(pal->txtPri) : QStringLiteral("#f8fafc");
        txtSec = pal ? QString::fromLatin1(pal->txtSec) : QStringLiteral("#94a3b8");
    } else {
        winBg = QStringLiteral("#f8fafc");
        cardBg = QStringLiteral("#ffffff");
        borderCol = QStringLiteral("#cbd5e1");
        accentCol = QStringLiteral("#2563eb");
        txtPri = QStringLiteral("#0f172a");
        txtSec = QStringLiteral("#475569");
    }

    QString prefStyle =
        QString(
            "QDialog {"
            "  background-color: %1;"
            "  color: %4;"
            "  font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', 'Segoe UI', Roboto, sans-serif;"
            "  font-size: 13px;"
            "}"
            "QScrollArea {"
            "  background-color: %1;"
            "  border: none;"
            "}"
            "QWidget#scrollAreaWidgetContents {"
            "  background-color: %1;"
            "}"
            "QGroupBox {"
            "  background-color: %2;"
            "  border: 1px solid %3;"
            "  border-left: 5px solid %6;"
            "  border-radius: 8px;"
            "  margin-top: 14px;"
            "  padding: 16px;"
            "  font-weight: 600;"
            "  color: %4;"
            "}"
            "QGroupBox::title {"
            "  subcontrol-origin: margin;"
            "  subcontrol-position: top left;"
            "  padding: 2px 8px;"
            "  color: %4;"
            "}"
            "QLabel {"
            "  color: %4;"
            "  font-size: 13px;"
            "}"
            "QCheckBox, QRadioButton {"
            "  color: %4;"
            "  spacing: 8px;"
            "  font-size: 13px;"
            "}"
            "QComboBox, QFontComboBox, QSpinBox, QLineEdit {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: 1px solid %3;"
            "  border-radius: 6px;"
            "  padding: 6px 10px;"
            "  font-size: 13px;"
            "}"
            "QComboBox:focus, QFontComboBox:focus, QSpinBox:focus, QLineEdit:focus {"
            "  border-color: %6;"
            "}"
            "QPushButton, QToolButton {"
            "  background-color: %2;"
            "  color: %4;"
            "  border: 1px solid %3;"
            "  border-radius: 6px;"
            "  padding: 6px 16px;"
            "  font-weight: 600;"
            "}"
            "QPushButton:hover, QToolButton:hover {"
            "  border-color: %6;"
            "  color: %6;"
            "}"
            "QPushButton:pressed, QToolButton:pressed {"
            "  background-color: %6;"
            "  color: #ffffff;"
            "}"
            "QScrollBar:vertical {"
            "  background: %1;"
            "  width: 10px;"
            "  margin: 0px;"
            "}"
            "QScrollBar::handle:vertical {"
            "  background: %3;"
            "  min-height: 20px;"
            "  border-radius: 5px;"
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "  height: 0px;"
            "}")
            .arg(winBg, cardBg, borderCol, txtPri, txtSec, accentCol);

    setStyleSheet(prefStyle);
}
