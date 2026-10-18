# NoteSciPad — Task List

**Project:** NoteSciPad — Alternative Notepad for Mac & Linux  
**Base:** Fresh source from alsyundawy/NotepadNext-MacOS (latest tag)  
**Engine:** Always Scintilla + Lexilla (vendored)  
**Local Path:** `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`  
**Last updated:** 2026-10-19 (after deep research of Notepad4 / Notepad3 / Notepad++ 2020–2026)  

Use this as the living task board. Check items as they are completed.

---

## Phase 0 — Foundation

### Source Acquisition (Mandatory)

- [x] Download latest source zip/tar.gz from <https://github.com/alsyundawy/NotepadNext-MacOS/releases> (prefer tag `0.15-02` or newer)
- [x] Extract into `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`
- [x] Verify `thirdparty/scintilla/` and `thirdparty/lexilla/` exist and are complete
- [x] Verify `src/ScintillaNext.*` and CMake integration are intact
- [x] `git init` + initial commit (new history, no clone)
- [x] Create new remote repository `NoteSciPad` (or `notesci-pad`) and push

### Project Setup

- [x] Update branding: application name **NoteSciPad**, About dialog, window title, Info.plist
- [x] Replace icons with NoteSciPad specific icons (.icns + high-res)
- [x] Update README (Indonesian + English) — mention Scintilla + Lexilla, source origin, and that it is **not** affiliated with Notepad++
- [x] Establish coding standards document (see WORKGROUND.md)
- [x] Verify clean build on arm64 and x64 (Qt 6.5 / 6.8 / 6.10)
- [x] Confirm Scintilla and Lexilla are statically linked and functional
- [x] Set up basic CI (GitHub Actions macOS + Linux runners)
- [x] Document current feature inventory of base NotepadNext
- [x] Create feature branch strategy (`main` / `develop` / `feature/*`)
- [x] Decide binary/package name: `notesci` or `notesci-pad`

---

## Phase 1 — Core Identity (Tier 1)

### 1. Mark All Occurrences + Matched Count

- [x] Design indicator/marker strategy (using Scintilla indicators)
- [x] Implement mark-all action (selection or word under caret)
- [x] Add matched count display (status bar or dedicated label)
- [x] Options: case sensitive, whole word, regex (optional)
- [x] Keyboard shortcut + menu entry
- [x] Clear marks action
- [x] Test with large files and CJK text

### 2. Enhanced Auto-completion

- [x] Auto-close parentheses, brackets, braces, quotes
- [x] Improve auto-indent rules
- [x] Context-based suggestions (where practical)
- [x] Configurable fill-up characters
- [x] Settings UI for auto-completion behavior
- [x] Test across major languages

### 3. File Browser Dock (matepath-style)

- [x] Create `FileBrowserDock` (QDockWidget)
- [x] Integrate QFileSystemModel + QTreeView
- [x] Double-click / Enter to open file
- [x] Context menu (Open, Reveal in Finder / File Manager, Copy Path, etc.)
- [x] Remember last directory and expanded state
- [x] Toolbar button / menu to toggle
- [x] Drag & drop support
- [x] Optional filter box

### 4. Base64 Encode / Decode

- [x] Action: Encode selection (or whole document)
- [x] Action: Decode selection
- [x] Encoding awareness (UTF-8 default)
- [x] Error handling for invalid Base64
- [x] Menu placement (Tools or Edit)

### 5. Number Conversion

- [x] Decimal ↔ Hexadecimal
- [x] Decimal ↔ Octal
- [x] Decimal ↔ Binary
- [x] Handle selection or number under caret
- [x] Prefix options (0x, 0b, etc.)

### 6. Insert GUID + Timestamps

- [x] Insert new GUID (various formats)
- [x] Insert current timestamp (configurable formats)
- [x] Menu entries + optional shortcuts

### 7. Full-screen Mode

- [x] Toggle full-screen (native macOS + Qt)
- [x] Menu entry + shortcut (F11 or Cmd+Ctrl+F)
- [x] Persist preference if desired

---

## Phase 2 — High-Value Enhancements (Tier 2)

- [ ] Color preview via CallTip (hex/RGB detection + swatch)
- [ ] Compress & Pretty for CSS, JavaScript, JSON, JSON5
- [ ] Copy as RTF (syntax-colored)
- [ ] Evaluate expression (math — start simple, evolve toward TinyExpr++ style)
- [ ] Focused View (filter to matching lines)
- [ ] Customizable status bar fields
- [ ] .LOG auto-timestamp behavior
- [ ] URL highlighting with configurable size threshold (UrlThreshold)
- [ ] Improved encoding detection (study & port useful parts from Notepad4/Notepad3)
- [ ] **Show Character Info** (code point, name, category, UTF-8/UTF-16)
- [ ] **Update File Time** (touch file without changing content)
- [ ] **Save with Original Time** (preserve original mtime on save)
- [ ] Improved case conversion (invert / title / sentence) + context menu entries + multi-selection support

---

## Phase 3 — Polish & Advanced (Tier 3)

- [ ] Function List dock
- [ ] Run / External tools menu
- [ ] Change History margin (optional)
- [ ] Stronger regex option exploration (PCRE2 / Boost)
- [ ] AES-256 encryption (evaluate security & UX carefully)
- [ ] Clipboard monitoring mode (optional)
- [ ] Transparent window mode
- [ ] Lexer / API data updates from Notepad4 (selective, keep Lexilla)
- [ ] Optional: Evaluate JS Expression
- [ ] Optional: Monitoring / tail mode improvements
- [ ] Performance profiling & optimization (Apple Silicon + Linux)

---

## Phase 4 — macOS & Linux Excellence & Release

- [ ] Native menu bar polish & Services integration
- [ ] Improve default-editor script
- [ ] Codesign + notarization workflow (macOS)
- [ ] Create .deb and .rpm packages
- [ ] Accessibility review (keyboard, VoiceOver)
- [ ] Dark mode visual polish
- [ ] Multi-monitor & Retina testing
- [ ] Documentation (user guide + developer)
- [ ] Public alpha / beta release
- [ ] Homebrew cask or formula (optional)
- [ ] Flatpak / AppImage (optional)

---

## Cross-Cutting Tasks

- [ ] Never replace or remove vendored Scintilla and Lexilla without explicit decision
- [ ] Unit / integration tests for new utilities (Base64, number conversion, etc.)
- [ ] Manual test checklist (encoding, large files, CJK, shortcuts)
- [ ] Localization updates (Indonesian priority + English)
- [ ] About dialog credits (NotepadNext, Notepad4, Notepad3, Scintilla, Lexilla) + clear non-affiliation statement
- [ ] Performance & memory baseline measurements
- [ ] Security review for any new features that handle external data

---

## Definition of Done (per feature)

- [ ] Code reviewed
  - BUG REVIEW.
  - SYNTAX REVIEW.
  - RUNTIME REVIEW.
  - LOGIC REVIEW.
  - MEMORY REVIEW.
  - DEAD CODE REVIEW.
  - DUPLICATE CODE REVIEW.
  - CIRCULAR DEPENDENCY REVIEW.
  - PERFORMANCE BOTTLENECK REVIEW.
  - SECURITY VULNERABILITY REVIEW.
  - MAINTAINABILITY REVIEW.
  - SCALABILITY REVIEW.
  - READABILITY REVIEW.
- [ ] Works on both arm64 and x64 (macOS) and Linux
- [ ] Keyboard shortcut assigned (where applicable)
- [ ] Menu item added
- [ ] Basic error handling
- [ ] No obvious regressions in existing features
- [ ] Still uses Scintilla + Lexilla
- [ ] Documented in changelog

---

### Notes

- Prefer small, reviewable PRs / commits.
- Mark tasks `[x]` when fully done.
- Move blocked items to a “Blocked” section with reason.
- Update this file regularly.
- Feature list reflects research of Notepad4, Notepad3, and Notepad++ up to October 2026.
- Local repository: `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`
