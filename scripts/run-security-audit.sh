#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: run-security-audit.sh
# DESCRIPTION: Static security review scanner for SparkNotePad codebase
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "======================================================================"
echo "⚡ SparkNotePad Static Security & Threat Audit"
echo "======================================================================"

VULNS=0

# 1. Scan for dangerous process/shell execution in features/utils (excluding QLocale::system and QRandomGenerator::system)
echo "[1/4] Checking for dangerous process spawning (system, popen, execvp)..."
DANGEROUS_CALLS=$(grep -rnE "(\bstd::system\(|\bpopen\(|\bexecl\(|\bexecvp\()" "${REPO_ROOT}/src/features" "${REPO_ROOT}/src/utils" 2>/dev/null || true)
if [[ -n ${DANGEROUS_CALLS} ]]; then
	echo "[WARN] Found raw process execution calls:"
	echo "${DANGEROUS_CALLS}"
	VULNS=$((VULNS + 1))
else
	echo "[PASS] Zero dangerous raw process execution calls found in features/utils."
fi

# 2. Check JavaScriptCore Sandboxing
echo "[2/4] Verifying JavaScriptCore sandbox isolation..."
if grep -q "JSGlobalContextCreate" "${REPO_ROOT}/src/utils/JsEvaluator.cpp" && grep -q "JSGlobalContextRelease" "${REPO_ROOT}/src/utils/JsEvaluator.cpp"; then
	echo "[PASS] JsEvaluator properly creates and releases isolated JS contexts."
else
	echo "[FAIL] JsEvaluator missing proper context lifecycle!" >&2
	VULNS=$((VULNS + 1))
fi

# 3. Check HTML escaping in rich text dialogs (XSS prevention)
echo "[3/4] Checking for XSS escaping in release and about dialogs..."
if grep -q "toHtmlEscaped()" "${REPO_ROOT}/src/features/ReleaseManager.cpp"; then
	echo "[PASS] ReleaseManager uses toHtmlEscaped() on external release notes."
else
	echo "[WARN] ReleaseManager missing toHtmlEscaped()!" >&2
	VULNS=$((VULNS + 1))
fi

# 4. Verify Cryptographic Salt & Iterations
echo "[4/4] Verifying PBKDF2 iteration strength in CryptoManager..."
if grep -q "100000" "${REPO_ROOT}/src/utils/CryptoManager.h"; then
	echo "[PASS] PBKDF2 iteration count is set to 100,000 rounds."
else
	echo "[FAIL] Inadequate PBKDF2 iteration count!" >&2
	VULNS=$((VULNS + 1))
fi

echo "======================================================================"
if [[ ${VULNS} -eq 0 ]]; then
	echo "✅ SECURITY AUDIT PASSED: Zero vulnerabilities detected."
	exit 0
else
	echo "❌ SECURITY AUDIT FAILED with ${VULNS} issues." >&2
	exit 1
fi
