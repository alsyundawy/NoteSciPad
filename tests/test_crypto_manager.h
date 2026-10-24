#pragma once
/*
 * SparkNotePad Cryptographic Engine Unit Tests
 * Verifies AES-256-CBC + HMAC-SHA256 (Encrypt-then-MAC) implementation,
 * dual-format container support (SPKENC01 / NSPENC01), and tamper-resistance.
 */

#include <QObject>
#include <QTest>
#include "utils/CryptoManager.h"

class TestCryptoManager : public QObject {
    Q_OBJECT

private slots:
    void testAesRoundtrip() {
        const QByteArray plain = "Confidential SparkNotePad Source Code 2026!";
        const QString pass = QStringLiteral("UltraSecurePassword#123");

        CryptoManager::CryptoResult enc = CryptoManager::encrypt(plain, pass);
        QVERIFY(enc.success);
        QVERIFY(!enc.data.isEmpty());
        QVERIFY(enc.data != plain);
        QVERIFY(CryptoManager::isEncryptedData(enc.data));

        CryptoManager::CryptoResult dec = CryptoManager::decrypt(enc.data, pass);
        QVERIFY(dec.success);
        QCOMPARE(dec.data, plain);
    }

    void testWrongPassphraseRejection() {
        const QByteArray plain = "Sensitive user notes that must remain confidential.";
        const QString correctPass = QStringLiteral("CorrectMasterKey_2026$");
        const QString wrongPass = QStringLiteral("IncorrectMasterKey_2026$");

        CryptoManager::CryptoResult enc = CryptoManager::encrypt(plain, correctPass);
        QVERIFY(enc.success);

        CryptoManager::CryptoResult dec = CryptoManager::decrypt(enc.data, wrongPass);
        QVERIFY(!dec.success);
        QVERIFY(dec.data.isEmpty());
        QVERIFY(!dec.errorMessage.isEmpty());
    }

    void testTamperedCiphertextHmacRejection() {
        const QByteArray plain = "Financial and identity records protected by SparkNotePad.";
        const QString pass = QStringLiteral("Crypt0_T3st_P@ssphr@se");

        CryptoManager::CryptoResult enc = CryptoManager::encrypt(plain, pass);
        QVERIFY(enc.success);

        QByteArray tampered = enc.data;
        const int cipherOffset = static_cast<int>(CryptoManager::HEADER_META_LEN);
        QVERIFY(tampered.size() > cipherOffset);
        tampered[cipherOffset] = static_cast<char>(tampered[cipherOffset] ^ 0x5A);

        CryptoManager::CryptoResult dec = CryptoManager::decrypt(tampered, pass);
        QVERIFY2(!dec.success, "Decryption must fail when ciphertext body is modified (HMAC verification failure)");
        QVERIFY(dec.data.isEmpty());
    }

    void testTamperedMacRejection() {
        const QByteArray plain = "Tamper detection test for MAC integrity.";
        const QString pass = QStringLiteral("MacIntegrityTestKey!42");

        CryptoManager::CryptoResult enc = CryptoManager::encrypt(plain, pass);
        QVERIFY(enc.success);

        const int macOffset = 8 + 4 + 16 + 16;
        QByteArray tampered = enc.data;
        tampered[macOffset] = static_cast<char>(tampered[macOffset] ^ 0xFF);

        CryptoManager::CryptoResult dec = CryptoManager::decrypt(tampered, pass);
        QVERIFY2(!dec.success, "Decryption must fail when MAC byte is flipped");
        QVERIFY(dec.data.isEmpty());
    }

    void testTamperedIvRejection() {
        const QByteArray plain = "IV modification test vector.";
        const QString pass = QStringLiteral("IvTestKey!2026");

        CryptoManager::CryptoResult enc = CryptoManager::encrypt(plain, pass);
        QVERIFY(enc.success);

        const int ivOffset = 8 + 4 + 16;
        QByteArray tampered = enc.data;
        tampered[ivOffset] = static_cast<char>(tampered[ivOffset] ^ 0xAA);

        CryptoManager::CryptoResult dec = CryptoManager::decrypt(tampered, pass);
        QVERIFY2(!dec.success, "Decryption must fail when IV is tampered with (HMAC covers IV)");
        QVERIFY(dec.data.isEmpty());
    }

    void testTamperedSaltRejection() {
        const QByteArray plain = "Salt modification test vector.";
        const QString pass = QStringLiteral("SaltTestKey!2026");

        CryptoManager::CryptoResult enc = CryptoManager::encrypt(plain, pass);
        QVERIFY(enc.success);

        const int saltOffset = 8 + 4;
        QByteArray tampered = enc.data;
        tampered[saltOffset] = static_cast<char>(tampered[saltOffset] ^ 0x33);

        CryptoManager::CryptoResult dec = CryptoManager::decrypt(tampered, pass);
        QVERIFY2(!dec.success, "Decryption must fail when Salt is tampered with");
        QVERIFY(dec.data.isEmpty());
    }

    void testMagicHeaderRecognition() {
        const QByteArray currentHeader = QByteArray(CryptoManager::MAGIC_HEADER) + QByteArray(68, '\0');
        QVERIFY(CryptoManager::isEncryptedData(currentHeader));

        const QByteArray legacyHeader = QByteArray(CryptoManager::LEGACY_MAGIC_HEADER) + QByteArray(68, '\0');
        QVERIFY(CryptoManager::isEncryptedData(legacyHeader));

        const QByteArray invalidHeader = "NOTMAGIC" + QByteArray(68, '\0');
        QVERIFY(!CryptoManager::isEncryptedData(invalidHeader));
    }

    void testEmptyPassphraseAndCorruptedInput() {
        CryptoManager::CryptoResult res1 = CryptoManager::encrypt("Some data", "");
        QVERIFY(!res1.success);

        CryptoManager::CryptoResult res2 = CryptoManager::decrypt("SPKENC01_too_short", "pass");
        QVERIFY(!res2.success);

        CryptoManager::CryptoResult res3 = CryptoManager::decrypt(QByteArray(), "pass");
        QVERIFY(!res3.success);
    }

    void testPasswordStrengthEvaluator() {
        auto weak = CryptoManager::evaluatePasswordStrength("12345");
        QVERIFY(weak.score < 50);

        auto strong = CryptoManager::evaluatePasswordStrength("S#p@rk!N0teP@d_2026_UltraLongSecret");
        QVERIFY(strong.score >= 70);
    }
};
