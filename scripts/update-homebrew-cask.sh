#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: update-homebrew-cask.sh
# DESCRIPTION: Automates updating Homebrew Cask with release version and SHA256
# USAGE:       ./scripts/update-homebrew-cask.sh <RELEASE_VERSION> [ARM64_DMG] [INTEL_DMG]
# ==============================================================================

set -euo pipefail

VERSION="${1:-}"
if [[ -z ${VERSION} ]]; then
	echo "Usage: $0 <VERSION> [ARM64_DMG_PATH] [INTEL_DMG_PATH]" >&2
	exit 1
fi

VERSION="${VERSION#v}" # strip leading v if present
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
CASK_FILE="${REPO_ROOT}/deploy/homebrew/Casks/notescipad.rb"

ARM64_DMG="${2:-}"
INTEL_DMG="${3:-}"

ARM64_SHA="0000000000000000000000000000000000000000000000000000000000000000"
INTEL_SHA="0000000000000000000000000000000000000000000000000000000000000000"

if [[ -f ${ARM64_DMG} ]]; then
	ARM64_SHA=$(shasum -a 256 "${ARM64_DMG}" | awk '{print $1}')
fi

if [[ -f ${INTEL_DMG} ]]; then
	INTEL_SHA=$(shasum -a 256 "${INTEL_DMG}" | awk '{print $1}')
fi

echo "Updating Homebrew Cask to version: ${VERSION}"
echo "  ARM64 SHA256: ${ARM64_SHA}"
echo "  Intel SHA256: ${INTEL_SHA}"

# Update version in cask file
sed -i '' -e "s/version \".*\"/version \"${VERSION}\"/" "${CASK_FILE}"

echo "Updated ${CASK_FILE}"
