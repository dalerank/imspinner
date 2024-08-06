# imspinner
Set of nice spinners for imgui

![spinners](https://github.com/user-attachments/assets/abe00d7f-fdaf-4393-928b-032b06231217)

# how to test

1. add a little code that enable test mode
```c++
#define IMSPINNER_DEMO
#include "../imspinner/imspinner.h"

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
