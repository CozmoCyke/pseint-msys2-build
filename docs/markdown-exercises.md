# Markdown In Packaged Exercises

This option only affects packaged exercises handled by `pseval`.

- Build switch: `ENABLE_MARKDOWN=1` enables Markdown support in `pseval`.
- Default: enabled, to preserve the current behavior.
- Disable with: `make ARCH=<arch> ENABLE_MARKDOWN=0`

Scope:

- Supported: exercise statements stored as `help.md` inside an exercise source directory or packaged exercise.
- Pipeline: Markdown -> HTML via Hoedown wrapper -> `wxHtmlWindow`.
- HTML help remains supported as before through `help.html`.
- If both `help.html` and `help.md` are present, `help.html` keeps priority.
- If Markdown support is disabled, `help.md` is ignored and `help.html` remains the fallback path.

Out of scope:

- General application help under `bin/help/*`
- `quickhelp.map`
- The historical HTML help pipeline
- Any Parsons-style exercise UI or grading
