/*
 * This file is part of SparkNotePad.
 *
 * Adapted from Notepad4 auto-completion & context suggestions logic.
 * Source: zufuliu/Notepad4 (src/EditAutoC.cpp)
 *
 * SparkNotePad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "AutoCompletion.h"
#include "SparkNotePadApplication.h"

#include <QApplication>
#include <QSet>
#include <QString>
#include <Qt>

using namespace Scintilla;

AutoCompletion::AutoCompletion(ScintillaNext *editor) : EditorDecorator(editor) {
    setObjectName(QStringLiteral("AutoCompletion"));

    editor->autoCSetOrder(SC_ORDER_PERFORMSORT);
    editor->autoCSetMaxHeight(10);
    editor->autoCSetIgnoreCase(true);
    editor->autoCSetFillUps(" (");

    connect(editor, &ScintillaNext::lexerChanged, this, [this]() {
        SparkNotePadApplication *app = qobject_cast<SparkNotePadApplication *>(qApp);
        if (app) {
            keywords = app->getLanguageKeywords(this->editor->languageName);
        }
    });
}

void AutoCompletion::notify(const NotificationData *pscn) {
    if (pscn->nmhdr.code == Notification::CharAdded && isEnabled()) {
        if (editor->autoCActive())
            return;

        showAutoCompletion();
    }
}

void AutoCompletion::showAutoCompletion() {
    if (!editor)
        return;

    int curPos = editor->currentPos();
    int startPos = editor->wordStartPosition(curPos, true);
    int endPos = editor->wordEndPosition(curPos, true);

    // Trigger with 2 or more characters for rapid response
    if ((curPos - startPos) < 2)
        return;

    const QByteArray current_word = editor->get_text_range(startPos, curPos);
    const bool isValidIdentifier = std::all_of(current_word.cbegin(), current_word.cend(), [](char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_';
    });
    if (!isValidIdentifier)
        return;

    const QByteArray regex = "\\b" + current_word + "[\\w]*";
    QSet<QByteArray> words;

    editor->setSearchFlags(SCFIND_REGEXP);

    // For large files (> 1MB or > 5,000 lines), constrain search to viewport +/- 200 lines
    Sci_PositionCR searchStart = 0;
    Sci_PositionCR searchEnd = static_cast<Sci_PositionCR>(editor->length());
    if (editor->length() > 1024 * 1024 || editor->lineCount() > 5000) {
        const int firstLine = qMax(0, static_cast<int>(editor->firstVisibleLine()) - 200);
        const int lastLine = qMin(static_cast<int>(editor->lineCount()),
                                  static_cast<int>(editor->firstVisibleLine() + editor->linesOnScreen() + 200));
        searchStart = static_cast<Sci_PositionCR>(editor->positionFromLine(firstLine));
        searchEnd = static_cast<Sci_PositionCR>(editor->lineEndPosition(lastLine));
    }

    int wordCount = 0;
    // Find words in buffer before caret
    if (searchStart < startPos) {
        editor->forEachMatchInRange(regex,
                                    [&](int start, int end) {
                                        words.insert(editor->get_text_range(start, end));
                                        if (++wordCount >= 100)
                                            return startPos;
                                        return end;
                                    },
                                    {searchStart, startPos});
    }

    // Find words in buffer after caret
    if (wordCount < 100 && endPos < searchEnd) {
        editor->forEachMatchInRange(regex,
                                    [&](int start, int end) {
                                        words.insert(editor->get_text_range(start, end));
                                        if (++wordCount >= 100)
                                            return static_cast<int>(searchEnd);
                                        return end;
                                    },
                                    {endPos, searchEnd});
    }

    // Add matching language keywords
    const QString currentWordStr = QString::fromUtf8(current_word);
    for (const QString &kw : keywords) {
        if (kw.startsWith(currentWordStr, Qt::CaseInsensitive)) {
            words.insert(kw.toUtf8());
        }
    }

    if (!words.isEmpty()) {
        QList<QByteArray> sortedWords = words.values();
        std::sort(sortedWords.begin(), sortedWords.end(),
                  [](const QByteArray &a, const QByteArray &b) { return a.toLower() < b.toLower(); });
        editor->autoCShow(current_word.length(), sortedWords.join(' '));
    }
}
