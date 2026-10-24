# SparkNotePad — Base Feature Inventory & Gap Analysis

**Date:** 2026-10-19
**Release Version:** SparkNotePad v1.0.0
**Editor Engine:** Scintilla 5.x + Lexilla 5.x (vendored in `thirdparty/`)

---

## 1. Existing Feature Inventory (Inherited from NotepadNext)

### 1.1 Core Editor & Scintilla Integration

- **ScintillaNext Wrapper**: C++ Qt wrapper encapsulating Scintilla's native document model, styles, margins, and commands.
- **Lexilla Lexers**: Static linking to Lexilla lexers covering standard languages (C/C++, Python, Java, JS, HTML/XML, SQL, PHP, Bash, etc.).
- **Lua-Based Language Definitions**: Extensible language definition system in `src/languages/*.lua` configuring keyword lists, comment characters, and style mapping.
- **Multi-Tab & Session Management**:
  - `SessionManager`: Automatic session saving, dirty file state preservation, and tab layout restore across restarts.
  - `TabsQuickActionsBar`: Quick tab switching, tab closure, and editor list menu.
- **Docking System**: Qt Advanced Docking System (`qtadvanceddocking-qt6`) enabling draggable, dockable, floating panels.
- **Encoding & EOL Support**:
  - Encoding auto-detection via `uchardet`.
  - UTF-8, UTF-16LE, UTF-16BE, Windows-1252, ISO-8859, and common legacy code pages.
  - Line ending conversions (CRLF, LF, CR).

### 1.2 Search & Navigation

- **QuickFindWidget**: In-buffer search bar with forward/backward find and incremental search.
- **Find / Replace Dialog**: Extended search with case sensitivity, whole word, wrap around, and regular expressions (`SCI_OWNREGEX`).
- **Goto Line / Position**: Modal dialog for direct buffer navigation.
- **Find in Files**: Multi-threaded directory search across project files.

### 1.3 Text Editing & Transformation Tools

- **Sorting**: `ScintillaSorter` supporting lexicographical, numeric, case-sensitive, and reverse sorting.
- **Indentation & Whitespace**: Tab-to-space, space-to-tab, trim trailing whitespace, indent guides, and visible whitespace toggle.
- **Case Conversion**: Basic Uppercase and Lowercase transformation.
- **Line Operations**: Duplicate line, delete line, move line up/down, join lines.
- **Bookmarking**: Margin bookmark indicators and navigation shortcuts.

### 1.4 Platform Polish (macOS & Linux)

- **macOS Bundle**: Native `.app` bundle, `Info.plist`, Dark Mode styling, Retina display assets.
- **Gatekeeper Integration**: `SparkNotePad-Default-Editor.sh` for registering as default text editor via `duti`.
- **Packaging**: macOS DMG builder (`macdeployqt`) and Linux native DEB / RPM packaging (`CPack`).

---

## 2. Gap Analysis vs. Notepad4 & Notepad3

While the base codebase provides a robust Scintilla foundation, power-user utilities from **Notepad4** (zufuliu) and **Notepad3** (rizonesoft) are absent:

| Capability                             | Base NotepadNext Status             | Notepad4 / Notepad3 Baseline                                    | SparkNotePad Target Phase |
| -------------------------------------- | ----------------------------------- | --------------------------------------------------------------- | ------------------------- |
| **Mark All Occurrences + Match Count** | ❌ No count, basic highlight        | ✅ Scintilla indicator marking + real-time count in status bar  | **Phase 1**               |
| **Enhanced Auto-completion**           | ⚠️ Keywords only                    | ✅ Auto-close quotes/brackets, context completion, smart indent | **Phase 1**               |
| **File Browser Dock**                  | ❌ None (uses standard file dialog) | ✅ Integrated file explorer dock (`matepath` style)             | **Phase 1**               |
| **Base64 Encode / Decode**             | ❌ None                             | ✅ Direct in-place selection encode/decode                      | **Phase 1**               |
| **Number Conversion**                  | ❌ None                             | ✅ Dec ↔ Hex ↔ Oct ↔ Bin with prefixes                          | **Phase 1**               |
| **Insert GUID & Timestamps**           | ❌ None                             | ✅ Configurable GUID formats and timestamp insertion            | **Phase 1**               |
| **Full-Screen Mode**                   | ⚠️ Partial                          | ✅ Native macOS + Qt full-screen toggle                         | **Phase 1**               |
| **Color Preview CallTip**              | ❌ None                             | ✅ Hex/RGB hover swatch preview via Scintilla CallTip           | **Phase 2**               |
| **Pretty & Compress (JSON/CSS/JS)**    | ❌ None                             | ✅ In-place formatters without heavy external plugins           | **Phase 2**               |
| **Focused View**                       | ❌ None                             | ✅ Filter editor buffer to display only matching lines          | **Phase 2**               |
| **Show Character Info**                | ❌ None                             | ✅ Code point, Unicode category, UTF-8/UTF-16 byte dump         | **Phase 2**               |
| **File Time Utilities**                | ❌ None                             | ✅ Touch file time & save preserving original timestamp         | **Phase 2**               |

---

## 3. Implementation Plan Reference

All identified gaps are sequenced into actionable tasks within `TASK_LIST.md` and guided by `WORKGROUND.md` and `IMPLEMENTATION_PLAN.md`.
