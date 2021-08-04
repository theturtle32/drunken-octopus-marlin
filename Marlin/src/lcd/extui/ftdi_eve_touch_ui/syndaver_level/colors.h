/***************************
 * syndaver_level/colors.h *
 ***************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver 3D                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#pragma once

namespace Theme {

  // SynDaver accent colors
  constexpr int      accent_hue             = 240;
  constexpr float    accent_sat             = 0.5;
  constexpr uint32_t syn_color              = hsl_to_rgb(accent_hue, accent_sat, 0.13);
  //constexpr uint32_t daver_color            = hsl_to_rgb(349, 0.73, 0.34);
  constexpr uint32_t daver_color            = hsl_to_rgb(343, 1.0, 0.23);

  // Shades of accent color
  constexpr uint32_t accent_color_0         = hsl_to_rgb(accent_hue, accent_sat, 0.15); // Darkest
  constexpr uint32_t accent_color_1         = hsl_to_rgb(accent_hue, accent_sat, 0.26);
  constexpr uint32_t accent_color_2         = hsl_to_rgb(accent_hue, accent_sat, 0.39);
  constexpr uint32_t accent_color_3         = hsl_to_rgb(accent_hue, accent_sat, 0.52);
  constexpr uint32_t accent_color_4         = hsl_to_rgb(accent_hue, accent_sat, 0.65);
  constexpr uint32_t accent_color_5         = hsl_to_rgb(accent_hue, accent_sat, 0.78);
  constexpr uint32_t accent_color_6         = hsl_to_rgb(accent_hue, accent_sat, 0.91); // Lightest

  // Shades of gray

  constexpr float    gray_sat               = 0.14;
  constexpr uint32_t gray_color_0           = hsl_to_rgb(accent_hue, gray_sat, 0.15); // Darkest
  constexpr uint32_t gray_color_1           = hsl_to_rgb(accent_hue, gray_sat, 0.26);
  constexpr uint32_t gray_color_2           = hsl_to_rgb(accent_hue, gray_sat, 0.39);
  constexpr uint32_t gray_color_3           = hsl_to_rgb(accent_hue, gray_sat, 0.52);
  constexpr uint32_t gray_color_4           = hsl_to_rgb(accent_hue, gray_sat, 0.65);
  constexpr uint32_t gray_color_5           = hsl_to_rgb(accent_hue, gray_sat, 0.78);
  constexpr uint32_t gray_color_6           = hsl_to_rgb(accent_hue, gray_sat, 0.91); // Lightest

  constexpr uint32_t theme_darkest          = accent_color_1;
  constexpr uint32_t theme_dark             = accent_color_4;

  constexpr uint32_t bg_color               = 0xFFFFFF;
  constexpr uint32_t axis_label             = gray_color_5;

  constexpr uint32_t bg_text_enabled        = accent_color_1;
  constexpr uint32_t bg_text_disabled       = gray_color_1;
  constexpr uint32_t bg_normal              = accent_color_4;
  constexpr uint32_t fg_disabled            = gray_color_6;
  constexpr uint32_t fg_normal              = accent_color_1;
  constexpr uint32_t fg_action              = daver_color;

  constexpr uint32_t logo_bg_rgb            = accent_color_5;
  constexpr uint32_t logo_fill_rgb          = accent_color_6;
  constexpr uint32_t logo_stroke_rgb        = accent_color_2;

  #define BED_MESH_POINTS_GRAY
  constexpr uint32_t bed_mesh_lines_rgb     = syn_color;
  constexpr uint32_t bed_mesh_shadow_rgb    = 0xAAAAAA;

  constexpr uint32_t shadow_rgb             = gray_color_6;
  constexpr uint32_t stroke_rgb             = accent_color_1;
  constexpr uint32_t fill_rgb               = accent_color_3;
  constexpr uint32_t syringe_rgb            = accent_color_5;
  constexpr uint32_t fluid_rgb              = accent_color_3;

  constexpr uint32_t x_axis                 = hsl_to_rgb(0,   1.00, 0.26);
  constexpr uint32_t y_axis                 = hsl_to_rgb(120, 1.00, 0.13);
  constexpr uint32_t z_axis                 = hsl_to_rgb(240, 1.00, 0.10);
  constexpr uint32_t e_axis                 = axis_label;
  constexpr uint32_t feedrate               = axis_label;
  constexpr uint32_t other                  = axis_label;

  // Status screen
  constexpr uint32_t progress               = axis_label;
  constexpr uint32_t status_msg             = axis_label;
  constexpr uint32_t fan_speed              = hsl_to_rgb(240, 0.5, 0.13);
  constexpr uint32_t temp                   = hsl_to_rgb(343, 1.0, 0.23);

  constexpr uint32_t disabled_icon          = gray_color_1;

  // Calibration Registers Screen
  constexpr uint32_t transformA             = 0x3010D0;
  constexpr uint32_t transformB             = 0x4010D0;
  constexpr uint32_t transformC             = 0x5010D0;
  constexpr uint32_t transformD             = 0x6010D0;
  constexpr uint32_t transformE             = 0x7010D0;
  constexpr uint32_t transformF             = 0x8010D0;
  constexpr uint32_t transformVal           = 0x104010;

  constexpr btn_colors disabled_btn         = {.bg = bg_color,      .grad = fg_disabled, .fg = fg_disabled,  .rgb = fg_disabled };
  constexpr btn_colors normal_btn           = {.bg = fg_action,     .grad = 0xFFFFFF,    .fg = fg_normal,    .rgb = 0xFFFFFF };
  constexpr btn_colors action_btn           = {.bg = bg_color,      .grad = 0xFFFFFF,    .fg = fg_action,    .rgb = 0xFFFFFF };
  constexpr btn_colors red_btn              = {.bg = 0xFF5555,      .grad = 0xFFFFFF,    .fg = 0xFF0000,     .rgb = 0xFFFFFF };
  constexpr btn_colors ui_slider            = {.bg = theme_darkest, .grad = 0xFFFFFF,    .fg = theme_dark,   .rgb = accent_color_3 };
  constexpr btn_colors ui_toggle            = {.bg = theme_darkest, .grad = 0xFFFFFF,    .fg = theme_dark,   .rgb = 0xFFFFFF };

  // Temperature color scale

  const rgb_t cool_rgb (  0,   0,   0);
  const rgb_t low_rgb  (128,   0,   0);
  const rgb_t med_rgb  (255, 128,   0);
  const rgb_t high_rgb (255, 255, 128);
};
