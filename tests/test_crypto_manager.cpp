#include <QObject>
#include <QTest>
#include "utils/CryptoManager.h"

class TestCryptoManager : public QObject
{
    Q_OBJECT

private slots:
    void testAesRoundtrip()
    {
        QString plain = "Confidential NoteSciPad Source Code 2026!";
        QString pass = "UltraSecurePassword#123";

        QString enc = CryptoManager::encrypt(plain, pass);
        QVERIFY(!enc.isEmpty());
        QVERIFY(enc != plain);
        QVERIFY(enc.contains("BEGIN NOTESCIPAD ENCRYPTED FILE"));

        QString dec = CryptoManager::decrypt(enc, pass);
        QCOMPARE(dec, plain);

        // Wrong password test
        QString wrongDec = CryptoManager::decrypt(enc, "WrongPassword");
        QVERIFY(wrongDec.isEmpty() || wrongDec != plain);
    }
};
