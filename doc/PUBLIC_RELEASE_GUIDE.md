# SparkNotePad Public Release & Update Pipeline Guide

## 1. Overview

SparkNotePad's public release pipeline provides automated builds, codesigning, packaging, checksum verification, and in-app update checks across macOS and Linux.

---

## 2. Release Management Tools

### 2.1 Automated Release Preparation

The maintainer utility `scripts/prepare-release.sh` coordinates end-to-end packaging:

```bash
# Prepare release for a specific version tag:
./scripts/prepare-release.sh v1.0.0-beta1

# Dry-run validation:
./scripts/prepare-release.sh v1.0.0-beta1 --dry-run
```

Outputs produced:

- `build-release-assets/SparkNotePad-*.dmg` (macOS Apple Silicon & Intel)
- `build-release-assets/SparkNotePad_*.deb` (Debian / Ubuntu)
- `build-release-assets/SparkNotePad-*.rpm` (Fedora / RHEL)
- `build-release-assets/SHA256SUMS` (Cryptographic verification digests)
- `RELEASE_NOTES.md` (Categorized release notes formatted for GitHub Releases)

### 2.2 Native In-App Update Engine (ReleaseManager)

Unlike legacy Windows-only updater implementations, `ReleaseManager`:

- Directly checks the official GitHub Releases API (`https://api.github.com/repos/alsyundawy/SparkNotePad/releases/latest`) using native `QNetworkAccessManager`.
- Compares semantic versions (handling major, minor, patch, and pre-release `-alpha`/`-beta` suffixes).
- Prompts the user with release notes and a direct action button to open the release download page.
- Available via `Help > Check for Updates...` (`Ctrl+Shift+U` / `Cmd+Shift+U`).

---

## 3. GitHub Actions CI/CD Release Flow

When a new version tag is pushed (e.g., `git push origin v1.0.0-beta1`):

1. `.github/workflows/release.yml` triggers.
2. Build matrix compiles SparkNotePad on macOS (arm64 + x64) and Linux.
3. macOS binaries are signed with Hardened Runtime entitlements and submitted to Apple for notarization.
4. Linux packages (.deb and .rpm) are generated via CPack.
5. All binaries and `SHA256SUMS` are attached to the draft GitHub Release.
