# NoteSciPad — Workground

**Purpose:** Development guidelines, environment, conventions, and working rules for the NoteSciPad project.  
**Intended use:** Antigravity IDE + human developers.  
**Local Path:** `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`  
**Last updated:** 2026-10-19 (after deep research of Notepad4 / Notepad3 / Notepad++ 2020–2026)  

---

## 0. Source Acquisition & Engine Policy (Critical)

### Fresh Source (No `git clone`)

Always begin from a clean snapshot of the latest **alsyundawy/NotepadNext-MacOS**:

1. Download the latest **Source code (zip)** or **tar.gz** from  
   <https://github.com/alsyundawy/NotepadNext-MacOS/releases>  
   (prefer the newest tag, currently around `0.15-02` or later).
2. Extract it into `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`.
3. Verify these directories/files exist and are intact:
   - `thirdparty/scintilla/`
   - `thirdparty/lexilla/`
   - `src/ScintillaNext.h` and `src/ScintillaNext.cpp`
   - Root `CMakeLists.txt`
4. Initialize a **new** Git repository (`git init`) so the history starts clean.
5. Push to the new `NoteSciPad` (or `notesci-pad`) repository.

### Scintilla + Lexilla (Mandatory)

- **Always** use the vendored copies in `thirdparty/scintilla` and `thirdparty/lexilla`.
- They are statically linked via CMake. Do **not** switch to system packages, Homebrew `qscintilla2`, or external QScintilla.
- Do **not** delete or replace these directories unless performing a carefully tested upstream update.
- When updating in the future:
  - Download official Scintilla and Lexilla releases.
  - Merge carefully while preserving the Qt platform layer and existing CMake integration.
  - Re-test folding, indicators, auto-completion, rendering, and large files on both arm64 and x64.
- All new editor features must be implemented on top of `ScintillaNext` (the existing wrapper).

### Naming Policy

- **Product name:** NoteSciPad
- **Binary / package name:** `notesci` or `notesci-pad` (prefer lowercase, no spaces)
- **macOS app bundle:** `NoteSciPad.app`
- Always state clearly in About and README that the project is **not affiliated** with Notepad++, NotepadNext, Notepad4, or Notepad3.

---

## 1. Development Environment

### Required

- macOS 12+ (preferably 13+ for full Qt matrix)
- Xcode Command Line Tools
- CMake ≥ 3.21
- Ninja
- Qt 6.5 LTS or newer (6.8 / 6.10 recommended)
- Git

### Recommended

- Qt Creator (optional, CMake-first workflow preferred)
- clang-format
- Apple Silicon Mac for primary development
- Local workspace: `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`

### Build Commands (baseline)

```bash
cd /Users/alsyundawy/Downloads/GitHub/NoteSciPad
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
# Run
open build/NoteSciPad.app
```

---

## 2. Repository & Branching

- `main` — stable, releasable
- `develop` — integration branch
- `feature/<name>` — individual features (e.g. `feature/mark-all`, `feature/file-browser`)
- `fix/<name>` — bug fixes

Keep commits focused. Prefer conventional commit messages:

```text
feat: add mark all occurrences with count
fix: correct Base64 decode padding handling
docs: update implementation plan
```

---

## 3. Coding Standards

### Language & Style

- C++20
- Follow existing NotepadNext style where possible
- Use clang-format (project `.clang-format` if present, otherwise Qt style)
- Prefer Qt types (`QString`, `QByteArray`, `QFile`, etc.) over raw C APIs
- RAII and smart pointers; avoid raw `new`/`delete`

### Naming

- Classes: `PascalCase`
- Methods / variables: `camelCase`
- Private members: `m_` prefix or trailing `_` (be consistent with surrounding code)
- Constants / enums: `PascalCase` or `kConstant`

### Comments

- Comment non-obvious logic, especially when porting behavior from Notepad4/Notepad3
- Reference original source when adapting logic:

  ```cpp
  // Adapted from Notepad4 EditAutoC / mark-occurrences logic
  // Uses Scintilla indicators via ScintillaNext
  ```

### Error Handling

- Prefer Qt signals/slots and return values over exceptions for UI code
- Validate user input (Base64, number conversion, file paths)
- Log important errors (use existing logging if available, otherwise `qWarning`)

---

## 4. Project Structure Guidelines

```text
src/
├── docks/              # FileBrowserDock, future FunctionList, etc.
├── dialogs/
├── widgets/
├── utils/              # Base64, NumberConversion, Guid, Timestamp, CharacterInfo helpers
├── features/           # MarkOccurrences, FocusedView, ColorPreview, etc.
└── ...
thirdparty/
├── scintilla/          # DO NOT REMOVE — always keep
├── lexilla/            # DO NOT REMOVE — always keep
└── ...
```

Keep new features modular. Prefer new classes over large modifications to core files when possible.

---

## 5. macOS & Linux Specific Rules

- Respect macOS keyboard conventions:
  - `Cmd` for primary shortcuts
  - `Option` for word navigation
  - Avoid conflicting with system shortcuts
- Use native full-screen when possible
- Test Gatekeeper / quarantine behavior
- High-DPI / Retina: use high-resolution icons and proper device pixel ratio
- Dark mode: verify colors in both light and dark appearance
- File paths: always use `QDir` / `QFileInfo`; never assume backslashes
- For Linux: test on both .deb and .rpm based systems when possible

---

## 6. Feature Implementation Guidelines

### When Porting from Notepad4 / Notepad3

1. Understand the original behavior thoroughly
2. Re-implement in Qt/Scintilla terms (do not copy Win32 code)
3. Keep the user-facing behavior as close as practical
4. Add a comment referencing the original feature
5. Write at least a basic test or manual verification steps
6. Always go through `ScintillaNext`

### Scintilla Usage

- Prefer the existing `ScintillaNext` wrapper
- Use indicators / markers for temporary highlighting (mark-all, focused view)
- Avoid fighting the Scintilla document model
- Never bypass Scintilla for core text operations

### UI Consistency

- Follow existing menu structure where possible
- Place new tools under **Tools** or **Edit** menus
- Provide both menu entry and keyboard shortcut for important actions
- Update status bar thoughtfully (don’t overcrowd)

---

## 7. Testing Checklist (minimum per feature)

- [ ] Works on Apple Silicon and Intel
- [ ] Works on Linux (at least one .deb and one .rpm distro if possible)
- [ ] Light and dark mode
- [ ] Empty document, single line, large file
- [ ] UTF-8, UTF-16, and at least one CJK file
- [ ] Undo/Redo still works correctly
- [ ] No crash on cancel / invalid input
- [ ] Keyboard shortcut functional
- [ ] Menu item present and correctly enabled/disabled
- [ ] Still correctly linked against vendored Scintilla + Lexilla

---

## 8. Antigravity IDE / AI Prompting Guidelines

When asking the AI to implement a feature:

1. Reference this Workground and the Task List
2. Explicitly remind: “Always use the vendored Scintilla and Lexilla. Do not replace them.”
3. Be specific: “Implement Mark All Occurrences in a new class `MarkOccurrences` that uses ScintillaNext indicators”
4. Require:
   - Clean, commented code
   - Menu + shortcut
   - Basic error handling
   - No unnecessary Win32 dependencies
5. Ask for a short summary of changes and testing steps
6. Prefer incremental changes over large rewrites

**Good prompt example:**
> Ikuti WORKGROUND.md dan TASK_LIST.md.  
> Implementasikan fitur "Mark All Occurrences + Matched Count" untuk NoteSciPad.  
> Gunakan ScintillaNext dan indicators. Jangan ganti atau hapus thirdparty/scintilla dan thirdparty/lexilla.  
> Buat class terpisah, tambahkan menu + shortcut, dan tampilkan jumlah match di status bar.

---

## 9. Credits & Attribution

Always maintain clear attribution:

- NotepadNext (dail8859)
- NotepadNext-MacOS (alsyundawy)
- Notepad4 (zufuliu)
- Notepad3 (rizonesoft)
- Scintilla / Lexilla (Neil Hodgson)
- Qt

**Mandatory statement in About dialog and README:**
> NoteSciPad is an independent project. It is not affiliated with, endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.

Update the About dialog and README when adding significant ported functionality.

---

## 10. Definition of Ready (for a task)

A task is ready to implement when:

- It is listed in TASK_LIST.md
- Acceptance criteria are clear
- Dependencies (if any) are done
- Potential macOS/Linux or encoding edge cases are noted
- It does not require replacing Scintilla or Lexilla

---

## 11. Quick Reference Commands

```bash
cd /Users/alsyundawy/Downloads/GitHub/NoteSciPad

# Format (if clang-format available)
find src -name "*.cpp" -o -name "*.h" | xargs clang-format -i

# Clean rebuild
rm -rf build && cmake -S . -B build -G Ninja && cmake --build build --parallel

# Check for obvious issues
# (add ctest or custom scripts later)
```

---

**This document is living.** Update it when conventions change or new patterns emerge.

**Primary maintainer:** HARRY DERTIN SUTISNA ALSYUNDAWY  
**Local repository:** `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`
