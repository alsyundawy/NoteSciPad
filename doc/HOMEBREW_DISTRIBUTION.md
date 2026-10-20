# SparkNotePad Homebrew Distribution Guide

## 1. Overview

Homebrew is the primary package manager for macOS power users and developers. SparkNotePad is distributed via an official Homebrew tap repository: `alsyundawy/homebrew-sparknotepad`.

---

## 2. Installation for End-Users

Users can install SparkNotePad with a single command:

```bash
# Tap the official SparkNotePad tap repository:
brew tap alsyundawy/sparknotepad

# Install SparkNotePad:
brew install --cask sparknotepad

# Launch from terminal:
sparknotepad
```

To update SparkNotePad via Homebrew:

```bash
brew upgrade --cask sparknotepad
```

---

## 3. Cask Structure & Artifacts

The Cask recipe (`deploy/homebrew/Casks/sparknotepad.rb`) features:

- Dual-architecture detection (`arch arm: "arm64", intel: "x86_64"`).
- Universal CLI binary symlink: `sparknotepad` linked directly to `/usr/local/bin` or `/opt/homebrew/bin`.
- Clean cleanup definitions in `zap trash` removing application caches and settings upon uninstallation.

---

## 4. Maintainer Release Automation

To bump the Cask after cutting a new GitHub Release:

```bash
./scripts/update-homebrew-cask.sh <VERSION> <PATH_TO_ARM64_DMG> <PATH_TO_INTEL_DMG>
```

This updates the Cask version, recalculates SHA256 checksums, and coordinates with the post-release notification workflow.
