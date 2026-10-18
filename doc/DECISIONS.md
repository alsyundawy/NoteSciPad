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
