# imspinner
Set of nice spinners for imgui

![spinners](https://github.com/user-attachments/assets/796d734b-eb2b-4d6f-9199-e4609a57935d)

<img width="913" height="177" alt="texts" src="https://github.com/user-attachments/assets/90374d7f-6282-44e7-97c3-1e3d681fe8bb" />

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

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

## License

[MIT](https://choosealicense.com/licenses/mit/)
