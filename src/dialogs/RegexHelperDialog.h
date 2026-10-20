#pragma once
/*
 * This file is part of SparkNotePad.
 *
 * PCRE2 Regular Expression Reference and Live Interactive Tester.
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef REGEXHELPERDIALOG_H
#define REGEXHELPERDIALOG_H

#include <QDialog>

namespace Ui {
class RegexHelperDialog;
}

class RegexHelperDialog : public QDialog {
    Q_OBJECT

public:
    explicit RegexHelperDialog(QWidget *parent = nullptr, const QString &initialPattern = QString());
    ~RegexHelperDialog() override;

private slots:
    void updateRegexTest();

private:
    void populateReferenceTree();

    Ui::RegexHelperDialog *ui;
};

#endif  // REGEXHELPERDIALOG_H
