#ifndef _IMSPINNER_H_
#define _IMSPINNER_H_

/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021-2022 Dalerank
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#include <functional>
#include <array>
#include <map>

namespace ImSpinner
{
#define DECLPROP(name,type,def) struct name { type value = def; operator type() { return value; } };
    enum SpinnerTypeT {
      e_st_rainbow = 0,
      e_st_angle,
      e_st_dots,
      e_st_ang,
      e_st_vdots,
      e_st_bounce_ball,
      e_st_eclipse,
      e_st_ingyang,

      e_st_count
    };

    using float_ptr = float *;

    DECLPROP (SpinnerType, SpinnerTypeT, e_st_rainbow)
    DECLPROP (Radius, float, 16.f)
    DECLPROP (Speed, float, 1.f)
    DECLPROP (Thickness, float, 1.f)
    DECLPROP (Color, ImColor, 0xffffffff)
    DECLPROP (BgColor, ImColor, 0xffffffff)
    DECLPROP (AltColor, ImColor, 0xffffffff)
    DECLPROP (Angle, float, IM_PI)
    DECLPROP (AngleMin, float, IM_PI)
    DECLPROP (AngleMax, float, IM_PI)
    DECLPROP (FloatPtr, float_ptr, nullptr)
    DECLPROP (Dots, int, 0)
    DECLPROP (MiddleDots, int, 0)
    DECLPROP (MinThickness, float, 0.f)
    DECLPROP (Reverse, bool, false)
    DECLPROP (Delta, float, 0.f)
#undef DECLPROP

    namespace detail {
      bool SpinnerBegin(const char *label, float radius, ImVec2 &pos, ImVec2 &size, ImVec2 &centre, int &num_segments) {
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
          return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        pos = window->DC.CursorPos;
        size = ImVec2((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(bb, style.FramePadding.y);

        num_segments = window->DrawList->_CalcCircleAutoSegmentCount(radius);

        centre = bb.GetCenter();
        if (!ImGui::ItemAdd(bb, id))
          return false;

        return true;
      }

#define IMPLRPOP(basetype,type) basetype m_##type; \
                                void set##type(const basetype& v) { m_##type = v;} \
                                void set(type h) { m_##type = h.value;} \
                                template<typename First, typename... Args> \
                                void set(const type& h, const Args&... args) { set##type(h.value); this->template set<Args...>(args...); }
      struct SpinnerConfig {
        SpinnerConfig() {}
        template<typename none = void> void set() {}

        template<typename... Args>
        SpinnerConfig(const Args&... args) { this->template set<Args...>(args...); }

        IMPLRPOP(SpinnerTypeT, SpinnerType)
        IMPLRPOP(float, Radius)
        IMPLRPOP(float, Speed)
        IMPLRPOP(float, Thickness)
        IMPLRPOP(ImColor, Color)
        IMPLRPOP(ImColor, BgColor)
        IMPLRPOP(ImColor, AltColor)
        IMPLRPOP(float, Angle)
        IMPLRPOP(float, AngleMin)
        IMPLRPOP(float, AngleMax)
        IMPLRPOP(float_ptr, FloatPtr)
        IMPLRPOP(int, Dots)
        IMPLRPOP(int, MiddleDots)
        IMPLRPOP(float, MinThickness)
        IMPLRPOP(bool, Reverse)
        IMPLRPOP(float, Delta)
      };
#undef IMPLRPOP
    }

#define SPINNER_HEADER(pos, size, centre, num_segments) ImVec2 pos, size, centre; int num_segments; if (!detail::SpinnerBegin(label, radius, pos, size, centre, num_segments)) { return; }; ImGuiWindow *window = ImGui::GetCurrentWindow();

    void SpinnerRainbow(const char *label, float radius, float thickness, const ImColor &color, float speed, float ang_min = 0.f, float ang_max = IM_PI * 2.f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = ImAbs(ImSin((float)ImGui::GetTime() * 1.8f) * (num_segments - 5));
        const float a_min = ImMax(ang_min, IM_PI * 2.0f * ((float)start) / (float)num_segments);
        const float a_max = ImMin(ang_max, IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments);

        window->DrawList->PathClear();
        for (size_t i = 0; i < num_segments; i++)
        {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            const float rspeed = a + (float)ImGui::GetTime() * speed;
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(rspeed) * radius, centre.y + ImSin(rspeed) * radius));
        }
        window->DrawList->PathStroke(color, false, thickness);
    }

    void SpinnerAng(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, float angle = IM_PI)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime()* speed;
        const float bg_angle_offset = IM_PI * 2.f / num_segments;

        window->DrawList->PathClear();
        for (size_t i = 0; i <= num_segments; i++)
        {
            const float a = start + (i * bg_angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        window->DrawList->PathStroke(bg, false, thickness);

        window->DrawList->PathClear();
        const float angle_offset = angle / num_segments;
        for (size_t i = 0; i < num_segments; i++)
        {
            const float a = start + (i * angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        window->DrawList->PathStroke(color, false, thickness);
    }

    void SpinnerClock(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float bg_angle_offset = IM_PI * 2.f / num_segments;
      
      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments; i++)
      {
        const float a = start + (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
      }
      window->DrawList->PathStroke(bg, false, thickness);

      window->DrawList->AddLine(centre, ImVec2(centre.x + ImCos(start) * radius, centre.y + ImSin(start) * radius), color, thickness * 2);
      window->DrawList->AddLine(centre, ImVec2(centre.x + ImCos(start * 0.5f) * radius / 2.f, centre.y + ImSin(start * 0.5f) * radius / 2.f), color, thickness * 2);
    }

    void SpinnerPulsar(const char *label, float radius, float thickness, const ImColor &bg = 0xffffff80, float speed = 2.8f, bool sequence = true)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImGuiStorage* storage = window->DC.StateStorage;
      const ImGuiID radiusbId = window->GetID("##radiusb");
      float radius_b = storage->GetFloat(radiusbId, 0.8f);

      const float start = (float)ImGui::GetTime() * speed;
      const float bg_angle_offset = IM_PI * 2.f / num_segments;

      float start_r = ImFmod(start, IM_PI / 2.f);
      float radius_k = ImSin(start_r);
      float radius1 = radius_k * radius;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments; i++)
      {
        const float a = start + (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
      }
      window->DrawList->PathStroke(bg, false, thickness);

      if (sequence)
      {
        radius_b -= (0.005f * speed);
        radius_b = ImMax(radius_k, ImMax(0.8f, radius_b));
      } 
      else 
      {
        radius_b = (1 - radius_k);
      }
      storage->SetFloat(radiusbId, radius_b);
      
      float radius_tb = sequence ? ImMax(radius_k, radius_b) * radius : (radius_b * radius);
      
      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments; i++)
      {
        const float a = start + (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius_tb, centre.y + ImSin(a) * radius_tb));
      }
      window->DrawList->PathStroke(bg, false, thickness);
    }

    void SpinnerDoubleFadePulsar(const char *label, float radius, float /*thickness*/, const ImColor &bg = 0xffffff80, float speed = 2.8f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImGuiStorage* storage = window->DC.StateStorage;
      const ImGuiID radiusbId = window->GetID("##radiusb");
      float radius_b = storage->GetFloat(radiusbId, 0.8f);

      const float start = (float)ImGui::GetTime() * speed;
      const float bg_angle_offset = IM_PI * 2.f / num_segments;

      float start_r = ImFmod(start, IM_PI / 2.f);
      float radius_k = ImSin(start_r);
      float radius1 = radius_k * radius;
      ImColor c = bg;
      c.Value.w = ImMin(0.1f, radius_k);
      window->DrawList->AddCircleFilled(centre, radius1, c, num_segments);

      radius_b = (1 - radius_k);
      storage->SetFloat(radiusbId, radius_b);

      float radius_tb = radius_b * radius;
      c = bg;
      c.Value.w = ImMin(0.3f, radius_b);
      window->DrawList->AddCircleFilled(centre, radius_tb, c, num_segments);
    }

    void SpinnerTwinPulsar(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int rings = 2)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float bg_angle_offset = IM_PI * 2.f / num_segments;
      const float koeff = IM_PI / (2 * (float)rings);
      float start = (float)ImGui::GetTime() * speed;

      for (int num_ring = 0; num_ring < rings; ++num_ring)
      {
        window->DrawList->PathClear();
        float start_r = ImFmod(start + (num_ring * koeff), IM_PI / 2.f);
        float radius_k = ImSin(start_r);
        float radius1 = radius_k * radius;
        ImColor c = color;
        if (radius_k > 0.5f)
        {
          c.Value.w = 2.f - (radius_k * 2.f);
        }

        for (size_t i = 0; i <= num_segments; i++)
        {
          const float a = start + (i * bg_angle_offset);
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
        }
        window->DrawList->PathStroke(c, false, thickness);
      }
    }

    void SpinnerFadePulsar(const char *label, float radius, const ImColor &color = 0xffffffff, float speed = 2.8f, int rings = 2)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float bg_angle_offset = IM_PI * 2.f / num_segments;
      const float koeff = IM_PI / (2 * (float)rings);
      float start = (float)ImGui::GetTime() * speed;

      for (int num_ring = 0; num_ring < rings; ++num_ring)
      {
        float start_r = ImFmod(start + (num_ring * koeff), IM_PI / 2.f);
        float radius_k = ImSin(start_r);
        float radius1 = radius_k * radius;
        ImColor c = color;
        if (radius_k > 0.5f)
        {
          c.Value.w = 2.f - (radius_k * 2.f);
        }

        c.Value.w = (radius_k > 0.5f) ? (2.f - (radius_k * 2.f)) : c.Value.w;
        window->DrawList->AddCircleFilled(centre, radius1, c, num_segments);
      }
    }

    void SpinnerDots(const char *label, float *nextdot, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 12, float minth = -1.f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = ImMin(dots, (size_t)32);
        const size_t mdots = dots / 2;

        float def_nextdot = 0;
        float &ref_nextdot = nextdot ? *nextdot : def_nextdot;
        if (ref_nextdot < 0.f)
          ref_nextdot = (float)dots;

        auto thcorrect = [&thickness, &ref_nextdot, &mdots, &minth] (size_t i) {
            const float nth = minth < 0.f ? thickness / 2.f : minth;
            return ImMax(nth, ImSin(((i - ref_nextdot) / mdots) * IM_PI) * thickness);
        };

        for (size_t i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = ImFmod(a, 2 * IM_PI);
            float th = minth < 0 ? thickness / 2.f : minth;

            if (ref_nextdot + mdots < dots) {
                if (i > ref_nextdot && i < ref_nextdot + mdots)
                    th = thcorrect(i);
            } else {
                if ((i > ref_nextdot && i < dots) || (i < ((int)(ref_nextdot + mdots)) % dots))
                    th = thcorrect(i);
            }

            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(-a) * radius, centre.y + ImSin(-a) * radius), th, color, 8);
        }
    }

    void SpinnerVDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bgcolor = 0xffffffff, float speed = 2.8f, size_t dots = 12, size_t mdots = 6)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = ImMin(dots, (size_t)32);

        for (size_t i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = ImFmod(a, 2 * IM_PI);
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(-a) * radius, centre.y + ImSin(-a) * radius), thickness / 2, bgcolor, 8);
        }

        window->DrawList->PathClear();
        const float d_ang = (mdots / (float)dots) * 2 * IM_PI;
        const float angle_offset = (d_ang) / dots;
        for (size_t i = 0; i < dots; i++)
        {
            const float a = start + (i * angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        window->DrawList->PathStroke(color, false, thickness);
    }

    void SpinnerBounceDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const ImGuiStyle &style = GImGui->Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      const float hsize = dots * (thickness * nextItemKoeff) / 2.f - (thickness * nextItemKoeff) * 0.5f;

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ImGui::ItemSize(bb, style.FramePadding.y);
      if (!ImGui::ItemAdd(bb, id))
        return;

      float start = (float)ImGui::GetTime() * speed;

      const float offset = IM_PI / dots;
      for (size_t i = 0; i < dots; i++)
      {
        float a = start + (IM_PI - i * offset);
        float sina = ImSin(a * heightSpeed);
        float y = centre.y + sina * thickness * heightKoeff;
        if (y > centre.y)
          y = centre.y;
        window->DrawList->AddCircleFilled(ImVec2(centre.x - hsize + i * (thickness * nextItemKoeff), y), thickness, color, 8);
      }
    }

    void SpinnerFadeDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = ImGui::GetCurrentWindow();
      if (window->SkipItems)
        return;

      const ImGuiStyle &style = GImGui->Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ImGui::ItemSize(bb, style.FramePadding.y);
      if (!ImGui::ItemAdd(bb, id))
        return;

      const ImVec2 centre = bb.GetCenter();

      // Render
      float start = (float)ImGui::GetTime() * speed;

      const float offset = IM_PI / dots;
      for (size_t i = 0; i < dots; i++)
      {
        float a = start + (IM_PI - i * offset);
        ImColor c = color;
        c.Value.w = ImMax(0.1f, ImSin(a * heightSpeed));
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x  + i * (thickness * nextItemKoeff), centre.y), thickness, c, 8);
      }
    }

    void SpinnerScaleDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = ImGui::GetCurrentWindow();
      if (window->SkipItems)
        return;

      const ImGuiStyle &style = GImGui->Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ImGui::ItemSize(bb, style.FramePadding.y);
      if (!ImGui::ItemAdd(bb, id))
        return;

      const ImVec2 centre = bb.GetCenter();
      float start = (float)ImGui::GetTime() * speed;

      const float offset = IM_PI / dots;
      for (size_t i = 0; i < dots; i++)
      {
        const float a = start + (IM_PI - i * offset);
        const float th = thickness * ImSin(a * heightSpeed);
        ImColor fade_color = color;
        fade_color.Value.w = 0.1f;
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x  + i * (thickness * nextItemKoeff), centre.y), thickness, fade_color, 8);
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x  + i * (thickness * nextItemKoeff), centre.y), th, color, 8);
      }
    }

    void SpinnerMovingDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = ImGui::GetCurrentWindow();
      if (window->SkipItems)
        return;

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ImGui::ItemSize(bb, style.FramePadding.y);
      if (!ImGui::ItemAdd(bb, id))
        return;

      const ImVec2 centre = bb.GetCenter();
      const float start = ImFmod((float)ImGui::GetTime() * speed, size.x);

      float offset = 0;
      for (size_t i = 0; i < dots; i++)
      {
        const float a = start + (i * IM_PI / dots);
        float th = thickness;
        offset =  ImFmod(start + i * (size.x / dots), size.x);
        if (offset < thickness)
        {
          th = offset;
        }
        if (offset > size.x - thickness)
          th = size.x - offset;
        
        window->DrawList->AddCircleFilled(ImVec2(pos.x + offset - thickness, centre.y), th, color, 8);
      }
    }

    void SpinnerRotateDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int dots = 2)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImGuiStorage* storage = window->DC.StateStorage;
      const ImGuiID velocityId = window->GetID("##velocity");
      const ImGuiID vtimeId = window->GetID("##velocitytime");

      float velocity = storage->GetFloat(velocityId, 0.f);
      float vtime = storage->GetFloat(vtimeId, 0.f);
     
      float dtime = ImFmod((float)vtime, IM_PI);
      float start = (vtime += velocity);
      if (dtime > 0.f && dtime < IM_PI / 2.f)
      {
        velocity += 0.001f * speed;
      }
      else if (dtime > IM_PI * 0.9f && dtime < IM_PI)
      {
        velocity -= 0.01f * speed;
      }
      if (velocity > 0.1f) velocity = 0.1f;
      if (velocity < 0.01f) velocity = 0.01f;

      storage->SetFloat(velocityId, velocity);
      storage->SetFloat(vtimeId, vtime);

      window->DrawList->AddCircleFilled(centre, thickness, color, 8);

      const float angle_offset = (2 * IM_PI) / dots;
      for (int i = 0; i < dots; i++)
      {
        const float a = start + (i * angle_offset);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, color, 8);
      }
    }

    void SpinnerTwinAng(const char *label, float radius1, float radius2, float thickness, const ImColor &color1 = 0xffffffff, const ImColor &color2 = 0xff0000ff, float speed = 2.8f, float angle = IM_PI)
    {
      const float radius = ImMax(radius1, radius2);
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime() * speed, IM_PI * 2.f);
      const float aoffset = ImFmod((float)ImGui::GetTime(), 1.5f * IM_PI);
      const float bofsset = (aoffset > angle) ? angle : aoffset;
      const float angle_offset = angle * 2.f / num_segments;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= 2 * num_segments; i++)
      {
        const float a = start + (i * angle_offset);
        if (i * angle_offset > 2 * bofsset)
          break;
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
      }
      window->DrawList->PathStroke(color1, false, thickness);

      window->DrawList->PathClear();
      for (size_t i = 0; i < num_segments / 2; i++)
      {
        const float a = start + (i * angle_offset);
        if (i * angle_offset > bofsset)
          break;
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius2, centre.y + ImSin(a) * radius2));
      }
      window->DrawList->PathStroke(color2, false, thickness);
    }

    void SpinnerFilling(const char *label, float radius, float thickness, const ImColor &color1 = 0xffffffff, const ImColor &color2 = 0xff0000ff, float speed = 2.8f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime() * speed, IM_PI * 2.f);
      const float angle_offset = IM_PI * 2.f / num_segments;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= 2 * num_segments; i++)
      {
        const float a = (i * angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
      }
      window->DrawList->PathStroke(color1, false, thickness);

      window->DrawList->PathClear();
      for (size_t i = 0; i < 2 * num_segments / 2; i++)
      {
        const float a = (i * angle_offset);
        if (a > start)
          break;
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
      }
      window->DrawList->PathStroke(color2, false, thickness);
    }

    void SpinnerTopup(const char *label, float radius1, float radius2, const ImColor &color = 0xff0000ff, const ImColor &fg = 0xffffffff, const ImColor &bg = 0xffffffff, float speed = 2.8f)
    {
      const float radius = ImMax(radius1, radius2);
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime() * speed, IM_PI);
      window->DrawList->AddCircleFilled(centre, radius1, bg, num_segments);

      const float abegin = (IM_PI * 0.5f) - start;
      const float aend = (IM_PI * 0.5f) + start;
      const float angle_offset = (aend - abegin) / num_segments;

      window->DrawList->PathClear();
      window->DrawList->PathArcTo(centre, radius1, abegin, aend, num_segments * 2);
      window->DrawList->PathFillConvex(color);

      window->DrawList->AddCircleFilled(centre, radius2, fg, num_segments);
    }

    void SpinnerTwinAng180(const char *label, float radius1, float radius2, float thickness, const ImColor &color1 = 0xffffffff, const ImColor &color2 = 0xff0000ff, float speed = 2.8f)
    {
      const float radius = ImMax(radius1, radius2);
      SPINNER_HEADER(pos, size, centre, num_segments);

      num_segments *= 8;
      const float start = ImFmod((float)ImGui::GetTime()* speed, IM_PI * 2.f);
      const float aoffset = ImFmod((float)ImGui::GetTime(), 2.f * IM_PI);
      const float bofsset = (aoffset > IM_PI) ? IM_PI : aoffset;
      const float angle_offset = IM_PI * 2.f / num_segments;
      float ared_min = 0, ared = 0;
      if (aoffset > IM_PI)
        ared_min = aoffset - IM_PI;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments / 2 + 1; i++)
      {
        ared = start + (i * angle_offset);

        if (i * angle_offset < ared_min)
          continue;

        if (i * angle_offset > bofsset)
          break;

        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(ared) * radius2, centre.y + ImSin(ared) * radius2));
      }
      window->DrawList->PathStroke(color2, false, thickness);

      window->DrawList->PathClear();
      for (size_t i = 0; i <= 2 * num_segments + 1; i++)
      {
        const float a = ared + ared_min + (i * angle_offset);
        if (i * angle_offset < ared_min)
          continue;

        if (i * angle_offset > bofsset)
          break;
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
      }
      window->DrawList->PathStroke(color1, false, thickness);
    }

    void SpinnerTwinAng360(const char *label, float radius1, float radius2, float thickness, const ImColor &color1 = 0xffffffff, const ImColor &color2 = 0xff0000ff, float speed1 = 2.8f, float speed2 = 2.5f)
    {
      const float radius = ImMax(radius1, radius2);
      SPINNER_HEADER(pos, size, centre, num_segments);

      num_segments *= 4;
      const float start1 = ImFmod((float)ImGui::GetTime() * speed1, IM_PI * 2.f);
      const float start2 = ImFmod((float)ImGui::GetTime() * speed2, IM_PI * 2.f);
      const float aoffset = ImFmod((float)ImGui::GetTime(), 2.f * IM_PI);
      const float bofsset = (aoffset > IM_PI) ? IM_PI : aoffset;
      const float angle_offset = IM_PI * 2.f / num_segments;
      float ared_min = 0, ared = 0;
      if (aoffset > IM_PI)
        ared_min = aoffset - IM_PI;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments + 1; i++)
      {
        ared = start1 + (i * angle_offset);

        if (i * angle_offset < ared_min * 2)
          continue;

        if (i * angle_offset > bofsset * 2.f)
          break;

        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(ared) * radius2, centre.y + ImSin(ared) * radius2));
      }
      window->DrawList->PathStroke(color2, false, thickness);

      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments + 1; i++)
      {
        ared = start2 + (i * angle_offset);

        if (i * angle_offset < ared_min * 2)
          continue;

        if (i * angle_offset > bofsset * 2.f)
          break;

        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(-ared) * radius1, centre.y + ImSin(-ared) * radius1));
      }
      window->DrawList->PathStroke(color1, false, thickness);
    }

    void SpinnerIncDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 6)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI / dots);
      start -= astart;
      const float bg_angle_offset = IM_PI / dots;
      dots = ImMin<size_t>(dots, 32);

      for (size_t i = 0; i <= dots; i++)
      {
        float a = start + (i * bg_angle_offset);
        ImColor c = color;
        c.Value.w = ImMax(0.1f, i / (float)dots);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, c, 8);
      }
    }

    void SpinnerIncFullDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 4)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      dots = ImMin<size_t>(dots, 32);
      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI / dots);
      start -= astart;
      const float bg_angle_offset = IM_PI / dots;

      for (size_t i = 0; i < dots * 2; i++)
      {
        float a = start + (i * bg_angle_offset);
        ImColor c = color;
        c.Value.w = 0.1f;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, c, 8);
      }

      for (size_t i = 0; i < dots; i++)
      {
        float a = start + (i * bg_angle_offset);
        ImColor c = color;
        c.Value.w = ImMax(0.1f, i / (float)dots);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, c, 8);
      }
    }

    void SpinnerFadeBars(const char *label, float w, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t bars = 3, bool scale = false)
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
        ImColor c = color;
        c.Value.w = ImMax(0.1f, ImSin(a * heightSpeed));
        float h = (scale ? (0.6f + 0.4f * c.Value.w) : 1.f) * size.y / 2;
        window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + i * (w * nextItemKoeff) - w / 2, centre.y - h * yOffsetKoeftt),
                                        ImVec2(pos.x + style.FramePadding.x + i * (w * nextItemKoeff) + w / 2, centre.y + h * yOffsetKoeftt), c);
      }
    }

    void SpinnerBarsRotateFade(const char *label, float rmin, float rmax , float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t bars = 6)
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
        ImColor c = color;
        c.Value.w = ImMax(0.1f, i / (float)bars);
        window->DrawList->AddLine(ImVec2(centre.x + ImCos(a) * rmin, centre.y + ImSin(a) * rmin), ImVec2(centre.x + ImCos(a) * rmax, centre.y + ImSin(a) * rmax), c, thickness);
      }
    }

    void SpinnerBarsScaleMiddle(const char *label, float w, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t bars = 3)
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
                                        ImVec2(centre.x + style.FramePadding.x + i * (w * nextItemKoeff) + w / 2, centre.y + h * yOffsetKoeftt), color);
        if (i == 0)
          continue;

        window->DrawList->AddRectFilled(ImVec2(centre.x + style.FramePadding.x - i * (w * nextItemKoeff) - w / 2, centre.y - h * yOffsetKoeftt),
                                        ImVec2(centre.x + style.FramePadding.x - i * (w * nextItemKoeff) + w / 2, centre.y + h * yOffsetKoeftt), color);
      }
    }

    void SpinnerAngTwin(const char *label, float radius1, float radius2, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, float angle = IM_PI, size_t arcs = 1)
    {
      float radius = ImMax(radius1, radius2);
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float bg_angle_offset = IM_PI * 2.f / num_segments;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments; i++)
      {
        const float a = start + (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
      }
      window->DrawList->PathStroke(bg, false, thickness);

      const float angle_offset = angle / num_segments;
      for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
      {
          window->DrawList->PathClear();
          float arc_start = 2 * IM_PI / arcs;
          for (size_t i = 0; i < num_segments; i++)
          {
            const float a = arc_start * arc_num + start + (i * angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius2, centre.y + ImSin(a) * radius2));
          }
          window->DrawList->PathStroke(color, false, thickness);
      }
    }

    void SpinnerArcRotation(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t arcs = 4)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float arc_angle = 2.f * IM_PI / (float)arcs;
      const float angle_offset = arc_angle / num_segments;
      
      for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
      {
        window->DrawList->PathClear();
        ImColor c = color;
        c.Value.w = ImMax(0.1f, arc_num / (float)arcs);
        for (size_t i = 0; i <= num_segments; i++)
        {
          const float a = start + arc_angle * arc_num + (i * angle_offset);
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        window->DrawList->PathStroke(c, false, thickness);
      }
    }

    void SpinnerArcFade(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t arcs = 4)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime()* speed, IM_PI * 4.f);
      const float arc_angle = 2.f * IM_PI / (float)arcs;
      const float angle_offset = arc_angle / num_segments;
      
      for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
      {
        window->DrawList->PathClear();
        for (size_t i = 0; i <= num_segments + 1; i++)
        {
          const float a = arc_angle * arc_num + (i * angle_offset) - IM_PI / 2.f - IM_PI / 4.f;
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        const float a = arc_angle * arc_num;
        ImColor c = color;
        if (start < IM_PI * 2.f) {
          c.Value.w = 0.f;
          if (start > a && start < (a + arc_angle))
          {
            c.Value.w = 1.f - (start - a) / (float)arc_angle;
          }
          else if (start < a)
          {
            c.Value.w = 1.f;
          }
          c.Value.w = ImMax(0.05f, 1.f - c.Value.w);
        } else {
          const float startk = start - IM_PI * 2.f;
          c.Value.w = 0.f;
          if (startk > a && startk < (a + arc_angle))
          {
            c.Value.w = 1.f - (startk - a) / (float)arc_angle;
          }
          else if (startk < a)
          {
            c.Value.w = 1.f;
          }
          c.Value.w = ImMax(0.05f, c.Value.w);
        }
       
        window->DrawList->PathStroke(c, false, thickness);
      }
    }

    void SpinnerSquareStrokeFade(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = ImFmod((float)ImGui::GetTime()* speed, IM_PI * 4.f);
        const float arc_angle = 2.f * IM_PI / 4.f;
        const float ht = thickness / 2.f;

        for (size_t arc_num = 0; arc_num < 4; ++arc_num)
        {
            float a = arc_angle * arc_num;
            ImColor c = color;
            if (start < IM_PI * 2.f) {
                c.Value.w = (start > a && start < (a + arc_angle))
                                ? 1.f - (start - a) / (float)arc_angle
                                : (start < a ? 1.f : 0.f);
                c.Value.w = ImMax(0.05f, 1.f - c.Value.w);
            } else {
                const float startk = start - IM_PI * 2.f;
                c.Value.w = (startk > a && startk < (a + arc_angle))
                                ? 1.f - (startk - a) / (float)arc_angle
                                : (startk < a ? 1.f : 0.f);
                c.Value.w = ImMax(0.05f, c.Value.w);
            }
            a -= IM_PI / 4.f;
            const float r = radius * 1.4f;
            const bool right = ImSin(a) > 0;
            const bool top = ImCos(a) < 0;
            ImVec2 p1(centre.x + ImCos(a) * r, centre.y + ImSin(a) * r);
            ImVec2 p2(centre.x + ImCos(a - IM_PI / 2.f) * r, centre.y + ImSin(a - IM_PI / 2.f) * r);
            switch (arc_num) {
            case 0: p1.x -= ht; p2.x -= ht; break;
            case 1: p1.y -= ht; p2.y -= ht; break;
            case 2: p1.x += ht; p2.x += ht; break;
            case 3: p1.y += ht; p2.y += ht; break;
            }
            window->DrawList->AddLine(p1, p2, c, thickness);
        }
    }

    void SpinnerSquareStrokeFill(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = ImFmod((float)ImGui::GetTime()* speed, IM_PI * 2.f);
        const float arc_angle = 2.f * IM_PI / 4.f;
        const float ht = thickness / 2.f;

        for (size_t arc_num = 0; arc_num < 4; ++arc_num)
        {
            float a = arc_angle * arc_num;
            float segment_progress = (start > a && start < (a + arc_angle))
                                        ? 1.f - (start - a) / (float)arc_angle
                                        : (start < a ? 1.f : 0.f);
            a -= IM_PI / 4.f;
            segment_progress = 1.f - segment_progress;
            const float r = radius * 1.4f;
            const bool right = ImSin(a) > 0;
            const bool top = ImCos(a) < 0;
            ImVec2 p1(centre.x + ImCos(a - IM_PI / 2.f) * r, centre.y + ImSin(a - IM_PI / 2.f) * r);
            ImVec2 p2(centre.x + ImCos(a) * r, centre.y + ImSin(a) * r);
            switch (arc_num) {
            case 0: p1.x -= ht; p2.x -= ht; p2.x = p1.x + (p2.x - p1.x) * segment_progress; break;
            case 1: p1.y -= ht; p2.y -= ht; p2.y = p1.y + (p2.y - p1.y) * segment_progress; break;
            case 2: p1.x += ht; p2.x += ht; p2.x = p1.x + (p2.x - p1.x) * segment_progress; break;
            case 3: p1.y += ht; p2.y += ht; p2.y = p1.y + (p2.y - p1.y) * segment_progress; break;
            }
            window->DrawList->AddLine(p1, p2, color, thickness);
        }
    }

    void SpinnerFilledArcFade(const char *label, float radius, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t arcs = 4)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime()* speed, IM_PI * 4.f);
      const float arc_angle = 2.f * IM_PI / (float)arcs;
      const float angle_offset = arc_angle / num_segments;
      for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
      {
        const float b = arc_angle * arc_num - IM_PI / 2.f - IM_PI / 4.f;
        const float e = arc_angle * arc_num + arc_angle - IM_PI / 2.f - IM_PI / 4.f;
        const float a = arc_angle * arc_num;
        ImColor c = color;
        if (start < IM_PI * 2.f) {
          c.Value.w = 0.f;
          if (start > a && start < (a + arc_angle))
          {
            c.Value.w = 1.f - (start - a) / (float)arc_angle;
          }
          else if (start < a)
          {
            c.Value.w = 1.f;
          }
          c.Value.w = ImMax(0.f, 1.f - c.Value.w);
        }
        else
        {
          const float startk = start - IM_PI * 2.f;
          c.Value.w = 0.f;
          if (startk > a && startk < (a + arc_angle))
          {
            c.Value.w = 1.f - (startk - a) / (float)arc_angle;
          }
          else if (startk < a)
          {
            c.Value.w = 1.f;
          }
        }

        window->DrawList->PathClear();
        window->DrawList->PathLineTo(centre);
        for (size_t i = 0; i <= num_segments + 1; i++)
        {
          const float ar = arc_angle * arc_num + (i * angle_offset) - IM_PI / 2.f - IM_PI / 4.f;
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(ar) * radius, centre.y + ImSin(ar) * radius));
        }
        window->DrawList->PathFillConvex(c);
      }
    }

    void SpinnerFilledArcColor(const char *label, float radius, const ImColor &color = 0xffff0000, const ImColor &bg = 0xffffffff, float speed = 2.8f, size_t arcs = 4)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime()* speed, IM_PI * 2.f);
      const float arc_angle = 2.f * IM_PI / (float)arcs;
      const float angle_offset = arc_angle / num_segments;

      window->DrawList->AddCircleFilled(centre, radius, bg, num_segments * 2);
      for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
      {
        const float b = arc_angle * arc_num - IM_PI / 2.f;
        const float e = arc_angle * arc_num + arc_angle - IM_PI / 2.f;
        const float a = arc_angle * arc_num;

        ImColor c = color;
        c.Value.w = 0.f;
        if (start > a && start < (a + arc_angle))
        {
          c.Value.w = 1.f - (start - a) / (float)arc_angle;
        }
        else if (start < a)
        {
          c.Value.w = 1.f;
        }
        c.Value.w = ImMax(0.f, 1.f - c.Value.w);
        
        window->DrawList->PathClear();
        window->DrawList->PathLineTo(centre);
        for (size_t i = 0; i < num_segments + 1; i++)
        {
          const float ar = arc_angle * arc_num + (i * angle_offset) - IM_PI / 2.f;
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(ar) * radius, centre.y + ImSin(ar) * radius));
        }
        window->DrawList->PathFillConvex(c);
      }
    }

    void SpinnerArcWedges(const char *label, float radius, const ImColor &color = 0xffff0000, float speed = 2.8f, size_t arcs = 4)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        const float arc_angle = 2.f * IM_PI / (float)arcs;
        const float angle_offset = arc_angle / num_segments;
        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);

        for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
        {
            const float b = arc_angle * arc_num - IM_PI / 2.f;
            const float e = arc_angle * arc_num + arc_angle - IM_PI / 2.f;
            const float a = arc_angle * arc_num;

            window->DrawList->PathClear();
            window->DrawList->PathLineTo(centre);
            for (size_t i = 0; i < num_segments + 1; i++)
            {
                const float start_a = ImFmod(start * (1.05f * (arc_num + 1)), IM_PI * 2.f);
                const float ar = start_a + arc_angle * arc_num + (i * angle_offset) - IM_PI / 2.f;
                window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(ar) * radius, centre.y + ImSin(ar) * radius));
            }
            window->DrawList->PathFillConvex(ImColor::HSV(out_h + (1.f / arcs) * arc_num, out_s, out_v, 0.7f));
        }
    }

    void SpinnerTwinBall(const char *label, float radius1, float radius2, float thickness, float b_thickness, const ImColor &ball = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, size_t balls = 2)
    {
      float radius = ImMax(radius1, radius2);
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float bg_angle_offset = IM_PI * 2.f / num_segments;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments; i++)
      {
        const float a = start + (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
      }
      window->DrawList->PathStroke(bg, false, thickness);

      for (size_t b_num = 0; b_num < balls; ++b_num)
      {
        float b_start = 2 * IM_PI / balls;
        const float a = b_start * b_num + start;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius2, centre.y + ImSin(a) * radius2), b_thickness, ball);
      }
    }

    void SpinnerBounceBall(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int dots = 1, bool shadow = false)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImGuiStorage* storage = window->DC.StateStorage;
      const ImGuiID vtimeId = window->GetID("##vtime");
      const ImGuiID hmaxId = window->GetID("##hmax");

      float vtime = storage->GetFloat(vtimeId, 0.f);
      float hmax = storage->GetFloat(hmaxId, 1.f);

      vtime += 0.05f;
      hmax += 0.01f;

      storage->SetFloat(vtimeId, vtime);
      storage->SetFloat(hmaxId, hmax);

      constexpr float rkoeff[9] = {0.1f, 0.15f, 0.17f, 0.25f, 0.31f, 0.19f, 0.08f, 0.24f, 0.9f};
      const int iterations = shadow ? 4 : 1;
      for (int j = 0; j < iterations; j++) {
          ImColor c = color;
          c.Value.w -= 0.15f * j;
          for (int i = 0; i < dots; i++) {
              float start = ImFmod((float)ImGui::GetTime() * speed * (1 + rkoeff[i % 9]) - (IM_PI / 12.f) * j, IM_PI);
              float sign = ((i % 2 == 0) ? 1 : -1);
              float offset = (i == 0) ? 0.f : (floor((i+1) / 2.f + 0.1f) * sign * 2.f * thickness);
              float maxht = ImMax(ImSin(ImFmod(hmax, IM_PI)), (0.7f + rkoeff[i % 9])) * radius;
              window->DrawList->AddCircleFilled(ImVec2(centre.x + offset, centre.y + radius - ImSin(start) * 2.f * maxht), thickness, c, 8);
          }
      }
    }

    void SpinnerIncScaleDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 6)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI / dots);
      start -= astart;
      const float bg_angle_offset = IM_PI / dots;
      dots = ImMin(dots, (size_t)32);

      for (size_t i = 0; i <= dots; i++)
      {
        float a = start + (i * bg_angle_offset);
        float th = thickness * ImMax(0.1f, i / (float)dots);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), th, color, 8);
      }
    }

    void SpinnerAngTriple(const char *label, float radius1, float radius2, float radius3, float thickness, const ImColor &c1 = 0xffffffff, const ImColor &c2 = 0xffffff80, const ImColor &c3 = 0xffffffff, float speed = 2.8f, float angle = IM_PI)
    {
      float radius = ImMax(ImMax(radius1, radius2), radius3);
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start1 = (float)ImGui::GetTime() * speed;
      const float angle_offset = angle / num_segments;

      window->DrawList->PathClear();
      for (size_t i = 0; i < num_segments; i++)
      {
        const float a = start1 + (i * angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
      }
      window->DrawList->PathStroke(c1, false, thickness);

      float start2 = (float)ImGui::GetTime() * 1.2f * speed;
      window->DrawList->PathClear();
      for (size_t i = 0; i < num_segments; i++)
      {
        const float a = start2 + (i * angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(-a) * radius2, centre.y + ImSin(-a) * radius2));
      }
      window->DrawList->PathStroke(c2, false, thickness);

      float start3 = (float)ImGui::GetTime() * 0.9f * speed;
      window->DrawList->PathClear();
      for (size_t i = 0; i < num_segments; i++)
      {
        const float a = start3 + (i * angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius3, centre.y + ImSin(a) * radius3));
      }
      window->DrawList->PathStroke(c3, false, thickness);
    }

    void SpinnerAngEclipse(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, float angle = IM_PI)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float angle_offset = angle / num_segments;
      const float th = thickness / num_segments;

      for (size_t i = 0; i < num_segments; i++)
      {
        const float a = start + (i * angle_offset);
        const float a1 = start + ((i+1) * angle_offset);
        window->DrawList->AddLine(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius),
                                  ImVec2(centre.x + ImCos(a1) * radius, centre.y + ImSin(a1) * radius),
                                  color,
                                  th * i);
      }
    }

    void SpinnerIngYang(const char *label, float radius, float thickness, bool reverse, float yang_detlta_r, const ImColor &colorI = 0xffffffff, const ImColor &colorY = 0xffffffff, float speed = 2.8f, float angle = IM_PI * 0.7f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float startI = (float)ImGui::GetTime() * speed;
      const float startY = (float)ImGui::GetTime() * (speed + (yang_detlta_r > 0.f ? ImClamp(yang_detlta_r * 0.5f, 0.5f, 2.f) : 0.f));
      const float angle_offset = angle / num_segments;
      const float th = thickness / num_segments;

      for (int i = 0; i < num_segments; i++)
      {
        const float a = startI + (i * angle_offset);
        const float a1 = startI + ((i + 1) * angle_offset);
        window->DrawList->AddLine(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius),
                                  ImVec2(centre.x + ImCos(a1) * radius, centre.y + ImSin(a1) * radius),
                                  colorI,
                                  th * i);
      }
      const float ai_end = startI + (num_segments * angle_offset);
      ImVec2 circle_i_center{centre.x + ImCos(ai_end) * radius, centre.y + ImSin(ai_end) * radius};
      window->DrawList->AddCircleFilled(circle_i_center, thickness / 2.f, colorI, num_segments);

      const float rv = reverse ? -1.f : 1.f;
      const float yang_radius = (radius - yang_detlta_r);
      for (int i = 0; i < num_segments; i++)
      {
        const float a = startY + IM_PI + (i * angle_offset);
        const float a1 = startY + IM_PI + ((i+1) * angle_offset);
        window->DrawList->AddLine(ImVec2(centre.x + ImCos(a * rv) * yang_radius, centre.y + ImSin(a * rv) * yang_radius),
                                  ImVec2(centre.x + ImCos(a1 * rv) * yang_radius, centre.y + ImSin(a1 * rv) * yang_radius),
                                  colorY,
                                  th * i);
      }
      const float ay_end = startY + IM_PI + (num_segments * angle_offset);
      ImVec2 circle_y_center{centre.x + ImCos(ay_end * rv) * yang_radius, centre.y + ImSin(ay_end * rv) * yang_radius};
      window->DrawList->AddCircleFilled(circle_y_center, thickness / 2.f, colorY, num_segments);
    }


    void SpinnerGooeyBalls(const char *label, float radius, const ImColor &color, float speed)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime() * speed, IM_PI);
      const float radius1 = (0.3f + 0.3f * ImSin(start)) * radius;
      const float radius2 = radius - radius1;

      window->DrawList->AddCircleFilled(ImVec2(centre.x - radius + radius1, centre.y), radius1, color, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x - radius + radius1 * 1.2f + radius2, centre.y), radius2, color, num_segments);
    }

    void SpinnerRotateGooeyBalls(const char *label, float radius, float thickness, const ImColor &color, float speed, int balls)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime(), IM_PI);
      const float rstart = ImFmod((float)ImGui::GetTime() * speed, IM_PI * 2);
      const float radius1 = (0.2f + 0.3f * ImSin(start)) * radius;
      const float angle_offset = IM_PI * 2.f / balls;

      for (int i = 0; i <= balls; i++)
      {
        const float a = rstart + (i * angle_offset);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1), thickness, color, num_segments);
      }
    }

    void SpinnerRotateTriangles(const char *label, float radius, float thickness, const ImColor &color, float speed, int tris)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime(), IM_PI);
      const float rstart = ImFmod((float)ImGui::GetTime() * speed, IM_PI * 2);
      const float radius1 = radius / 2.5f;
      const float angle_offset = IM_PI * 2.f / tris;

      for (int i = 0; i <= tris; i++)
      {
        const float a = rstart + (i * angle_offset);
        ImVec2 tri_centre(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1);
        ImVec2 p1(tri_centre.x + ImCos(-a) * radius1, tri_centre.y + ImSin(-a) * radius1);
        ImVec2 p2(tri_centre.x + ImCos(-a + 2.f * IM_PI / 3.f) * radius1, tri_centre.y + ImSin(-a + 2.f * IM_PI / 3.f) * radius1);
        ImVec2 p3(tri_centre.x + ImCos(-a - 2.f * IM_PI / 3.f) * radius1, tri_centre.y + ImSin(-a - 2.f * IM_PI / 3.f) * radius1);
        window->DrawList->AddTriangleFilled(p1, p2, p3, color);
      }
    }

    void SpinnerMoonLine(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xff000000, float speed = 2.8f, float angle = IM_PI)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float angle_offset = (angle * 0.5f) / num_segments;
      const float th = thickness / num_segments;

      window->DrawList->AddCircleFilled(centre, radius, bg, num_segments);

      auto draw_gradient = [&] (auto b, auto e, auto th) {
        for (int i = 0; i < num_segments; i++)
        {
          window->DrawList->AddLine(ImVec2(centre.x + ImCos(start + b(i)) * radius, centre.y + ImSin(start + b(i)) * radius),
                                    ImVec2(centre.x + ImCos(start + e(i)) * radius, centre.y + ImSin(start + e(i)) * radius),
                                    color,
                                    th(i));
        }

      };

      draw_gradient([&] (auto i) { return (num_segments + i) * angle_offset; },
                    [&] (auto i) { return (num_segments + i + 1) * angle_offset; },
                    [&] (auto i) { return thickness - th * i; });

      draw_gradient([&] (auto i) { return (i) * angle_offset; },
                    [&] (auto i) { return (i + 1) * angle_offset; },
                    [&] (auto i) { return th * i; });

      draw_gradient([&] (auto i) { return (num_segments + i) * angle_offset; },
                    [&] (auto i) { return (num_segments + i + 1) * angle_offset; },
                    [&] (auto i) { return thickness - th * i; });

      const float b_angle_offset = (2.f * IM_PI - angle) / num_segments; 
      draw_gradient([&] (auto i) { return num_segments * angle_offset * 2.f + (i * b_angle_offset); },
                    [&] (auto i) { return num_segments * angle_offset * 2.f + ((i + 1) * b_angle_offset); },
                    [] (auto) { return 1.f; });
    }

    void SpinnerCircleDrop(const char *label, float radius, float thickness, float thickness_drop, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, float angle = IM_PI) 
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime() * speed;
      const float bg_angle_offset = IM_PI * 2.f / num_segments;
      const float angle_offset = angle / num_segments;
      const float th = thickness_drop / num_segments;
      const float drop_radius_th = thickness_drop / num_segments;

      for (int i = 0; i < num_segments; i++)
      {
        const float a = start + (i * angle_offset);
        const float a1 = start + ((i + 1) * angle_offset);
        const float s_drop_radius = radius - thickness / 2.f - (drop_radius_th * i);
        window->DrawList->AddLine(ImVec2(centre.x + ImCos(a) * s_drop_radius, centre.y + ImSin(a) * s_drop_radius),
                                  ImVec2(centre.x + ImCos(a1) * s_drop_radius, centre.y + ImSin(a1) * s_drop_radius),
                                  color,
                                  th * 2.f * i);
      }
      const float ai_end = start + (num_segments * angle_offset);
      const float f_drop_radius = radius - thickness / 2.f - thickness_drop;
      ImVec2 circle_i_center{centre.x + ImCos(ai_end) * f_drop_radius, centre.y + ImSin(ai_end) * f_drop_radius};
      window->DrawList->AddCircleFilled(circle_i_center, thickness_drop, color, num_segments);

      window->DrawList->PathClear();
      for (int i = 0; i <= num_segments; i++)
      {
        const float a = (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
      }
      window->DrawList->PathStroke(bg, false, thickness);
    }

    void SpinnerSurroundedIndicator(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f) 
    {
      SPINNER_HEADER(pos, size, centre, num_segments);


      ImColor c = color;
      float lerp_koeff = (ImSin((float)ImGui::GetTime() * speed) + 1.f) * 0.5f;
      c.Value.w = ImMax(0.1f, ImMin(lerp_koeff, 1.f));
      window->DrawList->AddCircleFilled(centre, thickness, bg, num_segments);
      window->DrawList->AddCircleFilled(centre, thickness, c, num_segments);

      auto PathArc = [&] {
        const float bg_angle_offset = IM_PI * 2.f / num_segments;
        for (int i = 0; i <= num_segments; i++) {
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(i * bg_angle_offset) * radius, centre.y + ImSin(i * bg_angle_offset) * radius));
        }
      };
      window->DrawList->PathClear();
      PathArc();
      window->DrawList->PathStroke(bg, false, thickness);

      PathArc();
      lerp_koeff = (ImSin((float)ImGui::GetTime() * speed * 1.6f) + 1.f) * 0.5f;
      c.Value.w = 1.f - ImMax(0.1f, ImMin(lerp_koeff, 1.f));;
      window->DrawList->PathStroke(c, false, thickness);
    }

    void SpinnerTrianglesSeletor(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, size_t bars = 8) 
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImColor c = color;
      float lerp_koeff = (ImSin((float)ImGui::GetTime() * speed) + 1.f) * 0.5f;
      c.Value.w = ImMax(0.1f, ImMin(lerp_koeff, 1.f));
      float dr = radius - thickness - 3;
      window->DrawList->AddCircleFilled(centre, dr, bg, num_segments);
      window->DrawList->AddCircleFilled(centre, dr, c, num_segments);

      // Render
      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI * 2 / bars);
      start -= astart;
      const float angle_offset = IM_PI * 2 / bars;
      const float angle_offset_t = angle_offset * 0.3f;
      bars = ImMin<size_t>(bars, 32);

      const float rmin = radius - thickness;
      auto get_points = [&] (auto left, auto right) -> std::array<ImVec2, 4> {
        return {
          ImVec2(centre.x + ImCos(left) * rmin, centre.y + ImSin(left) * rmin),
          ImVec2(centre.x + ImCos(left) * radius, centre.y + ImSin(left) * radius),
          ImVec2(centre.x + ImCos(right) * radius, centre.y + ImSin(right) * radius),
          ImVec2(centre.x + ImCos(right) * rmin, centre.y + ImSin(right) * rmin)
        };
      };

      auto draw_sectors = [&] (auto s, auto color_func) {
        for (size_t i = 0; i <= bars; i++) {
          float left = s + (i * angle_offset) - angle_offset_t;
          float right = s + (i * angle_offset) + angle_offset_t;
          auto points = get_points(left, right);
          window->DrawList->AddConvexPolyFilled(points.data(), 4, color_func(i));
        }
      };

      draw_sectors(0, [&] (auto) { ImColor rc = bg; rc.Value.w = 0.1f; return rc; });
      draw_sectors(start, [&] (auto i) { ImColor rc = bg; rc.Value.w = (i / (float)bars) - 0.5f; return rc; });
    }

    using LeafColor = ImColor (int);
    void SpinnerCamera(const char *label, float radius, float thickness, LeafColor *leaf_color, float speed = 2.8f, size_t bars = 8) 
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime() * speed;
      const float angle_offset = IM_PI * 2 / bars;
      const float angle_offset_t = angle_offset * 0.3f;
      bars = ImMin<size_t>(bars, 32);

      const float rmin = radius - thickness - 1;
      auto get_points = [&] (auto left, auto right) -> std::array<ImVec2, 4> {
        return {
          ImVec2(centre.x + ImCos(left - 0.1f) * radius, centre.y + ImSin(left - 0.1f) * radius),
          ImVec2(centre.x + ImCos(right + 0.15f) * radius, centre.y + ImSin(right + 0.15f) * radius),
          ImVec2(centre.x + ImCos(right - 0.91f) * rmin, centre.y + ImSin(right - 0.91f) * rmin)
        };
      };

      auto draw_sectors = [&] (auto s, auto color_func) {
        for (size_t i = 0; i <= bars; i++) {
          float left = s + (i * angle_offset) - angle_offset_t;
          float right = s + (i * angle_offset) + angle_offset_t;
          auto points = get_points(left, right);
          window->DrawList->AddConvexPolyFilled(points.data(), 3, color_func((int)i));
        }
      };

      draw_sectors(start, leaf_color);
    }

    void SpinnerFlowingGradient(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xff000000, float speed = 2.8f, float angle = IM_PI)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float angle_offset = (angle * 0.5f) / num_segments;
      const float bg_angle_offset = (IM_PI * 2.f) / num_segments;
      const float th = thickness / num_segments;

      for (size_t i = 0; i <= num_segments; i++)
      {
        const float a = (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
      }
      window->DrawList->PathStroke(bg, false, thickness);

      auto draw_gradient = [&] (auto b, auto e, auto c) {
        for (size_t i = 0; i < num_segments; i++)
        {
          window->DrawList->AddLine(ImVec2(centre.x + ImCos(start + b(i)) * radius, centre.y + ImSin(start + b(i)) * radius),
                                    ImVec2(centre.x + ImCos(start + e(i)) * radius, centre.y + ImSin(start + e(i)) * radius),
                                    c(i),
                                    thickness);
        }
      };

      draw_gradient([&] (auto i) { return (i) * angle_offset; },
                    [&] (auto i) { return (i + 1) * angle_offset; },
                    [&] (auto i) { ImColor rc = color; rc.Value.w = (i / (float)num_segments); return rc; });

      draw_gradient([&] (auto i) { return (num_segments + i) * angle_offset; },
                    [&] (auto i) { return (num_segments + i + 1) * angle_offset; },
                    [&] (auto i) { ImColor rc = color; rc.Value.w = 1.f - (i / (float)num_segments); return rc; });
    }

    void SpinnerRotateSegments(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t arcs = 4, size_t layers = 1)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float arc_angle = 2.f * IM_PI / (float)arcs;
      const float angle_offset = arc_angle / num_segments;
      float r = radius;
      float reverse = 1.f;
      for (size_t layer = 0; layer < layers; layer++)
      {
        for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
        {
          window->DrawList->PathClear();
          for (size_t i = 2; i <= num_segments - 2; i++)
          {
            const float a = start * (1 + 0.1f * layer) + arc_angle * arc_num + (i * angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a * reverse) * r, centre.y + ImSin(a * reverse) * r));
          }
          window->DrawList->PathStroke(color, false, thickness);
        }

        r -= (thickness + 1);
        reverse *= -1.f;
      }
    }

    void SpinnerLemniscate(const char* label, float radius, float thickness, const ImColor& color = 0xffffffff, float speed = 2.8f, float angle = IM_PI / 2.0f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime() * speed;
      const float a = radius;
      const float t = start;
      const float step = angle / num_segments;
      const float th = thickness / num_segments;

      auto get_coord = [&](float const& a, float const& t) -> auto {
          return std::make_pair((a * ImCos(t)) / (1 + (powf(ImSin(t), 2.0f))), (a * ImSin(t) * ImCos(t)) / (1 + (powf(ImSin(t), 2.0f))));
      };

      for (size_t i = 0; i < num_segments; i++)
      {
          const auto xy0 = get_coord(a, start + (i * step));
          const auto xy1 = get_coord(a, start + ((i + 1) * step));
      
          window->DrawList->AddLine(ImVec2(centre.x + xy0.first, centre.y + xy0.second),
              ImVec2(centre.x + xy1.first, centre.y + xy1.second),
              color,
              th * i);
      }
    }

    void SpinnerRotateGear(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t pins = 12)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      const float bg_angle_offset = IM_PI * 2.f / num_segments;
      const float bg_radius = radius - thickness;

      window->DrawList->PathClear();
      for (size_t i = 0; i <= num_segments; i++)
      {
        const float a = (i * bg_angle_offset);
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * bg_radius, centre.y + ImSin(a) * bg_radius));
      }
      window->DrawList->PathStroke(color, false, bg_radius / 2);

      const float rmin = bg_radius;
      const float rmax = radius;
      const float pin_angle_offset = IM_PI * 2.f / pins;
      for (size_t i = 0; i <= pins; i++)
      {
        float a = start + (i * pin_angle_offset);
        window->DrawList->AddLine(ImVec2(centre.x + ImCos(a) * rmin, centre.y + ImSin(a) * rmin), ImVec2(centre.x + ImCos(a) * rmax, centre.y + ImSin(a) * rmax), color, thickness);
      }
    }

    void SpinnerAtom(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int elipses = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      elipses = std::min<int>(elipses, 3);

      auto draw_rotated_ellipse = [&] (float alpha, float start) {
        std::array<ImVec2, 36> pts;

        alpha = ImFmod(alpha, IM_PI);
        float a = radius;
        float b = radius / 2.f; 

        const float bg_angle_offset = IM_PI * 2.f / num_segments;
        for (int i = 0; i < num_segments; ++i) {
          float anga = (i * bg_angle_offset);

          pts[i].x = a * ImCos(anga) * ImCos(alpha) + b * ImSin(anga) * ImSin(alpha) + centre.x;
          pts[i].y = b * ImSin(anga) * ImCos(alpha) - a * ImCos(anga) * ImSin(alpha) + centre.y;
        }
        for (int i = 1; i < num_segments; ++i) {
          window->DrawList->AddLine(pts[i-1], pts[i], color, thickness);
        }
        window->DrawList->AddLine(pts[num_segments-1], pts[0], color, thickness);

        float anga = ImFmod(start, IM_PI * 2);
        float x = a * ImCos(anga) * ImCos(alpha) + b * ImSin(anga) * ImSin(alpha) + centre.x;
        float y = b * ImSin(anga) * ImCos(alpha) - a * ImCos(anga) * ImSin(alpha) + centre.y;
        return ImVec2{x, y};
      };

      ImVec2 ppos[3];
      for (int i = 0; i < elipses; ++i) {
        ppos[i % 3] = draw_rotated_ellipse((IM_PI * (float)i/ elipses), start * (1.f + 0.1f * i));
      }

      ImColor pcolors[3] = {ImColor(255, 0, 0), ImColor(0, 255, 0), ImColor(0, 0, 255)};
      for (int i = 0; i < elipses; ++i) {
        window->DrawList->AddCircleFilled(ppos[i], thickness * 2, pcolors[i], int(num_segments / 3.f));
      }
    }

    void SpinnerRotatedAtom(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int elipses = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = (float)ImGui::GetTime()* speed;
      auto draw_rotated_ellipse = [&] (float alpha) {
        std::array<ImVec2, 36> pts;

        alpha = ImFmod(alpha, IM_PI);
        float a = radius;
        float b = radius / 2.f; 

        const float bg_angle_offset = IM_PI * 2.f / num_segments;
        for (size_t i = 0; i < num_segments; ++i) {
          float anga = (i * bg_angle_offset);

          pts[i].x = a * ImCos(anga) * ImCos(alpha) + b * ImSin(anga) * ImSin(alpha) + centre.x;
          pts[i].y = b * ImSin(anga) * ImCos(alpha) - a * ImCos(anga) * ImSin(alpha) + centre.y;
        }
        for (size_t i = 1; i < num_segments; ++i) {
          window->DrawList->AddLine(pts[i-1], pts[i], color, thickness);
        }
        window->DrawList->AddLine(pts[num_segments-1], pts[0], color, thickness);
      };

      for (int i = 0; i < elipses; ++i) {
        draw_rotated_ellipse(start + (IM_PI * (float)i/ elipses));
      }
    }

    void SpinnerRainbowBalls(const char *label, float radius, float thickness, const ImColor &color, float speed, int balls = 5)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float start = ImFmod((float)ImGui::GetTime() * speed * 3.f, IM_PI);
      const float colorback = 0.3f + 0.2f * ImSin((float)ImGui::GetTime() * speed);
      const float rstart = ImFmod((float)ImGui::GetTime() * speed, IM_PI * 2);
      const float radius1 = (0.8f + 0.2f * ImSin(start)) * radius;
      const float angle_offset = IM_PI * 2.f / balls;
      const bool rainbow = ((ImU32)color.Value.w) == 0;

      float out_h, out_s, out_v;
      ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
      for (int i = 0; i <= balls; i++)
      {
        const float a = rstart + (i * angle_offset);
        ImColor c = rainbow ? ImColor::HSV(out_h + i * (1.f / balls) + colorback, out_s, out_v) : color;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1), thickness, c, num_segments);
      }
    }

    void SpinnerBarChartSine(const char *label, float radius, float thickness, const ImColor &color, float speed, int bars = 5, int mode = 0)
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
        float a = start + (IM_PI - i * offset);
        ImColor c = color;
        c.Value.w = ImMax(0.1f, ImSin(a * heightSpeed));
        float h = mode ? ImSin(a) * size.y / 2.f
                       : (0.6f + 0.4f * c.Value.w) * size.y;
        float halfs = mode ? 0 : size.y / 2.f;
        window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) - thickness / 2, centre.y + halfs),
                                        ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) + thickness / 2, centre.y + halfs - h * yOffsetKoeftt), c);
      }
    }

    void SpinnerBarChartRainbow(const char *label, float radius, float thickness, const ImColor &color, float speed, int bars = 5)
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
        window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) - thickness / 2, centre.y + size.y / 2.f),
                                        ImVec2(pos.x + style.FramePadding.x + i * (thickness * nextItemKoeff) + thickness / 2, centre.y + size.y / 2.f - h * yOffsetKoeftt), c);
      }
    }

    void SpinnerBlocks(const char *label, float radius, float thickness, const ImColor &bg, const ImColor &color, float speed)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImVec2 lt{centre.x - radius, centre.y - radius};
      const float offset_block = radius * 2.f / 3.f;

      int start = (int)ImFmod((float)ImGui::GetTime() * speed, 8.f);

      const ImVec2ih poses[] = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {1, 2}, {0, 2}, {0, 1}};

      int ti = 0;
      for (const auto &rpos: poses)
      {
          const ImColor &c = (ti == start) ? color : bg;
          window->DrawList->AddRectFilled(ImVec2(lt.x + rpos.x * (offset_block), lt.y + rpos.y * offset_block),
                                          ImVec2(lt.x + rpos.x * (offset_block) + thickness, lt.y + rpos.y * offset_block + thickness), c);
          ti++;
      }
    }

    void SpinnerScaleBlocks(const char *label, float radius, float thickness, const ImColor &color, float speed)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImVec2 lt{centre.x - radius, centre.y - radius};
      const float offset_block = radius * 2.f / 3.f;

      const ImVec2ih poses[] = {{0, 0}, {1, 0}, {2, 0}, {0, 1}, {1, 1}, {2, 1}, {0, 2}, {1, 2}, {2, 2}};
      constexpr float rkoeff[9] = {0.1f, 0.15f, 0.17f, 0.25f, 0.6f, 0.15f, 0.1f, 0.12f, 0.22f};

      int ti = 0;
      for (const auto &rpos: poses)
      {
        float h = (0.8f + 0.4f * ImSin((float)ImGui::GetTime() * (speed * rkoeff[ti % 9])));
        window->DrawList->AddRectFilled(ImVec2(lt.x + rpos.x * (offset_block), lt.y + rpos.y * offset_block),
                                        ImVec2(lt.x + rpos.x * (offset_block) + h * thickness, lt.y + rpos.y * offset_block + h * thickness), color);
        ti++;
      }
    }

    void SpinnerScaleSquares(const char *label, float radius, float offset, const ImColor &color, float speed)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        ImVec2 lt{centre.x - radius, centre.y - radius};
        const float offset_block = radius * 2.f / 3.f;
        const float hside = offset_block / 2.f;

        const ImVec2ih poses[] = {{0, 0}, {1, 0}, {0, 1}, {2, 0}, {1, 1}, {0, 2}, {2, 1}, {1, 2}, {2, 2}};
        const float offsets[] =  {0.f,    0.8f,   0.8f,   1.6f,   1.6f,   1.6f,   2.4f,   2.4f,   3.2f};

        int ti = 0;
        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        for (const auto &rpos: poses)
        {
            const ImColor c = ImColor::HSV(out_h + offsets[ti], out_s, out_v);
            const float strict = (0.5f + 0.5f * ImSin((float)-ImGui::GetTime() * speed + offsets[ti % 9]));
            const float side = ImClamp<float>(strict + 0.1f, 0.1f, 1.f) * hside;
            window->DrawList->AddRectFilled(ImVec2(lt.x + hside + (rpos.x * offset_block) - side, lt.y + hside + (rpos.y * offset_block) - side),
                                            ImVec2(lt.x + hside + (rpos.x * offset_block) + side, lt.y + hside + (rpos.y * offset_block) + side), c);
            ti++;
        }
    }

    void SpinnerFluid(const char *label, float radius, const ImColor &color, float speed, int bars = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const ImGuiStyle &style = GImGui->Style;
      const float hspeed = 0.1f + ImSin((float)ImGui::GetTime() * 0.1f) * 0.05f;
      constexpr float rkoeff[6][3] = {{0.15f, 0.1f, 0.1f}, {0.033f, 0.15f, 0.8f}, {0.017f, 0.25f, 0.6f}, {0.037f, 0.1f, 0.4f}, {0.25f, 0.1f, 0.3f}, {0.11f, 0.1f, 0.2f}};
      const float i_k = radius / bars;
      const float j_k = radius * 2.f / num_segments;
      float out_h, out_s, out_v;
      ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
      for (int i = 0; i < bars; i++)
      {
        ImColor c = ImColor::HSV(out_h - i * 0.1f, out_s, out_v);
        c.Value.w = rkoeff[i % 6][1];
        for (int j = 0; j < num_segments; ++j) {
          float h = (0.6f + 0.3f * ImSin((float)ImGui::GetTime() * (speed * rkoeff[i % 6][2] * 2.f) + (2.f * rkoeff[i % 6][0] * j * j_k))) * (radius * 2.f * rkoeff[i][2]);
          window->DrawList->AddRectFilled(ImVec2(pos.x + style.FramePadding.x + j * j_k, centre.y + size.y / 2.f),
                                          ImVec2(pos.x + style.FramePadding.x + (j + 1) * (j_k), centre.y + size.y / 2.f - h), c);
        }
      }
    }

    void SpinnerArcPolarFade(const char *label, float radius, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t arcs = 4)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      float arc_angle = 2.f * IM_PI / (float)arcs;
      const float angle_offset = arc_angle / num_segments;
      constexpr float rkoeff[6][3] = {{0.15f, 0.1f, 0.1f}, {0.033f, 0.15f, 0.8f}, {0.017f, 0.25f, 0.6f}, {0.037f, 0.1f, 0.4f}, {0.25f, 0.1f, 0.3f}, {0.11f, 0.1f, 0.2f}};
      for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
      {
        const float b = arc_angle * arc_num - IM_PI / 2.f - IM_PI / 4.f;
        const float e = arc_angle * arc_num + arc_angle - IM_PI / 2.f - IM_PI / 4.f;
        const float a = arc_angle * arc_num;
        ImColor c = color;
        float h = (0.6f + 0.3f * ImSin((float)ImGui::GetTime() * (speed * rkoeff[arc_num % 6][2] * 2.f) + (2 * rkoeff[arc_num % 6][0])));
        c.Value.w = h;

        window->DrawList->PathClear();
        window->DrawList->PathLineTo(centre);
        for (size_t i = 0; i <= num_segments + 1; i++)
        {
          const float ar = arc_angle * arc_num + (i * angle_offset) - IM_PI / 2.f - IM_PI / 4.f;
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(ar) * radius, centre.y + ImSin(ar) * radius));
        }
        window->DrawList->PathFillConvex(c);
      }
    }

    void SpinnerArcPolarRadius(const char *label, float radius, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t arcs = 4)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      float arc_angle = 2.f * IM_PI / (float)arcs;
      const float angle_offset = arc_angle / num_segments;
      constexpr float rkoeff[6][3] = {{0.15f, 0.1f, 0.41f}, {0.033f, 0.15f, 0.8f}, {0.017f, 0.25f, 0.6f}, {0.037f, 0.1f, 0.4f}, {0.25f, 0.1f, 0.3f}, {0.11f, 0.1f, 0.2f}};
      float out_h, out_s, out_v;
      ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
      for (size_t arc_num = 0; arc_num < arcs; ++arc_num)
      {
        const float b = arc_angle * arc_num - IM_PI / 2.f - IM_PI / 4.f;
        const float e = arc_angle * arc_num + arc_angle - IM_PI / 2.f - IM_PI / 4.f;
        const float a = arc_angle * arc_num;
        float r = (0.6f + 0.3f * ImSin((float)ImGui::GetTime() * (speed * rkoeff[arc_num % 6][2] * 2.f) + (2.f * rkoeff[arc_num % 6][0])));

        window->DrawList->PathClear();
        window->DrawList->PathLineTo(centre);
        ImColor c = ImColor::HSV(out_h + arc_num * 0.31f, out_s, out_v);
        for (size_t i = 0; i <= num_segments + 1; i++)
        {
          const float ar = arc_angle * arc_num + (i * angle_offset) - IM_PI / 2.f - IM_PI / 4.f;
          window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(ar) * (radius * r), centre.y + ImSin(ar) * (radius * r)));
        }
        window->DrawList->PathFillConvex(c);
      }
    }

    void SpinnerCaleidoscope(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t arcs = 6, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI * 2 / arcs);
      start -= astart;
      const float angle_offset = IM_PI * 2 / arcs;
      const float angle_offset_t = angle_offset * 0.3f;
      arcs = ImMin<size_t>(arcs, 32);

      auto get_points = [&] (auto left, auto right, auto r) -> std::array<ImVec2, 4> {
        const float rmin = r - thickness;
        return {
          ImVec2(centre.x + ImCos(left) * rmin, centre.y + ImSin(left) * rmin),
          ImVec2(centre.x + ImCos(left) * r, centre.y + ImSin(left) * r),
          ImVec2(centre.x + ImCos(right) * r, centre.y + ImSin(right) * r),
          ImVec2(centre.x + ImCos(right) * rmin, centre.y + ImSin(right) * rmin)
        };
      };

      auto draw_sectors = [&] (auto s, auto color_func, auto r) {
        for (size_t i = 0; i <= arcs; i++) {
          float left = s + (i * angle_offset) - angle_offset_t;
          float right = s + (i * angle_offset) + angle_offset_t;
          auto points = get_points(left, right, r);
          window->DrawList->AddConvexPolyFilled(points.data(), 4, color_func(i));
        }
      };

      float out_h, out_s, out_v;
      ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
      draw_sectors(start, [&] (auto i) { return ImColor::HSV(out_h + i * 0.31f, out_s, out_v); }, radius);
      switch (mode) {
      case 0: draw_sectors(-start * 0.78f, [&] (auto i) { return ImColor::HSV(out_h + i * 0.31f, out_s, out_v); }, radius - thickness - 2); break;
      case 1:
        {
          ImColor c = color;
          float lerp_koeff = (ImSin((float)ImGui::GetTime() * speed) + 1.f) * 0.5f;
          c.Value.w = ImMax(0.1f, ImMin(lerp_koeff, 1.f));
          float dr = radius - thickness - 3;
          window->DrawList->AddCircleFilled(centre, dr, c, num_segments);
        } 
        break;
      }
    }

    // spinner idea by nitz 'Chris Dailey'
    void SpinnerHboDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float minfade = 0.0f, float ryk = 0.f, float speed = 1.1f, size_t dots = 6)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        const float ang_offset = (IM_PI * 2.f) / dots;

        float offset = 0;
        for (size_t i = 0; i < dots; i++)
        {
            const float astart = start + ang_offset * i;
            ImColor c = color;
            c.Value.w = ImMax(minfade, ImSin(astart * 1.0f + IM_PI * 0.5f));
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(astart) * radius, centre.y + ryk * ImCos(astart) * radius), thickness, c, 8);
        }
    }

    namespace detail {
      struct SpinnerDraw { SpinnerTypeT type; void (*func)(const char *, const detail::SpinnerConfig &); } spinner_draw_funcs[e_st_count] = {
        { e_st_rainbow, [] (const char *label, const detail::SpinnerConfig &c) { SpinnerRainbow(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed, c.m_AngleMin, c.m_AngleMax); } },
        { e_st_angle,   [] (const char *label, const detail::SpinnerConfig &c) { SpinnerAng(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_BgColor, c.m_Speed, c.m_Angle); } },
        { e_st_dots,    [] (const char *label, const detail::SpinnerConfig &c) { SpinnerDots(label, c.m_FloatPtr, c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed, c.m_Dots, c.m_MinThickness); } },
        { e_st_ang,     [] (const char *label, const detail::SpinnerConfig &c) { SpinnerAng(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_BgColor, c.m_Speed, c.m_Angle); } },
        { e_st_vdots,   [] (const char *label, const detail::SpinnerConfig &c) { SpinnerVDots(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_BgColor, c.m_Speed, c.m_Dots); } },
        { e_st_bounce_ball, [] (const char *label,const detail::SpinnerConfig &c) { SpinnerBounceBall(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed, c.m_Dots); } },
        { e_st_eclipse, [] (const char *label, const detail::SpinnerConfig &c) { SpinnerAngEclipse(label , c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed); } },
        { e_st_ingyang, [] (const char *label, const detail::SpinnerConfig &c) { SpinnerIngYang(label, c.m_Radius, c.m_Thickness, c.m_Reverse, c.m_Delta, c.m_AltColor, c.m_Color, c.m_Speed, c.m_Angle); } }
      };
    }

    void Spinner(const char *label, const detail::SpinnerConfig& config)
    {
      if (config.m_SpinnerType < sizeof(detail::spinner_draw_funcs))
        detail::spinner_draw_funcs[config.m_SpinnerType].func(label, config);
    }

    template<SpinnerTypeT Type, typename... Args>
    void Spinner(const char *label, const Args&... args)
    {
      detail::SpinnerConfig config(SpinnerType{Type}, args...);
      Spinner(label, config);
    }

#ifdef IMSPINNER_DEMO
    void demoSpinners() {

      static int hue = 0;
      static float nextdot = 0, nextdot2;
      
      nextdot -= 0.07f;

      static float velocity = 1.f;
      static float widget_size = 50.f;

      static ImVec2 selected{0, 0};
      constexpr int num_spinners = 100;
      int sidex = int(500 / widget_size);
      int sidey = int(num_spinners / sidex);

      static int cci = 0, last_cci = 0;
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

      ImGuiStyle &style = GImGui->Style;
      ImVec2 lastSpacing = style.ItemSpacing, lastPadding = style.WindowPadding;
      style.ItemSpacing = style.WindowPadding = {0.f, 0.f};
      for (int y = 0; y < sidey; y++) {
        for (int x = 0; x < sidex; x++) {
          if (x > 0)
            ImGui::SameLine();
          int current_spi = y * sidex + x;
          ImGui::BeginChild(100 + current_spi, ImVec2(widget_size, widget_size), false, ImGuiWindowFlags_NoScrollbar);
          ImVec2 curpos_begin = ImGui::GetCursorPos();
          ImGui::PushID(current_spi);
          if (ImGui::Selectable("", selected.x == x && selected.y == y, 0, ImVec2(widget_size, widget_size))) {
            selected = ImVec2((float)x, (float)y);
            last_cci = current_spi;
          }
          float sp_radius = __rr.count(current_spi) ? __rr[current_spi] : 16.f;
          float sp_offset = (widget_size - sp_radius * 2.f ) / 2.f;
          ImGui::SetCursorPos({curpos_begin.x + sp_offset, curpos_begin.y + sp_offset});

#define $(i) i: cci = i;
          switch (current_spi) {
          case $( 0) ImSpinner::Spinner<e_st_rainbow>   ("Spinner",
                                                          Radius{R(16)}, Thickness{T(2)}, Color{ImColor::HSV(++hue * 0.005f, 0.8f, 0.8f)}, Speed{S(8) * velocity}, AngleMin{AMN(0.f)}, AngleMax{AMX(IM_PI * 2.f)}); break;
          case $( 1) ImSpinner::Spinner<e_st_angle>     ("SpinnerAng",
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(ImColor(255, 255, 255))}, BgColor{CB(ImColor(255, 255, 255, 128))}, Speed{S(8) * velocity}, Angle{A(IM_PI)}); break;
          case $( 2) ImSpinner::Spinner<e_st_dots>      ("SpinnerDots",
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(ImColor(255, 255, 255))}, FloatPtr{&nextdot}, Speed{S(1) * velocity}, Dots{DT(12)}, MinThickness{-1.f}); break;
          case $( 3) ImSpinner::Spinner<e_st_ang>       ("SpinnerAngNoBg",
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(ImColor(255, 255, 255))}, BgColor{CB(ImColor(255, 255, 255, 0))}, Speed{S(6) * velocity}, Angle{A(IM_PI)}); break;
          case $( 4) ImSpinner::Spinner<e_st_ang>       ("SpinnerAng270",
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(ImColor(255, 255, 255))}, BgColor{CB(ImColor(255, 255, 255, 128))}, Speed{S(6) * velocity}, Angle{A(0.75f * 2 * IM_PI)}); break;
          case $( 5) ImSpinner::Spinner<e_st_ang>       ("SpinnerAng270NoBg",
                                                          Radius{R(16)}, Thickness{T(2)}, Color{C(ImColor(255, 255, 255))}, BgColor{CB(ImColor(255, 255, 255, 0))}, Speed{S(6) * velocity}, Angle{A(0.75f * 2 * IM_PI)}); break;
          case $( 6) ImSpinner::Spinner<e_st_vdots>     ("SpinnerVDots",
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(ImColor(255, 255, 255))}, BgColor{CB(ImColor::HSV(hue * 0.0011f, 0.8f, 0.8f))}, Speed{S(2.7f) * velocity}, Dots{DT(12)}, MiddleDots{6}); break;
          case $( 7) ImSpinner::Spinner<e_st_bounce_ball>("SpinnerBounceBall",
                                                          Radius{R(16)}, Thickness{T(6)}, Color{C(ImColor(255, 255, 255))}, Speed{S(4) * velocity}, Dots{DT(1)}); break;
          case $( 8) ImSpinner::Spinner<e_st_eclipse>   ("SpinnerAngEclipse",
                                                          Radius{R(16)}, Thickness{T(5)}, Color{C(ImColor(255, 255, 255))}, Speed{S(6) * velocity}); break;
          case $( 9) ImSpinner::Spinner<e_st_ingyang>   ("SpinnerIngYang",
                                                          Radius{R(16)}, Thickness{T(5)}, Reverse{false}, Delta{0.f}, Color{C(ImColor(255, 255, 255))}, AltColor{ImColor(255, 0, 0)}, Speed{S(4) * velocity}, Angle{A(IM_PI * 0.8f)}); break;
          case $(10) ImSpinner::SpinnerBarChartSine     ("SpinnerBarChartSine",
                                                          R(16), 4, C(ImColor(255, 255, 255)), S(6.8f) * velocity, 4, 0); break;
          case $(11) ImSpinner::SpinnerBounceDots       ("SpinnerBounceDots", R(16),
                                                          T(6), C(ImColor(255, 255, 255)), S(6) * velocity, DT(3)); break;
          case $(12) ImSpinner::SpinnerFadeDots         ("SpinnerFadeDots",
                                                          T(6), C(ImColor(255, 255, 255)), S(8) * velocity, 3); break;
          case $(13) ImSpinner::SpinnerScaleDots        ("SpinnerScaleDots",
                                                          T(6), C(ImColor(255, 255, 255)), S(7) * velocity, 3); break;
          case $(14) ImSpinner::SpinnerMovingDots       ("SpinnerMovingDots",
                                                          T(6), C(ImColor(255, 255, 255)), S(30) * velocity, 3); break;
          case $(15) ImSpinner::SpinnerRotateDots       ("SpinnerRotateDots",
                                                          R(16), T(6), C(ImColor(255, 255, 255)), S(4) * velocity, 2); break;
          case $(16) ImSpinner::SpinnerTwinAng          ("SpinnerTwinAng",
                                                          R(16), 16, T(6), C(ImColor(255, 255, 255)), CB(ImColor(255, 0, 0)), S(4) * velocity, A(IM_PI)); break;
          case $(17) ImSpinner::SpinnerClock            ("SpinnerClock",
                                                          R(16), T(2), C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(4) * velocity); break;
          case $(18) ImSpinner::SpinnerIngYang          ("SpinnerIngYangR",
                                                          R(16), T(5), true, 0.1f, C(ImColor(255, 255, 255)), CB(ImColor(255, 0, 0)), S(4) * velocity, A(IM_PI * 0.8f)); break;
          case $(19) ImSpinner::SpinnerBarChartSine     ("SpinnerBarChartSine2",
                                                          R(16), T(4), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(4.8f) * velocity, 4, 1); break;
          case $(20) ImSpinner::SpinnerTwinAng180       ("SpinnerTwinAng",
                                                          R(16), 12, T(4), C(ImColor(255, 255, 255)), CB(ImColor(255, 0, 0)), S(4) * velocity); break;
          case $(21) ImSpinner::SpinnerTwinAng360       ("SpinnerTwinAng360",
                                                          R(16), 11, T(4), C(ImColor(255, 255, 255)), CB(ImColor(255, 0, 0)), S(4) * velocity); break;
          case $(22) ImSpinner::SpinnerIncDots          ("SpinnerIncDots",
                                                          R(16), T(4), C(ImColor(255, 255, 255)), S(5.6f) * velocity, 6); break;
          case $(23) nextdot2 -= 0.2f * velocity;
                     ImSpinner::SpinnerDots             ("SpinnerDotsWoBg",
                                                          &nextdot2, R(16), T(4), C(ImColor(255, 255, 255)), S(0.3f) * velocity, 12, 0.f); break;
          case $(24) ImSpinner::SpinnerIncScaleDots     ("SpinnerIncScaleDots",
                                                          R(16), T(4), C(ImColor(255, 255, 255)), S(6.6f) * velocity, 6); break;
          case $(25) ImSpinner::SpinnerAng              ("SpinnerAng90",
                                                          R(16), T(6), C(ImColor(255, 255, 255)), CB(ImColor(255, 255, 255, 128)), S(8.f) * velocity, A(IM_PI / 2.f)); break;
          case $(26) ImSpinner::SpinnerAng              ("SpinnerAng90",
                                                          R(16), 6, C(ImColor(255, 255, 255)), CB(ImColor(255, 255, 255, 0)), S(8.5f) * velocity, A(IM_PI / 2.f)); break;
          case $(27) ImSpinner::SpinnerFadeBars         ("SpinnerFadeBars",
                                                          10, C(ImColor(255, 255, 255)), S(4.8f) * velocity, 3); break;
          case $(28) ImSpinner::SpinnerPulsar           ("SpinnerPulsar",
                                                          R(16), T(2), C(ImColor(255, 255, 255)), S(1) * velocity); break;
          case $(29) ImSpinner::SpinnerIngYang          ("SpinnerIngYangR2",
                                                          R(16), T(5), true, 3.f, C(ImColor(255, 255, 255)), CB(ImColor(255, 0, 0)), S(4) * velocity, A(IM_PI * 0.8f)); break;
          case $(30) ImSpinner::SpinnerBarChartRainbow  ("SpinnerBarChartRainbow",
                                                          R(16), T(4), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(6.8f) * velocity, 4); break;
          case $(31) ImSpinner::SpinnerBarsRotateFade   ("SpinnerBarsRotateFade",
                                                          8, 18, T(4), C(ImColor(255, 255, 255)), S(7.6f) * velocity, 6); break;
          case $(32) ImSpinner::SpinnerFadeBars         ("SpinnerFadeScaleBars",
                                                          10, C(ImColor(255, 255, 255)), S(6.8f) * velocity, 3, true); break;
          case $(33) ImSpinner::SpinnerBarsScaleMiddle  ("SpinnerBarsScaleMiddle",
                                                          6, C(ImColor(255, 255, 255)), S(8.8f) * velocity, 3); break;
          case $(34) ImSpinner::SpinnerAngTwin          ("SpinnerAngTwin1",
                                                          R(16), 13, T(2), C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(6) * velocity, A(IM_PI / 2.f)); break;
          case $(35) ImSpinner::SpinnerAngTwin          ("SpinnerAngTwin2",
                                                          13, 16, T(2), C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(6) * velocity, A(IM_PI / 2.f)); break;
          case $(36) ImSpinner::SpinnerAngTwin          ("SpinnerAngTwin3",
                                                          13, 16, T(2), C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(6) * velocity, A(IM_PI / 2.f), 2); break;
          case $(37) ImSpinner::SpinnerAngTwin          ("SpinnerAngTwin4",
                                                          R(16), 13, T(2), C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(6) * velocity, A(IM_PI / 2.f), 2); break;
          case $(38) ImSpinner::SpinnerTwinPulsar       ("SpinnerTwinPulsar",
                                                          R(16), T(2), C(ImColor(255, 255, 255)), S(0.5f) * velocity, 2); break;
          case $(39) ImSpinner::SpinnerAngTwin          ("SpinnerAngTwin4",
                                                          R(14), 13, T(3), C(ImColor(255, 0, 0)), CB(ImColor(0, 0, 0, 0)), S(5) * velocity, A(IM_PI / 1.5f), 2); break;
          case $(40) ImSpinner::SpinnerBlocks           ("SpinnerBlocks",
                                                          R(16), T(7), C(ImColor(255, 255, 255, 30)), CB(ImColor::HSV(hue * 0.005f, 0.8f, 0.8f)), S(5) * velocity); break;
          case $(41) ImSpinner::SpinnerTwinBall         ("SpinnerTwinBall",
                                                          R(16), 11, T(2), 2.5f, C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(6) * velocity, 2); break;
          case $(42) ImSpinner::SpinnerTwinBall         ("SpinnerTwinBall2",
                                                          R(15), 19, T(2), 2.f, C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(6) * velocity, 3); break;
          case $(43) ImSpinner::SpinnerTwinBall         ("SpinnerTwinBall2",
                                                          16, 16, T(2), 5.f, C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(5) * velocity, 1); break;
          case $(44) ImSpinner::SpinnerAngTriple        ("SpinnerAngTriple",
                                                          16, 13, 10, T(1.3f), C(ImColor(255, 255, 255)), ImColor(255, 0, 0), ImColor(255, 255, 255), S(5) * velocity, A(1.5f * IM_PI)); break;
          case $(45) ImSpinner::SpinnerIncFullDots      ("SpinnerIncFullDots",
                                                          R(16), T(4), C(ImColor(255, 255, 255)), S(5.6f) * velocity, 4); break; 
          case $(46) ImSpinner::SpinnerGooeyBalls       ("SpinnerGooeyBalls",
                                                          R(16), C(ImColor(255, 255, 255)), S(2.f) * velocity); break;
          case $(47) ImSpinner::SpinnerRotateGooeyBalls ("SpinnerRotateGooeyBalls2",
                                                          R(16), T(5), C(ImColor(255, 255, 255)), S(6.f) * velocity, 2); break;
          case $(48) ImSpinner::SpinnerRotateGooeyBalls ("SpinnerRotateGooeyBalls3",
                                                          R(16), T(5), C(ImColor(255, 255, 255)), S(6.f) * velocity, 3); break;
          case $(49) ImSpinner::SpinnerMoonLine         ("SpinnerMoonLine",
                                                          R(16), T(3), C(ImColor(200, 80, 0)), ImColor(80, 80, 80), S(5) * velocity); break;
          case $(50) ImSpinner::SpinnerArcRotation      ("SpinnerArcRotation",
                                                          R(13), T(5), C(ImColor(255, 255, 255)), S(3) * velocity, 4); break;
          case $(51) ImSpinner::SpinnerFluid            ("SpinnerFluid",
                                                          R(16), C(ImColor(0, 0, 255)), S(3.8f) * velocity, 4); break;
          case $(52) ImSpinner::SpinnerArcFade          ("SpinnerArcFade",
                                                          R(13), T(5), C(ImColor(255, 255, 255)), S(3) * velocity, 4); break;
          case $(53) ImSpinner::SpinnerFilling          ("SpinnerFilling",
                                                          R(16), T(6), C(ImColor(255, 255, 255)), CB(ImColor(255, 0, 0)), S(4) * velocity); break;
          case $(54) ImSpinner::SpinnerTopup            ("SpinnerTopup",
                                                          R(16), 12, C(ImColor(255, 0, 0)), ImColor(80, 80, 80), CB(ImColor(255, 255, 255)), S(1) * velocity);  break;
          case $(55) ImSpinner::SpinnerFadePulsar       ("SpinnerFadePulsar",
                                                          R(16), C(ImColor(255, 255, 255)), S(1.5f) * velocity, 1);  break;
          case $(56) ImSpinner::SpinnerFadePulsar       ("SpinnerFadePulsar2",
                                                          R(16), C(ImColor(255, 255, 255)), S(0.9f) * velocity, 2); break;
          case $(57) ImSpinner::SpinnerPulsar           ("SpinnerPulsar",
                                                          R(16), T(2), C(ImColor(255, 255, 255)), S(1) * velocity, false); break;
          case $(58) ImSpinner::SpinnerDoubleFadePulsar ("SpinnerDoubleFadePulsar",
                                                          R(16), T(2), C(ImColor(255, 255, 255)), S(2) * velocity); break;
          case $(59) ImSpinner::SpinnerFilledArcFade    ("SpinnerFilledArcFade",
                                                          R(16), C(ImColor(255, 255, 255)), S(4) * velocity, 4); break;
          case $(60) ImSpinner::SpinnerFilledArcFade    ("SpinnerFilledArcFade6",
                                                          R(16), C(ImColor(255, 255, 255)), S(6) * velocity, 6); break;
          case $(61) ImSpinner::SpinnerFilledArcFade    ("SpinnerFilledArcFade6",
                                                          R(16), C(ImColor(255, 255, 255)), S(8) * velocity, 12); break;
          case $(62) ImSpinner::SpinnerFilledArcColor   ("SpinnerFilledArcColor",
                                                          R(16), C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(2.8f) * velocity, 4); break;
          case $(63) ImSpinner::SpinnerCircleDrop       ("SpinnerCircleDrop",
                                                          R(16), T(1.5f), 4.f, C(ImColor(255, 0, 0)), CB(ImColor(255, 255, 255)), S(2.8f) * velocity, A(IM_PI)); break;
          case $(64) ImSpinner::SpinnerSurroundedIndicator("SpinnerSurroundedIndicator",
                                                          R(16), T(5), C(ImColor(0, 0, 0)), CB(ImColor(255, 255, 255)), S(7.8f) * velocity); break;
          case $(65) ImSpinner::SpinnerTrianglesSeletor ("SpinnerTrianglesSeletor",
                                                          R(16), T(8), C(ImColor(0, 0, 0)), CB(ImColor(255, 255, 255)), S(4.8f) * velocity, 8); break;
          case $(66) ImSpinner::SpinnerFlowingGradient  ("SpinnerFlowingFradient",
                                                          R(16), T(6), C(ImColor(200, 80, 0)), CB(ImColor(80, 80, 80)), S(5) * velocity, A(IM_PI * 2.f)); break;
          case $(67) ImSpinner::SpinnerRotateSegments   ("SpinnerRotateSegments",
                                                          R(16), T(4), C(ImColor(255, 255, 255)), S(3) * velocity, 4); break;
          case $(68) ImSpinner::SpinnerRotateSegments   ("SpinnerRotateSegments2",
                                                          R(16), T(3), C(ImColor(255, 255, 255)), S(2.4f) * velocity, 4, 2); break;
          case $(69) ImSpinner::SpinnerRotateSegments   ("SpinnerRotateSegments3",
                                                          R(16), T(2), C(ImColor(255, 255, 255)), S(2.1f) * velocity, 4, 3); break;
          case $(70) ImSpinner::SpinnerLemniscate       ("SpinnerLemniscate",
                                                          R(20), T(3), C(ImColor(255, 255, 255)), S(2.1f) * velocity, 3); break;
          case $(71) ImSpinner::SpinnerRotateGear       ("SpinnerRotateGear",
                                                          R(16), T(6), C(ImColor(255, 255, 255)), S(2.1f) * velocity, 8); break;
          case $(72) ImSpinner::SpinnerRotatedAtom      ("SpinnerRotatedAtom",
                                                          R(16), T(2), C(ImColor(255, 255, 255)), S(2.1f) * velocity, 3); break;
          case $(73) ImSpinner::SpinnerAtom             ("SpinnerAtom",
                                                          R(16), T(2), C(ImColor(255, 255, 255)), S(4.1f) * velocity, 3); break;
          case $(74) ImSpinner::SpinnerRainbowBalls     ("SpinnerRainbowBalls",
                                                          R(16), T(4), ImColor::HSV(0.25f, 0.8f, 0.8f, 0.f), S(1.5f) * velocity, 5); break;
          case $(75) ImSpinner::SpinnerCamera           ("SpinnerCamera",
                                                          R(16), T(8), [] (int i) { return ImColor::HSV(i * 0.25f, 0.8f, 0.8f); }, S(4.8f) * velocity, 8); break;
          case $(76) ImSpinner::SpinnerArcPolarFade     ("SpinnerArcPolarFade",
                                                          R(16), C(ImColor(255, 255, 255)), S(6) * velocity, 6); break;
          case $(77) ImSpinner::SpinnerArcPolarRadius   ("SpinnerArcPolarRadius",
                                                          R(16), C(ImColor::HSV(0.25f, 0.8f, 0.8f)), S(6.f) * velocity, 6); break;
          case $(78) ImSpinner::SpinnerCaleidoscope     ("SpinnerArcPolarPies",
                                                          R(16), T(4), C(ImColor::HSV(0.25f, 0.8f, 0.8f)), S(2.6f) * velocity, 10, 0); break;
          case $(79) ImSpinner::SpinnerCaleidoscope     ("SpinnerArcPolarPies2",
                                                          R(16), T(4), C(ImColor::HSV(0.35f, 0.8f, 0.8f)), S(3.2f) * velocity, 10, 1); break;
          case $(80) ImSpinner::SpinnerScaleBlocks      ("SpinnerScaleBlocks",
                                                          R(16), T(8), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(5) * velocity); break;
          case $(81) ImSpinner::SpinnerRotateTriangles  ("SpinnerRotateTriangles",
                                                          R(16), T(5), C(ImColor(255, 255, 255)), S(6.f) * velocity, 3); break;
          case $(82) ImSpinner::SpinnerArcWedges        ("SpinnerArcWedges",
                                                          R(16), C(ImColor::HSV(0.3f, 0.8f, 0.8f)), S(2.8f) * velocity, 4); break;
          case $(83) ImSpinner::SpinnerScaleSquares     ("SpinnerScaleSquares",
                                                          R(16), T(8), ImColor::HSV(hue * 0.005f, 0.8f, 0.8f), S(5) * velocity); break;
          case $(84) ImSpinner::SpinnerHboDots          ("SpinnerMovingDots", R(16),
                                                          T(4), C(ImColor(255, 255, 255)), 0.f, 0.f, S(1.1f) * velocity, 6); break;
          case $(85) ImSpinner::SpinnerHboDots          ("SpinnerMovingDots", R(16),
                                                          T(4), C(ImColor(255, 255, 255)), 0.1f, 0.5f, S(1.1f) * velocity, 6); break;
          case $(86) ImSpinner::Spinner<e_st_bounce_ball>("SpinnerBounceBall3",
                                                          Radius{R(16)}, Thickness{T(4)}, Color{C(ImColor(255, 255, 255))}, Speed{S(3.2f) * velocity}, Dots{DT(5)}); break;
          case $(87) ImSpinner::SpinnerBounceBall       ("SpinnerBounceBallShadow",
                                                          R(16), T(4), C(ImColor(255, 255, 255)), S(2.2f) * velocity, DT(1), true); break;
          case $(88) ImSpinner::SpinnerBounceBall       ("SpinnerBounceBall5Shadow",
                                                          R(16), T(4), C(ImColor(255, 255, 255)), S(3.6f) * velocity, DT(5), true); break;
          case $(89) ImSpinner::SpinnerSquareStrokeFade ("SpinnerSquareStrokeFade",
                                                          R(13), T(5), C(ImColor(255, 255, 255)), S(3) * velocity); break;
          case $(90) ImSpinner::SpinnerSquareStrokeFill ("SpinnerSquareStrokeFill",
                                                           R(13), T(5), C(ImColor(255, 255, 255)), S(3) * velocity); break;
          }
          ImGui::PopID();
          ImGui::EndChild();
          if (x == sidex - 1) {
            ImGui::Dummy({0, 0});
          }
        }
      }
      style.ItemSpacing = lastSpacing;
      style.WindowPadding = lastPadding;
    
      ImGui::SetCursorPos({520, 20});
      ImGui::BeginChild("Options");
      ImGui::SliderFloat("Velocity", &velocity, 0.0f, 10.0f, "velocity = %.2f");
      ImGui::SliderFloat("Grid size", &widget_size, 0.0f, 100.0f, "size = %.2f");
      if (__rr.count(last_cci)) ImGui::SliderFloat("Radius", &__rr[last_cci], 0.0f, 100.0f, "radius = %.2f");
      if (__tt.count(last_cci)) ImGui::SliderFloat("Thickness", &__tt[last_cci], 0.0f, 100.0f, "thickness = %.2f");
      if (__cc.count(last_cci)) {
        ImGui::Checkbox("Change Color", &__hc[last_cci]);
        if (__hc[last_cci]) { __cc[last_cci] = ImColor::HSV(hue * 0.005f, 0.8f, 0.8f); }
        else {
          ImGui::SameLine(); ImGui::SetNextItemWidth(120);
          ImGui::ColorPicker3("##MyColor", (float *)&__cc[last_cci], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
        }
      }
      if (__cb.count(last_cci)) {
          ImGui::Checkbox("Change Bg Color", &__hcb[last_cci]);
          if (__hcb[last_cci]) { __cb[last_cci] = ImColor::HSV(hue * 0.008f, 0.8f, 0.8f); }
          else {
              ImGui::SameLine(); ImGui::SetNextItemWidth(120);
              ImGui::ColorPicker3("##MyBgColor", (float *)&__cb[last_cci], ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
          }
      }
      if (__ss.count(last_cci)) ImGui::SliderFloat("Speed", &__ss[last_cci], 0.0f, 100.0f, "speed = %.2f");
      if (__aa.count(last_cci)) ImGui::SliderFloat("Angle", &__aa[last_cci], 0.0f, 2 * IM_PI, "angle = %.2f");
      if (__amn.count(last_cci)) ImGui::SliderFloat("Angle Min", &__amn[last_cci], 0.0f, 2 * IM_PI, "angle min = %.2f");
      if (__amx.count(last_cci)) ImGui::SliderFloat("Angle Max", &__amx[last_cci], 0.0f, 2 * IM_PI, "angle max = %.2f");
      if (__dt.count(last_cci)) ImGui::SliderInt("Dots", &__dt[last_cci], 1, 100, "dots = %u");
      if (__mdt.count(last_cci)) ImGui::SliderInt("MidDots", &__mdt[last_cci], 1, 100, "mid dots = %u");


      ImGui::EndChild();
    }
#endif // IMSPINNER_DEMO
}

#endif // _IMSPINNER_H_