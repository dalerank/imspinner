#ifndef _IMSPINNER_DEMO_H_
#define _IMSPINNER_DEMO_H_

/*
 * imspinner demo showcase
 *
 * Interactive gallery of every spinner (demoSpinners()). Include this header
 * to get the demo; it pulls in "imspinner.h" and the "imspinner_text.h" add-on
 * (the text spinner is shown in the grid). Call ImSpinner::demoSpinners() once
 * per frame inside your ImGui loop.
 */

#include "imspinner_text.h"
#include "imspinner_dots.h"

namespace ImSpinner
{
    inline void demoSpinners() {
      static int hue = 0;
      static float nextdot = 0, nextdot2;
      static bool show_number = false;
      static int view_mode = 0;   // 0 = graphical spinners, 1 = text animations

      nextdot -= 0.07f;

      static float velocity = 1.f;
      static float widget_size = 50.f;

      static int selected_idx = 0;
      static ImColor spinner_filling_meb_bg;

      constexpr int num_spinners = 333;

      static int cci = 0, last_cci = 0;
      static std::map<int, const char*> __nn; auto Name = [] (const char* v) { if (!__nn.count(cci)) { __nn[cci] = v; }; return __nn[cci]; };
      static std::map<int, float> __rr; auto R = [] (float v) { if (!__rr.count(cci)) { __rr[cci] = v; }; return __rr[cci]; };
      static std::map<int, float> __tt; auto T = [] (float v) { if (!__tt.count(cci)) { __tt[cci] = v; }; return __tt[cci];  };
      static std::map<int, ImColor> __cc; auto C = [] (ImColor v) { if (!__cc.count(cci)) { __cc[cci] = v; }; return __cc[cci];  };
      static std::map<int, ImColor> __cb; auto CB = [] (ImColor v) { if (!__cb.count(cci)) { __cb[cci] = v; }; return __cb[cci];  };
      static std::map<int, bool> __hc; auto HC = [] (bool v) { if (!__hc.count(cci)) { __hc[cci] = v; }; return __hc[cci];  };
      static std::map<int, bool> __hcb; auto HCB = [] (bool v) { if (!__hcb.count(cci)) { __hcb[cci] = v; }; return __hcb[cci];  };
      static std::map<int, float> __ss; auto S = [] (float v) { if (!__ss.count(cci)) { __ss[cci] = v; }; return __ss[cci];  };
      static std::map<int, float> __aa; auto A = [] (float v) { if (!__aa.count(cci)) { __aa[cci] = v; }; return __aa[cci];  };
      static std::map<int, float> __amn; auto AMN = [] (float v) { if (!__amn.count(cci)) { __amn[cci] = v; }; return __amn[cci];  };
      static std::map<int, float> __amx; auto AMX = [] (float v) { if (!__amx.count(cci)) { __amx[cci] = v; }; return __amx[cci];  };
      static std::map<int, int> __dt; auto DT = [] (int v) { if (!__dt.count(cci)) { __dt[cci] = v; }; return __dt[cci];  };
      static std::map<int, int> __mdt; auto MDT = [] (int v) { if (!__mdt.count(cci)) { __mdt[cci] = v; }; return __mdt[cci];  };
      static std::map<int, float> __dd; auto D = [] (float v) { if (!__dd.count(cci)) { __dd[cci] = v; }; return __dd[cci];  };
      static std::map<int, int> __mm; auto M = [] (float v) { if (!__mm.count(cci)) { __mm[cci] = v; }; return __mm[cci];  };
      static std::map<int, int> __mx; auto MX = [] (float v) { if (!__mx.count(cci)) { __mx[cci] = v; }; return __mx[cci];  };

      const auto draw_spinner = [&](int spinner_idx, float widget_size)
      {
        const ImVec2 curpos_begin = ImGui::GetCursorPos();

        ImGui::PushID(spinner_idx);
        {
          if (show_number) {
              ImGui::Text("%04u", spinner_idx);
          }

          const bool is_selected = (selected_idx == spinner_idx);
          if (ImGui::Selectable("", is_selected, 0, ImVec2(widget_size, widget_size))) {
            selected_idx = spinner_idx;
            last_cci = spinner_idx;
          }

          // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
          if (is_selected) {
              ImGui::SetItemDefaultFocus();
          }

          const float sp_radius = __rr.count(spinner_idx) ? __rr[spinner_idx] : 16.f;
          const float sp_offset = (widget_size - sp_radius * 2.f ) / 2.f;
          ImGui::SetCursorPos({curpos_begin.x + sp_offset, curpos_begin.y + sp_offset});

#define $(i) i: cci = i;
          switch (spinner_idx) {
          case $( 0) ImSpinner::Spinner<e_st_rainbow>   (Name("Spinner"),
                                                         Radius{R(16)}, Thickness{T(2)}, Color{ImColor::HSV(++hue * 0.005f, 0.8f, 0.8f)}, Speed{S(8) * velocity}, AngleMin{AMN(0.f)}, AngleMax{AMX(PI_2)}, Dots{DT(1)}, Mode{M(0)}); break;
          case $( 1) ImSpinner::Spinner<e_st_rainbow>   (Name("Spinner"),
                                                         Radius{R(16)}, Thickness{T(2)}, Color{ImColor::HSV(++hue * 0.005f, 0.8f, 0.8f)}, Speed{S(8) * velocity}, AngleMin{AMN(0.f)}, AngleMax{AMX(PI_2)}, Dots{DT(4)}, Mode{M(1)}); break;
          case $( 2) ImSpinner::Spinner<e_st_dots>      (Name("SpinnerDots"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, FloatPtr{&nextdot}, Speed{S(1) * velocity}, Dots{DT(12)}, MinThickness{-1.f}, Mode{M(0)}); break;
          case $( 3) ImSpinner::Spinner<e_st_dots>      (Name("SpinnerDots"),
                                                         Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, FloatPtr{&nextdot}, Speed{S(1) * velocity}, Dots{DT(12)}, MinThickness{-1.f}, Mode{M(1)}); break;
          case $( 4) ImSpinner::Spinner<e_st_ang>       (Name("SpinnerAng270"),
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(white)}, BgColor{CB(ImColor(255, 255, 255, 128))}, Speed{S(6) * velocity}, Angle{A(0.75f * PI_2)}, Mode{M(0)}); break;
          case $( 5) ImSpinner::Spinner<e_st_ang>       (Name("SpinnerAng270NoBg"),
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(white)}, BgColor{CB(ImColor(255, 255, 255, 0))}, Speed{S(6) * velocity}, Angle{A(0.75f * PI_2)}); break;
          case $( 6) ImSpinner::Spinner<e_st_vdots>     (Name("SpinnerVDots"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, BgColor{CB(ImColor::HSV(hue * 0.0011f, 0.8f, 0.8f))}, Speed{S(2.7f) * velocity}, Dots{DT(12)}, MiddleDots{MDT(6)}, Mode{M(0)}); break;
          case $( 7) ImSpinner::Spinner<e_st_bounce_ball>(Name("SpinnerBounceBall"),
                                                          Radius{R(16)}, Thickness{T(6)}, Color{C(white)}, Speed{S(4) * velocity}, Dots{DT(1)}, Mode{M(0)}); break;
          case $( 8) ImSpinner::Spinner<e_st_eclipse>   (Name("SpinnerAngEclipse"),
                                                          Radius{R(16)}, Thickness{T(5)}, Color{C(white)}, Speed{S(6) * velocity}); break;
          case $( 9) ImSpinner::Spinner<e_st_ingyang>   (Name("SpinnerIngYang"),
                                                          Radius{R(16)}, Thickness{T(5)}, Reverse{false}, Delta{D(0.f)}, Color{C(white)}, AltColor{ImColor(255, 0, 0)}, Speed{S(4) * velocity}, Angle{A(IM_PI * 0.8f)}); break;
          case $(10) ImSpinner::Spinner<e_st_barchartsine>(Name("SpinnerBarChartSine"),
                                                           Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, Speed{S(6.8f) * velocity}, Dots{DT(4)}, Mode{M(0)}); break;
          case $(11) ImSpinner::SpinnerBounceDots       (Name("SpinnerBounceDots"), R(16),
                                                          T(6), C(white), S(6) * velocity, DT(3), M(0)); break;
          case $(12) ImSpinner::SpinnerFadeDots         (Name("SpinnerFadeDots"), R(16),
                                                          T(6), C(white), S(8) * velocity, DT(8)); break;
          case $(13) ImSpinner::SpinnerScaleDots        (Name("SpinnerScaleDots"), R(16),
                                                          T(6), C(white), S(7) * velocity, DT(8)); break;
          case $(14) ImSpinner::SpinnerMovingDots       (Name("SpinnerMovingDots"), R(16),
                                                          T(6), C(white), S(30) * velocity, DT(3)); break;
          case $(15) ImSpinner::SpinnerRotateDots       (Name("SpinnerRotateDots"),
                                                          R(16), T(6), C(white), S(4) * velocity, DT(2), M(0)); break;
          case $(16) ImSpinner::SpinnerTwinAng          (Name("SpinnerTwinAng"),
                                                          R(16), 16, T(6), C(white), CB(ImColor(255, 0, 0)), S(4) * velocity, A(IM_PI), M(0)); break;
          case $(17) ImSpinner::SpinnerClock            (Name("SpinnerClock"),
                                                          R(16), T(2), C(ImColor(255, 0, 0)), CB(white), S(4) * velocity); break;
          case $(18) ImSpinner::SpinnerIngYang          (Name("SpinnerIngYangR"),
                                                          R(16), T(5), true, 0.1f, C(white), CB(ImColor(255, 0, 0)), S(4) * velocity, A(IM_PI * 0.8f), M(0)); break;
          case $(19) ImSpinner::SpinnerBarChartSine     (Name("SpinnerBarChartSine2"),
                                                          R(16), T(4), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(4.8f) * velocity, DT(4), M(4)); break;
          case $(20) ImSpinner::SpinnerTwinAng180       (Name("SpinnerTwinAng"),
                                                          R(16), 12, T(4), C(white), CB(ImColor(255, 0, 0)), S(4) * velocity, A(PI_DIV_4), M(0)); break;
          case $(21) ImSpinner::SpinnerTwinAng180       (Name("SpinnerTwinAng2"),
                                                          R(16), 12, T(4), C(white), CB(ImColor(255, 0, 0)), S(4) * velocity, A(IM_PI), M(1)); break;
          case $(22) ImSpinner::SpinnerIncDots          (Name("SpinnerIncDots"),
                                                          R(16), T(4), C(white), S(5.6f) * velocity, 6); break;
          case $(23) nextdot2 -= 0.2f * velocity;
                     ImSpinner::SpinnerDots             (Name("SpinnerDotsWoBg"),
                                                          &nextdot2, R(16), T(4), C(white), S(0.3f) * velocity, DT(12), A(0.f), M(0)); break;
          case $(24) ImSpinner::SpinnerIncScaleDots     (Name("SpinnerIncScaleDots"),
                                                          R(16), T(4), C(white), S(6.6f) * velocity, DT(6), A(0), M(0)); break;
          case $(25) ImSpinner::SpinnerAng              (Name("SpinnerAng90"),
                                                          R(16), T(6), C(white), CB(ImColor(255, 255, 255, 128)), S(8.f) * velocity, A(PI_DIV_2), M(0)); break;
          case $(26) ImSpinner::SpinnerAng              (Name("SpinnerAng90"),
                                                          R(16), 6, C(white), CB(ImColor(255, 255, 255, 0)), S(8.5f) * velocity, A(PI_DIV_2), M(0)); break;
          case $(27) ImSpinner::SpinnerFadeBars         (Name("SpinnerFadeBars"),
                                                          10, C(white), S(4.8f) * velocity, 3); break;
          case $(28) ImSpinner::SpinnerPulsar           (Name("SpinnerPulsar"),
                                                          R(16), T(2), C(white), S(1) * velocity, true, A(0), M(0)); break;
          case $(29) ImSpinner::SpinnerIngYang          (Name("SpinnerIngYangR2"),
                                                          R(16), T(5), true, 3.f, C(white), CB(ImColor(255, 0, 0)), S(4) * velocity, A(IM_PI * 0.8f)); break;
          case $(30) ImSpinner::SpinnerBarChartRainbow  (Name("SpinnerBarChartRainbow"),
                                                          R(16), T(4), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(6.8f) * velocity, DT(4), M(0)); break;
          case $(31) ImSpinner::SpinnerBarsRotateFade   (Name("SpinnerBarsRotateFade"),
                                                          8, 18, T(4), C(white), S(7.6f) * velocity, DT(6)); break;
          case $(32) ImSpinner::SpinnerFadeBars         (Name("SpinnerFadeScaleBars"),
                                                          10, C(white), S(6.8f) * velocity, 3, true); break;
          case $(33) ImSpinner::SpinnerBarsScaleMiddle  (Name("SpinnerBarsScaleMiddle"),
                                                          6, C(white), S(8.8f) * velocity, 3); break;
          case $(34) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin1"),
                                                          R(16), 13, T(2), C(ImColor(255, 0, 0)), CB(white), S(6) * velocity, A(PI_DIV_2), DT(1), M(0)); break;
          case $(35) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin2"),
                                                          13, 16, T(2), C(ImColor(255, 0, 0)), CB(white), S(6) * velocity, A(PI_DIV_2), DT(1), M(0)); break;
          case $(36) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin3"),
                                                          13, 16, T(2), C(ImColor(255, 0, 0)), CB(white), S(6) * velocity, A(PI_DIV_2), DT(2), M(0)); break;
          case $(37) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin4"),
                                                          R(16), 13, T(2), C(ImColor(255, 0, 0)), CB(white), S(6) * velocity, A(PI_DIV_2), DT(2), M(0)); break;
          case $(38) ImSpinner::SpinnerTwinPulsar       (Name("SpinnerTwinPulsar"),
                                                          R(16), T(2), C(white), S(0.5f) * velocity, DT(2), M(0)); break;
          case $(39) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin4"),
                                                          R(14), 13, T(3), C(ImColor(255, 0, 0)), CB(ImColor(0, 0, 0, 0)), S(5) * velocity, A(IM_PI / 1.5f), 2); break;
          case $(40) ImSpinner::SpinnerBlocks           (Name("SpinnerBlocks"),
                                                          R(16), T(7), C(ImColor(255, 255, 255, 30)), CB(ImColor::HSV(hue * 0.005f, 0.8f, 0.8f)), S(5) * velocity); break;
          case $(41) ImSpinner::SpinnerTwinBall         (Name("SpinnerTwinBall"),
                                                          R(16), 11, T(2), 2.5f, C(ImColor(255, 0, 0)), CB(white), S(6) * velocity, DT(2), M(0)); break;
          case $(42) ImSpinner::SpinnerTwinBall         (Name("SpinnerTwinBall2"),
                                                          R(15), 19, T(2), 2.f, C(ImColor(255, 0, 0)), CB(white), S(6) * velocity, DT(3), M(0)); break;
          case $(43) ImSpinner::SpinnerTwinBall         (Name("SpinnerTwinBall2"),
                                                          16, 16, T(2), 5.f, C(ImColor(255, 0, 0)), CB(white), S(5) * velocity, DT(1), M(0)); break;
          case $(44) ImSpinner::SpinnerAngTriple        (Name("SpinnerAngTriple"),
                                                          16, 13, 10, T(1.3f), C(white), ImColor(255, 0, 0), white, S(5) * velocity, A(1.5f * IM_PI)); break;
          case $(45) ImSpinner::SpinnerIncFullDots      (Name("SpinnerIncFullDots"),
                                                          R(16), T(4), C(white), S(5.6f) * velocity, 4); break;
          case $(46) ImSpinner::SpinnerGooeyBalls       (Name("SpinnerGooeyBalls"),
                                                          R(16), C(white), S(2.f) * velocity); break;
          case $(47) ImSpinner::SpinnerRotateGooeyBalls (Name("SpinnerRotateGooeyBalls2"),
                                                          R(16), T(5), C(white), S(6.f) * velocity, DT(2), M(0)); break;
          case $(48) ImSpinner::SpinnerRotateGooeyBalls (Name("SpinnerRotateGooeyBalls3"),
                                                          R(16), T(5), C(white), S(6.f) * velocity, DT(3), M(0)); break;
          case $(49) ImSpinner::SpinnerMoonLine         (Name("SpinnerMoonLine"),
                                                          R(16), T(3), C(ImColor(200, 80, 0)), ImColor(80, 80, 80), S(5) * velocity); break;
          case $(50) ImSpinner::SpinnerArcRotation      (Name("SpinnerArcRotation"),
                                                          R(13), T(5), C(white), S(3) * velocity, DT(4), M(0)); break;
          case $(51) ImSpinner::SpinnerFluid            (Name("SpinnerFluid"),
                                                          R(16), C(ImColor(0, 0, 255)), S(3.8f) * velocity, 4); break;
          case $(52) ImSpinner::SpinnerArcFade          (Name("SpinnerArcFade"),
                                                          R(13), T(5), C(white), S(3) * velocity, DT(4), M(0)); break;
          case $(53) ImSpinner::SpinnerFilling          (Name("SpinnerFilling"),
                                                          R(16), T(6), C(white), CB(ImColor(255, 0, 0)), S(4) * velocity); break;
          case $(54) ImSpinner::SpinnerTopup            (Name("SpinnerTopup"),
                                                          R(16), 12, C(ImColor(255, 0, 0)), ImColor(80, 80, 80), CB(white), S(1) * velocity);  break;
          case $(55) ImSpinner::SpinnerFadePulsar       (Name("SpinnerFadePulsar"),
                                                          R(16), C(white), S(1.5f) * velocity, DT(1), M(0));  break;
          case $(56) ImSpinner::SpinnerFadePulsar       (Name("SpinnerFadePulsar2"),
                                                          R(16), C(white), S(0.9f) * velocity, DT(2), M(0)); break;
          case $(57) ImSpinner::SpinnerPulsar           (Name("SpinnerPulsar"),
                                                          R(16), T(2), C(white), S(1) * velocity, false); break;
          case $(58) ImSpinner::SpinnerDoubleFadePulsar (Name("SpinnerDoubleFadePulsar"),
                                                          R(16), T(2), C(white), S(2) * velocity); break;
          case $(59) ImSpinner::SpinnerFilledArcFade    (Name("SpinnerFilledArcFade"),
                                                          R(16), C(white), S(4) * velocity, 4); break;
          case $(60) ImSpinner::SpinnerFilledArcFade    (Name("SpinnerFilledArcFade6"),
                                                          R(16), C(white), S(6) * velocity, 6); break;
          case $(61) ImSpinner::SpinnerFilledArcFade    (Name("SpinnerFilledArcFade6"),
                                                          R(16), C(white), S(8) * velocity, 12); break;
          case $(62) ImSpinner::SpinnerFilledArcColor   (Name("SpinnerFilledArcColor"),
                                                          R(16), C(ImColor(255, 0, 0)), CB(white), S(2.8f) * velocity, 4); break;
          case $(63) ImSpinner::SpinnerCircleDrop       (Name("SpinnerCircleDrop"),
                                                          R(16), T(1.5f), 4.f, C(ImColor(255, 0, 0)), CB(white), S(2.8f) * velocity, A(IM_PI)); break;
          case $(64) ImSpinner::SpinnerSurroundedIndicator(Name("SpinnerSurroundedIndicator"),
                                                          R(16), T(5), C(ImColor(0, 0, 0)), CB(white), S(7.8f) * velocity); break;
          case $(65) ImSpinner::SpinnerTrianglesSelector (Name("SpinnerTrianglesSelector"),
                                                          R(16), T(8), C(ImColor(0, 0, 0)), CB(white), S(4.8f) * velocity, 8); break;
          case $(66) ImSpinner::SpinnerFlowingGradient  (Name("SpinnerFlowingFradient"),
                                                          R(16), T(6), C(ImColor(200, 80, 0)), CB(ImColor(80, 80, 80)), S(5) * velocity, A(PI_2)); break;
          case $(67) ImSpinner::SpinnerRotateSegments   (Name("SpinnerRotateSegments"),
                                                          R(16), T(4), C(white), S(3) * velocity, DT(4), MDT(1), M(0)); break;
          case $(68) ImSpinner::SpinnerRotateSegments   (Name("SpinnerRotateSegments2"),
                                                          R(16), T(3), C(white), S(2.4f) * velocity, DT(4), MDT(2), M(0)); break;
          case $(69) ImSpinner::SpinnerRotateSegments   (Name("SpinnerRotateSegments3"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(4), MDT(3), M(0)); break;
          case $(70) ImSpinner::SpinnerLemniscate       (Name("SpinnerLemniscate"),
                                                          R(20), T(3), C(white), S(2.1f) * velocity, 3); break;
          case $(71) ImSpinner::SpinnerRotateGear       (Name("SpinnerRotateGear"),
                                                          R(16), T(6), C(white), S(2.1f) * velocity, 8); break;
          case $(72) ImSpinner::SpinnerRotatedAtom      (Name("SpinnerRotatedAtom"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(3), M(0)); break;
          case $(73) ImSpinner::SpinnerAtom             (Name("SpinnerAtom"),
                                                          R(16), T(2), C(white), S(4.1f) * velocity, 3); break;
          case $(74) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls"),
                                                          R(16), T(4), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(5), M(0), MDT(1), MX(0)); break;
          case $(75) ImSpinner::SpinnerCamera           (Name("SpinnerCamera"),
                                                          R(16), T(8), [] (int i) { return ImColor::HSV(i * 0.25f, 0.8f, 0.8f); }, S(4.8f) * velocity, DT(8), M(0)); break;
          case $(76) ImSpinner::SpinnerArcPolarFade     (Name("SpinnerArcPolarFade"),
                                                          R(16), C(white), S(6) * velocity, DT(6), M(0)); break;
          case $(77) ImSpinner::SpinnerArcPolarRadius   (Name("SpinnerArcPolarRadius"),
                                                          R(16), C(ImColor::HSV(0.25f, 0.8f, 0.8f)), S(6.f) * velocity, DT(6), M(0)); break;
          case $(78) ImSpinner::SpinnerCaleidoscope     (Name("SpinnerArcPolarPies"),
                                                          R(16), T(4), C(ImColor::HSV(0.25f, 0.8f, 0.8f)), S(2.6f) * velocity, DT(10), M(0)); break;
          case $(79) ImSpinner::SpinnerCaleidoscope     (Name("SpinnerArcPolarPies2"),
                                                          R(16), T(4), C(ImColor::HSV(0.35f, 0.8f, 0.8f)), S(3.2f) * velocity, 10, 1); break;
          case $(80) ImSpinner::SpinnerScaleBlocks      (Name("SpinnerScaleBlocks"),
                                                          R(16), T(8), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(5) * velocity); break;
          case $(81) ImSpinner::SpinnerRotateTriangles  (Name("SpinnerRotateTriangles"),
                                                          R(16), T(2), C(white), S(6.f) * velocity, DT(3), M(0)); break;
          case $(82) ImSpinner::SpinnerArcWedges        (Name("SpinnerArcWedges"),
                                                          R(16), C(ImColor::HSV(0.3f, 0.8f, 0.8f)), S(2.8f) * velocity, DT(4), M(0)); break;
          case $(83) ImSpinner::SpinnerScaleSquares     (Name("SpinnerScaleSquares"),
                                                          R(16), T(8), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(5) * velocity); break;
          case $(84) ImSpinner::SpinnerHboDots          (Name("SpinnerHboDots"), R(16),
                                                          T(4), C(white), 0.f, 0.f, S(1.1f) * velocity, DT(6), M(0)); break;
          case $(85) ImSpinner::SpinnerHboDots          (Name("SpinnerHboDots2"), R(16),
                                                          T(4), C(white), 0.1f, 0.5f, S(1.1f) * velocity, DT(6), M(0)); break;
          case $(86) ImSpinner::Spinner<e_st_bounce_ball>(Name("SpinnerBounceBall3"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, Speed{S(3.2f) * velocity}, Dots{DT(5)}); break;
          case $(87) ImSpinner::SpinnerBounceBall       (Name("SpinnerBounceBallShadow"),
                                                          R(16), T(4), C(white), S(2.2f) * velocity, DT(1), true); break;
          case $(88) ImSpinner::SpinnerBounceBall       (Name("SpinnerBounceBall5Shadow"),
                                                          R(16), T(4), C(white), S(3.6f) * velocity, DT(5), true); break;
          case $(89) ImSpinner::SpinnerSquareStrokeFade (Name("SpinnerSquareStrokeFade"),
                                                          R(13), T(5), C(white), S(3) * velocity); break;
          case $(90) ImSpinner::SpinnerSquareStrokeFill (Name("SpinnerSquareStrokeFill"),
                                                          R(13), T(5), C(white), S(3) * velocity); break;
          case $(91) ImSpinner::SpinnerSwingDots        (Name("SpinnerSwingDots"),
                                                          R(16), T(6), C(ImColor(255, 0, 0)), S(4.1f) * velocity); break;
          case $(92) ImSpinner::SpinnerRotateWheel      (Name("SpinnerRotateWheel"),
                                                          R(16), T(10), C(ImColor(255, 255, 0)), CB(white), S(2.1f) * velocity, 8); break;
          case $(93) ImSpinner::SpinnerWaveDots         (Name("SpinnerWaveDots"), R(16),
                                                          T(3), C(white), S(6) * velocity, DT(8)); break;
          case $(94) ImSpinner::SpinnerRotateShapes     (Name("SpinnerRotateShapes"),
                                                          R(16), T(2), C(white), S(6.f) * velocity, DT(4), MDT(4)); break;
          case $(95) ImSpinner::SpinnerSquareStrokeLoading(Name("SpinnerSquareStrokeLoanding"),
                                                          R(13), T(5), C(white), S(3) * velocity); break;
          case $(96) ImSpinner::SpinnerSinSquares       (Name("SpinnerSinSquares"),
                                                          R(16), T(2), C(white), S(1.f) * velocity, M(0)); break;
          case $(97) ImSpinner::SpinnerZipDots          (Name("SpinnerZipDots"), R(16),
                                                          T(3), C(white), S(6) * velocity, DT(5)); break;
          case $(98) ImSpinner::SpinnerDotsToBar        (Name("SpinnerDotsToBar"), R(16),
                                                          T(3), D(0.5f), C(ImColor::HSV(0.31f, 0.8f, 0.8f)), S(5) * velocity, DT(5)); break;
          case $(99) ImSpinner::SpinnerSineArcs         (Name("SpinnerSineArcs"), R(16),
                                                          T(1), C(white), S(3) * velocity);
          case $(100) ImSpinner::SpinnerTrianglesShift  (Name("SpinnerTrianglesShift"),
                                                          R(16), T(8), C(ImColor(0, 0, 0)), CB(white), S(1.8f) * velocity, DT(8)); break;
          case $(101) ImSpinner::SpinnerCircularLines   (Name("SpinnerCircularLines"),
                                                          R(16), C(white), S(1.5f) * velocity, DT(8), M(0));  break;
          case $(102) ImSpinner::SpinnerLoadingRing     (Name("SpinnerLoadingRing"),
                                                          R(16), T(6), C(red), CB(ImColor(255, 255, 255, 128)), S(1.f) * velocity, DT(5)); break;
          case $(103) ImSpinner::SpinnerPatternRings    (Name("SpinnerPatternRings"),
                                                          R(16), T(2), C(white), S(4.1f) * velocity, DT(3)); break;
          case $(104) ImSpinner::SpinnerPatternSphere   (Name("SpinnerPatternSphere"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(6)); break;
          case $(105) ImSpinner::SpinnerRingSynchronous (Name("SpinnerRingSnchronous"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(3)); break;
          case $(106) ImSpinner::SpinnerRingWatermarks  (Name("SpinnerRingWatermarks"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(3)); break;
          case $(107) ImSpinner::SpinnerFilledArcRing   (Name("SpinnerFilledArcRing"),
                                                          R(16), T(6), C(red), CB(white), S(2.8f) * velocity, DT(8)); break;
          case $(108) ImSpinner::SpinnerPointsShift     (Name("SpinnerPointsShift"),
                                                          R(16), T(3), C(ImColor(0, 0, 0)), CB(white), S(1.8f) * velocity, DT(10)); break;
          case $(109) ImSpinner::SpinnerCircularPoints  (Name("SpinnerCircularPoints"),
                                                          R(16), T(1.2f), C(white), S(10.f) * velocity, DT(7));  break;
          case $(110) ImSpinner::SpinnerCurvedCircle    (Name("SpinnerCurvedCircle"),
                                                          R(16), T(1.2f), C(white), S(1.f) * velocity, DT(3));  break;
          case $(111) ImSpinner::SpinnerModCircle       (Name("SpinnerModCirclre"),
                                                          R(16), T(1.2f), C(white), AMN(1.f), AMX(2.f), S(3.f) * velocity);  break;
          case $(112) ImSpinner::SpinnerModCircle       (Name("SpinnerModCirclre2"),
                                                          R(16), T(1.2f), C(white), AMN(1.11f), AMX(3.33f), S(3.f) * velocity);  break;
          case $(113) ImSpinner::SpinnerPatternEclipse  (Name("SpinnerPatternEclipse"),
                                                          R(16), T(2), C(white), S(4.1f) * velocity, DT(5), AMN(2.f), AMX(0.f)); break;
          case $(114) ImSpinner::SpinnerPatternEclipse  (Name("SpinnerPatternEclipse2"),
                                                          R(16), T(2), C(white), S(4.1f) * velocity, DT(9), AMN(4.f), AMX(1.f)); break;
          case $(115) ImSpinner::SpinnerMultiFadeDots   (Name("SpinnerMultiFadeDots"), R(16),
                                                          T(2), C(white), S(8) * velocity, DT(8)); break;
          case $(116) ImSpinner::SpinnerRainbowShot     (Name("SpinnerRainbowShot"),
                                                          R(16), T(4), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(5), M(0)); break;
          case $(117) ImSpinner::SpinnerSpiral          (Name("SpinnerSpiral"),
                                                          R(16), T(2), C(white), S(6) * velocity, DT(5)); break;
          case $(118) ImSpinner::SpinnerSpiralEye       (Name("SpinnerSpiralEye"),
                                                          R(16), T(1), C(white), S(3) * velocity); break;
          case $(119) ImSpinner::SpinnerWifiIndicator   (Name("SpinnerWifiIndicator"),
                                                          R(16), T(1.5f), C(ImColor(0, 0, 0)), CB(white), S(7.8f) * velocity, AMN(5.52f), DT(3)); break;
          case $(120) ImSpinner::SpinnerHboDots         (Name("SpinnerHboDots"), R(16),
                                                          T(2), C(white), 0.f, 0.f, S(1.1f) * velocity, DT(10)); break;
          case $(121) ImSpinner::SpinnerHboDots         (Name("SpinnerHboDots2"), R(16),
                                                          T(4), C(white), 0.1f, 0.5f, S(1.1f) * velocity, DT(2)); break;
          case $(122) ImSpinner::SpinnerHboDots         (Name("SpinnerHboDots4"), R(16),
                                                          T(4), C(white), 0.1f, 0.5f, S(1.1f) * velocity, DT(3)); break;
          case $(123) ImSpinner::SpinnerDnaDots         (Name("SpinnerDnaDotsH"), R(16),
                                                          T(3), C(white), S(2) * velocity, DT(8), D(0.25f)); break;
          case $(124) ImSpinner::SpinnerDnaDots         (Name("SpinnerDnaDotsV"), R(16),
                                                          T(3), C(white), S(2) * velocity, DT(8), D(0.25f), true); break;
          case $(125) ImSpinner::SpinnerRotateDots      (Name("SpinnerRotateDots2"),
                                                          R(16), T(6), C(white), S(4) * velocity, ImMax<int>(int(ImSin((float)ImGui::GetTime() * 0.5f) * 8), 3)); break;
          case $(126) ImSpinner::SpinnerSevenSegments   (Name("SpinnerSevenSegments"), "012345679ABCDEF",
                                                          R(16), T(2), C(white), S(4) * velocity); break;
          case $(127) ImSpinner::SpinnerSolarBalls      (Name("SpinnerSolarBalls"),
                                                          R(16), T(4), C(red), CB(white), S(5) * velocity, DT(4)); break;
          case $(128) ImSpinner::SpinnerSolarArcs       (Name("SpinnerSolarArcs"),
                                                          R(16), T(4), C(red), CB(white), S(5) * velocity, DT(4)); break;
          case $(129) ImSpinner::SpinnerRainbow         (Name("Spinner"),
                                                          R(16), T(2), ImColor::HSV(++hue * 0.005f, 0.8f, 0.8f), S(8) * velocity, AMN(0.f), AMX(PI_2), DT(3)); break;
          case $(130) ImSpinner::SpinnerRotatingHeart   (Name("SpinnerRotatedHeart"),
                                                          R(16), T(2), C(red), S(8) * velocity, AMN(0.f)); break;
          case $(131) ImSpinner::SpinnerSolarScaleBalls (Name("SpinnerSolarScaleBalls"),
                                                          R(16), T(1.3f), C(red), S(1) * velocity, DT(36)); break;
          case $(132) ImSpinner::SpinnerOrionDots       (Name("SpinnerOrionDots"),
                                                          R(16), T(1.3f), C(white), S(4) * velocity, DT(12)); break;
          case $(133) ImSpinner::SpinnerGalaxyDots      (Name("SpinnerGalaxyDots"),
                                                          R(16), T(1.3f), C(white), S(0.2f) * velocity, DT(6)); break;
          case $(134) ImSpinner::SpinnerAsciiSymbolPoints(Name("SpinnerAsciiSymbolPoints"), "012345679ABCDEF",
                                                          R(16), T(2), C(white), S(4) * velocity); break;
          case $(135) ImSpinner::SpinnerRainbowCircle   (Name("SpinnerRainbowCircle"),
                                                          R(16), T(4), C(ImColor::HSV(0.25f, 0.8f, 0.8f)), S(1) * velocity, DT(4)); break;
          case $(136) ImSpinner::SpinnerRainbowCircle   (Name("SpinnerRainbowCircle2"),
                                                          R(16), T(2), ImColor::HSV(hue * 0.001f, 0.8f, 0.8f), S(2) * velocity, DT(8), D(0)); break;
          case $(137) ImSpinner::Spinner<e_st_vdots>    (Name("SpinnerVDots2"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, BgColor{CB(ImColor::HSV(hue * 0.0011f, 0.8f, 0.8f))}, Speed{S(2.1f) * velocity}, Dots{DT(2)}, MiddleDots{6}); break;
          case $(138) ImSpinner::Spinner<e_st_vdots>    (Name("SpinnerVDots3"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, BgColor{CB(ImColor::HSV(hue * 0.0011f, 0.8f, 0.8f))}, Speed{S(2.9f) * velocity}, Dots{DT(3)}, MiddleDots{6}); break;
          case $(139) ImSpinner::SpinnerSquareRandomDots(Name("SpinnerSquareRandomDots"),
                                                          R(16), T(2.8f), C(ImColor(255, 255, 255, 30)), CB(ImColor::HSV(hue * 0.005f, 0.8f, 0.8f)), S(5) * velocity); break;
          case $(140) ImSpinner::SpinnerFluidPoints     (Name("SpinnerFluidPoints"),
                                                          R(16), T(2.8f), C(ImColor(0, 0, 255)), S(3.8f) * velocity, Dots{DT(4)}, D(0.45f)); break;
          case $(141) ImSpinner::SpinnerDotsLoading     (Name("SpinnerDotsLoading"),
                                                          R(16), T(4.f), C(white), CB(white), S(2.f) * velocity); break;
          case $(142) ImSpinner::SpinnerDotsToPoints    (Name("SpinnerDotsToPoints"), R(16),
                                                          T(3), D(0.5f), C(ImColor::HSV(0.31f, 0.8f, 0.8f)), S(1.8) * velocity, DT(5)); break;
          case $(143) ImSpinner::SpinnerThreeDots       (Name("SpinnerThreeDots"), R(16),
                                                          T(6), C(white), S(4) * velocity, DT(8)); break;
          case $(144) ImSpinner::Spinner4Caleidospcope  (Name("Spinner4Caleidospcope"), R(16),
                                                          T(6), ImColor::HSV(hue * 0.0031f, 0.8f, 0.8f), S(4) * velocity, DT(8)); break;
          case $(145) ImSpinner::SpinnerFiveDots        (Name("SpinnerSixDots"), R(16),
                                                          T(6), C(white), S(4) * velocity, DT(8)); break;
          case $(146) ImSpinner::SpinnerFillingMem      (Name("SpinnerFillingMem"),
                                                          R(16), T(6), ImColor::HSV(hue * 0.001f, 0.8f, 0.8f), spinner_filling_meb_bg, S(4) * velocity); break;
          case $(147) ImSpinner::SpinnerHerbertBalls    (Name("SpinnerHerbertBalls"),
                                                          R(16), T(2.3f), C(white), S(2.f) * velocity, DT(4)); break;
          case $(148) ImSpinner::SpinnerHerbertBalls3D  (Name("SpinnerHerbertBalls3D"),
                                                          R(16), T(3.f), C(white), S(1.4f) * velocity); break;
          case $(149) ImSpinner::SpinnerSquareLoading   (Name("SpinnerSquareLoanding"),
                                                          R(16), T(2), C(white), S(3) * velocity); break;
#ifdef _IMSPINNER_TEXT_H_
          case $(150) ImSpinner::SpinnerTextFading      (Name("SpinnerTextFading"), "Loading",
                                                          R(16), T(15), C(ImColor::HSV(hue * 0.0011f, 0.8f, 0.8f)), S(4) * velocity); break;
#endif
          case $(151) ImSpinner::SpinnerBarChartAdvSine (Name("SpinnerBarChartAdvSine"),
                                                          R(16), T(5), C(white), S(4.8f) * velocity, 0); break;
          case $(152) ImSpinner::SpinnerBarChartAdvSineFade(Name("SpinnerBarChartAdvSineFade"),
                                                          R(16), T(5), C(white), S(4.8f) * velocity, 0); break;
          case $(153) ImSpinner::SpinnerMovingArcs       (Name("SpinnerMovingArcs"),
                                                          R(16), T(4), C(white), S(2) * velocity, DT(4)); break;
          case $(154) ImSpinner::SpinnerFadeTris         (Name("SpinnerFadeTris"),
                                                          R(20), C(white), S(5.f) * velocity, DT(2), false, M(0)); break;
          case $(155) ImSpinner::SpinnerBounceDots       (Name("SpinnerBounceDots1"), R(16),
                                                          T(2.5), C(white), S(3) * velocity, DT(6), M(1)); break;
          case $(156) ImSpinner::SpinnerRotateDots       (Name("SpinnerRotateDots"),
                                                          R(16), T(2), C(white), S(4) * velocity, DT(16), 1); break;
          case $(157) ImSpinner::SpinnerTwinAng360       (Name("SpinnerTwinAng360"),
                                                          R(16), 11, T(2), C(white), CB(ImColor(255, 0, 0)), 2.4f, 2.1f, 1); break;
          case $(158) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin1"),
                                                          R(18), 13, T(2), C(ImColor(255, 0, 0)), CB(white), S(3) * velocity, A(1.3), DT(3), 1); break;
          case $(159) ImSpinner::SpinnerGooeyBalls       (Name("SpinnerGooeyBalls"),
                                                          R(16), C(white), S(2.f) * velocity, 1); break;
          case $(160) ImSpinner::SpinnerArcRotation      (Name("SpinnerArcRotation"),
                                                          R(13), T(2.5), C(white), S(3) * velocity, DT(15), 1); break;
          case $(161) ImSpinner::SpinnerAng              (Name("SpinnerAng90Gravity"),
                                                          R(16), T(1), C(white), CB(ImColor(255, 255, 255, 128)), S(8.f) * velocity, A(PI_DIV_2), M(1)); break;
          case $(162) ImSpinner::SpinnerAng              (Name("SpinnerAng90SinRad"),
                                                          R(16), T(1), C(white), CB(ImColor(255, 255, 255, 0)), S(8.f) * velocity, A(0.75f * PI_2), M(2)); break;
          case $(163) ImSpinner::SpinnerSquishSquare     (Name("SpinnerSquishSquare"),
                                                          R(16), C(white), S(8.f) * velocity); break;
          case $(164) ImSpinner::SpinnerPulsarBall       (Name("SpinnerBounceBall"),
                                                          R(16), T(2), C(white), S(4) * velocity, DT(1)); break;
          case $(165) ImSpinner::SpinnerRainbowMix       (Name("Spinner"),
                                                          R(16), T(2), ImColor::HSV(0.005f, 0.8f, 0.8f), S(8) * velocity, AMN(0.f), AMX(PI_2), DT(5), 1); break;
          case $(166) ImSpinner::SpinnerAngMix           (Name("SpinnerAngMix"),
                                                          R(16), T(1), C(white), S(8.f) * velocity, A(IM_PI), DT(4), 0); break;
          case $(167) ImSpinner::SpinnerAngMix           (Name("SpinnerAngMixGravity"),
                                                          R(16), T(1), C(white), S(8.f) * velocity, A(PI_DIV_2), DT(6), 1); break;
          case $(168) ImSpinner::SpinnerScaleBlocks      (Name("SpinnerScaleBlocks"),
                                                          R(16), T(8), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(5) * velocity, 1); break;
          case $(169) ImSpinner::SpinnerFadeDots         (Name("SpinnerFadeDots3"), R(16),
                                                          T(6), C(white), S(8) * velocity, DT(4), 1); break;
          case $(170) ImSpinner::SpinnerFadeDots         (Name("SpinnerFadeDots6"), R(16),
                                                          T(3), C(white), S(8) * velocity, DT(4), 1); break;
          case $(171) ImSpinner::SpinnerFadeDots         (Name("SpinnerFadeDots2"), R(16),
                                                          T(2), C(white), S(5) * velocity, DT(8)); break;
          case $(172) ImSpinner::SpinnerScaleDots        (Name("SpinnerScaleDots2"), R(16),
                                                          T(2), C(white), S(4) * velocity, DT(8)); break;
          case $(173) ImSpinner::Spinner3SmuggleDots     (Name("Spinner3SmuggleDots"), R(16),
                                                          T(3), C(white), S(4) * velocity, DT(8), D(0.25f), true); break;
          case $(174) ImSpinner::SpinnerSimpleArcFade    (Name("SpinnerSimpleArcFade"),
                                                          R(13), T(2), C(white), S(4) * velocity); break;
          case $(175) ImSpinner::SpinnerTwinHboDots      (Name("SpinnerTwinHboDots"), R(16),
                                                          T(4), C(white), 0.1f, 0.5f, S(1.1f) * velocity, DT(6), D(0.f)); break;
          case $(176) ImSpinner::SpinnerTwinHboDots      (Name("SpinnerTwinHboDots2"), R(16),
                                                          T(4), C(white), 0.1f, 0.5f, S(3.1f) * velocity, DT(3), D(-0.5f)); break;
          case $(177) ImSpinner::SpinnerThreeDotsStar    (Name("SpinnerThreeDotsStar"), R(16),
                                                          T(4), C(white), 0.1f, 0.5f, S(5.1f) * velocity, D(-0.2f)); break;
          case $(178) ImSpinner::SpinnerSquareSpins      (Name("SpinnerSquareSpins"), R(16),
                                                          T(6), C(white), S(2) * velocity); break;
          case $(179) ImSpinner::SpinnerMoonDots         (Name("SpinnerMoonDots"), R(16),
                                                          T(8), C(white), CB(ImColor(0, 0, 0)), S(1.1f) * velocity); break;
          case $(180) ImSpinner::SpinnerFilledArcFade    (Name("SpinnerFilledArcFade7"),
                                                          R(16), C(white), S(6) * velocity, DT(6), 1); break;
          case $(181) ImSpinner::SpinnerRotateSegmentsPulsar(Name("SpinnerRotateSegmentsPulsar"),
                                                          R(16), T(2), C(white), S(1.1f) * velocity, DT(4), MDT(2)); break;
          case $(182) ImSpinner::SpinnerRotateSegmentsPulsar(Name("SpinnerRotateSegmentsPulsar2"),
                                                          R(16), T(2), C(white), S(1.1f) * velocity, DT(1), MDT(3)); break;
          case $(183) ImSpinner::SpinnerRotateSegmentsPulsar(Name("SpinnerRotateSegmentsPulsar3"),
                                                          R(16), T(2), C(white), S(1.1f) * velocity, DT(3), MDT(3)); break;
          case $(184) ImSpinner::SpinnerPointsArcBounce  (Name("SpinnerPointsArcBounce"),
                                                          R(16), T(2), C(white), S(3) * velocity, DT(12), 1, 0.f); break;
          case $(185) ImSpinner::SpinnerSomeScaleDots    (Name("SpinnerSomeScaleDots0"),
                                                          R(16), T(4), C(white), S(5.6f) * velocity, 6, 0); break;
          case $(186) ImSpinner::SpinnerSomeScaleDots    (Name("SpinnerSomeScaleDots1"),
                                                          R(16), T(4), C(white), S(6.6f) * velocity, 6, 1); break;
          case $(187) ImSpinner::SpinnerPointsArcBounce  (Name("SpinnerPointsArcBounce2"),
                                                          R(16), T(2), C(white), S(3) * velocity, DT(12), 1, 0.5f); break;
          case $(188) ImSpinner::SpinnerPointsArcBounce  (Name("SpinnerPointsArcBounce3"),
                                                          R(16), T(2), C(white), S(3) * velocity, DT(12), 2, 0.3f); break;
          case $(189) ImSpinner::SpinnerPointsArcBounce  (Name("SpinnerPointsArcBounce4"),
                                                          R(16), T(2), C(white), S(3) * velocity, DT(12), 3, 0.3f); break;
          case $(190) ImSpinner::SpinnerTwinBlocks       (Name("SpinnerTwinBlocks"),
                                                          R(16), T(7), C(ImColor(255, 255, 255, 30)), CB(ImColor::HSV(hue * 0.005f, 0.8f, 0.8f)), S(5) * velocity); break;
          case $(191) ImSpinner::SpinnerAng              (Name("SpinnerAng90"),
                                                          R(16), T(4), C(white), CB(ImColor(255, 255, 255, 128)), S(8.f) * velocity, A(PI_DIV_2), M(3)); break;
          case $(192) ImSpinner::SpinnerSplineAng        (Name("SpinnerSplineAng90"),
                                                          R(16), T(2), C(white), CB(ImColor(255, 255, 255, 128)), S(8.f) * velocity, A(PI_DIV_2), M(0)); break;
          case $(193) ImSpinner::Spinner<e_st_ang>       (Name("SpinnerAngNoBg"),
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(white)}, BgColor{CB(ImColor(255, 255, 255, 0))}, Speed{S(6) * velocity}, Angle{A(IM_PI)}, Mode{M(0)}); break;
          case $(194) ImSpinner::SpinnerBounceDots       (Name("SpinnerBounceDots2"), R(16),
                                                          T(2.5), C(white), S(1) * velocity, DT(6), M(2)); break;
          case $(195) ImSpinner::SpinnerRotateDots       (Name("SpinnerRotateDots"),
                                                          R(16), T(1.3), C(white), S(4) * velocity, DT(6), M(0)); break;
          case $(196) ImSpinner::SpinnerRotateDots       (Name("SpinnerRotateDots"),
                                                          R(16), T(2.3), C(white), S(4) * velocity, DT(5), M(2)); break;
          case $(197) ImSpinner::SpinnerTwinAng360       (Name("SpinnerTwinAng360"),
                                                          R(16), 11, T(4), C(white), CB(ImColor(255, 0, 0)), S(4) * velocity); break;
          case $(198) ImSpinner::SpinnerDots             (Name("SpinnerDotsWoBg2"),
                                                          &nextdot2, R(16), T(4), C(white), S(0.3f) * velocity, DT(6), A(1.49f), M(0)); break;
          case $(199) ImSpinner::SpinnerDots             (Name("SpinnerDotsWoBg3"),
                                                          &nextdot2, R(16), T(4), C(white), S(0.3f) * velocity, DT(4), A(1.49f), M(1)); break;
          case $(200) ImSpinner::SpinnerIncScaleDots     (Name("SpinnerIncScaleDots2"),
                                                          R(16), T(4), C(white), S(6.6f) * velocity, DT(8), A(1.22), M(1)); break;
          case $(201) ImSpinner::SpinnerPulsar           (Name("SpinnerPulsar2"),
                                                          R(16), T(2), C(white), S(1) * velocity, true, A(PI_2), M(1)); break;
          case $(202) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin4"),
                                                          R(16), 13, T(2), C(ImColor(255, 0, 0)), CB(white), S(1.6f) * velocity, A(3.14), DT(1), M(2)); break;
          case $(203) ImSpinner::SpinnerAngTwin          (Name("SpinnerAngTwin5"),
                                                          R(14), 16, T(2), C(ImColor(255, 0, 0)), CB(white), S(0.8f) * velocity, A(1.57), DT(3), M(2)); break;
          case $(204) ImSpinner::Spinner<e_st_dots>      (Name("SpinnerDotsX3"),
                                                          Radius{R(16)}, Thickness{T(2.3)}, Color{C(white)}, FloatPtr{&nextdot}, Speed{S(1) * velocity}, Dots{DT(3)}, MinThickness{-1.f}, Mode{M(2)}); break;
          case $(205) ImSpinner::Spinner<e_st_dots>      (Name("SpinnerDotsX13"),
                                                          Radius{R(16)}, Thickness{T(2.3)}, Color{C(white)}, FloatPtr{&nextdot}, Speed{S(1) * velocity}, Dots{DT(13)}, MinThickness{-1.f}, Mode{M(2)}); break;
          case $(206) ImSpinner::Spinner<e_st_angle>     (Name("SpinnerAng"),
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(white)}, BgColor{CB(ImColor(255, 255, 255, 128))}, Speed{S(2.8) * velocity}, Angle{A(PI_DIV_2)}, Mode{M(4)}); break;
          case $(207) ImSpinner::SpinnerTwinAng180       (Name("SpinnerTwinAngX"),
                                                          R(16), 12, T(2), C(white), CB(ImColor(255, 0, 0)), S(0.5f) * velocity, A(PI_DIV_4), M(2)); break;
          case $(208) ImSpinner::SpinnerAng8             (Name("SpinnerAng8"),
                                                          R(21), T(2), C(white), CB(0), S(8.f) * velocity, A(PI_DIV_4 * 6), M(3), D(0.5f)); break;
          case $(209) ImSpinner::SpinnerAng8             (Name("SpinnerAng8.1"),
                                                          R(14.5), T(2.5), C(white), CB(0), S(4) * velocity, A(PI_DIV_4), M(4), D(0.5f)); break;
          case $(210) ImSpinner::SpinnerAng8             (Name("SpinnerAng8.2"),
                                                          R(12), T(2.5), C(white), CB(0), S(5) * velocity, A(5.0f), M(5), D(0.75f)); break;
          case $(211) ImSpinner::SpinnerAng8             (Name("SpinnerAng8.3"),
                                                          R(19), T(2.5), C(white), CB(0), S(5) * velocity, A(5.0f), M(0), D(0.70f)); break;
          case $(212) ImSpinner::SpinnerRotateDots       (Name("SpinnerRotateDots"),
                                                          R(16), T(3), C(white), S(4) * velocity, DT(4), M(3)); break;
          case $(213) ImSpinner::Spinner<e_st_dots>      (Name("SpinnerDots/3"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, FloatPtr{&nextdot}, Speed{S(1) * velocity}, Dots{DT(12)}, MinThickness{-1.f}, Mode{M(3)}); break;
          case $(214) ImSpinner::Spinner<e_st_dots>      (Name("SpinnerDots/4"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, FloatPtr{&nextdot}, Speed{S(1) * velocity}, Dots{DT(12)}, MinThickness{-1.f}, Mode{M(4)}); break;
          case $(215) ImSpinner::Spinner<e_st_vdots>     (Name("SpinnerVDots/1"),
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(white)}, BgColor{CB(ImColor::HSV(hue * 0.0011f, 0.8f, 0.8f))}, Speed{S(2.7f) * velocity}, Dots{DT(12)}, MiddleDots{MDT(7)}, Mode{M(1)}); break;
          case $(216) ImSpinner::SpinnerSinSquares       (Name("SpinnerSinSquares/1"),
                                                          R(16), T(2), C(white), S(1.f) * velocity, M(1)); break;
          case $(217) ImSpinner::SpinnerSinSquares       (Name("SpinnerSinSquares/2"),
                                                          R(16), T(2), C(white), S(1.f) * velocity, M(2)); break;
          case $(218) ImSpinner::SpinnerCamera           (Name("SpinnerCamera/1"),
                                                          R(16), T(8), [] (int i) { return ImColor::HSV(i * 0.25f, 0.8f, 0.8f); }, S(2.8f) * velocity, DT(4), M(1)); break;
          case $(219) ImSpinner::SpinnerCamera           (Name("SpinnerCamera/1"),
                                                          R(16), T(8), [] (int i) { return ImColor::HSV(i * 0.25f, 0.8f, 0.8f); }, S(1.8f) * velocity, DT(3), M(1)); break;
          case $(220) ImSpinner::SpinnerRotateSegments   (Name("SpinnerRotateSegments/1"),
                                                          R(16), T(1.4), C(white), S(3) * velocity, DT(1), MDT(4), M(1)); break;
          case $(221) ImSpinner::SpinnerRotateSegments   (Name("SpinnerRotateSegments/1"),
                                                          R(16), T(4), C(white), S(3) * velocity, DT(4), MDT(1), M(1)); break;
          case $(222) ImSpinner::Spinner<e_st_angle>     (Name("SpinnerAng"),
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(white)}, BgColor{CB(ImColor(255, 255, 255, 128))}, Speed{S(8) * velocity}, Angle{A(IM_PI)}, Mode{M(1)}); break;
          case $(223) ImSpinner::SpinnerArcRotation      (Name("SpinnerArcRotation/3"),
                                                          R(13), T(5), C(white), S(3) * velocity, DT(4), M(3)); break;
          case $(224) ImSpinner::SpinnerArcFade          (Name("SpinnerArcFade/2"),
                                                          R(13), T(5), C(white), S(3) * velocity, DT(4), M(1)); break;
          case $(225) ImSpinner::SpinnerIncScaleDots     (Name("SpinnerIncScaleDots"),
                                                          R(16), T(4), C(white), S(4.6f) * velocity, DT(16), A(1), M(5)); break;
          case $(226) ImSpinner::SpinnerTwinPulsar       (Name("SpinnerTwinPulsar/5"),
                                                          R(16), T(2), C(white), S(0.5f) * velocity, DT(5), M(5)); break;
          case $(227) ImSpinner::SpinnerTwinPulsar       (Name("SpinnerTwinPulsar/0"),
                                                          R(16), T(2), C(white), S(0.5f) * velocity, DT(5), M(0)); break;
          case $(228) ImSpinner::SpinnerCircularLines   (Name("SpinnerCircularLines/4"),
                                                          R(16), C(white), S(1.5f) * velocity, DT(16), M(4));  break;
          case $(229) ImSpinner::SpinnerRotatedAtom      (Name("SpinnerRotatedAtom/5"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(2), M(5)); break;
          case $(230) ImSpinner::SpinnerRotatedAtom      (Name("SpinnerRotatedAtom/2"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(2), M(1)); break;
          case $(231) ImSpinner::SpinnerRotatedAtom      (Name("SpinnerRotatedAtom/2"),
                                                          R(16), T(2), C(white), S(2.1f) * velocity, DT(3), M(5)); break;
          case $(232) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls/1"),
                                                          R(16), T(4), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(3), M(1), MDT(1)); break;
          case $(233) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls/5"),
                                                          R(16), T(4), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(4), M(5), MDT(1)); break;
          case $(234) ImSpinner::SpinnerPointsRoller     (Name("SpinnerPointsRoller"),
                                                          R(16), T(3.5), C(white), S(1.4) * velocity, DT(8), MDT(1), 1.f); break;
          case $(235) ImSpinner::SpinnerPointsRoller     (Name("SpinnerPointsRoller2"),
                                                          R(16), T(1), C(white), S(1.0) * velocity, DT(18), MDT(3), 1.f); break;
          case $(236) ImSpinner::SpinnerPointsRoller     (Name("SpinnerPointsRoller2"),
                                                          R(16), T(3), C(white), S(1.0) * velocity, DT(2), MDT(12), 1.f); break;
          case $(237) ImSpinner::SpinnerThickToSin       (Name("SpinnerThickToSin"),
                                                          R(16), T(2), C(white), S(3) * velocity, MDT(1), DT(8) , M(0)); break;
          case $(238) ImSpinner::SpinnerThickToSin       (Name("SpinnerThickToSin2"),
                                                          R(16), T(1), C(white), S(3) * velocity, MDT(1), DT(20), M(5)); break;
          case $(239) ImSpinner::SpinnerThickToSin       (Name("SpinnerThickToSin2"),
                                                          R(16), T(1), C(white), S(3) * velocity, MDT(1), DT(20), M(6)); break;
          case $(240) ImSpinner::SpinnerFadePulsarSquare (Name("SpinnerFadePulsarSquare"),
                                                          R(16), C(white), S(1.5f) * velocity, DT(5), M(0));  break;
          case $(241) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls1"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.4f) * velocity, DT(9), M(1), MDT(10), MX(1)); break;
          case $(242) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls2"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.9f) * velocity, DT(9), M(2), MDT(16), MX(1)); break;
          case $(243) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls3"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.9f) * velocity, DT(9), M(6), MDT(16), MX(1)); break;
          case $(244) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls4"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.9f) * velocity, DT(7), M(0), MDT(24), MX(3)); break;
          case $(245) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls5"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.9f) * velocity, DT(17), M(4), MDT(30), MX(4)); break;
          case $(246) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls6"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.9f) * velocity, DT(17), M(4), MDT(30), MX(5)); break;
          case $(247) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls7"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.9f) * velocity, DT(17), M(4), MDT(30), MX(6)); break;
          case $(248) ImSpinner::SpinnerRainbowBalls     (Name("SpinnerRainbowBalls7"),
                                                          R(16), T(1), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(0.9f) * velocity, DT(4), M(0), MDT(30), MX(8)); break;
          case $(249) ImSpinner::SpinnerRainbowShot      (Name("SpinnerRainbowShot2"),
                                                          R(16), T(2), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(30), M(1)); break;
          case $(250) ImSpinner::SpinnerRainbowShot      (Name("SpinnerRainbowShot3"),
                                                          R(16), T(2), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(30), M(3)); break;
          case $(251) ImSpinner::SpinnerRainbowShot      (Name("SpinnerRainbowShot2"),
                                                          R(16), T(2), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(30), M(5)); break;
          case $(252) ImSpinner::SpinnerRainbowShot      (Name("SpinnerRainbowShot2"),
                                                          R(16), T(2), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, DT(30), M(6)); break;
#ifdef _IMSPINNER_TEXT_H_
          case $(253) ImSpinner::SpinnerTextFade        (Name("SpinnerTextFade"),
                                                          R(30), C(white), S(1.f) * velocity, "Loading..."); break;
          case $(254) ImSpinner::SpinnerTextUnderline   (Name("SpinnerTextUnderline"),
                                                          R(30), C(white), S(0.5f) * velocity, T(3), "Loading..."); break;
          case $(255) ImSpinner::SpinnerTextUnderlineDots(Name("SpinnerTextUnderlineDots"),
                                                          R(30), C(white), S(0.5f) * velocity, T(3), DT(6), "Loading..."); break;
          case $(256) ImSpinner::SpinnerTextTyping      (Name("SpinnerTextTyping"),
                                                          R(30), C(white), S(1.f) * velocity, DT(3), "Loading..."); break;
          case $(257) ImSpinner::SpinnerTextTyping      (Name("SpinnerTextTypingFull"),
                                                          R(30), C(white), S(0.5f) * velocity, DT(10), "Loading..."); break;
          case $(258) ImSpinner::SpinnerTextScroll      (Name("SpinnerTextScroll"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(259) ImSpinner::SpinnerTextColorFill   (Name("SpinnerTextColorFill"),
                                                          R(30), C(ImColor(0xC0, 0x29, 0x42)), CB(white), S(0.5f) * velocity, M(0), "Loading..."); break;
          case $(260) ImSpinner::SpinnerTextColorFill   (Name("SpinnerTextColorHighlight"),
                                                          R(30), C(ImColor(0xC0, 0x29, 0x42)), CB(white), S(0.5f) * velocity, M(1), "Loading..."); break;
          case $(261) ImSpinner::SpinnerTextScrollColors(Name("SpinnerTextScrollColors"),
                                                          R(30), C(white), S(0.2f) * velocity, "Loading..."); break;
          case $(262) ImSpinner::SpinnerTextColorCycle  (Name("SpinnerTextColorCycle"),
                                                          R(30), C(white), S(0.2f) * velocity, "Loading..."); break;
          case $(263) ImSpinner::SpinnerTextBounce      (Name("SpinnerTextBounce"),
                                                          R(30), C(white), S(1.f) * velocity, "Loading..."); break;
          case $(264) ImSpinner::SpinnerTextSplit       (Name("SpinnerTextSplit"),
                                                          R(30), C(white), S(1.f) * velocity, M(0), "Loading..."); break;
          case $(265) ImSpinner::SpinnerTextSplit       (Name("SpinnerTextSplitDelay"),
                                                          R(30), C(white), S(0.5f) * velocity, M(1), "Loading..."); break;
          case $(266) ImSpinner::SpinnerTextUnderlineScroll(Name("SpinnerTextUnderlineScroll"),
                                                          R(30), C(white), S(0.66f) * velocity, T(3), "Loading..."); break;
          case $(267) ImSpinner::SpinnerTextRoll        (Name("SpinnerTextRoll"),
                                                          R(30), C(white), S(1.f) * velocity, "Loading..."); break;
          case $(268) ImSpinner::SpinnerTextColorful    (Name("SpinnerTextColorful"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(269) ImSpinner::SpinnerTextCascade     (Name("SpinnerTextCascade"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(270) ImSpinner::SpinnerTextConveyor    (Name("SpinnerTextConveyor"),
                                                          R(30), C(white), S(0.33f) * velocity, "Loading..."); break;
          case $(271) ImSpinner::SpinnerTextReveal      (Name("SpinnerTextReveal"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(272) ImSpinner::SpinnerTextWave        (Name("SpinnerTextWave"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(273) ImSpinner::SpinnerTextSweep       (Name("SpinnerTextSweep"),
                                                          R(30), C(white), CB(ImColor(0, 0, 0)), S(0.5f) * velocity, "Loading..."); break;
          case $(274) ImSpinner::SpinnerTextShine       (Name("SpinnerTextShine"),
                                                          R(30), C(white), CB(ImColor(0, 0, 0)), S(0.5f) * velocity, "Loading..."); break;
          case $(275) ImSpinner::SpinnerTextScrollSweep (Name("SpinnerTextScrollSweep"),
                                                          R(30), C(white), CB(ImColor(0, 0, 0)), S(0.5f) * velocity, "Loading..."); break;
          case $(276) ImSpinner::SpinnerTextSpotlight   (Name("SpinnerTextSpotlight"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(277) ImSpinner::SpinnerTextShake       (Name("SpinnerTextShake"),
                                                          R(30), C(white), S(1.f) * velocity, M(0), "Loading..."); break;
          case $(278) ImSpinner::SpinnerTextShake       (Name("SpinnerTextShakeSmooth"),
                                                          R(30), C(white), S(1.f) * velocity, M(1), "Loading..."); break;
          case $(279) ImSpinner::SpinnerTextFlip        (Name("SpinnerTextFlip"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(280) ImSpinner::SpinnerTextSpin        (Name("SpinnerTextSpin"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(281) ImSpinner::SpinnerTextTumble      (Name("SpinnerTextTumble"),
                                                          R(30), C(white), S(0.5f) * velocity, M(0), "Loading..."); break;
          case $(282) ImSpinner::SpinnerTextTumble      (Name("SpinnerTextTumblePersp"),
                                                          R(30), C(white), S(0.5f) * velocity, M(1), "Loading..."); break;
          case $(283) ImSpinner::SpinnerTextSwirl       (Name("SpinnerTextSwirl"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(284) ImSpinner::SpinnerTextRollWave    (Name("SpinnerTextRollWave"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(285) ImSpinner::SpinnerTextVibrate     (Name("SpinnerTextVibrate"),
                                                          R(30), C(white), S(1.f) * velocity, "Loading..."); break;
          case $(286) ImSpinner::SpinnerTextGlitch      (Name("SpinnerTextGlitch"),
                                                          R(30), S(1.f) * velocity, "Loading..."); break;
          case $(287) ImSpinner::SpinnerTextBlur        (Name("SpinnerTextBlur"),
                                                          R(30), C(white), S(1.f) * velocity, "Loading..."); break;
          case $(288) ImSpinner::SpinnerTextDropout     (Name("SpinnerTextDropout"),
                                                          R(30), C(white), S(1.f) * velocity, "Loading..."); break;
          case $(289) ImSpinner::SpinnerTextScanline    (Name("SpinnerTextScanline"),
                                                          R(30), C(white), S(1.f) * velocity, M(0), "Loading..."); break;
          case $(290) ImSpinner::SpinnerTextScanline    (Name("SpinnerTextScanlineV"),
                                                          R(30), C(white), S(1.f) * velocity, M(1), "Loading..."); break;
          case $(291) ImSpinner::SpinnerTextSquash      (Name("SpinnerTextSquash"),
                                                          R(30), C(white), S(1.f) * velocity, "Loading..."); break;
          case $(292) ImSpinner::SpinnerTextScramble    (Name("SpinnerTextScramble"),
                                                          R(30), C(white), S(1.f) * velocity); break;
          case $(293) ImSpinner::SpinnerTextDecode      (Name("SpinnerTextDecode"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
          case $(294) ImSpinner::SpinnerTextVanish      (Name("SpinnerTextVanish"),
                                                          R(30), C(white), S(0.5f) * velocity, "Loading..."); break;
#endif
          case $(295) ImSpinner::SpinnerDotsTyping      (Name("SpinnerDotsTyping"),
                                                          R(16), T(4), C(white), S(1.f) * velocity, DT(3)); break;
          case $(296) ImSpinner::SpinnerDotsStep        (Name("SpinnerDotsStep"),
                                                          R(16), T(4), C(white), S(1.f) * velocity, DT(3)); break;
          case $(297) ImSpinner::SpinnerDotsGather      (Name("SpinnerDotsGather"),
                                                          R(16), T(4), C(white), S(0.5f) * velocity, DT(3)); break;
          case $(298) ImSpinner::SpinnerDotsShift       (Name("SpinnerDotsShift"),
                                                          R(16), T(4), C(white), S(0.5f) * velocity); break;
          case $(299) ImSpinner::SpinnerDotsOrbit       (Name("SpinnerDotsOrbit"),
                                                          R(16), T(4), C(white), S(0.66f) * velocity); break;
          case $(300) ImSpinner::SpinnerDotsCircle      (Name("SpinnerDotsCircle"),
                                                          R(16), T(4), C(white), S(0.66f) * velocity); break;
          case $(301) ImSpinner::SpinnerDotsSquare      (Name("SpinnerDotsSquare"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(302) ImSpinner::SpinnerDotsShuffle     (Name("SpinnerDotsShuffle"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(303) ImSpinner::SpinnerDotsSplit       (Name("SpinnerDotsSplit"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(304) ImSpinner::SpinnerDotsLeader      (Name("SpinnerDotsLeader"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(305) ImSpinner::SpinnerDotsRolling     (Name("SpinnerDotsRolling"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(306) ImSpinner::SpinnerDotsTriangle    (Name("SpinnerDotsTriangle"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(307) ImSpinner::SpinnerDotsCascade     (Name("SpinnerDotsCascade"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(308) ImSpinner::SpinnerDotsSwap        (Name("SpinnerDotsSwap"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(309) ImSpinner::SpinnerDotsSpread      (Name("SpinnerDotsSpread"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(310) ImSpinner::SpinnerDotsTwin        (Name("SpinnerDotsTwin"),
                                                          R(16), T(4), C(white), S(0.5f) * velocity); break;
          case $(311) ImSpinner::SpinnerDotsHop         (Name("SpinnerDotsHop"),
                                                          R(16), T(4), C(white), S(1.f / 1.5f) * velocity); break;
          case $(312) ImSpinner::SpinnerDotsJiggle      (Name("SpinnerDotsJiggle"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(313) ImSpinner::SpinnerDotsVibrate     (Name("SpinnerDotsVibrate"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(314) ImSpinner::SpinnerDotsWiper       (Name("SpinnerDotsWiper"),
                                                          R(16), T(4), C(white), S(2.f) * velocity); break;
          case $(315) ImSpinner::SpinnerDotsCollapse    (Name("SpinnerDotsCollapse"),
                                                          R(16), T(4), C(white), S(0.5f) * velocity); break;
          case $(316) ImSpinner::SpinnerDotsFlip        (Name("SpinnerDotsFlip"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(317) ImSpinner::SpinnerDotsTurn        (Name("SpinnerDotsTurn"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(318) ImSpinner::SpinnerDotsCarousel    (Name("SpinnerDotsCarousel"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(319) ImSpinner::SpinnerDotsHalfTurn    (Name("SpinnerDotsHalfTurn"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(320) ImSpinner::SpinnerDotsSlideFlip    (Name("SpinnerDotsSlideFlip"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(321) ImSpinner::SpinnerDotsStaggerTurn  (Name("SpinnerDotsStaggerTurn"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(322) ImSpinner::SpinnerDotsStretch      (Name("SpinnerDotsStretch"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(323) ImSpinner::SpinnerDotsMirrorStretch(Name("SpinnerDotsMirrorStretch"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(324) ImSpinner::SpinnerDotsPinch        (Name("SpinnerDotsPinch"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(325) ImSpinner::SpinnerDotsCorners      (Name("SpinnerDotsCorners"),
                                                          R(16), T(4), C(white), S(2.f) * velocity); break;
          case $(326) ImSpinner::SpinnerDotsNudgeRotate  (Name("SpinnerDotsNudgeRotate"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(327) ImSpinner::SpinnerDotsUnfold        (Name("SpinnerDotsUnfold"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(328) ImSpinner::SpinnerDotsShuttle     (Name("SpinnerDotsShuttle"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(329) ImSpinner::SpinnerDotsSpreadShuttle(Name("SpinnerDotsSpreadShuttle"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(330) ImSpinner::SpinnerDotsTriad         (Name("SpinnerDotsTriad"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(331) ImSpinner::SpinnerDotsSatellite   (Name("SpinnerDotsSatellite"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          case $(332) ImSpinner::SpinnerDotsTrack       (Name("SpinnerDotsTrack"),
                                                          R(16), T(4), C(white), S(1.f) * velocity); break;
          }
#undef $
        }
        ImGui::PopID();
      };

      if( ImGui::BeginTable("Demo table", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV) )
      {
        ImGui::TableNextColumn(); // Grid
        {
          // Extra 'Child region' needed here, to make scrollable-area
          if(ImGui::BeginChild("Grid"))
          {
            ImGuiStyle& style = ImGui::GetStyle();

            // Store previous Item spacing & Window padding (to restore it later)
            const ImVec2 prevSpacing = style.ItemSpacing;
            const ImVec2 prevPadding = style.WindowPadding;

            // Set Item spacing & Window padding as zero
            style.ItemSpacing = style.WindowPadding = {0.f, 0.f};

            // -----------------------------------------------------------------
            // For drawing spinners used 'Row-wrap' layout, same as in
            // Dear ImGui Demo > Layout > Basic Horizontal Layout > Manual wrapping:
            //   https://github.com/ocornut/imgui/blob/1029f57b8aa9118d08413d1d8a6dd9d32cf0d5f1/imgui_demo.cpp#L2866-L2878

            const float region_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetColumnWidth();

            const ImVec2 item_size = ImVec2(widget_size, widget_size);

            // Text-based animation cells; everything else is a graphical spinner.
            static const int text_spinners[] = { 134, 150, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294 };
            static const int dot_spinners[] = { 2, 3, 6, 11, 12, 13, 14, 15, 22, 23, 24, 45, 84, 85, 91, 93, 97, 98, 115, 120, 121, 122, 123, 124, 125, 132, 133, 137, 138, 139, 141, 142, 143, 145, 155, 156, 169, 170, 171, 172, 173, 175, 176, 177, 179, 185, 186, 194, 195, 196, 198, 199, 200, 204, 205, 212, 213, 214, 215, 225, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332 };
            auto in_list = [](const int *arr, int n, int idx) { for (int k = 0; k < n; ++k) if (arr[k] == idx) return true; return false; };
            auto category = [&](int idx) {                   // 0 = spinners, 1 = text, 2 = dots
              if (in_list(text_spinners, (int)(sizeof(text_spinners) / sizeof(int)), idx)) return 1;
              if (in_list(dot_spinners, (int)(sizeof(dot_spinners) / sizeof(int)), idx)) return 2;
              return 0;
            };

            // Pick the cells matching the current view mode.
            std::vector<int> shown;
            shown.reserve(num_spinners);
            for (int i = 0; i < num_spinners; i++)
              if (category(i) == view_mode)
                shown.push_back(i);

            for(size_t j = 0; j < shown.size(); j++)
            {
              const int current_spi = shown[j];
              // BeginChild here needed to restrict item width&height by specific size
              if( ImGui::BeginChild(100 + current_spi, item_size, false, ImGuiWindowFlags_NoScrollbar) )
              {
                  draw_spinner(current_spi, widget_size);
              }
              ImGui::EndChild();

              // Show tooltip over spinner
              if( ImGui::IsItemHovered() )
              {
                //if( )
                ImGui::BeginTooltip();
                {
                  // Number
                  ImGui::TextDisabled("%04u", current_spi);

                  // Spinner name
                  if(__nn.count(current_spi)) {
                      ImGui::SameLine();
                      ImGui::Text(" - %s", __nn[current_spi] );
                  }

                  ImGui::EndTooltip();
                }
              }

              // -------------------------------------------------------------

              const float last_item_x2 = ImGui::GetItemRectMax().x;
              const float next_item_x2 = last_item_x2 + style.ItemSpacing.x + item_size.x; // Expected position if next item was on same line
              if ((j + 1 < shown.size()) && (next_item_x2 < region_visible_x2)) {
                ImGui::SameLine();
              }
            }

            // -----------------------------------------------------------------

            // Restore previous Item spacing & Window padding
            style.ItemSpacing   = prevSpacing;
            style.WindowPadding = prevPadding;
          }
          ImGui::EndChild();
        }

        // ---------------------------------------------------------------------

        ImGui::TableNextColumn(); // Options
        {
          ImGui::TextUnformatted("View");
          if (ImGui::RadioButton("Spinners", &view_mode, 0)) widget_size = 50.f;
          ImGui::SameLine();
          if (ImGui::RadioButton("WDots", &view_mode, 2)) widget_size = 50.f;
          ImGui::SameLine();
          if (ImGui::RadioButton("Text", &view_mode, 1)) widget_size = 75.f;
          ImGui::Separator();

          ImGui::SliderFloat("Velocity", &velocity, 0.0f, 10.0f, "velocity = %.2f");
          ImGui::Checkbox("Show Numbers", &show_number);
          ImGui::SliderFloat("Grid size", &widget_size, 0.0f, 100.0f, "size = %.2f");

          // -----------------------------------------------------------------
          // Spinner-related parameters

          constexpr ImGuiColorEditFlags COLOR_EDIT_FLAGS =
            ImGuiColorEditFlags_PickerHueWheel |
            ImGuiColorEditFlags_NoSidePreview  |
            ImGuiColorEditFlags_NoInputs       |
            ImGuiColorEditFlags_NoAlpha;

          if(__nn.count(last_cci)) ImGui::Separator();

          if (__rr.count(last_cci)) ImGui::SliderFloat("Radius", &__rr[last_cci], 0.0f, 100.0f, "radius = %.2f");
          if (__tt.count(last_cci)) ImGui::SliderFloat("Thickness", &__tt[last_cci], 0.0f, 100.0f, "thickness = %.2f");
          if (__cc.count(last_cci)) {
          ImGui::Checkbox("Change Color", &__hc[last_cci]);
          if (__hc[last_cci]) { __cc[last_cci] = ImColor::HSV(hue * 0.005f, 0.8f, 0.8f); }
          else {
              ImGui::SameLine(); ImGui::SetNextItemWidth(120);
              ImGui::ColorPicker3("##MyColor", (float *)&__cc[last_cci], COLOR_EDIT_FLAGS);
          }
          }
          if (__cb.count(last_cci)) {
              ImGui::Checkbox("Change Bg Color", &__hcb[last_cci]);
              if (__hcb[last_cci]) { __cb[last_cci] = ImColor::HSV(hue * 0.008f, 0.8f, 0.8f); }
              else {
                  ImGui::SameLine(); ImGui::SetNextItemWidth(120);
                  ImGui::ColorPicker3("##MyBgColor", (float *)&__cb[last_cci], COLOR_EDIT_FLAGS);
              }
          }
          if (__ss.count(last_cci)) ImGui::SliderFloat("Speed", &__ss[last_cci], 0.0f, 100.0f, "speed = %.2f");
          if (__aa.count(last_cci)) ImGui::SliderFloat("Angle", &__aa[last_cci], 0.0f, PI_2, "angle = %.2f");
          if (__amn.count(last_cci)) ImGui::SliderFloat("Angle Min", &__amn[last_cci], 0.0f, PI_2, "angle min = %.2f");
          if (__amx.count(last_cci)) ImGui::SliderFloat("Angle Max", &__amx[last_cci], 0.0f, PI_2, "angle max = %.2f");
          if (__dt.count(last_cci)) ImGui::SliderInt("Dots", &__dt[last_cci], 1, 100, "dots = %u");
          if (__mdt.count(last_cci)) ImGui::SliderInt("MidDots", &__mdt[last_cci], 1, 100, "mid dots = %u");
          if (__dd.count(last_cci)) ImGui::SliderFloat("Delta", &__dd[last_cci], -1.f, 1.f, "delta = %f");
          if (__mm.count(last_cci)) ImGui::SliderInt("Mode", &__mm[last_cci], 0, 8, "mode = %d");
          if (__mx.count(last_cci)) ImGui::SliderInt("Ext", &__mx[last_cci], 0, 8, "ext = %d");
        }

        ImGui::EndTable();
      }
    }
}

#endif // _IMSPINNER_DEMO_H_
