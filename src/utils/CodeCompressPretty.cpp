/*
 * This file is part of NoteSciPad.
 *
 * Adapted from Notepad4 (zufuliu/notepad4) Compress and Pretty Code features.
 * Source: zufuliu/notepad4 (src/Edit.cpp, src/Styles.cpp)
 *
 * NoteSciPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "CodeCompressPretty.h"
#include "ScintillaNext.h"

#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QRegularExpression>

QString CodeCompressPretty::languageName(Language lang)
{
    switch (lang) {
    case Json: return QStringLiteral("JSON");
    case Json5: return QStringLiteral("JSON5");
    case Css: return QStringLiteral("CSS");
    case JavaScript: return QStringLiteral("JavaScript");
    default: return QStringLiteral("Auto");
    }
}

CodeCompressPretty::Language CodeCompressPretty::detectLanguage(ScintillaNext *editor, const QString &sample)
{
    if (editor && editor->isFile()) {
        const QString ext = QFileInfo(editor->filePath()).suffix().toLower();
        if (ext == QLatin1String("json")) return Json;
        if (ext == QLatin1String("json5")) return Json5;
        if (ext == QLatin1String("css") || ext == QLatin1String("scss") || ext == QLatin1String("less")) return Css;
        if (ext == QLatin1String("js") || ext == QLatin1String("mjs") || ext == QLatin1String("cjs") || ext == QLatin1String("ts") || ext == QLatin1String("jsx")) return JavaScript;
    }

    const QString text = sample.trimmed();
    if ((text.startsWith('{') || text.startsWith('[')) && (text.endsWith('}') || text.endsWith(']'))) {
        if (text.contains(QLatin1String("//")) || text.contains(QLatin1String("/*")) || text.contains('\'')) {
            return Json5;
        }
        return Json;
    }

    if (text.contains('{') && text.contains('}') && text.contains(':') && text.contains(';')) {
        if (text.contains(QStringLiteral("function")) || text.contains(QStringLiteral("const ")) || text.contains(QStringLiteral("let ")) || text.contains(QStringLiteral("var "))) {
            return JavaScript;
        }
        return Css;
    }

    if (text.contains(QStringLiteral("function")) || text.contains(QStringLiteral("const ")) || text.contains(QStringLiteral("let ")) || text.contains(QStringLiteral("=>"))) {
        return JavaScript;
    }

    return Json;
}

static QString cleanJson5(const QString &input)
{
    QString text = input;

    // 1. Remove single-line comments // ...
    static const QRegularExpression singleLineComments(QStringLiteral("//[^\r\n]*"));
    text.remove(singleLineComments);

    // 2. Remove multi-line comments /* ... */
    static const QRegularExpression multiLineComments(QStringLiteral("/\*[\s\S]*?\*/"));
    text.remove(multiLineComments);

    // 3. Remove trailing commas before } or ]
    static const QRegularExpression trailingCommas(QStringLiteral(",\s*([\}\]])"));
    text.replace(trailingCommas, QStringLiteral("\\1"));

    // 4. Quote unquoted object keys: { key: ... } or , key: ...
    static const QRegularExpression unquotedKeys(QStringLiteral("([{,]\s*)([a-zA-Z_$][a-zA-Z0-9_$]*)\s*:"));
    text.replace(unquotedKeys, QStringLiteral("\\1\"\\2\":"));

    // 5. Convert single-quoted strings: 'value' -> "value"
    static const QRegularExpression singleQuotes(QStringLiteral("'([^'\\\]*(?:\\.[^'\\\]*)*)'"));
    text.replace(singleQuotes, QStringLiteral("\"\\1\""));

    return text.trimmed();
}

CodeCompressPretty::Result CodeCompressPretty::prettifyJson(const QString &text, bool isJson5)
{
    QJsonParseError err;
    QByteArray utf8 = text.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(utf8, &err);

    if (doc.isNull() || isJson5) {
        const QString cleaned = cleanJson5(text);
        doc = QJsonDocument::fromJson(cleaned.toUtf8(), &err);
    }

    if (doc.isNull()) {
        return {false, false, Json, text, QString(),
                QStringLiteral("JSON Parse Error at line %1, offset %2: %3")
                    .arg(text.left(err.offset).count('\n') + 1)
                    .arg(err.offset)
                    .arg(err.errorString())};
    }

    const QString formatted = QString::fromUtf8(doc.toJson(QJsonDocument::Indented));
    return {true, false, isJson5 ? Json5 : Json, text, formatted, QString()};
}

CodeCompressPretty::Result CodeCompressPretty::compressJson(const QString &text, bool isJson5)
{
    QJsonParseError err;
    QByteArray utf8 = text.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(utf8, &err);

    if (doc.isNull() || isJson5) {
        const QString cleaned = cleanJson5(text);
        doc = QJsonDocument::fromJson(cleaned.toUtf8(), &err);
    }

    if (doc.isNull()) {
        return {false, false, Json, text, QString(),
                QStringLiteral("JSON Parse Error at line %1, offset %2: %3")
                    .arg(text.left(err.offset).count('\n') + 1)
                    .arg(err.offset)
                    .arg(err.errorString())};
    }

    const QString formatted = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    return {true, false, isJson5 ? Json5 : Json, text, formatted, QString()};
}

CodeCompressPretty::Result CodeCompressPretty::compressCss(const QString &text)
{
    QString res = text;

    // 1. Strip comments
    static const QRegularExpression comments(QStringLiteral("/\*[\s\S]*?\*/"));
    res.remove(comments);

    // 2. Collapse whitespace
    static const QRegularExpression whitespaces(QStringLiteral("\s+"));
    res.replace(whitespaces, QStringLiteral(" "));

    // 3. Remove space around delimiters
    static const QRegularExpression spaceAround(QStringLiteral("\s*([\{\}:;,>+~])\s*"));
    res.replace(spaceAround, QStringLiteral("\\1"));

    // 4. Remove trailing semicolon before }
    static const QRegularExpression trailingSemi(QStringLiteral(";\}"));
    res.replace(trailingSemi, QStringLiteral("}"));

    return {true, false, Css, text, res.trimmed(), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::prettifyCss(const QString &text)
{
    auto comp = compressCss(text);
    const QString minified = comp.formatted;

    QString res;
    res.reserve(minified.length() * 2);
    int indent = 0;

    for (int i = 0; i < minified.length(); ++i) {
        const QChar ch = minified[i];
        if (ch == '{') {
            res += QStringLiteral(" {\n");
            indent++;
            res += QString(indent * 4, ' ');
        } else if (ch == '}') {
            res += QStringLiteral("\n");
            indent = qMax(0, indent - 1);
            res += QString(indent * 4, ' ');
            res += QStringLiteral("}\n\n");
        } else if (ch == ';') {
            res += QStringLiteral(";\n");
            res += QString(indent * 4, ' ');
        } else if (ch == ':') {
            res += QStringLiteral(": ");
        } else if (ch == ',') {
            res += QStringLiteral(", ");
        } else {
            res += ch;
        }
    }

    return {true, false, Css, text, res.trimmed() + QStringLiteral("\n"), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::compressJs(const QString &text)
{
    QString res;
    res.reserve(text.length());

    bool inSingle = false;
    bool inDouble = false;
    bool inBacktick = false;
    const int n = text.length();

    for (int i = 0; i < n; ++i) {
        const QChar c = text[i];
        if (inSingle) {
            res += c;
            if (c == '\\' && i + 1 < n) {
                res += text[++i];
            } else if (c == '\'') {
                inSingle = false;
            }
        } else if (inDouble) {
            res += c;
            if (c == '\\' && i + 1 < n) {
                res += text[++i];
            } else if (c == '\"') {
                inDouble = false;
            }
        } else if (inBacktick) {
            res += c;
            if (c == '\\' && i + 1 < n) {
                res += text[++i];
            } else if (c == '`') {
                inBacktick = false;
            }
        } else if (c == '\'') {
            inSingle = true;
            res += c;
        } else if (c == '\"') {
            inDouble = true;
            res += c;
        } else if (c == '`') {
            inBacktick = true;
            res += c;
        } else if (c == '/' && i + 1 < n && text[i + 1] == '/') {
            // Skip single-line comment
            i += 2;
            while (i < n && text[i] != '\n') i++;
        } else if (c == '/' && i + 1 < n && text[i + 1] == '*') {
            // Skip multi-line comment
            i += 2;
            while (i + 1 < n && !(text[i] == '*' && text[i + 1] == '/')) i++;
            i++;
        } else {
            res += c;
        }
    }

    // Collapse multiple whitespace outside strings
    static const QRegularExpression multiline(QStringLiteral("\s*\n\s*"));
    res.replace(multiline, QStringLiteral(" "));
    static const QRegularExpression extraSpaces(QStringLiteral(" {2,}"));
    res.replace(extraSpaces, QStringLiteral(" "));

    return {true, false, JavaScript, text, res.trimmed(), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::prettifyJs(const QString &text)
{
    // Token-aware simple formatter
    QString res;
    res.reserve(text.length() * 2);

    int indent = 0;
    bool inSingle = false;
    bool inDouble = false;
    bool inBacktick = false;
    bool inForParen = false;
    int parenDepth = 0;
    const int n = text.length();

    for (int i = 0; i < n; ++i) {
        const QChar c = text[i];
        if (inSingle) {
            res += c;
            if (c == '\\' && i + 1 < n) res += text[++i];
            else if (c == '\'') inSingle = false;
        } else if (inDouble) {
            res += c;
            if (c == '\\' && i + 1 < n) res += text[++i];
            else if (c == '\"') inDouble = false;
        } else if (inBacktick) {
            res += c;
            if (c == '\\' && i + 1 < n) res += text[++i];
            else if (c == '`') inBacktick = false;
        } else if (c == '\'') {
            inSingle = true;
            res += c;
        } else if (c == '\"') {
            inDouble = true;
            res += c;
        } else if (c == '`') {
            inBacktick = true;
            res += c;
        } else if (c == '(') {
            parenDepth++;
            res += c;
        } else if (c == ')') {
            parenDepth = qMax(0, parenDepth - 1);
            res += c;
        } else if (c == '{') {
            res += QStringLiteral(" {\n");
            indent++;
            res += QString(indent * 4, ' ');
        } else if (c == '}') {
            res += QStringLiteral("\n");
            indent = qMax(0, indent - 1);
            res += QString(indent * 4, ' ');
            res += QStringLiteral("}\n");
            res += QString(indent * 4, ' ');
        } else if (c == ';' && parenDepth == 0) {
            res += QStringLiteral(";\n");
            res += QString(indent * 4, ' ');
        } else {
            res += c;
        }
    }

    return {true, false, JavaScript, text, res.trimmed() + QStringLiteral("\n"), QString()};
}

CodeCompressPretty::Result CodeCompressPretty::prettifyString(const QString &text, Language lang)
{
    if (lang == Auto) {
        lang = detectLanguage(nullptr, text);
    }

    switch (lang) {
    case Json:
        return prettifyJson(text, false);
    case Json5:
        return prettifyJson(text, true);
    case Css:
        return prettifyCss(text);
    case JavaScript:
        return prettifyJs(text);
    default:
        return prettifyJson(text, false);
    }
}

CodeCompressPretty::Result CodeCompressPretty::compressString(const QString &text, Language lang)
{
    if (lang == Auto) {
        lang = detectLanguage(nullptr, text);
    }

    switch (lang) {
    case Json:
        return compressJson(text, false);
    case Json5:
        return compressJson(text, true);
    case Css:
        return compressCss(text);
    case JavaScript:
        return compressJs(text);
    default:
        return compressJson(text, false);
    }
}

CodeCompressPretty::Result CodeCompressPretty::prettify(ScintillaNext *editor, Language lang)
{
    if (!editor) {
        return {false, false, lang, QString(), QString(), QStringLiteral("No active editor")};
    }
    if (editor->readOnly()) {
        return {false, false, lang, QString(), QString(), QStringLiteral("Document is read-only")};
    }

    int startPos = editor->selectionStart();
    int endPos = editor->selectionEnd();
    const bool wholeDoc = (startPos == endPos);

    if (wholeDoc) {
        startPos = 0;
        endPos = editor->length();
    }

    const QString text = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    if (text.trimmed().isEmpty()) {
        return {false, wholeDoc, lang, text, QString(), QStringLiteral("Selection or document is empty")};
    }

    if (lang == Auto) {
        lang = detectLanguage(editor, text);
    }

    auto result = prettifyString(text, lang);
    result.wholeDocument = wholeDoc;

    if (result.success) {
        editor->beginUndoAction();
        editor->setTargetRange(startPos, endPos);
        const QByteArray utf8 = result.formatted.toUtf8();
        editor->replaceTarget(utf8.length(), utf8.constData());
        editor->setSelection(startPos, startPos + utf8.length());
        editor->endUndoAction();
    }

    return result;
}

CodeCompressPretty::Result CodeCompressPretty::compress(ScintillaNext *editor, Language lang)
{
    if (!editor) {
        return {false, false, lang, QString(), QString(), QStringLiteral("No active editor")};
    }
    if (editor->readOnly()) {
        return {false, false, lang, QString(), QString(), QStringLiteral("Document is read-only")};
    }

    int startPos = editor->selectionStart();
    int endPos = editor->selectionEnd();
    const bool wholeDoc = (startPos == endPos);

    if (wholeDoc) {
        startPos = 0;
        endPos = editor->length();
    }

    const QString text = QString::fromUtf8(editor->get_text_range(startPos, endPos));
    if (text.trimmed().isEmpty()) {
        return {false, wholeDoc, lang, text, QString(), QStringLiteral("Selection or document is empty")};
    }

    if (lang == Auto) {
        lang = detectLanguage(editor, text);
    }

    auto result = compressString(text, lang);
    result.wholeDocument = wholeDoc;

    if (result.success) {
        editor->beginUndoAction();
        editor->setTargetRange(startPos, endPos);
        const QByteArray utf8 = result.formatted.toUtf8();
        editor->replaceTarget(utf8.length(), utf8.constData());
        editor->setSelection(startPos, startPos + utf8.length());
        editor->endUndoAction();
    }

    return result;
}
