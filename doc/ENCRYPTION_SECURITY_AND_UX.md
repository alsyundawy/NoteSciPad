# AES-256 Encryption Security & UX Evaluation Report

## 1. Executive Summary

This report documents the architectural design, cryptographic evaluation, and user experience (UX) model for **AES-256 Document Encryption** in NoteSciPad.

NoteSciPad introduces authenticated file encryption designed to protect sensitive code, notes, configurations, and private credentials stored on local drives or synchronized cloud storage. The implementation follows modern NIST, RFC, and OWASP cryptographic standards, delivering high security, zero external third-party dependencies, and a smooth user experience.

---

## 2. Threat Model & Security Objectives

| Threat Vector | Mitigation in NoteSciPad |
| --- | --- |
| **Physical theft / unauthorized storage access** | Strong 256-bit symmetric encryption renders document contents unreadable without the passphrase. |
| **Offline dictionary & GPU brute-force attacks** | Key derivation via **PBKDF2-HMAC-SHA256** with **100,000 iterations** and a unique 128-bit random salt per file. |
| **Malleability / bit-flipping / truncation attacks** | **Encrypt-then-MAC (HMAC-SHA256)** verifies integrity and authenticity before ciphertext decryption. |
| **Padding oracle attacks (e.g. Vaudenay's attack)** | Decryption is aborted immediately if HMAC verification fails in constant time; no padding checks are performed on unauthenticated data. |
| **Side-channel timing analysis** | HMAC comparison uses constant-time byte comparison (`CryptoManager::constantTimeCompare`), eliminating timing discrepancies. |
| **Accidental file corruption / write interruption** | Documents are written atomically to disk to prevent partial or corrupted file states. |
| **Memory leakage / persistence** | Passphrases and derived keys exist only in volatile memory during the active session and are never written to disk, cache, or logs. |

---

## 3. Cryptographic Specification

### Container Binary Layout (`NSPENC01`)

An encrypted file produced by NoteSciPad is structured as follows:

```text
+-------------------+--------------------+--------------------+--------------------+
| Magic Header      | Iterations (KDF)   | Salt               | IV                 |
| (8 bytes ASCII)   | (4 bytes Big-End)  | (16 bytes random)  | (16 bytes random)  |
| "NSPENC01"        | 100,000            |                    |                    |
+-------------------+--------------------+--------------------+--------------------+
| HMAC-SHA256 Tag   | Ciphertext Payload (AES-256-CBC with PKCS#7 padding)     |
| (32 bytes)        | (N * 16 bytes)                                           |
+-------------------+----------------------------------------------------------+
```

### Protocol Steps

1. **Random Generation**:
   - `Salt`: 16 cryptographically secure random bytes via `QRandomGenerator::system()`.
   - `IV`: 16 cryptographically secure random bytes via `QRandomGenerator::system()`.

2. **Key Derivation (PBKDF2-HMAC-SHA256)**:
   - Derives 64 pseudo-random bytes from the user passphrase and Salt over 100,000 iterations (RFC 8018).
   - Bytes `0..31`: AES-256 Encryption Key (`K_enc`).
   - Bytes `32..63`: HMAC-SHA256 Authentication Key (`K_mac`).

3. **Encryption (AES-256-CBC)**:
   - Plaintext is padded using standard PKCS#7 (RFC 5652).
   - Encrypted in CBC mode using `K_enc` and `IV`.
   - On macOS: Accelerated via Apple's native `CommonCrypto` (`CCCrypt`).
   - On Linux / POSIX: Executed via portable FIPS-197 standard AES-256 implementation.

4. **Authentication (Encrypt-then-MAC)**:
   - An authenticated payload is constructed: `Header (8B) + Iterations (4B) + Salt (16B) + IV (16B) + Ciphertext`.
   - `HMAC-SHA256` is computed over the authenticated payload using `K_mac`.
   - The resulting 32-byte MAC is prepended to the ciphertext.

5. **Decryption & Verification**:
   - Verify magic header and container minimum length (76 bytes).
   - Derive keys using stored Salt and Iteration count.
   - Compute expected HMAC and compare against stored HMAC in constant time.
   - If MAC mismatch: abort immediately with authentication failure.
   - Decrypt ciphertext and validate PKCS#7 padding.

---

## 4. Evaluation of Alternative Approaches

| Feature / Metric | Legacy Notepad3 | NppCrypt (Notepad++) | NoteSciPad (`NSPENC01`) |
| --- | --- | --- | --- |
| **Cipher Algorithm** | Rijndael-256 CBC | Multiple (AES, Blowfish, DES, RC2) | AES-256-CBC (FIPS 197) |
| **Key Derivation** | Single SHA-256 (0 iterations) | PBKDF2 / Scrypt (configurable) | PBKDF2-HMAC-SHA256 (100,000 iterations) |
| **Integrity Check** | None (Unauthenticated CBC) | Optional HMAC depending on cipher | Mandatory Encrypt-then-MAC (HMAC-SHA256) |
| **Padding Oracle Defense** | Vulnerable | Partial | Complete (verify-before-decrypt) |
| **Platform Portability** | Windows only (CryptoAPI) | Windows only | macOS (ARM64/x86_64) & Linux (POSIX) |
| **Dependencies** | Win32 API | OpenSSL / LibTomCrypt DLLs | Zero external libs (CommonCrypto + Qt std) |
| **UI Complexity** | Minimal prompt | Overly complex modal with algorithm selectors | Clean dialogs with live strength meter |

---

## 5. User Experience (UX) Architecture

1. **Automatic Format Detection**:
   When opening any file starting with `NSPENC01`, NoteSciPad detects encryption transparently and presents `DecryptDialog` displaying the document name and an unlock passphrase input.

2. **Interactive Password Strength Meter**:
   In `EncryptDialog`, users receive immediate visual feedback on passphrase strength (Weak, Fair, Good, Strong) based on length, character diversity, and common password pattern heuristics.

3. **Session Passphrase Retention**:
   To prevent asking the user for their passphrase on every single save keystroke (`Cmd+S`), the verified passphrase is kept in memory attached to the active document tab. When the tab is closed or NoteSciPad quits, the passphrase is permanently purged from memory.

4. **Status Bar Visibility**:
   Encrypted documents display a persistent status notification in the status bar (`🔒 AES-256 Encrypted`), reassuring the user that encryption remains active.

5. **Reversible Decryption**:
   Users can remove encryption at any time via `Tools -> Encryption -> Remove Encryption...` (`Cmd+Alt+D` / `Ctrl+Alt+D`). The document reverts to plaintext upon the next save.
