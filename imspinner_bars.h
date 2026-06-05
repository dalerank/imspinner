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

#ifndef _IMSPINNER_BARS_INTERNAL_
} // namespace ImSpinner
#endif

#endif // _IMSPINNER_BARS_H_
