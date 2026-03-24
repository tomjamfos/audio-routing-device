# audio routing device

macOS application for routing audio. built with JUCE 8 and C++17.

🦷

---

## requirements

- Xcode
- CMake ≥ 3.22
- Git

---

## build

```bash
git clone --recurse-submodules https://github.com/tomjamfos/audio-routing-device
cd audio-routing-device
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

if you forgot `--recurse-submodules`:

```bash
git submodule update --init --recursive
```

launch:

```bash
open build/AudioRoutingDevice_artefacts/Debug/Audio\ Routing\ Device.app
```

🌽🌽

macOS will request microphone permission on first launch. this is required for audio input.

---

## testing

install BlackHole virtual audio driver:

```bash
brew install blackhole-2ch
```

set BlackHole 2ch as your system output device in System Settings → Sound → Output. the application will route audio from that input to your selected output device.

🪤🦠🪤🦠🪤🦠🪤

---

## structure

```
Source/
├── Main.cpp
├── MainComponent.h
├── MainComponent.cpp     🧲
└── LevelMeter.h
```

`MainComponent` handles audio processing and UI layout. `LevelMeter` is a custom component running at 30Hz. audio thread state is passed via `std::atomic<float>`. no allocations occur on the audio thread.

---

🫀

---

## design

dark theme. VU meter with three zones: safe, caution, clip. settings persist to XML on disk between launches.

```
  0xff0d0d0f   ground
  0xff39ff6e   primary
  0xff00e5ff   secondary         🪸
  0xffff9500   warning
  0xffff2d55   alert
```

---

## notes

🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫🧫

this application was written by Claude Sonnet 4.6. every line of code. this line too.

🫧🪣🫧🪣🫧🪣
