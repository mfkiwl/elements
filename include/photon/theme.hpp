/*=================================================================================================
   Copyright (c) 2016 Joel de Guzman

   Licensed under a Creative Commons Attribution-ShareAlike 4.0 International.
   http://creativecommons.org/licenses/by-sa/4.0/
=================================================================================================*/
#if !defined(PHOTON_GUI_LIB_THEME_APRIL_15_2016)
#define PHOTON_GUI_LIB_THEME_APRIL_15_2016

#include <photon/rect.hpp>
#include <photon/color.hpp>
#include <photon/circle.hpp>
#include <memory>

// forward
struct NVGcontext;

namespace photon
{
   class theme
   {
   public:

                        theme() : _vg(0) {}
      virtual           ~theme() {}

      void              context(NVGcontext* vg_)   { _vg = vg_; }
      NVGcontext*       context() const { return _vg; }

      /////////////////////////////////////////////////////////////////////////////////////////////
      // Panels
      double            panel_corner_radius        = 3.0f;
      color             panel_color                = { 28, 30, 34, 192 };
      rect              panel_shadow_offset        = { -10, -10, +20, +30 };

      virtual void      draw_panel(rect const& b) const;

      /////////////////////////////////////////////////////////////////////////////////////////////
      // Sliders
      double            slider_knob_radius         = 0.25;  // fraction of size (width or height)
      double            slider_slot_size           = 0.2;   // fraction of size (width or height)
      color             slider_knob_fill_color     = { 40, 43, 48, 255 };
      color             slider_knob_outline_color  = { 0, 0, 0, 92 };

      virtual void      draw_slider(double pos, rect const& b) const;
      virtual void      draw_slider_knob(double pos, rect const& b) const;
      virtual circle    slider_knob_position(double pos, rect const& b) const;

      /////////////////////////////////////////////////////////////////////////////////////////////
      // Fonts

      char const*       icons       = "icons";
      char const*       sans        = "sans";
      char const*       sans_bold   = "sans-bold";

      virtual void      load_fonts() const;

   private:

      NVGcontext*       _vg;
   };

   using theme_ptr = std::shared_ptr<theme>;
}

#endif