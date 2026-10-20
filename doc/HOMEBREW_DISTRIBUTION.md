# NoteSciPad Homebrew Distribution Guide

## 1. Overview

Homebrew is the primary package manager for macOS power users and developers. NoteSciPad is distributed via an official Homebrew tap repository: `alsyundawy/homebrew-notescipad`.

---

## 2. Installation for End-Users

Users can install NoteSciPad with a single command:

```bash
# Tap the official NoteSciPad tap repository:
brew tap alsyundawy/notescipad

# Install NoteSciPad:
brew install --cask notescipad

# Launch from terminal:
notescipad
```

To update NoteSciPad via Homebrew:

```bash
brew upgrade --cask notescipad
```

---

## 3. Cask Structure & Artifacts

The Cask recipe (`deploy/homebrew/Casks/notescipad.rb`) features:

- Dual-architecture detection (`arch arm: "arm64", intel: "x86_64"`).
- Universal CLI binary symlink: `notescipad` linked directly to `/usr/local/bin` or `/opt/homebrew/bin`.
- Clean cleanup definitions in `zap trash` removing application caches and settings upon uninstallation.

---

## 4. Maintainer Release Automation

To bump the Cask after cutting a new GitHub Release:

```bash
./scripts/update-homebrew-cask.sh <VERSION> <PATH_TO_ARM64_DMG> <PATH_TO_INTEL_DMG>
```

This updates the Cask version, recalculates SHA256 checksums, and coordinates with the post-release notification workflow.
