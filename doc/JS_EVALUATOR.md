# JavaScript Expression & Script Evaluator

## Executive Summary

As part of Phase 3, Task 9 of NoteSciPad's roadmap, this document details the implementation of the **Evaluate JS Expression** feature. Adapted from **Notepad4** (authored by Zufu Liu), this capability allows developers and technical writers to evaluate arbitrary JavaScript code, calculate complex mathematical formulas, manipulate text, format JSON, compute dates, and execute functional array algorithms directly inside NoteSciPad.

---

## 1. Context & Architecture

In Notepad4 on Windows, JavaScript evaluation is implemented using the Windows Chakra / JScript COM interface (`IActiveScript`). 

For NoteSciPad on macOS and Linux:
1. **macOS Native Engine**: Uses Apple's native, pre-installed **JavaScriptCore.framework** (`<JavaScriptCore/JavaScript.h>`). JavaScriptCore is the high-performance ECMAScript engine powering WebKit and Safari. It requires zero third-party downloads, zero npm packages, and executes expressions in under 1 millisecond directly inside the NoteSciPad process.
2. **Linux & POSIX Fallback**: Automatically discovers system JS runtimes (`node`, `bun`, `deno`, or `qjs`) via `QStandardPaths::findExecutable` and safely executes scripts with standard input/output streaming and a 3-second timeout guard.
3. **Engine Integrity**: All text operations go through NoteSciPad's existing `ScintillaNext` wrapper with atomic undo/redo transaction grouping (`beginUndoAction` / `endUndoAction`). The core vendored `thirdparty/scintilla` and `thirdparty/lexilla` engines remain completely untouched.

---

## 2. Component Design

The feature is implemented across two dedicated classes:

### 2.1 `JsEvaluator` (`src/utils/JsEvaluator.h`, `src/utils/JsEvaluator.cpp`)
- **`evaluate(const QString &script)`**:
  - Compiles and evaluates JavaScript expressions and multi-line scripts.
  - Automatically formats primitive values (`null`, `undefined`, booleans, numbers, strings).
  - Automatically pretty-prints returned JavaScript Objects and Arrays using `JSON.stringify(val, null, 2)`.
  - Captures runtime errors, exceptions, and execution latency in milliseconds.
- **`evaluateInEditor(ScintillaNext *editor, Action action)`**:
  - Automatically detects whether the user selected a code range or placed the caret on an expression line.
  - Supports `ReplaceSelection` (replaces expression directly with result).
  - Supports `AppendComment` (appends ` // -> <result>` after the expression).

### 2.2 `JsEvaluationDialog` (`src/dialogs/JsEvaluationDialog.h`, `src/dialogs/JsEvaluationDialog.cpp`, `src/dialogs/JsEvaluationDialog.ui`)
- Interactive, multi-line JavaScript scratchpad dialog.
- Monospace editor for code entry and read-only result viewer.
- Real-time execution latency badge (e.g., `✓ Evaluated successfully in 0.18 ms`).
- **Presets & Templates**:
  - Math & Trigonometry (`Math.hypot`, powers, square roots).
  - Gaussian & Normal distribution constants.
  - Functional Array chains (`filter`, `map`, `reduce`).
  - JSON formatting & beautification (`JSON.stringify(obj, null, 2)`).
  - UTC & Unix timestamps (`new Date().toISOString()`).
  - String manipulation & word reversals.
  - Regular expression group extractions.
  - Byte to Hex encoding (`TextEncoder` + array mapping).
- Quick Action buttons:
  - "Insert into Editor"
  - "Copy Result"
  - "Clear"
  - Keyboard shortcut: `Ctrl+Enter` / `Cmd+Enter` to evaluate.

---

## 3. UI, Menus & Keyboard Shortcuts

- **Menu Path**: Under **Edit -> JavaScript Expression**:
  - **Evaluate JS Expression (Replace)**: Evaluates the selection/line and replaces it with the result.  
    **Shortcut: `Ctrl+Shift+J`** (`Cmd+Shift+J` on macOS).
  - **Evaluate JS Expression (Append `// -> ...`)**: Evaluates and appends result as an inline comment.
  - **Interactive JS Evaluation Dialog...**: Opens the rich interactive dialog.  
    **Shortcut: `Ctrl+Alt+J`** (`Cmd+Alt+J` on macOS).

---

## 4. Verification & Validation

- **Native macOS Build**: Verified linking with `-framework JavaScriptCore` and C++20.
- **Result Formatting**: Validated with primitive strings, numbers, booleans, nested JSON objects, arrays, and error throw cases.
- **Safety**: Uncaught JavaScript syntax or runtime errors are cleanly caught and displayed in the status bar or dialog without crashing or freezing the application.
