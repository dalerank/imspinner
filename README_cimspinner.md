## how to use imspinner in C 

1. Add [imspinner.h](imspinner.h), [cimspinner.h](cimspinner.h), [cimspinner_config.h](cimspinner_config.h), [cimspinner.cpp](cimspinner.cpp) to your project
1. Sample C code

   ```c
   #include "../imspinner/cimspinner.h"
   
   ImColor genColor(int i)  {
        ImColor col;
        ImColor_HSV(&col, (float)(i) * 0.25, 0.8, 0.8, 1.0);
        return col;
   }

   int main(int, char**) {
     ...
     while (!done) {
        ...
        {
            igBegin("ImSipinner in C", NULL, 0);

            static const ImColor red =  {.Value = {.x = 1.f, .y = 0.f, .z = 0.f, .w = 1.f}};
            static const ImColor blue = {.Value = {.x = 0.f, .y = 0.f, .z = 1.f, .w = 1.f}};

            /* These spinners are defined in cimspinner_conifg.h by defalult. */
            SpinnerDnaDotsEx(      "DnaDotV", 16, 2, red, 1.2, 8, 0.25f, true); igSameLine(0.0, -1.0); // SPINNER_DNADOTS
            SpinnerRainbowMix(     "Rmix",    16, 2, blue, 4);                  igSameLine(0.0, -1.0); // SPINNER_RAINBOWMIX
            SpinnerAng8(           "Ang",     16, 2);                           igSameLine(0.0, -1.0); // ...
            SpinnerPulsar(         "Pulsar",  16, 2);                           igSameLine(0.0, -1.0);
            SpinnerClock(          "Clock",   16, 2);                           igSameLine(0.0, -1.0);
            SpinnerAtom(           "atom",    16, 2);                           igSameLine(0.0, -1.0);
            SpinnerSwingDots(      "wheel",   16, 6);                           igSameLine(0.0, -1.0);
            SpinnerDotsToBar(      "tobar",   16, 2, 0.5);                      igSameLine(0.0, -1.0);
            SpinnerBarChartRainbow("rainbow", 16, 4, red, 4);                   igSameLine(0.0, -1.0);

            /* Add "-D SPINNER_CAMERA" to compilation option. */
            SpinnerCamera(         "Camera",  16, 8, genColor);

            igEnd();
        }
        ...
        demoSpinners(); /* Add "-D IMSPINNER_DEMO" to compilation option */

     }
   }
   ```
   

## License

[MIT](https://choosealicense.com/licenses/mit/)
