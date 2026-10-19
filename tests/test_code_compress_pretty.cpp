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

// Implement runAllTests in this translation unit
int runAllTests(int argc, char *argv[])
{
    int status = 0;
    {
        TestBase64 t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestNumberConverter t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestGuidTimestamp t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestCaseConverter t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestMathEvaluator t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestCryptoManager t;
        status |= QTest::qExec(&t, argc, argv);
    }
    {
        TestCodeCompressPretty t;
        status |= QTest::qExec(&t, argc, argv);
    }
    return status;
}
