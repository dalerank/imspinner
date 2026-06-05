#ifndef _IMSPINNER_SHAPES_H_
#define _IMSPINNER_SHAPES_H_

/*
 * imspinner shapes add-on
 *
 * Shape-based / CSS-port spinners kept out of imspinner.h. Include this header
 * (it pulls in "imspinner.h"). All spinners here use only the primitives from
 * imspinner.h (SPINNER_HEADER, color_alpha, detail::PathStroke, draw-list), so
 * the file is self-contained.
 */

#ifndef _IMSPINNER_SHAPES_INTERNAL_
#include "imspinner.h"
namespace ImSpinner
{
#endif

    // Morph shape:
    //   a single blob morphs circle -> square -> triangle and back over a 2 sec
    //   cycle, changing colour at each step (red -> purple -> orange). 
    inline void SpinnerMorphShape(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const float l = centre.x - radius, t = centre.y - radius, S = radius * 2.f;
      const float rgt = l + S, bot = t + S;

      // strong ease-out within each keyframe segment (approx cubic-bezier(.3,1,0,1))
      auto ease = [](float u) { const float k = 1.f - u; return 1.f - k * k * k; };

      const float time = (float)ImGui::GetTime() * speed;
      float p = ImFmod(time, 2.f) / 2.f;        // 0..1 over the 2 sec cycle
      if (mode == 2) p = 1.f - p;

      const ImVec4 red   (240.f / 255.f,  51.f / 255.f,  85.f / 255.f, 1.f);  // #f03355
      const ImVec4 purple( 81.f / 255.f,  75.f / 255.f, 130.f / 255.f, 1.f);  // #514b82
      const ImVec4 orange(255.f / 255.f, 165.f / 255.f,  22.f / 255.f, 1.f);  // #ffa516

      float collapse, round01, u; ImVec4 ca, cb;
      if (p < 1.f / 3.f)      { u = ease(p * 3.f);              collapse = 0.f;      round01 = 1.f - u; ca = red;    cb = purple; }
      else if (p < 2.f / 3.f) { u = ease((p - 1.f / 3.f) * 3.f); collapse = u;       round01 = 0.f;     ca = purple; cb = orange; }
      else                    { u = ease((p - 2.f / 3.f) * 3.f); collapse = 1.f - u; round01 = u;       ca = orange; cb = red;    }

      const ImColor c = color_alpha(ImColor(ImVec4(ca.x + (cb.x - ca.x) * u, ca.y + (cb.y - ca.y) * u,
                                                   ca.z + (cb.z - ca.z) * u, 1.f)), 1.f);
      const float rho = round01 * radius;       // corner radius: 0 = sharp, radius = circle

      // morphing quad corners, clockwise: top-left, top-right, bottom-right, bottom-left.
      // collapse slides the two top corners toward the top centre to form a triangle.
      const ImVec2 P[4] = {
        ImVec2(l + collapse * 0.5f * S, t),
        ImVec2(rgt - collapse * 0.5f * S, t),
        ImVec2(rgt, bot),
        ImVec2(l, bot),
      };

      window->DrawList->PathClear();
      for (int i = 0; i < 4; i++) {
        const ImVec2 Pi = P[i], Pp = P[(i + 3) % 4], Pn = P[(i + 1) % 4];
        ImVec2 v1(Pp.x - Pi.x, Pp.y - Pi.y), v2(Pn.x - Pi.x, Pn.y - Pi.y);
        const float l1 = ImSqrt(v1.x * v1.x + v1.y * v1.y), l2 = ImSqrt(v2.x * v2.x + v2.y * v2.y);
        if (rho < 0.5f || l1 < 1e-3f || l2 < 1e-3f) { window->DrawList->PathLineTo(Pi); continue; }
        v1.x /= l1; v1.y /= l1; v2.x /= l2; v2.y /= l2;

        float cosphi = ImClamp(v1.x * v2.x + v1.y * v2.y, -1.f, 1.f);
        const float half = ImAcos(cosphi) * 0.5f;          // half of the interior angle
        const float sh = ImSin(half), chf = ImCos(half);
        float tdist = (sh > 1e-4f) ? rho * chf / sh : 0.f;  // rho / tan(half)
        const float tmax = ImMin(l1, l2) * 0.5f;
        float rho_eff = rho;
        if (tdist > tmax) { tdist = tmax; rho_eff = (chf > 1e-4f) ? tdist * sh / chf : rho; }

        ImVec2 bis(v1.x + v2.x, v1.y + v2.y);
        const float bl = ImSqrt(bis.x * bis.x + bis.y * bis.y);
        if (bl < 1e-4f || sh < 1e-4f) { window->DrawList->PathLineTo(Pi); continue; }
        bis.x /= bl; bis.y /= bl;

        const ImVec2 C(Pi.x + bis.x * (rho_eff / sh), Pi.y + bis.y * (rho_eff / sh));
        const ImVec2 Tp(Pi.x + v1.x * tdist, Pi.y + v1.y * tdist);
        const ImVec2 Tn(Pi.x + v2.x * tdist, Pi.y + v2.y * tdist);
        const float a0 = ImAtan2(Tp.y - C.y, Tp.x - C.x);
        float d = ImAtan2(Tn.y - C.y, Tn.x - C.x) - a0;
        while (d >  IM_PI) d -= 2.f * IM_PI;                // keep the minor (outward) arc
        while (d < -IM_PI) d += 2.f * IM_PI;
        const int seg = ImMax(2, (int)(num_segments * ImFabs(d) / (2.f * IM_PI)));
        window->DrawList->PathArcTo(C, rho_eff, a0, a0 + d, seg);
      }
      window->DrawList->PathFillConvex(c);
    }

    // Flip triangle:
    //   a triangle made of three of the square's four corners "rolls" around the
    //   box - each quarter of the 2 sec cycle one vertex slides along an edge to
    //   the next corner.
    inline void SpinnerFlipTriangle(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const float l = centre.x - radius, t = centre.y - radius, S = radius * 2.f;
      const ImVec2 corner[4] = { {l, t}, {l + S, t}, {l + S, t + S}, {l, t + S} };  // TL, TR, BR, BL
      static const int tri[5][3] = { {0,1,2}, {0,1,3}, {0,2,3}, {1,2,3}, {1,2,0} };
      const ImColor c = color_alpha(ImColor(37, 176, 155), 1.f);  // #25b09b teal

      auto ease = [](float u) { const float k = 1.f - u; return 1.f - k * k * k; };

      float p = ImFmod((float)ImGui::GetTime() * speed, 2.f) / 2.f;   // 0..1 over the 2 sec cycle
      if (mode == 2) p = 1.f - p;
      int seg = (int)(p * 4.f);
      if (seg > 3) seg = 3;
      const float u = ease(p * 4.f - seg);

      ImVec2 v[3];
      for (int k = 0; k < 3; k++) {
        const ImVec2 a = corner[tri[seg][k]], b = corner[tri[seg + 1][k]];
        v[k] = ImVec2(a.x + (b.x - a.x) * u, a.y + (b.y - a.y) * u);
      }
      window->DrawList->AddTriangleFilled(v[0], v[1], v[2], c);
    }

    // Fold square:
    //   two triangles form a square, then slide apart, rotate and reassemble it
    //   along a different diagonal (the square appears to fold over) on a 1.5 sec
    //   linear loop.
    inline void SpinnerFoldSquare(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const float k = radius / 25.f;                          // CSS reference box is 50px
      const ImColor c = color_alpha(ImColor(81, 75, 130), 1.f); // #514b82

      static const float K[5]  = { 0.f, 0.25f, 0.5f, 0.75f, 1.f };   // keyframe times
      static const float Wv[5] = { 50.f, 50.f, 50.f, 70.70f, 70.70f };
      static const float Hv[5] = { 50.f, 50.f, 50.f, 35.35f, 35.35f };
      static const float Rv[5] = { 45.f, 0.f, 0.f, 0.f, 0.f };       // container rotation (deg)
      static const float TBv[5]= { 0.3f, 0.3f, -5.f, -5.f, 17.7f };  // ::before translateX (px)
      static const float TAv[5]= { -0.3f, -0.3f, 5.f, 5.f, -17.7f }; // ::after  translateX (px)
      auto pwl = [&](const float *V, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < 4; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[4];
      };

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      const float W = pwl(Wv, t), H = pwl(Hv, t), w2 = W * 0.5f;
      const float rot = pwl(Rv, t) * (IM_PI / 180.f);
      const float ca = ImCos(rot), sa = ImSin(rot);
      const float cf = ImClamp((t - 0.5f) / 0.25f, 0.f, 1.f);  // clip-path morph A -> B

      // clip-path polygons (fractions of each half-box), A = rest, B = folded
      static const float bAx[3] = {0,1,1}, bAy[3] = {0.5f,0,1};   // ::before A
      static const float bBx[3] = {0,0,1}, bBy[3] = {1,0,1};      // ::before B
      static const float aAx[3] = {0,1,0}, aAy[3] = {0,0.5f,1};   // ::after  A
      static const float aBx[3] = {0,1,1}, aBy[3] = {0,0,1};      // ::after  B

      auto emit = [&](bool before, float tx) {
        ImVec2 v[3];
        for (int i = 0; i < 3; i++) {
          const float px = before ? (bAx[i] + (bBx[i] - bAx[i]) * cf) : (aAx[i] + (aBx[i] - aAx[i]) * cf);
          const float py = before ? (bAy[i] + (bBy[i] - bAy[i]) * cf) : (aAy[i] + (aBy[i] - aAy[i]) * cf);
          float lx = (before ? -w2 : 0.f) + px * w2 + tx;        // ::before is the left half, ::after the right
          float ly = -H * 0.5f + py * H;
          const float rx = lx * ca - ly * sa, ry = lx * sa + ly * ca;
          v[i] = ImVec2(centre.x + rx * k, centre.y + ry * k);
        }
        window->DrawList->AddTriangleFilled(v[0], v[1], v[2], c);
      };
      emit(true,  pwl(TBv, t));
      emit(false, pwl(TAv, t));
    }

    // Pinwheel:
    //   four 90-degree wedges (apex 20px in from each edge, opening outward).
    //   The box pulses 40 -> 60 -> 40 px: at rest the apexes meet in the centre
    //   (a solid square), expanded they pull apart leaving a square hole (a
    //   pinwheel). The whole figure also spins one full turn per 1.5 sec.
    inline void SpinnerPinwheel(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const float k = radius / 30.f;            // map the expanded 60px box to the radius
      const ImColor c = color_alpha(ImColor(240, 51, 85), 1.f);  // #f03355

      auto ease = [](float u) { const float kk = 1.f - u; return 1.f - kk * kk * kk; };  // ~cubic-bezier(.3,1,0,1)

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      float Sz, rot;                            // box side (px) and rotation (rad)
      if (t < 0.5f) { const float u = ease(t / 0.5f);        Sz = 40.f + 20.f * u; rot = IM_PI * u; }
      else          { const float u = ease((t - 0.5f) / 0.5f); Sz = 60.f - 20.f * u; rot = IM_PI * (1.f + u); }
      const float ca = ImCos(rot), sa = ImSin(rot);
      const float h = Sz * 0.5f;                // half box; edge distance from centre

      static const float dx[4] = { 0, 1, 0, -1 }, dy[4] = { -1, 0, 1, 0 };   // wedge directions
      static const float px[4] = { 1, 0, -1, 0 }, py[4] = { 0, 1, 0, -1 };   // perpendiculars

      for (int i = 0; i < 4; i++) {
        const float verts[3][2] = {
          { dx[i] * (h - 20.f),          dy[i] * (h - 20.f) },           // apex (20px in from the edge)
          { dx[i] * h + px[i] * 20.f,    dy[i] * h + py[i] * 20.f },     // base corner +
          { dx[i] * h - px[i] * 20.f,    dy[i] * h - py[i] * 20.f },     // base corner -
        };
        ImVec2 v[3];
        for (int j = 0; j < 3; j++) {
          const float lx = verts[j][0], ly = verts[j][1];
          v[j] = ImVec2(centre.x + (lx * ca - ly * sa) * k, centre.y + (lx * sa + ly * ca) * k);
        }
        window->DrawList->AddTriangleFilled(v[0], v[1], v[2], c);
      }
    }

    // Corner squares:
    //   four fixed-size squares sit at the box corners. The box grows 40 -> 60 px
    //   (the squares spread apart leaving a "+" gap), then all four slide one
    //   corner clockwise along the edges, then the box shrinks back - on a 1.5 sec
    //   loop.
    inline void SpinnerCornerSquares(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const float k = radius / 30.f;            // map the expanded 60px box to the radius
      const float SQ = 21.f;                    // fixed square side (px)
      const ImColor c = color_alpha(ImColor(255, 165, 0), 1.f);  // orange

      auto ease = [](float u) { const float kk = 1.f - u; return 1.f - kk * kk * kk; };  // ~cubic-bezier(.3,1,0,1)
      const float th = 1.f / 3.f;

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      float Sz, rf;                             // box side (px) and corner-rotation factor
      if (t < th)        { Sz = 40.f + 20.f * ease(t / th);        rf = 0.f; }
      else if (t < 2*th) { Sz = 60.f;                              rf = ease((t - th) / th); }
      else               { Sz = 60.f - 20.f * ease((t - 2*th) / th); rf = 1.f; }

      static const float fx0[4] = {0,1,1,0}, fy0[4] = {0,0,1,1};   // corners at rest: TL,TR,BR,BL
      static const float fx1[4] = {1,1,0,0}, fy1[4] = {0,1,1,0};   // after a clockwise step
      const float span = Sz - SQ, half = Sz * 0.5f;

      for (int i = 0; i < 4; i++) {
        const float fx = fx0[i] + (fx1[i] - fx0[i]) * rf;
        const float fy = fy0[i] + (fy1[i] - fy0[i]) * rf;
        const float lx = fx * span - half, ly = fy * span - half;   // top-left in centred box coords
        window->DrawList->AddRectFilled(ImVec2(centre.x + lx * k, centre.y + ly * k),
                                        ImVec2(centre.x + (lx + SQ) * k, centre.y + (ly + SQ) * k), c);
      }
    }

    // Split square:
    //   two same-colour halves (one is the other point-reflected and time-reversed)
    //   slide vertically and morph their clip shape. In the static phases their
    //   union is a solid square (seamless loop); mid-cycle they pull apart into
    //   interlocking triangles.
    inline void SpinnerSplitSquare(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(81, 75, 130), 1.f);  // #514b82
      const float box = radius;                  // vertical travel reaches +/- radius, so fit to that

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      auto cmf = [](float tt) { return ImClamp((tt - 1.f / 3.f) / (1.f / 3.f), 0.f, 1.f); };  // clip morph A->B
      static const float TK[6] = { 0.f, 0.1f, 1.f / 3.f, 2.f / 3.f, 0.9f, 1.f };              // translateY keyframes
      static const float TV[6] = { 0.f, 0.f, 0.5f, 0.5f, 0.f, 0.f };
      auto tyf = [&](float tt) -> float {
        if (tt <= TK[0]) return TV[0];
        for (int i = 0; i < 5; i++)
          if (tt <= TK[i + 1]) { const float u = (tt - TK[i]) / (TK[i + 1] - TK[i]); return TV[i] + (TV[i + 1] - TV[i]) * u; }
        return TV[5];
      };
      // 5 clip vertices for morph factor cm (only v0 and v2 slide to the top centre)
      auto clipv = [](float cm, float out[5][2]) {
        out[0][0] = 0.5f * cm;       out[0][1] = 0.f;   // v0: (0,0) -> (.5,0)
        out[1][0] = 0.5f;            out[1][1] = 1.f;   // v1 (fixed)
        out[2][0] = 1.f - 0.5f * cm; out[2][1] = 0.f;   // v2: (1,0) -> (.5,0)
        out[3][0] = 1.f;             out[3][1] = 1.f;   // v3 (fixed)
        out[4][0] = 0.f;             out[4][1] = 1.f;   // v4 (fixed)
      };
      auto N2S = [&](float nx, float ny) { return ImVec2(centre.x + (nx - 0.5f) * box, centre.y + (ny - 0.5f) * box); };

      // ::before  - forward time, translated straight down
      float vb[5][2]; clipv(cmf(t), vb);
      const float tyb = tyf(t);
      ImVec2 b[5];
      for (int i = 0; i < 5; i++) b[i] = N2S(vb[i][0], vb[i][1] + tyb);
      window->DrawList->AddTriangleFilled(b[0], b[1], b[4], c);
      window->DrawList->AddTriangleFilled(b[2], b[3], b[1], c);

      // ::after  - reversed time, point-reflected through the centre (scale(-1))
      float va[5][2]; clipv(cmf(1.f - t), va);
      const float tya = tyf(1.f - t);
      ImVec2 a[5];
      for (int i = 0; i < 5; i++) a[i] = N2S(1.f - va[i][0], 1.f - va[i][1] - tya);
      window->DrawList->AddTriangleFilled(a[0], a[1], a[4], c);
      window->DrawList->AddTriangleFilled(a[2], a[3], a[1], c);
    }

    // Corner burst:
    //   a static diamond (the box's edge-midpoint square) with four corner
    //   triangles on top. The corners burst outward (box 40 -> 60 px), rotate
    //   90 degrees and snap back; at rest diamond + corners fill a solid square,
    //   expanded they leave gaps.
    inline void SpinnerCornerBurst(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(240, 51, 85), 1.f);   // #f03355
      const float k = radius / (30.f * 1.41421356f);              // fit the 60px box rotated 45 deg
      const float L = 14.5f * 1.41421356f;                        // corner-triangle leg (~20.5px)

      auto ease = [](float u) { const float kk = 1.f - u; return 1.f - kk * kk * kk; };  // ~cubic-bezier(.3,1,0,1)
      const float th = 1.f / 3.f;

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      float Sz, rot;                            // ::before box side (px) and rotation (rad)
      if (t < th)        { Sz = 40.f + 20.f * ease(t / th);        rot = 0.f; }
      else if (t < 2*th) { Sz = 60.f;                              rot = ease((t - th) / th) * (IM_PI * 0.5f); }
      else               { Sz = 60.f - 20.f * ease((t - 2*th) / th); rot = IM_PI * 0.5f; }
      const float ca = ImCos(rot), sa = ImSin(rot);

      // static diamond (40px box edge midpoints), not animated
      auto S = [&](float x, float y) { return ImVec2(centre.x + x * k, centre.y + y * k); };
      window->DrawList->AddQuadFilled(S(0, -20), S(20, 0), S(0, 20), S(-20, 0), c);

      // four corner triangles on the rotating / resizing ::before box
      auto R = [&](float x, float y) { return ImVec2(centre.x + (x * ca - y * sa) * k, centre.y + (x * sa + y * ca) * k); };
      const float h = Sz * 0.5f;
      static const int sx[4] = { -1, 1, 1, -1 }, sy[4] = { -1, -1, 1, 1 };
      for (int i = 0; i < 4; i++) {
        const float cx = sx[i] * h, cy = sy[i] * h;            // corner (box-local)
        window->DrawList->AddTriangleFilled(R(cx, cy), R(cx - sx[i] * L, cy), R(cx, cy - sy[i] * L), c);
      }
    }

    // Skew square:
    //   two triangular halves (one is the other point-reflected) form a square,
    //   split and fly to opposite corners, return, then the whole thing shears
    //   (skewX -45 deg) reforming the square along the other diagonal.
    inline void SpinnerSkewSquare(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(37, 176, 155), 1.f);  // #25b09b
      const float k = radius / 56.f;            // fit the corner excursion at 33%

      static const float vK[4] = { 0.f, 0.33f, 0.66f, 1.f };      // per-element translate (px)
      static const float vX[4] = { -0.5f, -19.f, -19.f, -19.5f };
      static const float vY[4] = { 0.f, -19.f, 0.f, 0.f };
      static const float sK[4] = { 0.f, 0.66f, 0.8f, 1.f };       // container skewX (deg)
      static const float sV[4] = { 0.f, 0.f, -45.f, -45.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      const float vx = pwl(vK, vX, 4, t), vy = pwl(vK, vY, 4, t);
      const float ang = pwl(sK, sV, 4, t) * (IM_PI / 180.f);
      const float ts = ImSin(ang) / ImCos(ang);   // tan(skew)

      static const float Tx[3] = { 0, 1, 1 }, Ty[3] = { 0, 0, 1 };   // clip triangle: TL, TR, BR
      ImVec2 bp[3], ap[3];
      for (int i = 0; i < 3; i++) {
        const float cx = Tx[i] * 40.f + vx - 20.f, cy = Ty[i] * 40.f + vy - 20.f;  // centred px
        const float sx = cx + ts * cy;             // skewX about the centre
        bp[i] = ImVec2(centre.x + sx * k, centre.y + cy * k);
        ap[i] = ImVec2(centre.x - sx * k, centre.y - cy * k);   // ::after = point reflection
      }
      window->DrawList->AddTriangleFilled(bp[0], bp[1], bp[2], c);
      window->DrawList->AddTriangleFilled(ap[0], ap[1], ap[2], c);
    }

    // Tumble square:
    //   a bar (40x20) with two squares stacked on top (a 40x40 block). The top
    //   squares swing +/-180 deg about their bottom corners while the whole block
    //   slides up and flips (scaleY -1), so the square appears to unfold and
    //   tumble.
    inline void SpinnerTumbleSquare(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(255, 165, 0), 1.f);   // orange
      const float k = radius / 50.f;            // fit the corner excursion mid-flip (~48px)
      const float Cy = 10.f;                    // container transform-origin (centre.y in loader px)

      static const float tyK[6] = { 0.f, 0.1f, 0.4999f, 0.5f, 0.9f, 1.f };   // translateY (fraction of height)
      static const float tyV[6] = { 0.f, 0.f, -0.5f, -0.5f, -1.f, -1.f };
      static const float rfK[5] = { 0.f, 0.1f, 0.5f, 0.9f, 1.f };            // rotate progress (triangle)
      static const float rfV[5] = { 0.f, 0.f, 1.f, 0.f, 0.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      const float ty = pwl(tyK, tyV, 6, t) * 20.f;
      const float sy = (t < 0.5f) ? 1.f : -1.f;
      const float rp = pwl(rfK, rfV, 5, t);

      // e: 0 = bar, 1 = ::before (left, pivot bottom-left), 2 = ::after (right, pivot bottom-right)
      auto xf = [&](float x, float y, int e) -> ImVec2 {
        if (e == 1) {                                   // rotate -180*rp about (0,0)
          const float a = -IM_PI * rp, ca = ImCos(a), sa = ImSin(a);
          const float nx = x * ca - y * sa, ny = x * sa + y * ca; x = nx; y = ny;
        } else if (e == 2) {                            // rotate +180*rp about (40,0)
          const float a = IM_PI * rp, ca = ImCos(a), sa = ImSin(a);
          const float dx = x - 40.f, dy = y;
          x = 40.f + dx * ca - dy * sa; y = dx * sa + dy * ca;
        }
        y = Cy + (y - Cy) * sy + ty;                    // container scaleY about Cy, then translateY
        return ImVec2(centre.x + (x - 20.f) * k, centre.y + y * k);
      };
      auto quad = [&](float x0, float y0, float x1, float y1, int e) {
        window->DrawList->AddQuadFilled(xf(x0, y0, e), xf(x1, y0, e), xf(x1, y1, e), xf(x0, y1, e), c);
      };

      quad(0.f, 0.f, 40.f, 20.f, 0);          // bar
      quad(0.f, -20.f, 20.f, 0.f, 1);         // ::before (left square, above bar)
      quad(20.f, -20.f, 40.f, 0.f, 2);        // ::after  (right square, above bar)
    }

    // Tri-diamond:
    //   two triangular halves (both pivoting about the bottom centre) morph
    //   between an upward triangle and a diamond, while the container flips
    //   (scaleY -1) and turns 180 deg, so the shape pulses and spins.
    inline void SpinnerTriDiamond(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor col = color_alpha(ImColor(81, 75, 130), 1.f);   // #514b82
      const float k = radius / 55.f;            // fit the corner excursion mid-rotation

      static const float rK[5]  = { 0.f, 0.1f, 0.35f, 0.70f, 1.f };   // l10-1 progress (1 = triangle, 0 = diamond)
      static const float rV[5]  = { 1.f, 1.f, 0.f, 1.f, 1.f };
      static const float crK[4] = { 0.f, 0.70f, 0.90f, 1.f };         // container rotate (deg)
      static const float crV[4] = { 0.f, 0.f, 180.f, 180.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;
      if (mode == 2) t = 1.f - t;

      const float rp = pwl(rK, rV, 5, t);
      const float sy = (t < 0.35f) ? 1.f : -1.f;
      const float cr = pwl(crK, crV, 4, t) * (IM_PI / 180.f);
      const float ccr = ImCos(cr), scr = ImSin(cr);
      const float angA = (IM_PI * 0.75f) * rp;    // ::after  rotate (s = +1) -> 135 deg
      const float angB = -(IM_PI * 0.75f) * rp;   // ::before rotate (s = -1)
      const float ty = -50.f * rp;                // translateY -100% -> -50px

      // element point: rotate(ang) about bottom-centre pivot, +translateY; then
      // container rotate(cr) about centre, then scaleY(sy).
      auto elem = [&](float lx, float ly, float ang, float t_y) -> ImVec2 {
        const float ca = ImCos(ang), sa = ImSin(ang);
        const float dx = lx - 25.f, dy = ly - 50.f;
        const float rx = 25.f + dx * ca - dy * sa, ry = 50.f + dx * sa + dy * ca + t_y;
        const float ex = rx - 25.f, ey = ry - 25.f;
        const float r2x = ex * ccr - ey * scr;
        float r2y = (ex * scr + ey * ccr) * sy;
        return ImVec2(centre.x + r2x * k, centre.y + r2y * k);
      };

      // ::after (right) - triangle (25,0),(50,25),(25,50); ::before (left) - (0,25),(25,0),(25,50)
      window->DrawList->AddTriangleFilled(elem(25, 0, angA, ty), elem(50, 25, angA, ty), elem(25, 50, angA, ty), col);
      window->DrawList->AddTriangleFilled(elem(0, 25, angB, ty), elem(25, 0, angB, ty), elem(25, 50, angB, ty), col);
    }

    // Twin blades:
    //   two triangular blades (tip at the centre) spin about the centre in
    //   stepped half-turns (pause, 180 deg, pause). The second blade is offset
    //   90 deg and a half-cycle in time.
    inline void SpinnerTwinBlades(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(37, 176, 155), 1.f);   // #25b09b
      const float k = radius / (20.f * 1.41421356f);               // far vertex reaches the radius

      static const float aK[6] = { 0.f, 0.125f, 0.375f, 0.625f, 0.875f, 1.f };   // stepped rotation (deg)
      static const float aV[6] = { 0.f, 0.f, -180.f, -180.f, -360.f, -360.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      float tt = ImFmod((float)ImGui::GetTime() * speed, 2.f) / 2.f;
      if (mode == 2) tt = 1.f - tt;

      const float angB = pwl(aK, aV, 6, tt) * (IM_PI / 180.f);
      const float angA = (90.f + pwl(aK, aV, 6, ImFmod(tt + 0.75f, 1.f))) * (IM_PI / 180.f);  // delay -1.5s, +90deg

      // blade triangle (centred): top-left, centre, bottom-left
      static const float Vx[3] = { -20.f, 0.f, -20.f }, Vy[3] = { -20.f, 0.f, 20.f };
      auto blade = [&](float ang) {
        const float ca = ImCos(ang), sa = ImSin(ang);
        ImVec2 v[3];
        for (int i = 0; i < 3; i++)
          v[i] = ImVec2(centre.x + (Vx[i] * ca - Vy[i] * sa) * k, centre.y + (Vx[i] * sa + Vy[i] * ca) * k);
        window->DrawList->AddTriangleFilled(v[0], v[1], v[2], c);
      };
      blade(angB);
      blade(angA);
    }

    // Cross blades:
    //   a static vertical bow-tie (two wedges) plus two horizontal blades that
    //   rotate -270 deg about opposite corners (with pauses), while the container
    //   mirrors (scaleX -1). At rest the four triangles fill a solid square; the
    //   blades swing away to reveal the bow-tie.
    inline void SpinnerCrossBlades(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(81, 75, 130), 1.f);   // #514b82
      const float k = radius / 64.f;            // fit the blade excursion mid-rotation

      auto S = [&](float x, float y) { return ImVec2(centre.x + (x - 20.f) * k, centre.y + (y - 20.f) * k); };

      // static background bow-tie (vertical): top and bottom wedges from the centre
      window->DrawList->AddTriangleFilled(S(20, 20), S(0, 0), S(40, 0), c);
      window->DrawList->AddTriangleFilled(S(20, 20), S(40, 40), S(0, 40), c);

      float phase = ImFmod((float)ImGui::GetTime() * speed, 2.f);
      if (mode == 2) phase = 2.f - phase;
      const float sx = (phase < 1.f) ? 1.f : -1.f;        // container scaleX flips at 1 sec
      const float f = (phase < 1.f) ? phase : (2.f - phase);   // 1 sec alternate -> triangle wave

      static const float kK[4] = { 0.f, 0.2f, 0.8f, 1.f };     // stepped rotate (deg)
      static const float kV[4] = { 0.f, 0.f, -270.f, -270.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };
      const float rot = pwl(kK, kV, 4, f) * (IM_PI / 180.f);
      const float ca = ImCos(rot), sa = ImSin(rot);

      auto elem = [&](float x, float y, float px, float py) -> ImVec2 {
        const float dx = x - px, dy = y - py;
        float rx = px + dx * ca - dy * sa;            // rotate about the pivot corner
        const float ry = py + dx * sa + dy * ca;
        rx = 20.f + (rx - 20.f) * sx;                 // container scaleX about the centre
        return ImVec2(centre.x + (rx - 20.f) * k, centre.y + (ry - 20.f) * k);
      };

      // ::before (left blade) pivots about the bottom-left corner
      window->DrawList->AddTriangleFilled(elem(0, 0, 0, 40), elem(20, 20, 0, 40), elem(0, 40, 0, 40), c);
      // ::after (right blade) pivots about the top-right corner
      window->DrawList->AddTriangleFilled(elem(40, 0, 40, 0), elem(40, 40, 40, 0), elem(20, 20, 40, 0), c);
    }

    // Drift square:
    //   two triangular halves (one is the other point-reflected) form a square,
    //   drift together along a diagonal, rotate 180 deg and return.
    inline void SpinnerDriftSquare(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(255, 165, 0), 1.f);   // orange
      const float k = radius / 57.f;            // fit the diagonal drift excursion

      static const float tK[6]  = { 0.f, 0.1f, 0.33f, 0.66f, 0.9f, 1.f };
      static const float txV[6] = { 0.f, 0.f, 20.f, 20.f, 0.f, 0.f };
      static const float tyV[6] = { 0.f, 0.f, -20.f, -20.f, 0.f, 0.f };
      static const float rV[6]  = { 0.f, 0.f, 0.f, 180.f, 180.f, 180.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      float t = ImFmod((float)ImGui::GetTime() * speed, 2.f) / 2.f;
      if (mode == 2) t = 1.f - t;

      const float tx = pwl(tK, txV, 6, t), ty = pwl(tK, tyV, 6, t);
      const float rot = pwl(tK, rV, 6, t) * (IM_PI / 180.f);
      const float ca = ImCos(rot), sa = ImSin(rot);

      // clip triangle (centred): top-left half - TL, TR, BL
      static const float Vx[3] = { -20.f, 20.f, -20.f }, Vy[3] = { -20.f, -20.f, 20.f };
      ImVec2 bp[3], ap[3];
      for (int i = 0; i < 3; i++) {
        const float qx = Vx[i] * ca - Vy[i] * sa + tx;   // rotate about centre, then translate
        const float qy = Vx[i] * sa + Vy[i] * ca + ty;
        bp[i] = ImVec2(centre.x + qx * k, centre.y + qy * k);
        ap[i] = ImVec2(centre.x - qx * k, centre.y - qy * k);   // ::after = point reflection
      }
      window->DrawList->AddTriangleFilled(bp[0], bp[1], bp[2], c);
      window->DrawList->AddTriangleFilled(ap[0], ap[1], ap[2], c);
    }

    inline void SpinnerPieBounce(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(240, 51, 85), 1.f);   // #f03355
      const float r = radius;
      const float k = r / 20.f;                  // CSS box is 40px -> half = 20px

      // outer: stepped 90deg rotation, held 1s each over a 4s cycle
      float phase = ImFmod((float)ImGui::GetTime() * speed, 4.f);
      if (mode == 2) phase = 4.f - phase;
      const int step = (int)phase;               // 0..3
      const float base = (float)step * (IM_PI * 0.5f);
      const float cb = ImCos(base), sb = ImSin(base);
      auto rot = [&](float x, float y) -> ImVec2 {
        return ImVec2(centre.x + (x * cb - y * sb), centre.y + (x * sb + y * cb));
      };

      // ::before: 0.5s alternate diagonal bounce translate(0,0) <-> (-10,-10)
      const float bt = ImFmod((float)ImGui::GetTime() * speed, 1.f);
      const float tri = (bt < 0.5f) ? bt * 2.f : (1.f - bt) * 2.f;   // 0..1..0
      const float off = -10.f * tri * k;

      const int N = 48;
      // body: 270deg pie, gap on the top-left quadrant (screen angles 180..270 empty)
      const float a0 = IM_PI * 1.5f;             // 270deg (up), sweep clockwise 270deg
      ImVec2 prev = rot(r * ImCos(a0), r * ImSin(a0));
      for (int i = 1; i <= N; i++) {
        const float a = a0 + (IM_PI * 1.5f) * ((float)i / (float)N);
        const ImVec2 cur = rot(r * ImCos(a), r * ImSin(a));
        window->DrawList->AddTriangleFilled(centre, prev, cur, c);
        prev = cur;
      }

      // ::before: rounded quarter filling the gap, bouncing out along the local diagonal
      const ImVec2 qc = rot(off, off);
      const float qa0 = IM_PI;                    // 180deg (left) .. 270deg (up)
      ImVec2 qprev = rot(r * ImCos(qa0) + off, r * ImSin(qa0) + off);
      const int QN = 16;
      for (int i = 1; i <= QN; i++) {
        const float a = qa0 + (IM_PI * 0.5f) * ((float)i / (float)QN);
        const ImVec2 cur = rot(r * ImCos(a) + off, r * ImSin(a) + off);
        window->DrawList->AddTriangleFilled(qc, qprev, cur, c);
        qprev = cur;
      }
    }

    // Leap-frog: two orange half panels share a hinge at the bottom-centre.
    // They fold open (rotate +/-90 deg) while the whole assembly translates
    // upward and flips vertically, tumbling end-over-end.
    inline void SpinnerLeapFrog(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(255, 165, 0), 1.f);   // orange
      const float k = radius / 50.f;            // fit the vertical tumble excursion

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;   // 0..1
      if (mode == 2) t = 1.f - t;

      // parent l15-0: translateY (% of 40px height) + scaleY flip about y=50 (125%)
      float ty;
      if (t <= 0.1f)      ty = 0.f;
      else if (t < 0.5f)  ty = -30.f * (t - 0.1f) / 0.4f;
      else if (t < 0.9f)  ty = -30.f - 30.f * (t - 0.5f) / 0.4f;
      else                ty = -60.f;
      const float sy = (t < 0.5f) ? 1.f : -1.f;

      // child l15-1: hinge rotation, triangle wave peaking at +/-90 deg at 50%
      float ra;
      if (t <= 0.1f || t >= 0.9f) ra = 0.f;
      else if (t < 0.5f)          ra = 90.f * (t - 0.1f) / 0.4f;
      else                        ra = 90.f * (0.9f - t) / 0.4f;

      // CSS coords (0..40, y down); hinge at bottom-centre (20,40), scale pivot y=50
      auto elem = [&](float x, float y, float ang) -> ImVec2 {
        const float rad = ang * (IM_PI / 180.f);
        const float ca = ImCos(rad), sa = ImSin(rad);
        const float dx = x - 20.f, dy = y - 40.f;
        const float rx = 20.f + (dx * ca - dy * sa);
        float ry = 40.f + (dx * sa + dy * ca);
        ry = 50.f + (ry - 50.f) * sy;             // parent scaleY about y=50
        ry += ty;                                 // parent translateY
        return ImVec2(centre.x + (rx - 20.f) * k, centre.y + (ry - 5.f) * k);
      };

      // ::before (left half) hinges -90, ::after (right half) hinges +90
      window->DrawList->AddQuadFilled(elem(0, 0, -ra), elem(20, 0, -ra), elem(20, 40, -ra), elem(0, 40, -ra), c);
      window->DrawList->AddQuadFilled(elem(20, 0, ra), elem(40, 0, ra), elem(40, 40, ra), elem(20, 40, ra), c);
    }

    // Plus squares: a static green plus sign, with four small squares pinned
    // to the corners. They expand outward, rotate 90 deg about the centre, then
    // contract back.
    inline void SpinnerPlusSquares(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(37, 176, 155), 1.f);   // #25b09b
      const float k = radius / 44.f;            // fit the expanded+rotated corner reach

      auto S = [&](float x, float y) { return ImVec2(centre.x + (x - 20.f) * k, centre.y + (y - 20.f) * k); };

      // static plus sign: horizontal + vertical 10px bars through the centre
      window->DrawList->AddRectFilled(S(0, 15), S(40, 25), c);
      window->DrawList->AddRectFilled(S(15, 0), S(25, 40), c);

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;   // 0..1
      if (mode == 2) t = 1.f - t;

      // l16: inset 0 -> -10 -> -10 -> 0, rotate 0 -> 0 -> 90 -> 90
      static const float kK[4] = { 0.f, 0.33f, 0.66f, 1.f };
      static const float insV[4] = { 0.f, -10.f, -10.f, 0.f };
      static const float rotV[4] = { 0.f, 0.f, 90.f, 90.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      const float ins = pwl(kK, insV, 4, t);
      const float rot = pwl(kK, rotV, 4, t) * (IM_PI / 180.f);
      const float ca = ImCos(rot), sa = ImSin(rot);

      const float h = 7.75f;                    // half of the 15.5px corner squares
      const float lo = ins + h, hi = 40.f - ins - h;
      static const float ox[4] = { -h, h, h, -h }, oy[4] = { -h, -h, h, h };
      const float cx[4] = { lo, hi, lo, hi }, cy[4] = { lo, lo, hi, hi };

      for (int s = 0; s < 4; s++) {
        const float dcx = cx[s] - 20.f, dcy = cy[s] - 20.f;
        const float rcx = 20.f + dcx * ca - dcy * sa;     // rotate the square centre about (20,20)
        const float rcy = 20.f + dcx * sa + dcy * ca;
        ImVec2 p[4];
        for (int i = 0; i < 4; i++)                        // rotate the square's own corners too
          p[i] = S(rcx + ox[i] * ca - oy[i] * sa, rcy + ox[i] * sa + oy[i] * ca);
        window->DrawList->AddQuadFilled(p[0], p[1], p[2], p[3], c);
      }
    }

    // Seven march: two "7"-shaped tiles (one is the other point-reflected) that
    // shuffle around one another - marching up, shifting right, and rotating
    // -90 deg before settling.
    inline void SpinnerSevenMarch(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(81, 75, 130), 1.f);   // #514b82
      const float k = radius / 50.f;            // fit the upward march excursion

      float t = ImFmod((float)ImGui::GetTime() * speed, 2.f) / 2.f;   // 0..1
      if (mode == 2) t = 1.f - t;

      // sp7 keyframes: translate(tx,ty) + rotate(deg), held at the ends
      static const float kK[6]  = { 0.f, 0.1f, 0.33f, 0.66f, 0.9f, 1.f };
      static const float txV[6] = { 0.f, 0.f, 0.f, 10.f, 10.f, 10.f };
      static const float tyV[6] = { 0.f, 0.f, -20.f, -20.f, -10.f, -10.f };
      static const float dgV[6] = { 0.f, 0.f, 0.f, -90.f, -90.f, -90.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      const float tx = pwl(kK, txV, 6, t), ty = pwl(kK, tyV, 6, t);
      const float rad = pwl(kK, dgV, 6, t) * (IM_PI / 180.f);
      const float ca = ImCos(rad), sa = ImSin(rad);

      // CSS coords 40x60, transform-origin centre (20,30): rotate, translate, scale
      auto X = [&](float x, float y, float sx, float sy) -> ImVec2 {
        const float qx = x - 20.f, qy = y - 30.f;
        float rx = qx * ca - qy * sa + tx;        // rotate about centre, then translate
        float ry = qx * sa + qy * ca + ty;
        rx *= sx; ry *= sy;                        // scale flip (::after = -1,-1)
        return ImVec2(centre.x + rx * k, centre.y + ry * k);
      };

      // the "7" tile decomposes into a top bar (full width) and a right-side leg
      auto seven = [&](float sx, float sy) {
        window->DrawList->AddQuadFilled(X(0, 0, sx, sy), X(40, 0, sx, sy), X(40, 20.4f, sx, sy), X(0, 20.4f, sx, sy), c);
        window->DrawList->AddQuadFilled(X(20, 20.4f, sx, sy), X(40, 20.4f, sx, sy), X(40, 40.2f, sx, sy), X(20, 40.2f, sx, sy), c);
      };

      seven(1.f, 1.f);       // ::before
      seven(-1.f, -1.f);     // ::after (point reflection)
    }

    // Corner arcs: a static centre dot ringed by an annulus. The ring splits
    // into four quarter pieces that pull apart radially, rotate 90 deg about the
    // centre, then reform into the ring. Rounded sibling of SpinnerCornerSquares.
    inline void SpinnerCornerArcs(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(240, 51, 85), 1.f);   // #f03355
      const float k = radius / 35.f;            // fit the split-apart reach

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;   // 0..1
      if (mode == 2) t = 1.f - t;

      // l18: inset 0 -> -10 -> -10 -> 0 (pieces split by |inset|*sqrt2), rotate 0->0->90->90
      static const float kK[4]   = { 0.f, 0.33f, 0.66f, 1.f };
      static const float offV[4] = { 0.f, 14.14f, 14.14f, 0.f };
      static const float rotV[4] = { 0.f, 0.f, 90.f, 90.f };
      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      const float off = pwl(kK, offV, 4, t);
      const float ar  = pwl(kK, rotV, 4, t) * (IM_PI / 180.f);
      const float car = ImCos(ar), sar = ImSin(ar);

      // static centre dot (the loader's own background gradient)
      window->DrawList->AddCircleFilled(centre, 10.f * k, c, 24);

      const float ri = 9.f, ro = 20.f;          // annulus inner/outer radii
      const int M = 16;
      for (int q = 0; q < 4; q++) {
        const float bis = (45.f + q * 90.f) * (IM_PI / 180.f);   // quadrant bisector
        const float ox = off * ImCos(bis), oy = off * ImSin(bis);
        const float a0 = (q * 90.f) * (IM_PI / 180.f);           // quadrant start angle
        ImVec2 pin_prev, pout_prev;
        for (int i = 0; i <= M; i++) {
          const float ang = a0 + (IM_PI * 0.5f) * ((float)i / (float)M);
          const float cx = ImCos(ang), cy = ImSin(ang);
          const float ixx = ox + ri * cx, iyy = oy + ri * cy;   // inner/outer arc points
          const float oxx = ox + ro * cx, oyy = oy + ro * cy;   // about the split-out centre
          const ImVec2 pin  = ImVec2(centre.x + (ixx * car - iyy * sar) * k, centre.y + (ixx * sar + iyy * car) * k);
          const ImVec2 pout = ImVec2(centre.x + (oxx * car - oyy * sar) * k, centre.y + (oxx * sar + oyy * car) * k);
          if (i > 0)
            window->DrawList->AddQuadFilled(pin_prev, pout_prev, pout, pin, c);
          pin_prev = pin; pout_prev = pout;
        }
      }
    }

    inline void SpinnerWalkBlocks(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(255, 165, 0), 1.f);   // orange
      const float k = radius / 40.f;            // fit the full walk-apart reach (~80 wide)

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;   // 0..1
      if (mode == 2) t = 1.f - t;

      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      // l19-0: container climbs translateY 0 -> -10 -> -20 and flips scaleY at 50%
      static const float cK[5]   = { 0.f, 0.3f, 0.5f, 0.7f, 1.f };
      static const float ctyV[5] = { 0.f, 0.f, -10.f, -20.f, -20.f };
      const float cty = pwl(cK, ctyV, 5, t);
      const float csy = (t < 0.5f) ? 1.f : -1.f;

      // l19-1: each half walks out, down, then back (tx,ty)
      static const float hK[7]  = { 0.f, 0.1f, 0.3f, 0.5f, 0.7f, 0.9f, 1.f };
      static const float txV[7] = { 0.f, 0.f, 20.f, 20.f, 20.f, 0.f, 0.f };
      static const float tyV[7] = { 0.f, 0.f, 0.f, 20.f, 0.f, 0.f, 0.f };
      const float tx = pwl(hK, txV, 7, t);
      const float ty = pwl(hK, tyV, 7, t);

      // container transform: scaleY about (20,10) then translateY, mapped to screen
      auto C = [&](float x, float y) -> ImVec2 {
        const float yy = 10.f + csy * (y - 10.f) + cty;
        return ImVec2(centre.x + (x - 20.f) * k, centre.y + (yy - 10.f) * k);
      };
      // half transform: l19-1 translate (+ scaleX flip for :before), then container
      auto H = [&](float px, float py, bool before) -> ImVec2 {
        const float x1 = before ? (20.f - px - tx) : (px + tx);
        const float y1 = py + ty;
        return C(x1, y1);
      };

      // the loader bar itself: x[0,40] y[0,20]
      window->DrawList->AddQuadFilled(C(0, 0), C(40, 0), C(40, 20), C(0, 20), c);
      // :before  (left half, local x[0,20] y[-20,0], scaleX -1)
      window->DrawList->AddQuadFilled(H(0, -20, true), H(20, -20, true), H(20, 0, true), H(0, 0, true), c);
      // :after  (right half, local x[20,40] y[-20,0])
      window->DrawList->AddQuadFilled(H(20, -20, false), H(40, -20, false), H(40, 0, false), H(20, 0, false), c);
    }

    inline void SpinnerTumbleHalfDisc(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(240, 51, 85), 1.f);   // #f03355
      const float k = radius / 60.f;            // fit the full tumble sweep (~120 tall)

      float t2 = ImFmod((float)ImGui::GetTime() * speed, 2.f) / 2.f;   // 0..1 over 2s
      if (mode == 2) t2 = 1.f - t2;

      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      // l20-1: halves ping-pong (1s alternate) -> triangle wave u
      const float u = (t2 < 0.5f) ? (t2 * 2.f) : ((1.f - t2) * 2.f);
      static const float uK[4]  = { 0.f, 0.2f, 0.5f, 1.f };
      static const float aV[4]  = { 0.f, 0.f, -90.f, -90.f };   // outer rotate
      static const float tyV[4] = { 0.f, 0.f, 0.f, -20.f };     // translate up
      static const float bV[4]  = { 0.f, 0.f, 0.f, -90.f };     // inner rotate
      const float A  = pwl(uK, aV, 4, u);
      const float ty = pwl(uK, tyV, 4, u);
      const float B  = pwl(uK, bV, 4, u);

      // l20-0: container climbs translateY 0 -> -80 and flips scaleY about (0,60)
      static const float cK[5]   = { 0.f, 0.1f, 0.5f, 0.9f, 1.f };
      static const float ctyV[5] = { 0.f, 0.f, -40.f, -80.f, -80.f };
      const float cty = pwl(cK, ctyV, 5, t2);
      const float csy = (t2 < 0.5f) ? 1.f : -1.f;

      const float Px = 20.f, Py = 40.f;         // shared half pivot (bottom of disc)
      const float Oy = 60.f;                     // container y-origin (0% 150%)

      // draw a half-disc: arc points, transformed (half rotate-translate-rotate, then container)
      auto drawHalf = [&](float a0, float s) {
        const float rA = s * A * (IM_PI / 180.f), cA = ImCos(rA), sA = ImSin(rA);
        const float rB = s * B * (IM_PI / 180.f), cB = ImCos(rB), sB = ImSin(rB);
        const int N = 24;
        ImVec2 pts[N + 1];
        for (int i = 0; i <= N; i++) {
          const float ang = (a0 + 180.f * (float)i / (float)N) * (IM_PI / 180.f);
          const float px = 20.f + 20.f * ImCos(ang), py = 20.f + 20.f * ImSin(ang);
          float rx = px - Px, ry = py - Py;
          float qx = rx * cB - ry * sB, qy = rx * sB + ry * cB;
          qy += ty;
          const float q2x = qx * cA - qy * sA, q2y = qx * sA + qy * cA;
          const float x1 = Px + q2x, y1 = Py + q2y;
          const float y2 = Oy + csy * (y1 - Oy) + cty;
          pts[i] = ImVec2(centre.x + (x1 - 20.f) * k, centre.y + (y2 - 20.f) * k);
        }
        window->DrawList->AddConvexPolyFilled(pts, N + 1, c);
      };

      drawHalf(90.f, 1.f);    // :before  left semicircle (90..270)
      drawHalf(-90.f, -1.f);  // :after   right semicircle (-90..90), --s:-1
    }

    inline void SpinnerFoldHalfDisc(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(240, 51, 85), 1.f);   // #f03355
      const float k = radius / 40.f;            // fit the swap sweep (~80 wide/tall)

      float t = ImFmod((float)ImGui::GetTime() * speed, 2.f) / 2.f;   // 0..1 over 2s
      if (mode == 2) t = 1.f - t;

      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      // l21: translate(tx,ty) then rotate(-90); rot shared, tx/ty scaled by --s per half
      static const float tK[5]   = { 0.f, 0.33f, 0.66f, 0.90f, 1.f };
      static const float txV[5]  = { 0.f, 0.f, 0.f, -20.f, -20.f };   // -100% of width(20)
      static const float tyV[5]  = { 0.f, 20.f, 20.f, 20.f, 20.f };   // 50% of height(40)
      static const float rotV[5] = { 0.f, 0.f, -90.f, -90.f, -90.f };
      const float rot = pwl(tK, rotV, 5, t) * (IM_PI / 180.f);
      const float cr = ImCos(rot), sr = ImSin(rot);
      const float tx = pwl(tK, txV, 5, t);
      const float ty = pwl(tK, tyV, 5, t);

      // draw a half-disc rotated about its own transform-origin, then translated
      auto drawHalf = [&](float a0, float s, float ox, float oy) {
        const int N = 24;
        ImVec2 pts[N + 1];
        for (int i = 0; i <= N; i++) {
          const float ang = (a0 + 180.f * (float)i / (float)N) * (IM_PI / 180.f);
          const float px = 20.f + 20.f * ImCos(ang), py = 20.f + 20.f * ImSin(ang);
          const float rx = px - ox, ry = py - oy;
          float qx = rx * cr - ry * sr + s * tx;
          float qy = rx * sr + ry * cr + s * ty;
          const float x2 = ox + qx, y2 = oy + qy;
          pts[i] = ImVec2(centre.x + (x2 - 20.f) * k, centre.y + (y2 - 20.f) * k);
        }
        window->DrawList->AddConvexPolyFilled(pts, N + 1, c);
      };

      drawHalf(90.f, 1.f, 20.f, 0.f);     // :before  left semicircle, origin top-right
      drawHalf(-90.f, -1.f, 20.f, 40.f);  // :after   right semicircle, origin bottom-left, --s:-1
    }

    inline void SpinnerWiperArrow(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(81, 75, 130), 1.f);   // #514b82
      const float k = radius / 44.f;            // fit the wiper sweep (~87 wide)
      const float d2r = IM_PI / 180.f;

      float t2 = ImFmod((float)ImGui::GetTime() * speed, 2.f) / 2.f;   // 0..1 over 2s
      if (mode == 2) t2 = 1.f - t2;

      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      // l22-1: pseudos wipe 0 -> 180deg (1s alternate -> triangle wave u), scaled by --s
      const float u = (t2 < 0.5f) ? (t2 * 2.f) : ((1.f - t2) * 2.f);
      static const float wK[4]  = { 0.f, 0.3f, 0.7f, 1.f };
      static const float wV[4]  = { 0.f, 0.f, 180.f, 180.f };
      const float r1 = pwl(wK, wV, 4, u);

      // l22-0: container flips scaleY at 50%, then rotates 180 near the end, about (18,18)
      const float csy = (t2 < 0.5f) ? 1.f : -1.f;
      static const float cK[3]   = { 0.f, 0.9f, 1.f };
      static const float crV[3]  = { 0.f, 0.f, 180.f };
      const float crot = pwl(cK, crV, 3, t2) * d2r;
      const float ccr = ImCos(crot), scr = ImSin(crot);

      // container transform (rotate then scaleY about centre) + map to screen
      auto CT = [&](float x, float y) -> ImVec2 {
        const float rx = x - 18.f, ry = y - 18.f;
        const float qx = rx * ccr - ry * scr;
        const float qy = (rx * scr + ry * ccr) * csy;
        return ImVec2(centre.x + qx * k, centre.y + qy * k);
      };
      // pseudo: rotate (s*r1) about its own origin, then container
      auto P = [&](float x, float y, float ox, float oy, float s) -> ImVec2 {
        const float pr = s * r1 * d2r, cp = ImCos(pr), sp = ImSin(pr);
        const float rx = x - ox, ry = y - oy;
        return CT(ox + rx * cp - ry * sp, oy + rx * sp + ry * cp);
      };

      // background: conic wedge clipped to box -> convex pentagon (apex top-centre)
      ImVec2 bg[5] = { CT(18, 0), CT(36, 17.38f), CT(36, 36), CT(0, 36), CT(0, 17.38f) };
      window->DrawList->AddConvexPolyFilled(bg, 5, c);
      // :before triangle, pivot top-right (0,18)
      ImVec2 bf[3] = { P(0, 18, 0, 18, 1.f), P(0, 36, 0, 18, 1.f), P(-18, 36, 0, 18, 1.f) };
      window->DrawList->AddConvexPolyFilled(bf, 3, c);
      // :after triangle, pivot top-left (36,18), --s:-1
      ImVec2 af[3] = { P(36, 18, 36, 18, -1.f), P(54, 36, 36, 18, -1.f), P(36, 36, 36, 18, -1.f) };
      window->DrawList->AddConvexPolyFilled(af, 3, c);
    }

    inline void SpinnerSkewSquares(const char *label, float radius, float thickness, const ImColor &color = white, float speed = 1.f, int mode = 0)
    {
      SPINNER_HEADER(pos, size, centre, num_segments);
      (void)thickness; (void)color;

      const ImColor c = color_alpha(ImColor(37, 176, 155), 1.f);   // #25b09b
      const float k = radius / 47.f;            // fit skew sweep (~87 wide) + 3px shadow
      const float d2r = IM_PI / 180.f;

      float t = ImFmod((float)ImGui::GetTime() * speed, 1.5f) / 1.5f;   // 0..1
      if (mode == 2) t = 1.f - t;

      auto pwl = [](const float *K, const float *V, int n, float tt) -> float {
        if (tt <= K[0]) return V[0];
        for (int i = 0; i < n - 1; i++)
          if (tt <= K[i + 1]) { const float u = (tt - K[i]) / (K[i + 1] - K[i]); return V[i] + (V[i + 1] - V[i]) * u; }
        return V[n - 1];
      };

      // l23-0: container skewX 0 -> -45deg about centre
      static const float skK[4] = { 0.f, 0.5f, 0.9f, 1.f };
      static const float skV[4] = { 0.f, 0.f, -45.f, -45.f };
      const float sk = pwl(skK, skV, 4, t) * d2r;
      const float tanx = ImSin(sk) / ImCos(sk); // tan via sin/cos (ImTan not available)

      // l23-1: pseudos rotate 0 -> 180deg about their origins; box-shadow grows to 3px
      static const float rK[3] = { 0.f, 0.4f, 1.f };
      static const float rV[3] = { 0.f, 180.f, 180.f };
      const float rot = pwl(rK, rV, 3, t) * d2r;
      const float cr = ImCos(rot), sr = ImSin(rot);
      const float sf = pwl(rK, rV, 3, t) / 180.f;   // 0 -> 1 shadow spread factor

      // container skewX about centre (18,18) -> screen
      auto CT = [&](float x, float y) -> ImVec2 {
        const float rx = x - 18.f, ry = y - 18.f;
        return ImVec2(centre.x + (rx + tanx * ry) * k, centre.y + ry * k);
      };
      // pseudo: rotate about its origin, then container
      auto P = [&](float x, float y, float ox, float oy) -> ImVec2 {
        const float rx = x - ox, ry = y - oy;
        return CT(ox + rx * cr - ry * sr, oy + rx * sr + ry * cr);
      };

      // background: 45deg gradient band (|x-y|<=18.72) clipped to box -> convex hexagon
      ImVec2 hex[6] = { CT(0, 0), CT(18.72f, 0), CT(36, 17.28f), CT(36, 36), CT(17.28f, 36), CT(0, 18.72f) };
      window->DrawList->AddConvexPolyFilled(hex, 6, c);

      // box-shadow: 3px outline around each pseudo border-box (square), rotated
      if (sf > 0.01f) {
        const ImColor sc = color_alpha(c, sf);
        const float th = 3.f * sf * k;
        ImVec2 sb[4] = { P(0, 18, 0, 18), P(18, 18, 0, 18), P(18, 36, 0, 18), P(0, 36, 0, 18) };
        window->DrawList->AddPolyline(sb, 4, sc, ImDrawFlags_Closed, th);
        ImVec2 sa[4] = { P(18, 0, 36, 18), P(36, 0, 36, 18), P(36, 18, 36, 18), P(18, 18, 36, 18) };
        window->DrawList->AddPolyline(sa, 4, sc, ImDrawFlags_Closed, th);
      }

      // :before triangle (bottom-left), pivot top-left (0,18)
      ImVec2 bf[3] = { P(0, 18, 0, 18), P(18, 36, 0, 18), P(0, 36, 0, 18) };
      window->DrawList->AddConvexPolyFilled(bf, 3, c);
      // :after triangle (top-right), pivot bottom-right (36,18)
      ImVec2 af[3] = { P(18, 0, 36, 18), P(36, 0, 36, 18), P(36, 18, 36, 18) };
      window->DrawList->AddConvexPolyFilled(af, 3, c);
    }

#ifndef _IMSPINNER_SHAPES_INTERNAL_
} // namespace ImSpinner
#endif

#endif // _IMSPINNER_SHAPES_H_
