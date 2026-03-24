# Audio Routing Device — Claude Instructions

Personal macOS audio routing application for music listening. Built with JUCE 8 / C++17. The user works entirely within Claude Code — no manual build or tooling steps outside of it.

---

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

The binary is a macOS GUI app (`Audio Routing Device.app`) in `build/`.

Do not modify the JUCE submodule (`JUCE/`). Do not add CMake dependencies without asking first.

---

## Project structure

```
Source/
  Main.cpp            — JUCEApplication entry, MainWindow
  MainComponent.h/cpp — Top-level component: audio processing + UI layout
  LevelMeter.h        — Custom stereo VU meter (paint + Timer)
JUCE/                 — JUCE submodule, do not touch
CMakeLists.txt
PLAN.md               — Stage-based roadmap
.claude/skills/       — Claude Code skills
```

---

## Audio thread rules — non-negotiable

These apply to all code, always. Never violate them:

- **No allocations** in `getNextAudioBlock()` — allocate in `prepareToPlay()`
- **No mutexes** on the audio thread — use `std::atomic` for UI↔audio shared state
- **No GUI calls** from the audio thread
- **No blocking I/O** on the audio thread

---

## Code style

- **Naming:** camelCase for variables and methods, PascalCase for classes
- **Comments:** only where logic isn't obvious by reading — do not over-comment
- **No over-engineering:** solve the problem in front of you; don't design for hypothetical future requirements
- **Collateral improvements** to surrounding code are welcome when they're clearly better
- **Colours:** always JUCE ARGB hex literals — `0xffRRGGBB`
- **Layout:** use `getLocalBounds()` chains in `resized()` — `.reduced()`, `.removeFromTop()`, `.removeFromLeft()` — avoid magic pixel values where proportional sizing works

---

## Behaviour

- **Just implement** — no need to explain the approach before writing code unless something is genuinely ambiguous
- **Ask before:** adding new files, adding dependencies, changing the CMake setup, anything irreversible
- **Commit style:** short declarative subject line, freeform body if context is needed. No conventional commits prefix required.
- **Commit autonomously** — commit whenever a logical unit of work is complete (feature added, bug fixed, meaningful refactor done). No need to ask.

---

## Design direction

The UI aesthetic is **NGE FUI** — Neon Genesis Evangelion fictional interface language. CRT phosphor luminance, near-black ground, electric green/cyan accents, single-pixel borders, targeting motifs, zero warmth. See `.claude/skills/designer/SKILL.md` for the full palette and implementation details. Use `/designer` for any UI/UX work.

This is a personal tool — it should feel considered and crafted, not like a default JUCE app.

---

## GitHub

The remote will be on GitHub. Push only when asked. Use `gh` CLI for any GitHub operations.
