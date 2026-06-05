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

    // Typing dots:
    //   a row of dots appears one by one (. -> .. -> ...) via a stepped clip, then
    //   resets. Drawn as 'dots' filled circles laid across the cell width.
    inline void SpinnerDotsTyping(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int dots = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const int n = ImMax(1, dots);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int shown = ImMin(n, (int)(t * (n + 1)) + 1);    // 1..n (CSS steps(n+1))
      const float step = (radius * 2.f) / (float)n;

      for (int i = 0; i < shown; i++) {
        const ImVec2 c(centre.x - radius + (i + 0.5f) * step, centre.y);
        window->DrawList->AddCircleFilled(c, thickness, color_alpha(color, 1.f), num_segments);
      }
    }

    // Stepping dot:
    //   a single dot hops across 'dots' slots (left -> ... -> right) in discrete
    //   steps, then jumps back. Only one dot is shown at a time.
    inline void SpinnerDotsStep(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int dots = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const int n = ImMax(1, dots);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int k = ImMin(n - 1, (int)(t * n));              // current slot (CSS steps(n))
      const float step = (radius * 2.f) / (float)n;
      const ImVec2 c(centre.x - radius + (k + 0.5f) * step, centre.y);
      window->DrawList->AddCircleFilled(c, thickness, color_alpha(color, 1.f), num_segments);
    }

    // Gathering dots:
    //   a row of dots spreads apart, then squeezes together at the centre (the dots
    //   also grow a little when gathered), ping-ponging.
    inline void SpinnerDotsGather(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 0.5f, int dots = 3)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const int n = ImMax(1, dots);
      const float ph = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float tri = (ph < 0.5f) ? ph * 2.f : 2.f - ph * 2.f; // 0..1..0 (CSS alternate)
      const float s = 1.f - tri;                             // 1 = spread, 0 = gathered
      const float r = thickness * (1.f + 0.5f * tri);        // grow a bit when gathered

      for (int i = 0; i < n; i++) {
        const float off = (n > 1) ? ((float)i / (n - 1) - 0.5f) * 2.f * radius * s : 0.f;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + off, centre.y), r, color_alpha(color, 1.f), num_segments);
      }
    }

    // Shifting dots:
    //   three dots; the centre one is fixed while the left dot pulls in toward it,
    //   then the right dot pushes out, ping-ponging (a little shuffle). Offsets are
    //   proportional to the dot size, as in the CSS box-shadow.
    inline void SpinnerDotsShift(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 0.5f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float ph = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float u = (ph < 0.5f) ? ph * 2.f : 2.f - ph * 2.f; // 0..1..0 (CSS alternate)
      const float th = thickness;
      float lx, rx;                                          // left/right offsets, in dot-radii (CSS px / 7.5)
      if (u < 0.5f) { lx = (-3.33f + 1.33f * (u / 0.5f)) * th; rx = 2.f * th; }
      else          { lx = -2.f * th; rx = (2.f + 1.33f * ((u - 0.5f) / 0.5f)) * th; }

      const ImColor c = color_alpha(color, 1.f);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + lx, centre.y), th, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x, centre.y), th, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + rx, centre.y), th, c, num_segments);
    }

    // Orbiting dots:
    //   three dots dance: a fixed left dot, a dot sliding right, and a dot orbiting a
    //   half-circle over them; the whole thing mirror-flips each half-cycle. The flip
    //   lands when the layout is symmetric, so it stays seamless. Offsets are in
    //   dot-radii (CSS px / 7.5).
    inline void SpinnerDotsOrbit(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 0.66f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float th = thickness;
      const float a = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float p = (a < 0.5f) ? a * 2.f : 2.f - a * 2.f;  // 0..1..0
      const float flip = (a < 0.5f) ? 1.f : -1.f;            // scale(-1) every half-cycle (steps(2))
      const float theta = -IM_PI * p;
      const ImColor c = color_alpha(color, 1.f);
      auto dot = [&](float x, float y) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + flip * x * th, centre.y + y * th), th, c, num_segments);
      };
      dot(-3.47f, 0.f);                                      // ::before main (fixed left)
      dot(3.47f * p, 0.f);                                   // ::before shadow (slides right)
      dot(1.73f + 1.73f * ImCos(theta), 1.73f * ImSin(theta)); // ::after (orbits a half-circle)
    }

    // Circling dots:
    //   two dots slide right and back together while a third dot orbits a full circle
    //   around the centre. Offsets are in dot-radii (CSS px / 7.5).
    inline void SpinnerDotsCircle(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 0.66f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float th = thickness;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float tx = (t < 0.5f) ? t * 2.f : 2.f - t * 2.f; // 0..1..0 (slide there and back)
      const float theta = -PI_2 * t;                         // full circle
      const ImColor c = color_alpha(color, 1.f);
      auto dot = [&](float x, float y) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x * th, centre.y + y * th), th, c, num_segments);
      };
      dot(3.47f * tx, 0.f);                                  // ::before main (slides right and back)
      dot(3.47f * (tx - 1.f), 0.f);                          // ::before shadow (trails it)
      dot(3.47f * ImCos(theta), 3.47f * ImSin(theta));       // ::after (orbits a full circle)
    }

    // Square-chasing dot:
    //   three dots sit on three corners of a square; each quarter one dot slides along
    //   an edge into the empty corner, so the gap chases around the square. The 0%/100%
    //   configurations match (same corner set), so it loops seamlessly.
    inline void SpinnerDotsSquare(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float th = thickness;
      const float r = ImMax(radius - thickness, thickness);
      const ImVec2 TL(-r, -r), TR(r, -r), BL(-r, r), BR(r, r);
      const ImVec2 d1[5] = { TL, TR, TR, TR, BR };
      const ImVec2 d2[5] = { BL, BL, TL, TL, TL };
      const ImVec2 d3[5] = { BR, BR, BR, BL, BL };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int seg = ImMin(3, (int)(t * 4.f));
      const float u = t * 4.f - seg;
      const ImColor c = color_alpha(color, 1.f);
      auto draw = [&](const ImVec2 *d) {
        const ImVec2 p(d[seg].x + (d[seg + 1].x - d[seg].x) * u, d[seg].y + (d[seg + 1].y - d[seg].y) * u);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + p.x, centre.y + p.y), th, c, num_segments);
      };
      draw(d1);
      draw(d2);
      draw(d3);
    }

    // Shuffling dots:
    //   four dots fan out of a row into little loops and shuffle back, swapping
    //   places; the 0%/100% sets match, so it loops seamlessly. Offsets are in
    //   dot-radii (x in {-3,0,3}, y in {-1,0,1}).
    inline void SpinnerDotsShuffle(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float th = thickness;
      const ImVec2 d1[4] = { ImVec2(-3.f, 0.f), ImVec2(-3.f, -1.f), ImVec2(0.f, -1.f), ImVec2(0.f, 0.f) };
      const ImVec2 d2[4] = { ImVec2(0.f, 0.f), ImVec2(0.f, 1.f), ImVec2(-3.f, 1.f), ImVec2(-3.f, 0.f) };
      const ImVec2 d3[4] = { ImVec2(0.f, 0.f), ImVec2(0.f, 1.f), ImVec2(3.f, 1.f), ImVec2(3.f, 0.f) };
      const ImVec2 d4[4] = { ImVec2(3.f, 0.f), ImVec2(3.f, -1.f), ImVec2(0.f, -1.f), ImVec2(0.f, 0.f) };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int seg = ImMin(2, (int)(t * 3.f));
      const float u = t * 3.f - seg;
      const ImColor c = color_alpha(color, 1.f);
      auto draw = [&](const ImVec2 *d) {
        const float x = (d[seg].x + (d[seg + 1].x - d[seg].x) * u) * th;
        const float y = (d[seg].y + (d[seg + 1].y - d[seg].y) * u) * th;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + y), th, c, num_segments);
      };
      draw(d1);
      draw(d2);
      draw(d3);
      draw(d4);
    }

    // Splitting dots:
    //   three columns, each a pair of dots that opens (one up, one down to the edges)
    //   and closes (meets at the centre), staggered so a split-wave runs left to right.
    //   Y values are in half-height units; the 83%->100% segment closes the last column.
    inline void SpinnerDotsSplit(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      // upper / lower dot Y per column over 6 keyframes (0, 1/6 .. 5/6); -1=top, +1=bottom, ±0.25=closed.
      static const float U[3][6] = {
        { -0.25f, -1.00f, -1.00f, -1.00f, -0.25f, -0.25f },
        { -0.25f, -0.50f, -1.00f, -1.00f, -1.00f, -0.25f },
        { -0.25f, -0.25f, -0.25f, -1.00f, -1.00f, -1.00f }
      };
      static const float L[3][6] = {
        {  0.25f,  1.00f,  1.00f,  1.00f,  0.25f,  0.25f },
        {  0.25f,  0.25f,  1.00f,  1.00f,  1.00f,  0.25f },
        {  0.25f,  0.25f,  0.25f,  1.00f,  1.00f,  1.00f }
      };

      const float th = thickness;
      const float colspan = radius * 0.7f;
      const float spread = radius * 0.6f;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const int seg = ImMin(5, (int)(t * 6.f));
      const int nxt = (seg + 1) % 6;
      const float u = t * 6.f - seg;
      const ImColor c = color_alpha(color, 1.f);

      for (int col = 0; col < 3; col++) {
        const float x = centre.x + (col - 1) * colspan;
        const float yu = (U[col][seg] + (U[col][nxt] - U[col][seg]) * u) * spread;
        const float yl = (L[col][seg] + (L[col][nxt] - L[col][seg]) * u) * spread;
        window->DrawList->AddCircleFilled(ImVec2(x, centre.y + yu), th, c, num_segments);
        window->DrawList->AddCircleFilled(ImVec2(x, centre.y + yl), th, c, num_segments);
      }
    }

    // Leader dots:
    //   a full dot sweeps left to right while two trailing pairs open to the edges and
    //   shift columns. A closed pair sits near the centre and reads as a single dot, so
    //   the leader/pair swap at the loop looks seamless. X in {-1,0,1}, Y in half-heights.
    inline void SpinnerDotsLeader(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      static const float X[5][4] = {
        { -1.f, -1.f,  1.f,  1.f },                          // leader
        {  0.f,  0.f, -1.f, -1.f }, {  0.f,  0.f, -1.f, -1.f },
        {  1.f,  1.f,  0.f,  0.f }, {  1.f,  1.f,  0.f,  0.f }
      };
      static const float Y[5][4] = {
        {  0.f,    0.f,  0.f,  0.f   },                       // leader stays centred
        { -0.25f, -1.f, -1.f, -0.25f }, { 0.25f, 1.f, 1.f, 0.25f },
        { -0.25f, -1.f, -1.f, -0.25f }, { 0.25f, 1.f, 1.f, 0.25f }
      };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      int seg, nxt; float u;
      if (t < 0.75f) { seg = (int)(t / 0.25f); u = (t - seg * 0.25f) / 0.25f; nxt = seg + 1; } // move
      else           { seg = 3; nxt = 3; u = 0.f; }                                            // hold, then jump-loop

      const float cs = radius * 0.7f, sp = radius * 0.6f, th = thickness;
      const ImColor c = color_alpha(color, 1.f);
      for (int d = 0; d < 5; d++) {
        const float x = (X[d][seg] + (X[d][nxt] - X[d][seg]) * u) * cs;
        const float y = (Y[d][seg] + (Y[d][nxt] - Y[d][seg]) * u) * sp;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + y), th, c, num_segments);
      }
    }

    // Rolling dots:
    //   a repeating row of dots mirror-flips each half-cycle (scaleX) while a clipped
    //   overlay (right half) rotates, so dots carousel through the row. Tile = 75%
    //   of width, dot at 1/6 of tile; overlay uses translateX(-37.5%) + rotate (0..1
    //   turn over the first 80% of its 0.5s cycle).
    inline void SpinnerDotsRolling(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f;
      const float tw = W * 0.75f;
      const float t0 = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float t1 = ImFmod((float)ImGui::GetTime() * speed * 2.f, 1.f);
      const float flip = (t0 < 0.5f) ? 1.f : -1.f;
      const float rot = (t1 <= 0.8f) ? (t1 / 0.8f) * PI_2 : PI_2;
      const float tx = -0.375f * W;
      const float cosr = ImCos(rot), sinr = ImSin(rot);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      ImVec2 dots[8];
      int ndots = 0;
      auto try_add = [&](float lx, float ly) {
        if (ImAbs(lx) > radius + th || ImAbs(ly) > radius) return;
        for (int i = 0; i < ndots; i++) {
          const float dx = dots[i].x - lx, dy = dots[i].y - ly;
          if (dx * dx + dy * dy < th * th * 0.25f) return;
        }
        if (ndots < (int)(sizeof(dots) / sizeof(dots[0]))) dots[ndots++] = ImVec2(lx, ly);
      };

      for (int n = -3; n <= 3; n++) {
        const float lx = -radius + n * tw + tw / 6.f;
        try_add(lx * flip, 0.f);
      }
      for (int n = -3; n <= 3; n++) {
        const float lx = -radius + n * tw + tw / 6.f;
        if (lx < 0.f) continue;                                         // clip-path: inset(0 0 0 50%)
        try_add(lx * cosr + tx, lx * sinr);                             // rotate(θ) then translateX
      }

      for (int i = 0; i < ndots; i++)
        window->DrawList->AddCircleFilled(ImVec2(centre.x + dots[i].x, centre.y + dots[i].y), th, c, num_segments);
    }

    inline void SpinnerDotsTriangle(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float hy = radius / 1.154f;                              // aspect-ratio 1.154 (width / height)
      const ImVec2 TOP(0.f, -hy), BL(-radius, hy), BR(radius, hy);
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float u = (t < 0.5f) ? t * 2.f : 1.f;                    // 0..1 over first half, then hold
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;
      auto lerp = [](const ImVec2 &a, const ImVec2 &b, float k) {
        return ImVec2(a.x + (b.x - a.x) * k, a.y + (b.y - a.y) * k);
      };
      auto draw = [&](const ImVec2 &a, const ImVec2 &b) {
        const ImVec2 p = lerp(a, b, u);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + p.x, centre.y + p.y), th, c, num_segments);
      };
      draw(TOP, BR);                                                 // dot 1: top -> bottom-right
      draw(BL, TOP);                                                 // dot 2: bottom-left -> top
      draw(BR, BL);                                                  // dot 3: bottom-right -> bottom-left
    }

    inline void SpinnerDotsCascade(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      struct KF { float t, x, y; };
      static const KF d1[] = { { 0.f, -1.f, -2.f }, { 0.1667f, -1.f, 0.f }, { 0.6667f, -1.f, 2.f }, { 1.f, -1.f, 2.f } };
      static const KF d2[] = { { 0.f,  0.f, -2.f }, { 0.3333f,  0.f, 0.f }, { 0.8333f,  0.f, 2.f }, { 1.f,  0.f, 2.f } };
      static const KF d3[] = { { 0.f,  1.f, -2.f }, { 0.45f,  1.f, 0.f }, { 0.8333f,  1.f, 0.f }, { 1.f,  1.f, 2.f } };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto eval = [&](const KF *k, int n) {
        int seg = 0;
        for (int i = 0; i < n - 1; i++)
          if (t >= k[i].t) seg = i;
        const float dt = k[seg + 1].t - k[seg].t;
        const float u = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
        return ImVec2((k[seg].x + (k[seg + 1].x - k[seg].x) * u) * radius,
                      (k[seg].y + (k[seg + 1].y - k[seg].y) * u) * radius);
      };

      const ImVec2 ps[] = { eval(d1, 4), eval(d2, 4), eval(d3, 4) };
      for (int i = 0; i < 3; i++)
        window->DrawList->AddCircleFilled(ImVec2(centre.x + ps[i].x, centre.y + ps[i].y), th, c, num_segments);
    }

    inline void SpinnerDotsSwap(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float d = (20.f / 7.5f) * thickness;
      struct KF { float t, x, y; };
      static const KF s1[] = { { 0.f, -1.f, 0.f }, { 0.25f, -1.f, -1.f }, { 0.5f, 0.f, -1.f }, { 0.75f, 1.f, -1.f }, { 1.f, 1.f, 0.f } };
      static const KF s2[] = { { 0.f,  1.f, 0.f }, { 0.25f,  1.f,  1.f }, { 0.5f, 0.f,  1.f }, { 0.75f, -1.f,  1.f }, { 1.f, -1.f, 0.f } };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto eval = [&](const KF *k, int n) {
        int seg = 0;
        for (int i = 0; i < n - 1; i++)
          if (t >= k[i].t) seg = i;
        const float dt = k[seg + 1].t - k[seg].t;
        const float u = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
        return ImVec2((k[seg].x + (k[seg + 1].x - k[seg].x) * u) * d,
                      (k[seg].y + (k[seg + 1].y - k[seg].y) * u) * d);
      };

      window->DrawList->AddCircleFilled(centre, th, c, num_segments);
      const ImVec2 p1 = eval(s1, 5), p2 = eval(s2, 5);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + p1.x, centre.y + p1.y), th, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + p2.x, centre.y + p2.y), th, c, num_segments);
    }

    inline void SpinnerDotsSpread(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float th = thickness;
      const float farr = 60.f / 7.5f, nearr = 19.f / 7.5f;
      struct KF { float t, x; };
      static const KF s1[] = { { 0.f, -farr }, { 0.4f, -nearr }, { 1.f, farr } };
      static const KF s2[] = { { 0.f, -farr }, { 0.3333f, 0.f }, { 0.8333f, farr }, { 1.f, farr } };
      static const KF s3[] = { { 0.f, -farr }, { 0.1667f, nearr }, { 0.6667f, farr }, { 1.f, farr } };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);

      auto eval = [&](const KF *k, int n) {
        int seg = 0;
        for (int i = 0; i < n - 1; i++)
          if (t >= k[i].t) seg = i;
        const float dt = k[seg + 1].t - k[seg].t;
        const float u = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
        return (k[seg].x + (k[seg + 1].x - k[seg].x) * u) * th;
      };

      window->DrawList->AddCircleFilled(centre, th, c, num_segments);
      const float xs[] = { eval(s1, 3), eval(s2, 4), eval(s3, 4) };
      for (int i = 0; i < 3; i++)
        window->DrawList->AddCircleFilled(ImVec2(centre.x + xs[i], centre.y), th, c, num_segments);
    }

    inline void SpinnerDotsTwin(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 0.5f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float pw = radius;                                       // half width (flex:1 each)
      const float tw = pw * 0.75f;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      float txf, rot;
      if (t <= 0.25f)      { txf = -0.25f * (t / 0.25f); rot = 0.f; }
      else if (t <= 0.5f)  { txf = -0.25f; rot = (t - 0.25f) / 0.25f * PI_2; }
      else if (t <= 0.75f) { txf = -0.25f * (1.f - (t - 0.5f) / 0.25f); rot = PI_2; }
      else                 { txf = 0.f; rot = PI_2; }
      const float tx = txf * pw;
      const float cosr = ImCos(rot), sinr = ImSin(rot);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      ImVec2 dots[16];
      int ndots = 0;
      auto try_add = [&](float x, float y) {
        if (ImAbs(x) > radius + th || ImAbs(y) > radius) return;
        for (int i = 0; i < ndots; i++) {
          const float dx = dots[i].x - x, dy = dots[i].y - y;
          if (dx * dx + dy * dy < th * th * 0.25f) return;
        }
        if (ndots < (int)(sizeof(dots) / sizeof(dots[0]))) dots[ndots++] = ImVec2(x, y);
      };

      for (int h = 0; h < 2; h++) {
        const float s = (h == 0) ? 1.f : -1.f;
        const float ox = (h == 0) ? -pw * 0.5f : pw * 0.5f;
        for (int n = -2; n <= 3; n++) {
          const float px = n * tw + tw / 3.f - pw * 0.5f;
          const float rx = px * cosr + tx, ry = px * sinr;
          try_add(ox + s * rx, s * ry);
        }
      }

      for (int i = 0; i < ndots; i++)
        window->DrawList->AddCircleFilled(ImVec2(centre.x + dots[i].x, centre.y + dots[i].y), th, c, num_segments);
    }

    inline void SpinnerDotsHop(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f / 1.5f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      struct KF { float t, x, y; };
      static const KF d1[] = { { 0.f, 0.f, -1.f }, { 0.31f, 0.f, -1.f }, { 0.33f, 0.f, 1.f }, { 0.43f, 0.f, -1.f }, { 1.f, 0.f, -1.f } };
      static const KF d2[] = { { 0.f, 1.f, -1.f }, { 0.64f, 1.f, -1.f }, { 0.66f, 1.f, 1.f }, { 0.79f, 1.f, -1.f }, { 1.f, 1.f, -1.f } };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float hy = radius * 28.f / 50.f;                         // aspect 50×28
      const float gx = (t <= 0.79f) ? 0.f : -(t - 0.79f) / 0.21f * radius * 2.f / 3.f;
      const float xr = radius * (-0.76f + 2.28f * t);              // ::before left 0 → 100%+7px
      const float tb = ImFmod((float)ImGui::GetTime() * speed * 3.f, 1.f);
      const float hop = ImSin(tb * IM_PI) * hy * 0.12f;              // l23-2 bounce (0.5s)
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto eval = [&](const KF *k, int n) {
        int seg = 0;
        for (int i = 0; i < n - 1; i++)
          if (t >= k[i].t) seg = i;
        const float dt = k[seg + 1].t - k[seg].t;
        const float u = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
        return ImVec2((k[seg].x + (k[seg + 1].x - k[seg].x) * u) * radius + gx,
                        (k[seg].y + (k[seg + 1].y - k[seg].y) * u) * hy);
      };

      auto draw = [&](ImVec2 p) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + p.x, centre.y + p.y), th, c, num_segments);
      };

      draw(eval(d1, 5));
      draw(eval(d2, 5));
      draw(ImVec2(xr + gx, -hy - hop));
    }

    // Jiggling dots:
    //   two background dots at 25%/75% wobble outward at 40% and the left nudges at
    //   90%; a centred ::before dot shakes horizontally (cubic-bezier elastic, 1s).
    inline void SpinnerDotsJiggle(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      struct KF { float t, x; };
      static const KF d1[] = { { 0.f, -0.5f }, { 0.24f, -0.5f }, { 0.89f, -0.5f }, { 0.90f, -0.7f }, { 1.f, -0.5f } };
      static const KF d2[] = { { 0.f,  0.5f }, { 0.40f,  0.7f }, { 0.50f,  0.5f }, { 1.f,  0.5f } };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float ty = -radius * 6.f / 44.f;                         // aspect 88×12, top row
      const float end = radius * 0.1f / 44.f;                        // l24-1 translate(0.1px)
      const float shake = end * t + radius * 0.06f * ImSin(t * IM_PI * 10.f) * (1.f - t);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto eval = [&](const KF *k, int n) {
        int seg = 0;
        for (int i = 0; i < n - 1; i++)
          if (t >= k[i].t) seg = i;
        const float dt = k[seg + 1].t - k[seg].t;
        const float u = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
        return (k[seg].x + (k[seg + 1].x - k[seg].x) * u) * radius;
      };

      auto draw = [&](float x) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + ty), th, c, num_segments);
      };

      draw(eval(d1, 5));
      draw(eval(d2, 4));
      draw(shake);
    }

    // Vibrating dots:
    //   static left/right background dots; the centred ::before dot vibrates
    //   horizontally (l25-1, 1s) and vertically (l25-2, 0.5s, inset bounce) with
    //   cubic-bezier(.5,-900,.5,900) elastic easing.
    inline void SpinnerDotsVibrate(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float ty = -radius * 6.f / 25.f;                         // aspect 50×12, top row
      const float lx = -radius * 19.f / 25.f;                          // left dot (12px at left edge)
      const float rx =  radius * 19.f / 25.f;
      const float t  = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float tv = ImFmod((float)ImGui::GetTime() * speed * 2.f, 1.f);
      const float end_x = radius * 0.12f / 25.f;
      const float end_y = -radius * 0.15f / 25.f;
      const float shake_x = end_x * t  + radius * 0.06f * ImSin(t  * IM_PI * 10.f) * (1.f - t);
      const float shake_y = end_y * tv + radius * 0.05f * ImSin(tv * IM_PI * 10.f) * (1.f - tv);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto draw = [&](float x, float y) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + y), th, c, num_segments);
      };

      draw(lx, ty);
      draw(rx, ty);
      draw(shake_x, ty + shake_y);
    }

    // Wiper dots:
    //   static left/right background dots plus two clipped semicircles (::before/
    //   ::after) that swing ±90° from side pivots (scaleY ±1, 0.5s alternate).
    inline void SpinnerDotsWiper(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float rc = radius * 0.25f;                               // circle r = height/2 (aspect 4)
      const float sx = radius * 0.75f;                               // background dots at left/right
      const float phase = ImFmod((float)ImGui::GetTime() * speed, 2.f);
      const float u = (phase < 1.f) ? phase : 2.f - phase;           // alternate ping-pong
      const float rot = (u <= 0.4f) ? 0.f : (u - 0.4f) / 0.6f * IM_PI * 0.5f;
      const float c = ImCos(rot), s = ImSin(rot);
      const ImColor col = color_alpha(color, 1.f);
      const float th = thickness;

      auto draw = [&](float x, float y) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + y), th, col, num_segments);
      };

      draw(-sx, 0.f);
      draw( sx, 0.f);
      draw(-rc + rc * c, -rc * s);                                  // ::before pivot (−100%, 50%)
      draw( rc - rc * c,  rc * s);                                  // ::after pivot (200%, 50%), scaleY(−1)
    }

    // Collapsing dots:
    //   a centre dot with an expanding spread ring; four box-shadow dots collapse
    //   inward one-by-one (right→left→bottom→top), then snap back (2s CSS cycle).
    inline void SpinnerDotsCollapse(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 0.5f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float u = thickness / 6.f;                               // CSS dot radius 6px
      struct KF { float t, v; };
      static const KF spread[] = { { 0.f, 0.f }, { 0.2f, 4.f }, { 0.4f, 8.f }, { 0.6f, 12.f }, { 0.8f, 16.f }, { 0.9f, 0.f }, { 1.f, 0.f } };
      static const KF rx[]     = { { 0.f, 40.f }, { 0.1f, 12.f }, { 0.2f, 0.f }, { 0.9f, 40.f }, { 1.f, 40.f } };
      static const KF lx[]     = { { 0.f, -40.f }, { 0.3f, -12.f }, { 0.4f, 0.f }, { 0.9f, -40.f }, { 1.f, -40.f } };
      static const KF by[]     = { { 0.f, 40.f }, { 0.5f, 12.f }, { 0.6f, 0.f }, { 0.9f, 40.f }, { 1.f, 40.f } };
      static const KF ty[]     = { { 0.f, -40.f }, { 0.7f, -12.f }, { 0.8f, 0.f }, { 0.9f, -40.f }, { 1.f, -40.f } };

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto eval = [&](const KF *k, int n) {
        int seg = 0;
        for (int i = 0; i < n - 1; i++)
          if (t >= k[i].t) seg = i;
        const float dt = k[seg + 1].t - k[seg].t;
        const float f = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
        return (k[seg].v + (k[seg + 1].v - k[seg].v) * f) * u;
      };

      const float sp = eval(spread, 7);
      auto draw = [&](float x, float y) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + y), th, c, num_segments);
      };

      window->DrawList->AddCircleFilled(centre, th + sp, c, num_segments); // 0 0 0 spread ring
      draw(eval(rx, 5), 0.f);
      draw(eval(lx, 5), 0.f);
      draw(0.f, eval(by, 5));
      draw(0.f, eval(ty, 5));
    }

    // Flipping dots:
    //   two columns (::before/::after) of top+bottom dots rotate ±180° about the
    //   bottom pivot while the loader bounces vertically (translateY, 1s).
    inline void SpinnerDotsFlip(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float s   = thickness * 2.f;                             // --s dot size
      const float col = s * 0.665f;                                  // column x (space-between)
      const float sep = s * 1.33f;                                   // top↔bottom spacing
      const float py  = s * 0.665f;                                  // bottom pivot y
      const float t   = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float gy  = t * (s - 2.33f * s);                         // l29-0 translateY(s − 100%)
      const float a   = IM_PI * t;
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto draw = [&](float x, float y) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + y), th, c, num_segments);
      };

      auto column = [&](float cx, float sign) {
        const float pyw = py + gy;
        draw(cx, pyw);                                               // bottom pivot (fixed)
        draw(cx - sign * sep * ImSin(a), pyw - sep * ImCos(a));       // top orbits ±180°
      };

      column(-col,  1.f);                                            // ::before rotate(−180°)
      column( col, -1.f);                                            // ::after  rotate(+180°)
    }

    // Turning dots:
    //   two columns of top+bottom dots; ::before pivots at the bottom, ::after at
    //   the top; both rotate −270° during the last 30% of the 1s cycle.
    inline void SpinnerDotsTurn(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float s   = thickness * 2.f;
      const float col = s * 0.665f;
      const float sep = s * 1.33f;
      const float py  = s * 0.665f;
      const float t   = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float a   = (t <= 0.7f) ? 0.f : (t - 0.7f) / 0.3f * (-IM_PI * 1.5f);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;

      auto draw = [&](float x, float y) {
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y + y), th, c, num_segments);
      };

      draw(-col,  py);                                               // ::before pivot (bottom)
      draw(-col + sep * ImSin(a), py - sep * ImCos(a));
      draw( col, -py);                                               // ::after pivot (top)
      draw( col + sep * ImSin(a), -py + sep * ImCos(a));
    }

    // Carousel dots, a port of the CSS dots/l31:
    //   two overlaid dot-pairs (::before/::after); the loader slides 37.5% while
    //   ::after uses rotate(−½turn) translate(37.5%) rotate(½turn) to swap dots.
    inline void SpinnerDotsCarousel(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W = radius * 2.f;
      const float tx = 0.375f * W;
      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float th_rot = IM_PI * t;
      const float cs = ImCos(th_rot), sn = ImSin(th_rot);
      const float loader_tx = tx * t;
      const float lx = -0.75f * radius, rx = 0.75f * radius;
      const ImColor col = color_alpha(color, 1.f);
      const float th = thickness;

      ImVec2 dots[4];
      int ndots = 0;
      auto try_add = [&](float x, float y) {
        for (int i = 0; i < ndots; i++) {
          const float dx = dots[i].x - x, dy = dots[i].y - y;
          if (dx * dx + dy * dy < th * th * 0.25f) return;
        }
        if (ndots < 4) dots[ndots++] = ImVec2(x, y);
      };

      auto after_xform = [&](float x, float y) {
        const float x1 = x * cs - y * sn;
        const float y1 = x * sn + y * cs;
        const float x2 = x1 + tx;
        const float y2 = y1;
        return ImVec2(x2 * cs + y2 * sn, -x2 * sn + y2 * cs);       // rotate(−θ)
      };

      try_add(lx + loader_tx, 0.f);
      try_add(rx + loader_tx, 0.f);
      const ImVec2 p1 = after_xform(lx, 0.f), p2 = after_xform(rx, 0.f);
      try_add(p1.x + loader_tx, p1.y);
      try_add(p2.x + loader_tx, p2.y);

      for (int i = 0; i < ndots; i++)
        window->DrawList->AddCircleFilled(ImVec2(centre.x + dots[i].x, centre.y + dots[i].y), th, col, num_segments);
    }

    // Half-turn dots, a port of the CSS dots/l32:
    //   two overlaid dot-pairs (::before at left/right, ::after at ⅓ and ⅔);
    //   both hold still until 80%, then rotate ±180° (reverse on ::after).
    inline void SpinnerDotsHalfTurn(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float t = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float a_fwd = (t <= 0.8f) ? 0.f : (t - 0.8f) / 0.2f * IM_PI;
      const float a_rev = (t <= 0.8f) ? IM_PI : IM_PI - (t - 0.8f) / 0.2f * IM_PI;
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;
      const float cs_f = ImCos(a_fwd), sn_f = ImSin(a_fwd);
      const float cs_r = ImCos(a_rev), sn_r = ImSin(a_rev);

      const float x_lr[2] = { -0.8f * radius, 0.8f * radius };       // ::before, background-size 20%
      const float x_mid[2] = { -radius / 3.f, radius / 3.f };        // ::after, at ⅓ and ⅔

      auto draw_rot = [&](float x, float y, float cs, float sn) {
        const float rx = x * cs - y * sn, ry = x * sn + y * cs;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + rx, centre.y + ry), th, c, num_segments);
      };

      for (int i = 0; i < 2; i++) {
        draw_rot(x_lr[i], 0.f, cs_f, sn_f);
        draw_rot(x_mid[i], 0.f, cs_r, sn_r);
      }
    }

    // Slide-flip dots, a port of the CSS dots/l33:
    //   two flex halves (::before/::after), each with left/right dots; both slide
    //   (100% − 3/7) while rotating ±½turn about the inner dot pivot.
    inline void SpinnerDotsSlideFlip(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W  = radius * 2.f;
      const float Wp = W * (140.f / 300.f);                          // width calc(140%/3)
      const float t  = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float th = thickness;
      const float ox_b = Wp * (3.f / 14.f);                          // ::before pivot (left dot)
      const float ox_a = Wp * (11.f / 14.f);                         // ::after pivot (right dot)
      const float tx   = Wp * (4.f / 7.f) * t;                       // 100% − calc(300%/7)
      const float ang  = IM_PI * t;
      const float dot_l = Wp * (3.f / 14.f);
      const float dot_r = Wp * (11.f / 14.f);

      auto xform = [](float x, float ox, float tdx, float a) {
        const float cs = ImCos(a), sn = ImSin(a);
        return ImVec2(ox + (x - ox) * cs + tdx, (x - ox) * sn);
      };

      const float base_b = -radius;
      const float base_a = radius - Wp;
      const ImVec2 pts[4] = {
        xform(dot_l, ox_b,  tx,  ang),
        xform(dot_r, ox_b,  tx,  ang),
        xform(dot_l, ox_a, -tx, -ang),
        xform(dot_r, ox_a, -tx, -ang),
      };
      const float bases[4] = { base_b, base_b, base_a, base_a };

      for (int i = 0; i < 4; i++)
        window->DrawList->AddCircleFilled(
          ImVec2(centre.x + bases[i] + pts[i].x, centre.y + pts[i].y), th, c, num_segments);
    }

    // Stagger-turn dots:
    //   two overlaid dot-pairs (left/right, 25%); ::after is offset translate(37.5%)
    //   with a 0.5 s delay; each layer rotates 180° during the first half of its cycle.
    inline void SpinnerDotsStaggerTurn(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W   = radius * 2.f;
      const float td  = 0.375f * W;                                  // ::after --d: 37.5%
      const float t   = (float)ImGui::GetTime() * speed;
      const ImColor c = color_alpha(color, 1.f);
      const float th  = thickness;
      const float lx[2] = { -0.75f * radius, 0.75f * radius };       // background-size 25%

      auto half_turn = [](float phase) {
        const float u = ImFmod(phase, 1.f);
        return (u < 0.5f) ? u * 2.f * IM_PI : IM_PI;
      };

      const float ang_b = half_turn(t);
      const float ang_a = half_turn(t + 0.5f);                       // animation-delay: .5s
      const float cs_b = ImCos(ang_b), sn_b = ImSin(ang_b);
      const float cs_a = ImCos(ang_a), sn_a = ImSin(ang_a);

      ImVec2 dots[4];
      int ndots = 0;
      auto try_add = [&](float x, float y) {
        for (int i = 0; i < ndots; i++) {
          const float dx = dots[i].x - x, dy = dots[i].y - y;
          if (dx * dx + dy * dy < th * th * 0.25f) return;
        }
        if (ndots < 4) dots[ndots++] = ImVec2(x, y);
      };

      for (int i = 0; i < 2; i++) {
        try_add(lx[i] * cs_b, lx[i] * sn_b);                         // ::before
        try_add(lx[i] * cs_a + td, lx[i] * sn_a);                    // ::after
      }

      for (int i = 0; i < ndots; i++)
        window->DrawList->AddCircleFilled(ImVec2(centre.x + dots[i].x, centre.y + dots[i].y), th, c, num_segments);
    }

    // Stretch dots:
    //   two overlaid bars (::before left, ::after right); each grows via aspect-ratio
    //   2.3→3.7 while translateY(±8px) bounces in opposite directions.
    inline void SpinnerDotsStretch(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float Hc    = radius * 2.f / 5.f;                        // loader height (aspect-ratio 5)
      const float tyMax = (8.f / 15.f) * Hc;                         // translateY 8px
      const float t     = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c   = color_alpha(color, 1.f);
      const float th    = thickness;

      float ty, ar;
      if (t <= 0.33f) {
        ty = tyMax * (t / 0.33f);
        ar = 2.3f;
      } else if (t <= 0.66f) {
        ty = tyMax;
        ar = 2.3f + (3.7f - 2.3f) * ((t - 0.33f) / 0.33f);
      } else {
        ty = tyMax * (1.f - (t - 0.66f) / 0.34f);
        ar = 3.7f;
      }

      const float pw   = Hc * ar;
      const float half = Hc * 0.5f;                                  // background-size 15px (full height)

      auto draw_layer = [&](float base_x, float sign) {
        const float y = sign * ty;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + base_x + half, centre.y + y), th, c, num_segments);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + base_x + pw - half, centre.y + y), th, c, num_segments);
      };

      draw_layer(-radius,  1.f);                                     // ::before, --s: 1
      draw_layer( radius - pw, -1.f);                               // ::after, margin-left: auto, --s: −1
    }

    // Mirror-stretch dots, a port of the CSS dots/l36:
    //   like l35 but l36-1 adds translateX(19px); the loader flips scaleY every 2 s.
    inline void SpinnerDotsMirrorStretch(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float Hc    = radius * 2.f / 5.f;
      const float tyMax = (8.f / 15.f) * Hc;
      const float txMax = (19.f / 15.f) * Hc;
      const float time  = (float)ImGui::GetTime() * speed;
      const float t     = ImFmod(time, 1.f);
      const float flipY = (ImFmod(time, 2.f) >= 1.f) ? -1.f : 1.f;
      const ImColor c   = color_alpha(color, 1.f);
      const float th    = thickness;

      float tx, ty, ar;
      if (t <= 0.33f) {
        tx = 0.f;
        ty = tyMax * (t / 0.33f);
        ar = 2.3f;
      } else if (t <= 0.66f) {
        tx = txMax * ((t - 0.33f) / 0.33f);
        ty = tyMax;
        ar = 2.3f + (3.7f - 2.3f) * ((t - 0.33f) / 0.33f);
      } else {
        tx = txMax;
        ty = tyMax * (1.f - (t - 0.66f) / 0.34f);
        ar = 3.7f;
      }

      const float pw   = Hc * ar;
      const float half = Hc * 0.5f;

      auto draw_layer = [&](float base_x, float sign) {
        const float dx = sign * tx;
        const float y  = flipY * sign * ty;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + base_x + half + dx, centre.y + y), th, c, num_segments);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + base_x + pw - half + dx, centre.y + y), th, c, num_segments);
      };

      draw_layer(-radius,       1.f);
      draw_layer(radius - pw, -1.f);
    }

    // Pinch dots, a port of the CSS dots/l37:
    //   centre dot + 4 box-shadow satellites; l37-1 (0.75 s alternate) pinches
    //   ±30/±10 → ±14/±12 with spread change; l37-2 (1.5 s) rotates 180°.
    inline void SpinnerDotsPinch(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float time = (float)ImGui::GetTime() * speed;
      const float u    = thickness / 3.f;                            // CSS centre radius 3 px (6 px wide)
      const ImColor c  = color_alpha(color, 1.f);

      const float p1 = ImFmod(time / 0.75f, 2.f);
      const float u1 = (p1 <= 1.f) ? p1 : (2.f - p1);                // l37-1 alternate ping-pong
      float pinch;
      if (u1 <= 0.2f)       pinch = 0.f;
      else if (u1 <= 0.6f)  pinch = (u1 - 0.2f) / 0.4f;
      else                  pinch = 1.f;

      const float p2 = ImFmod(time / 1.5f, 1.f);
      float ang;
      if (p2 <= 0.25f)      ang = 0.f;
      else if (p2 <= 0.5f)  ang = (p2 - 0.25f) / 0.25f * IM_PI;
      else                  ang = IM_PI;

      const float cs = ImCos(ang), sn = ImSin(ang);
      const float ax[4] = { -30.f * u, -10.f * u,  10.f * u,  30.f * u };
      const float bx[4] = { -14.f * u, -12.f * u,  12.f * u,  14.f * u };
      const float ar[4] = { thickness * 2.f, thickness * 2.f, thickness * 2.f, thickness * 2.f };
      const float br[4] = { thickness * 3.f, thickness * 2.f, thickness * 2.f, thickness * 3.f };

      window->DrawList->AddCircleFilled(centre, thickness, c, num_segments);

      for (int i = 0; i < 4; i++) {
        const float x = ax[i] + (bx[i] - ax[i]) * pinch;
        const float r = ar[i] + (br[i] - ar[i]) * pinch;
        const float rx = x * cs, ry = x * sn;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + rx, centre.y + ry), r, c, num_segments);
      }
    }

    // Corner-run dots:
    //   four corner dots (background-size 40%); each slides clockwise to the next
    //   corner every 0.5 s (TL→TR→BR→BL→…).
    inline void SpinnerDotsCorners(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 2.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float t  = ImFmod((float)ImGui::GetTime() * speed, 1.f); // speed 2 → 0.5 s cycle
      const float c  = radius * 0.6f;                                // dot inset (40% tile / 2)
      const ImColor col = color_alpha(color, 1.f);
      const ImVec2 p0(-c, -c), p1(c, -c), p2(c, c), p3(-c, c);
      const ImVec2 a[4] = { p0, p1, p2, p3 };
      const ImVec2 b[4] = { p1, p2, p3, p0 };

      for (int i = 0; i < 4; i++) {
        const ImVec2 p(a[i].x + (b[i].x - a[i].x) * t, a[i].y + (b[i].y - a[i].y) * t);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + p.x, centre.y + p.y), thickness, col, num_segments);
      }
    }

    // Nudge-rotate dots:
    //   two halves (l33 geometry); l39-1 nudges ±translate(400%/14) during 30–70%;
    //   l39-0 rotates the loader 180° between 30% and 50%.
    inline void SpinnerDotsNudgeRotate(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W   = radius * 2.f;
      const float Wp  = W * (140.f / 300.f);
      const float t   = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float th  = thickness;
      const float dot_l = Wp * (3.f / 14.f);
      const float dot_r = Wp * (11.f / 14.f);
      const float nudge = Wp * (4.f / 14.f);

      float nk;
      if (t < 0.3f)       nk = t / 0.3f;
      else if (t <= 0.7f) nk = 1.f;
      else                nk = (1.f - t) / 0.3f;

      float ang;
      if (t <= 0.3f)      ang = 0.f;
      else if (t <= 0.5f) ang = (t - 0.3f) / 0.2f * IM_PI;
      else                ang = IM_PI;

      const float cs = ImCos(ang), sn = ImSin(ang);
      const float tx_b = nudge * nk;
      const float tx_a = -nudge * nk;
      const float xs[4] = {
        -radius + dot_l + tx_b,
        -radius + dot_r + tx_b,
        radius - Wp + dot_l + tx_a,
        radius - Wp + dot_r + tx_a,
      };

      for (int i = 0; i < 4; i++)
        window->DrawList->AddCircleFilled(
          ImVec2(centre.x + xs[i] * cs, centre.y + xs[i] * sn), th, c, num_segments);
    }

    // Unfold dots:
    //   four dots (20% each) stack left then spread to 0, ⅓, ⅔, 100% (l40-1, 0.75 s
    //   alternate); l40-2 mirror-flips scale(±1) every 1.5 s alternate.
    inline void SpinnerDotsUnfold(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float time = (float)ImGui::GetTime() * speed;
      const ImColor c  = color_alpha(color, 1.f);

      const float p1 = ImFmod(time / 0.75f, 2.f);
      const float u1 = (p1 <= 1.f) ? p1 : (2.f - p1);
      const float p2 = ImFmod(time / 1.5f, 2.f);
      const float u2 = (p2 <= 1.f) ? p2 : (2.f - p2);
      const float flip = (u2 < 0.5f) ? 1.f : -1.f;

      static const float slot_x[4] = { -0.8f, -1.f / 3.f, 1.f / 3.f, 0.8f };

      auto slot_at = [&](int dot, float u) -> float {
        if (dot == 0) return 0.f;
        if (u <= 0.1f) return 0.f;
        if (dot == 1) {
          if (u <= 0.33f) return (u - 0.1f) / 0.23f;
          return 1.f;
        }
        if (dot == 2) {
          if (u <= 0.33f) return (u - 0.1f) / 0.23f;
          if (u <= 0.66f) return 1.f + (u - 0.33f) / 0.33f;
          return 2.f;
        }
        if (u <= 0.33f) return (u - 0.1f) / 0.23f;
        if (u <= 0.66f) return 1.f + (u - 0.33f) / 0.33f;
        if (u <= 0.9f)  return 2.f + (u - 0.66f) / 0.24f;
        return 3.f;
      };

      for (int d = 0; d < 4; d++) {
        const float s  = slot_at(d, u1);
        const int   i0 = (int)s;
        const int   i1 = ImMin(i0 + 1, 3);
        const float f  = s - (float)i0;
        const float x  = flip * radius * (slot_x[i0] + (slot_x[i1] - slot_x[i0]) * f);
        window->DrawList->AddCircleFilled(ImVec2(centre.x + x, centre.y), thickness, c, num_segments);
      }
    }

    // Shuttle dots:
    //   two static middle dots (⅓, ⅔); ::before/::after circles run a rectangular
    //   path (down → across → up) in mirror on the left/right ends.
    inline void SpinnerDotsShuttle(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W   = radius * 2.f;
      const float Hc  = W / 5.f;
      const float r   = Hc * 0.5f;
      const float t   = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float bx_l = -radius + r;
      const float bx_r =  radius - r;

      float tx, ty;
      if (t <= 0.33f) {
        tx = 0.f;
        ty = 1.3f * Hc * (t / 0.33f);
      } else if (t <= 0.66f) {
        tx = 4.f * Hc * ((t - 0.33f) / 0.33f);
        ty = 1.3f * Hc;
      } else {
        tx = 4.f * Hc;
        ty = 1.3f * Hc * (1.f - (t - 0.66f) / 0.34f);
      }

      const float mid_x[2] = { -radius + W / 3.f, -radius + 2.f * W / 3.f };
      const ImVec2 movers[2] = {
        ImVec2(bx_l + tx, ty),
        ImVec2(bx_r - tx, ty),                                        // ::after scale(−1,−1) mirror
      };

      for (int i = 0; i < 2; i++)
        window->DrawList->AddCircleFilled(
          ImVec2(centre.x + mid_x[i], centre.y), thickness, c, num_segments);

      for (int i = 0; i < 2; i++)
        window->DrawList->AddCircleFilled(
          ImVec2(centre.x + movers[i].x, centre.y + movers[i].y), thickness, c, num_segments);
    }

    // Spread-shuttle dots, a port of the CSS dots/l42:
    //   like l41 but end dots travel translate(400%/3); middle dots spread from
    //   ⅓/⅔ to the edges during the last 40% (l42-0).
    inline void SpinnerDotsSpreadShuttle(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float W   = radius * 2.f;
      const float Hc  = W / 5.f;
      const float r   = Hc * 0.5f;
      const float t   = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float bx_l = -radius + r;
      const float bx_r =  radius - r;
      const float txMax = (4.f / 3.f) * Hc;                          // calc(400%/3)

      float tx, ty;
      if (t <= 0.33f) {
        tx = 0.f;
        ty = 1.3f * Hc * (t / 0.33f);
      } else if (t <= 0.66f) {
        tx = txMax * ((t - 0.33f) / 0.33f);
        ty = 1.3f * Hc;
      } else {
        tx = txMax;
        ty = 1.3f * Hc * (1.f - (t - 0.66f) / 0.34f);
      }

      const float mid_in[2]  = { -radius + W / 3.f, -radius + 2.f * W / 3.f };
      const float mid_out[2] = { -0.8f * radius,     0.8f * radius };
      const float spread = (t <= 0.6f) ? 0.f : (t - 0.6f) / 0.4f;
      const float mid_x[2] = {
        mid_in[0] + (mid_out[0] - mid_in[0]) * spread,
        mid_in[1] + (mid_out[1] - mid_in[1]) * spread,
      };

      const ImVec2 movers[2] = {
        ImVec2(bx_l + tx, ty),
        ImVec2(bx_r - tx, ty),
      };

      for (int i = 0; i < 2; i++)
        window->DrawList->AddCircleFilled(
          ImVec2(centre.x + mid_x[i], centre.y), thickness, c, num_segments);

      for (int i = 0; i < 2; i++)
        window->DrawList->AddCircleFilled(
          ImVec2(centre.x + movers[i].x, centre.y + movers[i].y), thickness, c, num_segments);
    }

    // Triad dots:
    //   two clusters of three (middle / top / bottom); dots spread vertically then
    //   swap columns (0↔⅓ and ⅔↔1). Loader aspect-ratio 2.5, --s/8 rest offset.
    inline void SpinnerDotsTriad(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      struct KF { float t, x, y; };
      static const KF lm[] = { { 0.f, -1.f, 0.f }, { 0.33f, -1.f, 0.f }, { 0.66f, -1.f / 3.f, 0.f }, { 1.f, -1.f / 3.f, 0.f } };
      static const KF lt[] = { { 0.f, -1.f / 3.f, 0.1f }, { 0.33f, -1.f / 3.f, 0.4f }, { 0.66f, -1.f, 0.4f }, { 1.f, -1.f, 0.1f } };
      static const KF lb[] = { { 0.f, -1.f / 3.f, -0.1f }, { 0.33f, -1.f / 3.f, -0.4f }, { 0.66f, -1.f, -0.4f }, { 1.f, -1.f, -0.1f } };
      static const KF rm[] = { { 0.f, 1.f, 0.f }, { 0.33f, 1.f, 0.f }, { 0.66f, 1.f / 3.f, 0.f }, { 1.f, 1.f / 3.f, 0.f } };
      static const KF rt[] = { { 0.f, 1.f / 3.f, 0.1f }, { 0.33f, 1.f / 3.f, 0.4f }, { 0.66f, 1.f, 0.4f }, { 1.f, 1.f, 0.1f } };
      static const KF rb[] = { { 0.f, 1.f / 3.f, -0.1f }, { 0.33f, 1.f / 3.f, -0.4f }, { 0.66f, 1.f, -0.4f }, { 1.f, 1.f, -0.1f } };

      const float t  = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);

      auto eval = [&](const KF *k, int n) {
        int seg = 0;
        for (int i = 0; i < n - 1; i++)
          if (t >= k[i].t) seg = i;
        const float dt = k[seg + 1].t - k[seg].t;
        const float u  = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
        return ImVec2((k[seg].x + (k[seg + 1].x - k[seg].x) * u) * radius,
                      (k[seg].y + (k[seg + 1].y - k[seg].y) * u) * radius);
      };

      const KF *tracks[] = { lm, lt, lb, rm, rt, rb };
      const float th_mid = thickness * 1.25f;                          // background-size 20%×50%
      for (int i = 0; i < 6; i++) {
        const ImVec2 p = eval(tracks[i], 4);
        const float th = (i == 0 || i == 3) ? th_mid : thickness;
        window->DrawList->AddCircleFilled(ImVec2(centre.x + p.x, centre.y + p.y), th, c, num_segments);
      }
    }

    // Satellite dots:
    //   static left/right dots; two centre circles orbit pivots at ±75% width,
    //   rotating 360° during 58–100% (::after −1 turn, delay −0.5 s).
    inline void SpinnerDotsSatellite(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      const float orbit = 0.75f * radius;                            // 25% tile, pivot at ±75%
      const float time  = (float)ImGui::GetTime() * speed;
      const ImColor c   = color_alpha(color, 1.f);

      auto spin = [](float phase) {
        if (phase <= 0.58f) return 0.f;
        return (phase - 0.58f) / 0.42f * IM_PI * 2.f;
      };

      const float ang_b =  spin(ImFmod(time, 1.f));
      const float ang_a = -spin(ImFmod(time + 0.5f, 1.f));           // --s:−1, delay −0.5 s

      const ImVec2 pivot_l(-orbit, 0.f), pivot_r(orbit, 0.f);
      auto rotated = [](ImVec2 v, float a) {
        return ImVec2(v.x * ImCos(a) - v.y * ImSin(a), v.x * ImSin(a) + v.y * ImCos(a));
      };

      const ImVec2 vec_l(orbit, 0.f), vec_r(-orbit, 0.f);
      const ImVec2 r_l = rotated(vec_l, ang_b), r_a = rotated(vec_r, ang_a);
      const ImVec2 sat_l(pivot_l.x + r_l.x, pivot_l.y + r_l.y);
      const ImVec2 sat_r(pivot_r.x + r_a.x, pivot_r.y + r_a.y);

      window->DrawList->AddCircleFilled(ImVec2(centre.x + pivot_l.x, centre.y), thickness, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + pivot_r.x, centre.y), thickness, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + sat_l.x, centre.y + sat_l.y), thickness, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + sat_r.x, centre.y + sat_r.y), thickness, c, num_segments);
    }

    // Track dots:
    //   static left/right dots; ::before (background inherit) carries two dots through
    //   translate(±37.5%) + rotate(0→360°) keyframes over 2 s.
    inline void SpinnerDotsTrack(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 0.5f)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);

      struct KF { float t; float tx; float deg; };
      static const KF k[] = {
        { 0.f,      0.375f,   0.f },
        { 0.1667f,  0.375f,  90.f },
        { 0.3333f, -0.375f,  90.f },
        { 0.5f,    -0.375f, 180.f },
        { 0.6667f, -0.375f, 270.f },
        { 0.8333f,  0.375f, 270.f },
        { 1.f,      0.375f, 360.f },
      };

      const float W   = radius * 2.f;
      const float t   = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const ImColor c = color_alpha(color, 1.f);
      const float edge = 0.75f * radius;

      int seg = 0;
      for (int i = 0; i < 6; i++)
        if (t >= k[i].t) seg = i;
      const float dt = k[seg + 1].t - k[seg].t;
      const float u  = dt > 0.f ? (t - k[seg].t) / dt : 0.f;
      const float txf = k[seg].tx + (k[seg + 1].tx - k[seg].tx) * u;
      const float ang = (k[seg].deg + (k[seg + 1].deg - k[seg].deg) * u) * (IM_PI / 180.f);
      const float cs = ImCos(ang), sn = ImSin(ang);
      const float tdx = txf * W;

      auto xform = [&](float lx) {
        return ImVec2(lx * cs + tdx, lx * sn);
      };

      window->DrawList->AddCircleFilled(ImVec2(centre.x - edge, centre.y), thickness, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + edge, centre.y), thickness, c, num_segments);

      const ImVec2 p0 = xform(-edge), p1 = xform(edge);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + p0.x, centre.y + p0.y), thickness, c, num_segments);
      window->DrawList->AddCircleFilled(ImVec2(centre.x + p1.x, centre.y + p1.y), thickness, c, num_segments);
    }
}

#endif // _IMSPINNER_DOTS_H_
