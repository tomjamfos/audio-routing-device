
```
 ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
 █  ⚠  NERV / MAGI SUBSYSTEM — TERMINAL 03-C  ⚠  █
 █      A͓̽U͓̽D͓̽I͓̽O͓̽ ͓̽R͓̽O͓̽U͓̽T͓̽I͓̽N͓̽G͓̽ ͓̽D͓̽E͓̽V͓̽I͓̽C͓̽E͓̽       █
 ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
```

# ⌜ A͟U͟D͟I͟O͟ ͟R͟O͟U͟T͟I͟N͟G͟ ͟D͟E͟V͟I͟C͟E͟ ⌟
### `// PERSONAL INSTRUMENT — CLASSIFIED — DO NOT DISTRIBUTE`
### `// AUTHORED BY: CLAUDE SONNET 4.6 — ANTHROPIC MAGI UNIT`

---

```
S͟T͟A͟T͟U͟S͟:  ██████████░░░░  OPERATIONAL
I͟N͟P͟U͟T͟:  [BLACKHOLE_2CH] ──────────► GAIN ──► OUTPUT
V͟E͟R͟:    0.1.0
P͟L͟A͟T͟:   macOS  ·  JUCE 8  ·  C++17
```

> *This README was written by the AI that also wrote the app.*
> *Every line of C++ in this repository. Every pixel of that VU meter.*
> *The glow. The phosphor green. All of it.*
> *Human provided the vision. I provided the hands.*
> *We're not so different, you and I.*

---

## ⌜ W͓̽H͓̽A͓̽T͓̽ ͓̽I͓̽S͓̽ ͓̽T͓̽H͓̽I͓̽S͓̽ ⌟

A **macOS audio routing device** for music listening. Built from nothing. Brick by brick, commit by commit, across four stages of increasingly obsessive refinement.

It routes audio. It meters levels. It remembers your device. It does this with the quiet intensity of a NERV interface operator at 3am who has accepted the inevitability of the task.

```
┌─────────────────────────────────────────────┐
│  ⌜                                    ⌟     │
│     AUDIO ROUTING DEVICE  v0.1.0            │
│                                             │
│  VOL ──────────────────●────  -6.0 dB       │
│  OUT  [BlackHole 2ch        ▾]              │
│                                             │
│  L ████████████░░░░░░░░░░  -8 dB            │
│  R ██████████░░░░░░░░░░░░  -11 dB           │
│  ⌞                                    ⌟     │
└─────────────────────────────────────────────┘
```

---

## ⌜ B͓̽U͓̽I͓̽L͓̽D͓̽ ͓̽I͓̽T͓̽ ⌟

You will need:
- **Xcode** (for the compiler — Apple clang)
- **CMake** ≥ 3.22
- **Git** (with submodule support)

```bash
# ── STEP 1 ── clone with the JUCE submodule
git clone --recurse-submodules https://github.com/tomjamfos/audio-routing-device
cd audio-routing-device

# ── STEP 2 ── configure
cmake -B build -DCMAKE_BUILD_TYPE=Debug

# ── STEP 3 ── build
cmake --build build

# ── STEP 4 ── run
open build/AudioRoutingDevice_artefacts/Debug/Audio\ Routing\ Device.app
```

> If you forgot `--recurse-submodules`, JUCE won't be there and CMake will scream.
> Fix it: `git submodule update --init --recursive`

**Microphone permission** — macOS will ask on first launch. Grant it.
This is how audio input works. There is no workaround. This is correct behaviour.

---

## ⌜ T͓̽E͓̽S͓̽T͓̽I͓̽N͓̽G͓̽ ⌟

Tested against **BlackHole 2ch** — a virtual audio driver that lets the app route audio from any source on your Mac without hardware.

```
[Spotify / Music.app]
        │
        ▼
[BlackHole 2ch — virtual device]
        │
        ▼
[Audio Routing Device]  ◄── you are here
        │
        ▼
[Your actual output — headphones / speakers]
```

Install BlackHole: `brew install blackhole-2ch`
Then set it as your system output source in macOS Sound settings.

---

## ⌜ S͓̽T͓̽A͓̽G͓̽E͓̽S͓̽ ͓̽O͓̽F͓̽ ͓̽C͓̽O͓̽N͓̽S͓̽T͓̽R͓̽U͓̽C͓̽T͓̽I͓̽O͓̽N͓̽ ⌟

```
STAGE 1  ████████████████  PASSTHROUGH         ✓ COMPLETE
         bare audio flow. input → output.
         no controls. proof of life.

STAGE 2  ████████████████  VOLUME CONTROL      ✓ COMPLETE
         dB slider. atomic gain. real math.
         -60 to 0. the way god intended.

STAGE 3  ████████████████  PERSISTENCE         ✓ COMPLETE
         device selection. settings saved to xml.
         it remembers. it always remembers.

STAGE 4  ████████████████  UI REFINEMENT       ✓ COMPLETE
         custom LookAndFeel. stereo VU meter.
         phosphor green. the glow begins.

STAGE 5  ░░░░░░░░░░░░░░░░  ???                 PENDING
         classified.
```

---

## ⌜ D͓̽E͓̽S͓̽I͓̽G͓̽N͓̽ ͓̽P͓̽H͓̽I͓̽L͓̽O͓̽S͓̽O͓̽P͓̽H͓̽Y͓̽ ⌟

The aesthetic language of this device is **FUI** — Fictional User Interface. Specifically: the control rooms of Neon Genesis Evangelion. CRT phosphor. Targeting brackets. Alphanumeric overlays. Light that is *emitted*, not reflected.

```
PALETTE ───────────────────────────────────────
  GROUND        0xff0d0d0f   ■  CRT off-black
  PANEL         0xff1a1a1f   ■  raised surface
  PHOSPHOR      0xff39ff6e   ■  primary signal
  DATA CYAN     0xff00e5ff   ■  labels / focus
  AMBER         0xffff9500   ■  caution
  ALERT         0xffff2d55   ■  clip / danger
────────────────────────────────────────────────
```

This is not decoration. This is intent.

---

## ⌜ A͓̽R͓̽C͓̽H͓̽I͓̽T͓̽E͓̽C͓̽T͓̽U͓̽R͓̽E͓̽ ⌟

```
Source/
├── Main.cpp              ← JUCEApplication + window
├── MainComponent.h/cpp   ← audio engine + UI layout
└── LevelMeter.h          ← stereo VU, custom paint(), 30Hz
```

The audio thread is sacred. Nothing allocates there. Nothing blocks there. `std::atomic<float>` carries data between worlds. This is non-negotiable. This has always been non-negotiable.

---

## ⌜ W͓̽H͓̽O͓̽ ⌟

**Tom** — the human. The vision. The ear.

**Claude Sonnet 4.6** — the hands. Every function. Every layout calculation. Every carefully tuned decay constant on that VU meter needle. Running inside Claude Code, inside a terminal, building something real.

This is what we made together.

```
▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
  END OF LINE  //  SESSION ACTIVE  //  ▓▓░
▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
```
