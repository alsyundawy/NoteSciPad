#!/usr/bin/env python3
"""
Generate compile_commands.json for SonarQube, SonarLint, Clangd, and C/C++ extensions.
Scans src/ and tests/ for C, C++, Objective-C++, and header files.
Decomposed into modular functions with cognitive complexity < 10.
"""

import json
from pathlib import Path


def get_include_dirs(root: Path) -> list[str]:
    """Return all project and dependency include paths."""
    dirs = [
        str(root / "src"),
        str(root / "src" / "decorators"),
        str(root / "src" / "dialogs"),
        str(root / "src" / "docks"),
        str(root / "src" / "features"),
        str(root / "src" / "utils"),
        str(root / "src" / "widgets"),
        str(root / "build" / "src" / "SparkNotePad_autogen" / "include"),
        str(root / ".cpm-cache" / "ads" / "fb9d" / "src"),
        str(root / "build" / "_deps" / "ads-build" / "src"),
        str(root / ".cpm-cache" / "qsimpleupdater" / "531e" / "include"),
        str(root / ".cpm-cache" / "qsimpleupdater" / "531e" / "src"),
        str(root / ".cpm-cache" / "singleapplication" / "d282"),
        str(root / ".cpm-cache" / "editorconfig_core_qt" / "87f1"),
        str(root / "thirdparty" / "scintilla" / "include"),
        str(root / "thirdparty" / "scintilla" / "src"),
        str(root / "thirdparty" / "scintilla" / "qt" / "ScintillaEditBase"),
        str(root / "thirdparty" / "scintilla" / "qt" / "ScintillaEdit"),
        str(root / "thirdparty" / "lexilla" / "include"),
        str(root / "thirdparty" / "lexilla" / "access"),
        str(root / "thirdparty" / "lexilla" / "lexlib"),
        str(root / "thirdparty" / "lua" / "src"),
        str(root / "thirdparty" / "uchardet" / "src"),
        str(root / "tests"),
        # Qt system includes
        "/Users/alsyundawy/Library/FlyEnv/env/node/include",
        "/Users/alsyundawy/Library/FlyEnv/env/node/include/QtCore",
        "/Users/alsyundawy/Library/FlyEnv/env/node/include/QtGui",
        "/Users/alsyundawy/Library/FlyEnv/env/node/include/QtWidgets",
        "/Users/alsyundawy/Library/FlyEnv/env/node/include/QtNetwork",
        "/Users/alsyundawy/Library/FlyEnv/env/node/include/QtPrintSupport",
        "/Users/alsyundawy/Library/FlyEnv/env/node/include/QtCore5Compat",
        "/Users/alsyundawy/Library/FlyEnv/env/node/include/QtTest",
        "/Users/alsyundawy/Qt/6.12.0/macos/include",
        "/Users/alsyundawy/Qt/6.12.0/macos/include/QtCore",
        "/Users/alsyundawy/Qt/6.12.0/macos/include/QtGui",
        "/Users/alsyundawy/Qt/6.12.0/macos/include/QtWidgets",
        "/Users/alsyundawy/Qt/6.12.0/macos/include/QtNetwork",
        "/Users/alsyundawy/Qt/6.12.0/macos/include/QtPrintSupport",
        "/Users/alsyundawy/Qt/6.12.0/macos/include/QtCore5Compat",
        "/Users/alsyundawy/Qt/6.12.0/macos/include/QtTest",
    ]
    return [d for d in dirs if Path(d).exists()]


def get_base_defines() -> list[str]:
    """Return common preprocessor defines."""
    return [
        '-DAPP_VERSION="1.3.5"',
        "-DSCI_OWNREGEX",
        "-DADS_STATIC",
        "-DQSIMPLEUPDATER_STATIC",
        "-DQAPPLICATION_CLASS=QApplication",
        "-DQT_CORE_LIB",
        "-DQT_GUI_LIB",
        "-DQT_WIDGETS_LIB",
        "-DQT_NETWORK_LIB",
        "-DQT_PRINTSUPPORT_LIB",
        "-DQT_CORE5COMPAT_LIB",
        "-DSCINTILLA_QT=1",
        "-DSCI_LEXER=1",
        "-D_CRT_SECURE_NO_DEPRECATE=1",
    ]


def collect_source_files(search_paths: list[Path]) -> list[Path]:
    """Find all C++, C, Objective-C++, and header files."""
    valid_exts = {".cpp", ".c", ".mm", ".h", ".hpp"}
    files: list[Path] = []
    for sp in search_paths:
        if sp.exists():
            for p in sp.rglob("*"):
                if p.is_file() and p.suffix in valid_exts:
                    files.append(p)
    files.sort()
    return files


def determine_compiler_and_std(suffix: str) -> tuple[str, str]:
    """Select compiler and standard flags based on file extension."""
    if suffix == ".c":
        return "/usr/bin/clang", "-std=c17"
    if suffix == ".mm":
        return "/usr/bin/clang++", "-std=c++20 -x objective-c++"
    if suffix in {".h", ".hpp"}:
        return "/usr/bin/clang++", "-std=c++20 -x c++-header"
    return "/usr/bin/clang++", "-std=c++20"


def build_compile_entry(
    sf: Path, root: Path, inc_flags: list[str], base_defines: list[str]
) -> dict[str, str]:
    """Generate a single compile command dictionary."""
    rel_path = sf.relative_to(root)
    compiler, std_flag = determine_compiler_and_std(sf.suffix)

    defines = list(base_defines)
    if "tests" in rel_path.parts:
        defines.extend(["-DUNIT_TESTING=1", "-DQT_TESTLIB_LIB"])

    cmd_parts = [
        compiler,
        std_flag,
        *defines,
        *inc_flags,
        "-c",
        str(rel_path),
        "-o",
        f"build/{rel_path.stem}.o",
    ]

    return {
        "directory": str(root),
        "command": " ".join(cmd_parts),
        "file": str(sf),
    }


def write_database(entries: list[dict[str, str]], build_dir: Path, root: Path) -> None:
    """Save compile_commands.json to build/ and maintain root symlink."""
    out_file = build_dir / "compile_commands.json"
    with open(out_file, "w", encoding="utf-8") as f:
        json.dump(entries, f, indent=2)

    root_symlink = root / "compile_commands.json"
    try:
        if root_symlink.is_symlink() or root_symlink.exists():
            root_symlink.unlink()
        root_symlink.symlink_to("build/compile_commands.json")
    except OSError:
        with open(root_symlink, "w", encoding="utf-8") as f:
            json.dump(entries, f, indent=2)


def main() -> None:
    """Orchestrate compilation database generation."""
    root = Path(__file__).resolve().parent.parent
    build_dir = root / "build"
    build_dir.mkdir(parents=True, exist_ok=True)

    include_dirs = get_include_dirs(root)
    inc_flags = [f"-I{inc}" for inc in include_dirs]
    base_defines = get_base_defines()

    source_files = collect_source_files([root / "src", root / "tests"])
    entries = [
        build_compile_entry(sf, root, inc_flags, base_defines) for sf in source_files
    ]

    write_database(entries, build_dir, root)
    print(
        f"Generated compilation database with {len(entries)} entries at {build_dir / 'compile_commands.json'}"
    )


if __name__ == "__main__":
    main()
