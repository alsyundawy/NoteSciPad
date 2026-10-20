#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: NotepadNext-Default-Editor.sh (Compatibility Wrapper)
# NOTE:        Forwarding directly to NoteSciPad-Default-Editor.sh
# ==============================================================================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_SCRIPT="${SCRIPT_DIR}/NoteSciPad-Default-Editor.sh"

if [[ -f "${TARGET_SCRIPT}" ]]; then
	exec "${TARGET_SCRIPT}" "$@"
else
	echo "[ERR] Target script ${TARGET_SCRIPT} not found!" >&2
	exit 1
fi
