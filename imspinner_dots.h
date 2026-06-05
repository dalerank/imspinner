#ifndef _IMSPINNER_DOTS_H_
#define _IMSPINNER_DOTS_H_

/*
 * imspinner dots add-on
 *
 * Dot-based spinners moved out of imspinner.h. Include this header (it pulls in
 * "imspinner.h"). Note: SpinnerDots / SpinnerVDots stay in imspinner.h because the
 * core Spinner<> dispatcher (e_st_dots / e_st_vdots) references them.
 */

#include "imspinner.h"

namespace ImSpinner
{
    inline void SpinnerBounceDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, size_t dots = 3, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float nextItemKoeff = 2.5f;
      const float heightKoeff = 2.f;
      const float heightSpeed = 0.8f;
      const float hsize = dots * (thickness * nextItemKoeff) / 2.f - (thickness * nextItemKoeff) * 0.5f;

      float start = (float)ImGui::GetTime() * speed;
      const float offset = PI_DIV(dots);
      for (size_t i = 0; i < dots; i++) {
        float a = start + (IM_PI - i * offset);
        switch (mode) {
        case 1: a = damped_spring(1, 10.f, 1.0f, ImSin(ImFmod(start + i * PI_DIV(dots * 2), PI_2))); break;
        case 2: a = damped_infinity((float)(start + i * PI_DIV(dots * 2)), radius).second; break;
        }
        float y =  centre.y + ImSin(a * heightSpeed) * thickness * heightKoeff;
        window->DrawList->AddCircleFilled(ImVec2(centre.x - hsize + i * (thickness * nextItemKoeff), ImMin(y, centre.y)), thickness, color_alpha(color, 1.f), 8);
      }
    }

    inline void SpinnerZipDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, size_t dots = 5)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 3.5f;
        const float heightKoeff = 2.f;
        const float heightSpeed = 0.8f;
        const float hsize = dots * (thickness * nextItemKoeff) / 2.f - (thickness * nextItemKoeff) * 0.5f;
        const float start = (float)ImGui::GetTime() * speed;
        const float offset = PI_DIV(dots);

        for (size_t i = 0; i < dots; i++)
        {
            const float sina = ImSin((start + (IM_PI - i * offset)) * heightSpeed);
            const float y = ImMin(centre.y + sina * thickness * heightKoeff, centre.y);
            const float deltay = ImAbs(y - centre.y);
            window->DrawList->AddCircleFilled(ImVec2(centre.x - hsize + i * (thickness * nextItemKoeff), y), thickness, color_alpha(color, 1.f), 8);
            window->DrawList->AddCircleFilled(ImVec2(centre.x - hsize + i * (thickness * nextItemKoeff), y + 2 * deltay), thickness, color_alpha(color, 1.f), 8);
        }
    }

    inline void SpinnerDotsToPoints(const char *label, float radius, float thickness, float offset_k, const ImColor &color = white, float speed = 1.8f, size_t dots = 5)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 3.5f;
        const float hsize = dots * (thickness * nextItemKoeff) / 2.f - (thickness * nextItemKoeff) * 0.5f;
        const float start = ImFmod((float)ImGui::GetTime() * speed, PI_2);
        const float offset = PI_DIV(dots);

        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        if (start < PI_DIV_2) {
            const float sina = ImSin(start);
            for (size_t i = 0; i < dots; i++) {
                const float xx = ImMax(sina * (i * (thickness * nextItemKoeff)), 0.f);
                ImColor c = color_alpha(ImColor::HSV(out_h + i * ((1.f / dots) * 2.f), out_s, out_v), 1.f);
                window->DrawList->AddCircleFilled(ImVec2(centre.x - hsize + xx, centre.y), thickness, c, 8);
            }
        } else {
            for (size_t i = 0; i < dots; i++) {
                const float sina = ImSin(ImMax(start - (IM_PI / dots) * i, PI_DIV_2));
                const float xx = ImMax(1.f * (i * (thickness * nextItemKoeff)), 0.f);
                const float th = sina * thickness;
                ImColor c = color_alpha(ImColor::HSV(out_h + i * ((1.f / dots) * 2.f), out_s, out_v), 1.f);
                window->DrawList->AddCircleFilled(ImVec2(centre.x - hsize + xx, centre.y), th, c, 8);
            }
        }
    }

    inline void SpinnerDotsToBar(const char *label, float radius, float thickness, float offset_k, const ImColor &color = white, float speed = 2.8f, size_t dots = 5)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 3.5f;
        const float heightSpeed = 0.8f;
        const float hsize = dots * (thickness * nextItemKoeff) / 2.f - (thickness * nextItemKoeff) * 0.5f;
        const float start = (float)ImGui::GetTime() * speed;
        const float offset = PI_DIV(dots);
        const float hradius = (radius);

        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        for (size_t i = 0; i < dots; i++)
        {
            const float sina = ImSin((start + (IM_PI - i * offset)) * heightSpeed);
            const float sinb = ImSin((start + (IM_PI + IM_PI * offset_k - i * offset)) * heightSpeed);
            const float y = ImMin(centre.y + sina * hradius, centre.y);
            const float y2 = ImMin(sinb, 0.f) * (hradius * offset_k);
            const float y3 = (y + y2);
            const float deltay = ImAbs(y - centre.y);
            ImColor c = color_alpha(ImColor::HSV(out_h + i * ((1.f / dots) * 2.f), out_s, out_v), 1.f);
            ImVec2 p1(centre.x - hsize + i * (thickness * nextItemKoeff), y3);
            ImVec2 p2(centre.x - hsize + i * (thickness * nextItemKoeff), y3 + 2 * deltay);
            window->DrawList->AddCircleFilled(p1, thickness, c, 8);
            window->DrawList->AddCircleFilled(p2, thickness, c, 8);
            window->DrawList->AddLine(p1, p2, c, thickness * 2.f);
        }
    }

    inline void SpinnerWaveDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int lt = 8)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 2.5f;
        const float dots = (size.x / (thickness * nextItemKoeff));
        const float offset = PI_DIV(dots);
        const float start = (float)ImGui::GetTime() * speed;

        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        for (size_t i = 0; i < dots; i++)
        {
            float a = start + (IM_PI - i * offset);
            float y = centre.y + ImSin(a) * (size.y / 2.f);
            ImColor c = ImColor::HSV(out_h + i * (1.f / dots * 2.f), out_s, out_v);
            window->DrawList->AddCircleFilled(ImVec2(centre.x - (size.x / 2.f) + i * thickness * nextItemKoeff, y), thickness, color_alpha(c, 1.f), lt);
        }
    }

    inline void SpinnerFadeDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int lt = 8, int mode = 0)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        const float nextItemKoeff = 2.5f;
        const float dots = (size.x / (thickness * nextItemKoeff));
        const float heightSpeed = 0.8f;

        for (size_t i = 0; i < dots; i++)
        {
          float a = mode 
                        ? damped_spring(1, 10.f, 1.0f, ImSin(ImFmod(start + (IM_PI - i * (IM_PI / dots)), PI_2)))
                        : ImSin(start + (IM_PI - i * (IM_PI / dots)) * heightSpeed);
          window->DrawList->AddCircleFilled(ImVec2(centre.x - (size.x / 2.f) + i * thickness * nextItemKoeff, centre.y), thickness, color_alpha(color, ImMax(0.1f, a)), lt);
        }
    }

    inline void SpinnerThreeDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int lt = 8)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = ImFmod((float)ImGui::GetTime() * speed, PI_2);
        const float nextItemKoeff = 2.5f;
        const float offset = size.x / 4.f;

        float ab = start;
        int msize = 2;
        if (start < IM_PI) { ab = 0; msize = 1; }
        for (size_t i = 0; i < msize; i++)
        {
            float a = ab + i * IM_PI - PI_DIV_2;
            window->DrawList->AddCircleFilled(ImVec2(centre.x - offset + ImSin(a) * offset, centre.y + ImCos(a) * offset), thickness, color_alpha(color, 1.f), lt);
        }

        float ba = start; msize = 2;
        if (start > IM_PI && start < PI_2) { ba = 0; msize = 1; }
        for (size_t i = 0; i < msize; i++)
        {
            float a = -ba + i * IM_PI + PI_DIV_2;
            window->DrawList->AddCircleFilled(ImVec2(centre.x + offset + ImSin(a) * offset, centre.y + ImCos(a) * offset), thickness, color_alpha(color, 1.f), lt);
        }
    }

    inline void SpinnerFiveDots(const char *label, float radius, float thickness, const ImColor &color = 0xffffffff, float speed = 2.8f, int lt = 8)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = ImFmod((float)ImGui::GetTime() * speed, PI_2 * 2);
        const float nextItemKoeff = 2.5f;
        const float offset = size.x / 4.f;

        float ab = 0;
        int msize = 1;
        if (start < IM_PI) { ab = start; msize = 2; }
        for (size_t i = 0; i < msize; i++)
        {
            float a = -ab + i * IM_PI - PI_DIV_2;
            window->DrawList->AddCircleFilled(ImVec2(centre.x - offset + ImSin(a) * offset, centre.y + ImCos(a) * offset), thickness, color_alpha(color, 1.f), lt);
        }

        float ba = 0; msize = 1;
        if (start > IM_PI && start < PI_2) { ba = start; msize = 2; }
        for (size_t i = 0; i < msize; i++)
        {
            float a = -ba + i * IM_PI;
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(a) * offset, centre.y + offset + ImCos(a) * offset), thickness, color_alpha(color, 1.f), lt);
        }

        float bc = 0; msize = 1;
        if (start > PI_2 && start < IM_PI * 3) { bc = start; msize = 2; }
        for (size_t i = 0; i < msize; i++)
        {
            float a = -bc + i * IM_PI - IM_PI;
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(a) * offset, centre.y - offset + ImCos(a) * offset), thickness, color_alpha(color, 1.f), lt);
        }

        float bd = 0; msize = 1;
        if (start > IM_PI * 3 && start < IM_PI * 4) { bd = start; msize = 2; }
        for (size_t i = 0; i < msize; i++)
        {
            float a = -bd + i * IM_PI + PI_DIV_2;
            window->DrawList->AddCircleFilled(ImVec2(centre.x + offset + ImSin(a) * offset, centre.y + ImCos(a) * offset), thickness, color_alpha(color, 1.f), lt);
        }
    }

    inline void SpinnerMultiFadeDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int lt = 8)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        const float nextItemKoeff = 2.5f;
        const float dots = (size.x / (thickness * nextItemKoeff));
        const float heightSpeed = 0.8f;

        for (size_t j = 0; j < dots; j++)
        {
            for (size_t i = 0; i < dots; i++)
            {
                float a = start - (IM_PI - i * j * PI_DIV(dots));
                window->DrawList->AddCircleFilled(ImVec2(centre.x - (size.x / 2.f) + i * thickness * nextItemKoeff, centre.y - (size.y / 2.f) + j * thickness * nextItemKoeff), thickness, color_alpha(color, ImMax(0.1f, ImSin(a * heightSpeed))), lt);
            }
        }
    }

    inline void SpinnerScaleDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int lt = 8)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 2.5f;
        const float heightSpeed = 0.8f;
        const float dots = (size.x / (thickness * nextItemKoeff));
        const float start = (float)ImGui::GetTime() * speed;

        for (size_t i = 0; i < dots; i++)
        {
          const float a = start + (IM_PI - i * PI_DIV(dots));
          const float th = thickness * ImSin(a * heightSpeed);
          window->DrawList->AddCircleFilled(ImVec2(centre.x - (size.x / 2.f) + i * thickness * nextItemKoeff, centre.y), thickness, color_alpha(color, 0.1f), lt);
          window->DrawList->AddCircleFilled(ImVec2(centre.x - (size.x / 2.f) + i * thickness * nextItemKoeff, centre.y), th, color_alpha(color, 1.f), lt);
        }
    }

    inline void SpinnerMovingDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, size_t dots = 3)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 2.5f;
        const float heightKoeff = 2.f;
        const float heightSpeed = 0.8f;
        const float start = ImFmod((float)ImGui::GetTime() * speed, size.x);

        float offset = 0;
        for (size_t i = 0; i < dots; i++)
        {
          float th = thickness;
          offset = ImFmod(start + i * (size.x / dots), size.x);

          if (offset < thickness) { th = offset; }
          if (offset > size.x - thickness) { th = size.x - offset; }
        
          window->DrawList->AddCircleFilled(ImVec2(pos.x + offset - thickness, centre.y), th, color_alpha(color, 1.f), 8);
        }
    }

    inline void SpinnerRotateDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int dots = 2, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      ImGuiStorage* storage = window->DC.StateStorage;
      const ImGuiID velocityId = window->GetID("##velocity");
      const ImGuiID vtimeId = window->GetID("##velocitytime");

      float velocity = storage->GetFloat(velocityId, 0.f);
      float vtime = storage->GetFloat(vtimeId, 0.f);
     
      float dtime = ImFmod((float)vtime, IM_PI);
      float start = (vtime += velocity);
      if (dtime > 0.f && dtime < PI_DIV_2) { velocity += 0.001f * speed; }
      else if (dtime > IM_PI * 0.9f && dtime < IM_PI) { velocity -= 0.01f * speed; }
      if (velocity > 0.1f) velocity = 0.1f;
      if (velocity < 0.01f) velocity = 0.01f;

      storage->SetFloat(velocityId, velocity);
      storage->SetFloat(vtimeId, vtime);

      window->DrawList->AddCircleFilled(centre, thickness, color_alpha(color, 1.f), 8);

      for (int i = 0; i < dots; i++)
      {
          float a = 0.f;
          switch (mode) {
          case 1: a = start + i * PI_2_DIV(dots) + damped_spring(1, 10.f, 1.0f, ImSin(start + i * PI_2_DIV(dots)), PI_2_DIV(dots), 0); break;
          case 2: a = start + i * PI_2_DIV(dots) + damped_infinity(start + i * PI_DIV(dots * 2), 1.f).second; break;
          default:
            a = start + (i * PI_2_DIV(dots));
          }
          window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, color_alpha(color, 1.f), 8);
      }

      if (mode == 3) {
          float sm_thickness = thickness * 0.5f;
          const float bstart = (float)ImGui::GetTime() * speed;
          for (size_t i = 0; i < dots; i++)
          {
              const float b = bstart + (IM_PI - i * PI_DIV(dots) * 2.f);
              window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(b) * radius, centre.y + ImSin(b) * radius), sm_thickness, color_alpha(color, 1.f), 8);
          }
      }
    }

    inline void SpinnerOrionDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int arcs = 4)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        ImGuiStorage* storage = window->DC.StateStorage;
        const ImGuiID velocityId = window->GetID("##velocity");
        const ImGuiID vtimeId = window->GetID("##velocitytime");

        float velocity = storage->GetFloat(velocityId, 0.f);
        float vtime = storage->GetFloat(vtimeId, 0.f);

        float dtime = ImFmod((float)vtime, IM_PI);
        float start = (vtime += velocity);
        if (dtime > 0.f && dtime < PI_DIV_2) { velocity += 0.001f * speed; }
        else if (dtime > IM_PI * 0.9f && dtime < IM_PI) { velocity -= 0.01f * speed; }

        if (velocity > 0.1f) velocity = 0.1f;
        if (velocity < 0.01f) velocity = 0.01f;

        storage->SetFloat(velocityId, velocity);
        storage->SetFloat(vtimeId, vtime);

        window->DrawList->AddCircleFilled(centre, thickness, color_alpha(color, 1.f), 8);

        for (int j = 1; j < arcs; ++j) {
            const float r = (radius / (arcs + 1)) * j;
            for (int i = 0; i < j + 1; i++)
            {
                const float a = start + (i * PI_2_DIV(j+1));
                window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * r, centre.y + ImSin(a) * r), thickness, color_alpha(color, 1.f), 8);
            }
        }
    }

    inline void SpinnerGalaxyDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int arcs = 4)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        ImGuiStorage* storage = window->DC.StateStorage;
        const ImGuiID velocityId = window->GetID("##velocity");
        const ImGuiID vtimeId = window->GetID("##velocitytime");

        float velocity = storage->GetFloat(velocityId, 0.f);
        float vtime = storage->GetFloat(vtimeId, 0.f);

        float dtime = ImFmod((float)vtime, IM_PI);
        float start = (vtime += (velocity * speed));
        if (dtime > 0.f && dtime < PI_DIV_2) { velocity += 0.001f; }
        else if (dtime > IM_PI * 0.9f && dtime < IM_PI) { velocity -= 0.01f; }

        if (velocity > 0.1f) velocity = 0.1f;
        if (velocity < 0.01f) velocity = 0.01f;

        storage->SetFloat(velocityId, velocity);
        storage->SetFloat(vtimeId, vtime);

        window->DrawList->AddCircleFilled(centre, thickness, color_alpha(color, 1.f), 8);

        for (int j = 1; j < arcs; ++j) {
            const float r = ((j / (float)arcs) * radius);
            for (int i = 0; i < arcs; i++)
            {
                const float a = start * (1.f + j * 0.1f) + (i * PI_2_DIV(arcs));
                window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * r, centre.y + ImSin(a) * r), thickness, color_alpha(color, 1.f), 8);
            }
        }
    }

    inline void SpinnerIncDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, size_t dots = 6)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      float start = (float)ImGui::GetTime() * speed;
      float astart = ImFmod(start, PI_DIV(dots));
      start -= astart;
      dots = ImMin<size_t>(dots, 32);

      for (size_t i = 0; i <= dots; i++)
      {
        float a = start + (i * PI_DIV(dots - 1));
        ImColor c = color_alpha(color, ImMax(0.1f, i / (float)dots));
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, c, 8);
      }
    }

    inline void SpinnerIncFullDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, size_t dots = 4)
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
        ImColor c = color_alpha(color, 0.1f);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, c, 8);
      }

      for (size_t i = 0; i < dots; i++)
      {
        float a = start + (i * bg_angle_offset);
        ImColor c = color_alpha(color, ImMax(0.1f, i / (float)dots));
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), thickness, c, 8);
      }
    }

    inline void SpinnerIncScaleDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, size_t dots = 6, float angle = 0.f, int mode = 0)
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
        a += ease((ease_mode)mode, a, angle);
        float th = thickness * ImMax(0.1f, i / (float)dots);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * radius, centre.y + ImSin(a) * radius), th, color_alpha(color, 1.f), 8);
      }
    }

    inline void SpinnerSomeScaleDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, size_t dots = 6, int mode = 0)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        float start = (float)ImGui::GetTime() * speed;
        float astart = ImFmod(start, IM_PI / dots);
        start -= astart;
        const float bg_angle_offset = IM_PI / dots;
        dots = ImMin(dots, (size_t)32);

        for (size_t j = 0; j < 4; j++)
        {
            float r = radius * (1.f - (0.15f * j));
            for (size_t i = 0; i <= dots; i++)
            {
                float a = start * (mode ? (1.f + j * 0.05f) : 1.f) + (i * bg_angle_offset);
                float th = thickness * ImMax(0.1f, i / (float)dots);
                float thh = th * (1.f - (0.2f * j));
                window->DrawList->AddCircleFilled(ImVec2(centre.x + ImCos(a) * r, centre.y + ImSin(a) * r), thh, color_alpha(color, 1.f), 8);
            }
        }
    }

    inline void SpinnerDotsLoading(const char *label, float radius, float thickness, const ImColor &color, const ImColor &bg, float speed)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = ImFmod((float)ImGui::GetTime() * speed, IM_PI);
        const float radius1 = (2.f * ImSin(start)) * radius;

        float startb = ImFmod(start, PI_DIV_2);
        float lenb = startb < PI_DIV_2 ? ImAbs((0.5f * ImSin(start * 2)) * radius) : radius * 0.5f;
        float radius2 = radius * 0.25f;

        float deltae = thickness - ImMin(thickness, ImMax<float>(0, (2.f * radius - radius1 + thickness + lenb) * 0.25f));
        float deltag = ImMin(thickness, ImAbs(centre.x - radius + radius1 + thickness + lenb - centre.x - radius) * 0.25f);
        window->DrawList->AddCircleFilled(ImVec2(centre.x - radius, centre.y), radius2 + deltag, color_alpha(bg, 1.f), num_segments);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + radius + thickness, centre.y), radius2 + deltae, color_alpha(bg, 1.f), num_segments);

        window->DrawList->AddRectFilled(ImVec2(centre.x - radius + radius1 - thickness - lenb, centre.y - thickness), ImVec2(centre.x - radius + radius1 + thickness + lenb, centre.y + thickness), color_alpha(color, 1.f), thickness);
    }

    inline void SpinnerSquareRandomDots(const char *label, float radius, float thickness, const ImColor &bg, const ImColor &color, float speed)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float offset_block = radius * 2.f / 3.f;
        ImVec2 lt{centre.x - offset_block, centre.y - offset_block};

        int start = (int)ImFmod((float)ImGui::GetTime() * speed, 9.f);

        ImGuiStorage* storage = window->DC.StateStorage;
        const ImGuiID vtimeId = window->GetID("##vtime");
        const ImGuiID vvald = window->GetID("##vval");
        int vtime = storage->GetInt(vtimeId, 0);
        int vval = storage->GetInt(vvald, 0);

        if (vtime != start) {
            vval = rand() % 9;
            storage->SetInt(vvald, vval);
            storage->SetInt(vtimeId, start);
        }

        const ImVec2ih poses[] = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {1, 2}, {0, 2}, {0, 1}, {1, 1}};
        int ti = 0;
        for (const auto &rpos: poses)
        {
            const ImColor &c = (ti == vval) ? color : bg;
            window->DrawList->AddCircleFilled(ImVec2(lt.x + rpos.x * (offset_block), lt.y + rpos.y * offset_block), thickness,
                                              color_alpha(c, 1.f));
            ti++;
        }
    }

    // spinner idea by nitz 'Chris Dailey'
    inline void SpinnerHboDots(const char *label, float radius, float thickness, const ImColor &color = white, float minfade = 0.0f, float ryk = 0.f, float speed = 1.1f, size_t dots = 6, int mode = 0)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;

        for (size_t i = 0; i < dots; i++)
        {
            const float ab = ease((ease_mode)mode, start + i * PI_DIV(2) / dots, IM_PI, 1.0f, 0.0f);
            const float astart = start + ab + PI_2_DIV(dots) * i;
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(astart) * radius, centre.y + ryk * ImCos(astart) * radius), thickness,
                                              color_alpha(color, ImMax(minfade, ImSin(astart + PI_DIV_2))),
                                              8);
        }
    }

    inline void SpinnerMoonDots(const char *label, float radius, float thickness, const ImColor &first, const ImColor &second, float speed = 1.1f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;

        const float astart = ImFmod(start, IM_PI * 2.f);
        const float bstart = astart + IM_PI;

        const float sina = ImSin(astart);
        const float sinb = ImSin(bstart);

        if (astart < PI_DIV_2 || astart > IM_PI + PI_DIV_2) {
            window->DrawList->AddCircleFilled(ImVec2(centre.x + sina * thickness, centre.y), thickness, color_alpha(first, 1.f), 16);
            window->DrawList->AddCircleFilled(ImVec2(centre.x + sinb * thickness, centre.y), thickness, color_alpha(second, 1.f), 16);
            window->DrawList->AddCircle(ImVec2(centre.x + sinb * thickness, centre.y), thickness, color_alpha(first, 1.f), 16);
        } else {
            window->DrawList->AddCircleFilled(ImVec2(centre.x + sinb * thickness, centre.y), thickness, color_alpha(second, 1.f), 16);
            window->DrawList->AddCircle(ImVec2(centre.x + sinb * thickness, centre.y), thickness, color_alpha(first, 1.f), 16);
            window->DrawList->AddCircleFilled(ImVec2(centre.x + sina * thickness, centre.y), thickness, color_alpha(first, 1.f), 16);
        }
    }

    inline void SpinnerTwinHboDots(const char *label, float radius, float thickness, const ImColor &color = white, float minfade = 0.0f, float ryk = 0.f, float speed = 1.1f, size_t dots = 6, float delta = 0.f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;

        for (size_t i = 0; i < dots; i++)
        {
            const float astart = start + PI_2_DIV(dots) * i;
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(astart) * radius, centre.y + ryk * ImCos(astart) * radius + radius * delta), thickness,
                                              color_alpha(color, ImMax(minfade, ImSin(astart + PI_DIV_2))),
                                              8);
        }

        for (size_t i = 0; i < dots; i++)
        {
            const float astart = start + PI_2_DIV(dots) * i;
            window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(astart) * radius, centre.y - ryk * ImCos(astart) * radius - radius * delta), thickness,
                                              color_alpha(color, ImMax(minfade, ImSin(astart + PI_DIV_2))),
                                              8);
        }
    }

    inline void SpinnerThreeDotsStar(const char *label, float radius, float thickness, const ImColor &color = white, float minfade = 0.0f, float ryk = 0.f, float speed = 1.1f, float delta = 0.f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;

        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(-start) * radius, centre.y - ryk * ImCos(-start) * radius + radius * delta), thickness, color_alpha(color, ImMax(minfade, ImSin(-start + PI_DIV_2))), 8);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(start) * radius, centre.y - ryk * ImCos(start) * radius - radius * delta), thickness, color_alpha(color, ImMax(minfade, ImSin(start + PI_DIV_2))), 8);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ImSin(start + PI_DIV_4) * radius, centre.y - ryk * ImCos(start + PI_DIV_4) * radius - radius * delta), thickness, color_alpha(color, ImMax(minfade, ImSin(start + PI_DIV_4 + PI_DIV_2))), 8);
    }

    inline void SpinnerSwingDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float start = (float)ImGui::GetTime() * speed;
        constexpr int elipses = 2;

        auto get_rotated_ellipse_pos = [&] (float alpha, float start) {
            std::array<ImVec2, 36> pts;

            alpha = ImFmod(alpha, IM_PI);
            float a = radius;
            float b = radius / 10.f; 

            float anga = ImFmod(start, PI_2);
            float x = a * ImCos(anga) * ImCos(alpha) + b * ImSin(anga) * ImSin(alpha) + centre.x;
            float y = b * ImSin(anga) * ImCos(alpha) - a * ImCos(anga) * ImSin(alpha) + centre.y;
            return ImVec2{x, y};
        };

        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        for (int i = 0; i < elipses; ++i) {
            ImVec2 ppos = get_rotated_ellipse_pos((IM_PI * (float)i/ elipses) + PI_DIV_4, start + PI_DIV_2 * i);
            const float y_delta = ImAbs(centre.y - ppos.y);
            float th_koeff = ImMax((y_delta / size.y) * 4.f, 0.5f);
            window->DrawList->AddCircleFilled(ppos, th_koeff * thickness, color_alpha(ImColor::HSV(out_h + i * 0.5f, out_s, out_v), 1.f), num_segments);
        }
    }

    inline void SpinnerDnaDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.8f, int lt = 8, float delta = 0.5f, bool mode = 0)
    {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 2.5f;
        const float dots = (size.x / (thickness * nextItemKoeff));
        const float start = ImFmod((float)ImGui::GetTime() * speed, PI_2);

        float out_h, out_s, out_v;
        ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, out_h, out_s, out_v);
        auto draw_point = [&] (float angle, int i) {
            float a = angle + start + (IM_PI - i * PI_DIV(dots));
            float th_koeff = 1.f + ImSin(a + PI_DIV_2) * 0.5f;

            float pp = mode ? centre.x + ImSin(a) * size.x * delta
                            : centre.y + ImSin(a) * size.y * delta;
            ImColor c = ImColor::HSV(out_h + i * (1.f / dots * 2.f), out_s, out_v);
            ImVec2 p = mode ? ImVec2(pp, centre.y - (size.y * 0.5f) + i * thickness * nextItemKoeff)
                            : ImVec2(centre.x - (size.x * 0.5f) + i * thickness * nextItemKoeff, pp);
            window->DrawList->AddCircleFilled(p, thickness * th_koeff, color_alpha(c, 1.f), lt);
            return p;
        };


        for (int i = 0; i < dots; i++) {
            ImVec2 p1 = draw_point(0, i);
            ImVec2 p2 = draw_point(IM_PI, i);
            window->DrawList->AddLine(p1, p2, color_alpha(color, 1.f), thickness * 0.5f);
        }
    }

    inline void Spinner3SmuggleDots(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 4.8f, int lt = 8, float delta = 0.5f, bool mode = 0)     {
        SPINNER_HEADER(pos, size, centre, num_segments);

        const float nextItemKoeff = 2.5f;
        const float dots = 2;// (size.x / (thickness * nextItemKoeff));
        const float start = ImFmod((float)ImGui::GetTime() * speed, PI_2);

        auto draw_point = [&] (float angle, int i, float k) {
            float a = angle + k * start + k * (IM_PI - i * PI_DIV(dots));
            float th_koeff = 1.f + ImSin(a + PI_DIV_2) * 0.3f;

            float pp = mode ? centre.x + ImSin(a) * size.x * delta
                            : centre.y + ImSin(a) * size.y * delta;
            ImVec2 p = mode ? ImVec2(pp, centre.y - (size.y * 0.5f) + i * thickness * nextItemKoeff)
                            : ImVec2(centre.x - (size.x * 0.5f) + i * thickness * nextItemKoeff, pp);
            window->DrawList->AddCircleFilled(p, thickness * th_koeff, color_alpha(color, 1.f), lt);
            return p;
        };

        {
            ImVec2 p1 = draw_point(0, 1, -1);
            ImVec2 p2 = draw_point(IM_PI, 2, 1);
            //window->DrawList->AddLine(p1, p2, color_alpha(color, 1.f), thickness * 0.5f);
            ImVec2 p3 = draw_point(PI_DIV_2, 3, -1);
            //window->DrawList->AddLine(p2, p3, color_alpha(color, 1.f), thickness * 0.5f);
        }
    }
}

#endif // _IMSPINNER_DOTS_H_
