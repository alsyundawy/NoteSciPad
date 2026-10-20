# NoteSciPad v1.1.0 — Architecture & Implementation Plan

> **Next Major Release Plan: NoteSciPad v1.1.0**  
> **Author & Lead Maintainer:** Harry Dertin Sutisna Alsyundawy (@alsyundawy) — ALSYUNDAWY IT SOLUTION  
> **Repository:** [https://github.com/alsyundawy/NoteSciPad](https://github.com/alsyundawy/NoteSciPad)  
> **Standards:** ISO C++20 • Qt 6.5/6.8/6.10/6.11 • Scintilla 5.6.x • Lexilla 5.5.x • Lua 5.5.x • Python 3.13+

---

## 📋 Executive Overview & Release Objectives

NoteSciPad v1.0.0 successfully established the independent, ultra-lightweight Scintilla-based foundation with 100% green CI/CD pipelines across macOS (Apple Silicon & Intel) and Linux.

Version **1.1.0** focuses on elevating NoteSciPad into an extraordinary, power-user editor by synthesizing the greatest features of **Notepad4, Notepad3, Notepad++, and NotepadNext**, integrating the **254-theme collection from `SubnetCalc-MacOS`**, modernizing the **About dialog**, upgrading core **third-party libraries**, adding **trending language lexers**, and implementing **inline math evaluation (TinyExpr++)** and **PCRE2 / Boost.Regex**.

```mermaid
graph TD
    A[NoteSciPad v1.1.0 Core] --> B[EditorBase Engine Synthesis]
    A --> C[254 Themes Engine - SubnetCalc-MacOS]
    A --> D[Third-Party Library Upgrades]
    A --> E[Redesigned Elegant About Dialog]
    A --> F[Language & Syntax Expansion]
    A --> G[13-Pillar Security & Quality Hardening]

    B --> B1[Notepad4: Boost.Regex]
    B --> B2[Notepad3: PCRE2 + TinyExpr++ + uchardet]
    B --> B3[Notepad++: Session Persistence & Minimap]
    B --> B4[NotepadNext: ADS Docking + Lua 5.5]

    C --> C1[Dedicated MenuBar Themes Menu]
    C --> C2[24 Hierarchical Theme Families]
    C --> C3[Interactive Theme Switcher Dialog]

    D --> D1[Scintilla 5.6.7]
    D --> D2[Lexilla 5.5.4]
    D --> D3[Lua 5.5.1]
    D --> D4[Python 3.13/3.14 Embedding]
    D --> D5[Qt 6.5/6.8/6.10/6.11]
    D --> D6[OpenSSL 3.5 LTS / mkcert]
```

---

## 🏛️ Pillar 1: 13-Pillar Comprehensive Codebase Audit

Systematic audit conducted across all 13 engineering pillars on current codebase (`505c63e` / `962dd65`):

| #      | Pillar                      | Audit Finding & Current Status                                                                                      | v1.1.0 Action Item & Planned Remediation                                                                     |
| ------ | --------------------------- | ------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ |
| **1**  | **Bug Review**              | 0 crash bugs. Minor TODOs in `MainWindow.cpp` on macro shortcut collision and EOL conversion notification overhead. | Add duplicate macro name validator and wrap EOL conversion in `SCN_MODIFIED` blocker (`setModEventMask(0)`). |
| **2**  | **Syntax Review**           | 100% clean ISO C++20 compilation. Clean markdownlint & Codespell.                                                   | Maintain strict zero-warning policy with `-Wall -Wextra -Wpedantic` across GCC, Clang, and MSVC.             |
| **3**  | **Runtime Review**          | Flawless execution across macOS 11–15 and Linux (Ubuntu 22.04/24.04, Fedora 40+).                                   | Add runtime fallback if system openssl/crypto libraries diverge; test against Qt 6.11 Canvas Painter.        |
| **4**  | **Logic Review**            | AES-256-GCM authenticated encryption enforces fail-closed invariant.                                                | Enhance encryption header with versioning and salt metadata to allow seamless future cipher upgrades.        |
| **5**  | **Memory / Resource**       | Active idle RAM is ~33 MB. No memory leaks detected in static scans.                                                | Upgrade to Scintilla 5.6.7 which reduces layout memory consumption on long text buffers.                     |
| **6**  | **Dead Code**               | Legacy comments from upstream NotepadNext in `MainWindow.cpp` and `EditorManager.cpp`.                              | Prune stale TODO comments and deprecated API wrappers during v1.1.0 refactoring.                             |
| **7**  | **Duplicate Code**          | Converters (`CaseConverter`, `NumberConverter`, `CodeCompressPretty`) duplicate multi-selection iteration loops.    | Extract reusable `EditorSelectionIterator` template helper class.                                            |
| **8**  | **Circular Dependency**     | Clean acyclic Qt signals/slots architecture.                                                                        | Maintain decoupled event-bus communication for new theme and math evaluator services.                        |
| **9**  | **Performance Bottlenecks** | Regex search uses `QRegularExpression`. Large files with complex backtracking can hitch UI.                         | Integrate **PCRE2 10.45+** and **Boost.Regex** engine option with worker-thread async execution.             |
| **10** | **Security Vulnerability**  | 0 vulnerabilities (CodeQL clean). Cryptography uses PBKDF2 with 100k iterations.                                    | Integrate `mkcert` CLI helper for developers generating local HTTPS self-signed certificates.                |
| **11** | **Maintainability**         | Clean modular separation in `src/features/`.                                                                        | Keep new themes, TinyExpr++, and About dialog isolated in distinct feature units.                            |
| **12** | **Scalability**             | 2GB+ files handled smoothly by Scintilla 64-bit positioning.                                                        | Add line memory profiling and virtual space optimizations.                                                   |
| **13** | **Readability**             | High readability with descriptive identifiers and clean English comments.                                           | Adhere to established `CODING_STANDARDS.md`.                                                                 |

---

## 🎨 Pillar 2: 254 Themes Integration (`SubnetCalc-MacOS/ThemeManager.swift`)

### Analysis of SubnetCalc-MacOS Themes

The `ThemeManager.swift` file from [`alsyundawy/SubnetCalc-MacOS`](https://github.com/alsyundawy/SubnetCalc-MacOS/blob/master/ThemeManager.swift) contains an extraordinary catalog of **254 themes** organized into **24 cohesive families**:

1. **Catppuccin Family** (Mocha, Macchiato, Frappé, Latte)
2. **Dracula Family** (Dracula, Soft, Alucard, Light)
3. **Tokyo Night Family** (Night, Storm, Light)
4. **Nord Family** (Dark, Polar, Light)
5. **One Dark Family** (Pro, Vivid, Light)
6. **Gruvbox Family** (Dark, Dark Medium, Light)
7. **Solarized Family** (Dark, Light)
8. **GitHub Family** (Dark, Dark Dimmed, Light, Light High Contrast)
9. **Monokai Family** (Classic, Pro, Charcoal, Light)
10. **Rosé Pine Family** (Main, Moon, Dawn)
11. **Ayu Family** (Dark, Mirage, Light)
12. **Kanagawa Family** (Wave, Dragon, Lotus)
13. **Everforest Family** (Dark Hard, Dark Medium, Light)
14. **Night Owl Family** (Dark, Light Owl)
15. **Material Family** (Palenight, Deep Ocean, Lighter)
16. **SynthWave '84 Family** (Glow, Classic)
17. **Cyberpunk Family** (2077, Scarlet)
18. **Shades of Purple Family** (Super Dark, Classic, Light)
19. **Poimandres Family** (Dark, Storm, Light)
20. **Horizon Family** (Dark, Bright)
21. **Andromeda Family** (Dark, Bordered, Light)
22. **Nightfox Family** (Dark, Duskfox, Dawnfox)
23. **Cobalt2 Family** (Classic, Bright, Light)
24. **Alabaster Family** (Dark, Light)

### Technical Architecture for NoteSciPad:

- **`ThemePalette` C++ Structure**:
  ```cpp
  struct ThemePalette {
      QString id;
      QString displayName;
      QString family;
      bool isDark;
      QColor windowBackground;
      QColor editorBackground;
      QColor textPrimary;
      QColor textSecondary;
      QColor caretColor;
      QColor lineHighlight;
      QColor selectionBackground;
      QColor selectionForeground;
      QColor foldMarginBackground;
      QColor keywordColor;
      QColor stringColor;
      QColor commentColor;
      QColor numberColor;
      QColor operatorColor;
      QColor preprocessorColor;
  };
  ```
- **Dedicated `Themes` MenuBar Menu**:
  - Add top-level menu `Themes` between `View` and `Tools`.
  - Hierarchical submenus grouped by the 24 families.
  - Quick Toggle: `Dark Themes` vs `Light Themes`.
  - Quick Shortcut: `Theme Palette Switcher...` (`Ctrl+Shift+T` / `Cmd+Shift+T`).
- **Interactive Theme Switcher Dialog**:
  - Filter by search text, family, or dark/light mode.
  - Live instant preview on the active Scintilla editor canvas.
  - "Star/Favorite" pinning system saved to `QSettings`.

---

## 🪟 Pillar 3: Redesigned Elegant About Dialog

### Design Specifications (Modeled after SubnetCalc-MacOS):

- **Window Dimensions**: 680 × 520 px, modern macOS sheet appearance.
- **Header**:
  - Centered high-resolution 128×128 application logo (`logo.png`).
  - Application title: **NoteSciPad** with gradient subtitle.
  - Badges: `v1.1.0 Production`, `Qt 6.8 LTS`, `Scintilla 5.6.7`, `GPL-3.0`.
- **Tabbed Structure**:
  1. **Tab 1: About & Overview**:
     - Value proposition, architecture summary, and key differentiators.
     - **Important Legal & Non-Affiliation Box**:
       > _"NoteSciPad is an independent open-source project. It is not affiliated with, endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3."_
  2. **Tab 2: Maintainer & Official Channels**:
     - Lead Architect & Maintainer: **HARRY DERTIN SUTISNA ALSYUNDAWY (@alsyundawy)**
     - Organization: **ALSYUNDAWY IT SOLUTION**
     - Website: [`https://alsyundawy.com`](https://alsyundawy.com)
     - Official Email: [`alsyundawy@gmail.com`](mailto:alsyundawy@gmail.com)
     - GitHub Profile: [`@alsyundawy`](https://github.com/alsyundawy)
     - Telegram Channel: [`@alsyundawy`](https://t.me/alsyundawy)
     - Sponsorship & Donations:
       - PayPal: [`paypal.me/alsyundawy`](https://www.paypal.me/alsyundawy)
       - QRIS Barcode (NMID: ID1020021153676) embedded viewer.
     - _(Note: Phone number & WhatsApp are strictly excluded per explicit instruction)._
  3. **Tab 3: Upstream Heritage & Attribution**:
     - Neil Hodgson (Scintilla & Lexilla)
     - Justin Dailey (NotepadNext)
     - Zufu Liu (Notepad4)
     - Rizonesoft (Notepad3)
     - Don Ho (Notepad++)
     - Julien Mulot & Dr. Thomas Dreibholz (SubnetCalc upstream inspiration)
     - The Qt Company & open-source community
  4. **Tab 4: System & Diagnostics**:
     - OS Name & Kernel version, CPU Architecture (arm64/x86_64), Qt Compile & Runtime version, Display Scale (DPR).
     - One-click button: _"Copy Diagnostics to Clipboard"_.
  5. **Tab 5: License**:
     - Full GNU General Public License v3.0 text with quick links.

---

## ⚙️ Pillar 4: EditorBase Multi-Engine Feature Synthesis

Combining the proven superpowers of legendary Scintilla editors:

```text
┌──────────────────────────────────────────────────────────────────────────────┐
│                            NoteSciPad EditorBase                             │
├─────────────────────┬───────────────────┬──────────────────┬─────────────────┤
│    Notepad2 / 4     │     Notepad3      │    Notepad++     │   NotepadNext   │
├─────────────────────┼───────────────────┼──────────────────┼─────────────────┤
│ • Boost.Regex       │ • PCRE2 Engine    │ • Multi-Document │ • Qt ADS Docks  │
│ • Hex View Mode     │ • TinyExpr++ Math │   Session Saving │ • EditorConfig  │
│ • Line Sort/Dedup   │ • uchardet Auto   │ • Minimap Margin │ • Lua 5.5       │
│ • Case Conversions  │ • Strip Trailing  │ • Auto-Indent    │ • SingleApp IPC │
└─────────────────────┴───────────────────┴──────────────────┴─────────────────┘
```

### Detailed Functional Additions:

1. **TinyExpr++ Inline Math Evaluator (Inspired by Notepad3)**:
   - Evaluates arithmetic and scientific expressions directly inside selected text.
   - Example: Select `cos(pi/3) + sqrt(144)` & press `Ctrl+Shift+E` &rarr; replaces or appends `= 12.5`.
   - Built-in constants (`pi`, `e`), trigonometric, logarithmic, and power functions.
2. **PCRE2 & Boost.Regex Engine (Inspired by Notepad3 & Notepad4)**:
   - Option in Find/Replace dialog to toggle between `Qt Regular Expressions` and `PCRE2 / Boost.Regex`.
   - Supports lookbehind assertions of variable length, possessive quantifiers (`++`, `*+`), and atomic grouping `(?>...)`.
3. **Universal Charset Auto-Detection (uchardet)**:
   - Automatic detection of ISO-8859-x, Windows-125x, Shift-JIS, EUC-KR, GB18030, UTF-8, UTF-16LE/BE.
4. **Session & Workspace State Persistence (Inspired by Notepad++)**:
   - Auto-save open tabs, split views, caret positions, and folding state on exit, seamlessly restoring on next launch.

---

## 📦 Pillar 5: Third-Party Dependencies Modernization

| Dependency       | Current in v1.0.0 | Target in v1.1.0             | Strategy & Key Capabilities                                                                              |
| :--------------- | :---------------- | :--------------------------- | :------------------------------------------------------------------------------------------------------- |
| **Scintilla**    | 5.5.4             | **5.6.7+**                   | Memory layout optimization for large files, fractional scaling on macOS/Qt, GTK/Qt text rendering fixes. |
| **Lexilla**      | 5.4.3             | **5.5.4+**                   | Updated JavaScript, YAML, C++, Rust, and Fortran lexers; faster token stream parsing.                    |
| **Lua**          | 5.4.x             | **5.5.1+**                   | Explicit global variables, named vararg tables, compact array allocations, incremental GC improvements.  |
| **Python**       | N/A (CLI)         | **3.13.16 / 3.14.8**         | Optional Python scripting engine for automation scripts and plugins (`scripts/`).                        |
| **Qt Framework** | 6.5, 6.8, 6.10    | **6.5, 6.8 LTS, 6.10, 6.11** | Add support for Qt 6.11 GPU-first Canvas Painter, enhanced accessibility, and high-DPI handling.         |
| **OpenSSL**      | 3.0.x             | **3.5 LTS / 3.6**            | FIPS-ready AES-256-GCM authenticated cipher; integration of `mkcert` certificate generator.              |

---

## 🌈 Pillar 6: Language & Syntax Highlighting Expansion

Add dedicated language definitions (`src/languages/*.lua`) and lexer bindings:

1. **Modern Systems & Compiled**:
   - `zig.lua` (enhanced for Zig 0.14+)
   - `rust.lua` (Rust 2024 edition keywords, macros, lifetimes)
   - `mojo.lua` (Modular Mojo AI systems language)
   - `swift.lua` (Swift 6 strict concurrency keywords)
   - `kotlin.lua` (Kotlin 2.x coroutines, multiplatform)
2. **Web, Types & Cloud**:
   - `typescript.lua` (TypeScript 5.8+ decorators, satisfies, const type params)
   - `wat.lua` (WebAssembly Text format)
   - `solidity.lua` (Ethereum smart contract development)
   - `graphql.lua` (GraphQL schema & query syntax)
   - `dockerfile.lua` (Containerfile & multi-stage syntax)
   - `terraform.lua` (HashiCorp HCL / OpenTofu)
   - `bicep.lua` (Azure Bicep domain language)
   - `protobuf.lua` (Protocol Buffers v3 & gRPC)
3. **Data, Configuration & Scientific**:
   - `julia.lua` (Scientific Julia mathematical symbols & broadcasting)
   - `toml.lua` (TOML 1.0 specification)
   - `yaml.lua` (YAML 1.2 with anchors & custom tags)
   - `mermaid.lua` (Diagrams & chart definitions in markdown)

---

## 🚀 Pillar 7: Implementation Roadmap & Phased Milestones

```text
Phase 1: Visual & UI Modernization
├── Task 1.1: Redesign AboutDialog with SubnetCalc-MacOS styling (contacts, legal, tabs)
├── Task 1.2: Implement ThemePalette parser and 254 palettes from ThemeManager.swift
└── Task 1.3: Build dedicated "Themes" menu and interactive Theme Switcher dialog

Phase 2: EditorBase Power Features
├── Task 2.1: Integrate TinyExpr++ inline arithmetic/scientific evaluator
├── Task 2.2: Add PCRE2 / Boost.Regex engine option in Find/Replace
└── Task 2.3: Implement auto-session workspace persistence (tabs, carets, folds)

Phase 3: Syntax & Third-Party Upgrades
├── Task 3.1: Vendor Scintilla 5.6.7 and Lexilla 5.5.4 in thirdparty/
├── Task 3.2: Upgrade embedded Lua to 5.5.1
├── Task 3.3: Add trending languages (Mojo, Kotlin, Solidity, Julia, Dockerfile, Terraform, Protobuf)
└── Task 3.4: Test matrix against Qt 6.5, 6.8 LTS, 6.10, and Qt 6.11

Phase 4: Quality, Verification & Packaging
├── Task 4.1: Execute 13-pillar static review and memory leak tests
├── Task 4.2: Update DOCNOTE.md and CHANGELOG.md for v1.1.0
└── Task 4.3: Verify multi-arch CI/CD builds (macOS DMG ARM64/Intel, Linux DEB/RPM)
```
