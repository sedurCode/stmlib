// Copyright 2012 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// A set of basic operands, especially useful for fixed-point arithmetic, with
// fast ASM implementations.

#ifndef STMLIB_UTILS_DSP_H_
#define STMLIB_UTILS_DSP_H_

#include "stmlib/stmlib.h"

#define CLIP(x) if (x < -32767) x = -32767; if (x > 32767) x = 32767;

namespace stmlib {

inline int16_t Interpolate824(const int16_t* table, uint32_t phase)
  __attribute__((always_inline));

inline uint16_t Interpolate824(const uint16_t* table, uint32_t phase)
  __attribute__((always_inline));

inline int16_t Interpolate824(const uint8_t* table, uint32_t phase)
  __attribute__((always_inline));

inline int16_t Interpolate1022(const uint8_t* table, uint32_t phase)
  __attribute__((always_inline));

inline uint16_t Interpolate88(const uint16_t* table, uint16_t index)
  __attribute__((always_inline));

inline int16_t Interpolate88(const int16_t* table, uint16_t index)
  __attribute__((always_inline));

inline int16_t Crossfade(
    const int16_t* table_a,
    const int16_t* table_b,
    uint32_t phase,
    uint16_t balance)
  __attribute__((always_inline));

inline int16_t Crossfade(
    const uint8_t* table_a,
    const uint8_t* table_b,
    uint32_t phase,
    uint16_t balance)
  __attribute__((always_inline));

inline int16_t Mix(int16_t a, int16_t b, uint16_t balance) {
  return (a * (65535 - balance) + b * balance) >> 16;
}

inline uint16_t Mix(uint16_t a, uint16_t b, uint16_t balance) {
  return (a * (65535 - balance) + b * balance) >> 16;
}

inline int16_t Interpolate824(const int16_t* table, uint32_t phase) {
  int32_t a = table[phase >> 24];
  int32_t b = table[(phase >> 24) + 1];
  return a + ((b - a) * static_cast<int32_t>((phase >> 9) & 0x7fff) >> 15);
}

inline uint16_t Interpolate824(const uint16_t* table, uint32_t phase) {
  int32_t a = table[phase >> 24];
  int32_t b = table[(phase >> 24) + 1];
  return a + ((b - a) * static_cast<int32_t>((phase >> 9) & 0x7fff) >> 15);
}

inline int16_t Interpolate824(const uint8_t* table, uint32_t phase) {
  int32_t a = table[phase >> 24];
  int32_t b = table[(phase >> 24) + 1];
  return (a << 8) + \
      ((b - a) * static_cast<int32_t>((phase >> 1) & 0x7fffff) >> 15) - 32768;
}

inline uint16_t Interpolate88(const uint16_t* table, uint16_t index) {
  int32_t a = table[index >> 8];
  int32_t b = table[(index >> 8) + 1];
  return a + ((b - a) * static_cast<int32_t>(index & 0xff) >> 8);
}

inline int16_t Interpolate88(const int16_t* table, uint16_t index) {
  int32_t a = table[index >> 8];
  int32_t b = table[(index >> 8) + 1];
  return a + ((b - a) * static_cast<int32_t>(index & 0xff) >> 8);
}

inline int16_t Interpolate1022(const int16_t* table, uint32_t phase) {
  int32_t a = table[phase >> 22];
  int32_t b = table[(phase >> 22) + 1];
  return a + ((b - a) * static_cast<int32_t>((phase >> 7) & 0x7fff) >> 15);
}

inline int16_t Crossfade(
    const int16_t* table_a,
    const int16_t* table_b,
    uint32_t phase,
    uint16_t balance) {
  return (Interpolate824(table_a, phase) * (65535 - balance) + \
      Interpolate824(table_b, phase) * balance) >> 16;
}

inline int16_t Crossfade(
    const uint8_t* table_a,
    const uint8_t* table_b,
    uint32_t phase,
    uint16_t balance) {
  return (Interpolate824(table_a, phase) * (65535 - balance) + \
      Interpolate824(table_b, phase) * balance) >> 16;
}

}  // namespace stmlib

#endif  // STMLIB_UTILS_DSP_H_