/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <elements/element/drag_and_drop.hpp>
#include <elements/element/traversal.hpp>
#include <elements/element/composite.hpp>
#include <elements/element/port.hpp>
#include <elements/support/context.hpp>
#include <elements/support/theme.hpp>
#include <elements/view.hpp>

namespace cycfi { namespace elements
{
   drop_base::drop_base(std::initializer_list<std::string> mime_types_)
    : _mime_types{mime_types_}
   {}

   bool drop_base::wants_control() const
   {
      return true;
   }

   void drop_base::track_drop(context const& ctx, drop_info const& info, cursor_tracking status)
   {
      // Return early if none of registered mime types is in the `drop_info`
      if (!std::any_of(_mime_types.begin(), _mime_types.end(),
         [&](auto const& item)
         {
            return info.data.find(item) != info.data.end();
         }))
         return;

      auto new_is_tracking = status != cursor_tracking::leaving;
      if (new_is_tracking != _is_tracking)
      {
         _is_tracking = new_is_tracking;
         ctx.view.refresh(ctx.bounds);
      }
   }

   bool drop_base::drop(context const& /*ctx*/, drop_info const& /*info*/)
   {
      _is_tracking = false;
      return false;
   }

   drop_box_base::drop_box_base(std::initializer_list<std::string> mime_types_)
    : base_type{mime_types_}
   {}

   void drop_box_base::draw(context const& ctx)
   {
      proxy_base::draw(ctx);
      if (is_tracking())
      {
         auto& cnv = ctx.canvas;
         auto& bounds = ctx.bounds;
         cnv.stroke_style(get_theme().indicator_hilite_color.opacity(0.5));
         cnv.line_width(2.0);
         cnv.add_rect(bounds);
         cnv.stroke();
      }
   }

   bool drop_box_base::drop(context const& ctx, drop_info const& info)
   {
      base_type::drop(ctx, info);
      bool r = on_drop(info);
      ctx.view.refresh(ctx.bounds);
      return r;
   }

   drop_inserter_base::drop_inserter_base(std::initializer_list<std::string> mime_types_)
    : base_type{mime_types_}
   {}

   void drop_inserter_base::draw(context const& ctx)
   {
      proxy_base::draw(ctx);
      if (is_tracking())
      {
         if (auto c = find_subject<composite_base*>(this))
         {
            in_context_do(ctx, *c, [&](context const& cctx)
            {
               point cursor_pos = ctx.cursor_pos();
               auto hit_info = c->hit_element(cctx, cursor_pos, false);
               if (hit_info.element_ptr)
               {
                  rect const& bounds = hit_info.bounds;
                  bool before = cursor_pos.y < (bounds.top + (bounds.height()/2));
                  float pos = before? bounds.top : bounds.bottom;
                  _insertion_pos = before? hit_info.index : hit_info.index+1;

                  auto &cnv = cctx.canvas;
                  cnv.stroke_style(get_theme().indicator_hilite_color.opacity(0.5));
                  cnv.line_width(2.0);
                  cnv.move_to({bounds.left, pos});
                  cnv.line_to({bounds.right, pos});
                  cnv.stroke();
               }
            });
         }
      }
   }

   void drop_inserter_base::track_drop(context const& ctx, drop_info const& info, cursor_tracking status)
   {
      base_type::track_drop(ctx, info, status);
      if (is_tracking())
      {
         static constexpr auto offset = 20;
         rect r = {info.where.x-offset, info.where.y-offset, info.where.x+offset, info.where.y+offset};
         scrollable::find(ctx).scroll_into_view(r);
         ctx.view.refresh(ctx.bounds);
      }
   }

   bool drop_inserter_base::drop(context const& ctx, drop_info const& info)
   {
      base_type::drop(ctx, info);
      if (_insertion_pos > 0)
      {
         bool r = on_drop(info, _insertion_pos);
         ctx.view.refresh(ctx.bounds);
         _insertion_pos = -1;
         return r;
      }
      return false;
   }
}}

