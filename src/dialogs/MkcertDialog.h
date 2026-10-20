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

#ifndef MKCERTDIALOG_H
#define MKCERTDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;
class QTextEdit;

class MkcertDialog : public QDialog {
    Q_OBJECT

public:
    explicit MkcertDialog(QWidget *parent = nullptr);
    ~MkcertDialog() override = default;

private slots:
    void onInstallCaClicked();
    void onBrowseDirClicked();
    void onGenerateClicked();
    void checkToolsStatus();

private:
    void applyTheme();

    QLineEdit *m_domainsEdit;
    QLineEdit *m_dirEdit;
    QPushButton *m_browseDirBtn;
    QPushButton *m_installCaBtn;
    QPushButton *m_generateBtn;
    QPushButton *m_closeBtn;
    QLabel *m_statusLabel;
    QTextEdit *m_outputView;

    QString m_mkcertPath;
    QString m_opensslPath;
};

#endif  // MKCERTDIALOG_H
