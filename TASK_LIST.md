# NoteSciPad v1.1.0 — Master Task List

**Project:** NoteSciPad — Advanced Cross-Platform Code & Text Editor  
**Version:** 1.1.0 Next Major Release  
**Lead Maintainer:** Harry Dertin Sutisna Alsyundawy (@alsyundawy) — ALSYUNDAWY IT SOLUTION  
**Local Workspaces:** `/Users/alsyundawy/Downloads/GitHub/NoteSciPad` & `/Users/alsyundawy/Downloads/NOTEPAD-MAC`  
**Engine Base:** Scintilla 5.6.x + Lexilla 5.5.x (Vendored) • Qt 6.5 / 6.8 LTS / 6.10 / 6.11

---

## 📌 Status Summary

- **Current Production Release:** v1.1.0 (Implemented, Verified & Complete)
- **Target Release:** v1.1.0 Production Release
- **Mode:** Full Implementation Complete across all 4 Phases

---

## Phase 1 — Visual & UI Modernization

### 1.1 Redesigned Elegant About Dialog (SubnetCalc-MacOS Format)

- [x] Create modern 5-tab dialog in `src/dialogs/AboutDialog.cpp` & `AboutDialog.h`
- [x] Tab 1: Application Overview, version badges, Scintilla 5.6.x engine details, and legal non-affiliation statement
- [x] Tab 2: Maintainer card (Harry Dertin Sutisna Alsyundawy, ALSYUNDAWY IT SOLUTION, Website, Email, GitHub, Telegram, PayPal, QRIS Barcode) — _Strictly exclude phone & WhatsApp_
- [x] Tab 3: Upstream Heritage (Scintilla, Lexilla, NotepadNext, Notepad4, Notepad3, Notepad++, Qt)
- [x] Tab 4: System Diagnostics with one-click "Copy Diagnostics to Clipboard"
- [x] Tab 5: GNU General Public License v3.0 full text & links

### 1.2 254 Theme Palettes from SubnetCalc-MacOS

- [x] Extract all 254 theme palettes from `SubnetCalc-MacOS/ThemeManager.swift`
- [x] Implement `ThemePalette` C++ data models in `src/features/ThemeManager.h` & `ThemeManager.cpp`
- [x] Map all 24 cohesive families:
  - [x] 1. Catppuccin Family (Mocha, Macchiato, Frappé, Latte)
  - [x] 2. Dracula Family (Dracula, Soft, Alucard, Light)
  - [x] 3. Tokyo Night Family (Night, Storm, Light)
  - [x] 4. Nord Family (Dark, Polar, Light)
  - [x] 5. One Dark Family (Pro, Vivid, Light)
  - [x] 6. Gruvbox Family (Dark, Dark Medium, Light)
  - [x] 7. Solarized Family (Dark, Light)
  - [x] 8. GitHub Family (Dark, Dark Dimmed, Light, Light High Contrast)
  - [x] 9. Monokai Family (Classic, Pro, Charcoal, Light)
  - [x] 10. Rosé Pine Family (Main, Moon, Dawn)
  - [x] 11. Ayu Family (Dark, Mirage, Light)
  - [x] 12. Kanagawa Family (Wave, Dragon, Lotus)
  - [x] 13. Everforest Family (Dark Hard, Dark Medium, Light)
  - [x] 14. Night Owl Family (Dark, Light Owl)
  - [x] 15. Material Family (Palenight, Deep Ocean, Lighter)
  - [x] 16. SynthWave '84 Family (Glow, Classic)
  - [x] 17. Cyberpunk Family (2077, Scarlet)
  - [x] 18. Shades of Purple Family (Super Dark, Classic, Light)
  - [x] 19. Poimandres Family (Dark, Storm, Light)
  - [x] 20. Horizon Family (Dark, Bright)
  - [x] 21. Andromeda Family (Dark, Bordered, Light)
  - [x] 22. Nightfox Family (Dark, Duskfox, Dawnfox)
  - [x] 23. Cobalt2 Family (Classic, Bright, Light)
  - [x] 24. Alabaster Family (Dark, Light)
- [x] Connect theme styles to Scintilla editor tokens (default text, background, caret, selection, fold margin, comments, strings, keywords, numbers)

### 1.3 Dedicated Top-Level MenuBar `Themes` Menu & Switcher

- [x] Add dedicated top-level `Themes` menu between `View` and `Tools` in `src/dialogs/MainWindow.cpp`
- [x] Construct dynamic hierarchical submenus for each of the 24 families
- [x] Add quick filters: `Dark Themes` vs `Light Themes`
- [x] Implement `ThemeSwitcherDialog` (`Ctrl+Shift+T` / `Cmd+Shift+T`) with live search, instant preview, and favorite starring

---

## Phase 2 — EditorBase Multi-Engine Feature Synthesis

### 2.1 TinyExpr++ Inline Math Evaluator (Inspired by Notepad3)

- [x] Vendor lightweight `tinyexpr-plusplus` header in `thirdparty/tinyexpr-plusplus/`
- [x] Implement `src/features/InlineMathEvaluator.h` and `.cpp`
- [x] Support evaluating expressions in selected text (e.g. `2 * pi * 5 = 31.4159`)
- [x] Add Menu action `Tools → Evaluate Math Expression` (`Ctrl+Shift+E` / `Cmd+Shift+E`)
- [x] Support scientific functions (`sin`, `cos`, `tan`, `sqrt`, `ln`, `log10`, `pow`, `abs`)

### 2.2 PCRE2 & Boost.Regex Engine Options (Inspired by Notepad3 & Notepad4)

- [x] Add PCRE2 10.45+ engine support in `src/QRegexSearch.cpp`
- [x] Add regex engine toggle in Find/Replace dialog:
  - `Standard (QRegularExpression)`
  - `PCRE2 (Perl Compatible, Advanced Lookbehind & Atomic Groups)`
  - `Boost.Regex (High-Speed POSIX / ECMAScript)`
- [x] Add multithreaded regex search execution to prevent UI freezing on huge files

### 2.3 Universal Charset Detection (uchardet) & State Persistence

- [x] Expand `uchardet` integration for seamless auto-detection on file open
- [x] Implement persistent workspace session auto-save on exit (open tab list, caret positions, fold states)
- [x] Add Document Map / Minimap margin toggle in `View → Document Map`

---

## Phase 3 — Syntax & Third-Party Library Modernization

### 3.1 Scintilla & Lexilla Modernization

- [x] Prepare vendor update to **Scintilla 5.6.7** in `thirdparty/scintilla/`
- [x] Prepare vendor update to **Lexilla 5.5.4** in `thirdparty/lexilla/`
- [x] Verify `ScintillaNext` wrapper maintains strict ABI invariants
- [x] Verify fractional scaling on macOS Retina and high-DPI displays

### 3.2 Scripting Engine Upgrades (Lua 5.5.1 & Python 3.13+)

- [x] Upgrade embedded Lua to **5.5.1** in `thirdparty/lua/`
- [x] Add Python 3.13 / 3.14 scripting integration bridge in `src/features/PythonScriptEngine.cpp`
- [x] Provide sample automation scripts in `scripts/` (formatters, data validators, code generators)

### 3.3 Trending Language Lexer Expansion

- [x] Add `zig.lua` (Zig 0.14+ syntax)
- [x] Add `rust.lua` (Rust 2024 edition)
- [x] Add `mojo.lua` (Modular Mojo AI systems language)
- [x] Add `swift.lua` (Swift 6 strict concurrency)
- [x] Add `kotlin.lua` (Kotlin 2.x coroutines)
- [x] Add `typescript.lua` (TypeScript 5.8+)
- [x] Add `wat.lua` (WebAssembly Text)
- [x] Add `solidity.lua` (Ethereum smart contracts)
- [x] Add `graphql.lua` (GraphQL schemas)
- [x] Add `dockerfile.lua` (Containerfile multi-stage)
- [x] Add `terraform.lua` (HCL / OpenTofu)
- [x] Add `bicep.lua` (Azure Bicep)
- [x] Add `protobuf.lua` (Proto3 & gRPC)
- [x] Add `julia.lua` (Scientific Julia)
- [x] Add `toml.lua` (TOML 1.0)
- [x] Add `mermaid.lua` (Mermaid diagrams)

### 3.4 Modern Qt & Crypto Upgrades

- [x] Ensure build compatibility with Qt 6.5 LTS, 6.8 LTS, 6.10, and Qt 6.11
- [x] OpenSSL 3.5 LTS / 3.6 FIPS-ready crypto integration
- [x] Implement `mkcert` helper dialog (`Tools → Generate Local Development Certificate`)

---

## Phase 4 — Quality Gates, 13-Pillar Audit & Release Packaging

### 4.1 13-Pillar Verification

- [x] Bug Review: Verify zero crash bugs, validate selection bounds, wrap EOL signal storm
- [x] Syntax Review: Verify ISO C++20 compliance with `-Wall -Wextra -Wpedantic`
- [x] Runtime Review: Verify on macOS (11.0–15.x) and modern Linux distributions
- [x] Logic Review: Verify fail-closed cryptography and atomic file writes
- [x] Memory Review: Zero memory leaks verified with AddressSanitizer / Valgrind
- [x] Dead Code Review: Prune legacy upstream NotepadNext comments and dead stubs
- [x] Duplicate Code Review: Deduplicate multi-selection loops via `EditorSelectionIterator`
- [x] Circular Dependency Review: Verify decoupled Qt signals/slots
- [x] Performance Review: Profile regex and large file scrolling benchmarks
- [x] Security Review: CodeQL SAST clean, zero OWASP Top 10 2025 violations
- [x] Maintainability Review: Single-responsibility classes in `src/features/`
- [x] Scalability Review: Smooth handling of 2GB+ files
- [x] Readability Review: Adhere strictly to `CODING_STANDARDS.md`

### 4.2 Documentation & Release Materials

- [x] Update `DOCNOTE.md` with complete v1.1.0 architectural notes
- [x] Update `CHANGELOG.md` following Keep a Changelog standard
- [x] Update `README.md` with new features, themes catalog, and screenshot previews
- [x] Update `updates.json` with v1.1.0 metadata

### 4.3 Multi-Arch CI/CD Matrix & Official Publication

- [x] Validate 8-job GitHub Actions matrix (`build.yml`)
- [x] Tag `v1.1.0` and trigger official production release workflow
- [x] Publish release with attached DMG (ARM64 & x64), DEB, RPM, and `SHA256SUMS.txt`

---

## Phase 5 — v1.2.0 UI/UX Polish, Apple HIG Icons, Dialogs, PDF & Multi-Qt Matrix

### 5.1 Apple HIG Multi-Resolution Icons

- [x] Resample icon with 82% squircle grid (824x824 inside 1024x1024 canvas with 100px optical padding) to eliminate oversized macOS Dock display
- [x] Generate 10-resolution macOS iconset (`NoteSciPad.iconset`: 16, 32, 128, 256, 512 + @2x) and compile `NoteSciPad.icns`
- [x] Generate multi-resolution Windows `NoteSciPad.ico` (16, 24, 32, 48, 64, 128, 256 px)
- [x] Update high-DPI Linux icons (`512x512` and `128x128`)

### 5.2 Modern Vector-Badged Message Box (`ModernMessageBox`)

- [x] Implement `ModernMessageBox.h` and `ModernMessageBox.cpp` with native sheet modal presentation
- [x] Design vector SVG badges for Question/Save, Warning, Critical, and Information states
- [x] Connect dialog background, border, buttons, and text dynamically to active `ThemePalette`
- [x] Replace legacy `QMessageBox` in `MainWindow::promptForSave` with `ModernMessageBox::promptForSave`

### 5.3 AboutDialog Redesign & Theme Binding

- [x] Remove murky grayscale `palette(alternate-base)` cards; replace with dynamic `ThemePalette::card` + border accent
- [x] Connect `AboutDialog` to `ThemeManager::themeChanged` signal for live repainting on theme switches
- [x] Enrich content: 6 tabs (About, Maintainer, Credits, Diagnostics, Shortcuts, License)
- [x] Purge "Buy Me A Coffee" references from maintainer card and throughout project

### 5.4 Universal Typography & File Menu Expansions

- [x] Set up universal font stacks across macOS (`-apple-system`/`SF Pro Text`) and Linux (`Inter`/`Ubuntu`/`Roboto`) in `src/main.cpp`
- [x] Add File -> `New Window` (`actionNewWindow`, `Ctrl+Shift+N` / `Cmd+Shift+N`) spawning detached editor process
- [x] Add File -> `Export to PDF...` (`actionExportPdf`) with native multi-page vector printing via `QPdfWriter` and `QPainter`

### 5.5 Localization & Multi-Qt CI/CD

- [x] Prune 21 untranslated `.ts` translation files, keeping only 100% complete English (`en`) and Indonesian (`id`)
- [x] Support and test Qt 6.5, 6.8, and 6.10 on macOS and Linux CI matrices
- [x] Fix DMG artifact pattern matching in `.github/workflows/release.yml` (`NoteSciPad*.dmg`)
- [x] Bump project version to `1.2.0` in `CMakeLists.txt`, `updates.json`, `README.md`, `CHANGELOG.md`, and `DOCNOTE.md`

