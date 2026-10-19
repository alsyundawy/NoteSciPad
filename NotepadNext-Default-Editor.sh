#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: NotepadNext-Default-Editor.sh (Compatibility Wrapper)
# NOTE:        Forwarding directly to scripts/NoteSciPad-Default-Editor.sh
#              Default bundle identifier: io.github.alsyundawy.NoteSciPad
# ==============================================================================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
TARGET_SCRIPT="${SCRIPT_DIR}/scripts/NoteSciPad-Default-Editor.sh"

if [[ -f "${TARGET_SCRIPT}" ]]; then
    exec "${TARGET_SCRIPT}" "$@"
else
    echo "[ERR] Target script ${TARGET_SCRIPT} not found!" >&2
    exit 1
fi
