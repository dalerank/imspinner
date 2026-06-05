#ifndef _IMSPINNER_BARS_H_
#define _IMSPINNER_BARS_H_

/*
 * imspinner bars add-on
 *
 * Bar-chart and fading-bar spinners moved out of imspinner.h. Include this
 * header (it pulls in "imspinner.h"). SpinnerBarChartSine is wired into
 * Spinner<> via imspinner_compat.h (e_st_barchartsine).
 */

#ifndef _IMSPINNER_BARS_INTERNAL_
#include "imspinner.h"
namespace ImSpinner
{
#endif

    inline void SpinnerFadeBars(const char *label, float w, const ImColor &color = white, float speed = 2.8f, size_t bars = 3, bool scale = false)
    {
      float radius = (w * 0.5f) * bars;
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const float nextItemKoeff = 1.5f;
      const float yOffsetKoeftt = 0.8f;
      const float heightSpeed = 0.8f;
      const float start = (float)ImGui::GetTime() * speed;

      const float offset = IM_PI / bars;
      for (size_t i = 0; i < bars; i++)
      {
        float a = start + (IM_PI - i * offset);
        ImColor c = color_alpha(color, ImMax(0.1f, ImSin(a * heightSpeed)));
        float h = (scale ? (0.6f + 0.4f * c.Value.w) : 1.f) * size.y / 2;
        window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + i * (w * nextItemKoeff) - w / 2, centre.y - h * yOffsetKoeftt),
                                        ImVec2(pos.x + style.FramePadding.x + i * (w * nextItemKoeff) + w / 2, centre.y + h * yOffsetKoeftt), c);
      }
    }
    inline void SpinnerBarsRotateFade(const char *label, float rmin, float rmax , float thickness, const ImColor &color = white, float speed = 2.8f, size_t bars = 6)
    {
      float radius = rmax;
      SPINNER_HEADER(pos, size, centre, num_segments);

      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI / bars);
      start -= astart;
      const float bg_angle_offset = IM_PI / bars;
      bars = ImMin<size_t>(bars, 32);

      for (size_t i = 0; i <= bars; i++)
      {
        float a = start + (i * bg_angle_offset);
        ImColor c = color_alpha(color, ImMax(0.1f, i / (float)bars));
        window->DrawList->AddLine(ImVec2(centre.x + ImCos(a) * rmin, centre.y + ImSin(a) * rmin), ImVec2(centre.x + ImCos(a) * rmax, centre.y + ImSin(a) * rmax), c, thickness);
      }
    }
    inline void SpinnerBarsScaleMiddle(const char *label, float w, const ImColor &color = white, float speed = 2.8f, size_t bars = 3)
    {
      float radius = (w) * bars;
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const float nextItemKoeff = 1.5f;
      const float yOffsetKoeftt = 0.8f;
      const float heightSpeed = 0.8f;
      float start = (float)ImGui::GetTime() * speed;
      const float offset = IM_PI / bars;

      for (size_t i = 0; i < bars; i++)
      {
        float a = start + (IM_PI - i * offset);
        float h = (0.4f + 0.6f * ImMax(0.1f, ImSin(a * heightSpeed))) * (size.y / 2);
        window->DrawList->AddRectFilled(ImVec2(centre.x + style.FramePadding.x + i * (w * nextItemKoeff) - w / 2, centre.y - h * yOffsetKoeftt),
                                        ImVec2(centre.x + style.FramePadding.x + i * (w * nextItemKoeff) + w / 2, centre.y + h * yOffsetKoeftt), color_alpha(color, 1.f));
        if (i == 0)
          continue;

        window->DrawList->AddRectFilled(ImVec2(centre.x + style.FramePadding.x - i * (w * nextItemKoeff) - w / 2, centre.y - h * yOffsetKoeftt),
                                        ImVec2(centre.x + style.FramePadding.x - i * (w * nextItemKoeff) + w / 2, centre.y + h * yOffsetKoeftt), color_alpha(color, 1.f));
      }
    }
    inline void SpinnerBarChartSine(const char *label, float radius, float thickness, const ImColor &color, float speed, int bars = 5, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const ImGuiStyle &style = GImGui->Style;
      const float nextItemKoeff = 1.5f;
      const float yOffsetKoeftt = 0.8f;
      const float heightSpeed = 0.8f;

      const float start = (float)ImGui::GetTime() * speed;
      const float offset = IM_PI / bars;
      for (int i = 0; i < bars; i++)
      {
        const float ab = ease((ease_mode)mode, start + i * PI_DIV(2) / num_segments, IM_PI, 1.0f, 0.0f);
        const float angle = ImMax(PI_DIV_2, (1.f - i/(float)bars) * IM_PI);
        float a = start + ab + (IM_PI - i * offset);
        ImColor c = color_alpha(color, ImMax(0.1f, ImSin(a * heightSpeed)));
        float h = mode ? ImSin(a) * size.y / 2.f
                       : (0.6f + 0.4f * c.Value.w) * size.y;
        float halfs = mode ? 0 : size.y / 2.f;
        window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) - thickness / 2, centre.y + halfs),
                                        ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) + thickness / 2, centre.y + halfs - h * yOffsetKoeftt),
                                        c);
      }
    }

    inline void SpinnerBarChartAdvSine(const char *label, float radius, float thickness, const ImColor &color, float speed, int mode = 0)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 1.5f;
        const float start = (float)ImGui::GetTime() * speed;
        const int bars = radius * 2 / thickness;
        const float offset = PI_DIV_2 / bars;
        for (int i = 0; i < bars; i++)
        {
            float a = start + (PI_DIV_2 - i * offset);
            float halfsx = thickness * ImSin(a);
            float halfsy = (ImMax(0.1f, ImSin(a) + 1.f)) * radius * 0.5f;
            window->DrawList->AddRectFilled(ImVec2(pos.x + i * (thickness * nextItemKoeff) - thickness / 2 + halfsx, centre.y + halfsy),
                                            ImVec2(pos.x + i * (thickness * nextItemKoeff) + thickness / 2 + halfsx, centre.y - halfsy),
                                            color);
        }
    }

    inline void SpinnerBarChartAdvSineFade(const char *label, float radius, float thickness, const ImColor &color, float speed, int mode = 0)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        const int bars = radius * 2 / thickness;
        const float offset = PI_DIV_2 / bars;
        for (int i = 0; i < bars; i++)
        {
            float a = start - i * offset;
            float halfsy = ImMax(0.1f, ImCos(a) + 1.f) * radius * 0.5f;
            window->DrawList->AddRectFilled(ImVec2(pos.x + i * thickness - thickness / 2, centre.y + halfsy),
                                            ImVec2(pos.x + i * thickness + thickness / 2, centre.y - halfsy),
                                            color_alpha(color, ImMax(0.1f, halfsy / radius)));
        }
    }

    inline void SpinnerBarChartRainbow(const char *label, float radius, float thickness, const ImColor &color, float speed, int bars = 5, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const ImGuiStyle &style = GImGui->Style;
      const float nextItemKoeff = 1.5f;
      const float yOffsetKoeftt = 0.8f;

      const float start = (float)ImGui::GetTime() * speed;
      const float hspeed = 0.1f + ImSin((float)ImGui::GetTime() * 0.1f) * 0.05f;
      constexpr float rkoeff[6] = {4.f, 13.f, 3.4f, 8.7f, 25.f, 11.f};
      float out_h, out_s, out_v;
      ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
      for (int i = 0; i < bars; i++)
      {
        ImColor c = ImColor::HSV(out_h + i * 0.1f, out_s, out_v);
        float h = (0.6f + 0.4f * ImSin(start + (1.f + rkoeff[i % 6] * i * hspeed)) ) * size.y;
        h += ease((ease_mode)mode, start, radius);
        window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) - thickness / 2, centre.y + size.y / 2.f),
                                        ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) + thickness / 2, centre.y + size.y / 2.f - h * yOffsetKoeftt),
                                        color_alpha(c, 1.f));
      }
    }
    inline void SpinnerFluid(const char *label, float radius, const ImColor &color, float speed, int bars = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const ImGuiStyle &style = GImGui->Style;
      const float hspeed = 0.1f + ImSin((float)ImGui::GetTime() * 0.1f) * 0.05f;
      constexpr float rkoeff[6][3] = {{0.15f, 0.1f, 0.1f}, {0.033f, 0.15f, 0.8f}, {0.017f, 0.25f, 0.6f}, {0.037f, 0.1f, 0.4f}, {0.25f, 0.1f, 0.3f}, {0.11f, 0.1f, 0.2f}};
      const float j_k = radius * 2.f / num_segments;
      float out_h, out_s, out_v;
      ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
      for (int i = 0; i < bars; i++)
      {
        ImColor c = color_alpha(ImColor::HSV(out_h - i * 0.1f, out_s, out_v), rkoeff[i % 6][1]);
        for (int j = 0; j < num_segments; ++j) {
          float h = (0.6f + 0.3f * ImSin((float)ImGui::GetTime() * (speed * rkoeff[i % 6][2] * 2.f) + (2.f * rkoeff[i % 6][0] * j * j_k))) * (radius * 2.f * rkoeff[i % 6][2]);
          window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + j * j_k, centre.y + size.y / 2.f),
                                          ImVec2(pos.x + style.FramePadding.x + (j + 1) * (j_k), centre.y + size.y / 2.f - h),
                                          c);
        }
      }
    }

    inline void SpinnerFluidPoints(const char *label, float radius, float thickness, const ImColor &color, float speed, size_t dots = 6, float delta = 0.35f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const ImGuiStyle &style = GImGui->Style;
        const float rkoeff[3] = {0.033f, 0.3f, 0.8f};
        const float hspeed = 0.1f + ImSin((float)ImGui::GetTime() * 0.1f) * 0.05f;
        const float j_k = radius * 2.f / num_segments;

        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        for (int j = 0; j < num_segments; ++j) {
            float h = (0.6f + delta * ImSin((float)ImGui::GetTime() * (speed * rkoeff[2] * 2.f) + (2.f * rkoeff[0] * j * j_k))) * (radius * 2.f * rkoeff[2]);
            for (int i = 0; i < dots; i++) {
                ImColor c = color_alpha(ImColor::HSV(out_h - i * 0.1f, out_s, out_v), 1.f);
                window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x + j * j_k, centre.y + size.y / 2.f - (h / dots) * i), thickness, c);
            }
        }
    }

    inline float bars_kf_eval(const float *kt, const float *kv, int n, float t)
    {
      if (t <= kt[0]) return kv[0];
      for (int i = 0; i < n - 1; i++) {
        if (t <= kt[i + 1]) {
          const float dt = kt[i + 1] - kt[i];
          const float u = dt > 0.f ? (t - kt[i]) / dt : 0.f;
          return kv[i] + (kv[i + 1] - kv[i]) * u;
        }
      }
      return kv[n - 1];
    }

    inline void bars_draw_v(ImDrawList *dl, float cx, float top, float bot, float hw, const ImColor &c)
    {
      dl->AddRectFilled(ImVec2(cx - hw, top), ImVec2(cx + hw, bot), c);
    }

    inline void bars_draw_box(ImDrawList *dl, float left, float top, float W, float H,
                              float ax, float ay, float wf, float hf, const ImColor &c)
    {
      const float bw = wf * W, bh = hf * H;
      float x0, x1;
      if (ax <= 0.f)      { x0 = left; x1 = left + bw; }
      else if (ax >= 1.f) { x1 = left + W; x0 = x1 - bw; }
      else                { x0 = left + (W - bw) * 0.5f; x1 = x0 + bw; }
      float y0, y1;
      if (ay <= 0.f)      { y0 = top; y1 = top + bh; }
      else if (ay >= 1.f) { y1 = top + H; y0 = y1 - bh; }
      else                { y0 = top + (H - bh) * 0.5f; y1 = y0 + bh; }
      dl->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), c);
    }

    inline void bars_draw_six_cols(float left, float W, float top, float H, float hf,
                                   const ImColor &c, ImDrawList *dl)
    {
      static const float ax[] = { 0.f, 0.f, 0.5f, 0.5f, 1.f, 1.f };
      static const float ay[] = { 0.f, 1.f, 0.f, 1.f, 0.f, 1.f };
      for (int i = 0; i < 6; i++)
        bars_draw_box(dl, left, top, W, H, ax[i], ay[i], 0.2f, hf, c);
    }

    inline void bars_draw_conic_tile(ImDrawList *dl, float ox, float oy, float s, const ImColor &c, int segs)
    {
      const ImVec2 c0(ox + s * 0.5f, oy + s * 0.5f);
      dl->PathArcTo(c0, s * 0.5f, -IM_PI * 0.5f, 0.f, segs);
      dl->PathLineTo(c0);
      dl->PathFillConvex(c);
    }

    inline void bars_draw_row_stripes(ImDrawList *dl, float left, float y, float W, float rh, float px,
                                      float period, float bw, const ImColor &c)
    {
      for (float x = left + px - period * 3.f; x < left + W + period; x += period) {
        if (x + bw <= left || x >= left + W) continue;
        dl->AddRectFilled(ImVec2(ImMax(x, left), y), ImVec2(ImMin(x + bw, left + W), y + rh), c);
      }
    }

    inline float bars_row_px_cascade(int row, float t, float W)
    {
      static const float kt[] = { 0.f, 0.125f, 0.25f, 0.375f, 0.45f, 0.55f, 0.625f, 0.75f, 0.875f, 1.f };
      static const float px0[] = { -1.f, -1.f,  0.f,  0.f,  0.f,  0.f,  0.f,  1.f,  1.f,  1.f };
      static const float px1[] = { -1.f, -1.f, -1.f,  0.f,  0.f,  0.f,  0.f,  0.f,  1.f,  1.f };
      static const float px2[] = { -1.f, -1.f, -1.f, -1.f,  0.f,  0.f,  0.f,  0.f,  0.f,  1.f };
      static const float px3[] = { -1.f, -1.f, -1.f, -1.f,  0.f,  0.f,  0.f,  0.f,  1.f,  1.f };
      const float *pxv = (row == 0) ? px0 : (row == 1) ? px1 : (row == 2) ? px2 : px3;
      return bars_kf_eval(kt, pxv, 10, t) * W;
    }

    inline float bars_rows4_rh(float H) { return H * 0.25f; }

    inline void bars_draw_rows4_h(ImDrawList *dl, float left, float top, float W, float H,
                                  float period, float bw, float t, const ImColor &c)
    {
      const float rh = bars_rows4_rh(H);
      for (int r = 0; r < 4; r++)
        bars_draw_row_stripes(dl, left, top + (float)r * rh, W, rh,
                              bars_row_px_cascade(r, t, W), period, bw, c);
    }

    inline float bars_row_py_cascade(int row, float t, float H)
    {
      static const float kt[] = { 0.f, 0.125f, 0.25f, 0.375f, 0.45f, 0.5f, 0.625f, 0.75f, 0.875f, 1.f };
      static const float py0[] = { -1.f, -1.f, -1.f, -1.f,  0.f,  0.f,  0.f,  0.f,  1.f,  1.f };
      static const float py1[] = { -1.f, -1.f, -1.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  1.f };
      static const float py2[] = { -1.f, -1.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f };
      static const float py3[] = { -1.f,  0.f,  0.f,  0.f,  0.f,  0.f,  0.f,  1.f,  1.f,  1.f };
      const float *pyv = (row == 0) ? py0 : (row == 1) ? py1 : (row == 2) ? py2 : py3;
      return bars_kf_eval(kt, pyv, 10, t) * H;
    }

    inline void bars_draw_grid3(ImDrawList *dl, ImVec2 centre, float cell, float dot_r,
                               const ImColor &c, const uint16_t vis, const float *spread)
    {
      for (int gy = -1; gy <= 1; gy++)
        for (int gx = -1; gx <= 1; gx++) {
          const int i = (gy + 1) * 3 + (gx + 1);
          if (!(vis & (1 << i))) continue;
          const float sr = dot_r + (spread ? spread[i] : 0.f);
          dl->AddCircleFilled(ImVec2(centre.x + gx * cell, centre.y + gy * cell), sr, c, 8);
        }
    }

    // Sequential height pulse:
    //   three bars (20% width); each shrinks to 10% height in turn (33/50/66%).
    inline void SpinnerBarsSeqPulse(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.33f, 0.5f, 0.66f, 1.f };
      static const float h0[] = { 1.f, 0.1f, 1.f, 1.f, 1.f };
      static const float h1[] = { 1.f, 1.f, 0.1f, 1.f, 1.f };
      static const float h2[] = { 1.f, 1.f, 1.f, 0.1f, 1.f };
      const float hs[] = { bars_kf_eval(kt, h0, 5, t), bars_kf_eval(kt, h1, 5, t), bars_kf_eval(kt, h2, 5, t) };
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float hh = hs[i] * halfH;
        bars_draw_v(window->DrawList, cx[i], centre.y - hh, centre.y + hh, hw, c);
      }
    }

    // Cascade grow:
    //   three bottom-anchored bars grow in sequence (60→80→100% cascade).
    inline void SpinnerBarsCascadeGrow(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.2f, 0.4f, 0.6f, 0.8f, 1.f };
      static const float h0[] = { 1.f, 0.6f, 0.8f, 1.f, 1.f, 1.f };
      static const float h1[] = { 1.f, 1.f, 0.6f, 0.8f, 1.f, 1.f };
      static const float h2[] = { 1.f, 1.f, 1.f, 0.6f, 0.8f, 1.f };
      const float hs[] = { bars_kf_eval(kt, h0, 6, t), bars_kf_eval(kt, h1, 6, t), bars_kf_eval(kt, h2, 6, t) };
      const float yb = centre.y + halfH;
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++)
        bars_draw_v(window->DrawList, cx[i], yb - hs[i] * H, yb, hw, c);
    }

    // Rising bars:
    //   three bars slide upward from the bottom (staggered 1/6 phase each).
    inline void SpinnerBarsRise(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float barH = H / 3.f;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float ph = ImFmod(t + (float)(i + 1) / 6.f, 1.f);
        const float yC = centre.y + halfH - barH * 0.5f - ph * (H - barH);
        bars_draw_v(window->DrawList, cx[i], yC - barH * 0.5f, yC + barH * 0.5f, hw, c);
      }
    }

    // Corner hop:
    //   three bars hop diagonally from bottom-right anchor toward the top row.
    inline void SpinnerBarsCornerHop(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.33f, 0.5f, 0.66f, 1.f };
      static const float a0[] = { 1.f, 0.5f, 0.f, 0.f, 0.f };
      static const float a1[] = { 1.f, 1.f, 0.5f, 0.f, 0.f };
      static const float a2[] = { 1.f, 1.f, 1.f, 0.5f, 0.f };
      const float yt = centre.y - halfH, yb = centre.y + halfH;
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float *av = (i == 0) ? a0 : (i == 1) ? a1 : a2;
        const float ay = bars_kf_eval(kt, av, 5, t);
        bars_draw_v(window->DrawList, cx[i], yt + ay * H, yb, hw, c);
      }
    }

    // Diagonal grow:
    //   three bars (aspect 0.75) hop between corners via background-position steps.
    inline void SpinnerBarsDiagonalGrow(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W / 0.75f, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.2f, 0.4f, 0.6f, 0.8f, 1.f };
      static const float ax[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
      static const float ay0[] = { 1.f, 0.5f, 0.f, 1.f, 1.f, 1.f };
      static const float ay1[] = { 1.f, 1.f, 0.5f, 0.f, 1.f, 1.f };
      static const float ay2[] = { 1.f, 1.f, 1.f, 0.5f, 0.f, 1.f };
      const float yt = centre.y - halfH, yb = centre.y + halfH;
      const float bh = 0.65f * H;
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float *ayv = (i == 0) ? ay0 : (i == 1) ? ay1 : ay2;
        const float ay = bars_kf_eval(kt, ayv, 6, t);
        const float axp = bars_kf_eval(kt, ax, 6, t);
        const float x = cx[i] + (axp - 0.5f) * 0.4f * W;
        bars_draw_v(window->DrawList, x, yt + ay * (H - bh), yt + ay * (H - bh) + bh, hw, c);
      }
    }

    // Bounce center:
    //   three centre-anchored bars (max 50% height) bounce between vertical slots.
    inline void SpinnerBarsBounceCenter(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W / 0.75f, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.2f, 0.4f, 0.6f, 0.8f, 1.f };
      static const float ay0[] = { 0.5f, 0.f, 1.f, 0.5f, 0.5f, 0.5f };
      static const float ay1[] = { 0.5f, 0.5f, 0.f, 1.f, 0.5f, 0.5f };
      static const float ay2[] = { 0.5f, 0.5f, 0.5f, 0.f, 1.f, 0.5f };
      const float bh = 0.5f * H;
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float *ayv = (i == 0) ? ay0 : (i == 1) ? ay1 : ay2;
        const float ay = bars_kf_eval(kt, ayv, 6, t);
        const float yC = centre.y - halfH + ay * H;
        bars_draw_v(window->DrawList, cx[i], yC - bh * 0.5f, yC + bh * 0.5f, hw, c);
      }
    }

    // Scale alternate:
    //   three bars pulse height 50%↔20%↔100% in sequence (animation-direction: alternate).
    inline void SpinnerBarsScaleAlt(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W / 0.75f, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float ph = ImFmod((float)ImGui::GetTime() * speed, 2.f);
      const float t = (ph <= 1.f) ? ph : 2.f - ph;
      static const float kt[] = { 0.f, 0.2f, 0.4f, 0.6f, 0.8f, 1.f };
      static const float h0[] = { 0.5f, 0.2f, 0.5f, 1.f, 0.5f, 0.5f };
      static const float h1[] = { 0.5f, 0.5f, 0.2f, 0.5f, 1.f, 0.5f };
      static const float h2[] = { 0.5f, 0.5f, 0.5f, 0.2f, 0.5f, 1.f };
      const float hs[] = { bars_kf_eval(kt, h0, 6, t), bars_kf_eval(kt, h1, 6, t), bars_kf_eval(kt, h2, 6, t) };
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float h = hs[i] * halfH;
        bars_draw_v(window->DrawList, cx[i], centre.y - h, centre.y + h, hw, c);
      }
    }

    // Corner wave:
    //   three bottom bars step through a corner wave (16.67% keyframes).
    inline void SpinnerBarsCornerWave(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W / 0.75f, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.1667f, 0.3333f, 0.5f, 0.6667f, 0.8333f, 1.f };
      static const float ay0[] = { 1.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f };
      static const float ay1[] = { 1.f, 1.f, 0.f, 0.f, 0.f, 1.f, 1.f };
      static const float ay2[] = { 1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 1.f };
      const float yt = centre.y - halfH, bh = 0.65f * H;
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float *ayv = (i == 0) ? ay0 : (i == 1) ? ay1 : ay2;
        const float ay = bars_kf_eval(kt, ayv, 7, t);
        bars_draw_v(window->DrawList, cx[i], yt + ay * (H - bh), yt + ay * (H - bh) + bh, hw, c);
      }
    }

    // Jump bars:
    //   three bars (60% max height) swap between top and bottom at 33/66%.
    inline void SpinnerBarsJump(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W / 0.75f, halfH = H * 0.5f, hw = thickness * 0.5f;
      const float cx[] = { centre.x - 0.4f * W, centre.x, centre.x + 0.4f * W };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.33f, 0.66f, 1.f };
      static const float ay0[] = { 0.f, 1.f, 0.f, 0.f };
      static const float ay1[] = { 1.f, 0.f, 1.f, 1.f };
      static const float ay2[] = { 0.f, 1.f, 0.f, 0.f };
      const float bh = 0.6f * H;
      const ImColor c = color_alpha(color, 1.f);
      for (int i = 0; i < 3; i++) {
        const float *ayv = (i == 0) ? ay0 : (i == 1) ? ay1 : ay2;
        const float ay = bars_kf_eval(kt, ayv, 4, t);
        const float yC = centre.y - halfH + ay * (H - bh) + bh * 0.5f;
        bars_draw_v(window->DrawList, cx[i], yC - bh * 0.5f, yC + bh * 0.5f, hw, c);
      }
    }

    // Double row:
    //   two rows of three bar segments slide horizontally (33/66/100% positions).
    inline void SpinnerBarsDoubleRow(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f, H = W / 1.2f, halfH = H * 0.5f;
      const float segW = W / 6.f, bh = halfH;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.33f, 0.66f, 1.f };
      static const float px0[] = { 0.5f, 0.f, 1.f, 0.5f };
      static const float px1[] = { 0.5f, 1.f, 0.f, 0.5f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - W * 0.5f;
      for (int row = 0; row < 2; row++) {
        const float y0 = centre.y - halfH + row * bh;
        const float y1 = y0 + bh;
        const float *pxv = (row == 0) ? px0 : px1;
        const float px = bars_kf_eval(kt, pxv, 4, t);
        const float x0 = left + px * (W - segW);
        window->DrawList->AddRectFilled(ImVec2(x0, y0), ImVec2(x0 + segW, y1), c);
        window->DrawList->AddRectFilled(ImVec2(x0 + segW * 2.f, y0), ImVec2(x0 + segW * 3.f, y1), c);
        window->DrawList->AddRectFilled(ImVec2(x0 + segW * 4.f, y0), ImVec2(x0 + segW * 5.f, y1), c);
      }
    }

    // Six-bar pulse:
    //   six corner bars (3×2) pulse height 50%→30% at 80% of the cycle.
    inline void SpinnerBarsSixPulse(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.8f, 0.9f, 1.f };
      static const float hf[] = { 0.5f, 0.3f, 0.5f, 0.5f };
      const float h = bars_kf_eval(kt, hf, 4, t);
      bars_draw_six_cols(centre.x - radius, W, centre.y - radius, H, h, color_alpha(color, 1.f), window->DrawList);
    }

    // Six-bar stagger:
    //   corner bars shrink to 30% height one-by-one, then grow back in wave order.
    inline void SpinnerBarsSixStagger(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.1667f, 0.3333f, 0.5f, 0.6667f, 0.8333f, 1.f };
      static const float h0[] = { 0.5f, 0.3f, 0.3f, 0.3f, 0.5f, 0.5f, 0.5f };
      static const float h1[] = { 0.5f, 0.3f, 0.3f, 0.3f, 0.5f, 0.5f, 0.5f };
      static const float h2[] = { 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 0.5f, 0.5f };
      static const float h3[] = { 0.5f, 0.5f, 0.3f, 0.3f, 0.5f, 0.5f, 0.5f };
      static const float h4[] = { 0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 0.5f };
      static const float h5[] = { 0.5f, 0.5f, 0.5f, 0.3f, 0.3f, 0.3f, 0.5f };
      const float *hs[] = { h0, h1, h2, h3, h4, h5 };
      static const float ax[] = { 0.f, 0.f, 0.5f, 0.5f, 1.f, 1.f };
      static const float ay[] = { 0.f, 1.f, 0.f, 1.f, 0.f, 1.f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int i = 0; i < 6; i++)
        bars_draw_box(window->DrawList, left, top, W, H, ax[i], ay[i], 0.2f, bars_kf_eval(kt, hs[i], 7, t), c);
    }

    // Morph plus:
    //   five bars morph 20×100% → 20×20% → 100×20% (alternate ping-pong).
    inline void SpinnerBarsMorphPlus(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float ph = ImFmod((float)ImGui::GetTime() * speed, 2.f);
      const float t = (ph <= 1.f) ? ph : 2.f - ph;
      static const float kt[] = { 0.f, 0.1f, 0.5f, 0.9f, 1.f };
      static const float wf[] = { 0.2f, 0.2f, 0.2f, 1.f, 1.f };
      static const float hf[] = { 1.f, 1.f, 0.2f, 0.2f, 0.2f };
      const float w = bars_kf_eval(kt, wf, 5, t), h = bars_kf_eval(kt, hf, 5, t);
      static const float ax[] = { 0.f, 0.f, 0.5f, 1.f, 1.f };
      static const float ay[] = { 0.f, 1.f, 0.5f, 0.f, 1.f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int i = 0; i < 5; i++)
        bars_draw_box(window->DrawList, left, top, W, H, ax[i], ay[i], w, h, c);
    }

    // Flip six:
    //   six bars shrink height (0.5 s alternate) while positions flip every 2 s.
    inline void SpinnerBarsFlipSix(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float time = (float)ImGui::GetTime() * speed;
      const float ph = ImFmod(time * 2.f, 2.f);
      const float ts = (ph <= 1.f) ? ph : 2.f - ph;
      static const float ks[] = { 0.f, 0.1f, 1.f };
      static const float hs[] = { 1.f, 1.f, 0.2f };
      const float hf = bars_kf_eval(ks, hs, 3, ts);
      const bool flip = ImFmod(time * 0.5f, 1.f) >= 0.5f;
      static const float ax0[] = { 0.f, 0.f, 0.5f, 0.5f, 1.f, 1.f };
      static const float ay0[] = { 0.f, 1.f, 0.5f, 0.5f, 0.f, 1.f };
      static const float ax1[] = { 0.f, 0.f, 0.5f, 0.5f, 1.f, 1.f };
      static const float ay1[] = { 0.5f, 0.5f, 0.f, 1.f, 0.5f, 0.5f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int i = 0; i < 6; i++)
        bars_draw_box(window->DrawList, left, top, W, H,
                      flip ? ax1[i] : ax0[i], flip ? ay1[i] : ay0[i], 0.2f, hf, c);
    }

    // Swap tri-bottom:
    //   three bars shrink to 40% height while bottom anchors cycle positions.
    inline void SpinnerBarsSwapTriBottom(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float ks[] = { 0.f, 0.33f, 0.66f, 1.f };
      static const float hs[] = { 1.f, 0.4f, 0.4f, 1.f };
      const float hf = bars_kf_eval(ks, hs, 4, t);
      static const float ax0[] = { 0.f, 0.5f, 1.f }, ay0[] = { 0.f, 1.f, 1.f };
      static const float ax1[] = { 1.f, 0.f, 0.5f }, ay1[] = { 0.f, 1.f, 1.f };
      const bool phase2 = t >= 0.66f;
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int i = 0; i < 3; i++)
        bars_draw_box(window->DrawList, left, top, W, H,
                      phase2 ? ax1[i] : ax0[i], phase2 ? ay1[i] : ay0[i], 0.2f, hf, c);
    }

    // Swap tri-zigzag:
    //   three bars shrink while anchors zigzag between corner pairs.
    inline void SpinnerBarsSwapTriZigzag(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float ks[] = { 0.f, 0.33f, 0.66f, 1.f };
      static const float hs[] = { 1.f, 0.4f, 0.4f, 1.f };
      const float hf = bars_kf_eval(ks, hs, 4, t);
      static const float ax0[] = { 0.f, 0.5f, 1.f }, ay0[] = { 0.f, 1.f, 0.f };
      static const float ax1[] = { 0.f, 0.5f, 1.f }, ay1[] = { 1.f, 0.f, 1.f };
      const bool phase2 = t >= 0.66f;
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int i = 0; i < 3; i++)
        bars_draw_box(window->DrawList, left, top, W, H,
                      phase2 ? ax1[i] : ax0[i], phase2 ? ay1[i] : ay0[i], 0.2f, hf, c);
    }

    // Morph diagonal:
    //   three diagonal bars morph 20×100% → 20×20% → 100×20%.
    inline void SpinnerBarsMorphDiagonal(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float ph = ImFmod((float)ImGui::GetTime() * speed, 2.f);
      const float t = (ph <= 1.f) ? ph : 2.f - ph;
      static const float kt[] = { 0.f, 0.1f, 0.5f, 0.9f, 1.f };
      static const float wf[] = { 0.2f, 0.2f, 0.2f, 1.f, 1.f };
      static const float hf[] = { 1.f, 1.f, 0.2f, 0.2f, 0.2f };
      const float w = bars_kf_eval(kt, wf, 5, t), h = bars_kf_eval(kt, hf, 5, t);
      static const float ax[] = { 0.f, 0.5f, 1.f };
      static const float ay[] = { 0.f, 0.5f, 1.f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int i = 0; i < 3; i++)
        bars_draw_box(window->DrawList, left, top, W, H, ax[i], ay[i], w, h, c);
    }

    // Slide diagonal:
    //   three diagonal bars shrink to 20% height while outer positions swap.
    inline void SpinnerBarsSlideDiagonal(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float ks[] = { 0.f, 0.33f, 0.66f, 1.f };
      static const float hs[] = { 1.f, 0.2f, 0.2f, 1.f };
      const float hf = bars_kf_eval(ks, hs, 4, t);
      static const float ax0[] = { 0.f, 0.5f, 1.f }, ay0[] = { 0.f, 0.5f, 1.f };
      static const float ax1[] = { 1.f, 0.5f, 0.f }, ay1[] = { 0.f, 0.5f, 1.f };
      const bool phase2 = t >= 0.66f;
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int i = 0; i < 3; i++)
        bars_draw_box(window->DrawList, left, top, W, H,
                      phase2 ? ax1[i] : ax0[i], phase2 ? ay1[i] : ay0[i], 0.2f, hf, c);
    }

    // Conic alternate:
    //   two 40% conic-corner tiles; the second slides on a diagonal (alternate).
    inline void SpinnerBarsConicAlternate(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W, s = 0.4f * W;
      const float ph = ImFmod((float)ImGui::GetTime() * speed, 2.f);
      const float t = (ph <= 1.f) ? ph : 2.f - ph;
      static const float kt[] = { 0.f, 0.1f, 0.5f, 0.9f, 1.f };
      static const float px[] = { 0.f, 0.f, W / 3.f, W / 3.f, 0.f };
      static const float py[] = { H / 3.f, H / 3.f, H / 3.f, 0.f, 0.f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      bars_draw_conic_tile(window->DrawList, left, top, s, c, num_segments);
      bars_draw_conic_tile(window->DrawList, left + bars_kf_eval(kt, px, 5, t),
                           top + bars_kf_eval(kt, py, 5, t), s, c, num_segments);
    }

    // Conic walk:
    //   two conic-corner tiles step through a 2×2 corner path (1.5 s loop).
    inline void SpinnerBarsConicWalk(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W, s = 0.4f * W, third = H / 3.f;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.2f, 0.33f, 0.66f, 0.8f, 1.f };
      static const float x0[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f };
      static const float y0[] = { 0.f, 0.f, third, third, third, third };
      static const float x1[] = { 0.f, 0.f, W / 3.f, W / 3.f, 0.f, 0.f };
      static const float y1[] = { third, third, third, 0.f, 0.f, 0.f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      bars_draw_conic_tile(window->DrawList, left + bars_kf_eval(kt, x0, 6, t),
                           top + bars_kf_eval(kt, y0, 6, t), s, c, num_segments);
      bars_draw_conic_tile(window->DrawList, left + bars_kf_eval(kt, x1, 6, t),
                           top + bars_kf_eval(kt, y1, 6, t), s, c, num_segments);
    }

    // March rows in:
    //   four stripe rows enter from the left, pause, then exit right (1.5 s).
    inline void SpinnerBarsMarchIn(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float period = W * 0.4f, bw = W * 0.2f;
      const float t = ImFmod((float)ImGui::GetTime() * speed / 1.5f, 1.f);
      bars_draw_rows4_h(window->DrawList, centre.x - radius, centre.y - radius, W, H,
                        period, bw, t, color_alpha(color, 1.f));
    }

    // March rows down:
    //   four stripe rows drop in from the top, pause, then exit downward (1.5 s).
    inline void SpinnerBarsMarchDown(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float rh = bars_rows4_rh(H);
      const float period = W * 0.4f, bw = W * 0.2f;
      const float t = ImFmod((float)ImGui::GetTime() * speed / 1.5f, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int r = 0; r < 4; r++) {
        const float y = top + (float)r * rh + bars_row_py_cascade(r, t, H);
        bars_draw_row_stripes(window->DrawList, left, y, W, rh, 0.f, period, bw, c);
      }
    }

    // Wave rows skew:
    //   four rows skew diagonally (aspect 1.6) then snap back to centre.
    inline void SpinnerBarsWaveSkew(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float H = radius * 2.f, W = H * 1.6f;
      const float rh = bars_rows4_rh(H);
      const float period = W * 0.625f * 0.4f, bw = W * 0.625f * 0.2f;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.1f, 0.33f, 0.66f, 0.9f, 1.f };
      static const float px0[] = { 0.5f, 0.5f, 1.f, 0.f, 0.5f, 0.5f };
      static const float px1[] = { 0.5f, 0.5f, 1.f - 9.f / 45.f, 9.f / 45.f, 0.5f, 0.5f };
      static const float px2[] = { 0.5f, 0.5f, 9.f / 45.f, 1.f - 9.f / 45.f, 0.5f, 0.5f };
      static const float px3[] = { 0.5f, 0.5f, 0.f, 1.f, 0.5f, 0.5f };
      const float *pxv[] = { px0, px1, px2, px3 };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - W * 0.5f, top = centre.y - radius;
      for (int r = 0; r < 4; r++)
        bars_draw_row_stripes(window->DrawList, left, top + (float)r * rh, W, rh,
                              (bars_kf_eval(kt, pxv[r], 6, t) - 0.5f) * W, period, bw, c);
    }

    // Row drop stagger:
    //   four rows drop down one-by-one (aspect 0.8, alternate ping-pong).
    inline void SpinnerBarsRowDrop(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W / 0.8f;
      const float rh = bars_rows4_rh(H);
      const float period = W * 0.4f, bw = W * 0.2f;
      const float ph = ImFmod((float)ImGui::GetTime() * speed / 0.75f, 2.f);
      const float t = (ph <= 1.f) ? ph : 2.f - ph;
      static const float kt[] = { 0.f, 0.1f, 0.25f, 0.5f, 0.75f, 0.9f, 1.f };
      static const float py0[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f };
      static const float py1[] = { 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, 1.f };
      static const float py2[] = { 0.f, 0.f, 0.f, 1.f, 1.f, 1.f, 1.f };
      static const float py3[] = { 0.f, 0.f, 1.f, 1.f, 1.f, 1.f, 1.f };
      const float *pyv[] = { py0, py1, py2, py3 };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - H * 0.5f;
      for (int r = 0; r < 4; r++) {
        const float band = bars_kf_eval(kt, pyv[r], 7, t) * rh;
        bars_draw_row_stripes(window->DrawList, left, top + band, W, rh, 0.f, period, bw, c);
      }
    }

    // Zigzag rows:
    //   four rows alternate stripe alignment left/right (0.75 s).
    inline void SpinnerBarsZigzagRows(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float rh = bars_rows4_rh(H);
      const float period = W * 1.4f * (2.f / 7.f), bw = W * 1.4f / 7.f;
      const float t = ImFmod((float)ImGui::GetTime() * speed / 0.75f, 1.f);
      static const float kt[] = { 0.f, 0.25f, 0.5f, 0.75f, 1.f };
      static const float px0[] = { 0.f, 1.f, 1.f, 1.f, 1.f };
      static const float px1[] = { 1.f, 1.f, 0.f, 0.f, 0.f };
      static const float px2[] = { 0.f, 0.f, 0.f, 1.f, 1.f };
      static const float px3[] = { 1.f, 0.f, 1.f, 1.f, 0.f };
      const float *pxv[] = { px0, px1, px2, px3 };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int r = 0; r < 4; r++)
        bars_draw_row_stripes(window->DrawList, left, top + (float)r * rh, W, rh,
                              bars_kf_eval(kt, pxv[r], 5, t) * (W - bw), period, bw, c);
    }

    // Fill cascade:
    //   stripe rows fill left→right in a staggered cascade (0.75 s linear).
    inline void SpinnerBarsFillCascade(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float rh = bars_rows4_rh(H);
      const float period = W * 1.4f * (2.f / 7.f), bw = W * 1.4f / 7.f;
      const float t = ImFmod((float)ImGui::GetTime() * speed / 0.75f, 1.f);
      static const float kt[] = { 0.f, 0.05f, 0.2f, 0.4f, 0.6f, 0.8f, 0.95f, 1.f };
      static const float px0[] = { 0.f, 0.f, 0.5f, 1.f, 1.f, 1.f, 1.f, 1.f };
      static const float px1[] = { 0.f, 0.f, 0.f, 0.5f, 1.f, 1.f, 1.f, 1.f };
      static const float px2[] = { 0.f, 0.f, 0.f, 0.f, 0.5f, 1.f, 1.f, 1.f };
      static const float px3[] = { 0.f, 0.f, 0.f, 0.f, 0.f, 0.5f, 1.f, 1.f };
      const float *pxv[] = { px0, px1, px2, px3 };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int r = 0; r < 4; r++)
        bars_draw_row_stripes(window->DrawList, left, top + (float)r * rh, W, rh,
                              bars_kf_eval(kt, pxv[r], 8, t) * (W - bw), period, bw, c);
    }

    // Ping-pong rows:
    //   four rows alternate stripe alignment (0↔100%) with a long hold.
    inline void SpinnerBarsPingPongRows(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float W = radius * 2.f, H = W;
      const float rh = bars_rows4_rh(H);
      const float period = W * 1.4f * (2.f / 7.f), bw = W * 1.4f / 7.f;
      const float t = ImFmod((float)ImGui::GetTime() * speed / 0.75f, 1.f);
      static const float px0[] = { 0.f, 1.f }, px1[] = { 1.f, 0.f }, px2[] = { 0.f, 1.f }, px3[] = { 1.f, 0.f };
      const float *pxv[] = { px0, px1, px2, px3 };
      const float u = (t < 0.2f) ? 0.f : (t > 0.8f) ? 1.f : (t - 0.2f) / 0.6f;
      static const float kt2[] = { 0.f, 1.f };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - radius, top = centre.y - radius;
      for (int r = 0; r < 4; r++)
        bars_draw_row_stripes(window->DrawList, left, top + (float)r * rh, W, rh,
                              bars_kf_eval(kt2, pxv[r], 2, u) * (W - bw), period, bw, c);
    }

    // Wave rows wide:
    //   four rows wave between centre, left, and right (aspect 1.4).
    inline void SpinnerBarsWaveRowsWide(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness;
      const float H = radius * 2.f, W = H * 1.4f;
      const float rh = bars_rows4_rh(H);
      const float period = W * 0.72f * 0.4f, bw = W * 0.72f * 0.2f;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float kt[] = { 0.f, 0.2f, 0.4f, 0.6f, 0.8f, 1.f };
      static const float px0[] = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
      static const float px1[] = { 0.5f, 0.5f, 1.f, 0.f, 0.5f, 0.5f };
      static const float px2[] = { 0.5f, 0.5f, 0.f, 1.f, 0.5f, 0.5f };
      static const float px3[] = { 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f };
      const float *pxv[] = { px0, px1, px2, px3 };
      const ImColor c = color_alpha(color, 1.f);
      const float left = centre.x - W * 0.5f, top = centre.y - radius;
      for (int r = 0; r < 4; r++)
        bars_draw_row_stripes(window->DrawList, left, top + (float)r * rh, W, rh,
                              (bars_kf_eval(kt, pxv[r], 6, t) - 0.5f) * W, period, bw, c);
    }

    // Grid fade:
    //   3×3 dot grid; cells fade out in a travelling wave (2 s).
    inline void SpinnerBarsGridFade(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      const float cell = (radius * 2.f) / 5.f;
      const float dot_r = thickness * 0.5f;
      const float t = ImFmod((float)ImGui::GetTime() * speed / 2.f, 1.f);
      static const uint16_t masks[] = {
        0x1FF, 0x0BF, 0x0B7, 0x077, 0x0EE, 0x1D9, 0x1B6, 0x16F, 0x1FD, 0x1FF
      };
      const int phase = ImMin(9, (int)(t * 10.f));
      bars_draw_grid3(window->DrawList, centre, cell, dot_r, color_alpha(color, 1.f), masks[phase], nullptr);
    }

    // Grid spread:
    //   3×3 dot grid; cells pulse box-shadow spread in a wave (2 s).
    inline void SpinnerBarsGridSpread(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      const float cell = (radius * 2.f) / 5.f;
      const float dot_r = thickness * 0.5f;
      const float spread_u = (2.f / 15.f) * thickness;
      const float t = ImFmod((float)ImGui::GetTime() * speed / 2.f, 1.f);
      static const uint16_t spread_mask[] = {
        0x000, 0x124, 0x092, 0x049, 0x092, 0x124, 0x248, 0x091, 0x122, 0x000
      };
      const int phase = ImMin(9, (int)(t * 10.f));
      float spread[9] = {};
      const uint16_t m = spread_mask[phase];
      for (int i = 0; i < 9; i++)
        if (m & (1 << i)) spread[i] = spread_u;
      bars_draw_grid3(window->DrawList, centre, cell, dot_r, color_alpha(color, 1.f), 0x1FF, spread);
    }

#ifndef _IMSPINNER_BARS_INTERNAL_
} // namespace ImSpinner
#endif

#endif // _IMSPINNER_BARS_H_
