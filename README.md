# imspinner
Set of nice spinners for imgui

![spinners](https://github.com/user-attachments/assets/796d734b-eb2b-4d6f-9199-e4609a57935d)

# headers

The library is header-only and split into add-on files; each one includes the
one below it, so just include the highest level you need:

| header | what you get | includes |
|--------|--------------|----------|
| `imspinner.h` | the spinners themselves | — |
| `imspinner_compat.h` | `Spinner<>` typed dispatcher | `imspinner.h` + `imspinner_bars.h` |
| `imspinner_bars.h` | bar-chart spinners (e.g. `SpinnerFadeBars`) | `imspinner.h` |
| `imspinner_dots.h` | dot-based spinners (e.g. `SpinnerBounceDots`) | `imspinner.h` |
| `imspinner_text.h` | text loaders (e.g. `SpinnerTextFade`) | `imspinner.h` |
| `imspinner_demo.h` | the interactive demo gallery (`demoSpinners()`) | above + `imspinner_compat.h` |

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

# bar spinners

<img width="989" height="298" alt="bars" src="https://github.com/user-attachments/assets/7c20449b-4de8-4e0e-8d44-ac4d84668716" />

Bar-chart and fading-bar spinners live in `imspinner_bars.h`. `SpinnerBarChartSine` is
also wired into the core `Spinner<e_st_barchartsine>` dispatcher.

```c++
#include "../imspinner/imspinner_bars.h"   // pulls in imspinner.h
...
ImSpinner::SpinnerFadeBars("bars", 6.f, ImColor(255, 255, 255), 2.8f, 3);
ImSpinner::SpinnerBarChartRainbow("rainbow", 16.f, 4.f, ImColor(255, 255, 255), 4.f);
```

| function | effect |
|----------|--------|
| `SpinnerFadeBars` | vertical bars fade in sequence (`scale` stretches height) |
| `SpinnerBarsRotateFade` | radial bars rotate with fading opacity |
| `SpinnerBarsScaleMiddle` | bars scale from the centre outward |
| `SpinnerBarChartSine` | sine-wave bar chart (also `Spinner<e_st_barchartsine>`) |
| `SpinnerBarChartAdvSine` | dense sine bars with horizontal wobble |
| `SpinnerBarChartAdvSineFade` | dense sine bars with fade |
| `SpinnerBarChartRainbow` | rainbow bars with independent wave heights |
| `SpinnerFluid` | fluid equalizer-style columns |
| `SpinnerFluidPoints` | fluid columns drawn as stacked dots |
| `SpinnerBarsSeqPulse` | three bars shrink height in sequence |
| `SpinnerBarsCascadeGrow` | bottom bars grow in a cascade |
| `SpinnerBarsRise` | three bars slide upward with stagger |
| `SpinnerBarsCornerHop` | bars hop diagonally toward the top |
| `SpinnerBarsDiagonalGrow` | bars step through corner positions |
| `SpinnerBarsBounceCenter` | centre-anchored bars bounce between slots |
| `SpinnerBarsScaleAlt` | bars pulse height with alternate timing |
| `SpinnerBarsCornerWave` | bottom bars step in a corner wave |
| `SpinnerBarsJump` | bars swap between top and bottom |
| `SpinnerBarsDoubleRow` | two rows of bar segments slide horizontally |
| `SpinnerBarsSixPulse` | six corner bars pulse height 50↔30% |
| `SpinnerBarsSixStagger` | six bars shrink in staggered wave order |
| `SpinnerBarsMorphPlus` | five bars morph tall→square→wide |
| `SpinnerBarsFlipSix` | six bars shrink while positions flip every 2 s |
| `SpinnerBarsSwapTriBottom` | three bars swap bottom anchors while shrinking |
| `SpinnerBarsSwapTriZigzag` | three bars zigzag between corner pairs |
| `SpinnerBarsMorphDiagonal` | three diagonal bars morph tall→square→wide |
| `SpinnerBarsSlideDiagonal` | three diagonal bars shrink while outers swap |
| `SpinnerBarsConicAlternate` | two conic-corner tiles slide diagonally |
| `SpinnerBarsConicWalk` | two conic corners step a 2×2 path |
| `SpinnerBarsMarchIn` | four stripe rows march in from the left |
| `SpinnerBarsMarchDown` | four stripe rows drop in from the top |
| `SpinnerBarsWaveSkew` | four rows skew diagonally then reset |
| `SpinnerBarsRowDrop` | four rows drop one-by-one |
| `SpinnerBarsZigzagRows` | four rows alternate stripe alignment |
| `SpinnerBarsFillCascade` | rows fill left→right in stagger |
| `SpinnerBarsPingPongRows` | rows ping-pong stripe alignment |
| `SpinnerBarsWaveRowsWide` | four rows wave centre↔edges |
| `SpinnerBarsGridFade` | 3×3 dot grid fade wave |
| `SpinnerBarsGridSpread` | 3×3 dot grid spread pulse |
| `SpinnerBarsStretch` | three pill bars stretch vertically from a centre dot |
| `SpinnerBarsStretchSeq` | three pill bars grow to full height one-by-one |
| `SpinnerBarsBounceBall` | ball rolls across three bars while bouncing |
| `SpinnerBarsKnockDots` | bouncing bar knocks a row of dots downward |
| `SpinnerBarsStaircase` | ball bounces down a descending staircase of bars |
| `SpinnerBarsKnockAway` | bar knocks two dots down then shoves the last away |
| `SpinnerBarsGates` | column gates retract to let a ball roll through |
| `SpinnerBarsCapture` | ball rolls to the centre and the gate traps it |
| `SpinnerBarsEscape` | trapped ball slips aside and shoots out of the top |
| `SpinnerBarsDevour` | ball swallows two dots, grows, then dashes away |
| `SpinnerBarsLift` | three tall rounded bars rise in a staggered wave |
| `SpinnerBarsGapSlide` | a round gap travels along three static bars |
| `SpinnerBarsConverge` | bar and dot meet in the centre of each column |
| `SpinnerBarsSwapEnds` | bar and dot swap top/bottom ends in a wave |
| `SpinnerBarsRelay` | bars drop, rise with dots, then dots drop in a wave |
| `SpinnerBarsPush` | descending dots push the bars down column by column |
| `SpinnerBarsPushWave` | a single dot-push travels across the columns |
| `SpinnerBarsGather` | four arms assemble a rotating plus around a dot |
| `SpinnerBarsSplit` | two bars spread from the centre while the figure rotates |
| `SpinnerBarsSlot` | a dot passes through two opening bars while rotating |


| mode | effect |
|------|--------|
| `0` | default timing and direction |
| `1` | alternate animation per spinner (sync pulse, reverse stagger, top anchor, single walking tile, synchronized rows, reverse grid wave, etc.) |
| `2` | reversed timeline (`t → 1−t`) |


| function | effect |
|----------|--------|
| `SpinnerDotsBarsGrowSync` | three bars grow in sync; end dots ride top/bottom |
| `SpinnerDotsBarsGrowCascade` | columns grow one after another |
| `SpinnerDotsBarsDotRun` | three bottom bars; dot runs along the top |
| `SpinnerDotsBarsBarRun` | three top dots bounce; bar runs below |
| `SpinnerDotsBarsDotArc` | tiered bars; dot arcs diagonally |
| `SpinnerDotsBarsBarJump` | bar jumps along the bottom; dots swap above |
| `SpinnerDotsBarsHalfPulse` | six half-bars pulse; dot walks left→right |
| `SpinnerDotsBarsHalfPulseMid` | like HalfPulse; dot stops at centre |
| `SpinnerDotsBarsHalfPulseExit` | like HalfPulseMid; dot exits upward |
| `SpinnerDotsBarsVersus` | wide arena — scaling dot vs shuttling bar |
| `SpinnerDotsBarsMaskGrow` | three bars grow upward with cap dots |
| `SpinnerDotsBarsMaskCascade` | cap dots slide along full-height bars |
| `SpinnerDotsBarsColSplit` | bars split around dots per column |
| `SpinnerDotsBarsColSwap` | bars and dots swap top/bottom |
| `SpinnerDotsBarsColDrop` | bars shrink from bottom in sequence |
| `SpinnerDotsBarsSwapAlt` | bars and dots bounce in alternating order |
| `SpinnerDotsBarsSwapLoop` | continuous SwapAlt loop |
| `SpinnerDotsBarsCrossSpin` | four bars slide in; frame rotates 90° |
| `SpinnerDotsBarsRailSpin` | bar–dot–bar slides off-screen while rotating |
| `SpinnerDotsBarsRailOrbit` | like RailSpin; dot also orbits vertically |

These also accept optional `mode` (0 = default, 2 = reversed timeline).

# dot spinners

<img width="922" height="303" alt="dots" src="https://github.com/user-attachments/assets/7956b9d3-d314-4164-8b3e-57224b7941df" />

A large family of **dot**-based spinners lives in the `imspinner_dots.h` add-on. They
mostly share the shape `SpinnerXxxDots(const char *label, float radius, float thickness,
ImColor color = white, float speed = 2.8f, …)`, where the trailing count argument is
usually `dots`/`lt` (number of dots). `SpinnerDots` additionally takes a `float*` phase
pointer; some take `bg`/`mode`/`delta`. (`SpinnerDots` and `SpinnerVDots` stay in the
core `imspinner.h` because the `Spinner<>` dispatcher references them.)

```c++
#include "../imspinner/imspinner_dots.h"   // pulls in imspinner.h
...
ImSpinner::SpinnerBounceDots("dots", 16.f, 6.f, ImColor(255, 255, 255), 6.f, 3);
```

| function | effect |
|----------|--------|
| `SpinnerDots` | dots fade in turn around a circle (also exposed via `Spinner<e_st_dots>`) |
| `SpinnerVDots` | ring of dots with a second rotating set (also `Spinner<e_st_vdots>`) |
| `SpinnerBounceDots` | a few dots bounce up in sequence |
| `SpinnerZipDots` | dots zip together and apart |
| `SpinnerDotsToPoints` | dots contract from a ring toward points |
| `SpinnerDotsToBar` | dots line up into a bar and back |
| `SpinnerWaveDots` | a row of dots ripples in a sine wave |
| `SpinnerFadeDots` | dots fade in/out in sequence |
| `SpinnerThreeDots` | three pulsing dots |
| `SpinnerFiveDots` | five pulsing dots |
| `SpinnerMultiFadeDots` | multiple rows of fading dots |
| `SpinnerScaleDots` | dots scale up/down in turn |
| `SpinnerMovingDots` | dots travel across and wrap around |
| `SpinnerRotateDots` | groups of dots rotate around the centre |
| `SpinnerOrionDots` | dots spread along rotating arcs |
| `SpinnerGalaxyDots` | dots swirl along galaxy-like arms |
| `SpinnerIncDots` | dots light up incrementally around a circle |
| `SpinnerIncFullDots` | incremental dots filling the full ring |
| `SpinnerIncScaleDots` | dots scale up incrementally around a circle |
| `SpinnerSomeScaleDots` | a subset of dots scales in turn |
| `SpinnerDotsLoading` | dots with a separate background ring |
| `SpinnerSquareRandomDots` | dots blink at random inside a square grid |
| `SpinnerHboDots` | HBO-style fading dots |
| `SpinnerMoonDots` | two dots chasing like a moon orbit |
| `SpinnerTwinHboDots` | two interleaved HBO dot rings |
| `SpinnerThreeDotsStar` | three dots arranged/rotating in a star |
| `SpinnerSwingDots` | dots swinging like a pendulum |
| `SpinnerDnaDots` | dots winding along a DNA double helix |
| `Spinner3SmuggleDots` | three dots weaving/smuggling past each other |
| `SpinnerDotsTyping` | a row of dots appears one by one, then resets (typing) |
| `SpinnerDotsStep` | a single dot hops across slots left→right, then loops |
| `SpinnerDotsGather` | a row of dots spreads apart and squeezes together |
| `SpinnerDotsShift` | three dots; the side dots shuffle in/out around a fixed centre |
| `SpinnerDotsOrbit` | three dots dance — one orbits a half-circle over the others |
| `SpinnerDotsCircle` | two dots slide while a third orbits a full circle |
| `SpinnerDotsSquare` | three dots chase around the corners of a square |
| `SpinnerDotsShuffle` | four dots fan out into loops and shuffle back into a row |
| `SpinnerDotsSplit` | three dot pairs open to the edges and close, in a left→right wave |
| `SpinnerDotsLeader` | a leader dot sweeps left→right while trailing pairs shift columns |
| `SpinnerDotsRolling` | a row of dots mirror-flips while a rotating overlay carousels them |
| `SpinnerDotsTriangle` | three dots on a triangle rotate one corner clockwise each half-cycle |
| `SpinnerDotsCascade` | three dots drop one-by-one into a 3×3 grid, pause, then fall to the bottom row |
| `SpinnerDotsSwap` | a fixed centre dot; two satellites cycle horizontal → diagonal → vertical → swapped |
| `SpinnerDotsSpread` | four dots stack left, fan into a row, pause, then collapse to the right |
| `SpinnerDotsTwin` | two mirrored dot-halves slide −25%, spin one turn, then slide back |
| `SpinnerDotsHop` | two dots drop in turn while a third rolls across the top with a hop |
| `SpinnerDotsJiggle` | side dots wobble at 40%/90%; the centre dot shakes elastically |
| `SpinnerDotsVibrate` | static side dots; the centre dot vibrates horizontally (1s) and vertically (0.5s) |
| `SpinnerDotsWiper` | static side dots; two semicircles swing ±90° from left/right pivots |
| `SpinnerDotsCollapse` | four dots collapse inward in turn while a spread ring grows, then snap back |
| `SpinnerDotsFlip` | two columns of dots flip ±180° about the bottom pivot with a vertical bounce |
| `SpinnerDotsTurn` | two columns rotate −270° — left pivots at bottom, right at top (last 30% of cycle) |
| `SpinnerDotsCarousel` | two dot-pairs swap via rotate-translate-rotate while the row slides 37.5% |
| `SpinnerDotsHalfTurn` | two dot-pairs at edges and thirds; both rotate ±180° in the last 20% of the cycle |
| `SpinnerDotsSlideFlip` | two halves slide toward centre while flipping ±180° about their inner dot |
| `SpinnerDotsStaggerTurn` | two offset dot-pairs rotate 180° in alternation (0.5 s stagger) |
| `SpinnerDotsStretch` | two bars stretch (aspect 2.3→3.7) while bouncing ±8px in opposite directions |
| `SpinnerDotsMirrorStretch` | like Stretch plus translateX(±19px); loader mirror-flips vertically every 2 s |
| `SpinnerDotsPinch` | centre dot + 4 satellites pinch inward (box-shadow) while the group rotates 180° |
| `SpinnerDotsCorners` | four corner dots chase clockwise around a square (0.5 s per edge) |
| `SpinnerDotsNudgeRotate` | two halves nudge ±400%/14 (30–70%) while the loader rotates 180° (30–50%) |
| `SpinnerDotsUnfold` | four dots stack left then spread to 0/⅓/⅔/100%; mirror-flips every 1.5 s |
| `SpinnerDotsShuttle` | two fixed middle dots; end dots run a mirrored □ path (down → across → up) |
| `SpinnerDotsSpreadShuttle` | like Shuttle but shorter hop (400%/3); middle dots spread to edges (60–100%) |
| `SpinnerDotsTriad` | two 3-dot clusters spread vertically then swap columns (0↔⅓, ⅔↔1) |
| `SpinnerDotsSatellite` | static end dots; two centre dots orbit them (±1 turn, 0.5 s stagger) |
| `SpinnerDotsTrack` | static end dots; two middle dots follow translate(±37.5%)+rotate (2 s loop) |

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
