# NoteSciPad v1.1.0 — Master Task List

**Project:** NoteSciPad — Advanced Cross-Platform Code & Text Editor  
**Version:** 1.1.0 Next Major Release  
**Lead Maintainer:** Harry Dertin Sutisna Alsyundawy (@alsyundawy) — ALSYUNDAWY IT SOLUTION  
**Local Workspaces:** `/Users/alsyundawy/Downloads/GitHub/NoteSciPad` & `/Users/alsyundawy/Downloads/NOTEPAD-MAC`  
**Engine Base:** Scintilla 5.6.x + Lexilla 5.5.x (Vendored) • Qt 6.5 / 6.8 LTS / 6.10 / 6.11

---

## 📌 Status Summary

- **Current Production Release:** v1.0.0 (Verified & Published)
- **Target Release:** v1.1.0
- **Mode:** Analysis & Implementation Planning Complete

---

## Phase 1 — Visual & UI Modernization

### 1.1 Redesigned Elegant About Dialog (SubnetCalc-MacOS Format)

- [ ] Create modern 5-tab dialog in `src/dialogs/AboutDialog.cpp` & `AboutDialog.h`
- [ ] Tab 1: Application Overview, version badges, Scintilla 5.6.x engine details, and legal non-affiliation statement
- [ ] Tab 2: Maintainer card (Harry Dertin Sutisna Alsyundawy, ALSYUNDAWY IT SOLUTION, Website, Email, GitHub, Telegram, PayPal, Buy Me a Coffee, QRIS Barcode) — _Strictly exclude phone & WhatsApp_
- [ ] Tab 3: Upstream Heritage (Scintilla, Lexilla, NotepadNext, Notepad4, Notepad3, Notepad++, Qt)
- [ ] Tab 4: System Diagnostics with one-click "Copy Diagnostics to Clipboard"
- [ ] Tab 5: GNU General Public License v3.0 full text & links

### 1.2 254 Theme Palettes from SubnetCalc-MacOS

- [ ] Extract all 254 theme palettes from `SubnetCalc-MacOS/ThemeManager.swift`
- [ ] Implement `ThemePalette` C++ data models in `src/features/ThemeManager.h` & `ThemeManager.cpp`
- [ ] Map all 24 cohesive families:
  - [ ] 1. Catppuccin Family (Mocha, Macchiato, Frappé, Latte)
  - [ ] 2. Dracula Family (Dracula, Soft, Alucard, Light)
  - [ ] 3. Tokyo Night Family (Night, Storm, Light)
  - [ ] 4. Nord Family (Dark, Polar, Light)
  - [ ] 5. One Dark Family (Pro, Vivid, Light)
  - [ ] 6. Gruvbox Family (Dark, Dark Medium, Light)
  - [ ] 7. Solarized Family (Dark, Light)
  - [ ] 8. GitHub Family (Dark, Dark Dimmed, Light, Light High Contrast)
  - [ ] 9. Monokai Family (Classic, Pro, Charcoal, Light)
  - [ ] 10. Rosé Pine Family (Main, Moon, Dawn)
  - [ ] 11. Ayu Family (Dark, Mirage, Light)
  - [ ] 12. Kanagawa Family (Wave, Dragon, Lotus)
  - [ ] 13. Everforest Family (Dark Hard, Dark Medium, Light)
  - [ ] 14. Night Owl Family (Dark, Light Owl)
  - [ ] 15. Material Family (Palenight, Deep Ocean, Lighter)
  - [ ] 16. SynthWave '84 Family (Glow, Classic)
  - [ ] 17. Cyberpunk Family (2077, Scarlet)
  - [ ] 18. Shades of Purple Family (Super Dark, Classic, Light)
  - [ ] 19. Poimandres Family (Dark, Storm, Light)
  - [ ] 20. Horizon Family (Dark, Bright)
  - [ ] 21. Andromeda Family (Dark, Bordered, Light)
  - [ ] 22. Nightfox Family (Dark, Duskfox, Dawnfox)
  - [ ] 23. Cobalt2 Family (Classic, Bright, Light)
  - [ ] 24. Alabaster Family (Dark, Light)
- [ ] Connect theme styles to Scintilla editor tokens (default text, background, caret, selection, fold margin, comments, strings, keywords, numbers)

### 1.3 Dedicated Top-Level MenuBar `Themes` Menu & Switcher

- [ ] Add dedicated top-level `Themes` menu between `View` and `Tools` in `src/dialogs/MainWindow.cpp`
- [ ] Construct dynamic hierarchical submenus for each of the 24 families
- [ ] Add quick filters: `Dark Themes` vs `Light Themes`
- [ ] Implement `ThemeSwitcherDialog` (`Ctrl+Shift+T` / `Cmd+Shift+T`) with live search, instant preview, and favorite starring

---

## Phase 2 — EditorBase Multi-Engine Feature Synthesis

### 2.1 TinyExpr++ Inline Math Evaluator (Inspired by Notepad3)

- [ ] Vendor lightweight `tinyexpr-plusplus` header in `thirdparty/tinyexpr-plusplus/`
- [ ] Implement `src/features/InlineMathEvaluator.h` and `.cpp`
- [ ] Support evaluating expressions in selected text (e.g. `2 * pi * 5 = 31.4159`)
- [ ] Add Menu action `Tools → Evaluate Math Expression` (`Ctrl+Shift+E` / `Cmd+Shift+E`)
- [ ] Support scientific functions (`sin`, `cos`, `tan`, `sqrt`, `ln`, `log10`, `pow`, `abs`)

### 2.2 PCRE2 & Boost.Regex Engine Options (Inspired by Notepad3 & Notepad4)

- [ ] Add PCRE2 10.45+ engine support in `src/QRegexSearch.cpp`
- [ ] Add regex engine toggle in Find/Replace dialog:
  - `Standard (QRegularExpression)`
  - `PCRE2 (Perl Compatible, Advanced Lookbehind & Atomic Groups)`
  - `Boost.Regex (High-Speed POSIX / ECMAScript)`
- [ ] Add multithreaded regex search execution to prevent UI freezing on huge files

### 2.3 Universal Charset Detection (uchardet) & State Persistence

- [ ] Expand `uchardet` integration for seamless auto-detection on file open
- [ ] Implement persistent workspace session auto-save on exit (open tab list, caret positions, fold states)
- [ ] Add Document Map / Minimap margin toggle in `View → Document Map`

---

## Phase 3 — Syntax & Third-Party Library Modernization

### 3.1 Scintilla & Lexilla Modernization

- [ ] Prepare vendor update to **Scintilla 5.6.7** in `thirdparty/scintilla/`
- [ ] Prepare vendor update to **Lexilla 5.5.4** in `thirdparty/lexilla/`
- [ ] Verify `ScintillaNext` wrapper maintains strict ABI invariants
- [ ] Verify fractional scaling on macOS Retina and high-DPI displays

### 3.2 Scripting Engine Upgrades (Lua 5.5.1 & Python 3.13+)

- [ ] Upgrade embedded Lua to **5.5.1** in `thirdparty/lua/`
- [ ] Add Python 3.13 / 3.14 scripting integration bridge in `src/features/PythonScriptEngine.cpp`
- [ ] Provide sample automation scripts in `scripts/` (formatters, data validators, code generators)

### 3.3 Trending Language Lexer Expansion

- [ ] Add `zig.lua` (Zig 0.14+ syntax)
- [ ] Add `rust.lua` (Rust 2024 edition)
- [ ] Add `mojo.lua` (Modular Mojo AI systems language)
- [ ] Add `swift.lua` (Swift 6 strict concurrency)
- [ ] Add `kotlin.lua` (Kotlin 2.x coroutines)
- [ ] Add `typescript.lua` (TypeScript 5.8+)
- [ ] Add `wat.lua` (WebAssembly Text)
- [ ] Add `solidity.lua` (Ethereum smart contracts)
- [ ] Add `graphql.lua` (GraphQL schemas)
- [ ] Add `dockerfile.lua` (Containerfile multi-stage)
- [ ] Add `terraform.lua` (HCL / OpenTofu)
- [ ] Add `bicep.lua` (Azure Bicep)
- [ ] Add `protobuf.lua` (Proto3 & gRPC)
- [ ] Add `julia.lua` (Scientific Julia)
- [ ] Add `toml.lua` (TOML 1.0)
- [ ] Add `mermaid.lua` (Mermaid diagrams)

### 3.4 Modern Qt & Crypto Upgrades

- [ ] Ensure build compatibility with Qt 6.5 LTS, 6.8 LTS, 6.10, and Qt 6.11
- [ ] OpenSSL 3.5 LTS / 3.6 FIPS-ready crypto integration
- [ ] Implement `mkcert` helper dialog (`Tools → Generate Local Development Certificate`)

---

## Phase 4 — Quality Gates, 13-Pillar Audit & Release Packaging

### 4.1 13-Pillar Verification

- [ ] Bug Review: Verify zero crash bugs, validate selection bounds, wrap EOL signal storm
- [ ] Syntax Review: Verify ISO C++20 compliance with `-Wall -Wextra -Wpedantic`
- [ ] Runtime Review: Verify on macOS (11.0–15.x) and modern Linux distributions
- [ ] Logic Review: Verify fail-closed cryptography and atomic file writes
- [ ] Memory Review: Zero memory leaks verified with AddressSanitizer / Valgrind
- [ ] Dead Code Review: Prune legacy upstream NotepadNext comments and dead stubs
- [ ] Duplicate Code Review: Deduplicate multi-selection loops via `EditorSelectionIterator`
- [ ] Circular Dependency Review: Verify decoupled Qt signals/slots
- [ ] Performance Review: Profile regex and large file scrolling benchmarks
- [ ] Security Review: CodeQL SAST clean, zero OWASP Top 10 2025 violations
- [ ] Maintainability Review: Single-responsibility classes in `src/features/`
- [ ] Scalability Review: Smooth handling of 2GB+ files
- [ ] Readability Review: Adhere strictly to `CODING_STANDARDS.md`

### 4.2 Documentation & Release Materials

- [ ] Update `DOCNOTE.md` with complete v1.1.0 architectural notes
- [ ] Update `CHANGELOG.md` following Keep a Changelog standard
- [ ] Update `README.md` with new features, themes catalog, and screenshot previews
- [ ] Update `updates.json` with v1.1.0 metadata

### 4.3 Multi-Arch CI/CD Matrix & Official Publication

- [ ] Validate 8-job GitHub Actions matrix (`build.yml`)
- [ ] Tag `v1.1.0` and trigger official production release workflow
- [ ] Publish release with attached DMG (ARM64 & x64), DEB, RPM, and `SHA256SUMS.txt`
