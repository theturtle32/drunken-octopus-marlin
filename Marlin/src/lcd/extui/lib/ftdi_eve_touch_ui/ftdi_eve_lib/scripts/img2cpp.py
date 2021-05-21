#!/usr/bin/python

# Written By Marcio Teixeira 2021 - SynDaver Labs, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# To view a copy of the GNU General Public License, go to the following
# location: <https://www.gnu.org/licenses/>.

from __future__ import print_function
from PIL import Image
import argparse
import textwrap
import os
import sys
import zlib

class WriteSource:
  def __init__(self):
    self.values      = []

  def convert_to_rgb565(self, color):
     return ((color[0] & 0xF8) << 8) + ((color[1] & 0xFC) << 3) + ((color[2] & 0xF8) >> 3);
     
  def deflate(self, data):
     return zlib.compress(data)

  def add_pixel(self, value):
    value = self.convert_to_rgb565(value);
    self.values.append((value & 0x00FF) >> 0);
    self.values.append((value & 0xFF00) >> 8);

  def end_row(self, y):
    pass

  def write(self, varname, deflate):
    print("Length of uncompressed data: ", len(self.values), file=sys.stderr)
    data = bytes(bytearray(self.values))
    if deflate:
      data = self.deflate(data)
      print("Length of data after compression: ", len(data), file=sys.stderr)
    data = bytearray(data)
    data = list(map(lambda a: "0x" + format(a, '02x'), data))
    nElements = len(data)
    data = ', '.join(data)
    data = textwrap.fill(data, 75, initial_indent = '  ', subsequent_indent = '  ')

    print("const unsigned char " + varname + "[" + format(nElements) + "] PROGMEM = {")
    print(data)
    print("};")

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Converts a bitmap into a C array')
  parser.add_argument("input")
  parser.add_argument("-d", "--deflate", action="store_true", help="Packs the data using the deflate algorithm")
  args = parser.parse_args()

  varname = os.path.splitext(os.path.basename(args.input))[0];

  writer = WriteSource()

  img = Image.open(args.input)
  print("Image height: ", img.height, file=sys.stderr)
  print("Image width:  ", img.width,  file=sys.stderr)
  for y in range(img.height):
    for x in range(img.width):
      writer.add_pixel(img.getpixel((x,y)))
    writer.end_row(y)
  writer.write(varname, args.deflate)
