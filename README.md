# imspinner
Set of nice spinners for imgui

![spinners](https://github.com/user-attachments/assets/796d734b-eb2b-4d6f-9199-e4609a57935d)

# headers

The library is header-only and split into three files; each one includes the
one below it, so just include the highest level you need:

| header | what you get | includes |
|--------|--------------|----------|
| `imspinner.h` | the spinners themselves + the `Spinner<>` dispatcher | — |
| `imspinner_text.h` | text loaders (e.g. `SpinnerTextFade`) | `imspinner.h` |
| `imspinner_demo.h` | the interactive demo gallery (`demoSpinners()`) | `imspinner_text.h` |

```c++
#include "../imspinner/imspinner.h"          // graphical spinners only
// or
#include "../imspinner/imspinner_text.h"     // + text loaders
// or
#include "../imspinner/imspinner_demo.h"     // + interactive demo

// use a single spinner anywhere inside an ImGui window:
ImSpinner::SpinnerRainbow("rainbow", 16.f, 2.f, ImColor(255, 255, 255), 8.f);
```

# how to test

1. include the demo header (no `#define` needed — including it is the opt-in)
   and call `demoSpinners()` once per frame:
```c++
#include "../imspinner/imspinner_demo.h"

int main(int, char**) {
  ...
  while (!done) {
     ...
     ImSpinner::demoSpinners();
  }
}
```
2. enable edit mode with checkbox 'show number' or play with options
![image](https://github.com/dalerank/imspinner/assets/918081/dac20ca2-30ab-48a2-b4bb-36caa4676d2c)

# text loaders

<img width="922" height="294" alt="texts" src="https://github.com/user-attachments/assets/0e34807d-8054-4f46-95d6-1f5bb7c3381d" />

`imspinner_text.h` adds a family of animated **text** loaders "Loading…" effects. 
They share a common signature `SpinnerTextXxx(const char *label, float radius, 
ImColor color = white, float speed = …, …, const char *text = "Loading...")`
 and the text is auto-scaled to fit a `2*radius`-wide cell. 
A few take extra params (`thickness`, `mode`, etc.) see the notes column.

```c++
#include "../imspinner/imspinner_text.h"
...
ImSpinner::SpinnerTextTyping("typing", 30.f, ImColor(255, 255, 255), 1.f, 3, "Loading...");
```

| function | effect | notes |
|----------|--------|-------|
| `SpinnerTextFade` | whole text pulses opacity in/out | |
| `SpinnerTextFading` | per-character fading shimmer runs over the text | takes `text, radius, fsize` |
| `SpinnerTextUnderline` | underline bar grows left→right | `thickness` |
| `SpinnerTextUnderlineDots` | dashed underline lights up dash by dash | `thickness`, `dashes` |
| `SpinnerTextTyping` | trailing chars reveal one by one | `tail` (≤0 = whole word) |
| `SpinnerTextScroll` | seamless horizontal marquee | |
| `SpinnerTextColorFill` | two-tone fill / single moving highlight | `bg`, `mode` (0 fill, 1 highlight) |
| `SpinnerTextScrollColors` | multi-color scrolling marquee | |
| `SpinnerTextColorCycle` | text wipes through a 4-color palette | |
| `SpinnerTextBounce` | alternating letters bob up/down | |
| `SpinnerTextSplit` | top/bottom halves slide apart and wrap | `mode` (0 sync, 1 delayed) |
| `SpinnerTextUnderlineScroll` | underline fills, then the text scrolls away | `thickness` |
| `SpinnerTextRoll` | alternating letters roll vertically | |
| `SpinnerTextColorful` | per-letter color blink (rainbow palette) | |
| `SpinnerTextCascade` | letters drop in from above / fall out one by one | |
| `SpinnerTextConveyor` | letters slide in from the right / out the left | |
| `SpinnerTextReveal` | letters fade in, then out, sequentially | |
| `SpinnerTextWave` | a hop wave travels across the letters | |
| `SpinnerTextSweep` | a bar sweeps across, inverting the text inside it | `bg` |
| `SpinnerTextShine` | a glint band passes over the text | `bg` |
| `SpinnerTextScrollSweep` | scroll + full-width inverting sweep | `bg` |
| `SpinnerTextSpotlight` | a round spotlight scans back and forth | |
| `SpinnerTextShake` | the whole word jitters between offsets | `mode` (0 snap, 1 smooth) |
| `SpinnerTextFlip` | flat "card flip" (rotateX → rotateY → scale) | |
| `SpinnerTextSpin` | the two halves spin a full turn alternately | |
| `SpinnerTextTumble` | spin + flips | `mode` (0 flat, 1 3D perspective) |
| `SpinnerTextSwirl` | alternating letters counter-rotate about the centre | |
| `SpinnerTextRollWave` | vertical roll wave across the letters | |
| `SpinnerTextVibrate` | nervous per-letter vibration | |
| `SpinnerTextGlitch` | RGB-split chromatic-aberration buzz | fixed R/G/B (no `color`) |
| `SpinnerTextBlur` | random letters flicker out of focus | faked halo (no real blur) |
| `SpinnerTextDropout` | characters blink out (missing letters) | |
| `SpinnerTextScanline` | VHS scanline displacement glitch | `mode` (0 horizontal, 1 vertical) |
| `SpinnerTextSquash` | a scanning band squashes the text vertically | |
| `SpinnerTextScramble` | slot-machine scramble of the letters | fixed `"Loading..."` (no `text`) |
| `SpinnerTextDecode` | letters decode/encode into symbols | |
| `SpinnerTextVanish` | letters fly up & fade out, then drop back in | |

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
