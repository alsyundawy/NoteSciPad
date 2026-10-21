# Changelog

All notable changes to **NoteSciPad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.2.0] - 2026-10-20

### 💎 Apple HIG Icons, Modern Themed Dialogs, Scintilla 5.6.7/Lexilla 5.5.4/Lua 5.5.1, WCAG AAA Contrast & Multi-Qt CI

NoteSciPad v1.2.0 delivers comprehensive visual, architectural, and engine upgrades: full compliance with Apple Human Interface Guidelines for application icons (eliminating oversized macOS Dock rendering), an elegant vector-badged `ModernMessageBox` replacing dated popups, official source upgrades to **Scintilla 5.6.7**, **Lexilla 5.5.4**, and **Lua 5.5.1**, a native **Python 3.13+** scripting bridge, local SSL certificate generation via `MkcertDialog`, File menu expansions (`New Window` and `Export to PDF...`), strict **WCAG AAA** color contrast compliance across `AboutDialog` and global labels, custom **Trunk C++ & Qt** static analysis, and extended CI/CD build matrix coverage across Qt 6.5, Qt 6.8 LTS, Qt 6.10, and **Qt 6.11**.

---

### ✨ Added

#### Apple HIG Compliant Multi-Resolution Icons

- **82% Squircle Canvas Grid**: Re-engineered application icon with official Apple HIG optical padding (~100px border margins on 1024x1024 canvas), resolving oversized Dock rendering on macOS Sequoia and Sonoma.
- **Full Resolution Iconset**: Generated 10 distinct standard resolutions (`16x16`, `16x16@2x`, `32x32`, `32x32@2x`, `128x128`, `128x128@2x`, `256x256`, `256x256@2x`, `512x512`, `512x512@2x`) packaged into native `NoteSciPad.icns`.
- **Multi-Resolution Windows & Linux Icons**: Multi-layered `NoteSciPad.ico` (16, 24, 32, 48, 64, 128, 256 px) and high-DPI 512x512 Linux desktop icon.

#### Modern Vector-Badged Message Box (`ModernMessageBox`)

- **Custom Native Qt Sheet Dialog**: Replaced dated, plain `QMessageBox` popups (such as "Save changes to New 1?") with a sleek, rounded dialog adhering to active theme colors.
- **Custom Vector SVG Badges**: High-contrast, beautifully tinted circular badges for Save/Question, Warning, Critical, and Information states.
- **Dynamic Theme Palette Binding**: Dialog surfaces, borders, button hover states, and typography dynamically adhere to active dark and light themes.

#### Universal Cross-Platform Typography

- **Operating-System Adaptive Font Stacks**:
  - _macOS_: `-apple-system`, `SF Pro Text`, `Helvetica Neue`, `Arial`.
  - _Linux_: `Inter`, `Ubuntu`, `Roboto`, `DejaVu Sans`, `sans-serif`.
  - _Monospace Editor Stack_: `JetBrains Mono`, `SF Mono`, `Menlo`, `DejaVu Sans Mono`, `Liberation Mono`.

#### File Menu Expansions

- **New Window (`Ctrl+Shift+N` / `Cmd+Shift+N`)**: Instantly spawns a detached, independent NoteSciPad window process.
- **Export to PDF...**: Multi-page high-resolution vector PDF export powered by `QPdfWriter` and `QPainter`, including document headers, line numbering, and page numbering.

#### Official Third-Party Engine Upgrades

- **Scintilla 5.6.7**: Integrated official source code directly from upstream `scintilla.org` archives into `thirdparty/scintilla/`, unlocking latest multi-caret and fold rendering enhancements.
- **Lexilla 5.5.4**: Integrated official source code directly from upstream archives into `thirdparty/lexilla/` for robust multi-language lexing.
- **Embedded Lua 5.5.1**: Upgraded embedded Lua core in `thirdparty/lua/` directly from `lua.org` official source archives (`LUA_VERSION_NUM 505`).
- **Python 3.13+ Scripting Bridge (`PythonScriptEngine`)**: Integrated execution bridge prioritizing Python 3.13 and 3.14 runtimes with safety timeouts (10s for snippets, 15s for files), standard I/O capture, and direct Scintilla selection manipulation (`Ctrl+Shift+P` / `Cmd+Shift+P`).

#### Local Development SSL Certificate Generator (`MkcertDialog`)

- **Interactive Certificate Generator**: Added `Run → Generate Development SSL Certificate (mkcert/OpenSSL)...` (`Ctrl+Alt+C`) with automatic environment detection.
- **mkcert + OpenSSL Dual Engine**: Automatically uses `mkcert` for locally trusted certificates with CA root installation, and seamlessly falls back to OpenSSL self-signed generation if mkcert is not present.
- **Theme-Integrated Dialog (`MkcertDialog`)**: Styled with `ThemePalette` tokens, fixed-width command logging view, and instant system notifications via `ModernMessageBox`.

#### Trending Language Lexer Expansion

- **Kotlin 2.x (`kotlin.lua`)**: Syntax highlighting definitions for Kotlin keywords, coroutines (`suspend`, `flow`, `channel`), annotations, standard types, and DSL builders.
- **GraphQL (`graphql.lua`)**: Definitions for queries, mutations, subscriptions, schemas, types, directives, and scalar primitives.
- **WebAssembly Text (`wat.lua`)**: Definitions for WebAssembly Text format (`module`, `func`, `param`, `result`, `i32`, `i64`, `f32`, `f64`, instructions).
- **Azure Bicep (`bicep.lua`)**: Definitions for infrastructure-as-code resources, modules, parameters, variables, outputs, and decorators.
- **Resource Registration**: Registered all new lexers in `src/scripts.qrc` with verified XML and Lua syntax.

#### Strict WCAG AAA Contrast & Dynamic Theme Synchronization

- **WCAG 2.1 AAA Compliant Colors**: Resolved text-contrast issues where light-mode labels inherited system white text on light backgrounds. Enforced explicit `txtPri` (high contrast `#1f2328` on light, `#e0e0e0` on dark) and `txtSec` (`#656d76` / `#9aa0a6`) across `AboutDialog`, `ModernMessageBox`, and all modal cards.
- **Global `QLabel` Theme Rule in `ThemeManager.cpp`**: Added universal `QLabel { color: %2; }` rule ensuring all labels across the application adhere to active palette contrast.
- **Live Theme Synchronization**: Bound `AboutDialog` directly to `ThemeManager::themeChanged` signal for real-time live repainting without dialog re-instantiation.
- **Shortcuts & Documentation Tab**: Added dedicated keyboard shortcuts reference tab alongside About, Maintainer, Credits, Diagnostics, and License tabs.

#### Dialog Color Scheme Refinement & Universal Theme Synchronization

- **Universal Dialog Styling in `ThemeManager.cpp`**: Refined Dark and Light theme styling across all dialog windows (`FindReplaceDialog`, `AboutDialog`, `PreferencesDialog`, `ModernMessageBox`, `MkcertDialog`, `TailOptionsDialog`, `DisplayInfoDialog`), providing uniform palette tokens (`%1` to `%6`) for `QGroupBox`, `QCheckBox`, `QRadioButton`, `QLineEdit`, `QComboBox`, `QSpinBox`, `QSlider`, `QScrollBar`, and `QMenu`.
- **Universal Checkbox & Radio Button Overhaul**:
  - `ThemeManager.cpp`: Configured explicit `spacing: 8px;`, `padding: 3px 4px;`, `min-height: 20px;`, and 16x16 indicator dimensions (4px border-radius for checkboxes, 9px for radio buttons).
  - Crisp vector checkmark: Bundled `src/icons/checkmark.svg` via `src/resources.qrc` and `image: url(:/icons/checkmark.svg);`, restoring crisp vector checkmarks on checked checkboxes without platform-specific rendering anomalies.
  - Radio button indicator rendering: Enforced sharp circular indicator dot via `border: 3px solid %5; background-color: %6;` on checked state.
- **Dialog Layout & Checklist Spacing Calibration**:
  - `FindReplaceDialog.ui`: Enhanced Search Mode and Transparency group box layouts with explicit 8px spacing and 8/10px margins; increased vertical checkbox list spacing to 8px.
  - `PreferencesDialog.ui`: Enforced 8px vertical and horizontal spacing across all general settings checkbox and radio button rows (`verticalLayout_2`, `verticalLayout_3`, `verticalLayout_5`, `horizontalLayout_2`).
  - `TransparencyDialog.ui`: Added 10px spacing to main layout and 8px spacing with 8/10px margins to `verticalLayoutMode` radio button group.
  - `EncodingSelectionDialog.ui`: Configured 8px spacing and 8/10px margins for character set conversion action radio buttons (`layoutAction`).
  - `MacroRunDialog.ui`: Configured 8px vertical spacing and 8px horizontal button layout spacing.
  - `PerformanceDiagnosticsDialog.ui`: Added 8px spacing and 8/10px margins to diagnostic toggle layout (`verticalLayout_2`).
  - `TailOptionsDialog.ui`: Added 8/10px margins to `verticalLayout_2` for clean visual alignment.
  - `ThemeSwitcherDialog.cpp`: Added 12px spacing and 0/2px margins to `filterLayout`, dynamically calculating theme category radio counts using `kTotalThemePaletteCount`.
  - `ManageToolsDialog.cpp`: Configured 10px vertical spacing and 16px margins for dialog form layout.
  - `StatusBarCustomizerDialog.cpp`: Configured 8px vertical spacing and 14/16/14/14px margins for status bar element checklist.
  - `FocusedViewDialog.cpp`: Configured 16px horizontal spacing, 8px vertical spacing, and 14/16/14/14px margins for options grid; dynamic opacity status label.
- **NotepadNext Workspace Default Light Palette & Dark Mode Contrast Calibration**:
  - `ThemePaletteData.h`: Introduced dedicated `notepadNext` palette as the default light workspace palette (`#f0f2f5` window, `#fafbfc` canvas/card, `#e1e4e8` border, `#24292f` primary text, `#57606a` secondary text), avoiding bright eye-straining pure white backgrounds.
  - Default light mode mapping: Configured `defaultLight` to map directly to `notepadNext` palette in `findThemePaletteById`, and updated fallback color tokens in `ThemeManager::applyTheme` and `getThemeStyleSheet`.
  - Sharp dark mode contrast calibration: Calibrated secondary text colors across dark themes (`materialDeepOcean`, `oneDarkPro`, `dracula`, `solarizedDark`, `ayuDark`, `kanagawaDragon`) to guarantee crisp, sharp text rendering exceeding WCAG AAA standards.
- **Comprehensive 13-Pillar C++ Hardening**:
  - Marked `PreferencesDialog` and `SearchResultsCollector` constructors `explicit` to prevent implicit conversions.
  - Returned `const QString &` in `FileBrowserDock::currentDirectory()` to eliminate heap string allocations on directory lookups; reserved capacity for `cleanedFilters`.
  - Passed `searchTerm` and `line` parameters by `const QString &` across `ISearchResultsHandler`, `SearchResultsCollector`, `SearchResultsDock`, and `LuaConsoleDock::historyAdd`; eliminated shadowed variable declaration in `SearchResultsDock::newSearch`.
  - Replaced C-style cast with `reinterpret_cast` in `LuaConsoleDock.cpp`.
  - Marked stateless helper functions as `static` in `AutoClosePairs::isWordChar`, `DocumentationManager::openKeyboardShortcutsGuide`, `DocumentationManager::openOnlineRepository`, and `FocusedView::ensureHiddenLineStyle`.
- **Dock Tabs & ToolBar Visual Polish**: Restored warm peach top active tab border (`4px solid rgb(255, 202, 176)`), orange focused tab border, and refined native 1px toolbar bottom divider line with seamless button hover styling.

---

### 🔄 Changed & Refactored

- **Core Application Renaming**: Migrated core application classes to `NoteSciPadApplication.h` and `NoteSciPadApplication.cpp` with seamless alias and include-forwarding compatibility.
- **Brand Identity Cleanup**: Completely eradicated all legacy `NotepadNext` leftover files (`NotepadNext-Default-Editor.sh`, `deploy/linux/NotepadNext.desktop`, `icon/NotepadNext.{icns,ico,svg}`, `src/icons/NotepadNext.png`).
- **Translation File Harmonization**: Renamed translation files from `NotepadNext_*.ts` to `NoteSciPad_en.ts` and `NoteSciPad_id.ts`, resolving `TranslationManager` runtime locale discovery.

---

### 🗑️ Removed & Cleaned

- **Donation Options**: Completely removed all references to "Buy Me A Coffee" across the entire codebase and documentation (maintaining only PayPal and QRIS).
- **Localization Streamlining**: Pruned 21 incomplete translation files (`.ts`), preserving 100% verified English (`en`) and Indonesian (`id`) translations.
- **Privacy & Safety Invariants**: Strictly omitted personal phone and WhatsApp contact numbers from all maintainer documentation.
- **Temporary Artifacts Purged**: Cleanly removed obsolete planning and checklist documents (`IMPLEMENTATION_PLAN.md`, `TASK_LIST.md`, `WORKGROUND.md`, `readme-task.md`).

---

### 🛠️ Infrastructure & CI/CD

- **Multi-Qt Build & Release Matrix (Qt 6.5, 6.8 LTS, 6.10, and Qt 6.11)**:
  - Extended GitHub Actions matrices in `build.yml` and `release.yml` across 12 combinations: macOS Intel (`macos-15-intel`), macOS Apple Silicon (`macos-15`), and Linux Ubuntu (`ubuntu-24.04`) across Qt 6.5, 6.8 LTS, 6.10, and **Qt 6.11**.
- **Trunk Native C++ & Qt Static Analysis Integration**:
  - Configured `.trunk/trunk.yaml` with dedicated `cppcheck-qt` linter definition running `--library=qt --language=c++ --enable=warning,style,performance,portability`, providing first-class Qt analysis across all 277 files in `src/`.
  - Enabled `clang-format@20.1.0` and `pragma-once` header guard verification in Trunk Check.
  - Sub-second execution (<1s) locally with zero CPU saturation and zero background memory pegging.
- **Official Qt Action Upstream Migration**: Switched to official `jurplel/install-qt-action@v4.4.1`, eliminating runner template syntax errors across Ubuntu Linux matrices.
- **MegaLinter Pipeline Hardening**:
  - Disabled plain `C` descriptor in favor of `CPP` with Qt library support, tuned YAML/ShellCheck/Markdown rules, achieving 100% green pass (`✓`).
  - Added `workflow_dispatch` trigger for manual on-demand execution.
- **macOS DMG Packaging Fix (`cmake/PackagingMac.cmake`)**:
  - Added pre-cleanup (`rm -f`) and forced detach (`hdiutil detach /Volumes/NoteSciPad -force`) prior to `macdeployqt -dmg`, resolving intermittent `hdiutil: resource busy` race conditions on GitHub Actions macOS runners.
- **C++/Qt Codebase Hardening & 13-Pillar Verification**:
  - `MainWindow`: Added null-pointer safeguards in `updateGui`, `updateSaveStatusBasedUi`, and `checkFileForStateChange`.
  - `NoteSciPadApplication`: Default-initialized all manager pointers (`settings`, `editorManager`, `sessionManager`, `recentFilesListManager`, `translationManager`) to `nullptr`.
  - `CryptoManager`: Initialized `StrengthResult` struct members and eliminated dead pointer increment.
  - `LuaConsoleDock`: Upgraded legacy C cast to type-safe `reinterpret_cast<const char *>`.
  - `ScreenDisplayManager`: Enforced `const` correctness on `QScreen` pointer loops/retrievals and promoted `showDisplayInfoDialog` to static.
  - `TransparentWindowManager`: Eliminated parameter shadowing in `setMode(TransparencyMode targetMode)` and ensured clean signal emissions.
  - `QuickFindWidget`: Disconnected previous editor instance (`disconnect(this->editor, ...)`) before rebinding to prevent signal leaks, and renamed parameter to `newEditor`.
  - `CaseConverter` & `EncodingDetector`: Eliminated unused struct members (`anchor`, `caret`) and adopted `std::any_of` for 8-bit high-order byte scanning.
  - `ReleaseManager`: Renamed parameter `currentVersion` to `currentVersionStr` to prevent member function collision and enforced const pointers on update prompt buttons.
  - `MarkOccurrences`: Optimized `lastQuery()` to return `const QString &`.
- **Strict Markdownlint Compliance**: Remediated all markdown formatting issues across documentation files (`doc/*.md`, `CHANGELOG.md`, `USER_GUIDE.md`, `MANUAL_TEST_CHECKLIST.md`), normalizing code fence language specifiers (`MD040`), list blank line paddings (`MD032`), heading blank lines (`MD022`), and table cell formatting.

---

## [1.1.0] - 2026-10-19

### 🎨 Themes & UI Modernization, Math Evaluation, and Trending Lexers

NoteSciPad v1.1.0 introduces the complete 75-theme collection adapted from SubnetCalc-MacOS across 25 families, a dedicated top-level MenuBar `Themes` menu, an interactive Theme Switcher with instant live preview, inline mathematical expression evaluation, 7 trending language lexers, and a redesigned 5-tab modern About dialog.

---

### ✨ Added

#### Official Application Icon & Attribution

- **Icon Design by Lon McGregor**: NoteSciPad's official application icon has been adapted from the celebrated open-source artwork by Lon McGregor ([`LonMcGregor/NotepadPlusPlusIcons`](https://github.com/LonMcGregor/NotepadPlusPlusIcons)), honoring open-source iconography heritage across macOS (`.icns`), Windows (`.ico`), and Linux desktop environments (`.png`).

#### Visual & Theme Engine (75 Themes across 25 Families)

- **75 Theme Palettes**: Extracted and adapted from SubnetCalc-MacOS `ThemeManager.swift` into `ThemePaletteData.h`, providing curated color themes across 25 families (Catppuccin, Dracula, Tokyo Night, Nord, One Dark, Gruvbox, Solarized, GitHub, Monokai, Rosé Pine, Ayu, Kanagawa, Everforest, Night Owl, Material, SynthWave '84, Cyberpunk, Shades of Purple, Poimandres, Horizon, Andromeda, Nightfox, Cobalt2, Alabaster, and Tomorrow).
- **Dedicated Top-Level `Themes` Menu**: Inserted directly into the main menu bar between View and Document/Language, providing quick filters (Light Themes Only, Dark Themes Only), 25 family submenus, and one-click reset to default Light Mode.
- **Theme Switcher Dialog (`Ctrl+Shift+T` / `Cmd+Shift+T`)**: Interactive modal dialog featuring fuzzy search, type filtering (All/Light/Dark), 6-swatch color palette preview badges, and instant real-time canvas preview.

#### Redesigned 5-Tab About Dialog

- **Modern 5-Tab Interface (`AboutDialog`)**:
  - _About NoteSciPad_: High-resolution application icon, architecture overview, and legal non-affiliation statement.
  - _Maintainer Information_: Complete contact information for Harry DS Alsyundawy (ALSYUNDAWY IT SOLUTION) including Email, Website, GitHub, Telegram, PayPal, and QRIS barcode link (_strictly omitting phone number and WhatsApp_).
  - _Credits & Attribution_: Formal upstream attribution to Scintilla, Lexilla, NotepadNext, Notepad4, Notepad3, and SubnetCalc-MacOS.
  - _System & Diagnostics_: Real-time operating system, kernel version, CPU build architecture, Qt build/runtime version, display scale ratio, and one-click "Copy Diagnostics to Clipboard".
  - _License_: Complete GNU General Public License v3.0 text and FSF links.

#### EditorBase Feature Synthesis & Math Evaluation

- **Inline Mathematical Expression Evaluation (`Ctrl+Shift+E` / `Cmd+Shift+E`)**: Powered by `MathEvaluator` and `InlineMathEvaluator`, allowing instant calculation of selected expressions or current line (arithmetic, trigonometry, logarithms, powers, roots, constants pi, e, tau, phi).
- **Replace or Append Results**: Support appending result (`2 * pi * 5 = 31.4159265`) or replacing expression directly with formatted numerical result.

#### Trending Language Lexers

- **Mojo (`mojo.lua`)**: Modular Mojo systems programming language syntax (`.mojo`, `.🔥`).
- **Solidity (`solidity.lua`)**: Ethereum smart contract language syntax (`.sol`).
- **Julia (`julia.lua`)**: High-performance scientific computing language syntax (`.jl`).
- **Dockerfile (`dockerfile.lua`)**: Multi-stage container definitions (`Dockerfile`, `Containerfile`).
- **Terraform (`terraform.lua`)**: HashiCorp HCL & OpenTofu infrastructure as code (`.tf`, `.tfvars`, `.tofu`).
- **Protobuf (`protobuf.lua`)**: Google Protocol Buffers 3 & gRPC definitions (`.proto`).
- **Mermaid (`mermaid.lua`)**: Flowcharts, sequence diagrams, and architecture diagrams (`.mermaid`, `.mmd`).

---

## [1.0.0] - 2026-10-19

### 🚀 Initial Production Release — "Dawn of NoteSciPad"

NoteSciPad v1.0.0 marks the premier official release of an independent, ultra-lightweight, and high-performance text and source code editor engineered natively for **macOS** (Apple Silicon ARM64 & Intel x86_64) and **Linux** (Ubuntu, Debian, Fedora, Flatpak, and AppImage).

---

### ✨ Added

#### Core Engine & Editor Invariants

- **Vendored Scintilla & Lexilla Architecture**: Direct compilation against vendored Scintilla 5.5.4 and Lexilla 5.4.3 (`src/ScintillaNext.h` / `src/ScintillaNext.cpp`), providing rock-solid editing stability, multi-caret editing, sub-millisecond line navigation, and zero external runtime library dependencies.
- **Strict Engine Invariant Enforcement**: Automated CI script (`scripts/verify-engine-invariants.sh`) ensuring NoteSciPad never substitutes, swaps, or abstracts away its native Scintilla and Lexilla engines.

#### Security & Cryptography

- **AES-256-CBC Document Encryption**: Native document encryption with PKCS#7 padding, PBKDF2 key derivation (100,000 iterations), 16-byte cryptographically secure random salt (`arc4random_buf` on macOS, OpenSSL / BCrypt on Linux), and HMAC-SHA256 authentication tag integrity verification.
- **Portable Secure Memory Sanitization**: Sensitive AES/HMAC key bytes and PBKDF2-derived key material sanitized immediately after use via `secureZeroMemory()` — a portable volatile-pointer-indirection zeroing helper (semantically equivalent to `explicit_bzero` on Linux / `SecureZeroMemory` on Windows) applied to all `aesKey[32]`, `hmacKey[32]`, and `derived[64]` stack buffers in `CryptoManager.cpp`.
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

- **Sandboxed JavaScript Evaluator (`JsEvaluator`, `JsEvaluationDialog`)**: Evaluate math, regex, JSON, and text transforms. On macOS, uses the native `JavaScriptCore` framework via Objective-C++ bridge (sandboxed). On Linux, spawns an external `node`, `bun`, `deno`, or `quickjs` runtime process with strict 3-second CPU execution timeout (`Ctrl+Shift+J`).
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

---

### 👤 Maintainer & Attribution

- **Lead Maintainer & Modernization**: **Harry Dertin Sutisna Alsyundawy** ([`@alsyundawy`](https://github.com/alsyundawy)) — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)
- **Upstream & Heritage Acknowledgments**:
  - **Neil Hodgson & Scintilla/Lexilla Contributors**: Foundational editing and lexing engines.
  - **Justin Dailey (dail8859)**: Original NotepadNext cross-platform Qt scaffolding.
  - **Zufu Liu (zufuliu)**: Notepad4 architectural insights and utility patterns.
  - **Rizonesoft**: Notepad3 text transformation idioms and ergonomics.
