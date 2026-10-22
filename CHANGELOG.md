# Changelog

All notable changes to **NoteSciPad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.0] - 2026-10-22

### 🚀 Initial Production Release — 13-Pillar Quality, Theme Leakage Elimination & "Dawn of NoteSciPad"

NoteSciPad v1.0.0 marks the premier official release of an independent, ultra-lightweight, and high-performance text and source code editor engineered natively for **macOS** (Apple Silicon ARM64 & Intel x86_64) and **Linux** (Ubuntu, Debian, Fedora, Flatpak, and AppImage). This release incorporates exhaustive 13-pillar code quality verification, complete elimination of light-mode dark theme leakage across all dialogs, and native C++20 static analysis compliance.

#### 🛡️ Theme Leakage Elimination & Dialog Polish
- **Zero Dark Mode Leakage in Light Mode**: Completely eliminated all usage of fragile Qt `palette(alternate-base)` and `palette(base)` stylesheets across `AboutDialog` and `DisplayInfoDialog`. On host operating systems set to Dark Mode (such as macOS Sequoia/Sonoma or Linux dark themes), Qt previously resolved those palette macros to dark gray (`#2d2d2d`), resulting in unsightly dark rectangles inside light-mode dialogs.
- **Dynamic Theme Palette Binding**: Color tokens are resolved deterministically using explicit CSS hex definitions (`#fafbfc` card background, `#f0f2f5` window background, `#e1e4e8` border, `#24292f` primary text, `#57606a` secondary text in Light Mode; `#282a36`, `#1e1f29`, `#44475a`, `#f8f8f2` in Dark Mode).

#### 🔧 13-Pillar C++ & Qt Static Audit
- **Trunk Check & Cppcheck Clean**: Passed all linters across all source files in `src/`. Resolved all warnings for `missingOverride`, `noExplicitConstructor`, `shadowFunction`, `shadowMember`, `cstyleCast`, `constVariablePointer`, `ignoredReturnErrorCode`, and `passedByValue`.

---

### ✨ Added

#### Core Engine & Editor Invariants

- **Vendored Scintilla & Lexilla Architecture**: Direct compilation against vendored Scintilla 5.5.4 and Lexilla 5.4.3 (`src/ScintillaNext.h` / `src/ScintillaNext.cpp`), providing rock-solid editing stability, multi-caret editing, sub-millisecond line navigation, and zero external runtime library dependencies.
- **Strict Engine Invariant Enforcement**: Automated CI script (`scripts/verify-engine-invariants.sh`) ensuring NoteSciPad never substitutes, swaps, or abstracts away its native Scintilla and Lexilla engines.

#### Security & Cryptography

- **AES-256-CBC Document Encryption**: Native document encryption with PKCS#7 padding, PBKDF2 key derivation (100,000 iterations), 16-byte cryptographically secure random salt (`arc4random_buf` on macOS, OpenSSL / BCrypt on Linux), and HMAC-SHA256 authentication tag integrity verification.
- **Zero-Footprint Memory Sanitization**: Sensitive passphrases and plaintext buffers sanitized immediately upon completion using `explicit_bzero` / `SecureZeroMemory`.
- **Decryption & Encryption UI (`CryptoManager`, `DecryptDialog`)**: Interactive modal workflows with keyboard shortcuts (`Ctrl+Alt+E` to encrypt, `Ctrl+Alt+D` to decrypt) and lock status indicators.

#### File Monitoring & Streaming

- **Live Disk Tail Monitoring (`tail -f`)**: Background filesystem watching (`QFileSystemWatcher`) with debounced auto-reloading (500ms), auto-scrolling to EOF, and non-modal log updates (`Ctrl+Alt+Y`).
- **Log Auto-Timestamp (.LOG Header)**: Notepad-compatible `.LOG` header detection and automatic timestamp appending upon opening and saving.

#### 25 Developer Theme Families & World Color Standards

- **Premier 25 Theme Engine (`ThemeManager`)**: Complete collection of 25 curated developer theme families (Catppuccin, Dracula, Tokyo Night, Nord, One Dark, Gruvbox, Solarized, GitHub, Monokai, Rosé Pine, Ayu, Kanagawa, Everforest, Night Owl, Material, SynthWave '84, Cyberpunk, Shades of Purple, Poimandres, Horizon, Andromeda, Nightfox, Cobalt2, Alabaster, and Tomorrow).
- **WCAG 2.1 AA/AAA Compliance**: Canvas contrast ratio of 12.6:1 (surpassing 7.0:1 AAA standard) and UI contrast ratio of 7.8:1, fully documented in `doc/COLOR_STANDARDS_AND_ACCESSIBILITY.md` (ADR-024).
- **Endianness-Safe Scintilla Colors**: Dedicated `scColor(r, g, b)` and `scFromRgb(QRgb)` helpers eliminating BGR inversion across macOS and Linux.

#### macOS Deep System Integration

- **macOS System Services (`NSServicesHandler`)**: Native AppKit service bindings (`Open Selection in NoteSciPad`, `Open File Path in NoteSciPad`) registered in `Info.plist`.
- **Multi-Monitor & Retina Management (`ScreenDisplayManager`)**: Multi-display geometry safety, off-screen restoration prevention, and fractional DPI scaling.
- **Window Transparency Controller (`TransparentWindowManager`)**: Dynamic opacity control (20%–100%) with alpha-channel compositing and global shortcut (`Ctrl+Alt+T`).
- **Default Editor Configuration Script**: `NotepadNext-Default-Editor.sh` for one-click file association across common development extensions.

#### Language Lexers & Syntax Highlighting

- **Extended Modern Language Lexers**: High-precision syntax highlighting for modern languages including WGSL, Typst, Mojo, Zig, Solidity, Vyper, and BaanC.
- **Universal Character Encoding Support**: Real-time universal encoding detection via `uchardet` with one-click conversion across UTF-8, UTF-8 BOM, ANSI, UTF-16LE, UTF-16BE, Shift-JIS, GB18030, and Big5.

#### Developer Tooling & Productive Utilities

- **Sandboxed JavaScript Evaluator (`JsEvaluator`, `JsEvaluationDialog`)**: Evaluate math, regex, JSON, and text transforms via `QJSEngine` with strict CPU execution timeouts (`Ctrl+Shift+J`).
- **Regular Expression Tester (`RegexHelperDialog`)**: Live interactive regular expression matcher with multiline, dot-all, and case-sensitivity toggles (`Ctrl+Alt+Shift+R`).
- **Color Call-Tip Previewer (`ColorPreviewCallTip`)**: Real-time color swatches and tooltips displayed directly above hexadecimal (`#RRGGBB`), RGB, RGBA, and HSL color codes under cursor.
- **Base64 Converter (`Base64Utils`)**: Instant document or selection encoding and decoding.
- **Unicode Character Inspector (`CharacterInfo`)**: Deep Unicode character breakdown including code point (`U+XXXX`), decimal, and category.
- **Customizable Status Bar (`EditorInfoStatusBar`)**: Modular field customizer with persistent visibility settings, clickable encoding picker, and right-click EOL conversion.

#### Linux Desktop & Cross-Platform Packaging

- **AppStream Metainfo**: `io.github.alsyundawy.NoteSciPad.metainfo.xml` adhering to FreeDesktop specifications.
- **Desktop Specification**: `io.github.alsyundawy.NoteSciPad.desktop` with categories, mime-types, and vector icons.
- **Flatpak & AppImage Manifests**: `io.github.alsyundawy.NoteSciPad.yml` recipe supporting sandboxed Linux deployment.

#### Automated Release & Update Architecture

- **Automatic Update Engine (`ReleaseManager`)**: Semver release comparison against GitHub Releases API with notification dialogs and direct package downloads.

---

### 🛡️ Quality Gates & CI/CD Verification

- **Automated Matrix CI Runner (`build.yml`)**: Continuous multi-platform builds testing 8 platform targets (Ubuntu 24.04 with GCC 6.5 & 6.8; macOS 15 Apple Silicon with Qt 6.5, 6.8, 6.10; macOS 15 Intel with Qt 6.5, 6.8, 6.10) with 100% green exit status.
- **MegaLinter Quality Suite (`mega-linter.yml`)**: Static code and configuration verification across 35+ linters with 0 errors.
- **GitHub CodeQL Advanced (`codeql.yml`)**: Deep semantic security analysis with 0 vulnerabilities detected.
- **Codespell Spelling Audit (`codespell.yml`)**: Lexicon and spelling validation across 100% of repository files.
- **Trunk Linter Integration (`.trunk/trunk.yaml`)**: Fast, lightweight native static check (`trunk check -n -j 2`) executing in 0.3s without CPU/RAM spikes.
- **13-Pillar Production Code Review**: Systematic audit covering Bug, Syntax, Runtime, Logic, Memory, Dead Code, Duplicate Code, Circular Dependency, Performance, Security, Maintainability, Scalability, and Readability documented in `doc/13_PILLARS_CODE_REVIEW_REPORT.md` (ADR-025).
- **Strict Markdownlint Compliance**: Remediated all markdown formatting issues across documentation files (`doc/*.md`, `CHANGELOG.md`, `USER_GUIDE.md`, `MANUAL_TEST_CHECKLIST.md`), normalizing code fence language specifiers (`MD040`), list blank line paddings (`MD032`), heading blank lines (`MD022`), and table cell formatting.

---

### 👤 Maintainer & Attribution

- **Lead Maintainer & Modernization**: **Harry Dertin Sutisna Alsyundawy** ([`@alsyundawy`](https://github.com/alsyundawy)) — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)
- **Upstream & Heritage Acknowledgments**:
  - **Neil Hodgson & Scintilla/Lexilla Contributors**: Foundational editing and lexing engines.
  - **Justin Dailey (dail8859)**: Original NotepadNext cross-platform Qt scaffolding.
  - **Zufu Liu (zufuliu)**: Notepad4 architectural insights and utility patterns.
  - **Rizonesoft**: Notepad3 text transformation idioms and ergonomics.
