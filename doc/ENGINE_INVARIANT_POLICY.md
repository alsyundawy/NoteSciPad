# SparkNotePad Core Engine Invariant Policy

## 1. Absolute Rule

The core editing and lexing foundation of SparkNotePad is strictly bound to:

1. **Vendored Scintilla:** Located in `thirdparty/scintilla`
2. **Vendored Lexilla:** Located in `thirdparty/lexilla`

**Rule:** Under NO circumstances may vendored Scintilla or Lexilla be deleted, bypassed, or replaced with WebView, Monaco, generic QTextEdit/QPlainTextEdit, or external dynamic library wrappers.

---

## 2. Enforcement & Verification

The automated verification script `scripts/verify-engine-invariants.sh` is executed as a required status check in CI/CD pipelines (`.github/workflows/build.yml`) and before every release tag:

- Validates existence of `include/Scintilla.h`, `include/Sci_Position.h`, `include/Lexilla.h`, `include/SciLexer.h`.
- Confirms CMake build targets statically link to `scintilla` and `lexilla`.
- Confirms `ScintillaNext` acts as the native Qt controller for all editor canvases.
