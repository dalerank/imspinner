#ifndef _IMSPINNER_H_
#define _IMSPINNER_H_

#include <functional>

namespace ImSpinner
{
    namespace detail {
      bool SpinnerBegin(const char *label, float radius, ImVec2 &pos, ImVec2 &size, ImVec2 &centre) {
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

        centre = bb.GetCenter();
        if (!ImGui::ItemAdd(bb, id))
          return false;

        return true;
      }
    }

#define SPINNER_HEADER(pos, size, centre) ImVec2 pos, size, centre; if (!detail::SpinnerBegin(label, radius, pos, size, centre)) { return; }; ImGuiWindow *window = ImGui::GetCurrentWindow();

    void Spinner(const char *label, float radius, float thickness, const ImColor &color, float speed)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        window->DrawList->PathClear();
        const size_t num_segments = window->DrawList->_CalcCircleAutoSegmentCount(radius);
        float start = ImAbs(ImSin((float)ImGui::GetTime() * 1.8f) * (num_segments - 5));

        const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
        const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

        for (size_t i = 0; i < num_segments; i++)
        {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + (float)ImGui::GetTime() * speed) * radius,
                                         centre.y + ImSin(a + (float)ImGui::GetTime() * speed) * radius));
        }
        window->DrawList->PathStroke(color, false, thickness);
    }

    void SpinnerAng(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, float angle = IM_PI)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        window->DrawList->PathClear();
        const size_t num_segments = window->DrawList->_CalcCircleAutoSegmentCount(radius);
        float start = (float)ImGui::GetTime()* speed;
        const float bg_angle_offset = IM_PI * 2.f / num_segments;
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
    /*auto is_between = [] (float start, float end, float mid){
    end = (end - start) < 0.0f ? (end - start + IM_PI) : end - start;
    mid = (mid - start) < 0.0f ? (mid - start + IM_PI) : mid - start;
    return (mid < end);
    };*/

    void SpinnerDots(const char *label, float &nextdot, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 12, size_t mdots = 6, float minth = -1.f)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        float start = (float)ImGui::GetTime() * speed;
        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = min(dots, 32);

        if (nextdot < 0.f)
            nextdot = (float)dots;

        auto thcorrect = [&thickness, &nextdot, &mdots, &minth] (int i) {
            const float nth = minth < 0.f ? thickness / 2.f : minth;
            return fmaxf(nth, ImSin(((i - nextdot) / mdots) * IM_PI) * thickness);
        };

        for (size_t i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = ImFmod(a, 2 * IM_PI);
            float th = minth < 0 ? thickness / 2.f : minth;

            if (nextdot + mdots < dots) {
                if (i > nextdot && i < nextdot + mdots)
                    th = thcorrect(i);
            } else {
                if ((i > nextdot && i < dots) || (i < ((int)(nextdot + mdots)) % dots))
                    th = thcorrect(i);
            }

            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(-a) * radius, centre.y + ImSin(-a) * radius), th, color, 8);
        }
    }

    void SpinnerVDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 12, size_t mdots = 6)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        float start = (float)ImGui::GetTime() * speed;
        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = min(dots, 32);

        for (size_t i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = ImFmod(a, 2 * IM_PI);
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(-a) * radius, centre.y + ImSin(-a) * radius), thickness / 2, color, 8);
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

    void SpinnerBounceDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
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

      // Render
      float start = (float)g.Time * speed;

      const float offset = IM_PI / dots;
      for (size_t i = 0; i < dots; i++)
      {
        float a = start + (IM_PI - i * offset);
        float sina = ImSin(a * heightSpeed);
        float y = centre.y + sina * thickness * heightKoeff;
        if (y > centre.y)
          y = centre.y;
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x  + i * (thickness * nextItemKoeff), y), thickness, color, 8);
      }
    }

    void SpinnerFadeDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
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

      // Render
      float start = (float)g.Time * speed;

      const float offset = IM_PI / dots;
      for (size_t i = 0; i < dots; i++)
      {
        float a = start + (IM_PI - i * offset);
        ImColor c = color;
        c.Value.w = std::max<float>(0.1f, ImSin(a * heightSpeed));
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x  + i * (thickness * nextItemKoeff), centre.y), thickness, c, 8);
      }
    }

    void SpinnerScaleDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
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

      // Render
      float start = (float)g.Time * speed;

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

      // Render
      float start = ImFmod((float)g.Time * speed, size.x);

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
        
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x + offset, centre.y), th, color, 8);
      }
    }

    void SpinnerRotateDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int dots = 2)
    {
      SPINNER_HEADER(pos, size, centre);

      // Render
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

    void SpinnerTwinAng(const char *label, float radius1, float radius2, float thickness, const ImColor &color1 = 0xffffffff, const ImColor &color2 = 0xff0000ff, float speed = 2.8f)
    {
      const float radius = std::max<float>(radius1, radius2);
      SPINNER_HEADER(pos, size, centre);

      // Render
      window->DrawList->PathClear();
      const size_t num_segments = window->DrawList->_CalcCircleAutoSegmentCount(radius) * 2;
      const float start = ImFmod((float)ImGui::GetTime() * speed, IM_PI * 2.f);
      const float aoffset = ImFmod((float)ImGui::GetTime(), 1.5f * IM_PI);
      const float bofsset = (aoffset > IM_PI) ? IM_PI : aoffset;

      const float angle_offset = IM_PI * 2.f / num_segments;
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

    void SpinnerTwinAng180(const char *label, float radius1, float radius2, float thickness, const ImColor &color1 = 0xffffffff, const ImColor &color2 = 0xff0000ff, float speed = 2.8f)
    {
      const float radius = std::max<float>(radius1, radius2);
      SPINNER_HEADER(pos, size, centre);

      // Render
      window->DrawList->PathClear();
      const size_t num_segments = window->DrawList->_CalcCircleAutoSegmentCount(radius) * 4;
      const float start = ImFmod((float)ImGui::GetTime()* speed, IM_PI * 2.f);
      const float aoffset = ImFmod((float)ImGui::GetTime(), 2.f * IM_PI);
      const float bofsset = (aoffset > IM_PI) ? IM_PI : aoffset;

      const float angle_offset = IM_PI * 2.f / num_segments;
      window->DrawList->PathClear();
      float ared_min = 0, ared = 0;
      if (aoffset > IM_PI)
        ared_min = aoffset - IM_PI;

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
      const float radius = std::max<float>(radius1, radius2);
      SPINNER_HEADER(pos, size, centre);

      // Render
      const size_t num_segments = window->DrawList->_CalcCircleAutoSegmentCount(radius) * 2;
      const float start1 = ImFmod((float)ImGui::GetTime() * speed1, IM_PI * 2.f);
      const float start2 = ImFmod((float)ImGui::GetTime() * speed2, IM_PI * 2.f);
      const float aoffset = ImFmod((float)ImGui::GetTime(), 2.f * IM_PI);
      const float bofsset = (aoffset > IM_PI) ? IM_PI : aoffset;

      const float angle_offset = IM_PI * 2.f / num_segments;
      window->DrawList->PathClear();
      float ared_min = 0, ared = 0;
      if (aoffset > IM_PI)
        ared_min = aoffset - IM_PI;

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
      SPINNER_HEADER(pos, size, centre);

      // Render
      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI / dots);
      start -= astart;
      const float bg_angle_offset = IM_PI / dots;
      dots = std::min<size_t>(dots, 32);

      for (size_t i = 0; i <= dots; i++)
      {
        float a = start + (i * bg_angle_offset);
        ImColor c = color;
        c.Value.w = std::max<float>(0.1f, i / (float)dots);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, c, 8);
      }
    }

#ifdef IMSPINNER_DEMO
    void SpinnerIncScaleDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 6)
    {
      SPINNER_HEADER(pos, size, centre);

      // Render
      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, IM_PI / dots);
      start -= astart;
      const float bg_angle_offset = IM_PI / dots;
      dots = min(dots, 32);

      for (size_t i = 0; i <= dots; i++)
      {
        float a = start + (i * bg_angle_offset);
        float th = thickness * max(0.1f, i / (float)dots);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), th, color, 8);
      }
    }

    void demoSpinners() {
      static int hue = 0;
      static float nextdot = 0, nextdot2;
      
      nextdot -= 0.07f;

      static float velocity = 1.f;
      ImGui::SliderFloat("Speed", &velocity, 0.0f, 10.0f, "velocity = %.3f");

      ImSpinner::Spinner("Spinner", 16, 2, ImColor::HSV(++hue * 0.005f, 0.8f, 0.8f), 8 * velocity);

      ImGui::SameLine();
      ImSpinner::SpinnerAng("SpinnerAng", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 128), 6 * velocity);
      
      ImGui::SameLine();
      ImSpinner::SpinnerDots("SpinnerDots", nextdot, 16, 4, ImColor(255, 255, 255), 1 * velocity);

      ImGui::SameLine();
      ImSpinner::SpinnerAng("SpinnerAngNoBg", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 0), 6 * velocity);

      ImGui::SameLine();
      ImSpinner::SpinnerAng("SpinnerAng270", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 128), 6 * velocity, 270.f / 360.f * 2 * IM_PI);

      ImGui::SameLine();
      ImSpinner::SpinnerAng("SpinnerAng270NoBg", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 0), 6 * velocity, 270.f / 360.f * 2 * IM_PI );

      ImGui::SameLine();
      ImSpinner::SpinnerVDots("SpinnerVDots", 16, 4, ImColor(255, 255, 255), 2.7f * velocity);

      // Next Line
      ImSpinner::SpinnerBounceDots("SpinnerBounceDots", 6, ImColor(255, 255, 255), 6 * velocity, 3);

      ImGui::SameLine();
      ImSpinner::SpinnerFadeDots("SpinnerFadeDots", 6, ImColor(255, 255, 255), 8 * velocity, 3);

      ImGui::SameLine();
      ImSpinner::SpinnerScaleDots("SpinnerMovingDots", 6, ImColor(255, 255, 255), 7 * velocity, 3);

      ImGui::SameLine();
      ImSpinner::SpinnerMovingDots("SpinnerMovingDots", 6, ImColor(255, 255, 255), 30 * velocity, 3);

      ImGui::SameLine(); ImGui::Dummy({10, 0}); ImGui::SameLine();
      ImSpinner::SpinnerRotateDots("SpinnerRotateDots", 16, 6, ImColor(255, 255, 255), 4 * velocity, 2);

      ImGui::SameLine(); ImGui::Dummy({10, 0}); ImGui::SameLine();
      ImSpinner::SpinnerTwinAng("SpinnerTwinAng", 16, 16, 6, ImColor(255, 255, 255), ImColor(255, 0, 0), 4 * velocity);

      // next line
      ImSpinner::SpinnerTwinAng180("SpinnerTwinAng", 16, 12, 4, ImColor(255, 255, 255), ImColor(255, 0, 0), 4 * velocity);

      ImGui::SameLine();
      ImSpinner::SpinnerTwinAng360("SpinnerTwinAng360", 16, 11, 4, ImColor(255, 255, 255), ImColor(255, 0, 0), 4 * velocity);

      ImGui::SameLine();
      ImSpinner::SpinnerIncDots("SpinnerIncDots", 16, 4, ImColor(255, 255, 255), 5.6f, 6);

      ImGui::SameLine();
      nextdot2 -= 0.2f * velocity;
      ImSpinner::SpinnerDots("SpinnerDots", nextdot2, 16, 4, ImColor(255, 255, 255), 0.3f, 12, 6, 0.f);

      ImGui::SameLine();
      ImSpinner::SpinnerIncScaleDots("SpinnerIncScaleDots", 16, 4, ImColor(255, 255, 255), 6.6f, 6);

      ImGui::SameLine();
      ImSpinner::SpinnerAng("SpinnerAng90", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 128), 8.f * velocity, IM_PI / 2.f);

      ImGui::SameLine();
      ImSpinner::SpinnerAng("SpinnerAng90", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 0), 8.5f * velocity, IM_PI / 2.f);
    }
#endif // IMSPINNER_DEMO
}

#endif // _IMSPINNER_H_