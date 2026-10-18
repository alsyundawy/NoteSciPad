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
