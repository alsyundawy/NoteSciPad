#pragma once
#include <QObject>
#include <QTest>
#include "utils/CodeCompressPretty.h"

class TestCodeCompressPretty : public QObject
{
    Q_OBJECT

private slots:
    void testJsonMinifyPrettify()
    {
        QString messy = "{   \"key\":   \"value\" ,  \"number\": 42 }";
        QString mini = CodeCompressPretty::minifyJson(messy);
        QVERIFY(!mini.contains("  "));
        QVERIFY(mini.contains("\"key\":\"value\""));

        QString pretty = CodeCompressPretty::prettifyJson(mini);
        QVERIFY(pretty.contains("\n"));
        QVERIFY(pretty.contains("    \"key\""));
    }

    void testCssMinify()
    {
        QString css = "/* comment */ body { color: red; margin: 0px; }";
        QString mini = CodeCompressPretty::minifyCss(css);
        QVERIFY(!mini.contains("comment"));
        QVERIFY(mini.contains("body{color:red;margin:0px;}"));
    }
};
