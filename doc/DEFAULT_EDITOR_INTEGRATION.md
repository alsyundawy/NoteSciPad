# Default Editor Integration Guide (macOS & Linux)

## 1. Overview

NoteSciPad includes an automated, production-grade default editor configuration tool:
`scripts/NoteSciPad-Default-Editor.sh`.

It deterministically binds NoteSciPad as the default system handler for:
- **macOS:** 38 UTIs (including `public.data` for extensionless files) and 470+ file extensions across 15 categories.
- **Linux:** 26+ text and programming MIME types via `xdg-mime`, with optional `/usr/bin/editor` alternative registration.

---

## 2. macOS Architecture & LaunchServices Takeover

### 2.1 The Problem
macOS defaults to TextEdit for plain text, source code, and extensionless files (`public.data`). Furthermore:
1. Web downloads or home-compiled apps often carry the `com.apple.quarantine` extended attribute.
2. LaunchServices caches associations in daemon memory; stale caches prevent Finder from picking up newly registered handlers.
3. Blindly calling `lsregister -kill -domain system` on macOS 13+ (Ventura, Sonoma, Sequoia) corrupts ExtensionKit and System Settings preference panes.

### 2.2 The NoteSciPad Solution
`NoteSciPad-Default-Editor.sh` solves these issues with surgical precision:
1. **Gatekeeper Unquarantine:** Recursively strips `com.apple.quarantine` via `xattr -dr com.apple.quarantine`.
2. **ExtensionKit Safe Reset:** Restricts `lsregister -kill` strictly to `-domain user`, then immediately re-seeds system domains and re-registers `/System/Library/ExtensionKit/Extensions` and `System Settings.app`.
3. **Atomic High-Performance Batch Mode:** Compiles all 38 UTIs and 470+ extensions into a single temporary batch file (`mktemp -t notescipad_duti_XXXXXX` with `chmod 600`) and executes `duti` once (<0.3s runtime).
4. **Desktop Daemon Refresh:** Gracefully restarts Finder and Dock so changes take effect immediately without requiring a reboot.

---

## 3. Linux Integration (FreeDesktop)

On Linux systems:
1. Uses `xdg-mime default NoteSciPad.desktop <mimetype>` across all standard text, code, markup, and config MIME types.
2. When executed with superuser privileges (`sudo`), configures Debian/Ubuntu `update-alternatives --install /usr/bin/editor editor ...` for seamless CLI workflow (`git commit`, `crontab -e`).

---

## 4. Usage & CLI Reference

### 4.1 Audit Associations
Inspect current default editor associations for key file types:
```bash
./scripts/NoteSciPad-Default-Editor.sh --status
# or
./scripts/NoteSciPad-Default-Editor.sh -s
```

### 4.2 Force Set NoteSciPad as Default
Interactively configure with confirmation:
```bash
./scripts/NoteSciPad-Default-Editor.sh
```

Non-interactive force mode (CI/CD or automated setups):
```bash
./scripts/NoteSciPad-Default-Editor.sh -f
```

### 4.3 Dry-Run Simulation
Preview changes without modifying system settings:
```bash
./scripts/NoteSciPad-Default-Editor.sh -d
```

### 4.4 Custom App Bundle Path
Specify a custom build or installation directory:
```bash
./scripts/NoteSciPad-Default-Editor.sh --app-path /Applications/NoteSciPad.app -f
```

### 4.5 Restore System Defaults
Restore basic plain-text associations to Apple TextEdit:
```bash
./scripts/NoteSciPad-Default-Editor.sh --restore
```
