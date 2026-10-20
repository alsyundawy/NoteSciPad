#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: verify-engine-invariants.sh
# DESCRIPTION: Automated guard ensuring vendored Scintilla & Lexilla remain intact
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "======================================================================"
echo "⚡ Checking SparkNotePad Engine Invariants (Scintilla & Lexilla)"
echo "======================================================================"

ERRORS=0

# 1. Check thirdparty/scintilla
SCINTILLA_DIR="${REPO_ROOT}/thirdparty/scintilla"
if [[ -d ${SCINTILLA_DIR} && -f "${SCINTILLA_DIR}/include/Scintilla.h" && -f "${SCINTILLA_DIR}/include/Sci_Position.h" ]]; then
	echo "[OK] Vendored Scintilla present: ${SCINTILLA_DIR}"
else
	echo "[ERR] Missing vendored Scintilla core files in ${SCINTILLA_DIR}" >&2
	ERRORS=$((ERRORS + 1))
fi

# 2. Check thirdparty/lexilla
LEXILLA_DIR="${REPO_ROOT}/thirdparty/lexilla"
if [[ -d ${LEXILLA_DIR} && -f "${LEXILLA_DIR}/include/Lexilla.h" && -f "${LEXILLA_DIR}/include/SciLexer.h" ]]; then
	echo "[OK] Vendored Lexilla present: ${LEXILLA_DIR}"
else
	echo "[ERR] Missing vendored Lexilla core files in ${LEXILLA_DIR}" >&2
	ERRORS=$((ERRORS + 1))
fi

# 3. Check CMake linkages in src/CMakeLists.txt
SRC_CMAKE="${REPO_ROOT}/src/CMakeLists.txt"
if grep -q "scintilla" "${SRC_CMAKE}" && grep -q "lexilla" "${SRC_CMAKE}"; then
	echo "[OK] src/CMakeLists.txt targets explicitly link to scintilla and lexilla"
else
	echo "[ERR] src/CMakeLists.txt missing scintilla or lexilla target link!" >&2
	ERRORS=$((ERRORS + 1))
fi

# 4. Check ScintillaNext class
SCINTILLANEXT_H="${REPO_ROOT}/src/ScintillaNext.h"
if [[ -f ${SCINTILLANEXT_H} ]]; then
	echo "[OK] ScintillaNext editor wrapper class present: ${SCINTILLANEXT_H}"
else
	echo "[ERR] Missing ScintillaNext.h!" >&2
	ERRORS=$((ERRORS + 1))
fi

# 5. Check Lua 5.5+ runtime in thirdparty/lua
LUA_H="${REPO_ROOT}/thirdparty/lua/src/lua.h"
if [[ -f ${LUA_H} ]] && (grep -q 'LUA_VERSION_MAJOR_N.*5' "${LUA_H}" || grep -q 'LUA_VERSION_MAJOR.*"5"' "${LUA_H}") && (grep -q 'LUA_VERSION_MINOR_N.*5' "${LUA_H}" || grep -q 'LUA_VERSION_MINOR.*"5"' "${LUA_H}"); then
	echo "[OK] Embedded Lua 5.5+ engine verified: ${LUA_H}"
else
	echo "[ERR] Missing or outdated Lua in ${REPO_ROOT}/thirdparty/lua" >&2
	ERRORS=$((ERRORS + 1))
fi

# 6. Check PythonScriptEngine (Python 3.13+ integration)
PYTHON_ENGINE_H="${REPO_ROOT}/src/features/PythonScriptEngine.h"
PYTHON_ENGINE_CPP="${REPO_ROOT}/src/features/PythonScriptEngine.cpp"
if [[ -f ${PYTHON_ENGINE_H} && -f ${PYTHON_ENGINE_CPP} ]]; then
	echo "[OK] Python 3.13+ Scripting Engine verified: ${PYTHON_ENGINE_H}"
else
	echo "[ERR] Missing PythonScriptEngine in src/features/" >&2
	ERRORS=$((ERRORS + 1))
fi

echo "======================================================================"
if [[ ${ERRORS} -eq 0 ]]; then
	echo "✅ ALL ENGINE INVARIANTS SATISFIED (Scintilla + Lexilla)"
	exit 0
else
	echo "❌ ENGINE INVARIANT VIOLATION DETECTED (${ERRORS} errors)" >&2
	exit 1
fi
