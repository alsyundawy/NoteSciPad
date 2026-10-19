# NoteSciPad Linux Packaging Guide (.deb & .rpm)

## 1. Overview

NoteSciPad provides native packaging support for modern Linux distributions via CMake and CPack:
- **Debian / Ubuntu / Linux Mint:** Debian packages (`.deb`)
- **Fedora / RHEL / AlmaLinux / openSUSE:** RPM packages (`.rpm`)
- **Cross-Distro Portable:** AppImage

---

## 2. Package Specifications

| Attribute | Debian (`.deb`) | RPM (`.rpm`) |
| --- | --- | --- |
| Package Name | `notesci-pad` | `notesci-pad` |
| Output Filename | `notesci-pad_<version>_<arch>.deb` | `notesci-pad-<version>-1.<arch>.rpm` |
| Section / Group | `editors` | `Applications/Editors` |
| License | GPL-3.0-or-later | GPL-3.0-or-later |
| Dependencies | `libc6`, `libstdc++6` (auto-detected via shlibdeps) | auto-detected via AutoReq |
| Recommends | `xdg-utils` | `xdg-utils` |
| Desktop Entry | `/usr/share/applications/NoteSciPad.desktop` | `/usr/share/applications/NoteSciPad.desktop` |
| Application Icon | `/usr/share/icons/hicolor/scalable/apps/NoteSciPad.svg` | `/usr/share/icons/hicolor/scalable/apps/NoteSciPad.svg` |

---

## 3. Building Packages Locally

### 3.1 Prerequisites
```bash
# Ubuntu / Debian
sudo apt install build-essential cmake ninja-build dpkg-dev rpm

# Fedora / Red Hat
sudo dnf install gcc-c++ cmake ninja-build rpm-build
```

### 3.2 Build via CMake Targets
```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

# Build .deb package:
cmake --build build --target deb

# Build .rpm package:
cmake --build build --target rpm
```

### 3.3 Build via Helper Script
```bash
./scripts/package-deb-rpm.sh build
```

---

## 4. Installation & Verification

### Debian / Ubuntu
```bash
sudo dpkg -i build/notesci-pad_*.deb
sudo apt-get install -f  # resolves any missing dependencies
```

### Fedora / Red Hat
```bash
sudo dnf install build/notesci-pad-*.rpm
```
