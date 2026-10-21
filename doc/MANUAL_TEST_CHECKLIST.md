# NoteSciPad Comprehensive Manual QA Test Checklist

This manual verification protocol ensures high quality, rock-solid stability, zero UI regression, and robust text processing across macOS and Linux prior to public releases.

---

## 1. Character Encodings & BOMs

| Test Case     | Procedure                              | Expected Result                                                                | Status   |
| ------------- | -------------------------------------- | ------------------------------------------------------------------------------ | -------- |
| **TC-ENC-01** | Open UTF-8 with BOM file               | Status bar shows `UTF-8-BOM`; text renders cleanly without `` artifacts        | [ ] Pass |
| **TC-ENC-02** | Open plain UTF-8 (No BOM)              | Status bar shows `UTF-8`; no unexpected character conversions                  | [ ] Pass |
| **TC-ENC-03** | Open UTF-16 LE and BE files            | Auto-detected as UTF-16; multi-byte characters display properly                | [ ] Pass |
| **TC-ENC-04** | Open ISO-8859-1 / Windows-1252         | Auto-detected by uchardet; accented European characters (`é, ç, ü, ñ`) correct | [ ] Pass |
| **TC-ENC-05** | Convert Encoding (`Encoding > ...`)    | Converts in-place; dirty dot updates; save verifies converted bytes on disk    | [ ] Pass |
| **TC-ENC-06** | Convert Line Endings (`CRLF` <-> `LF`) | Status bar reflects line ending; hex dump confirms `\r\n` or `\n`              | [ ] Pass |

---

## 2. Large Files & Performance

| Test Case      | Procedure                         | Expected Result                                                    | Status   |
| -------------- | --------------------------------- | ------------------------------------------------------------------ | -------- |
| **TC-PERF-01** | Open 10 MB text/code file         | Opens in < 500ms; smooth scrolling at 60/120 fps; no UI freeze     | [ ] Pass |
| **TC-PERF-02** | Open 50 MB log file               | Opens without crashing; memory footprint remains < 150MB           | [ ] Pass |
| **TC-PERF-03** | Find/Replace in 50 MB file        | Find highlights matches quickly; Replace All completes smoothly    | [ ] Pass |
| **TC-PERF-04** | Syntax Highlighting on large file | Large source file lexing does not stutter typing or caret movement | [ ] Pass |

---

## 3. CJK & Complex Script Typography

| Test Case     | Procedure                            | Expected Result                                                                                        | Status   |
| ------------- | ------------------------------------ | ------------------------------------------------------------------------------------------------------ | -------- |
| **TC-CJK-01** | Simplified & Traditional Chinese     | Input Chinese via native IME; candidate popup aligns with caret; glyphs sharp                          | [ ] Pass |
| **TC-CJK-02** | Japanese (Kanji, Hiragana, Katakana) | Japanese characters render crisp; arrow key moves through double-width glyphs correctly                | [ ] Pass |
| **TC-CJK-03** | Korean (Hangul)                      | Hangul syllables compose cleanly without ghost backspaces                                              | [ ] Pass |
| **TC-CJK-04** | Emoji & Symbols (`Ctrl+Cmd+Space`)   | Palette opens; inserting multi-codepoint emojis (`👨‍💻, 🚀, ⚡`) works without split surrogate artifacts | [ ] Pass |

---

## 4. Keyboard Shortcuts & Menu Invariants

| Test Case     | Shortcut (macOS)            | Shortcut (Linux)               | Verification                                    | Status   |
| ------------- | --------------------------- | ------------------------------ | ----------------------------------------------- | -------- |
| **TC-KEY-01** | `Cmd+N` / `Cmd+O` / `Cmd+S` | `Ctrl+N` / `Ctrl+O` / `Ctrl+S` | Document lifecycle works                        | [ ] Pass |
| **TC-KEY-02** | `Cmd+W`                     | `Ctrl+W`                       | Closes active tab (prompts if dirty)            | [ ] Pass |
| **TC-KEY-03** | `Cmd+F` / `Cmd+H`           | `Ctrl+F` / `Ctrl+H`            | Find & Replace dialogs open and focus           | [ ] Pass |
| **TC-KEY-04** | `Ctrl+Alt+E`                | `Ctrl+Alt+E`                   | Focuses Scintilla editor canvas immediately     | [ ] Pass |
| **TC-KEY-05** | `Cmd+Alt+T`                 | `Ctrl+Alt+T`                   | Toggles Dark / Light mode instantly             | [ ] Pass |
| **TC-KEY-06** | `Cmd+Alt+Right` / `Left`    | `Ctrl+Alt+Right` / `Left`      | Moves window between connected screens          | [ ] Pass |
| **TC-KEY-07** | `Cmd+Alt+C`                 | `Ctrl+Alt+C`                   | Centers window within active display            | [ ] Pass |
| **TC-KEY-08** | `Cmd+Alt+D`                 | `Ctrl+Alt+D`                   | Opens Display & Retina diagnostics dialog       | [ ] Pass |
| **TC-KEY-09** | `Shift+F1`                  | `Shift+F1`                     | Opens Keyboard Shortcuts & Accessibility dialog | [ ] Pass |
| **TC-KEY-10** | `F1`                        | `Ctrl+F1`                      | Opens User Guide                                | [ ] Pass |
| **TC-KEY-11** | `Cmd+Shift+U`               | `Ctrl+Shift+U`                 | Checks for updates via ReleaseManager           | [ ] Pass |

---

## 5. macOS Native Features & Services

| Test Case     | Procedure                  | Expected Result                                                               | Status   |
| ------------- | -------------------------- | ----------------------------------------------------------------------------- | -------- |
| **TC-MAC-01** | Window dirty dot indicator | Typing creates black dirty dot in red close button; saving clears it          | [ ] Pass |
| **TC-MAC-02** | Proxy Icon dragging        | Dragging document title proxy icon into Mail/Finder links file                | [ ] Pass |
| **TC-MAC-03** | macOS Services menu        | Highlight text in TextEdit/Safari -> Services -> NoteSciPad actions function  | [ ] Pass |
| **TC-MAC-04** | VoiceOver speech           | Turn on VoiceOver (`Cmd+F5`) -> Tab switch produces spoken title announcement | [ ] Pass |

---

## 6. Security & Utilities

| Test Case      | Procedure                           | Expected Result                                                                   | Status   |
| -------------- | ----------------------------------- | --------------------------------------------------------------------------------- | -------- |
| **TC-SEC-01**  | AES-256 Encrypt & Decrypt           | Envelope generated; valid password restores identical text; bad password rejected | [ ] Pass |
| **TC-UTIL-01** | JavaScript Evaluator (`Ctrl+Alt+J`) | Evaluates JS expressions; formats JSON; handles syntax errors cleanly             | [ ] Pass |
| **TC-UTIL-02** | Log Tail Monitor (`Ctrl+Alt+M`)     | Follows file updates; Pause freezes stream; Regex highlights lines                | [ ] Pass |
| **TC-UTIL-03** | Base64 Codec (`Ctrl+Alt+B`)         | Encodes and decodes selection; preserves binary-safe UTF-8 bytes                  | [ ] Pass |
