#!/bin/bash

for file in *.png; do
  base=${file%%.*}
  echo
  echo Generating $base
  ../../../ftdi_eve_lib/scripts/img2cpp.py --deflate  --mode rgb332 $base.png > $base.h
done
echo