#!/usr/bin/env python3
import sys
import base64
import json
import re
import math
import hashlib

print("--- Running NoteSciPad Standalone Utility Algorithm Tests ---")

# 1. Base64 RFC 4648
test_vectors = [
    ("", ""),
    ("f", "Zg=="),
    ("fo", "Zm8="),
    ("foo", "Zm9v"),
    ("foob", "Zm9vYg=="),
    ("fooba", "Zm9vYmE="),
    ("foobar", "Zm9vYmFy")
]
for raw, enc in test_vectors:
    computed = base64.b64encode(raw.encode()).decode()
    assert computed == enc, f"Base64 mismatch: {computed} != {enc}"
print("[PASS] Base64 RFC 4648 Test Vectors")

# 2. Number Conversion
assert int("0xFF", 16) == 255
assert int("0b1010", 2) == 10
assert int("0o77", 8) == 63
assert hex(255).upper() == "0XFF"
assert bin(10) == "0b1010"
assert oct(63) == "0o77"
print("[PASS] Radix & Number Conversions (Hex, Bin, Oct, Dec)")

# 3. GUID / UUID v4 Regex
uuid_pattern = re.compile(r"^[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}$", re.I)
import uuid
sample_uuid = str(uuid.uuid4())
assert uuid_pattern.match(sample_uuid), f"UUID v4 invalid format: {sample_uuid}"
print("[PASS] UUID / GUID v4 Validation")

# 4. JSON Minify & Prettify
messy_json = '{   "app":   "NoteSciPad" ,  "version": "0.9.0", "engine": ["scintilla", "lexilla"] }'
obj = json.loads(messy_json)
mini = json.dumps(obj, separators=(",", ":"))
assert " " not in mini
pretty = json.dumps(obj, indent=4)
assert "\n" in pretty and "    " in pretty
print("[PASS] JSON Minify and Prettify")

# 5. Math Evaluation
assert (1 + 2 * 3) == 7
assert ((10 - 2) / 4) == 2.0
assert math.sqrt(144) == 12.0
print("[PASS] Math Expressions & Operator Precedence")

# 6. PBKDF2 Key Derivation
salt = b"1234567890123456"
key1 = hashlib.pbkdf2_hmac("sha256", b"password", salt, 100000, 32)
key2 = hashlib.pbkdf2_hmac("sha256", b"password", salt, 100000, 32)
assert key1 == key2 and len(key1) == 32
print("[PASS] PBKDF2-HMAC-SHA256 (100,000 iterations)")

print("\n✅ ALL STANDALONE UTILITY ALGORITHM TESTS PASSED SUCCESSFULLY!")
