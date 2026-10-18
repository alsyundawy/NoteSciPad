# ⚡ NoteSciPad

> 🇬🇧 **NoteSciPad** — An independent, lightweight, and high-performance text and source code editor for **macOS and Linux**, powered by vendored **Scintilla** and **Lexilla** engines.
>
> 🇮🇩 **NoteSciPad** — Editor teks dan kode sumber mandiri, ringan, dan berkinerja tinggi untuk **macOS dan Linux**, ditenagai oleh mesin vendored **Scintilla** dan **Lexilla**.

[![Build NoteSciPad](https://github.com/alsyundawy/NoteSciPad/workflows/Build%20Notepad%20Next/badge.svg)](https://github.com/alsyundawy/NoteSciPad/actions)
[![Latest Version](https://img.shields.io/github/v/release/alsyundawy/NoteSciPad)](https://github.com/alsyundawy/NoteSciPad/releases)
[![Language: Indonesian & English](https://img.shields.io/badge/Language-ID%20%7C%20EN-blue.svg)](#localization--language-support)
[![License: GPL-3.0](https://img.shields.io/badge/License-GPL--3.0-green.svg)](LICENSE)
[![Engine: Scintilla + Lexilla](https://img.shields.io/badge/Engine-Scintilla%20%2B%20Lexilla%20(Vendored)-orange.svg)](#editor-engine-policy)
[![Platform: macOS & Linux](https://img.shields.io/badge/Platform-macOS%20%7C%20Linux-purple.svg)](#installation--platforms)

---

## ⚠️ Important Legal & Trademark Notice / Pernyataan Hukum Penting

> **English:**  
> **NoteSciPad is an independent project. It is not affiliated with, endorsed by, or associated with Notepad++, NotepadNext, Notepad4, or Notepad3.**

> **Bahasa Indonesia:**  
> **NoteSciPad adalah proyek independen. Proyek ini tidak berafiliasi dengan, didukung oleh, atau terkait dengan Notepad++, NotepadNext, Notepad4, atau Notepad3.**

---

## 📖 Overview / Gambaran Umum

### 🇬🇧 English
**NoteSciPad** is designed to deliver a native, ultra-responsive, and reliable editing experience for developers, sysadmins, and writers on macOS and Linux. It combines the clean Qt 6 cross-platform architecture with the legendary text editing capabilities of **Scintilla** and **Lexilla**, alongside lightweight power-user utilities inspired by the best ideas in the Scintilla editor ecosystem (including Notepad4 and Notepad3).

Key highlights:
- **Rock-solid Core**: Uses vendored, statically linked Scintilla and Lexilla engines for battle-tested syntax highlighting, folding, and massive file handling.
- **Lightweight & Fast**: Instant startup, minimal memory overhead, zero telemetry, and native UI responsiveness.
- **Power Features**: Mark occurrences with live match counts, enhanced auto-completion (brackets/quotes), File Browser dock, Base64 & number converters, GUID & timestamp generators, and comprehensive encoding detection.
- **Full macOS & Linux Polish**: Native Apple Silicon (arm64) & Intel (x64) binaries, dark mode, Retina high-DPI icons, and standard desktop integrations (.desktop / AppStream).
- **First-class Localization**: Complete English and Indonesian interface translations.

### 🇮🇩 Bahasa Indonesia
**NoteSciPad** dirancang untuk menghadirkan pengalaman penyuntingan native yang sangat cepat, ringan, dan andal bagi pengembang, administrator sistem, dan penulis di macOS dan Linux. Menggabungkan arsitektur Qt 6 modern dengan ketangguhan mesin penyunting teks **Scintilla** dan **Lexilla**, serta fitur-fitur praktis yang terinspirasi dari inovasi terbaik ekosistem editor Scintilla (termasuk Notepad4 dan Notepad3).

Keunggulan utama:
- **Inti Sangat Tangguh**: Menggunakan mesin Scintilla dan Lexilla vendored yang ditautkan secara statis untuk penyorotan sintaksis akurat, code folding, dan penanganan file berukuran besar.
- **Ringan & Cepat**: Waktu buka instan, konsumsi memori minimal, tanpa telemetri, dan respons antarmuka native.
- **Fitur Produktivitas**: Tandai semua kemunculan (Mark All) dengan penghitung kecocokan langsung, auto-completion cerdas (kurung/tanda kutip), panel File Browser (matepath-style), konverter Base64 & angka, generator GUID & timestamp, serta deteksi encoding cerdas.
- **Optimal di macOS & Linux**: Mendukung penuh Apple Silicon (arm64) & Intel (x64), mode gelap (Dark Mode), ikon resolusi tinggi Retina, serta paket Linux standar.
- **Dukungan Bahasa**: Pelokalan penuh Bahasa Indonesia dan Bahasa Inggris.

---

## ⚙️ Editor Engine Policy / Kebijakan Mesin Editor

NoteSciPad adheres to a strict engine policy:
1. **Always Scintilla + Lexilla**: The editor engine is strictly vendored under `thirdparty/scintilla` and `thirdparty/lexilla`.
2. **Statically Linked**: It is statically compiled via CMake without external runtime dependencies like system QScintilla.
3. **ScintillaNext Wrapper**: All UI and document interactions are routed cleanly through `src/ScintillaNext.*`.

---

## 🚀 Quickstart / Panduan Singkat

### macOS
```bash
# 1. Unduh rilis terbaru / Download latest release (.dmg)
# https://github.com/alsyundawy/NoteSciPad/releases

# 2. Atau jadikan NoteSciPad sebagai default editor teks macOS:
chmod +x NotepadNext-Default-Editor.sh
./NotepadNext-Default-Editor.sh -f
```

### Linux
```bash
# Jalankan AppImage / Run AppImage
chmod +x NoteSciPad-*.AppImage
./NoteSciPad-*.AppImage
```

---

## 🛠️ Building from Source / Membangun dari Kode Sumber

### Requirements
- **macOS** 12+ or **Linux** (Ubuntu 22.04+, Debian 12+, Fedora 38+, Arch)
- **CMake** ≥ 3.21
- **Ninja** build system
- **Qt 6** (Qt 6.5 LTS, 6.8, or 6.10+)
- **C++20** compatible compiler (Clang / GCC / Apple Clang)

### Build Commands
```bash
# 1. Masuk ke repositori
cd /Users/alsyundawy/Downloads/GitHub/NoteSciPad

# 2. Konfigurasi build dengan Ninja
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release

# 3. Kompilasi
cmake --build build --parallel

# 4. Jalankan aplikasi (macOS)
open build/src/NoteSciPad.app
```

---

## 🗺️ Roadmap & Features / Rencana Pengembangan

- [x] **Phase 0: Foundation** — Clean source acquisition, branding NoteSciPad, custom high-res icons (.icns / .ico / .svg), standards, and repository setup.
- [ ] **Phase 1: Core Identity**
  - Mark All Occurrences + live match count
  - Enhanced auto-completion (auto-close brackets/quotes, indent rules)
  - File Browser dock (matepath-style QFileSystemModel)
  - Base64 encode & decode utilities
  - Number conversion (Dec ↔ Hex ↔ Oct ↔ Bin)
  - Insert GUID and configurable timestamps
  - Full-screen mode toggle
- [ ] **Phase 2: High-Value Tools** — CallTip color previews, JSON/CSS pretty & compress, Copy as RTF, Math expression evaluation, Focused View, Show Character Info, Update File Time & Save with Original Time.
- [ ] **Phase 3: Advanced Polish** — Function List, Run external tools, Change History margin, PCRE2 regex integration.

---

## 🤝 Origin & Credits / Asal Kode & Penghargaan

NoteSciPad stands on the shoulders of giants in the open-source community:

- **alsyundawy/NotepadNext-MacOS**: Original macOS packaging, Gatekeeper fixes, and Indonesian translations.
- **dail8859/NotepadNext**: Cross-platform Qt framework and docking architecture.
- **zufuliu/Notepad4**: Innovative lightweight features, character info, file time utilities, and editor design principles.
- **rizonesoft/Notepad3**: Focused view concept, TinyExpr++ math ideas, and practical power tools.
- **Neil Hodgson**: The foundational **Scintilla** and **Lexilla** editing engines.
- **The Qt Company**: Modern cross-platform Qt 6 framework.

---

## 📄 License / Lisensi

NoteSciPad is free software licensed under the **GNU General Public License v3.0 or later (GPL-3.0-or-later)**.  
See the [LICENSE](LICENSE) file for complete details.

**Primary Maintainer:** HARRY DERTIN SUTISNA ALSYUNDAWY  
**Repository:** [https://github.com/alsyundawy/NoteSciPad](https://github.com/alsyundawy/NoteSciPad)
