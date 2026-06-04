#ifndef _IMSPINNER_TEXT_H_
#define _IMSPINNER_TEXT_H_

/*
 * imspinner text add-on
 *
 * Text-based loaders for imspinner. Include this header instead of (or in
 * addition to) "imspinner.h" — it pulls in "imspinner.h" itself.
 *
 * The macro _IMSPINNER_TEXT_H_ is defined before including "imspinner.h" so
 * that imspinner.h's demo (demoSpinners(), under IMSPINNER_DEMO) can pick up
 * the text spinners automatically.
 */

#include "imspinner.h"

namespace ImSpinner
{
    // Pulsing text loader, a port of the CSS:
    //   .loader:before { content:"Loading..." }
    //   @keyframes l1 { to { opacity: 0 } }   /* 1s linear infinite alternate */
    // 'alternate' + 'linear' => triangle wave; a full fade-out/fade-in cycle
    // spans 2 units of (time*speed).
    inline void SpinnerTextFade(const char *label, float radius, const ImColor &color = white, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float phase = ImFmod((float)ImGui::GetTime() * speed, 2.f);
      const float opacity = (phase < 1.f) ? (1.f - phase) : (phase - 1.f);

      // Scale the font down so the text fits inside the spinner cell (2*radius wide).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      window->DrawList->AddText(font, font_size, ImVec2(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f),
                                color_alpha(color, opacity), text);
    }

    // Per-character fading text: each glyph fades in/out with a phase offset and a
    // hue shift along the string, producing a travelling shimmer over the text.
    inline void SpinnerTextFading(const char *label, const char* text, float radius, float fsize, const ImColor &color = white, float speed = 2.8f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        if (!text || !*text)
            return;

        const float start = ImFmod((float)ImGui::GetTime() * speed, PI_2);
        const char *last_symbol = ImGui::FindRenderedTextEnd(text);
        const ImVec2 text_size = ImGui::CalcTextSize(text, last_symbol);
#if IMGUI_VERSION_NUM < 19150
        const // Newer imgui versions require make the current context's font non-const
#endif
        ImFont* font = ImGui::GetCurrentContext()->Font;

        ImVec2 pp(centre.x - text_size.x / 2.f, centre.y - text_size.y / 2.f);

        const int text_len = last_symbol - text;
        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        for (int i = 0; text != last_symbol; ++text, ++i) {
            const ImFontGlyph* glyph = ImGui::GetCurrentContext()->IMSPINNER_FIND_GLYPH(*text);

            const float alpha = ImClamp(ImSin(-start + (i / (float)text_len * PI_DIV_2)), 0.f, 1.f);
            ImColor c = ImColor::HSV(out_h + i * (1.f / text_len), out_s, out_v);
            font->RenderChar(window->DrawList, fsize, pp, color_alpha(c, alpha), (ImWchar)*text);
            pp.x += glyph->AdvanceX;
        }
    }
}

#endif // _IMSPINNER_TEXT_H_
