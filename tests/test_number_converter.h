#pragma once
#include <QObject>
#include <QTest>
#include "utils/NumberConverter.h"

class TestNumberConverter : public QObject
{
    Q_OBJECT

private slots:
    void testConversions()
    {
        bool ok = false;
        qint64 dec = NumberConverter::parseNumber("255", &ok);
        QVERIFY(ok);
        QCOMPARE(dec, 255LL);

        qint64 hex = NumberConverter::parseNumber("0xFF", &ok);
        QVERIFY(ok);
        QCOMPARE(hex, 255LL);

        qint64 bin = NumberConverter::parseNumber("0b1010", &ok);
        QVERIFY(ok);
        QCOMPARE(bin, 10LL);

        qint64 oct = NumberConverter::parseNumber("0o77", &ok);
        QVERIFY(ok);
        QCOMPARE(oct, 63LL);

        QCOMPARE(NumberConverter::toHex(255), QString("0xFF"));
        QCOMPARE(NumberConverter::toBinary(10), QString("0b1010"));
        QCOMPARE(NumberConverter::toOctal(63), QString("0o77"));
    }
};
