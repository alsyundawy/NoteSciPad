# Native Menu Bar Polish, Window Management & macOS Services Integration

## 1. Overview

SparkNotePad is designed to deliver a first-class native experience on macOS and modern Linux desktop environments. Prior to this enhancement, the menu bar lacked a standard **Window** menu, window proxy icons were not linked to the underlying file path, the macOS system-wide **Services** menu could not interact with SparkNotePad, and Linux `.desktop` entries only associated with basic `text/plain`.

This update establishes full compliance with Apple Human Interface Guidelines (HIG) and the FreeDesktop Desktop Entry Specification.

---

## 2. Key Capabilities

### 2.1 Native macOS Window Menu (`menuWindow`)

- Placed in the canonical HIG position immediately before the **Help** menu.
- **Minimize** (`Cmd+M` on macOS, `Alt+F9` on Linux).
- **Zoom** (Maximize / Restore).
- **Toggle Full Screen** (`Cmd+Ctrl+F` on macOS, `F11` on Linux/Windows).
- **Next Document** (`Ctrl+Tab` / `Ctrl+PageDown`).
- **Previous Document** (`Ctrl+Shift+Tab` / `Ctrl+PageUp`).
- **Bring All to Front** (brings SparkNotePad windows to front).
- **Dynamic Document List**:
  - Dynamically updates upon `aboutToShow`.
  - Displays all open tabs with clean dirty indicators (`● `).
  - Checkmarks the currently active document.
  - Numbers the first 9 documents (`&1` through `&9`) for rapid keyboard navigation.
  - Sets full file paths as tooltips and status tips.

### 2.2 macOS Title Bar Proxy Icon & Dirty State

- **Proxy Icon**: Implemented via `QMainWindow::setWindowFilePath(editor->getFilePath())`.
  - Enables dragging the document proxy icon directly from the window title bar into Finder, Terminal, Mail, Slack, or other apps.
  - Enables `Cmd+Click` on the title bar to display and navigate the parent folder hierarchy.
- **Window Dirty State**: Controlled via `setWindowModified(!editor->isSavedToDisk())`.
  - Displays the native macOS dirty dot indicator inside the red close button when unsaved changes exist.

### 2.3 Native Emoji & Symbols Palette

- Located at **Edit > Emoji & Symbols** with native macOS shortcut `Ctrl+Cmd+Space`.
- Triggers Cocoa `[NSApp orderFrontCharacterPalette:nil]` directly at the cursor insertion point.

### 2.4 macOS System Services (`NSServices`)

Implemented via `MacServicesHandler` and Cocoa `SparkNotePadServicesProvider`:

1. **SparkNotePad: New Document with Selection**:
   - Available in any macOS application (Safari, Mail, Terminal, TextEdit, Pages).
   - Right-click or use **App > Services > SparkNotePad: New Document with Selection** to immediately spawn a new tab in SparkNotePad with the selected text.
2. **SparkNotePad: Append Selection to Current Document**:
   - Appends selected text from external apps directly to the active SparkNotePad document.
3. **SparkNotePad: Open File**:
   - Enables opening files directly via the macOS Services menu.

### 2.5 Linux Desktop Integration (`deploy/linux/SparkNotePad.desktop`)

- Associated with all common source code and text MIME types:
  - `text/plain`, `text/x-c`, `text/x-c++`, `text/x-python`, `text/x-shellscript`, `application/json`, `text/markdown`, `text/html`, `text/css`, `application/xml`, `text/x-lua`, `text/x-rust`, `text/x-go`, `text/x-php`, `text/x-java`, `text/x-sql`, `text/x-yaml`.
- Provides Desktop Actions:
  - **New Window** (`SparkNotePad`)
  - **New Empty Document** (`SparkNotePad`)

---

## 3. Architecture & Cross-Platform Implementation

```
               ┌────────────────────────────────────────────────────────┐
               │                MainWindow / Menu Bar                   │
               │  - Window Menu (Minimize, Zoom, Tabs, Dynamic Docs)    │
               │  - Edit > Emoji & Symbols (Ctrl+Cmd+Space)             │
               │  - Title Bar Proxy Icon (setWindowFilePath)            │
               └───────────────────────────┬────────────────────────────┘
                                           │
                                           ▼
               ┌────────────────────────────────────────────────────────┐
               │                  MacServicesHandler                    │
               │  - Singleton instance()                                │
               │  - initialize(app) & setActiveWindow(window)           │
               └───────────────┬────────────────────────┬───────────────┘
                               │                        │
                     (APPLE)   │                        │   (NON-APPLE)
                               ▼                        ▼
               ┌────────────────────────┐      ┌────────────────────────┐
               │  MacServicesHandler.mm │      │ MacServicesHandler.cpp │
               │  - Objective-C++       │      │ - Lightweight C++ stub │
               │  - NSApp Services      │      │ - Zero overhead        │
               │  - NSServicesProvider  │      │ - Clean Linux fallback │
               └────────────────────────┘      └────────────────────────┘
```

- **Objective-C++ on macOS**: Compiled when `APPLE` is defined, linking `-framework AppKit`.
- **Pure C++ Fallback on Linux**: Statically compiled on Linux with no Cocoa or Mac runtime dependencies.
- **CMake Support**: Root `CMakeLists.txt` automatically enables `OBJCXX` on Apple platforms via `enable_language(OBJCXX)`.

---

## 4. Verification & Testing

- `deploy/macos/info.plist`: Linted and verified using Apple's `plutil -lint`.
- Static structure & bracket balance verified across all C++, Objective-C++, and header files.
- Verified absence of external dependencies or regressions in core ScintillaNext editor components.
