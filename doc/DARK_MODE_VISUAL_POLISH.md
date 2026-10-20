# Native Dark Mode Visual Polish & Theme Engine

## 1. Overview

SparkNotePad delivers a visual experience tailored to modern desktop operating systems (macOS Sonoma, Sequoia & modern Linux GNOME/KDE). Prior to this enhancement, docking tabs, title bars, and status bars were hardcoded with legacy light gray colors that clashed harshly when running in dark mode.

The new **ThemeManager** engine provides real-time, dynamic theme switching across the entire application interface and Scintilla editing canvas.

---

## 2. Key Features

### 2.1 Dynamic Mode Support

- **System Default (Auto):** Automatically monitors the OS appearance (`QStyleHints::colorSchemeChanged`) and adapts SparkNotePad's theme dynamically without requiring an application restart.
- **Dark Mode:** Forces a sleek dark palette across dock tabs, title bars, status bar, and code editor.
- **Light Mode:** Forces a clean, high-contrast modern light theme.
- **Quick Keyboard Toggle:** Press `Ctrl+Alt+T` (or `Cmd+Alt+T` on macOS) to instantly switch between dark and light modes.

### 2.2 UI & Component Styling (QSS)

| Component                | Dark Mode Palette                            | Light Mode Palette                          |
| ------------------------ | -------------------------------------------- | ------------------------------------------- |
| **Dock Tabs (Inactive)** | `#25282c` with subtle `#1e1e1e` borders      | `#e1e4e8` with `#d0d7de` borders            |
| **Active Document Tab**  | `#181a1f` with vibrant `#388bfd` accent line | `#ffffff` with `#0969da` accent line        |
| **Focused Document Tab** | `#181a1f` with vibrant `#58a6ff` accent line | `#ffffff` with `#218bff` accent line        |
| **Dock Title Bar**       | `#212429` with `#33373e` bottom border       | `#f6f8fa` with `#d0d7de` bottom border      |
| **Status Bar**           | `#1e1e1e` with `#33373e` top border          | `#f6f8fa` with `#d0d7de` top border         |
| **Quick Find Widget**    | `#212429` with `#388bfd` bottom accent line  | `#ffffff` with `#0969da` bottom accent line |
| **Tab Close Button**     | Transparent with `#eb5757` hover highlight   | Transparent with `#cf222e` hover highlight  |

### 2.3 Scintilla Editor Canvas Theming

| Scintilla Element              | Dark Mode Value        | Light Mode Value       |
| ------------------------------ | ---------------------- | ---------------------- |
| **Default Foreground**         | `#d4d4d4` (`0xD4D4D4`) | `#000000` (`0x000000`) |
| **Default Background**         | `#1e1e1e` (`0x1E1E1E`) | `#ffffff` (`0xFFFFFF`) |
| **Line Numbers Fore / Back**   | `#858585` / `#252526`  | `#808080` / `#e4e4e4`  |
| **Caret & Caret Line**         | `#ffffff` / `#2a2d2e`  | `#000000` / `#ffffe8`  |
| **Selection Background**       | `#3e4451`              | `#c0d0ff`              |
| **Fold Margin & Lines**        | `#1e1e1e` / `#505050`  | `#ffffff` / `#a0a0a0`  |
| **Brace Match (Light / Bad)**  | `#00d7ff` / `#5050ff`  | `#0000ff` / `#000080`  |
| **Indent Guides & Whitespace** | `#404040` / `#505050`  | `#c0c0c0` / `#d0d0d0`  |

---

## 3. Architecture

```
                 ┌──────────────────────────────────────────────────┐
                 │                OS System Appearance              │
                 │        (macOS / Linux Color Scheme Event)        │
                 └─────────────────────────┬────────────────────────┘
                                           │
                                           ▼
                 ┌──────────────────────────────────────────────────┐
                 │                   ThemeManager                   │
                 │  - ThemeMode: Auto, Light, Dark                  │
                 │  - isDarkActive()                                │
                 │  - applyTheme(window)                            │
                 │  - applyThemeToEditor(editor)                    │
                 │  - getThemeStyleSheet()                          │
                 └─────────────┬──────────────────────┬─────────────┘
                               │                      │
                               ▼                      ▼
                 ┌────────────────────────┐┌────────────────────────┐
                 │ MainWindow Stylesheet  ││ ScintillaNext Editors  │
                 │ (Tabs, Status, Docks)  ││ (Fore, Back, Margins)  │
                 └────────────────────────┘└────────────────────────┘
```

---

## 4. Verification

- Tested real-time stylesheet switching and Scintilla style invalidation via `styleClearAll()`.
- Verified persistent setting retention (`App/ThemeMode` in `ApplicationSettings`).
- Confirmed zero balance regression in C++ files and stylesheets.
