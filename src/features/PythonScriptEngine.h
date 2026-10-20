#pragma once
/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef PYTHONSCRIPTENGINE_H
#define PYTHONSCRIPTENGINE_H

#include <QString>
#include <QStringList>

class ScintillaNext;

struct PythonExecutionResult {
    bool success = false;
    int exitCode = -1;
    QString standardOutput;
    QString standardError;
    QString combinedOutput;
    QString errorMessage;
};

class PythonScriptEngine {
public:
    static QString getPythonInterpreterPath();
    static QString getPythonVersion();
    static bool isPython313OrNewer();

    static PythonExecutionResult executeCode(const QString &pythonCode, const QStringList &arguments = QStringList());
    static PythonExecutionResult executeFile(const QString &scriptFilePath,
                                             const QStringList &arguments = QStringList());
    static bool executeInEditor(ScintillaNext *editor, bool replaceSelection, QString *outMessage = nullptr);
};

#endif  // PYTHONSCRIPTENGINE_H
