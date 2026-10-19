# NoteSciPad Security Review & Threat Analysis (External Data)

## 1. Overview

This document presents a comprehensive security review and STRIDE threat model evaluating all NoteSciPad features that ingest, parse, or transmit external data or user inputs.

---

## 2. Threat Modeling & Component Audit

### 2.1 CryptoManager (AES-256-CBC, PBKDF2, PKCS#7)
- **Attack Vector:** Passphrase brute forcing, ciphertext tampering, padding oracle attacks.
- **Controls & Defenses:**
  - **PBKDF2-HMAC-SHA256:** Enforces **100,000 iterations** with a 128-bit cryptographically secure random salt generated via `SecRandomCopyBytes` (macOS) / `RAND_bytes`.
  - **PKCS#7 Validation:** Explicit padding byte boundary validation prevents padding oracle leakage.
  - **Envelope Format:** Enclosed in Base64 headers with clear boundary markers.

### 2.2 JsEvaluator (JavaScriptCore)
- **Attack Vector:** Malicious user scripts, prototype pollution, arbitrary code execution, filesystem probing.
- **Controls & Defenses:**
  - Sandboxed inside Apple's native `JSGlobalContextRef` (`JSGlobalContextCreate(nullptr)`).
  - No DOM (`window`, `document`), no network APIs (`fetch`, `XMLHttpRequest`), and no Node.js/process APIs (`fs`, `child_process`).
  - Explicit RAII context destruction (`JSGlobalContextRelease`) preventing heap leakage.

### 2.3 TailMonitoringManager (Live File Streaming)
- **Attack Vector:** Symlink traversal, arbitrary path disclosure, denial-of-service via huge log files.
- **Controls & Defenses:**
  - Enforces `QFileInfo::exists()` and checks `QFileInfo::isFile()`.
  - Incremental offset seeking (`seek(m_lastOffset)`) reads only delta chunks; never reads entire file into memory repeatedly.
  - Detects file truncation/rotation and resets offsets cleanly.

### 2.4 ReleaseManager (Update Verification)
- **Attack Vector:** Man-in-the-middle, malicious release payload execution, XSS injection in release notes.
- **Controls & Defenses:**
  - Strict HTTPS endpoint: `https://api.github.com/repos/alsyundawy/NoteSciPad/releases/latest`.
  - XSS Prevention: All external Markdown / HTML notes are escaped via `toHtmlEscaped()` before insertion into `QMessageBox` rich text view.
  - No automatic binary execution: NoteSciPad never downloads or runs binary installers silently. It routes the user to the official browser release page.

### 2.5 ScreenDisplayManager
- **Attack Vector:** Window placement exploits, screen geometry confusion.
- **Controls & Defenses:**
  - Hardened bounds checking ensures windows always intersect active display geometry by at least 150x150 pixels.

---

## 3. Automated Security Verification

The automated security scan script `scripts/run-security-audit.sh` is executed as part of CI validation:
```bash
./scripts/run-security-audit.sh
```
