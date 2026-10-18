# NoteSciPad — Implementation Plan

**Project:** NoteSciPad — Alternative Notepad for Mac & Linux  
**Base:** Fresh source from alsyundawy/NotepadNext-MacOS (latest tag, preferably 0.15-02 or newer)  
**Editor Engine:** Always Scintilla + Lexilla (vendored in `thirdparty/`)  
**Goal:** Lightweight, feature-rich Scintilla-based editor for macOS and Linux  
**Local Repository Path:** `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`  
**Date:** 2026-10-19  
**Status:** Planning (updated after deep research of Notepad4 / Notepad3 / Notepad++ 2020–2026)  

---

## Research Note (2020–2026)

Deep research was performed on official GitHub repositories, release notes, and community sources up to October 2026:

- **Notepad4** (zufuliu): Latest v26.10r6392 (18 Oct 2026). Key recent additions include Update File Time, Save with Original Time, Show Character Info, Calculate Expression + Evaluate JS Expression, configurable URL threshold, improved find/replace, and continuous lexer/API updates.
- **Notepad3** (rizonesoft): Recent 7.26.x series (2026). Strengthened TinyExpr++ math evaluation, Focused View with multiple modes, Mark All + count, AES-256, PCRE2, context-menu case conversion, and many practical utilities.
- **Notepad++**: Up to v8.9.8.1 (Sep 2026). Mostly security fixes, multi-editing polish, monitoring improvements, and longer search limits. Many advanced features are plugin-based and less relevant for a lightweight port.

The feature list below has been updated to incorporate the most practical and high-value items from this research while keeping the project lightweight.

---

## 0. Source Acquisition (Mandatory — No `git clone`)

**Always start from the freshest official source of alsyundawy/NotepadNext-MacOS without using `git clone`.**

### Recommended method (clean snapshot)

1. Go to <https://github.com/alsyundawy/NotepadNext-MacOS/releases>
2. Download the latest release **Source code (zip)** or **Source code (tar.gz)**  
   - Preferred: tag `0.15-02` or any newer tag if available  
   - Alternatively: download the latest `master` branch archive from the Code tab → “Download ZIP”
3. Extract the archive into a clean working directory (recommended: `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`).
4. Verify the presence of:
   - `thirdparty/scintilla/`
   - `thirdparty/lexilla/`
   - `src/ScintillaNext.*`
   - `CMakeLists.txt`
5. Initialize a **new** Git repository:

   ```bash
   cd /Users/alsyundawy/Downloads/GitHub/NoteSciPad
   git init
   git add .
   git commit -m "Initial import from alsyundawy/NotepadNext-MacOS <tag>"
   ```

6. Create the new remote repository (`NoteSciPad` or `notesci-pad`) and push.

### Why this method

- Guarantees a clean, reproducible starting point
- Avoids carrying unnecessary Git history or submodules
- Matches the current structure where Scintilla and Lexilla are vendored (not external submodules)

### Scintilla & Lexilla Policy (Strict)

- **Always** keep and use the vendored copies under `thirdparty/scintilla` and `thirdparty/lexilla`.
- Do **not** replace them with system packages, Homebrew, or QScintilla unless explicitly decided later.
- Do **not** remove the static linking setup in CMake.
- When updating Scintilla/Lexilla in the future:
  1. Download the official release from <https://www.scintilla.org/> and Lexilla.
  2. Carefully merge into `thirdparty/` while preserving the Qt platform layer and CMake integration already present.
  3. Test thoroughly (especially folding, indicators, auto-completion, and macOS/Linux rendering).
- Current known baseline (as of late 2026): Scintilla ≈ 5.6.x and Lexilla ≈ 5.5.x (exact versions are inside the vendored sources).

---

## 1. Project Overview

Create a lightweight text editor for **macOS and Linux** that combines:

- The solid cross-platform foundation of **NotepadNext** (Qt 6, Scintilla + Lexilla, docking, session, packaging)
- The lightweight philosophy and distinctive features of **Notepad4** (zufuliu) and valuable additions from **Notepad3** (rizonesoft)

**Product name:** NoteSciPad  
**Binary / Package name:** `notesci` or `notesci-pad`  
**macOS app:** `NoteSciPad.app`

The result should feel closer to Notepad4 / Notepad3 than a full Notepad++ clone: fast startup, excellent encoding handling, strong auto-completion, mark-all + count, file browser, and practical power-user tools without bloat.

### Non-Goals

- Full Notepad++ plugin compatibility (Win32-specific)
- Perfect 1:1 UI clone of Windows Notepad4
- Replacing the entire NotepadNext architecture
- Switching away from Scintilla + Lexilla
- Using names that could be confused with Notepad++ trademark

---

## 2. Architecture Decisions

| Decision | Choice | Rationale |
| --- | --- | --- |
| Base codebase | Fresh snapshot of alsyundawy/NotepadNext-MacOS | Platform layer, CMake, .app packaging, Gatekeeper, Retina, dark mode, i18n already solved |
| UI Framework | Qt 6 Widgets | Already in use; mature on macOS and Linux |
| Editor engine | **Scintilla + Lexilla** (vendored in `thirdparty/`) | Mandatory. Proven; same family as Notepad4/Notepad3 |
| File browser | New dock (QFileSystemModel) | Simpler and more maintainable than porting matepath Win32 code |
| Settings | QSettings + optional INI compatibility | Qt-native; later add Notepad4.ini import if needed |
| License | GPL-3.0 (inherited from NotepadNext) | Clear; document attribution to Notepad4/Notepad3 |
| Product name | **NoteSciPad** | Distinct, safe from trademark issues |

### Key Source References

- Notepad4 logic: `src/EditAutoC.cpp`, `Edit.cpp`, `Styles.cpp`, mark-occurrences, Base64, number conversion, character info, file time utilities
- Notepad3 ideas: Focused View, TinyExpr++-style evaluation, .LOG handling, context menu utilities
- NotepadNext: Existing `ScintillaNext`, docking, session, find/replace, thirdparty/scintilla + lexilla

---

## 3. Phased Roadmap

### Phase 0 — Foundation (1–2 weeks)

- Download fresh source (see Section 0) and create new repository at `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`
- Clean project setup, branding (**NoteSciPad**), Info.plist, icons
- Establish coding standards and folder structure
- Baseline build matrix (arm64 + x64, Qt 6.5 / 6.8 / 6.10+)
- Confirm Scintilla + Lexilla static linking works
- Document current feature inventory of base NotepadNext

### Phase 1 — Core Identity (Tier 1)

Priority features that define the product:

1. Mark all occurrences + matched count
2. Enhanced Auto-completion (braces/quotes, context-based, better indent)
3. File Browser dock (matepath-style)
4. Base64 encode/decode
5. Number conversion (dec/hex/oct/bin)
6. Insert GUID + timestamps
7. Full-screen mode

### Phase 2 — High-Value Enhancements (Tier 2)

1. Color preview via CallTip
2. Compress & Pretty (CSS/JS/JSON/JSON5)
3. Copy as RTF
4. Evaluate expression (math — start simple, evolve toward TinyExpr++ style; optional JS later)
5. Focused View
6. Customizable status bar
7. .LOG auto-timestamp
8. URL highlighting with configurable size threshold (UrlThreshold)
9. **Show Character Info** (Unicode code point, name, category, UTF-8/UTF-16 bytes)
10. **Update File Time** (touch current file without changing content)
11. **Save with Original Time** (preserve original timestamp on save)
12. Improved case conversion (invert, title, sentence) with multi-selection support + context menu entries

### Phase 3 — Polish & Advanced (Tier 3)

1. AES-256 encryption (optional)
2. Function List
3. Run / External tools menu
4. Change History margin
5. Stronger regex option (PCRE2 or Boost)
6. Improved encoding detection (port Notepad4/Notepad3 logic + uchardet)
7. Lexer/API data updates from Notepad4 (while keeping Lexilla)
8. Optional: Evaluate JS Expression (using system JavaScript engine if practical on macOS)
9. Optional: Clipboard monitoring mode
10. Optional: Monitoring / tail mode improvements

### Phase 4 — macOS & Linux Excellence

- Native menu bar polish, Services, Quick Look
- Default editor script improvements
- Notarization + hardened runtime (macOS)
- .deb and .rpm packaging
- Performance profiling on Apple Silicon and Linux
- Accessibility (VoiceOver, keyboard)

---

## 4. Technical Approach per Feature

### Mark All Occurrences + Count

- Use Scintilla indicators or custom markers
- Maintain count in status bar or dedicated label
- Support case sensitivity and whole-word options
- Reference: Notepad4 mark logic + Notepad3 Focused View foundation

### Enhanced Auto-completion

- Extend existing NotepadNext auto-completion
- Add auto-close for `()`, `[]`, `{}`, quotes
- Context-aware suggestions (reuse or adapt Notepad4 `EditAutoC` data where practical)
- Better auto-indent rules

### File Browser Dock

- `QDockWidget` + `QFileSystemModel` + `QTreeView`
- Quick open, drag-drop, context menu (Open, Reveal in Finder / File Manager, Copy Path, etc.)
- Remember last path and expansion state
- Optional: filter, bookmarks inside browser

### Text Utilities (Base64, Number, GUID, etc.)

- Implement as actions in Edit menu or dedicated Tools menu
- Pure C++ / Qt (no external deps where possible)
- Base64: `QByteArray::toBase64` / `fromBase64` with encoding awareness

### Show Character Info

- For character under caret or first selected character
- Display: code point, Unicode name (if available), category, UTF-8 and UTF-16 representations
- Use Qt or ICU-like lookup; fallback to basic info if full name database is heavy

### File Time Utilities

- Update File Time: use `QFile` or `utimes` to touch the file
- Save with Original Time: capture original mtime before save and restore after successful write

### Color Preview CallTip

- Detect hex/RGB color under caret or selection
- Show CallTip with colored swatch
- Optional click → `QColorDialog`

---

## 5. Risks & Mitigations

| Risk | Mitigation |
| --- | --- |
| Feature creep | Strict tier system; Phase 1 must ship first |
| Encoding edge cases | Heavy testing with CJK, UTF-16, BOM, NFO, legacy code pages |
| macOS keyboard conventions | Map Cmd vs Ctrl carefully; respect system shortcuts |
| Performance on large files | Profile early; reuse Notepad4 large-file lessons |
| License / attribution | Clear credits in About dialog and README |
| Trademark issues | Use distinct name **NoteSciPad**; never claim affiliation with Notepad++ |
| Divergence from upstream NotepadNext | Prefer minimal invasive changes; keep upstream mergeability where possible |
| Accidental replacement of Scintilla/Lexilla | Strict policy: always keep vendored copies in `thirdparty/` |

---

## 6. Success Criteria

- Phase 1 features fully working and stable on arm64 + x64 (macOS) and Linux
- Startup time and memory footprint remain competitive with Notepad4 (lightweight)
- Encoding handling at least as good as current NotepadNext, preferably better
- Scintilla + Lexilla remain the sole editor engine
- Clean, maintainable code with comments on non-obvious logic
- Documented build & contribution guide
- Ready for public alpha after Phase 1

---

## 7. Next Steps

1. Download the freshest alsyundawy/NotepadNext-MacOS source (Section 0)
2. Create the new repository at `/Users/alsyundawy/Downloads/GitHub/NoteSciPad` and perform initial branding to **NoteSciPad**
3. Verify build with the existing Scintilla + Lexilla
4. Implement Mark All Occurrences as first vertical slice
5. Iterate with real usage feedback

**Owner:** HARRY DERTIN SUTISNA ALSYUNDAWY  
**Repository target:** `NoteSciPad` (local: `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`)  
**Primary tools:** Antigravity IDE, Qt Creator / CMake, Apple Silicon Mac
