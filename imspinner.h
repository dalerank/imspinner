#ifndef _IMSPINNER_H_
#define _IMSPINNER_H_

#include <array>
namespace ImGui
{
    namespace detail {
      bool SpinnerBegin(const char *label, float radius, ImVec2 &pos, ImVec2 &size, ImVec2 &centre) {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
          return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        pos = window->DC.CursorPos;
        size = ImVec2((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);

        centre = bb.GetCenter();
        if (!ItemAdd(bb, id))
          return false;

        return true;
      }
    }

#define SPINNER_HEADER(pos, size, centre) ImVec2 pos, size, centre; \
    if (!detail::SpinnerBegin(label, radius, pos, size, centre)) { return false; } \
    ImGuiContext &g = *GImGui; \
    ImGuiWindow *window = GetCurrentWindow();

    bool Spinner(const char *label, float radius, float thickness, const ImColor &color)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        window->DrawList->PathClear();

        int num_segments = 30;
        float start = fabsf(ImSin((float)g.Time * 1.8f) * (num_segments - 5));

        const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
        const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

        for (int i = 0; i < num_segments; i++)
        {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + (float)g.Time * 8) * radius,
                                         centre.y + ImSin(a + (float)g.Time * 8) * radius));
        }

        window->DrawList->PathStroke(color, false, thickness);
        return true;
    }

    bool SpinnerAng(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, float angle = IM_PI)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        window->DrawList->PathClear();
        int num_segments = 30;
        float start = (float)g.Time * speed;
        const float bg_angle_offset = IM_PI * 2.f / num_segments;
        for (int i = 0; i <= num_segments; i++)
        {
            const float a = start + (i * bg_angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        window->DrawList->PathStroke(bg, false, thickness);

        window->DrawList->PathClear();
        const float angle_offset = angle / num_segments;
        for (int i = 0; i < num_segments; i++)
        {
            const float a = start + (i * angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        window->DrawList->PathStroke(color, false, thickness);
        return true;
    }
    /*auto is_between = [] (float start, float end, float mid){
    end = (end - start) < 0.0f ? (end - start + IM_PI) : end - start;
    mid = (mid - start) < 0.0f ? (mid - start + IM_PI) : mid - start;
    return (mid < end);
    };*/

    bool SpinnerDots(const char *label, float &nextdot, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 12, size_t mdots = 6)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        float start = (float)g.Time * speed;
        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = std::min<size_t>(dots, 32);

        if (nextdot < 0.f)
            nextdot = (float)dots;

        auto thcorrect = [&thickness, &nextdot, &mdots] (int i) {
            return  std::max<float>(thickness / 2.f, ImSin(((i - nextdot) / mdots) * IM_PI) * thickness);
        };

        for (size_t i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = fmodf(a, 2 * IM_PI);
            float th = thickness / 2.f;

            if (nextdot + mdots < dots) {
                if (i > nextdot && i < nextdot + mdots)
                    th = thcorrect(i);
            } else {
                if ((i > nextdot && i < dots) || (i < ((int)(nextdot + mdots)) % dots))
                    th = thcorrect(i);
            }

            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(-a) * radius, centre.y + ImSin(-a) * radius), th, color, 8);
        }

        return true;
    }

    bool SpinnerVDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 12, size_t mdots = 6)
    {
        SPINNER_HEADER(pos, size, centre);

        // Render
        float start = (float)g.Time * speed;
        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = std::min<size_t>(dots, 32);

        for (size_t i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = fmodf(a, 2 * IM_PI);
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

        return true;
    }

    bool SpinnerBounceDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = GetCurrentWindow();
      if (window->SkipItems)
        return false;

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ItemSize(bb, style.FramePadding.y);
      if (!ItemAdd(bb, id))
        return false;

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
      
      return true;
    }

    bool SpinnerFadeDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = GetCurrentWindow();
      if (window->SkipItems)
        return false;

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ItemSize(bb, style.FramePadding.y);
      if (!ItemAdd(bb, id))
        return false;

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

      return true;
    }

    bool SpinnerScaleDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = GetCurrentWindow();
      if (window->SkipItems)
        return false;

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ItemSize(bb, style.FramePadding.y);
      if (!ItemAdd(bb, id))
        return false;

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

      return true;
    }

    bool SpinnerMovingDots(const char *label, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = GetCurrentWindow();
      if (window->SkipItems)
        return false;

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ItemSize(bb, style.FramePadding.y);
      if (!ItemAdd(bb, id))
        return false;

      const ImVec2 centre = bb.GetCenter();

      // Render
      float start = fmodf((float)g.Time * speed, size.x);

      float offset = 0;
      for (size_t i = 0; i < dots; i++)
      {
        const float a = start + (i * IM_PI / dots);
        float th = thickness;
        offset =  fmodf(start + i * (size.x / dots), size.x);
        if (offset < thickness)
        {
          th = offset;
        }
        if (offset > size.x - thickness)
          th = size.x - offset;
        
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x + offset, centre.y), th, color, 8);
      }

      return true;
    }

    bool SpinnerRotateDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int dots = 2)
    {
      SPINNER_HEADER(pos, size, centre);
      (void)g;

      // Render
      static float velocity = 0;
      static float gtime = 0;
      float dtime = fmodf((float)gtime, IM_PI);
      float start = (gtime += velocity);
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

      window->DrawList->AddCircleFilled(centre, thickness, color, 8);

      const float angle_offset = (2 * IM_PI) / dots;
      for (int i = 0; i < dots; i++)
      {
        const float a = start + (i * angle_offset);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, color, 8);
      }

      return true;
    }

    bool SpinnerTwinAng(const char *label, float radius1, float radius2, float thickness, const ImColor &color1 = 0xffffffff, const ImColor &color2 = 0xff0000ff, float speed = 2.8f)
    {
      const float radius = std::max<float>(radius1, radius2);
      SPINNER_HEADER(pos, size, centre);

      // Render
      window->DrawList->PathClear();
      const int num_segments = 30;
      const float start = fmodf((float)g.Time * speed, IM_PI * 2.f);
      const float aoffset = fmodf((float)g.Time, 1.5f * IM_PI);
      const float bofsset = (aoffset > IM_PI) ? IM_PI : aoffset;

      const float angle_offset = IM_PI * 2.f / num_segments;
      for (int i = 0; i <= 2 * num_segments; i++)
      {
        const float a = start + (i * angle_offset);
        if (i * angle_offset > 2 * bofsset)
          break;
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius1, centre.y + ImSin(a) * radius1));
      }
      window->DrawList->PathStroke(color1, false, thickness);

      window->DrawList->PathClear();
      for (int i = 0; i < num_segments / 2; i++)
      {
        const float a = start + (i * angle_offset);
        if (i * angle_offset > bofsset)
          break;
        window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius2, centre.y + ImSin(a) * radius2));
      }
      window->DrawList->PathStroke(color2, false, thickness);
      return true;
    }

    void demoSpinners() {
      static int hue = 0;
      static float nextdot = 0;
      
      nextdot -= 0.07f;

      ImGui::Spinner("Spinner", 16, 2, ImColor::HSV(++hue * 0.005f, 0.8f, 0.8f));
      ImGui::SameLine();

      ImGui::SpinnerAng("SpinnerAng", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 128), 6);
      ImGui::SameLine();
      
      ImGui::SpinnerDots("SpinnerDots", nextdot, 16, 4, ImColor(255, 255, 255), 1);
      ImGui::SameLine();

      ImGui::SpinnerAng("SpinnerAngNoBg", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 0), 6);
      ImGui::SameLine();

      ImGui::SpinnerAng("SpinnerAng270", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 128), 6, 270.f / 360.f * 2 * IM_PI );
      ImGui::SameLine();

      ImGui::SpinnerAng("SpinnerAng270NoBg", 16, 6, ImColor(255, 255, 255), ImColor(255, 255, 255, 0), 6, 270.f / 360.f * 2 * IM_PI );
      ImGui::SameLine();

      ImGui::SpinnerVDots("SpinnerVDots", 16, 4, ImColor(255, 255, 255), 1);

      // Next Line
      ImGui::SpinnerBounceDots("SpinnerBounceDots", 6, ImColor(255, 255, 255), 6, 3);
      ImGui::SameLine();

      ImGui::SpinnerFadeDots("SpinnerFadeDots", 6, ImColor(255, 255, 255), 8, 3);
      ImGui::SameLine();

      ImGui::SpinnerScaleDots("SpinnerMovingDots", 6, ImColor(255, 255, 255), 7, 3);
      ImGui::SameLine();

      ImGui::SpinnerMovingDots("SpinnerMovingDots", 6, ImColor(255, 255, 255), 30, 3);
      ImGui::SameLine();

      ImGui::SameLine(); ImGui::Dummy({10, 0}); ImGui::SameLine();
      ImGui::SpinnerRotateDots("SpinnerRotateDots", 16, 6, ImColor(255, 255, 255), 4, 2);

      ImGui::SameLine(); ImGui::Dummy({10, 0}); ImGui::SameLine();
      ImGui::SpinnerTwinAng("SpinnerTwinAng", 16, 16, 6, ImColor(255, 255, 255), ImColor(255, 0, 0), 4);
    }
}

#endif // _IMSPINNER_H_