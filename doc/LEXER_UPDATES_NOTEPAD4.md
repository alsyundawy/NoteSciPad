# Selective Lexer & Language Definition Updates from Notepad4 (Keeping Lexilla)

## Executive Summary

As part of Phase 3, Task 8 of NoteSciPad's roadmap, this document details the selective integration of lexer configurations and modern language syntax definitions from **Notepad4** (authored by Zufu Liu) into NoteSciPad, while strictly preserving our vendored **ScintillaNext + thirdparty/scintilla + thirdparty/lexilla** engine architecture.

---

## 1. Context & Architectural Rationale

NoteSciPad is a lightweight, high-performance C++20 / Qt 6 editor designed for macOS and Linux. While NoteSciPad bundles Lexilla 5.4.x (which already contains advanced lexers such as `LexTOML.cxx`, `LexZig.cxx`, `LexDart.cxx`, and `LexCPP.cxx`), its Lua-based language definition layer lacked mappings for several modern programming and configuration languages.

Specifically:
- **TOML**: Despite `LexTOML.cxx` existing in Lexilla (originally ported directly from Zufu Liu's Notepad4 TOML lexer by Jiri Techet), NoteSciPad had no `toml.lua` definition.
- **Zig**: Lexilla provides `LexZig.cxx` (`SCLEX_ZIG`), but NoteSciPad lacked a `zig.lua` style and keyword map.
- **Dart**: Lexilla provides `LexDart.cxx` (`SCLEX_DART`), but NoteSciPad had no `dart.lua`.
- **TypeScript**: TypeScript was previously bundled inside `javascript.lua`, preventing distinct styling, extension routing, and TypeScript-specific keyword support (such as `satisfies`, `keyof`, `infer`, utility types, and generic decorators).
- **C++20 / C++23**: `cpp.lua` was missing modern ISO C++ keywords (`co_await`, `co_yield`, `co_return`, `import`, `module`, `char8_t`).
- **Rust 2021 / 2024**: `rust.lua` was missing `await`, `macro_rules`, and key standard library types (`Option`, `Result`, `Vec`, `Box`, `Pin`, `Poll`, etc.).
- **Go 1.18 - 1.24**: `go.lua` mixed control statements and data types in a single style category and lacked modern generics (`any`, `comparable`) and built-in functions (`min`, `max`, `clear`).

---

## 2. Integrated Changes & Modernizations

### 2.1 TOML Support (`src/languages/toml.lua`)
- **Lexer**: `toml` (`SCLEX_TOML`).
- **Extensions**: `.toml`.
- **Keywords**: `true`, `false`, `inf`, `nan`, `+inf`, `-inf`, `+nan`, `-nan`.
- **Styles**: Complete style mapping matching Lexilla `SCE_TOML_*` indices:
  - `DEFAULT` (0), `COMMENT` (1), `IDENTIFIER` (2), `KEYWORD` (3), `NUMBER` (4), `TABLE` (5), `KEY` (6), `ERROR` (7), `OPERATOR` (8), `STRING_SQ` (9), `STRING_DQ` (10), `TRIPLE_STRING_SQ` (11), `TRIPLE_STRING_DQ` (12), `ESCAPECHAR` (13), `DATETIME` (14), `STRINGEOL` (15).

### 2.2 Zig Support (`src/languages/zig.lua`)
- **Lexer**: `zig` (`SCLEX_ZIG`).
- **Extensions**: `.zig`, `.zon`.
- **Keywords**:
  - `[0]` Primary: `const`, `var`, `pub`, `fn`, `struct`, `enum`, `union`, `error`, `try`, `catch`, `comptime`, `defer`, `errdefer`, `async`, `await`, etc.
  - `[1]` Types: `u8`..`u128`, `i8`..`i128`, `usize`, `isize`, `f16`..`f128`, `bool`, `void`, `anyerror`, `anyopaque`, etc.
  - `[2]` Built-ins: 80+ `@`-prefixed compiler intrinsics (`@import`, `@as`, `@intCast`, `@ptrCast`, `@memcpy`, `@panic`, etc.).
- **Styles**: `SCE_ZIG_*` mapping for comments, documentation comments (`///` and `//!`), keywords, operators, multiline strings, and built-in functions.

### 2.3 Dart Support (`src/languages/dart.lua`)
- **Lexer**: `dart` (`SCLEX_DART`).
- **Extensions**: `.dart`.
- **Keywords**:
  - `[0]` Primary: Modern Dart 3 keywords (`sealed`, `base`, `interface`, `final`, `mixin`, `when`, `required`, `late`, `async`, `await`, `yield`, etc.).
  - `[1]` Types: `int`, `double`, `num`, `String`, `List`, `Map`, `Set`, `Future`, `Stream`, `Object`, `Never`, `Type`, `Record`, etc.
- **Styles**: Complete `SCE_DART_*` mapping for raw strings, multiline strings, metadata annotations (`@override`, etc.), and doc comments.

### 2.4 Dedicated TypeScript Support (`src/languages/typescript.lua`)
- **Lexer**: `cpp` with `lexer.cpp.backquoted.strings = 1`.
- **Extensions**: `.ts`, `.tsx`, `.mts`, `.cts`.
- **Separation**: Cleanly extracted from `javascript.lua` so `.ts`/`.tsx` files map directly to TypeScript.
- **Keywords**:
  - `[0]` Core syntax: `type`, `interface`, `enum`, `namespace`, `declare`, `satisfies`, `keyof`, `infer`, `override`, `readonly`, `abstract`, etc.
  - `[1]` Types: `any`, `unknown`, `never`, `Record`, `Partial`, `Required`, `Readonly`, `Pick`, `Omit`, `ReturnType`, `Awaited`, etc.
  - `[2]` TSDoc / JSDoc: `@param`, `@returns`, `@type`, `@template`, `@deprecated`, etc.
  - `[3]` Environment: `globalThis`, `process`, `console`, etc.

### 2.5 C++20 / C++23 Modernization (`src/languages/cpp.lua`)
- Added C++20 coroutine keywords: `co_await`, `co_yield`, `co_return`.
- Added C++20 module keywords: `import`, `module`.
- Added C++20 UTF-8 type: `char8_t`.

### 2.6 Rust 2021 / 2024 Modernization (`src/languages/rust.lua`)
- Added missing edition keywords: `await`, `macro_rules`.
- Added standard library types: `Option`, `Result`, `Some`, `None`, `Ok`, `Err`, `String`, `Vec`, `Box`, `Rc`, `Arc`, `Pin`, `Poll`, `Cell`, `RefCell`, `Mutex`, `RwLock`, `NonZeroU8`..`NonZeroUsize`.

### 2.7 Go 1.18 - 1.24 Modernization (`src/languages/go.lua`)
- Separated instruction keywords (`break`, `func`, `go`, `select`, etc.) into slot `[0]` for primary keyword styling.
- Placed types and built-in functions (`any`, `comparable`, `min`, `max`, `clear`, `make`, `new`, `append`, `iota`, `panic`, etc.) into slot `[1]` for secondary/type highlighting.

---

## 3. UI, Shortcut & Menu Integration

1. **Language Menu**: All 4 newly registered languages (`TOML`, `Zig`, `Dart`, `TypeScript`) are automatically incorporated into NoteSciPad's alphabetized **Language** menu (`ui->menuLanguage`), bringing the total supported languages to 91.
2. **Language Inspector Shortcut**:
   - `LanguageInspectorDock` is wired to the main window with global shortcut **`Ctrl+Alt+L`** (or **`Cmd+Alt+L`** on macOS).
   - A dedicated entry is permanently available under both **View -> Language Inspector** and at the bottom of the **Language** menu.
3. **File Filter & Autodetection**:
   - Files with `.toml`, `.zig`, `.zon`, `.dart`, `.ts`, `.tsx`, `.mts`, and `.cts` now automatically trigger their respective lexers upon opening or renaming.

---

## 4. Verification & Validation

- **Lua Syntax**: All 8 modified/created `.lua` files passed syntax and runtime evaluation via PUC-Rio Lua 5.5 without warnings or errors.
- **Resource Manifest**: `src/scripts.qrc` validated via `xml.etree.ElementTree` parsing.
- **Engine Invariants**: All modifications were performed purely at the configuration and presentation layers on top of `ScintillaNext` and Lexilla, with zero disruption to core C++ Scintilla document models.
