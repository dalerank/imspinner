#ifndef _IMSPINNER_H_
#define _IMSPINNER_H_

#include <array>
namespace ImGui
{
    bool Spinner(const char *label, float radius, int thickness, const ImColor &color)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        // Render
        window->DrawList->PathClear();

        int num_segments = 30;
        int start = fabs(ImSin(g.Time * 1.8f) * (num_segments - 5));

        const float a_min = IM_PI * 2.0f * ((float)start) / (float)num_segments;
        const float a_max = IM_PI * 2.0f * ((float)num_segments - 3) / (float)num_segments;

        const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

        for (int i = 0; i < num_segments; i++)
        {
            const float a = a_min + ((float)i / (float)num_segments) * (a_max - a_min);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a + g.Time * 8) * radius,
                                         centre.y + ImSin(a + g.Time * 8) * radius));
        }

        window->DrawList->PathStroke(color, false, thickness);
        return true;
    }

    bool SpinnerAng(const char *label, float radius, int thickness, const ImColor &color = 0xffffffff, const ImColor &bg = 0xffffff80, float speed = 2.8f, float angle = IM_PI)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        // Render
        window->DrawList->PathClear();

        int num_segments = 30;
        float start = g.Time * speed;

        const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

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

    bool SpinnerDots(const char *label, float &nextdot, float radius, int thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 12, size_t mdots = 6)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        // Render
        float start = g.Time * speed;
        const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = std::min<size_t>(dots, 32);

        if (nextdot < 0.f)
            nextdot = dots;

        auto thcorrect = [&thickness, &nextdot, &mdots] (int i) {
            return  std::max<float>(thickness / 2.f, ImSin(((i - nextdot) / mdots) * IM_PI) * thickness);
        };

        for (int i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = fmod(a, 2 * IM_PI);
            float th = thickness / 2;

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

    bool SpinnerVDots(const char *label, float radius, int thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 12, size_t mdots = 6)
    {
        ImGuiWindow *window = GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const ImGuiID id = window->GetID(label);

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size((radius) * 2, (radius + style.FramePadding.y) * 2);

        const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ItemSize(bb, style.FramePadding.y);
        if (!ItemAdd(bb, id))
            return false;

        // Render
        float start = g.Time * speed;
        const ImVec2 centre = ImVec2(pos.x + radius, pos.y + radius + style.FramePadding.y);

        const float bg_angle_offset = IM_PI * 2.f / dots;
        dots = std::min<size_t>(dots, 32);

        for (int i = 0; i <= dots; i++)
        {
            float a = start + (i * bg_angle_offset);
            a = fmod(a, 2 * IM_PI);
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(-a) * radius, centre.y + ImSin(-a) * radius), thickness / 2, color, 8);
        }
        window->DrawList->PathClear();

        const float d_ang = (mdots / (float)dots) * 2 * IM_PI;
        const float angle_offset = (d_ang) / dots;
        for (int i = 0; i < dots; i++)
        {
            const float a = start + (i * angle_offset);
            window->DrawList->PathLineTo(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius));
        }
        window->DrawList->PathStroke(color, false, thickness);

        return true;
    }

    bool SpinnerBounceDots(const char *label, int thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = GetCurrentWindow();
      if (window->SkipItems)
        return false;

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ItemSize(bb, style.FramePadding.y);
      if (!ItemAdd(bb, id))
        return false;

      const ImVec2 centre = bb.GetCenter();

      // Render
      float start = g.Time * speed;

      const float offset = IM_PI / dots;
      for (int i = 0; i < dots; i++)
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

    bool SpinnerFadeDots(const char *label, int thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, size_t dots = 3)
    {
      ImGuiWindow *window = GetCurrentWindow();
      if (window->SkipItems)
        return false;

      ImGuiContext &g = *GImGui;
      const ImGuiStyle &style = g.Style;
      const ImGuiID id = window->GetID(label);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8;
      ImVec2 pos = window->DC.CursorPos;
      ImVec2 size( (thickness * nextItemKoeff) * dots + style.FramePadding.x, thickness * 4 * heightKoeff + style.FramePadding.y);

      const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
      ItemSize(bb, style.FramePadding.y);
      if (!ItemAdd(bb, id))
        return false;

      const ImVec2 centre = bb.GetCenter();

      // Render
      float start = g.Time * speed;

      const float offset = IM_PI / dots;
      for (int i = 0; i < dots; i++)
      {
        float a = start + (IM_PI - i * offset);
        ImColor c = color;
        c.Value.w = std::max<float>(0.1, ImSin(a * heightSpeed));
        window->DrawList->AddCircleFilled(ImVec2(pos.x + style.FramePadding.x  + i * (thickness * nextItemKoeff), centre.y), thickness, c, 8);
      }

      return true;
    }

    void demoSpinners() {
      static int hue = 0;
      static float nextdot = 0;
      
      nextdot -= 0.07;

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

      ImGui::SpinnerBounceDots("SpinnerBounceDots", 6, ImColor(255, 255, 255), 6, 3);
      ImGui::SameLine();

      ImGui::SpinnerFadeDots("SpinnerFadeDots", 6, ImColor(255, 255, 255), 8, 3);
    }
}

#endif // _IMSPINNER_H_