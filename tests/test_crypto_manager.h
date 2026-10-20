#pragma once
#include <QObject>
#include <QTest>
#include "utils/CryptoManager.h"

class TestCryptoManager : public QObject {
    Q_OBJECT

private slots:
    void testAesRoundtrip() {
        QByteArray plain = "Confidential SparkNotePad Source Code 2026!";
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

        // Backward compatibility verification: isEncryptedData recognizes both headers
        QByteArray legacyDummy = QByteArray(CryptoManager::LEGACY_MAGIC_HEADER) + QByteArray(8, '\0');
        QVERIFY(CryptoManager::isEncryptedData(legacyDummy));
        QByteArray newDummy = QByteArray(CryptoManager::MAGIC_HEADER) + QByteArray(8, '\0');
        QVERIFY(CryptoManager::isEncryptedData(newDummy));
    }
};
