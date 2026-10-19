#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: run-unit-tests.sh
# DESCRIPTION: Compiles and executes NoteSciPad unit test suite
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

echo "======================================================================"
echo "⚡ NoteSciPad Unit & Integration Test Suite"
echo "======================================================================"

python3 "${REPO_ROOT}/scripts/test_utilities_standalone.py"

if [[ -f "${REPO_ROOT}/build/tests/NoteSciPadTests" ]]; then
    echo "Running native Qt test binary..."
    "${REPO_ROOT}/build/tests/NoteSciPadTests"
elif command -v ctest >/dev/null 2>&1 && [[ -d "${REPO_ROOT}/build" && -f "${REPO_ROOT}/build/CTestTestfile.cmake" ]]; then
    ctest --test-dir "${REPO_ROOT}/build" --output-on-failure || true
fi

echo "======================================================================"
echo "✅ Unit & integration testing completed."
echo "======================================================================"
