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

- [x] Color preview via CallTip (hex/RGB detection + swatch)
- [x] Compress & Pretty for CSS, JavaScript, JSON, JSON5
- [x] Copy as RTF (syntax-colored)
- [x] Evaluate expression (math — start simple, evolve toward TinyExpr++ style)
- [x] Focused View (filter to matching lines)
- [x] Customizable status bar fields
- [x] .LOG auto-timestamp behavior
- [x] URL highlighting with configurable size threshold (UrlThreshold)
- [x] Improved encoding detection (study & port useful parts from Notepad4/Notepad3)
- [x] **Show Character Info** (code point, name, category, UTF-8/UTF-16)
- [x] **Update File Time** (touch file without changing content)
- [x] **Save with Original Time** (preserve original mtime on save)
- [x] Improved case conversion (invert / title / sentence) + context menu entries + multi-selection support

---

## Phase 3 — Polish & Advanced (Tier 3)

- [x] Function List dock
- [x] Run / External tools menu
- [x] Change History margin (optional)
- [x] Stronger regex option exploration (PCRE2 / Boost)
- [x] AES-256 encryption (evaluate security & UX carefully)
- [x] Clipboard monitoring mode (optional)
- [x] Transparent window mode
- [x] Lexer / API data updates from Notepad4 (selective, keep Lexilla)
- [x] Optional: Evaluate JS Expression
- [x] Optional: Monitoring / tail mode improvements
- [x] Performance profiling & optimization (Apple Silicon + Linux)

---

## Phase 4 — macOS & Linux Excellence & Release

- [x] Native menu bar polish & Services integration
- [x] Improve default-editor script
- [x] Codesign + notarization workflow (macOS)
- [x] Create .deb and .rpm packages
- [x] Accessibility review (keyboard, VoiceOver)
- [x] Dark mode visual polish
- [x] Multi-monitor & Retina testing
- [x] Documentation (user guide + developer)
- [x] Public alpha / beta release
- [x] Homebrew cask or formula (optional)
- [x] Flatpak / AppImage (optional)

---

## Cross-Cutting Tasks

- [x] Never replace or remove vendored Scintilla and Lexilla without explicit decision
- [x] Unit / integration tests for new utilities (Base64, number conversion, etc.)
- [x] Manual test checklist (encoding, large files, CJK, shortcuts)
- [x] Localization updates (Indonesian priority + English)
- [x] About dialog credits (NotepadNext, Notepad4, Notepad3, Scintilla, Lexilla) + clear non-affiliation statement
- [x] Performance & memory baseline measurements
- [x] Security review for any new features that handle external data

---

## Definition of Done (per feature)

- [x] Code reviewed
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
- [x] Works on both arm64 and x64 (macOS) and Linux
- [x] Keyboard shortcut assigned (where applicable)
- [x] Menu item added
- [x] Basic error handling
- [x] No obvious regressions in existing features
- [x] Still uses Scintilla + Lexilla
- [x] Documented in changelog

---

## Phase 5 — CI/CD Perfection, 13-Pillar Audit & Trunk Quality Engineering

### Task 20: Fix GitHub Actions Workflow Failures & Modernize Actions

- [x] Unignore & track `src/languages/makefile.lua` in git (fixes missing file Ninja failure)
- [x] Create `.mega-linter.yml` with exclusions for `thirdparty/`, `build/`, `installer/`
- [x] Fix `build.yml` Linux Qt 6.8+ architecture (`linux_gcc_64` vs `gcc_64`) and optimize runners
- [x] Fix `release.yml` non-existent runner `macos-26` (replace with `macos-15`)
- [x] Fix `lupdate.yml`, `codespell.yml`, `codeql.yml`, and `jekyll-gh-pages.yml` branch (`main`) and repo URLs
- [x] Modernize all workflow action versions (`checkout@v4`, `upload-artifact@v4`)

### Task 21: Trunk Linter Optimization & Zero-Spike Configuration

- [x] Add `thirdparty/**`, `build/**`, `installer/**`, `deploy/windows/**` to `.trunk/trunk.yaml` ignore list
- [x] Run `trunk check` safely with CPU/RAM conservation and verify 100% clean exit

### Task 22: World Standard Color Palette & WCAG 2.1 AA/AAA Compliance Verification

- [x] Audit editor canvas and chrome colors for WCAG 2.1 AA/AAA contrast ratios
- [x] Implement explicit `scColor(r, g, b)` / `scFromRgb(QRgb)` helper for Scintilla BGR conversion
- [x] Author `doc/COLOR_STANDARDS_AND_ACCESSIBILITY.md` and record ADR-024

### Task 23: Exhaustive 13-Pillars Code Quality & Security Audit

- [x] Bug Review (null checks, boundary conditions, off-by-one errors)
- [x] Syntax Review (C++20 standards, CMake, shell scripts)
- [x] Runtime Review (cross-platform path separators, locale safety)
- [x] Logic Review (state machine transitions, crypto/hash roundtrips)
- [x] Memory Review (RAII, Qt parent-child hierarchy, file handles)
- [x] Dead Code Review (clean stubs, obsolete declarations)
- [x] Duplicate Code Review (clean deduplication without premature abstraction)
- [x] Circular Dependency Review (clean include hierarchy, forward declarations)
- [x] Performance Bottleneck Review (hot path allocations, ReDoS safety)
- [x] Security Vulnerability Review (OWASP Top 10 2025, CWE Top 25 2025)
- [x] Maintainability Review (single responsibility, clean interfaces)
- [x] Scalability Review (large file buffers, margin widths)
- [x] Readability Review (idiomatic C++20 / Qt 6 conventions)
- [x] Author `doc/13_PILLARS_CODE_REVIEW_REPORT.md` and record ADR-025

### Task 24: Empirical Verification & CI Confirmation

- [x] Run `scripts/verify-engine-invariants.sh` (100% pass)
- [x] Run `scripts/run-unit-tests.sh` (100% pass)
- [x] Run `scripts/run-security-audit.sh` (100% pass)
- [x] Run `scripts/benchmark-baseline.sh` (100% pass)
- [x] Run `trunk check` (100% pass)
- [ ] Commit, push to `origin/main`, and confirm GitHub Actions success

---

### Notes

- Prefer small, reviewable PRs / commits.
- Mark tasks `[x]` when fully done.
- Move blocked items to a “Blocked” section with reason.
- Update this file regularly.
- Feature list reflects research of Notepad4, Notepad3, and Notepad++ up to October 2026.
- Local repository: `/Users/alsyundawy/Downloads/GitHub/NoteSciPad`
