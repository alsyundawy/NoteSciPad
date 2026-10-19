/*
 * NoteSciPad Unit Test Suite Runner
 * Copyright 2026 NoteSciPad Project
 */

#include <QTest>
#include <QCoreApplication>
#include <iostream>

// Declarations of individual test classes
class TestBase64;
class TestNumberConverter;
class TestGuidTimestamp;
class TestCaseConverter;
class TestMathEvaluator;
class TestCryptoManager;
class TestCodeCompressPretty;

int runAllTests(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    std::cout << "===================================================\n";
    std::cout << "⚡ NoteSciPad Unit & Integration Test Suite\n";
    std::cout << "===================================================\n";

    return runAllTests(argc, argv);
}
