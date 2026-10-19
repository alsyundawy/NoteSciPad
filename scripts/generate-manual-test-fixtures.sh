#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: generate-manual-test-fixtures.sh
# DESCRIPTION: Generates test files for manual QA (Encodings, CJK, Large Files)
# USAGE:       ./scripts/generate-manual-test-fixtures.sh [OUTPUT_DIR]
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"
TARGET_DIR="${1:-${REPO_ROOT}/build-test-fixtures}"

mkdir -p "${TARGET_DIR}"
echo "Generating manual QA test fixtures in: ${TARGET_DIR}"

# 1. Plain UTF-8
cat << 'EOF' > "${TARGET_DIR}/test_utf8.txt"
NoteSciPad UTF-8 Test Document
ASCII: Hello World 1234567890
Latin-1 Supplement: Café, naïve, façade, über, señor
Mathematics: ∀x ∈ ℝ, e^(iπ) + 1 = 0, ∑, ∫, √
Emojis: ⚡ 🚀 💻 🛡️ 🎨
EOF

# 2. CJK Multilingual Text
cat << 'EOF' > "${TARGET_DIR}/test_cjk.txt"
=== NoteSciPad CJK Multi-Language Test ===
Simplified Chinese: 欢迎使用 NoteSciPad 高性能文本编辑器！
Traditional Chinese: 歡迎使用 NoteSciPad 高性能文字編輯器！
Japanese: NoteSciPad へようこそ！高速で軽量なエディタです。
Korean: NoteSciPad에 오신 것을 환영합니다! 가볍고 빠른 에디터입니다.
Mixed CJK & English: NoteSciPad 支持 100+ 种编程语言的语法高亮。
EOF

# 3. UTF-8 with BOM
printf 'ï»¿NoteSciPad UTF-8 with BOM Test
This file starts with a byte-order mark (0xEF, 0xBB, 0xBF).
' > "${TARGET_DIR}/test_utf8_bom.txt"

# 4. Large 10 MB Test File
echo "Generating 10 MB synthetic code file..."
python3 -c "
with open('${TARGET_DIR}/test_10mb.log', 'w') as f:
    for i in range(150000):
        f.write(f'2026-10-19 12:00:{i%60:02d}.000 [INFO] [Thread-{i%8}] NoteSciPad transaction log record ID={i:08d} payload=ABCDEF123456\n')
"

echo "[OK] Generated test fixtures:"
ls -lh "${TARGET_DIR}"
