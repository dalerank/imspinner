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

    // Two-tone color sweep, a port of the CSS background-clip:text loaders:
    //   l7 (mode 0): linear-gradient(#C02942 .., #000 0) -> a hard edge sweeps across,
    //                chars fill from 'bg' to 'color' left to right (cumulative).
    //   l8 (mode 1): linear-gradient(#000 .., #C02942 1ch, #000 0) -> a 1-char 'color'
    //                band scans across, one character is highlighted at a time.
    // Both step one character per cycle (CSS steps()), then repeat.
    // (CSS base is #000; here 'bg' defaults to white so it shows on a dark theme.)
    inline void SpinnerTextColorFill(const char *label, float radius, const ImColor &color = ImColor(0xC0, 0x29, 0x42), const ImColor &bg = white, float speed = 0.5f, int mode = 0, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);
      const int nsteps = len + 1;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int step = ImMin(len, (int)(t * nsteps));        // 0..len (CSS steps())

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor cfg = color_alpha(color, 1.f);
      const ImColor cbg = color_alpha(bg, 1.f);
      auto char_x = [&](int n) { return tp.x + font->CalcTextSizeA(font_size, 99999.f, 0.f, text, text + n).x; };

      if (mode == 0) {
        // Cumulative fill: prefix [0, step) in 'color', the rest in 'bg'.
        window->DrawList->AddText(font, font_size, tp, cfg, text, text + step);
        window->DrawList->AddText(font, font_size, ImVec2(char_x(step), tp.y), cbg, text + step, text + len);
      } else {
        // Single moving highlight: whole text in 'bg', the char at 'step' in 'color'.
        window->DrawList->AddText(font, font_size, tp, cbg, text, text + len);
        if (step < len)
          window->DrawList->AddText(font, font_size, ImVec2(char_x(step), tp.y), cfg, text + step, text + step + 1);
      }
    }

    // Multi-color scrolling marquee, a port of the CSS l9:
    //   5 text-shadow copies (base, olive, crimson, teal, base) spaced 11ch apart,
    //   shifting left by 11ch each quarter with cubic-bezier(.3,1,0,1) easing;
    //   animation: 5s infinite. overflow:hidden clips to the text width.
    // Each segment snaps the text to the next color, looping seamlessly because the
    // first and last copies share 'color'. (CSS base is #000; here it defaults to
    // white so it shows on a dark theme.)
    inline void SpinnerTextScrollColors(const char *label, float radius, const ImColor &color = white, float speed = 0.2f, const char *text = "Loading...")
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
      const ImColor palette[] = { color, ImColor(0x8A, 0x9B, 0x0F), ImColor(0xC0, 0x29, 0x42), ImColor(0x00, 0xA0, 0xB0), color };
      const int K = (int)(sizeof(palette) / sizeof(palette[0]));

      // Per-segment ease (each 1/(K-1) of the cycle shifts one wrap, cubic-bezier-ish).
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int seg = ImMin(K - 2, (int)(t * (K - 1)));
      const float local = t * (K - 1) - seg;
      const float shift = (seg + ease_outcubic(local)) * wrap;

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);

      // overflow: hidden -> clip to the text window, then draw the colored copies.
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      for (int i = 0; i < K; i++)
        window->DrawList->AddText(font, font_size, ImVec2(tp.x + i * wrap - shift, tp.y), color_alpha(palette[i], 1.f), text);
      window->DrawList->PopClipRect();
    }

    // Color-cycling text, a port of the CSS l10:
    //   linear-gradient of 4 solid blocks (base, olive, crimson, teal), each one
    //   text-width wide, scrolled under background-clip:text with cubic-bezier easing.
    // The whole text cycles through the palette; on each transition the boundary
    // between two colors wipes across the text (pixel-accurate, not per-character).
    // animation: 5s infinite, loops seamlessly (teal -> base -> ...).
    // (CSS base is #000; here it defaults to white so it shows on a dark theme.)
    inline void SpinnerTextColorCycle(const char *label, float radius, const ImColor &color = white, float speed = 0.2f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImColor palette[] = { color, ImColor(0x8A, 0x9B, 0x0F), ImColor(0xC0, 0x29, 0x42), ImColor(0x00, 0xA0, 0xB0) };
      const int K = (int)(sizeof(palette) / sizeof(palette[0]));

      // Pick the current color pair and the eased wipe progress within the segment.
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int seg = ImMin(K - 1, (int)(t * K));
      const float e = ease_outcubic(t * K - seg);
      const ImColor cA = color_alpha(palette[seg], 1.f);
      const ImColor cB = color_alpha(palette[(seg + 1) % K], 1.f);

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float bx = tp.x + (1.f - e) * ts.x;              // colorA left of bx, colorB right of it

      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(bx, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, tp, cA, text);
      window->DrawList->PopClipRect();

      window->DrawList->PushClipRect(ImVec2(bx, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, tp, cB, text);
      window->DrawList->PopClipRect();
    }

    // Bouncing letters, a port of the CSS l11:
    //   two stacked copies masked to alternating characters move in opposite
    //   directions; a wild cubic-bezier(.5,220,.5,-220) makes translateY oscillate.
    // Here each character is drawn with a vertical offset whose sign alternates by
    // index and oscillates over time -> neighbouring letters bob up/down in antiphase.
    inline void SpinnerTextBounce(const char *label, float radius, const ImColor &color = white, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
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
      const float osc = ImSin((float)ImGui::GetTime() * speed * PI_2); // -1..1, 1s period at speed 1
      const float amp = ts.y * 0.25f;

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float sign = (i & 1) ? -1.f : 1.f;
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + sign * amp * osc), c, text + i, text + i + 1);
        x += cw;
      }
    }

    // Split-and-slide text, a port of the CSS l12 / l13:
    //   two stacked copies clipped to the top / bottom half of the text slide in
    //   opposite directions by one text-width, then pause; a shadow copy one
    //   text-width away wraps them seamlessly, overflow:hidden clips to the width.
    //   mode 0 (l12): both halves move together (top right, bottom left).
    //   mode 1 (l13): the bottom half is half a cycle behind, and the dash happens
    //                 in the first quarter -> the halves slide one after the other.
    inline void SpinnerTextSplit(const char *label, float radius, const ImColor &color = white, float speed = 1.f, int mode = 0, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell (the slide window width).
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
      const float midy = tp.y + ts.y * 0.5f;

      // l12: move over 1/2 of the cycle, no delay; l13: move over 1/4, bottom half delayed half a cycle.
      const float move_frac = (mode == 0) ? 0.5f : 0.25f;
      const float delay = (mode == 0) ? 0.f : 0.5f;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float shift_t = ease_inoutquad(ImMin(1.f, t / move_frac)) * ts.x;
      const float tb = ImFmod(t + delay, 1.f);
      const float shift_b = ease_inoutquad(ImMin(1.f, tb / move_frac)) * ts.x;

      // Top half slides right (with a wrap copy entering from the left).
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, midy), true);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x + shift_t, tp.y), c, text);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x + shift_t - ts.x, tp.y), c, text);
      window->DrawList->PopClipRect();

      // Bottom half slides left (with a wrap copy entering from the right).
      window->DrawList->PushClipRect(ImVec2(tp.x, midy), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift_b, tp.y), c, text);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift_b + ts.x, tp.y), c, text);
      window->DrawList->PopClipRect();
    }

    // Fill-then-scroll text, a port of the CSS l14 (two phases over the cycle):
    //   0..80%:   text stays put while an underline bar grows 0% -> 100%.
    //   80..100%: the bar retracts 100% -> 0% and the text scrolls one text-width
    //             left (a wrap copy slides in from the right, overflow:hidden clips).
    // So the progress fills, then the line slides away and a fresh one comes in.
    inline void SpinnerTextUnderlineScroll(const char *label, float radius, const ImColor &color = white, float speed = 0.66f, float thickness = 3.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell (the scroll window width).
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
      const float wrap = ts.x;                               // 10ch == text width

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float fill_end = 0.8f;
      float bar_frac, shift;
      if (t < fill_end) { bar_frac = t / fill_end; shift = 0.f; }            // grow the bar, text still
      else { const float l = (t - fill_end) / (1.f - fill_end); bar_frac = 1.f - l; shift = l * wrap; } // retract + scroll

      // Text: two scrolling copies clipped to the window (overflow: hidden).
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift, tp.y), c, text);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift + wrap, tp.y), c, text);
      window->DrawList->PopClipRect();

      // Underline bar.
      const float y = tp.y + ts.y + thickness;
      window->DrawList->AddRectFilled(ImVec2(tp.x, y), ImVec2(tp.x + ts.x * bar_frac, y + thickness), c);
    }

    // Rolling letters, a port of the CSS l15:
    //   alternating characters roll vertically by one line-height in opposite
    //   directions (a copy rolls out while an identical one rolls in), moving over
    //   0..80% of the cycle then pausing. Clipped to the line box so it's seamless.
    inline void SpinnerTextRoll(const char *label, float radius, const ImColor &color = white, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
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
      const float H = ts.y;                                  // one line-height roll distance
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float roll = ImMin(1.f, t / 0.8f);               // roll over 0..80%, hold 80..100%

      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float sign = (i & 1) ? -1.f : 1.f;             // neighbours roll opposite ways
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + sign * roll * H), c, text + i, text + i + 1);          // rolling out
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + sign * (roll - 1.f) * H), c, text + i, text + i + 1);  // rolling in
        x += cw;
      }
      window->DrawList->PopClipRect();
    }

    // Per-letter color blink, a port of the CSS l16. The CSS uses letter-spacing +
    // 9 shifted text-shadows so every character is an independent shadow that can be
    // colored on its own; keyframes (20/40/60/80%) recolor different letters, here
    // reproduced as a palette + a 5-keyframe table, interpolated over the cycle.
    // (CSS uncolored letters are currentColor; here that's 'color', default white.)
    // The color table targets the 10 chars of "Loading..."; extra chars stay 'color'.
    inline void SpinnerTextColorful(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImColor pal[] = {
        color,                       // 0: base (CSS currentColor)
        ImColor(255, 0, 0),          // 1: red
        ImColor(0, 128, 0),          // 2: green
        ImColor(255, 165, 0),        // 3: orange
        ImColor(255, 255, 0),        // 4: yellow
        ImColor(173, 216, 230),      // 5: lightblue
        ImColor(128, 128, 128),      // 6: grey
        ImColor(255, 165, 22),       // 7: #ffa516
        ImColor(99, 255, 244),       // 8: #63fff4
        ImColor(233, 69, 233)        // 9: #e945e9
      };
      static const int kf[5][10] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //  0%
        { 0, 0, 1, 0, 7, 8, 0, 0, 2, 0 }, // 20%
        { 0, 0, 1, 9, 0, 2, 3, 0, 2, 0 }, // 40%
        { 0, 5, 0, 9, 0, 2, 0, 4, 7, 1 }, // 60%
        { 0, 5, 4, 8, 7, 1, 0, 6, 8, 0 }  // 80%
      };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int seg = ImMin(4, (int)(t * 5.f));
      const float u = t * 5.f - seg;
      auto lerp = [](const ImColor &a, const ImColor &b, float k) {
        return ImColor(a.Value.x + (b.Value.x - a.Value.x) * k, a.Value.y + (b.Value.y - a.Value.y) * k,
                       a.Value.z + (b.Value.z - a.Value.z) * k, a.Value.w + (b.Value.w - a.Value.w) * k);
      };

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const ImColor col = (i < 10) ? lerp(pal[kf[seg][i]], pal[kf[(seg + 1) % 5][i]], u) : color;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), color_alpha(col, 1.f), text + i, text + i + 1);
        x += cw;
      }
    }

    // Cascading letters, a port of the CSS l17:
    //   letters drop in from above one by one (left to right) over 0..40%, hold the
    //   whole word 40..60%, then fall out downward one by one (right to left) 60..100%.
    //   Clipped to the line box so off-line letters are hidden.
    inline void SpinnerTextCascade(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
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
      const float H = ts.y;                                  // one line-height drop distance
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float step = 0.4f / (float)len;                  // per-letter stagger (0..40% in, 60..100% out)

      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float in_s = i * step, in_e = in_s + step;                 // drops in: -H -> 0
        const float out_s = 0.6f + (len - 1 - i) * step, out_e = out_s + step; // falls out: 0 -> +H (last letter first)
        float y;
        if (t < in_s)        y = -H;
        else if (t < in_e)   y = -H + H * (t - in_s) / step;
        else if (t < out_s)  y = 0.f;
        else if (t < out_e)  y = H * (t - out_s) / step;
        else                 y = H;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + y), c, text + i, text + i + 1);
        x += cw;
      }
      window->DrawList->PopClipRect();
    }

    // Conveyor letters, a port of the CSS l18 (the horizontal twin of the cascade):
    //   letters slide in from the right one by one (left to right) over 0..40%, hold
    //   the whole word 40..60%, then slide out to the left one by one 60..100%.
    //   Clipped to the text window so off-window letters are hidden.
    inline void SpinnerTextConveyor(const char *label, float radius, const ImColor &color = white, float speed = 0.33f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell (the slide window width).
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
      const float D = ts.x;                                  // off-window slide distance
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float step = 0.4f / (float)len;                  // per-letter stagger

      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      float nx = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float in_s = i * step, in_e = in_s + step;                 // slides in from the right: +D -> 0
        const float out_s = 0.6f + i * step, out_e = out_s + step;       // slides out to the left: 0 -> -D
        float dx;
        if (t < in_s)        dx = D;
        else if (t < in_e)   dx = D * (1.f - (t - in_s) / step);
        else if (t < out_s)  dx = 0.f;
        else if (t < out_e)  dx = -D * (t - out_s) / step;
        else                 dx = -D;
        window->DrawList->AddText(font, font_size, ImVec2(nx + dx, tp.y), c, text + i, text + i + 1);
        nx += cw;
      }
      window->DrawList->PopClipRect();
    }

    // Sequential fade reveal, a port of the CSS l19:
    //   letters fade in one by one (left to right) over 0..40%, hold the whole word
    //   40..60%, then fade out one by one (left to right) 60..100%. The letters do
    //   not move; only their alpha changes.
    inline void SpinnerTextReveal(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float step = 0.4f / (float)len;                  // per-letter stagger

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float in_s = i * step, in_e = in_s + step;                 // fade in:  0 -> 1
        const float out_s = 0.6f + i * step, out_e = out_s + step;       // fade out: 1 -> 0
        float a;
        if (t < in_s)        a = 0.f;
        else if (t < in_e)   a = (t - in_s) / step;
        else if (t < out_s)  a = 1.f;
        else if (t < out_e)  a = 1.f - (t - out_s) / step;
        else                 a = 0.f;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), color_alpha(color, a), text + i, text + i + 1);
        x += cw;
      }
    }

    // Traveling hop wave, a port of the CSS l20:
    //   each letter hops up in turn (left to right), one at a time, so a wave runs
    //   across the word and repeats. Letter i peaks at (i+1)/(len+1) of the cycle;
    //   the linear keyframes make each hop a triangle (up then back).
    inline void SpinnerTextWave(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
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
      const float amp = ts.y * 0.35f;                        // hop height (~10px at 30px font)
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float hw = 1.f / (float)(len + 1);               // half-width of each hop window

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float peak = (i + 1) * hw;                     // this letter's peak time
        float d = ImFabs(t - peak);
        d = ImMin(d, 1.f - d);                               // cyclic distance
        const float hop = ImMax(0.f, 1.f - d / hw) * amp;    // triangle hop
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y - hop), c, text + i, text + i + 1);
        x += cw;
      }
    }

    // Sweeping highlight, a port of the CSS l21:
    //   a bar sweeps across the text left to right; inside the bar the text is
    //   inverted (drawn in 'bg' over a 'color' fill), outside it stays 'color'.
    //   The bar fills over the cycle, then restarts. (CSS is white-on-black vs
    //   black-on-page; adapted here with a 'color' fill so it reads on a dark theme.)
    inline void SpinnerTextSweep(const char *label, float radius, const ImColor &color = white, const ImColor &bg = ImColor(0, 0, 0), float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor cfg = color_alpha(color, 1.f);
      const ImColor cbg = color_alpha(bg, 1.f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float boundary = tp.x + ts.x * t;                // bar covers [tp.x, boundary]

      // The bar (filled) behind the swept part, with inverted text on it.
      window->DrawList->AddRectFilled(ImVec2(tp.x, tp.y), ImVec2(boundary, tp.y + ts.y), cfg);
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(boundary, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, tp, cbg, text);
      window->DrawList->PopClipRect();

      // Normal text on the rest.
      window->DrawList->PushClipRect(ImVec2(boundary, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, tp, cfg, text);
      window->DrawList->PopClipRect();
    }

    // Shine band, a port of the CSS l22:
    //   a band (CSS: a 135deg diagonal ~1em stripe) sweeps across the text; inside
    //   the band the text is inverted ('bg' over a 'color' fill), like a glint
    //   passing over it. Here the band is vertical (axis-aligned clips can't tilt).
    inline void SpinnerTextShine(const char *label, float radius, const ImColor &color = white, const ImColor &bg = ImColor(0, 0, 0), float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor cfg = color_alpha(color, 1.f);
      const ImColor cbg = color_alpha(bg, 1.f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float halfw = ts.x * 0.14f;                      // band half-width (~1em)
      const float bc = tp.x - halfw + t * (ts.x + 2.f * halfw); // band centre sweeps left -> right
      const float x0 = ImMax(tp.x, bc - halfw);
      const float x1 = ImMin(tp.x + ts.x, bc + halfw);

      // Normal text everywhere.
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, tp, cfg, text);
      window->DrawList->PopClipRect();

      // The shine band: fill + inverted text inside it.
      if (x1 > x0) {
        window->DrawList->AddRectFilled(ImVec2(x0, tp.y), ImVec2(x1, tp.y + ts.y), cfg);
        window->DrawList->PushClipRect(ImVec2(x0, tp.y), ImVec2(x1, tp.y + ts.y), true);
        window->DrawList->AddText(font, font_size, tp, cbg, text);
        window->DrawList->PopClipRect();
      }
    }

    // Scroll + inverting sweep, a port of the CSS l23:
    //   the text scrolls left (seamless marquee) while a full-width band sweeps
    //   across; inside the band the text is inverted ('bg' over a 'color' fill).
    //   The band covers the whole window at mid-cycle. (CSS is white/black on a
    //   light page; adapted here with a 'color' fill so it reads on a dark theme.)
    inline void SpinnerTextScrollSweep(const char *label, float radius, const ImColor &color = white, const ImColor &bg = ImColor(0, 0, 0), float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell (the scroll window width).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor cfg = color_alpha(color, 1.f);
      const ImColor cbg = color_alpha(bg, 1.f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float shift = t * ts.x;                          // text scrolls left one word per cycle
      const float bl = tp.x + ts.x - t * 2.f * ts.x;         // band left edge: off-right -> full window -> off-left
      const float ix0 = ImMax(tp.x, bl);
      const float ix1 = ImMin(tp.x + ts.x, bl + ts.x);

      // Normal scrolling text (two copies for a seamless wrap), clipped to the window.
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift, tp.y), cfg, text);
      window->DrawList->AddText(font, font_size, ImVec2(tp.x + ts.x - shift, tp.y), cfg, text);
      window->DrawList->PopClipRect();

      // Inverting band: fill + inverted scrolling text inside it.
      if (ix1 > ix0) {
        window->DrawList->AddRectFilled(ImVec2(ix0, tp.y), ImVec2(ix1, tp.y + ts.y), cfg);
        window->DrawList->PushClipRect(ImVec2(ix0, tp.y), ImVec2(ix1, tp.y + ts.y), true);
        window->DrawList->AddText(font, font_size, ImVec2(tp.x - shift, tp.y), cbg, text);
        window->DrawList->AddText(font, font_size, ImVec2(tp.x + ts.x - shift, tp.y), cbg, text);
        window->DrawList->PopClipRect();
      }
    }

    // Scanning spotlight, a port of the CSS l24:
    //   a round spotlight (radial gradient) sweeps back and forth across the text
    //   (alternate), brightening the letters it passes (bright inside, dim outside).
    //   A true round clip isn't available, so each letter's brightness is set by its
    //   distance to the spotlight centre, giving the same scanning-glow effect.
    inline void SpinnerTextSpotlight(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float R = ts.y * 0.8f;                           // spotlight radius (~one line height)
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float tri = (t < 0.5f) ? t * 2.f : 2.f - t * 2.f; // ping-pong 0..1..0 (CSS alternate)
      const float bx = tp.x + tri * ts.x;                    // spotlight centre

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float d = ImFabs((x + cw * 0.5f) - bx);        // distance from letter centre to spotlight
        const float b = ImClamp(1.f - d / R, 0.f, 1.f);
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), color_alpha(color, 0.2f + 0.8f * b), text + i, text + i + 1);
        x += cw;
      }
    }

    // Glitchy shake, a port of the CSS l25:
    //   the whole word moves between 8 fixed offsets over the cycle (scaled from the
    //   CSS px values at 30px font). mode 0: snap (each held for 1/8, then jumps),
    //   like the CSS; mode 1: smoothly interpolate between offsets for a fluid wobble.
    inline void SpinnerTextShake(const char *label, float radius, const ImColor &color = white, float speed = 1.f, int mode = 0, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float k = font_size / 30.f;                      // CSS offsets are px at 30px font
      const ImVec2 off[8] = {
        ImVec2(10, 0), ImVec2(4, -4), ImVec2(2, 8), ImVec2(12, -6),
        ImVec2(0, 12), ImVec2(-8, -4), ImVec2(-12, 6), ImVec2(6, 0)
      };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int seg = ImMin(7, (int)(t * 8.f));
      ImVec2 o = off[seg];
      if (mode != 0) {                                       // smooth: lerp toward the next offset
        const float u = t * 8.f - seg;
        const ImVec2 n = off[(seg + 1) % 8];
        o = ImVec2(o.x + (n.x - o.x) * u, o.y + (n.y - o.y) * u);
      }
      window->DrawList->AddText(font, font_size, ImVec2(tp.x + o.x * k, tp.y + o.y * k), color_alpha(color, 1.f), text);
    }

    // 3D-ish flip, an approximation of the CSS l26:
    //   the word flips over X, then over Y, then scales to -1, returning to identity.
    //   True perspective isn't available, so the text's vertices are scaled about its
    //   centre (sx = cos(rotY)*s, sy = cos(rotX)*s): it squashes to a line and mirrors
    //   -- a flat 'card flip' rather than a perspective rotation.
    inline void SpinnerTextFlip(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      auto ramp = [](float v, float a, float b) { return ImClamp((v - a) / (b - a), 0.f, 1.f); };
      const float ax = ramp(t, 0.10f, 0.30f) * IM_PI;        // rotateX 0 -> 180
      const float ay = ramp(t, 0.36f, 0.63f) * IM_PI;        // rotateY 0 -> 180
      const float s = 1.f - 2.f * ramp(t, 0.69f, 0.90f);     // scale 1 -> -1
      const float sx = ImCos(ay) * s;
      const float sy = ImCos(ax) * s;

      // Draw the text, then scale its vertices about the centre (the 'flip').
      const ImVec2 ctr(tp.x + ts.x * 0.5f, tp.y + ts.y * 0.5f);
      const int vtx0 = window->DrawList->VtxBuffer.Size;
      window->DrawList->AddText(font, font_size, tp, color_alpha(color, 1.f), text);
      ImDrawVert *vtx = window->DrawList->VtxBuffer.Data;
      for (int i = vtx0; i < window->DrawList->VtxBuffer.Size; i++) {
        vtx[i].pos.x = ctr.x + (vtx[i].pos.x - ctr.x) * sx;
        vtx[i].pos.y = ctr.y + (vtx[i].pos.y - ctr.y) * sy;
      }
    }

    // Two halves spinning, a port of the CSS l27:
    //   the text is split in two; each half rotates a full turn over the first half
    //   of its cycle then holds, with the second half delayed by half a cycle, so the
    //   halves spin alternately. Vertices are rotated about each half's own centre.
    inline void SpinnerTextSpin(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor col = color_alpha(color, 1.f);
      const int half = len / 2;
      const float wA = font->CalcTextSizeA(font_size, 99999.f, 0.f, text, text + half).x;

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float angA = PI_2 * ImMin(1.f, t / 0.5f);        // spins over first half, then holds
      const float tB = ImFmod(t + 0.5f, 1.f);                // second half delayed by half a cycle
      const float angB = PI_2 * ImMin(1.f, tB / 0.5f);

      auto drawPart = [&](const char *b, const char *e, float x, float ang) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, b, e).x;
        const ImVec2 cc(x + cw * 0.5f, tp.y + ts.y * 0.5f);  // rotate about this half's centre
        const float co = ImCos(ang), si = ImSin(ang);
        const int v0 = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), col, b, e);
        ImDrawVert *vtx = window->DrawList->VtxBuffer.Data;
        for (int i = v0; i < window->DrawList->VtxBuffer.Size; i++) {
          const float dx = vtx[i].pos.x - cc.x, dy = vtx[i].pos.y - cc.y;
          vtx[i].pos.x = cc.x + dx * co - dy * si;
          vtx[i].pos.y = cc.y + dx * si + dy * co;
        }
      };
      drawPart(text, text + half, tp.x, angA);
      drawPart(text + half, text + len, tp.x + wA, angB);
    }

    // Tumble flip, a port of the CSS l28:
    //   the word spins 180 in-plane, then flips over Y, then over X, returning to
    //   identity. mode 0: flat tumble (vertices scaled by the flips, then rotated);
    //   mode 1: true 3D perspective (vertices rotated in 3D about Z/Y/X, then
    //   projected with depth division, like CSS perspective(300px)).
    inline void SpinnerTextTumble(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, int mode = 0, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      auto ramp = [](float v, float a, float b) { return ImClamp((v - a) / (b - a), 0.f, 1.f); };
      const float rz = ramp(t, 0.10f, 0.30f) * IM_PI;        // in-plane rotate 0 -> 180
      const float ay = ramp(t, 0.36f, 0.63f) * IM_PI;        // rotateY 0 -> 180
      const float ax = ramp(t, 0.69f, 0.90f) * IM_PI;        // rotateX 0 -> 180
      const float cz = ImCos(rz), sz = ImSin(rz);
      const float cy = ImCos(ay), sy = ImSin(ay);
      const float cx = ImCos(ax), sx = ImSin(ax);
      const float P = font_size * 10.f;                      // perspective(300px) at 30px font

      const ImVec2 ctr(tp.x + ts.x * 0.5f, tp.y + ts.y * 0.5f);
      const int vtx0 = window->DrawList->VtxBuffer.Size;
      window->DrawList->AddText(font, font_size, tp, color_alpha(color, 1.f), text);
      ImDrawVert *vtx = window->DrawList->VtxBuffer.Data;
      for (int i = vtx0; i < window->DrawList->VtxBuffer.Size; i++) {
        const float rx = vtx[i].pos.x - ctr.x;
        const float ry = vtx[i].pos.y - ctr.y;
        if (mode == 0) {
          // Flat: scale by the flip cosines, then rotate in-plane.
          const float dx = rx * cy, dy = ry * cx;
          vtx[i].pos.x = ctr.x + dx * cz - dy * sz;
          vtx[i].pos.y = ctr.y + dx * sz + dy * cz;
        } else {
          // 3D: rotateZ, rotateY, rotateX, then perspective-project (z toward viewer).
          const float x1 = rx * cz - ry * sz, y1 = rx * sz + ry * cz; // rotateZ (z=0)
          const float x2 = x1 * cy, z2 = -x1 * sy;                    // rotateY
          const float y3 = y1 * cx - z2 * sx, z3 = y1 * sx + z2 * cx; // rotateX
          const float f = P / ImMax(P - z3, 1.f);                     // perspective division
          vtx[i].pos.x = ctr.x + x2 * f;
          vtx[i].pos.y = ctr.y + y3 * f;
        }
      }
    }

    // Counter-rotating swirl, a port of the CSS l29:
    //   alternating characters spin a full turn about the WHOLE word's centre in
    //   opposite directions (even +1 turn, odd -1 turn) over 0..80%, then hold.
    //   Each letter's vertices are rotated about the shared centre, so the letters
    //   swirl around it.
    inline void SpinnerTextSwirl(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImVec2 cc(tp.x + ts.x * 0.5f, tp.y + ts.y * 0.5f); // word centre
      const ImColor col = color_alpha(color, 1.f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float angle = PI_2 * ImMin(1.f, t / 0.8f);        // spin over 0..80%, then hold

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float ang = (i & 1) ? -angle : angle;          // even/odd counter-rotate
        const float co = ImCos(ang), si = ImSin(ang);
        const int v0 = window->DrawList->VtxBuffer.Size;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), col, text + i, text + i + 1);
        ImDrawVert *vtx = window->DrawList->VtxBuffer.Data;
        for (int j = v0; j < window->DrawList->VtxBuffer.Size; j++) {
          const float dx = vtx[j].pos.x - cc.x, dy = vtx[j].pos.y - cc.y;
          vtx[j].pos.x = cc.x + dx * co - dy * si;
          vtx[j].pos.y = cc.y + dx * si + dy * co;
        }
        x += cw;
      }
    }

    // Vertical roll wave, an approximation of the CSS l30:
    //   alternating letters roll vertically by one line, half a cycle out of phase,
    //   so a vertical wave runs across the word. Each column rolls seamlessly (a wrap
    //   copy enters from above), clipped to the line box.
    inline void SpinnerTextRollWave(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor col = color_alpha(color, 1.f);
      const float H = ts.y;                                  // one line-height roll distance
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);

      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float ph = (i & 1) ? ImFmod(t + 0.5f, 1.f) : t; // odd columns half a cycle behind
        const float yo = ph * H;
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + yo), col, text + i, text + i + 1);         // rolling down
        window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + yo - H), col, text + i, text + i + 1);     // wrap copy from above
        x += cw;
      }
      window->DrawList->PopClipRect();
    }

    // Vibrating text, an approximation of the CSS l31:
    //   the wild cubic-bezier(.5,-150,.5,150) amplifies tiny per-letter offsets into a
    //   fast buzz. Here each letter jitters with its own incommensurate x/y frequency
    //   and phase, giving a continuous nervous vibration.
    inline void SpinnerTextVibrate(const char *label, float radius, const ImColor &color = white, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor col = color_alpha(color, 1.f);
      const float a = (float)ImGui::GetTime() * speed;
      const float A = font_size * 0.12f;                     // jitter amplitude

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        const float ox = A * ImSin(a * 38.f + i * 1.7f);     // per-letter, incommensurate x/y
        const float oy = A * ImSin(a * 47.f + i * 2.9f);
        window->DrawList->AddText(font, font_size, ImVec2(x + ox, tp.y + oy), col, text + i, text + i + 1);
        x += cw;
      }
    }

    // RGB-split glitch, an approximation of the CSS l32:
    //   three text copies (red/green/blue) with tiny offsets; the wild cubic-bezier
    //   amplifies them into a chromatic-aberration buzz. Here each channel jitters
    //   with its own phase, so the colors separate and shimmer. (Colors are fixed
    //   R/G/B, so there is no 'color' parameter.)
    inline void SpinnerTextGlitch(const char *label, float radius, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float a = (float)ImGui::GetTime() * speed;
      const float A = font_size * 0.06f;                     // split amplitude
      const int al = (int)(255.f * ImGui::GetStyle().Alpha);

      auto chan = [&](ImU32 col, float px, float py) {
        window->DrawList->AddText(font, font_size, ImVec2(tp.x + px, tp.y + py), col, text);
      };
      chan(IM_COL32(255, 0, 0, al), A * ImSin(a * 55.f + 0.0f), A * 0.6f * ImSin(a * 61.f + 1.0f));
      chan(IM_COL32(0, 255, 0, al), A * ImSin(a * 55.f + 2.1f), A * 0.6f * ImSin(a * 61.f + 3.2f));
      chan(IM_COL32(0, 0, 255, al), A * ImSin(a * 55.f + 4.2f), A * 0.6f * ImSin(a * 61.f + 5.3f));
    }

    // Random letter blur, an approximation of the CSS l33:
    //   single letters flicker out of focus (a text-shadow blur) one at a time during
    //   the middle of the cycle. ImGui can't blur, so a 'blurred' letter is faked with
    //   a translucent offset halo plus a dimmed centre. The schedule is the CSS table.
    inline void SpinnerTextBlur(const char *label, float radius, const ImColor &color = white, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float k = font_size / 30.f;                      // CSS blur px at 30px font
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);

      struct Pulse { int idx; float time; float blur; };
      static const Pulse pulses[] = {
        { 3, 0.20f, 9 }, { 7, 0.25f, 7 }, { 5, 0.30f, 9 }, { 9, 0.35f, 7 }, { 0, 0.40f, 5 },
        { 5, 0.45f, 7 }, { 4, 0.50f, 5 }, { 6, 0.55f, 8 }, { 8, 0.60f, 5 }, { 1, 0.65f, 7 }, { 2, 0.70f, 9 }
      };
      const int np = (int)(sizeof(pulses) / sizeof(pulses[0]));
      const float hw = 0.05f;                                // pulse half-width (keyframes are 5% apart)

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        float bl = 0.f;                                      // this letter's blur amount (px) right now
        for (int p = 0; p < np; p++)
          if (pulses[p].idx == i) {
            const float d = ImFabs(t - pulses[p].time);
            if (d < hw) bl = ImMax(bl, (1.f - d / hw) * pulses[p].blur);
          }
        if (bl > 0.05f) {
          const float r = bl * 0.4f * k;                     // halo spread
          const ImColor halo = color_alpha(color, 0.30f);
          window->DrawList->AddText(font, font_size, ImVec2(x + r, tp.y), halo, text + i, text + i + 1);
          window->DrawList->AddText(font, font_size, ImVec2(x - r, tp.y), halo, text + i, text + i + 1);
          window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + r), halo, text + i, text + i + 1);
          window->DrawList->AddText(font, font_size, ImVec2(x, tp.y - r), halo, text + i, text + i + 1);
          window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), color_alpha(color, 1.f - 0.5f * ImMin(1.f, bl / 9.f)), text + i, text + i + 1);
        } else {
          window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), color_alpha(color, 1.f), text + i, text + i + 1);
        }
        x += cw;
      }
    }

    // Character dropout glitch, a port of the CSS l34:
    //   individual characters blink out (become spaces) at scheduled times, so the
    //   word flickers with missing letters, then returns to whole. Char positions are
    //   preserved (monospace). The drop schedule is the CSS table (10-char "Loading...").
    inline void SpinnerTextDropout(const char *label, float radius, const ImColor &color = white, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor col = color_alpha(color, 1.f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);

      // bit i set => char i is dropped (space); one mask per 5% keyframe from 20%..70%.
      static const unsigned masks[11] = {
        0u, 1u << 2, 1u << 4, 1u << 0, 1u << 1, 1u << 6, 1u << 3,
        (1u << 7) | (1u << 8), (1u << 0) | (1u << 3), 1u << 1, (1u << 4) | (1u << 6)
      };
      unsigned mask = 0u;
      if (t >= 0.20f && t < 0.75f)
        mask = masks[ImMin(10, (int)((t - 0.20f) / 0.05f))];

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        if (!((mask >> i) & 1u))                             // draw unless this char is dropped
          window->DrawList->AddText(font, font_size, ImVec2(x, tp.y), col, text + i, text + i + 1);
        x += cw;
      }
    }

    // Scanline glitch, an approximation of the CSS l35 / l36:
    //   mode 0: a thin horizontal band scans vertically; the slice inside it is
    //           displaced horizontally (a fast jitter), the rest stays crisp.
    //   mode 1: a vertical band (1.5ch) scans horizontally; the slice is displaced
    //           vertically. A VHS/scanline displacement glitch either way.
    //   (CSS uses mask-composite; here clip regions.)
    inline void SpinnerTextScanline(const char *label, float radius, const ImColor &color = white, float speed = 1.f, int mode = 0, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor col = color_alpha(color, 1.f);
      const float k = font_size / 30.f;                      // CSS px at 30px font
      const float a = (float)ImGui::GetTime();
      const float t = ImFmod(a * speed, 1.f);
      const int seg = ImMin(4, (int)(t * 5.f));
      const float u = t * 5.f - seg;
      const float disp = 3.f * k * ImSin(a * speed * 60.f);  // fast displacement

      auto slice = [&](float lo, float hi, bool vertical) {
        // crisp text on both sides of [lo, hi], displaced text inside it
        if (vertical) {
          window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(lo, tp.y + ts.y), true);
          window->DrawList->AddText(font, font_size, tp, col, text);
          window->DrawList->PopClipRect();
          window->DrawList->PushClipRect(ImVec2(hi, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
          window->DrawList->AddText(font, font_size, tp, col, text);
          window->DrawList->PopClipRect();
          window->DrawList->PushClipRect(ImVec2(lo, tp.y), ImVec2(hi, tp.y + ts.y), true);
          window->DrawList->AddText(font, font_size, ImVec2(tp.x, tp.y + disp), col, text);
          window->DrawList->PopClipRect();
        } else {
          window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(tp.x + ts.x, lo), true);
          window->DrawList->AddText(font, font_size, tp, col, text);
          window->DrawList->PopClipRect();
          window->DrawList->PushClipRect(ImVec2(tp.x, hi), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
          window->DrawList->AddText(font, font_size, tp, col, text);
          window->DrawList->PopClipRect();
          window->DrawList->PushClipRect(ImVec2(tp.x, lo), ImVec2(tp.x + ts.x, hi), true);
          window->DrawList->AddText(font, font_size, ImVec2(tp.x + disp, tp.y), col, text);
          window->DrawList->PopClipRect();
        }
      };

      if (mode == 0) {
        // Horizontal band scanning vertically (l35).
        const float Yk[6] = { 20.f * k, 8.f * k, ts.y, 3.f * k, 15.f * k, 0.f };
        const float bh = 5.f * k;
        const float by = ImClamp(Yk[seg] + (Yk[seg + 1] - Yk[seg]) * u, 0.f, ImMax(0.f, ts.y - bh));
        slice(tp.y + by, tp.y + by + bh, false);
      } else {
        // Vertical band scanning horizontally (l36).
        const float ch = ts.x / (float)len;
        const float Xk[6] = { 0.f, 0.5f * ch, ts.x, 4.5f * ch, 6.5f * ch, 2.5f * ch };
        const float bw = 1.5f * ch;
        const float bx = ImClamp(Xk[seg] + (Xk[seg + 1] - Xk[seg]) * u, 0.f, ImMax(0.f, ts.x - bw));
        slice(tp.x + bx, tp.x + bx + bw, true);
      }
    }

    // Squash-slice scan, an approximation of the CSS l37:
    //   a vertical band (2ch) scans horizontally; inside the band the text is squashed
    //   vertically (scaleY 0.5), the rest stays crisp -- a scanning vertical compression.
    inline void SpinnerTextSquash(const char *label, float radius, const ImColor &color = white, float speed = 1.f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const ImColor col = color_alpha(color, 1.f);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float ch = ts.x / (float)len;

      // Band X schedule (ch positions; the 2nd/6th keyframes are 100% = ts.x).
      const float Xk[9] = { 1.f * ch, ts.x, 4.f * ch, 8.f * ch, 2.f * ch, ts.x, 0.f, 6.f * ch, 3.f * ch };
      const int seg = ImMin(7, (int)(t * 8.f));
      const float u = t * 8.f - seg;
      const float bw = 2.f * ch;
      const float bx = ImClamp(Xk[seg] + (Xk[seg + 1] - Xk[seg]) * u, 0.f, ImMax(0.f, ts.x - bw));
      const float bl = tp.x + bx, br = bl + bw;

      // Crisp text on both sides of the band.
      window->DrawList->PushClipRect(ImVec2(tp.x, tp.y), ImVec2(bl, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, tp, col, text);
      window->DrawList->PopClipRect();
      window->DrawList->PushClipRect(ImVec2(br, tp.y), ImVec2(tp.x + ts.x, tp.y + ts.y), true);
      window->DrawList->AddText(font, font_size, tp, col, text);
      window->DrawList->PopClipRect();

      // Vertically squashed slice inside the band.
      window->DrawList->PushClipRect(ImVec2(bl, tp.y), ImVec2(br, tp.y + ts.y), true);
      const int v0 = window->DrawList->VtxBuffer.Size;
      window->DrawList->AddText(font, font_size, tp, col, text);
      const float cy = tp.y + ts.y * 0.5f;
      ImDrawVert *vtx = window->DrawList->VtxBuffer.Data;
      for (int i = v0; i < window->DrawList->VtxBuffer.Size; i++)
        vtx[i].pos.y = cy + (vtx[i].pos.y - cy) * 0.5f;
      window->DrawList->PopClipRect();
    }

    // Scramble shuffle, a port of the CSS l38:
    //   the word snaps between 6 scrambled arrangements of "Loading..." (a slot-machine
    //   scramble) over the cycle (steps(6)); one of them is the readable word. The
    //   scrambles are baked in (they are authored for "Loading..."), so there is no
    //   'text' parameter.
    inline void SpinnerTextScramble(const char *label, float radius, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      static const char *lines[6] = {
        "Loading...", "godnLai...", "oiaglni...", "Liongad...", "gindola...", "naloidg..."
      };
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const char *text = lines[ImMin(5, (int)(t * 6.f))];    // snap to one scramble (steps(6))

      // Scale the font down so the full text fits the cell (all lines share length).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      window->DrawList->AddText(font, font_size, tp, color_alpha(color, 1.f), text);
    }

    // Decode/encode glitch, an approximation of the CSS l39:
    //   letters are progressively replaced by symbols left to right, then back
    //   (steps(11) alternate), like a decoding effect. The CSS uses rare APL/alchemy
    //   glyphs the default font lacks, so ASCII symbols are substituted here.
    inline void SpinnerTextDecode(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);
      static const char sym[] = "#@%&$*?+=~";
      const int symn = (int)(sizeof(sym) - 1);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float tri = (t < 0.5f) ? t * 2.f : 2.f - t * 2.f; // ping-pong (CSS alternate)
      const int k = ImMin(len, (int)(tri * (len + 1)));       // 0..len chars encoded

      char buf[256];
      const int n = ImMin(len, 255);
      for (int i = 0; i < n; i++) buf[i] = (i < k) ? sym[i % symn] : text[i];
      buf[n] = '\0';

      // Scale the font down so the full text fits the cell (length is constant).
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, buf);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, buf);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      window->DrawList->AddText(font, font_size, tp, color_alpha(color, 1.f), buf);
    }

    // Letters fly off, a port of the CSS l40:
    //   letters float up (-20px) and fade out one by one in a scattered order until
    //   the word is gone, then they all drop back in together. Then it repeats.
    inline void SpinnerTextVanish(const char *label, float radius, const ImColor &color = white, float speed = 0.5f, const char *text = "Loading...")
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      if (!text || !*text)
        return;

      const int len = (int)strlen(text);

      // Scale the font down so the full text fits the cell.
      ImFont *font = ImGui::GetFont();
      float font_size = ImGui::GetFontSize();
      const float max_width = radius * 2.f;
      ImVec2 ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      if (ts.x > max_width && ts.x > 0.f) {
        font_size *= max_width / ts.x;
        ts = font->CalcTextSizeA(font_size, 99999.f, 0.f, text);
      }

      const ImVec2 tp(centre.x - ts.x * 0.5f, centre.y - ts.y * 0.5f);
      const float k = font_size / 30.f;                      // CSS px at 30px font
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      static const float vt[10] = { 0.27f, 0.72f, 0.09f, 0.90f, 0.54f, 0.36f, 0.18f, 0.81f, 0.45f, 0.63f }; // per-letter vanish time

      float x = tp.x;
      for (int i = 0; i < len; i++) {
        const float cw = font->CalcTextSizeA(font_size, 99999.f, 0.f, text + i, text + i + 1).x;
        float y, a;
        if (t >= 0.90f) {                                    // all letters drop back in together
          const float rp = (t - 0.90f) / 0.10f;
          y = -20.f * k * (1.f - rp);
          a = rp;
        } else {
          const float tv = (i < 10) ? vt[i] : 1.0f;
          if (t < tv - 0.09f)      { y = 0.f;          a = 1.f; }                                       // still there
          else if (t < tv)         { const float p = (t - (tv - 0.09f)) / 0.09f; y = -20.f * k * p; a = 1.f - p; } // flying up + fading
          else                     { y = -20.f * k;    a = 0.f; }                                       // gone
        }
        if (a > 0.001f)
          window->DrawList->AddText(font, font_size, ImVec2(x, tp.y + y), color_alpha(color, a), text + i, text + i + 1);
        x += cw;
      }
    }
}

#endif // _IMSPINNER_TEXT_H_
