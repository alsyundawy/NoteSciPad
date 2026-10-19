#include <QObject>
#include <QTest>
#include "utils/CaseConverter.h"

class TestCaseConverter : public QObject
{
    Q_OBJECT

private slots:
    void testConversions()
    {
        QCOMPARE(CaseConverter::toUpperCase("NoteSciPad"), QString("NOTESCIPAD"));
        QCOMPARE(CaseConverter::toLowerCase("NoteSciPad"), QString("notescipad"));
        QCOMPARE(CaseConverter::toTitleCase("hello world"), QString("Hello World"));
        QCOMPARE(CaseConverter::toCamelCase("hello_world_text"), QString("helloWorldText"));
        QCOMPARE(CaseConverter::toSnakeCase("helloWorldText"), QString("hello_world_text"));
        QCOMPARE(CaseConverter::toKebabCase("helloWorldText"), QString("hello-world-text"));
    }
};
