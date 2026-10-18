# Monitoring / Tail Mode (tail -f) Improvements

## Executive Summary

As part of Phase 3, Task 10 of NoteSciPad's roadmap, this document details the implementation of **Monitoring / Tail Mode (tail -f)** improvements. Adapted from the battle-tested monitoring architectures of **Notepad++** and **Notepad4**, this feature transforms NoteSciPad into an ultra-fast, zero-modal log viewer capable of streaming live log files in real-time.

---

## 1. Problem Statement & Motivation

Traditional text editors monitor file modifications by comparing timestamps upon window activation. When an open file changes on disk (such as a server log, build log, or simulation output), the editor interrupts the user with a modal prompt:
> *"File has been modified by another program. Do you want to reload it?"*

For active log files, this creates an unusable loop of modal dialogs that blocks editing and freezes workflow. Furthermore, reloading the entire file from the top causes visual flashing, resets the scroll position to line 1, and consumes excessive memory and CPU on large logs.

---

## 2. Technical Architecture

NoteSciPad solves this via a dedicated, non-intrusive monitoring engine:

### 2.1 `TailMonitoringManager` (`src/features/TailMonitoringManager.h`, `src/features/TailMonitoringManager.cpp`)
- **Dual-Engine Notification**: Combines native `QFileSystemWatcher` events with a heartbeat timer (configurable 250ms–5000ms, default 750ms). This ensures prompt detection on all OS filesystems, including those where writes bypass immediate filesystem events.
- **Atomic Delta Appending**: When a monitored file grows on disk, `TailMonitoringManager` seeks directly to `lastSize` and reads only the newly appended delta bytes. It then calls Scintilla's native `appendText(length, data)` method, appending the new lines in sub-millisecond time without reloading the existing document.
- **Log Rotation & Truncation Handling**: If the file size shrinks (`currentSize < lastSize`) or jumps beyond the configurable reload threshold (default 10 MB), the manager recognizes that the log was rotated or truncated and automatically reloads the entire document cleanly.
- **Read-Only Protection**: The document is locked in read-only mode during active monitoring to prevent accidental keystrokes from conflicting with incoming log streams.
- **Modal Dialog Bypass**: When an editor is under active tail monitoring, `MainWindow::checkFileForModification` skips the modal reload confirmation dialog, allowing log streams to flow uninterrupted.
- **Auto-scroll to End**: Automatically scrolls the viewport and caret to the end of the document (`editor->gotoLine(editor->lineCount())`, `editor->scrollCaret()`) on every update. Can be toggled off to allow inspecting historical log lines without jumping.

### 2.2 `TailOptionsDialog` (`src/dialogs/TailOptionsDialog.h`, `src/dialogs/TailOptionsDialog.cpp`, `src/dialogs/TailOptionsDialog.ui`)
Provides full user control over streaming and safety parameters:
- **Polling Heartbeat Interval**: 250 ms (Ultra High Speed), 500 ms (Fast), 750 ms (Standard / Recommended), 1000 ms, 2000 ms, 5000 ms.
- **Full Reload Threshold**: 1 MB, 5 MB, 10 MB, 25 MB (safeguard against huge file jumps).
- **Auto-scroll to End**: Checkbox (default checked).
- **Read-Only Lock**: Checkbox (default checked).

---

## 3. UI, Menus & Keyboard Shortcuts

- **Menu Path**: Under **View -> Monitoring (tail -f)**:
  - **Monitoring (tail -f)** (Checkable toggle): Starts or stops live tail monitoring on the active document.  
    **Shortcut: `Ctrl+Shift+Y`** (`Cmd+Shift+Y` on macOS).
  - **Auto-scroll to End on Update** (Checkable toggle): Enables or disables snapping to the bottom of the document upon new lines.
  - **Force Check / Reload Now**: Immediately forces a disk check and update.
  - **Tail Monitoring Options...**: Opens the configuration dialog.
- **Status Bar Integration**: Displays live streaming notifications:
  - `👁️ Monitoring (tail -f) started on server.log`
  - `👁️ Tail: +14 lines streamed (1024 bytes)`
  - `👁️ Tail: File rotated or truncated, reloaded (server.log)`

---

## 4. Invariant Compliance

- **Engine Invariant**: Strictly uses NoteSciPad's vendored `ScintillaNext + thirdparty/scintilla + thirdparty/lexilla`.
- **Zero Modal Blocking**: Log streaming never interrupts the user with modal popups.
- **Clean Architecture**: Implemented as a modular separate class (`TailMonitoringManager`), decoupled from core document models.
