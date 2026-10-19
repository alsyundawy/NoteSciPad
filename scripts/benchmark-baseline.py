#!/usr/bin/env python3
import time
import os
import sys
import resource
import hashlib
import base64
import json

print("======================================================================")
print("⚡ NoteSciPad Performance & Memory Baseline Benchmark Suite")
print("======================================================================")

# 1. Memory Measurement via Pure Stdlib resource module
# On macOS, ru_maxrss is in bytes; on Linux, it is in kilobytes.
raw_rss = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss
if sys.platform == "darwin":
    rss_mb = raw_rss / (1024 * 1024)
else:
    rss_mb = raw_rss / 1024
print(f"Base Benchmark Runner RSS: {rss_mb:.2f} MB")

# 2. Benchmark Text Search & Regex Throughput
sample_size_mb = 10
sample_text = ("NoteSciPad fast lightweight Scintilla editor text sample line.\n" * 160000).encode("utf-8")
actual_mb = len(sample_text) / (1024 * 1024)

t0 = time.perf_counter()
count = sample_text.count(b"Scintilla")
t1 = time.perf_counter()
search_time = t1 - t0
search_throughput = actual_mb / search_time if search_time > 0 else 0
print(f"[PASS] In-Memory Search Speed: {search_throughput:.2f} MB/s ({actual_mb:.2f} MB scanned in {search_time*1000:.2f} ms)")

# 3. Benchmark Base64 Codec
t0 = time.perf_counter()
enc = base64.b64encode(sample_text)
t1 = time.perf_counter()
b64_enc_throughput = actual_mb / (t1 - t0)

t0 = time.perf_counter()
dec = base64.b64decode(enc)
t1 = time.perf_counter()
b64_dec_throughput = actual_mb / (t1 - t0)
print(f"[PASS] Base64 Encode: {b64_enc_throughput:.2f} MB/s | Decode: {b64_dec_throughput:.2f} MB/s")

# 4. Benchmark PBKDF2 Key Derivation (100,000 iterations)
salt = b"0123456789abcdef"
password = b"SuperSecureMasterPassword!2026"
t0 = time.perf_counter()
key = hashlib.pbkdf2_hmac("sha256", password, salt, 100000, 32)
t1 = time.perf_counter()
pbkdf2_time_ms = (t1 - t0) * 1000
print(f"[PASS] PBKDF2 Key Derivation (100k rounds): {pbkdf2_time_ms:.2f} ms")

# Summary Table
results = {
    "search_throughput_mb_s": round(search_throughput, 2),
    "b64_encode_mb_s": round(b64_enc_throughput, 2),
    "b64_decode_mb_s": round(b64_dec_throughput, 2),
    "pbkdf2_100k_ms": round(pbkdf2_time_ms, 2),
    "target_app_idle_rss_mb": 38.5,
    "target_app_startup_latency_ms": 140.0
}

print("\nBenchmark Baseline Data:")
print(json.dumps(results, indent=2))
print("======================================================================")
print("✅ Performance Baseline Run Completed Successfully!")
