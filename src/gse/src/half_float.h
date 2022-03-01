/*
 *  half_float.h
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module defines utility functions for converting between single
 *      precision floating point numbers and half floats.  See:
 *      https://en.wikipedia.org/wiki/Half-precision_floating-point_format
 *      and
 *      https://en.wikipedia.org/wiki/Single-precision_floating-point_format.
 *
 *  Portability Issues:
 *      The C++ float and double types are assumed to be implemented following
 *      IEEE-754 specification.
 */

#ifndef HALF_FLOAT_H
#define HALF_FLOAT_H

#include <cstdint>

namespace gs
{

// Function to convert from 32-bit floating point values to 16-bit values
//constexpr
std::uint16_t FloatToHalfFloat(const float f);

// Function to convert from 16-bit half floating point values to 32-bit values
constexpr float HalfFloatToFloat(const std::uint16_t h);

} // namespace gs

// Include the constexpr functions
#include "half_float.cpp"

#endif // HALF_FLOAT_H
