# SparkNotePad Unit & Integration Testing Framework

## 1. Overview

SparkNotePad features a native unit and integration test framework under \`tests/\` built on Qt Test and CTest, covering:

- **Base64 Codec:** RFC-4648 test vectors and URL-safe conversions.
- **Number Converter:** Binary (\`0b\`), Octal (\`0o\`), Decimal, and Hexadecimal (\`0x\`) conversions.
- **UUID / GUID & Timestamps:** RFC-4122 v4 regex compliance and ISO-8601 timestamps.
- **Case Converter:** UPPER, lower, Title, camelCase, snake_case, kebab-case.
- **Math Evaluator:** Arithmetic operator precedence and trigonometric functions.
- **Crypto Manager:** AES-256-CBC envelope encryption, PBKDF2 100,000 iterations, and PKCS7 padding.
- **Code Compress & Pretty:** JSON and CSS minify/prettify routines.

---

## 2. Running Unit Tests

```bash
# Execute unit tests via runner script:
./scripts/run-unit-tests.sh

# Or directly through CMake/CTest:
cmake -B build -DBUILD_TESTS=ON
cmake --build build --target SparkNotePadTests
ctest --test-dir build --output-on-failure
```
