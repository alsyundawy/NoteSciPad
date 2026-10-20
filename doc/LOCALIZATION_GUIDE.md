# NoteSciPad Localization Guide (Indonesian & English)

## 1. Overview

NoteSciPad prioritizes first-class **Bahasa Indonesia** and **English** interface translations. Translation source files are maintained under `i18n/` in standard Qt Linguist XML format (`.ts`) and compiled into binary `.qm` resources at build time.

Supported languages:

- **Indonesian (`id_ID`):** `i18n/NotepadNext_id.ts` (Primary priority)
- **English (`en_US`):** `i18n/NotepadNext_en.ts` (Source reference)
- Additional community languages: Arabic, German, Spanish, French, Italian, Japanese, Korean, Dutch, Polish, Portuguese, Russian, Swedish, Turkish, Ukrainian, Vietnamese, Chinese (Simplified/Traditional).

---

## 2. Localization Workflow for New Features

Whenever a new UI dialog, menu entry, or user notification is added:

1. **Mark User-Facing Strings:**
   Wrap all strings in `tr("...")` or `QCoreApplication::translate("Context", "...")`.
2. **Update Translation Files:**
   Add matching `<message>` entries in `i18n/NotepadNext_id.ts` and `i18n/NotepadNext_en.ts`.
3. **Compile Translations:**
   CMake automatically compiles `.ts` files into `.qm` resources via `qt_add_translations`.
