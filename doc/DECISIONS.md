# Architecture & Project Decisions (ADR)

## ADR-001: Application and Binary Naming Convention

### Context
The project needs a distinct, consistent naming scheme across macOS app bundles, CLI commands, and Linux package managers (`.deb`, `.rpm`, AppImage) while complying with trademark policies and usability standards.

### Decision
1. **Product Display Name**: `NoteSciPad`
2. **CLI Executable / Command**: `notesci` (lowercase, concise, follows UNIX convention like `code`, `subl`)
3. **Linux Package Name**: `notesci` (with aliases `notesci-pad`)
4. **macOS App Bundle**: `NoteSciPad.app`
5. **macOS Bundle Identifier**: `com.alsyundawy.NoteSciPad`

### Rationale
- `notesci` provides a swift and frictionless terminal command (`notesci file.txt`).
- `NoteSciPad.app` provides standard macOS human-friendly naming in `/Applications`.
- Prevents trademark confusion with Notepad++.

---

## ADR-002: Strict Engine Policy

### Context
Maintaining high-performance text editing and accurate syntax highlighting across 470+ languages on macOS and Linux.

### Decision
Scintilla and Lexilla remain strictly vendored inside `thirdparty/` and statically linked. All new features must be implemented on top of `ScintillaNext` indicators and APIs without swapping the editor engine.

---

---

## ADR-003: Retaining and Enhancing Qt Embedded PCRE2 Engine over Boost.Regex

### Context
Users requested stronger regular expression capabilities matching PCRE / Perl / Notepad++ standards, including numbered backreferences in substitution (`\1`..`\9`, `$1`..`$9`, `\0`, `&`), Dot Matches Newline flag, bidirectional reverse regex search, and interactive testing. We evaluated PCRE2 C library, Boost.Regex C++, `std::regex`, and Qt 6 `QRegularExpression`.

### Decision
Qt 6 `QRegularExpression` is already implemented directly on top of PCRE2 with JIT compilation enabled. The search and replace limitations in legacy code were not caused by the engine, but by bridge omissions in `QRegexSearch.cpp`:
1. `SubstituteByPosition` performed naive string substitution, dropping backreferences.
2. `FindOption::Cxx11RegEx` was not translated to `QRegularExpression::DotMatchesEverythingOption`.
3. Backward regex search failed to calculate bounds correctly.

We enhanced `QRegexSearch.cpp` with a full PCRE2 substitution parser, enabled bidirectional search, wired the UI flag `ui->checkBoxRegexMatchesNewline` into `FindOption::Cxx11RegEx`, and integrated `RegexHelperDialog` (PCRE2 live tester and reference cheat sheet).

We decided to retain and optimize Qt embedded PCRE2 rather than linking external Boost.Regex (avoiding ~150MB build bloat) or separate libpcre2 C bindings.

### Rationale & Consequences
- **Zero New Dependencies**: Clean builds on macOS and Linux without external package manager hurdles.
- **High Performance**: Native PCRE2 JIT execution.
- **Full Compatibility**: Full support for backreferences, character classes, lookaheads, lookbehinds, and multiline mode.
- **Architectural Report**: Complete benchmarks and findings documented in `doc/REGEX_ENGINE_EXPLORATION.md`.

---

## ADR-004: Authenticated AES-256 Document Encryption (NSPENC01)

### Context
Users require robust, transparent file encryption for confidential notes, source code, and credentials. Prior implementations in Notepad3 utilized unauthenticated raw AES-256-CBC with 0 PBKDF iterations (vulnerable to bit-flipping, padding oracle, and dictionary attacks), while NppCrypt in Notepad++ introduced confusing menus with obsolete ciphers (DES, Blowfish) and external DLL dependencies.

### Decision
NoteSciPad implements authenticated document encryption using the `NSPENC01` binary format:
1. **Cipher**: AES-256-CBC with PKCS#7 padding (Apple CommonCrypto on macOS, portable standard FIPS-197 AES in pure C++ on Linux).
2. **Key Derivation**: PBKDF2-HMAC-SHA256 with 100,000 iterations and 16-byte random salt, deriving 64 bytes (32 bytes AES key + 32 bytes HMAC key).
3. **Authentication**: Mandatory Encrypt-then-MAC using HMAC-SHA256 over all container metadata and ciphertext.
4. **Timing Attack Defense**: Constant-time HMAC comparison before decryption.
5. **Zero New Dependencies**: Uses platform native CommonCrypto on macOS and Qt Core primitives + portable AES on Linux.
6. **UX**: Automatic recognition upon opening encrypted files (`DecryptDialog`), interactive live strength evaluation in `EncryptDialog`, and menu actions under `Tools -> Encryption`.

### Rationale & Consequences
- **Proven Cryptographic Security**: Fully compliant with NIST SP 800-38A, SP 800-132, and RFC 8018 standards.
- **Cross-Platform Interoperability**: Encrypted files created on macOS can be opened and decrypted seamlessly on Linux and vice-versa.
- **Data Safety**: Memory-only ephemeral passphrase retention per tab, with atomic file save preventing corruption.
- **Full Report**: Security evaluation, threat modeling, and UX specifications documented in `doc/ENCRYPTION_SECURITY_AND_UX.md`.


## ADR-005: Selective Lexer and Language Definition Updates from Notepad4 (Keeping Lexilla)

### Status
Accepted (2026-10-19)

### Context
NoteSciPad is committed to maintaining its vendored, statically compiled Scintilla 5.5.x and Lexilla 5.4.x engines (`thirdparty/scintilla`, `thirdparty/lexilla`, `src/ScintillaNext`). However, modern programming and configuration languages (such as TOML, Zig, Dart, and TypeScript) were either absent or improperly categorized in NoteSciPad's Lua language catalog. Notepad4 (by Zufu Liu) contains battle-tested lexer definitions and modern language catalogs designed specifically for Scintilla/Lexilla.

### Decision
NoteSciPad selectively ports language definitions, keyword lists, and style indices from Notepad4 into NoteSciPad's native Lua language layer without modifying or replacing the underlying Lexilla C++ engine:
1. **New Languages**: Added full definitions for `TOML` (`LexTOML.cxx`), `Zig` (`LexZig.cxx`), `Dart` (`LexDart.cxx`), and `TypeScript` (`LexCPP.cxx` with backquoted string support).
2. **Modern Keyword Sets**: Enhanced `cpp.lua` (C++20/C++23 coroutines and modules), `rust.lua` (Rust 2021/2024 `await`, `macro_rules`, and stdlib types), and `go.lua` (Go 1.18–1.24 generics, built-in functions, and type separation).
3. **Dedicated TypeScript Routing**: Removed `.ts` and `.tsx` from `javascript.lua` to ensure distinct TypeScript styling and metadata.
4. **Language Inspector Access**: Assigned `Ctrl+Alt+L` (`Cmd+Alt+L` on macOS) to `LanguageInspectorDock` and placed it prominently in both the `Language` and `View` menus.
5. **Engine Integrity**: Zero changes to Scintilla or Lexilla C++ source trees; all lexer IDs bind to existing compiled Lexilla modules.

### Rationale & Consequences
- **Expanded Modern Ecosystem**: Increases total registered languages from 87 to 91, delivering instant syntax highlighting for modern systems languages (Zig, Rust), modern web/mobile development (TypeScript, Dart), and configuration formats (TOML).
- **Zero Risk of Engine Instability**: By relying on Lexilla's existing built-in lexers (`lmTOML`, `lmZig`, `lmDart`, `lmCPP`), performance and memory characteristics remain strictly identical.
- **Detailed Reference**: Full mapping documentation recorded in `doc/LEXER_UPDATES_NOTEPAD4.md`.


## ADR-006: Native JavaScriptCore and Portable JavaScript Expression Evaluation

### Status
Accepted (2026-10-19)

### Context
NoteSciPad's roadmap (Phase 3, Task 9) includes an optional JavaScript expression evaluation feature inspired by Notepad4's "Evaluate JS Expression" tool. In Notepad4, Windows Chakra/JScript was used. NoteSciPad requires a high-performance, cross-platform solution that does not introduce heavy runtime dependencies or alter the Scintilla/Lexilla engine.

### Decision
NoteSciPad implements JavaScript expression evaluation through a modular `JsEvaluator` utility and `JsEvaluationDialog`:
1. **Engine Selection**:
   - On macOS: Links directly to Apple's native, system-bundled `JavaScriptCore.framework`. Requires zero package installations and delivers sub-millisecond execution.
   - On Linux / POSIX: Auto-detects standard CLI runtimes (`node`, `bun`, `deno`, or `qjs`) via `QStandardPaths` with process isolation and timeout protection.
2. **Result Formatting**: Automatically formats objects and arrays via `JSON.stringify(..., null, 2)` and captures detailed exception backtraces.
3. **Editor Integration**:
   - In-place replacement with shortcut `Ctrl+Shift+J` (`Cmd+Shift+J` on macOS).
   - In-place comment append (`// -> <result>`).
   - Interactive scratchpad dialog with shortcut `Ctrl+Alt+J` (`Cmd+Alt+J` on macOS).
4. **Scintilla Safety**: All document changes occur through `ScintillaNext` with undo/redo transactions.

### Rationale & Consequences
- **Instant Developer Utility**: Developers can quickly calculate numbers, format JSON, manipulate text, or run snippets without switching windows or launching a browser.
- **Zero Heavy Dependencies**: Avoids pulling in `qt6-declarative-dev` (QML), keeping NoteSciPad's build dependencies minimal and lean.
- **Complete Documentation**: Full architectural details recorded in `doc/JS_EVALUATOR.md`.


## ADR-007: Zero-Modal File Tail Monitoring Mode (tail -f) with Delta Appending

### Status
Accepted (2026-10-19)

### Context
Text editors typically alert users to external disk modifications using modal message boxes. When inspecting live log files, this behavior causes frustrating interruptions and repetitive dialogs. Notepad++ and Notepad4 provide dedicated "Monitoring (tail -f)" modes to solve this. NoteSciPad requires an efficient, zero-modal log streaming capability that operates safely on top of Scintilla.

### Decision
NoteSciPad implements live file monitoring through `TailMonitoringManager` and `TailOptionsDialog`:
1. **Delta Appending**: Instead of reloading the full file on disk writes, seeks to the previous file offset and appends only newly streamed bytes directly into `ScintillaNext` using `appendText()`.
2. **Modal Bypass**: Skips modal reload prompts in `checkFileForModification` when monitoring is active.
3. **Rotation Detection**: Detects file truncation or log rotation (`size < lastSize`), triggering a clean document reload.
4. **Auto-Scroll & Read-Only**: Locks document editing during streaming and scrolls caret to the bottom on every appended batch.
5. **Configurable Heartbeat**: Dual-mode notification with `QFileSystemWatcher` and customizable polling timer (250ms–5000ms).
6. **Shortcut & Menu**: Accessible under `View -> Monitoring (tail -f)` with shortcut `Ctrl+Shift+Y` (`Cmd+Shift+Y` on macOS).

### Rationale & Consequences
- **High-Performance Log Viewing**: Can stream high-throughput logs without UI lag or memory allocation churn.
- **Uninterrupted Workflow**: Eliminates modal popups during active development, test runs, and system monitoring.
- **Reference**: Full details documented in `doc/TAIL_MONITORING.md`.


## ADR-008: Hardware-Aware Performance Profiling, Memory Diagnostics, and Large-File Optimization

### Status
Accepted (2026-10-19)

### Context
Maintaining high responsiveness on modern hardware (Apple Silicon ARM64 and Linux multi-core architectures) requires transparent memory visibility, performance diagnostics, and optimization strategies for massive documents.

### Decision
NoteSciPad implements a hardware-aware performance engine through `PerformanceProfiler` and `PerformanceDiagnosticsDialog`:
1. **Zero-Overhead Native Diagnostics**:
   - macOS: Uses Mach kernel APIs (`mach_task_basic_info`, `machdep.cpu.brand_string`) to report true process RSS and CPU architecture without child processes.
   - Linux: Uses `/proc/self/statm` page calculation.
2. **Scintilla Throughput Benchmark**: Built-in 50,000-line ingestion test providing empirical throughput measurements (MB/s and lines/s).
3. **Large-File High-Performance Mode**: Provides an instant toggle to bypass word-wrap reflow calculation bottlenecks and enable double-buffered rendering on huge documents.
4. **Menu & Shortcut**: Integrated under `Help -> Performance & Diagnostics...` with shortcut `Ctrl+Alt+P` (`Cmd+Alt+P` on macOS).
5. **Engine Integrity**: Strictly preserves vendored `ScintillaNext + thirdparty/scintilla + thirdparty/lexilla`.

### Rationale & Consequences
- **Transparent Diagnostics**: Developers can inspect real-time memory usage and system capabilities directly inside the editor.
- **Smooth Large-File Handling**: Prevents UI stuttering when viewing multi-megabyte log files or large source code files.
- **Reference**: Full details documented in `doc/PERFORMANCE_PROFILING.md`.

---

## ADR-009: Native macOS Menu Bar Polish, Proxy Icon, and NSServices Integration

- **Status:** Accepted
- **Date:** 2026-10-19
- **Deciders:** Harry DS Alsyundawy (Maintainer), Antigravity Engine Architecture Team

### Context
NoteSciPad targets power users on macOS and Linux. macOS users expect strict adherence to Apple Human Interface Guidelines:
1. A standard **Window** menu containing window management controls (Minimize, Zoom, Full Screen), tab cycling, and a dynamic list of open documents.
2. A functional window proxy icon in the title bar allowing files to be dragged directly into other applications, as well as `Cmd+Click` parent folder navigation.
3. Native dirty dot indicator in the close button.
4. Native Emoji & Symbols palette access via `Ctrl+Cmd+Space`.
5. Integration with macOS system-wide **Services** (`NSServices`) so text selected in Safari, Mail, or Terminal can be opened or appended into NoteSciPad with one click.
6. Modern Linux desktop environments require comprehensive MIME type associations and desktop action shortcuts.

### Decision
1. **MacServicesHandler Architecture:**
   - Implemented `MacServicesHandler` with an Objective-C++ implementation (`.mm`) on macOS and a clean C++ stub (`.cpp`) on Linux.
   - On macOS, registers `NoteSciPadServicesProvider` with `[NSApp setServicesProvider:]` and updates system dynamic services (`NSUpdateDynamicServices()`).
   - Dispatches received service requests onto the main Qt GUI thread using `dispatch_async(dispatch_get_main_queue(), ...)`.
2. **Native Window Menu:**
   - Implemented `MainWindow::setupWindowMenu()` inserting `menuWindow` immediately prior to `menuHelp`.
   - Wired Minimize, Zoom, Full Screen, Next Document, Previous Document, Bring All to Front, and dynamic checked open documents (`&1` through `&9`).
3. **Proxy Icon & Window State:**
   - Added `setWindowFilePath(isFile ? editor->getFilePath() : QString())` and `setWindowModified(!editor->isSavedToDisk())` in `MainWindow`.
4. **Emoji & Symbols:**
   - Added `Edit > Emoji & Symbols` (`Ctrl+Cmd+Space`) invoking Cocoa `[NSApp orderFrontCharacterPalette:nil]`.
5. **Metadata & Deployment:**
   - Updated `deploy/macos/info.plist` with comprehensive `CFBundleDocumentTypes` (Plain Text & Source Code) and `NSServices` (`openSelectionService`, `appendSelectionService`, `openFileService`).
   - Updated `deploy/linux/NoteSciPad.desktop` with programming MIME types and actions (`NewWindow`, `NewEmptyDocument`).

### Consequences
- **Positive:** NoteSciPad feels like a first-class citizen on macOS and Linux, matching the behavior of native macOS editors like BBEdit and TextMate.
- **Positive:** Zero overhead on Linux; CMake conditionally compiles `.mm` or `.cpp` based on `APPLE`.
- **Positive:** 100% compliant with ScintillaNext and Lexilla engine invariants.
