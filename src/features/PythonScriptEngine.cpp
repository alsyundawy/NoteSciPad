/*
 * This file is part of NoteSciPad.
 * Copyright 2026 Harry DS Alsyundawy (NoteSciPad Maintainer)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "PythonScriptEngine.h"
#include "ScintillaNext.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>

QString PythonScriptEngine::getPythonInterpreterPath() {
    // Search candidates prioritizing Python 3.14, 3.13, then standard python3
    const QStringList candidateNames = {"python3.14", "python3.13", "python3", "python"};

    const QStringList standardPrefixes = {"/usr/local/bin", "/opt/homebrew/bin", "/usr/bin",
                                          "C:/Program Files/Python313", "C:/Program Files/Python314"};

    for (const QString &candidate : candidateNames) {
        QString found = QStandardPaths::findExecutable(candidate);
        if (!found.isEmpty()) {
            return found;
        }

        for (const QString &prefix : standardPrefixes) {
            QString fullPath = QDir(prefix).filePath(candidate);
#if defined(Q_OS_WIN)
            if (!fullPath.endsWith(".exe", Qt::CaseInsensitive)) {
                fullPath += ".exe";
            }
#endif
            if (QFileInfo::exists(fullPath) && QFileInfo(fullPath).isExecutable()) {
                return fullPath;
            }
        }
    }

    return QStringLiteral("python3");
}

QString PythonScriptEngine::getPythonVersion() {
    QString interpreter = getPythonInterpreterPath();
    QProcess process;
    process.start(interpreter, QStringList() << "--version");
    if (process.waitForFinished(3000)) {
        QString ver = QString::fromUtf8(process.readAllStandardOutput()).trimmed();
        if (ver.isEmpty()) {
            ver = QString::fromUtf8(process.readAllStandardError()).trimmed();
        }
        return ver;
    }
    return QStringLiteral("Python 3.13+ (detected)");
}

bool PythonScriptEngine::isPython313OrNewer() {
    QString ver = getPythonVersion();
    if (ver.contains("3.13") || ver.contains("3.14") || ver.contains("3.15")) {
        return true;
    }
    return false;
}

PythonExecutionResult PythonScriptEngine::executeCode(const QString &pythonCode, const QStringList &arguments) {
    PythonExecutionResult result;
    QString interpreter = getPythonInterpreterPath();

    QProcess process;
    QStringList args;
    args << "-u" << "-c" << pythonCode;
    args.append(arguments);

    process.start(interpreter, args);
    if (!process.waitForStarted(3000)) {
        result.success = false;
        result.errorMessage = QStringLiteral("Failed to launch Python interpreter: ") + interpreter;
        result.standardError = result.errorMessage;
        return result;
    }

    // Safety timeout: 10 seconds max to prevent CPU hanging
    if (!process.waitForFinished(10000)) {
        process.kill();
        process.waitForFinished(1000);
        result.success = false;
        result.errorMessage = QStringLiteral("Python script execution timed out (limit: 10s).");
        result.standardError = result.errorMessage;
        return result;
    }

    result.exitCode = process.exitCode();
    result.success = (result.exitCode == 0);
    result.standardOutput = QString::fromUtf8(process.readAllStandardOutput());
    result.standardError = QString::fromUtf8(process.readAllStandardError());

    if (!result.standardOutput.isEmpty() && !result.standardError.isEmpty()) {
        result.combinedOutput = result.standardOutput + "\n[STDERR]\n" + result.standardError;
    } else if (!result.standardOutput.isEmpty()) {
        result.combinedOutput = result.standardOutput;
    } else {
        result.combinedOutput = result.standardError;
    }

    return result;
}

PythonExecutionResult PythonScriptEngine::executeFile(const QString &scriptFilePath, const QStringList &arguments) {
    PythonExecutionResult result;
    if (!QFileInfo::exists(scriptFilePath)) {
        result.success = false;
        result.errorMessage = QStringLiteral("Script file not found: ") + scriptFilePath;
        result.standardError = result.errorMessage;
        return result;
    }

    QString interpreter = getPythonInterpreterPath();
    QProcess process;
    QStringList args;
    args << "-u" << scriptFilePath;
    args.append(arguments);

    process.start(interpreter, args);
    if (!process.waitForStarted(3000)) {
        result.success = false;
        result.errorMessage = QStringLiteral("Failed to launch Python interpreter: ") + interpreter;
        result.standardError = result.errorMessage;
        return result;
    }

    if (!process.waitForFinished(15000)) {
        process.kill();
        process.waitForFinished(1000);
        result.success = false;
        result.errorMessage = QStringLiteral("Python script execution timed out (limit: 15s).");
        result.standardError = result.errorMessage;
        return result;
    }

    result.exitCode = process.exitCode();
    result.success = (result.exitCode == 0);
    result.standardOutput = QString::fromUtf8(process.readAllStandardOutput());
    result.standardError = QString::fromUtf8(process.readAllStandardError());
    result.combinedOutput = result.standardOutput.isEmpty() ? result.standardError : result.standardOutput;

    return result;
}

bool PythonScriptEngine::executeInEditor(ScintillaNext *editor, bool replaceSelection, QString *outMessage) {
    if (!editor) {
        if (outMessage) {
            *outMessage = QStringLiteral("No active editor.");
        }
        return false;
    }

    QString codeToRun;
    bool hasSelection = (editor->selectionStart() != editor->selectionEnd());
    if (hasSelection) {
        codeToRun = QString::fromUtf8(editor->get_text_range(editor->selectionStart(), editor->selectionEnd()));
    } else {
        codeToRun = QString::fromUtf8(editor->get_text_range(0, editor->length()));
    }

    if (codeToRun.trimmed().isEmpty()) {
        if (outMessage) {
            *outMessage = QStringLiteral("No code to execute.");
        }
        return false;
    }

    PythonExecutionResult execRes = executeCode(codeToRun);
    if (!execRes.success && execRes.combinedOutput.isEmpty()) {
        if (outMessage) {
            *outMessage = QStringLiteral("Execution error: exit code ") + QString::number(execRes.exitCode);
        }
        return false;
    }

    QString output = execRes.combinedOutput;
    QByteArray utf8 = output.toUtf8();

    if (replaceSelection && hasSelection) {
        editor->replaceSel(utf8.constData());
    } else {
        // Append output to the end of the document
        sptr_t docEnd = editor->length();
        QByteArray newlinePrefix = "\n";
        editor->insertText(docEnd, newlinePrefix + utf8.constData());
        editor->gotoPos(docEnd + utf8.length() + 1);
    }

    if (outMessage) {
        *outMessage = execRes.success ? QStringLiteral("Script executed successfully.")
                                      : QStringLiteral("Execution returned warnings/errors.");
    }

    return execRes.success;
}
