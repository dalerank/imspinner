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

    // Text with a growing underline, a port of the CSS:
    //   background: linear-gradient(currentColor 0 0) 0 100%/0% 3px no-repeat;
    //   @keyframes l2 { to { background-size: 100% 3px } }   /* 2s linear infinite */
    // A 'thickness'-tall bar under the text grows from 0 to full text width, then
    // snaps back to 0 and repeats (linear, not alternate).
    inline void SpinnerTextUnderline(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, float thickness = 3.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float progress = ImFmod((float)ImGui::GetTime() * speed, 1.f);

      // Scale the font down so the text fits inside the spinner cell (2*radius wide).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor c = color_alpha(color, 1.f);
      window->DrawList->AddText(font, font_size, tp, c, text);

      // Underline bar growing left-to-right (CSS background-size 0% -> 100%).
      const float y = tp.y + ts.y + thickness;
      window->DrawList->AddRectFilled(ImVec2(tp.x, y), ImVec2(tp.x + ts.x * progress, y + thickness), c);
    }

    // Text with a marching dashed underline, a port of the CSS:
    //   background: repeating-linear-gradient(90deg,currentColor 0 8%,#0000 0 10%) ... 3px;
    //   @keyframes l3 { to { background-position: 80% 100% } }   /* 2s steps(6) infinite */
    // A row of dashes under the text shifts by one period over the cycle in
    // 'steps' discrete jumps (steps() timing), looping seamlessly.
    inline void SpinnerTextUnderlineDots(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, float thickness = 3.f, int dashes = 6, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const int ndashes = ImMax(1, dashes);

      // Scale the font down so the text fits inside the spinner cell (2*radius wide).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor c = color_alpha(color, 1.f);
      window->DrawList->AddText(font, font_size, tp, c, text);

      // Dashes light up one after another, left to right (CSS steps()): the count
      // grows from 1 to 'dashes' over the cycle, then resets. Dash fills 80% of the
      // period (8% solid / 2% gap in CSS).
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int shown = ImMin(ndashes, (int)(t * ndashes) + 1);
      const float period = ts.x / (float)ndashes;
      const float dash = period * 0.8f;
      const float y = tp.y + ts.y + thickness;
      for (int i = 0; i < shown; i++) {
        const float x0 = tp.x + i * period;
        window->DrawList->AddRectFilled(ImVec2(x0, y), ImVec2(x0 + dash, y + thickness), c);
      }
    }

    // Typing text, a port of the CSS clip-path typers:
    //   l4: clip-path inset 3ch  -> -1ch, steps(4)   -> reveals the last 'tail' chars
    //   l5: clip-path inset 100% -> -1ch, steps(11)  -> types the whole word out
    // The last 'tail' characters are revealed one by one, left to right (e.g.
    // "Loading" -> "Loading." -> "Loading.." -> "Loading..."), then it repeats.
    // Pass tail <= 0 to type out the entire string. The full text is laid out
    // (left edge fixed) so nothing shifts as it reveals.
    inline void SpinnerTextTyping(const char *label, float radius, const ImColor &color = white, float speed = 1.f, int tail = 3, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);
      const int ntail = (tail <= 0) ? len : ImMin(tail, len); // tail<=0 -> type out the whole text
      const int nsteps = ntail + 1;                          // hidden..fully revealed

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int step = ImMin(nsteps - 1, (int)(t * nsteps)); // 0..ntail (CSS steps())
      const int visible = (len - ntail) + step;

      // Scale the font down so the FULL text fits the cell (so it never jumps).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      window->DrawList->AddText(font, font_size, tp, color_alpha(color, 1.f), text, text + visible);
    }

    // Scrolling marquee text, a port of the CSS:
    //   color: #0000; text-shadow: 0 0 c, 11ch 0 c;  overflow: hidden;
    //   @keyframes l6 { to { text-shadow: -11ch 0 c, 0ch 0 c } }   /* 2s linear infinite */
    // Two copies of the text, spaced one text-width + 1ch apart, slide left inside
    // a text-width window: as one leaves to the left the next enters from the
    // right, looping seamlessly.
    inline void SpinnerTextScroll(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell (the scroll window width).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const float ch = ts.x / (float)len;                    // monospace char width
      const float wrap = ts.x + ch;                          // 11ch for a 10-char string
      const float shift = ImFmod((float)ImGui::GetTime() * speed, 1.f) * wrap;

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor c = color_alpha(color, 1.f);

      // overflow: hidden -> clip to the text window, then draw both sliding copies.
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift, tp.y), c, text);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift + wrap, tp.y), c, text);
      window->DrawList->PopClipRect();
    }
}

#endif // _IMSPINNER_TEXT_H_
