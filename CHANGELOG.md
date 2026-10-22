# Changelog

All notable changes to **NoteSciPad** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.1.0] - 2026-10-22

### 🎨 Native Light/Dark UI Streamlining, 13-Pillar Quality, Theme Leakage Fix & High-Precision Toolbar Iconography

NoteSciPad v1.1.0 delivers streamlined native Light and Dark modes without external palette overhead, restored toolbar dividing line and classic peach tab styling, Scintilla 5.6.7/Lexilla 5.5.4/Lua 5.5.1 engine upgrades, inline mathematical expression evaluation, 7 trending language lexers, complete elimination of light-mode dark theme leakage in `AboutDialog` and `DisplayInfoDialog`, compiler scope resolution for native palettes, redesigned 5-tab modern About dialog, universal dialog checkbox/radio layout overhaul, and pixel-perfect 16x16 toolbar iconography.

#### 🛡️ Theme Leakage Elimination & Dialog Polish
- **Zero Dark Mode Leakage in Light Mode**: Completely eliminated all usage of fragile Qt `palette(alternate-base)` and `palette(base)` stylesheets across `AboutDialog` and `DisplayInfoDialog`. On host operating systems set to Dark Mode (such as macOS Sequoia/Sonoma or Linux dark themes), Qt previously resolved those palette macros to dark gray (`#2d2d2d`), resulting in unsightly dark rectangles inside light-mode dialogs.
- **Dynamic Theme Palette Binding (`updateThemeColors`)**: Implemented dynamic theme event listeners connected to `ThemeManager::themeChanged` in `AboutDialog` and `DisplayInfoDialog`. Color tokens are resolved deterministically using explicit CSS hex definitions (`#fafbfc` card background, `#f0f2f5` window background, `#e1e4e8` border, `#24292f` primary text, `#57606a` secondary text in Light Mode; `#282a36`, `#1e1f29`, `#44475a`, `#f8f8f2` in Dark Mode).
- **Proportional About Dialog Sizing**: Calibrated About Dialog dimensions to a compact, elegant 540x400 px frame with multi-tab diagnostics.

#### 🔧 Compiler Hardening & 13-Pillar C++ Refactoring
- **Scope Resolution in ThemeManager**: Fixed Linux GCC compiler errors where `findThemePaletteById` was referenced without being in scope by binding direct pointer fallbacks (`&kDarkPalette` / `&kLightPalette`). Verified 100% build success on `ubuntu-24.04` (gcc 6.5 & gcc 6.8).
- **13-Pillar Static Audit Clean**: Clean across all files in `src/` via Trunk Check and Cppcheck. Resolved all warnings for `missingOverride`, `noExplicitConstructor`, `shadowFunction`, `shadowMember`, `cstyleCast`, `constVariablePointer`, `ignoredReturnErrorCode`, and `passedByValue`.

---

### ✨ Added

#### Native Dark & Light Mode Streamlining

- **Simplified Native Dark and Light Modes**: Streamlined ThemeManager to exclusively support dedicated Dark and Light modes (`kLightPalette` and `kDarkPalette`) without external palette catalogs or ThemeSwitcherDialog.
- **Emoji-Free GUI**: Removed all emoji icons from theme menu actions ("Light Mode", "Dark Mode", "Toggle Dark / Light Mode") ensuring a clean, distraction-free native user interface.
- **Dynamic Mode Synchronization**: Bound action check states directly to active theme mode with real-time status bar feedback.

#### Toolbar Icon Clarity & High-Precision Redesign

- **Pixel-Perfect 16x16 Integer Grid**: Restored native 16x16 pixel-grid base icons to eliminate bilinear interpolation blur and invisibility on dark themes (`#181a1f`, `#1e1e1e`, `#25282c`) caused by downscaled 32x32 faint outlines.
- **Completely Redesigned Toolbar Icons**:
  - **Find (`find.png`)**: High-precision azure/cyan magnifying glass with 3D specular white gleam and polished gold metallic handle.
  - **Find & Replace (`findReplace.png`)**: Compact search lens paired with reciprocal exchange swap arrows in neon emerald (`#10b981`) and electric amber/gold (`#f59e0b`) with sharp contour borders.
  - **Zoom In (`zoomIn.png`)**: Ocean azure magnifying glass enclosing a bold, high-contrast white plus sign (`+`).
  - **Zoom Out (`zoomOut.png`)**: Matching ocean azure magnifying glass enclosing a bold, high-contrast white minus sign (`−`).
  - **Show Indent (`indentGuide.png`)**: Code block hierarchy with glowing dual amber/gold vertical alignment rails (`#f59e0b`) guiding indented emerald and cyan code bars.
- **Contrast Invariants**: Verified across both Dark Mode (average luminance delta > 75 on dark toolbars, peak 225.0) and Light Mode (average luminance delta > 102).

#### Universal Checkbox & Radio Button Layout & Spacing Overhaul

- **Standardized Spacing & Padding**: Configured explicit `spacing: 8px;`, `padding: 3px 4px;`, `min-height: 20px;`, and 16x16 indicator dimensions (4px border-radius for checkboxes, 9px for radio buttons).
- **Crisp Vector Checkmark**: Bundled `src/icons/checkmark.svg` via `src/resources.qrc` and `image: url(:/icons/checkmark.svg);`, restoring crisp vector checkmarks without platform rendering anomalies.
- **Radio Button Indicator Rendering**: Enforced sharp circular indicator dot via `border: 3px solid %5; background-color: %6;` on checked state.
- **Dialog Layout Calibration**: Calibrated dialog layouts across Find & Replace, Preferences, Transparency, EncodingSelection, MacroRun, PerformanceDiagnostics, TailOptions, ManageToolsDialog, StatusBarCustomizerDialog, and FocusedViewDialog.

#### NotepadNext Workspace Default Light Palette & Dark Mode Contrast Calibration

- **Light Palette Calibration**: Calibrated `kLightPalette` to match the NotepadNext default workspace aesthetic (`#f0f2f5` window, `#fafbfc` canvas/card, `#e1e4e8` border, `#24292f` primary text, `#57606a` secondary text), avoiding harsh, eye-straining pure white backgrounds.
- **Dark Mode Text Contrast**: Calibrated `kDarkPalette` text contrast (`#cdd6f4` primary text, `#a6adc8` secondary text) for sharp, crisp readability without blurriness.

#### Official Thirdparty Engine Modernization (Scintilla 5.6.7, Lexilla 5.5.4, Lua 5.5.1)

- **Scintilla 5.6.7**: Integrated upstream Scintilla release from the main branch into `thirdparty/scintilla/`, providing enhanced multi-caret editing, scroll optimization, and sub-millisecond line navigation.
- **Lexilla 5.5.4**: Integrated upstream Lexilla release into `thirdparty/lexilla/` for robust multi-language lexing.
- **Lua 5.5.1 / 5.4.7**: Upgraded embedded Lua core in `thirdparty/lua/` directly from upstream source archives.

#### Native UI Divider & Tab Border Restoration

- **Toolbar Dividing Line Restoration**: Added `border-bottom: 1px solid %3;` and refined padding to `QToolBar` in `src/features/ThemeManager.cpp`, restoring the clean horizontal separation line between toolbar and editor tabs.
- **Classic Tab Top Border Color Restoration**: Restored classic soft peach (`rgb(255, 202, 176)`) active tab top border and orange focus top border matching `npp.css`, stripping out unwanted blue accent line from `ads--CDockWidgetTab`.

#### Comprehensive 13-Pillar Code Quality Audit

- **Signal Disconnect Safety**: Fixed signal disconnect bug on editor switch in `QuickFindWidget::setEditor(ScintillaNext *newEditor)` where previously `disconnect` was invoked on the new editor instead of `this->editor`.
- **Parameter Shadowing Elimination**: Eliminated parameter shadows in `ReleaseManager` (`currentVersionStr`), `TransparentWindowManager` (`targetMode`), and `QuickFindWidget` (`newEditor`).
- **Const Pointer Correctness**: Enforced pointer constness on `ScreenDisplayManager` (`activeScreen`, `primary`, `scr`) and `ReleaseManager` (`btnDownload`).
- **Explicit Constructors**: Marked `PreferencesDialog` and `SearchResultsCollector` constructors `explicit` to prevent implicit conversions.
- **Const Reference Returns**: Returned `const QString &` in `FileBrowserDock::currentDirectory()`; reserved capacity for `cleanedFilters`.
- **Parameter Constness**: Passed `searchTerm` and `line` parameters by `const QString &` across search handlers.
- **Type-Safe Casts**: Replaced C-style cast with `reinterpret_cast` in `LuaConsoleDock.cpp`.
- **Static Helpers**: Marked stateless helper functions as `static` in `AutoClosePairs`, `DocumentationManager`, and `FocusedView`.

#### Official Application Icon & Attribution

- **Icon Design by Lon McGregor**: NoteSciPad's official application icon has been adapted from the celebrated open-source artwork by Lon McGregor ([`LonMcGregor/NotepadPlusPlusIcons`](https://github.com/LonMcGregor/NotepadPlusPlusIcons)), honoring open-source iconography heritage across macOS (`.icns`), Windows (`.ico`), and Linux desktop environments (`.png`).

#### Redesigned 5-Tab About Dialog

- **Modern 5-Tab Interface (`AboutDialog`)**:
  - _About NoteSciPad_: High-resolution application icon, architecture overview, and legal non-affiliation statement.
  - _Maintainer Information_: Complete contact information for Harry DS Alsyundawy (ALSYUNDAWY IT SOLUTION) including Email, Website, GitHub, Telegram, PayPal, and QRIS barcode link (_strictly omitting phone number and WhatsApp_).
  - _Credits & Attribution_: Formal upstream attribution to Scintilla, Lexilla, NotepadNext, Notepad4, Notepad3, and open-source contributors.
  - _System & Diagnostics_: Real-time operating system, kernel version, CPU build architecture, Qt build/runtime version, display scale ratio, and one-click "Copy Diagnostics to Clipboard".
  - _License_: Complete GNU General Public License v3.0 text and FSF links.

#### EditorBase Feature Synthesis & Math Evaluation

- **Inline Mathematical Expression Evaluation (`Ctrl+Shift+E` / `Cmd+Shift+E`)**: Powered by `MathEvaluator` and `InlineMathEvaluator`, allowing instant calculation of selected expressions or current line (arithmetic, trigonometry, logarithms, powers, roots, constants pi, e, tau, phi).
- **Replace or Append Results**: Support appending result (`2 * pi * 5 = 31.4159265`) or replacing expression directly with formatted numerical result.

#### Trending Language Lexers

- **Mojo (`mojo.lua`)**: Modular Mojo systems programming language syntax (`.mojo`, `.🔥`).
- **Solidity (`solidity.lua`)**: Ethereum smart contract language syntax (`.sol`).
- **Julia (`julia.lua`)**: High-performance scientific computing language syntax (`.jl`).
- **Dockerfile (`docker.lua`)**: Multi-stage container definitions (`Dockerfile`, `Containerfile`).
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

---

### 👤 Maintainer & Attribution

- **Lead Maintainer & Modernization**: **Harry Dertin Sutisna Alsyundawy** ([`@alsyundawy`](https://github.com/alsyundawy)) — [`ALSYUNDAWY IT SOLUTION`](https://alsyundawy.com)
- **Upstream & Heritage Acknowledgments**:
  - **Neil Hodgson & Scintilla/Lexilla Contributors**: Foundational editing and lexing engines.
  - **Justin Dailey (dail8859)**: Original NotepadNext cross-platform Qt scaffolding.
  - **Zufu Liu (zufuliu)**: Notepad4 architectural insights and utility patterns.
  - **Rizonesoft**: Notepad3 text transformation idioms and ergonomics.
