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
import math

class WriteSource:
  def __init__(self, img, mode):
    self.values      = []
    self.mode        = mode
    self.offset      = 8
    self.byte        = 0
    self.img         = img

  def finish_byte(self):
    if self.offset != 8:
      self.values.append(self.byte)
      self.offset = 8
      self.byte   = 0

  def add_bits_to_byte(self, value, size = 1):
    self.offset -= size
    self.byte = self.byte | value << self.offset
    if self.offset == 0:
      self.finish_byte()

  def append_rgb565(self, color):
    value = ((color[0] & 0xF8) << 8) + ((color[1] & 0xFC) << 3) + ((color[2] & 0xF8) >> 3)
    self.values.append((value & 0x00FF) >> 0);
    self.values.append((value & 0xFF00) >> 8);

  def append_rgb332(self, color):
    value = (color[0] & 0xE0) + ((color[1] & 0xE0) >> 3) + ((color[2] & 0xC0) >> 6)
    self.values.append(value);

  def append_argb2(self, color):
    value = (color[3] & 0xC0) + ((color[0] & 0xC0) >> 2) + ((color[1] & 0xC0) >> 4) + ((color[2] & 0xC0) >> 6)
    self.values.append(value);

  def append_argb4(self, color):
    value = ((color[3] & 0xF0) << 8) + ((color[0] & 0xF0) << 4) + (color[1] & 0xF0) + ((color[2] & 0xF0) >> 4)
    self.values.append((value & 0x00FF) >> 0);
    self.values.append((value & 0xFF00) >> 8);

  def append_argb1555(self, color):
    value = ((color[3] & 0x80) << 8) + ((color[0] & 0xF8) << 7) + ((color[1] & 0xF8) << 2) + ((color[2] & 0xF8) >> 3)
    self.values.append((value & 0x00FF) >> 0);
    self.values.append((value & 0xFF00) >> 8);

  def append_grayscale(self, color, bits):
    luminance = int(0.2126 * color[0] + 0.7152 * color[1] + 0.0722 * color[2])
    self.add_bits_to_byte(luminance >> (8 - bits), bits)

  def deflate(self, data):
     return zlib.compress(data)

  def add_pixel(self, color):
    if self.mode == "l1":
      self.append_grayscale(color, 1)
    elif self.mode == "l2":
      self.append_grayscale(color, 2)
    elif self.mode == "l4":
      self.append_grayscale(color, 4)
    elif self.mode == "l8":
      self.append_grayscale(color, 8)
    elif self.mode == "rgb565":
      self.append_rgb565(color)
    elif self.mode == "rgb332":
      self.append_rgb332(color)
    elif self.mode == "argb2":
      self.append_argb2(color)
    elif self.mode == "argb4":
      self.append_argb4(color)
    elif self.mode == "argb1555":
      self.append_argb1555(color)

  def end_row(self, y):
    if self.mode in ["l1", "l2", "l3"]:
       self.finish_byte()

  def get_line_stride(self):
    if self.mode == "l1":
      bbp = 1
    elif self.mode == "l2":
      bbp = 2
    elif self.mode == "l4":
      bbp = 4
    elif self.mode == "l8":
      bbp = 8
    elif self.mode == "rgb565":
      bbp = 16
    elif self.mode == "rgb332":
      bbp = 8
    elif self.mode == "argb2":
      bbp = 8
    elif self.mode == "argb4":
      bbp = 16
    elif self.mode == "argb1555":
      bbp = 16
    return int(math.ceil(self.img.width * bbp / 8))

  def write_info(self, varname, args):
    print("constexpr PROGMEM bitmap_info_t " + varname + "_Info = {")
    print("  .format        = " + self.mode.upper() + ",")
    print("  .width         = " + format(self.img.width) + ",")
    print("  .height        = " + format(self.img.height) + ",")
    print("  .linestride    = " + format(self.get_line_stride()) + ",")
    if mode in ["argb2", "argb4", "argb1555"]:
      print("  .filter        = NEAREST,")
    else:
      print("  .filter        = BILINEAR,")
    print("  .wrapx         = BORDER,")
    print("  .wrapy         = BORDER,")
    print("  .inflated_size = " + format(self.uncompressed_size) + ",")
    if args.offset:
      print("  .RAMG_offset   = " + format(args.offset) + ",")
    print("};")
    print()

  def write(self, varname, args):
    self.uncompressed_size = len(self.values)
    print("Length of uncompressed data: ", self.uncompressed_size, file=sys.stderr)
    data = bytes(bytearray(self.values))
    if args.deflate:
      data = self.deflate(data)
      self.compressed_size = len(data)
      print("Length of data after compression: ", self.compressed_size, file=sys.stderr)
    else:
      self.compressed_size = 0
    data = bytearray(data)
    data = list(map(lambda a: "0x" + format(a, '02X'), data))
    nElements = len(data)
    data = ', '.join(data)
    data = textwrap.fill(data, 75, initial_indent = '  ', subsequent_indent = '  ')

    self.write_info(varname, args)
    print("constexpr PROGMEM unsigned char " + varname + "[" + format(nElements) + "] = {")
    print(data)
    print("};")

if __name__ == "__main__":
  parser = argparse.ArgumentParser(description='Converts a bitmap into a C array')
  parser.add_argument("input")
  parser.add_argument("-d", "--deflate", action="store_true", help="Packs the data using the deflate algorithm")
  parser.add_argument("-m", "--mode", default="l1", help="Mode, can be l1, l2, l4, l8, rgb332, rgb565, argb2, argb4 or argb1555")
  parser.add_argument("-o", "--offset", default="0", help="If provided, will write an offset to the information block")
  args = parser.parse_args()

  varname = os.path.splitext(os.path.basename(args.input))[0];

  img = Image.open(args.input)
  img = img.convert('RGBA')
  print("Image height: ", img.height, file=sys.stderr)
  print("Image width:  ", img.width,  file=sys.stderr)
  writer = WriteSource(img, args.mode)

  for y in range(img.height):
    for x in range(img.width):
      writer.add_pixel(img.getpixel((x,y)))
    writer.end_row(y)
  writer.write(varname, args)
