# Multi-Monitor & Retina High-DPI Adaptation Engine

## 1. Overview

Modern desktop setups frequently utilize heterogeneous display topologies:

- High-resolution Apple Liquid Retina XDR displays (2.0x Device Pixel Ratio) paired with external 4K monitors (1.5x fractional or 1.0x native scaling) or 1080p projectors.
- Hot-plugging and unplugging external monitors while SparkNotePad is running or asleep.
- Clamshell mode transitions (closing the laptop lid to switch primary output to an external screen).

SparkNotePad features a dedicated **ScreenDisplayManager** and **DisplayInfoDialog** designed to ensure visual crispness, seamless screen transitions, font subpixel rendering fidelity, and boundary safety.

---

## 2. Core Capabilities

### 2.1 Fractional Scaling & PassThrough Policy

In `src/main.cpp`:

```cpp
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
```

By setting `PassThrough`, Qt avoids integer rounding (snapping 1.25x or 1.5x up to 2.0x or down to 1.0x). Text glyphs rendered by Scintilla remain crisp, and caret line bounds match physical pixel boundaries precisely without blurry artifacts.

### 2.2 Dynamic Retina DPR Handshake with Scintilla

When a window transitions between displays with different pixel densities (e.g., from an Apple Retina display at 2.0x DPR to a 1080p display at 1.0x DPR):

1. `QWindow::screenChanged` is captured by `ScreenDisplayManager`.
2. `devicePixelRatioChanged(oldDpr, newDpr)` is emitted.
3. All active `ScintillaNext` editor canvases are invalidated and updated (`editor->update()`), ensuring Scintilla surface metrics and font glyph rasters are recalculated for the new DPI context.

### 2.3 Off-Screen Boundary Safety & Recovery

When an external monitor is unplugged, windows previously located on that screen can become stranded in "ghost coordinates" beyond active display viewports.

- SparkNotePad's `ScreenDisplayManager::ensureWindowOnScreen(QWidget *window)` calculates the intersection between the window's geometry and the available geometry of all connected screens.
- If fewer than 150x150 pixels of the window intersect any active display, the window is automatically restored and centered on `QGuiApplication::primaryScreen()`.
- This verification runs automatically both on startup (`MainWindow::restoreWindowState()`) and whenever a monitor is disconnected (`QGuiApplication::screenRemoved`).

### 2.4 Multi-Monitor Navigation & Shortcuts

| Action                      | Menu Location                                                                           | Keyboard Shortcut                  | Function                                                           |
| --------------------------- | --------------------------------------------------------------------------------------- | ---------------------------------- | ------------------------------------------------------------------ |
| **Move to Next Screen**     | `Window > Move to Next Screen`                                                          | `Ctrl+Alt+Right` / `Cmd+Alt+Right` | Moves window to the next connected display preserving optimal size |
| **Move to Previous Screen** | `Window > Move to Previous Screen`                                                      | `Ctrl+Alt+Left` / `Cmd+Alt+Left`   | Moves window to previous display                                   |
| **Center Window on Screen** | `Window > Center Window on Screen`                                                      | `Ctrl+Alt+C` / `Cmd+Alt+C`         | Centers window within active screen available geometry             |
| **Display Diagnostics**     | `Window > Display & Retina Diagnostics...`<br/>`View > Display & Retina Diagnostics...` | `Ctrl+Alt+D` / `Cmd+Alt+D`         | Opens the interactive display inspector and controller             |

---

## 3. Architecture

```text
                      ┌──────────────────────────────────────────┐
                      │          QGuiApplication Screens         │
                      │  - screenAdded / screenRemoved           │
                      │  - primaryScreenChanged                  │
                      └────────────────────┬─────────────────────┘
                                           │
                                           ▼
                      ┌──────────────────────────────────────────┐
                      │           ScreenDisplayManager           │
                      │  - ensureWindowOnScreen(window)          │
                      │  - moveToNextScreen(window)              │
                      │  - moveToPreviousScreen(window)          │
                      │  - centerOnCurrentScreen(window)         │
                      │  - getConnectedDisplaysInfo()            │
                      └──────────────┬───────────────────────────┘
                                     │
           ┌─────────────────────────┼─────────────────────────┐
           ▼                         ▼                         ▼
┌─────────────────────┐   ┌─────────────────────┐   ┌─────────────────────┐
│  DisplayInfoDialog  │   │  Window Reposition  │   │ Scintilla Editors   │
│  (Diagnostics UI)   │   │  (Boundary Guard)   │   │ (Glyph Redraw/DPR)  │
└─────────────────────┘   └─────────────────────┘   └─────────────────────┘
```

---

## 4. Verification

- Tested DPR calculations and display metadata extraction on single and multi-screen environments.
- Confirmed off-screen safety clamping when disconnected screens are simulated.
- Validated keyboard shortcut invocation (`Ctrl+Alt+Right`, `Ctrl+Alt+Left`, `Ctrl+Alt+C`, `Ctrl+Alt+D`).
- Verified zero memory leaks and balanced Qt event handling.
