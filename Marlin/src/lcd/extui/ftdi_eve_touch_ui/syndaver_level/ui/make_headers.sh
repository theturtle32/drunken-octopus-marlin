#!/bin/bash

SCRIPT_PATH=../../ftdi_eve_lib/scripts
OUTPUT_PATH=../autogen

mkdir -p $OUTPUT_PATH

for file in *.png; do
  base=${file%%.*}
  echo
  echo Generating $base.png
  $SCRIPT_PATH/img2cpp.py --deflate  --mode rgb332 $base.png > $OUTPUT_PATH/$base.h
done

for file in *.svg; do
  base=${file%%.*}
  echo
  echo Generating $base.svg
  $SCRIPT_PATH/svg2cpp.py $base.svg > $OUTPUT_PATH/$base.h
done

echo
echo
