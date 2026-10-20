#pragma once
#include <QObject>
#include <QTest>
#include "utils/Base64Utils.h"

class TestBase64 : public QObject {
    Q_OBJECT

private slots:
    void testStandardVectors() {
        // RFC 4648 standard test vectors
        QCOMPARE(Base64Utils::encode(""), QString(""));
        QCOMPARE(Base64Utils::encode("f"), QString("Zg=="));
        QCOMPARE(Base64Utils::encode("fo"), QString("Zm8="));
        QCOMPARE(Base64Utils::encode("foo"), QString("Zm9v"));
        QCOMPARE(Base64Utils::encode("foob"), QString("Zm9vYg=="));
        QCOMPARE(Base64Utils::encode("fooba"), QString("Zm9vYmE="));
        QCOMPARE(Base64Utils::encode("foobar"), QString("Zm9vYmFy"));

        QCOMPARE(Base64Utils::decode("Zg=="), QString("f"));
        QCOMPARE(Base64Utils::decode("Zm8="), QString("fo"));
        QCOMPARE(Base64Utils::decode("Zm9v"), QString("foo"));
        QCOMPARE(Base64Utils::decode("Zm9vYg=="), QString("foob"));
        QCOMPARE(Base64Utils::decode("Zm9vYmE="), QString("fooba"));
        QCOMPARE(Base64Utils::decode("Zm9vYmFy"), QString("foobar"));
    }

    void testUrlSafe() {
        QString sample = "Hello? SparkNotePad / World+Test=";
        QString enc = Base64Utils::encodeUrlSafe(sample);
        QVERIFY(!enc.contains('+'));
        QVERIFY(!enc.contains('/'));
        QCOMPARE(Base64Utils::decodeUrlSafe(enc), sample);
    }
};
