#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: codesign-and-notarize-macos.sh
# DESCRIPTION: Production-grade automated codesigning and notarization workflow
#              for NoteSciPad on macOS (Developer ID Application & Apple Notary).
# AUTHOR:      Harry DS Alsyundawy (NoteSciPad Maintainer)
# VERSION:     1.0.0
# LICENSE:     GPL-3.0-or-later / MIT
# ==============================================================================

set -Eeuo pipefail
IFS=$'\n\t'

export PATH="/opt/homebrew/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:${PATH:-}"

APP_BUNDLE=""
DMG_PATH=""
ENTITLEMENTS_PATH="deploy/macos/NoteSciPad.entitlements"
SIGNING_IDENTITY="${DEVELOPER_ID_APPLICATION:-}"
AD_HOC=0
DO_NOTARIZE=1
KEYCHAIN_NAME="notescipad_signing.keychain"
KEYCHAIN_PASSWORD="temporary_keychain_password_123"

if [[ -t 1 ]] && [[ -z ${NO_COLOR:-} ]]; then
	GREEN='\033[0;32m'
	BLUE='\033[0;34m'
	YELLOW='\033[1;33m'
	RED='\033[0;31m'
	BOLD='\033[1m'
	NC='\033[0m'
else
	GREEN=''
	BLUE=''
	YELLOW=''
	RED=''
	BOLD=''
	NC=''
fi

info() { printf "%b[INFO]%b %s\n" "${BLUE}" "${NC}" "$*"; }
step() { printf "%b[STEP]%b %s\n" "${BOLD}" "${NC}" "$*"; }
ok() { printf "%b[ OK ]%b %s\n" "${GREEN}" "${NC}" "$*"; }
warn() { printf "%b[WARN]%b %s\n" "${YELLOW}" "${NC}" "$*" >&2; }
err() { printf "%b[ERR ]%b %s\n" "${RED}" "${NC}" "$*" >&2; }

cleanup() {
	if security list-keychains 2>/dev/null | grep -q "${KEYCHAIN_NAME}"; then
		info "Cleaning up temporary keychain..."
		security default-keychain -s login.keychain 2>/dev/null || true
		security delete-keychain "${KEYCHAIN_NAME}" 2>/dev/null || true
	fi
}
trap cleanup EXIT INT TERM HUP

show_help() {
	cat <<EOF
NoteSciPad macOS Codesign & Notarization Automation Tool

Usage: $(basename "$0") [OPTIONS]

Options:
  --app <path>             Path to NoteSciPad.app bundle
  --dmg <path>             Path to NoteSciPad .dmg file
  --entitlements <path>    Path to entitlements.plist (default: deploy/macos/NoteSciPad.entitlements)
  --identity <name>        Developer ID Application identity string
  --ad-hoc                 Sign with ad-hoc signature (-) without certificates
  --no-notarize            Skip Apple notarization step
  -h, --help               Display this help message

Environment Variables:
  APPLE_CERTIFICATE_BASE64       Base64-encoded Developer ID Application .p12 certificate
  APPLE_CERTIFICATE_PASSWORD     Password for .p12 certificate
  APPLE_ID                       Apple Developer ID (email)
  APPLE_APP_SPECIFIC_PASSWORD    Apple App-Specific Password for notarytool
  APPLE_TEAM_ID                  10-character Apple Developer Team ID
EOF
}

while [[ $# -gt 0 ]]; do
	case "$1" in
	--app)
		APP_BUNDLE="$2"
		shift 2
		;;
	--dmg)
		DMG_PATH="$2"
		shift 2
		;;
	--entitlements)
		ENTITLEMENTS_PATH="$2"
		shift 2
		;;
	--identity)
		SIGNING_IDENTITY="$2"
		shift 2
		;;
	--ad-hoc)
		AD_HOC=1
		shift
		;;
	--no-notarize)
		DO_NOTARIZE=0
		shift
		;;
	-h | --help)
		show_help
		exit 0
		;;
	*)
		err "Unknown option: $1"
		show_help
		exit 1
		;;
	esac
done

if [[ -n ${DMG_PATH} ]] && [[ ${DMG_PATH} == *"*"* ]]; then
	expanded=$(find "$(dirname "${DMG_PATH}")" -maxdepth 1 -name "$(basename "${DMG_PATH}")" | head -n 1 || true)
	if [[ -n ${expanded} ]] && [[ -f ${expanded} ]]; then
		DMG_PATH="${expanded}"
	fi
fi

if [[ -n ${APPLE_CERTIFICATE_BASE64:-} ]] && [[ -n ${APPLE_CERTIFICATE_PASSWORD:-} ]]; then
	step "Importing Developer ID Certificate into temporary keychain..."
	security create-keychain -p "${KEYCHAIN_PASSWORD}" "${KEYCHAIN_NAME}"
	security set-keychain-settings -lut 21600 "${KEYCHAIN_NAME}"
	security unlock-keychain -p "${KEYCHAIN_PASSWORD}" "${KEYCHAIN_NAME}"

	P12_TEMP=$(mktemp -t cert_XXXXXX.p12)
	echo "${APPLE_CERTIFICATE_BASE64}" | base64 --decode >"${P12_TEMP}"

	security import "${P12_TEMP}" \
		-k "${KEYCHAIN_NAME}" \
		-P "${APPLE_CERTIFICATE_PASSWORD}" \
		-T /usr/bin/codesign \
		-T /usr/bin/security
	rm -f "${P12_TEMP}"

	security set-key-partition-list \
		-S apple-tool:,apple:,codesign: \
		-s -k "${KEYCHAIN_PASSWORD}" "${KEYCHAIN_NAME}" &>/dev/null || true

	if [[ -z ${SIGNING_IDENTITY} ]]; then
		SIGNING_IDENTITY=$(security find-identity -v -p codesigning "${KEYCHAIN_NAME}" | grep -o 'Developer ID Application: [^"]*' | head -n 1 || true)
		if [[ -n ${SIGNING_IDENTITY} ]]; then
			ok "Found signing identity: ${SIGNING_IDENTITY}"
		fi
	fi
fi

if [[ ${AD_HOC} -eq 1 ]] || [[ -z ${SIGNING_IDENTITY} ]]; then
	if [[ ${AD_HOC} -eq 0 ]]; then
		warn "No Developer ID signing identity provided or found. Falling back to ad-hoc signing (-)."
	fi
	SIGNING_IDENTITY="-"
	DO_NOTARIZE=0
fi

if [[ -n ${APP_BUNDLE} ]] && [[ -d ${APP_BUNDLE} ]]; then
	step "Deep-signing application bundle: ${APP_BUNDLE}..."

	find "${APP_BUNDLE}/Contents" \( -name "*.dylib" -o -name "*.so" -o -type f -perm +111 \) -print0 | while IFS= read -r -d '' binary; do
		if file "${binary}" | grep -q "Mach-O"; then
			codesign --force --verify --verbose=1 \
				--options runtime \
				--timestamp \
				--sign "${SIGNING_IDENTITY}" \
				"${binary}" 2>/dev/null || true
		fi
	done

	if [[ -d "${APP_BUNDLE}/Contents/Frameworks" ]]; then
		find "${APP_BUNDLE}/Contents/Frameworks" -maxdepth 2 -name "*.framework" -print0 | while IFS= read -r -d '' fw; do
			codesign --force --verify --verbose=1 \
				--options runtime \
				--timestamp \
				--sign "${SIGNING_IDENTITY}" \
				"${fw}" 2>/dev/null || true
		done
	fi

	ENT_ARGS=()
	if [[ -f ${ENTITLEMENTS_PATH} ]]; then
		ENT_ARGS=(--entitlements "${ENTITLEMENTS_PATH}")
	fi

	codesign --force --verify --verbose \
		--options runtime \
		--timestamp \
		"${ENT_ARGS[@]}" \
		--sign "${SIGNING_IDENTITY}" \
		"${APP_BUNDLE}"

	ok "App bundle signed successfully."
	codesign --verify --deep --strict --verbose=2 "${APP_BUNDLE}"
fi

if [[ -n ${DMG_PATH} ]] && [[ -f ${DMG_PATH} ]]; then
	step "Signing disk image: ${DMG_PATH}..."
	codesign --force --verify --verbose \
		--sign "${SIGNING_IDENTITY}" \
		--timestamp \
		"${DMG_PATH}"
	ok "DMG signed successfully."
fi

if [[ ${DO_NOTARIZE} -eq 1 ]] && [[ -n ${DMG_PATH} ]] && [[ -f ${DMG_PATH} ]]; then
	if [[ -n ${APPLE_ID:-} ]] && [[ -n ${APPLE_APP_SPECIFIC_PASSWORD:-} ]] && [[ -n ${APPLE_TEAM_ID:-} ]]; then
		step "Submitting DMG to Apple Notary Service via xcrun notarytool..."
		xcrun notarytool submit "${DMG_PATH}" \
			--apple-id "${APPLE_ID}" \
			--password "${APPLE_APP_SPECIFIC_PASSWORD}" \
			--team-id "${APPLE_TEAM_ID}" \
			--wait

		step "Stapling notarization ticket to DMG..."
		xcrun stapler staple "${DMG_PATH}"
		ok "Notarization ticket stapled successfully."

		step "Validating Gatekeeper acceptance via spctl..."
		spctl --assess --type open --context context:primary-signature --verbose "${DMG_PATH}" || true
		ok "Gatekeeper assessment verified."
	else
		warn "Skipping notarization: APPLE_ID, APPLE_APP_SPECIFIC_PASSWORD, or APPLE_TEAM_ID not set."
	fi
fi

ok "Codesigning and packaging workflow completed successfully."
