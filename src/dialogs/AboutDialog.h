#pragma once
/*
 * This file is part of SparkNotePad.
 * Copyright 2026 Harry DS Alsyundawy (SparkNotePad Maintainer)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

class QTabWidget;
class QLabel;
class QTextBrowser;

class AboutDialog : public QDialog {
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override = default;

private slots:
    void copySystemInfoToClipboard() const;
    void updateThemeColors();

private:
    void setupUi();
    QWidget *createAboutTab();
    QWidget *createMaintainerTab();
    QWidget *createCreditsTab();
    QWidget *createEnvironmentTab();
    QWidget *createShortcutsTab();
    QWidget *createLicenseTab();

    QTabWidget *m_tabWidget = nullptr;
    QLabel *m_titleLabel = nullptr;
    QLabel *m_badgesLabel = nullptr;
    QLabel *m_metaLabel = nullptr;
    QLabel *m_noticeBox = nullptr;
    QLabel *m_diagnosticsCard = nullptr;
    QTextBrowser *m_maintainerBrowser = nullptr;
    QTextBrowser *m_creditsBrowser = nullptr;
    QTextBrowser *m_shortcutsBrowser = nullptr;
    QTextBrowser *m_licenseBrowser = nullptr;
};

#endif  // ABOUTDIALOG_H
