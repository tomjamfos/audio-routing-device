# Audio Routing Device — Roadmap

## Real-Time Audio Rules (non-negotiable)
- **No allocation on audio thread** — allocate in `prepareToPlay`, not in `getNextAudioBlock`
- **No mutexes on audio thread** — use `std::atomic` for any UI→audio shared state
- **No GUI calls from audio thread**
- **No blocking I/O on audio thread**

---

## Stage 1 — Bare passthrough ✅
Get audio flowing: input → output, no processing, no UI controls beyond the window.
- Project scaffold (CMakeLists.txt, .gitignore, JUCE submodule)
- `Main.cpp`: `JUCEApplication` + `MainWindow`
- `MainComponent`: extends `AudioAppComponent`
  - `prepareToPlay`: nothing to allocate yet
  - `getNextAudioBlock`: copy input channels to output channels, sample-for-sample
  - `releaseResources`: nothing to release yet
- Window size ~400×200, title "Audio Routing Device"

## Stage 2 — dB volume control
- Add `std::atomic<float> gain { 1.0f }` to `MainComponent`
- Add `juce::Slider` (range -60 to +12 dB, default 0, suffix " dB")
- Slider `onValueChange` writes `gain = Decibels::decibelsToGain(slider.getValue())`
- `getNextAudioBlock` reads `gain.load()` once per block, multiplies each sample

## Stage 3 — Device selection UI
- Add `AudioDeviceSelectorComponent` to the window (input device, output device, sample rate, buffer size)
- Persist settings to XML via `AudioDeviceManager::createStateXml()`

## Stage 4 — Channel routing matrix (optional)
- Checkboxes mapping input channels to output channels
