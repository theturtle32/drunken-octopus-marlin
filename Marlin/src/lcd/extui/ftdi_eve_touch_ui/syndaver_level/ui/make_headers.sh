#!/bin/bash

SCRIPT_PATH=../../ftdi_eve_lib/scripts
OUTPUT_PATH=../autogen

mkdir -p $OUTPUT_PATH

make_img() {
  format=$1
  offset=$2
  file=$3
  base=`basename ${file%%.*}`
  echo
  echo Generating $base.png
  $SCRIPT_PATH/img2cpp.py --deflate --mode $format $file --offset $offset > $OUTPUT_PATH/$base.h
}

make_svg() {
  file=$1
  base=`basename ${file%%.*}`
  echo
  echo Generating $base.png
  $SCRIPT_PATH/svg2cpp.py $file > $OUTPUT_PATH/$base.h
}

make_img argb1555 LIGHTBULB_OFFSET  lightbulb.png
make_img rgb332   BACKGROUND_OFFSET leveling_with_auto_menu.png
make_img rgb332   BACKGROUND_OFFSET leveling_wo_auto_menu.png
make_img rgb332   BACKGROUND_OFFSET move_menu.png
make_img rgb332   BACKGROUND_OFFSET printing_screen.png
make_img rgb332   BACKGROUND_OFFSET settings_with_wifi_menu.png
make_img rgb332   BACKGROUND_OFFSET settings_wo_wifi_menu.png
make_img rgb332   BACKGROUND_OFFSET status_screen.png
make_img rgb332   BACKGROUND_OFFSET tools_menu.png

make_svg layout_3_icons.svg
make_svg layout_4_icons.svg
make_svg layout_5_icons.svg
make_svg move_screen.svg