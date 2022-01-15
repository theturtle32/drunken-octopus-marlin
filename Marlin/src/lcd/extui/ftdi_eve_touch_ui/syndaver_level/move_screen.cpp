/**********************************
 * syndaver_level/move_screen.cpp *
 **********************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2019 - Cocoa Press                          *
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

#include "../config.h"
#include "../screens.h"
#include "../screen_data.h"

#ifdef SYNDAVER_LEVEL_MOVE_SCREEN

#include "autogen/move_screen.h"

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))

const uint8_t shadow_depth = 5;
const float   max_speed  = 1.00;
const float   min_speed  = 0.02;

using namespace FTDI;
using namespace ExtUI;

constexpr static MoveScreenData &mydata = screen_data.MoveScreen;

void MoveScreen::draw_arrows(draw_mode_t what) {
  const bool isHomed = isAxisPositionKnown(X) && isAxisPositionKnown(Y) && isAxisPositionKnown(Z);

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  ui.button_fill  (Theme::fill_rgb);
  ui.button_stroke(Theme::stroke_rgb, 28);
  ui.button_shadow(Theme::shadow_rgb, shadow_depth);

  constexpr uint8_t style = PolyUI::REGULAR;

  if ((what & BACKGROUND) || isHomed) {
    ui.button(isHomed ? 1 : 0, POLY(x_neg), style);
    ui.button(isHomed ? 2 : 0, POLY(x_pos), style);
  }

  if ((what & BACKGROUND) || isHomed) {
    ui.button(isHomed ? 3 : 0, POLY(y_neg), style);
    ui.button(isHomed ? 4 : 0, POLY(y_pos), style);
  }

  if ((what & BACKGROUND) || isHomed) {
    ui.button(isHomed ? 5 : 0, POLY(z_neg), style);
    ui.button(isHomed ? 6 : 0, POLY(z_pos), style);
  }
}

void MoveScreen::draw_overlay_icons(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  ui.button_fill  (Theme::fill_rgb);
  ui.button_stroke(Theme::stroke_rgb, 28);
  ui.button_shadow(Theme::shadow_rgb, shadow_depth);

  constexpr uint8_t style = PolyUI::REGULAR;
  ui.button(16, POLY(home_all), style);
}

void MoveScreen::draw_adjuster(draw_mode_t what, uint8_t tag, FSTR_P label, float value, int16_t x, int16_t y, int16_t w, int16_t h) {
  #define SUB_COLS 10
  #define SUB_ROWS 1

  CommandProcessor cmd;
  cmd.tag(0)
     .font(Theme::font_medium);
  if (what & BACKGROUND) {
    cmd.cmd(COLOR_RGB(Theme::bg_text_enabled))
       .text(  SUB_POS(1,1), SUB_SIZE(1,1), label)
       .colors(Theme::normal_btn)
       .button(SUB_POS(2,1), SUB_SIZE(5,1), F(""), OPT_FLAT);
  }

  if (what & FOREGROUND) {
    char str[32];
    dtostrf(value, 3, 0, str);
    strcat_P(str, PSTR(" "));
    strcat_P(str, (const char*) GET_TEXT_F(MSG_UNITS_MM));

    cmd.text(SUB_POS(2,1), SUB_SIZE(5,1), str)
       .colors(Theme::normal_btn)
       .tag(tag  ).button(SUB_POS(7,1), SUB_SIZE(2,1), F("-"))
       .tag(tag+1).button(SUB_POS(9,1), SUB_SIZE(2,1), F("+"));
  }
}

void MoveScreen::draw_disabled(draw_mode_t what, int16_t x, int16_t y, int16_t w, int16_t h) {
  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.cmd(SAVE_CONTEXT())
       .tag(0)
       .cmd(COLOR_A(224))
       .cmd(COLOR_RGB(Theme::bg_color))
       .rectangle(x, y, w, h)
       .cmd(RESTORE_CONTEXT());
  }
}

void MoveScreen::draw_increments(draw_mode_t what, int16_t x, int16_t y, int16_t w, int16_t h) {
  #undef  SUB_COLS
  #undef  SUB_ROWS
  #define SUB_COLS 10
  #define SUB_ROWS 1

  using namespace Theme;

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(font_small);
    cmd.tag(13).colors(13 == mydata.inc_tag ? action_btn : normal_btn).button(SUB_POS(2,1), SUB_SIZE(3,1), F("1"));
    cmd.tag(14).colors(14 == mydata.inc_tag ? action_btn : normal_btn).button(SUB_POS(5,1), SUB_SIZE(3,1), F("10"));
    cmd.tag(15).colors(15 == mydata.inc_tag ? action_btn : normal_btn).button(SUB_POS(8,1), SUB_SIZE(3,1), F("100"));
  }
}

float MoveScreen::get_increment() {
  switch(mydata.inc_tag) {
    case 13: return 1;
    case 14: return 10;
    default: return 100;
  }
}

void MoveScreen::draw_buttons(draw_mode_t what) {
  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  if (what & FOREGROUND) {
    int16_t x, y, w, h;
    ui.bounds(POLY(done_btn), x, y, w, h);

    cmd.colors(Theme::action_btn)
       .font(Theme::font_medium)
       .tag(20).button(x, y, w, h, GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

void MoveScreen::onEntry() {
  BaseScreen::onEntry();
  mydata.inc_tag = 15;
  mydata.jog_inc = 0;
}

void MoveScreen::onRedraw(draw_mode_t what) {
  const bool isHomed = isAxisPositionKnown(X) && isAxisPositionKnown(Y) && isAxisPositionKnown(Z);

  CommandProcessor cmd;
  PolyUI ui(cmd, what);

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(Theme::bg_color))
       .cmd(CLEAR(true,true,true))
       .tag(0);
  }

  int16_t x, y, w, h;
  ui.bounds(POLY(x_axis), x, y, w, h);

  draw_arrows(what);
  draw_overlay_icons(what);
  draw_buttons(what);

  ui.bounds(POLY(increments), x, y, w, h);
  draw_increments(what, x, y, w, h);
  ui.bounds(POLY(x_axis), x, y, w, h);
  draw_adjuster(what, 7, F("X:"), getAxisPosition_mm(X), x, y, w, h);
  ui.bounds(POLY(y_axis), x, y, w, h);
  draw_adjuster(what, 9, F("Y:"), getAxisPosition_mm(Y), x, y, w, h);
  ui.bounds(POLY(z_axis), x, y, w, h);
  draw_adjuster(what, 11, F("Z:"), getAxisPosition_mm(Z), x, y, w, h);
  if(!isHomed) {
      ui.bounds(POLY(axis_controls), x, y, w, h);
      draw_disabled(what, x, y, w, h);
  }
}

bool MoveScreen::onTouchStart(uint8_t) {
  mydata.jog_inc = 0;
  return true;
}

bool MoveScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case  1:
    case  2:
    case  3:
    case  4:
    case  5:
    case  6:
      jog({ 0, 0, 0 });
      break;
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
      onTouchHeld(tag);
      break;
    case 13:
    case 14:
    case 15:
      mydata.inc_tag = tag;
      break;
    case 16:
      SpinnerDialogBox::enqueueAndWait(F("G28")); break;
      break;
    case 20: GOTO_PREVIOUS(); break;
    default: return false;
  }
  return true;
}

float MoveScreen::getManualFeedrate(uint8_t axis, float increment_mm) {
  // Compute feedrate so that the tool lags the adjuster when it is
  // being held down, this allows enough margin for the planner to
  // connect segments and even out the motion.
  constexpr xyze_feedrate_t max_manual_feedrate = MANUAL_FEEDRATE;
  return min(max_manual_feedrate[axis] / 60.0f, abs(increment_mm * (TOUCH_REPEATS_PER_SECOND) * 0.80f));
}

void MoveScreen::setManualFeedrate(ExtUI::axis_t axis, float increment_mm) {
  ExtUI::setFeedrate_mm_s(getManualFeedrate(X_AXIS + (axis - ExtUI::X), increment_mm));
}

bool MoveScreen::onTouchHeld(uint8_t tag) {
  #define UI_INCREMENT_AXIS(axis) setManualFeedrate(axis, increment); UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) setManualFeedrate(axis, increment); UI_DECREMENT(AxisPosition_mm, axis);
  const float increment = get_increment();
  const float s = min_speed + (max_speed - min_speed) * sq(mydata.jog_inc);
  mydata.jog_inc = min(1.0f, mydata.jog_inc + 0.1f);
  switch (tag) {
    case 1: jog({-s,  0,  0}); return false;
    case 2: jog({ s,  0,  0}); return false;
    case 3: jog({ 0, -s,  0}); return false;
    case 4: jog({ 0,  s,  0}); return false;
    case 5: jog({ 0,  0,  s}); return false; // Z orientation reversed because bed rather than nozzle moves
    case 6: jog({ 0,  0, -s}); return false;
    case 7: UI_DECREMENT_AXIS(X); break;
    case 8: UI_INCREMENT_AXIS(X); break;
    case 9: UI_DECREMENT_AXIS(Y); break;
    case 10: UI_INCREMENT_AXIS(Y); break;
    case 11: UI_DECREMENT_AXIS(Z); break;
    case 12: UI_INCREMENT_AXIS(Z); break;
    default:
      return false;
  }
  return true;
}

void MoveScreen::onIdle() {
  reset_menu_timeout();
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    if (!EventLoop::is_touch_held())
      onRefresh();
    refresh_timer.start();
  }
}

#endif // SYNDAVER_LEVEL_MOVE_SCREEN
