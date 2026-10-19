#pragma once
#include <QObject>
#include <QTest>
#include "utils/CryptoManager.h"

class TestCryptoManager : public QObject
{
    Q_OBJECT

private slots:
    void testAesRoundtrip()
    {
        QByteArray plain = "Confidential NoteSciPad Source Code 2026!";
        QString pass = "UltraSecurePassword#123";

        CryptoManager::CryptoResult enc = CryptoManager::encrypt(plain, pass);
        QVERIFY(enc.success);
        QVERIFY(!enc.data.isEmpty());
        QVERIFY(enc.data != plain);
        QVERIFY(CryptoManager::isEncryptedData(enc.data));

        CryptoManager::CryptoResult dec = CryptoManager::decrypt(enc.data, pass);
        QVERIFY(dec.success);
        QCOMPARE(dec.data, plain);

        // Wrong password test
        CryptoManager::CryptoResult wrongDec = CryptoManager::decrypt(enc.data, "WrongPassword");
        QVERIFY(!wrongDec.success);
    }
};
