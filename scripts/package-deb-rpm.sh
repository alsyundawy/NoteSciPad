#!/usr/bin/env bash
# ==============================================================================
# SCRIPT NAME: build-deb-rpm.sh
# DESCRIPTION: Automates building native .deb and .rpm packages for NoteSciPad.
# AUTHOR:      Harry DS Alsyundawy (NoteSciPad Maintainer)
# VERSION:     1.0.0
# ==============================================================================

set -Eeuo pipefail
IFS=$'\n\t'

BUILD_DIR="${1:-build}"
DO_DEB=1
DO_RPM=1

show_help() {
    cat <<EOF
NoteSciPad Linux Package Builder (.deb & .rpm)

Usage: $(basename "$0") [BUILD_DIR] [OPTIONS]

Options:
  --deb-only   Build only Debian (.deb) package
  --rpm-only   Build only RPM (.rpm) package
  -h, --help   Show this help message

Requirements:
  - CMake >= 3.21 and CPack
  - For .deb: dpkg-dev (dpkg-shlibdeps)
  - For .rpm: rpm-build / rpmbuild
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --deb-only) DO_RPM=0; shift ;;
        --rpm-only) DO_DEB=0; shift ;;
        -h|--help)  show_help; exit 0 ;;
        *)
            if [[ -d "$1" ]] || [[ "$1" == "build"* ]]; then
                BUILD_DIR="$1"; shift
            else
                echo "Unknown option: $1" >&2; exit 1
            fi
            ;;
    esac
done

if [[ ! -d "${BUILD_DIR}" ]]; then
    echo "[ERR] Build directory '${BUILD_DIR}' does not exist. Please configure and build NoteSciPad first." >&2
    exit 1
fi

cd "${BUILD_DIR}"

if [[ ${DO_DEB} -eq 1 ]]; then
    echo "=== Building Debian Package (.deb) ==="
    cpack -G DEB
    echo "[OK] Debian package built in ${BUILD_DIR}/"
fi

if [[ ${DO_RPM} -eq 1 ]]; then
    echo "=== Building Red Hat Package (.rpm) ==="
    if command -v rpmbuild &>/dev/null; then
        cpack -G RPM
        echo "[OK] RPM package built in ${BUILD_DIR}/"
    else
        echo "[WARN] rpmbuild command not found. Skipping RPM package." >&2
    fi
fi

echo "Package generation complete."
