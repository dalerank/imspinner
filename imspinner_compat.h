#ifndef _IMSPINNER_COMPAT_H_
#define _IMSPINNER_COMPAT_H_

/*
 * imspinner compat layer
 *
 * Wires add-on spinners into the core Spinner<> dispatcher (spinner_draw_funcs)
 * and provides Spinner() / Spinner<Type>() entry points.
 * Included from imspinner.h; include directly for Spinner<> without other add-ons.
 */

#ifndef _IMSPINNER_COMPAT_INTERNAL_
#include "imspinner.h"
namespace ImSpinner
{
#endif

#define _IMSPINNER_BARS_INTERNAL_
#include "imspinner_bars.h"
#undef _IMSPINNER_BARS_INTERNAL_

    namespace detail {
      static struct SpinnerDraw { SpinnerTypeT type; void (*func)(const char *, const detail::SpinnerConfig &); } spinner_draw_funcs[e_st_count] = {
        { e_st_rainbow, [] (const char *label, const detail::SpinnerConfig &c) { SpinnerRainbow(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed, c.m_AngleMin, c.m_AngleMax, c.m_Dots, c.m_Mode); } },
        { e_st_angle,   [] (const char *label, const detail::SpinnerConfig &c) { SpinnerAng(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_BgColor, c.m_Speed, c.m_Angle, c.m_Mode); } },
        { e_st_dots,    [] (const char *label, const detail::SpinnerConfig &c) { SpinnerDots(label, c.m_FloatPtr, c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed, c.m_Dots, c.m_MinThickness, c.m_Mode); } },
        { e_st_ang,     [] (const char *label, const detail::SpinnerConfig &c) { SpinnerAng(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_BgColor, c.m_Speed, c.m_Angle, c.m_Mode); } },
        { e_st_vdots,   [] (const char *label, const detail::SpinnerConfig &c) { SpinnerVDots(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_BgColor, c.m_Speed, c.m_Dots, c.m_MiddleDots, c.m_Mode); } },
        { e_st_bounce_ball, [] (const char *label,const detail::SpinnerConfig &c) { SpinnerBounceBall(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed, c.m_Dots); } },
        { e_st_eclipse, [] (const char *label, const detail::SpinnerConfig &c) { SpinnerAngEclipse(label , c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed); } },
        { e_st_ingyang, [] (const char *label, const detail::SpinnerConfig &c) { SpinnerIngYang(label, c.m_Radius, c.m_Thickness, c.m_Reverse, c.m_Delta, c.m_AltColor, c.m_Color, c.m_Speed, c.m_Angle); } },
        { e_st_barchartsine, [] (const char *label, const detail::SpinnerConfig &c) { SpinnerBarChartSine(label, c.m_Radius, c.m_Thickness, c.m_Color, c.m_Speed, c.m_Dots, c.m_Mode); } }
      };
    }

    inline void Spinner(const char *label, const detail::SpinnerConfig& config)
    {
        if (config.m_SpinnerType < sizeof(detail::spinner_draw_funcs) / sizeof(detail::spinner_draw_funcs[0]))
            detail::spinner_draw_funcs[config.m_SpinnerType].func(label, config);
    }

    template<SpinnerTypeT Type, typename... Args>
    inline void Spinner(const char *label, const Args&... args)
    {
      detail::SpinnerConfig config(SpinnerType{Type}, args...);
      Spinner(label, config);
    }

#ifndef _IMSPINNER_COMPAT_INTERNAL_
} // namespace ImSpinner
#endif

#endif // _IMSPINNER_COMPAT_H_
