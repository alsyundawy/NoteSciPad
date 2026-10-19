#include <QObject>
#include <QTest>
#include "utils/MathEvaluator.h"

class TestMathEvaluator : public QObject
{
    Q_OBJECT

private slots:
    void testEvaluation()
    {
        bool ok = false;
        double r1 = MathEvaluator::evaluate("1 + 2 * 3", &ok);
        QVERIFY(ok);
        QCOMPARE(r1, 7.0);

        double r2 = MathEvaluator::evaluate("(10 - 2) / 4", &ok);
        QVERIFY(ok);
        QCOMPARE(r2, 2.0);

        double r3 = MathEvaluator::evaluate("sqrt(144)", &ok);
        QVERIFY(ok);
        QCOMPARE(r3, 12.0);
    }
};
