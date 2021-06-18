#!/bin/bash

for file in *.svg; do
  base=${file%%.*}
  echo
  echo Generating $base
  ../../../ftdi_eve_lib/scripts/svg2cpp.py $base.svg > $base.h
done
echo