# NoteSciPad Universal Linux Packaging (Flatpak & AppImage)

## 1. Overview

In addition to native Debian (\`.deb\`) and Red Hat (\`.rpm\`) packages, NoteSciPad supports universal, distribution-agnostic Linux distribution formats:

- **Flatpak:** Sandboxed desktop application standard for Flathub, GNOME Software, and KDE Discover.
- **AppImage:** Single-binary portable executable running out-of-the-box on Ubuntu, Fedora, Arch, openSUSE, and Debian without system root privileges.

---

## 2. Flatpak Packaging

The Flatpak manifest (\`deploy/linux/flatpak/io.github.alsyundawy.NoteSciPad.yml\`) uses the standard KDE/Qt 6 runtime (\`org.kde.Platform\` 6.6):

```bash
# Build Flatpak bundle:
flatpak-builder --force-clean build-flatpak deploy/linux/flatpak/io.github.alsyundawy.NoteSciPad.yml

# Test locally:
flatpak-builder --run build-flatpak deploy/linux/flatpak/io.github.alsyundawy.NoteSciPad.yml NoteSciPad
```

Permissions granted:

- X11 and Wayland display sockets.
- Full filesystem access (\`--filesystem=host\`) enabling seamless file editing across all user partitions.
- D-Bus integration for desktop file manager opening.

---

## 3. AppImage Packaging

AppImage bundling is automated via \`scripts/package-flatpak-appimage.sh\`:

```bash
./scripts/package-flatpak-appimage.sh --appimage
```

The resulting \`NoteSciPad-x86_64.AppImage\` bundles all runtime dependencies, Qt 6 platform plugins, and vendored Scintilla/Lexilla shared modules into an executable image.
