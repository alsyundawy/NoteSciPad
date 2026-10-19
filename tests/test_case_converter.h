#pragma once
#include <QObject>
#include <QTest>
#include "utils/CaseConverter.h"

class TestCaseConverter : public QObject
{
    Q_OBJECT

private slots:
    void testConversions()
    {
        QCOMPARE(CaseConverter::toUpper("NoteSciPad"), QString("NOTESCIPAD"));
        QCOMPARE(CaseConverter::toLower("NoteSciPad"), QString("notescipad"));
        QCOMPARE(CaseConverter::toTitleCase("hello world"), QString("Hello World"));
        QCOMPARE(CaseConverter::toInvertCase("Hello World"), QString("hELLO wORLD"));
        QCOMPARE(CaseConverter::toSentenceCase("hello world. good morning."), QString("Hello world. Good morning."));
    }
};
