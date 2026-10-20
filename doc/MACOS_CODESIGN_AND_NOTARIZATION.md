# macOS Codesign & Notarization Workflow

## 1. Overview

Apple macOS requires distributed applications to be signed with an Apple Developer ID Application certificate and notarized by Apple's Notary Service to run smoothly without Gatekeeper warnings.

NoteSciPad provides a production-grade automated pipeline for codesigning with Hardened Runtime and automated notarization:

- **Entitlements:** `deploy/macos/NoteSciPad.entitlements`
- **Automation Script:** `scripts/codesign-and-notarize-macos.sh`
- **CI/CD Integration:** `.github/workflows/release.yml`

---

## 2. Security & Hardened Runtime Entitlements

The entitlements file (`deploy/macos/NoteSciPad.entitlements`) declares the minimal required permissions for NoteSciPad:

- `allow-jit` & `allow-unsigned-executable-memory`: Required by JavaScriptCore for inline expression evaluation.
- `disable-library-validation`: Permits dynamic loading of Lexilla lexers and plugins.
- `automation.apple-events`: Enables integration with system-wide macOS Services.

---

## 3. Automation Script (`scripts/codesign-and-notarize-macos.sh`)

The script performs an end-to-end signing and notarization workflow:

1. **Temporary Keychain Management:** If base64 certificate secrets are present, creates an isolated temporary keychain, imports the `.p12` file, and grants access to `codesign`. The keychain is purged automatically on exit.
2. **Deep Bundle Signing:** Recursively signs nested binaries, frameworks (`Contents/Frameworks/*.framework`), and dylibs with Hardened Runtime (`--options runtime`) and timestamps.
3. **App Bundle Signature:** Signs `NoteSciPad.app` with the entitlements file and Developer ID Application identity.
4. **DMG Signature:** Signs the container `.dmg` file.
5. **Notarization:** Submits the signed DMG to Apple Notary Service via `xcrun notarytool submit --wait`.
6. **Stapling:** Attaches the notarization ticket to the DMG using `xcrun stapler staple`.
7. **Gatekeeper Verification:** Assesses the resulting DMG using `spctl --assess --type open`.

---

## 4. GitHub Actions Configuration

To enable automated signing and notarization in GitHub Releases, configure the following GitHub Repository Secrets:

| Secret Name | Description |
| --- | --- |
| `APPLE_CERTIFICATE_BASE64` | Base64-encoded Developer ID Application `.p12` certificate |
| `APPLE_CERTIFICATE_PASSWORD` | Password for the `.p12` certificate file |
| `APPLE_ID` | Apple Developer Account email |
| `APPLE_APP_SPECIFIC_PASSWORD` | App-Specific Password generated on appleid.apple.com |
| `APPLE_TEAM_ID` | 10-character Apple Developer Team ID (e.g. `ABC123XYZ0`) |

If these secrets are not configured, the GitHub Actions release workflow builds the DMG and skips signing without failing.
