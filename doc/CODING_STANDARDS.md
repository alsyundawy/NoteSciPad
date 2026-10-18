# NoteSciPad — Coding Standards & Architectural Guidelines

**Project:** NoteSciPad (Alternative Notepad for macOS & Linux)  
**Standard:** C++20 / Qt 6.5+ / ScintillaNext  
**Reference Document:** WORKGROUND.md  

---

## 1. Core Architectural Invariants (Non-Negotiable)

1. **Vendored Editor Engine**: Always use vendored copies of Scintilla (`thirdparty/scintilla`) and Lexilla (`thirdparty/lexilla`). Never replace them with system libraries or dynamic Qt bindings.
2. **ScintillaNext Wrapper**: All editor operations, indicators, markers, and styling calls must pass through `ScintillaNext` or clean helper wrappers. Do not bypass the Scintilla document model.
3. **Product Branding**: The product name is **NoteSciPad**. Binary name is `NoteSciPad` or `notesci`. Never use Notepad++, NotepadNext, or Notepad4 as application names.
4. **Mandatory Attribution & Non-Affiliation**: Maintain clear attribution in About dialog and README. Every ported algorithm from Notepad4 or Notepad3 must include source commentary.

---

## 2. C++ Language & Qt Idioms

- **C++ Standard**: C++20.
- **Resource Management**: Strict RAII; prefer smart pointers (`std::unique_ptr`, `std::shared_ptr`, `QScopedPointer`, `QPointer`) over raw `new`/`delete`.
- **Qt Types**: Prefer idiomatic Qt types (`QString`, `QByteArray`, `QList`, `QDir`, `QFileInfo`) for application code.
- **Const Correctness**: Mark non-mutating methods and parameters `const`.
- **Strings**: Use `QStringLiteral` for compile-time string constants and `tr()` for user-facing translatable strings.

---

## 3. Naming Conventions

| Entity | Convention | Example |
|---|---|---|
| Classes / Structs | `PascalCase` | `MarkOccurrences`, `FileBrowserDock` |
| Member Functions | `camelCase` | `markAll()`, `updateMatchCount()` |
| Local Variables | `camelCase` | `matchCount`, `editorInstance` |
| Member Variables | `camelCase` or `m_` prefix | `m_matchCount`, `editor` |
| Constants & Enums | `PascalCase` or `kConstant` | `IndicatorMarkOccurrences`, `kDefaultLimit` |
| Signals & Slots | `camelCase` (descriptive verbs) | `matchesCountChanged(int)` |

---

## 4. Modularity & File Organization

New features must be modular to keep core files maintainable:
- Dock widgets: `src/docks/` (e.g., `FileBrowserDock.h/.cpp`)
- Utilities & Converters: `src/utils/` (e.g., `Base64Utils.h`, `NumberConverter.h`)
- Editor Features: `src/features/` (e.g., `MarkOccurrences.h/.cpp`)
- Dialogs: `src/dialogs/`

Avoid large monolithic additions to `MainWindow.cpp`. Instead, instantiate feature controller classes that connect to `MainWindow` actions via Qt signals/slots.

---

## 5. Porting from Notepad4 / Notepad3

When adapting functionality from upstream projects:
1. Re-implement cleanly using Qt 6 and Scintilla APIs; avoid Win32 API calls (`HWND`, `SendMessage`, `LRESULT`).
2. Add a clear source comment:
   ```cpp
   // Adapted from Notepad4 EditAutoC / mark-occurrences logic
   // Source: zufuliu/Notepad4 (src/Edit.cpp)
   ```
3. Test edge cases: Unicode / CJK text, large files, empty documents, undo/redo stacks.

---

## 6. Keyboard Shortcuts & UI Guidelines

- **macOS vs Linux**:
  - Use `QKeySequence::StandardKey` where available (e.g., `QKeySequence::Find`).
  - On macOS, use `Cmd` for application actions (`Ctrl` equivalent).
  - Never conflict with native macOS system shortcuts (Cmd+H, Cmd+M, Cmd+Q, Cmd+Space).
- **Every Feature Requires**:
  - Menu action with clear title and tooltip.
  - Shortcut (if user-facing editor action).
  - Defensive error handling (non-crashing fallback).
