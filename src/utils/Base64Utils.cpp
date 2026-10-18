/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 Base64 conversion utilities.
 * Source: zufuliu/Notepad4 (src/Edit.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "Base64Utils.h"
#include "ScintillaNext.h"

Base64Utils::Result Base64Utils::encode(ScintillaNext *editor)
{
    if (!editor) {
        return {false, false, QStringLiteral("No active editor")};
    }

    if (editor->selectionEmpty()) {
        const int docLength = editor->length();
        if (docLength == 0) {
            return {false, true, QStringLiteral("Document is empty")};
        }

        const QByteArray raw = editor->get_text_range(0, docLength);
        const QByteArray encoded = raw.toBase64();

        editor->beginUndoAction();
        editor->setText(encoded.constData());
        editor->endUndoAction();

        return {true, true, QString()};
    }

    const QByteArray selText = editor->getSelText();
    const QByteArray encoded = selText.toBase64();

    editor->beginUndoAction();
    editor->replaceSel(encoded.constData());
    editor->endUndoAction();

    return {true, false, QString()};
}

Base64Utils::Result Base64Utils::decode(ScintillaNext *editor)
{
    if (!editor) {
        return {false, false, QStringLiteral("No active editor")};
    }

    if (editor->selectionEmpty()) {
        const int docLength = editor->length();
        if (docLength == 0) {
            return {false, true, QStringLiteral("Document is empty")};
        }

        const QByteArray raw = editor->get_text_range(0, docLength).trimmed();
        auto result = QByteArray::fromBase64Encoding(raw, QByteArray::AbortOnBase64DecodingErrors);
        if (!result) {
            return {false, true, QStringLiteral("Invalid Base64 content or bad padding")};
        }

        editor->beginUndoAction();
        editor->setText(result->constData());
        editor->endUndoAction();

        return {true, true, QString()};
    }

    const QByteArray selText = editor->getSelText().trimmed();
    auto result = QByteArray::fromBase64Encoding(selText, QByteArray::AbortOnBase64DecodingErrors);
    if (!result) {
        return {false, false, QStringLiteral("Invalid Base64 selection or bad padding")};
    }

    editor->beginUndoAction();
    editor->replaceSel(result->constData());
    editor->endUndoAction();

    return {true, false, QString()};
}
