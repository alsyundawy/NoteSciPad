# Accessibility & VoiceOver Integration in NoteSciPad

## 1. Overview

NoteSciPad is designed to be fully accessible to developers, writers, and power users with visual, motor, or auditory impairments. It conforms to:

- **WCAG 2.2 Level AAA** for non-web software applications.
- **Apple macOS Accessibility Standards** (VoiceOver, Full Keyboard Access, Character Palette).
- **FreeDesktop AT-SPI** for Linux screen readers (Orca).

---

## 2. Core Accessibility Pillars

### 2.1 Complete Keyboard Operability

Every feature, dialog, menu item, and panel is operable without requiring a mouse or trackpad:

- **Global Editor Focus:** Press `Ctrl+Alt+E` (or `Escape` from search/dock panels) to immediately return focus to the ScintillaNext canvas.
- **Tab & Document Navigation:** Cycle tabs smoothly with `Ctrl+Tab` (Next Document) and `Ctrl+Shift+Tab` (Previous Document).
- **Numbered Document Access:** In the **Window** menu, the first 9 open tabs are accessible via mnemonics (`&1` through `&9`).
- **Interactive Shortcut Guide:** Press `Shift+F1` (macOS) or `F1` (Linux) to open the interactive **Keyboard Shortcuts & Accessibility Guide** dialog with real-time text filtering.

### 2.2 Apple VoiceOver & Linux Orca Integration

Implemented via `AccessibilityManager` and Cocoa `MacServicesHandler`:

- **Document State Announcements:**
  - When switching tabs, NoteSciPad posts an accessibility notification via `NSAccessibilityAnnouncementRequestedNotification` (macOS) and `QAccessible::Alert` (Linux).
  - VoiceOver announces the tab position, document name, and unsaved status (e.g., _"Document 2 of 5: main.cpp (unsaved)"_).
- **Editor Canvas Context:**
  - `ScintillaNext` is configured with accessible roles and names (`NoteSciPad Code Editor`).
  - Screen readers can inspect total line counts, cursor position (line and column), and selection ranges.
- **Native Emoji & Character Palette:**
  - Located at **Edit > Emoji & Symbols** (`Ctrl+Cmd+Space`), activating Apple's native Cocoa character palette right at the cursor position.

### 2.3 Visual Ergonomics & Low-Vision Features

- **Fluid Font Zooming:** `Ctrl + Plus` (Zoom In), `Ctrl + Minus` (Zoom Out), `Ctrl + 0` (Reset Zoom).
- **Focused View Mode:** `F9` dims background lines and focuses visual attention exclusively on the active line.
- **High-Contrast Theme Compatibility:** Uses system-tailored dark and light palette tokens with strong contrast ratios.

---

## 3. Architecture & Implementation

```text
               ┌────────────────────────────────────────────────────────┐
               │                  MainWindow & Actions                  │
               │  - Help > Keyboard Shortcuts & Accessibility (Shift+F1)│
               │  - Window > Focus Editor (Ctrl+Alt+E)                  │
               │  - Live Status Announcements on Tab Change             │
               └───────────────────────────┬────────────────────────────┘
                                           │
                                           ▼
               ┌────────────────────────────────────────────────────────┐
               │                  AccessibilityManager                  │
               │  - setupAccessibility(window)                          │
               │  - announce(context, message)                          │
               │  - announceDocumentState(editor, index, total)         │
               │  - openAccessibilityHelp(parent)                       │
               └───────────────┬────────────────────────┬───────────────┘
                               │                        │
                      (macOS)  │                        │  (Linux / Cross-Platform)
                               ▼                        ▼
               ┌────────────────────────┐      ┌────────────────────────┐
               │  MacServicesHandler.mm │      │  QAccessible::Alert    │
               │  - NSAccessibility-    │      │  - AT-SPI bus event    │
               │    Announcement-       │      │  - Orca speech dispatch│
               │    Notification        │      │                        │
               └────────────────────────┘      └────────────────────────┘
```

---

## 4. Testing & Verification

- Verified `QAccessible::Alert` dispatch without crashes or performance penalties.
- Verified keyboard shortcut table indexing and real-time filtering in `AccessibilityHelpDialog`.
- Verified Apple VoiceOver accessibility notifications via native Cocoa `NSAccessibilityPostNotificationWithUserInfo`.
