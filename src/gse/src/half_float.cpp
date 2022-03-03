/*
 *  half_float.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module implements utility functions for converting between single-
 *      precision floating point numbers and half floats.  See:
 *      https://en.wikipedia.org/wiki/Half-precision_floating-point_format
 *      and
 *      https://en.wikipedia.org/wiki/Single-precision_floating-point_format.
 *
 *      Note that these are constexpr functions and this file is included in
 *      the header file.  This file is not a separate compilation unit.
 *
 *      Additionally, there are two lines of code that cast floats to/from
 *      unsigned integers.  This is quite a hack, but it allowed these functions
 *      to compile as constexpr functions on both g++ and MSVC.  It would be
 *      best to replace those lines with std::bit_cast, but that was not
 *      supported by the g++ compiler used in development.  g++ 11 and later
 *      does support std::bit_cast.
 *
 *  Portability Issues:
 *      The C++ float and double types are assumed to be implemented following
 *      IEEE-754 specification.
 */

#ifndef HALF_FLOAT_CPP
#define HALF_FLOAT_CPP

#include <cstring>
#include <cstdint>
#include <cmath>
#include <bit>

namespace gs
{

/*
 *  FloatToHalfFloat
 *
 *  Description:
 *      This function will convert a 32-bit single-precision floating point
 *      value to a 16-bit half-precision floating point value.
 *
 *  Parameters:
 *      f [in]
 *          The floating point value to convert.
 *
 *  Returns:
 *      Returns the half-precision floating point value stored in
 *      a 16-bit unsigned integer field.
 *
 *  Comments:
 *      None.
 */
constexpr std::uint16_t FloatToHalfFloat(const float f)
{
    constexpr std::uint32_t nan_value = 0b0111111000000000; // qNaN
    constexpr std::uint32_t inf_value = 0b0111110000000000;
    std::uint32_t bits{};
    std::uint32_t sign_bit{};
    std::uint32_t exponent{};
    std::uint32_t mantissa{};
    std::uint16_t h{};

    // Ensure values are the right number of octets
    static_assert(sizeof(f) == 4);
    static_assert(sizeof(bits) == 4);
    static_assert(sizeof(h) == 2);

    // Assign the float in a bit array 32-bits long
    union
    {
        std::uint32_t i;
        float f;
    } u{};
    u.f = f;
    bits = u.i;

    // Extract the sign bit, shifting into position
    sign_bit = (bits & 0x8000'0000) >> 16;

    // Extract the exponent, shifting all the way to the right
    exponent = (bits & 0x7f80'0000) >> 23;

    // Extact the mantissa, shifting into final position
    mantissa = (bits & 0x007f'ffff) >> 13;

    // If this isn't an infinite value or NAN
    if (exponent != 0b1111'1111)
    {
        // Is the exponent within range of a half float?
        if ((exponent >= 113) && (exponent <= 142))
        {
            // Adjust the exponent bias for half floats
            exponent -= 112;

            // Compose a half float value from the components
            h = static_cast<std::uint16_t>(sign_bit | (exponent << 10) |
                                           mantissa);

            // Round up if the Msb of the truncated mantissa is 1
            // NOTE: this could result in a INF result, which would be correct
            if (bits & 0x0000'1000) h++;
        }
        else if (!exponent)
        {
            // True zero or subnormal number

            // A subnormal number would be so small that it would just be
            // truncated to a zero value; just insert the sign bit
            h = static_cast<std::uint16_t>(sign_bit);
        }
        else if (exponent <= 112)
        {
            // Number is subnormal (approaching 0)

            // Subnormal numbers have a leading implicit 0 bit, while normal
            // floats have an implicit leading 1 bit; add that 1 bit and
            // shift to the right, which might produce a 0 value if
            // the number is really small
            mantissa = (mantissa | 0x0000'0400) >> (113 - exponent);

            h = static_cast<std::uint16_t>(sign_bit | mantissa);
        }
        else
        {
            // Number is too large, assign infinite and set sign bit
            h = static_cast<std::uint16_t>(sign_bit | inf_value);
        }
    }
    else
    {
        // Infinite numbers have a zero mantissa (check original mantissa)
        if (!(bits & 0x007f'ffff))
        {
            h = static_cast<std::uint16_t>(sign_bit | inf_value);
        }
        else
        {
            // Must be a NaN value
            h = static_cast<std::uint16_t>(sign_bit | nan_value);
        }
    }

    return h;
}

/*
 *  HalfFloatToFloat
 *
 *  Description:
 *      This function will convert a 16-bit half-precision floating point value
 *      to a 32-bit single-precision floating point value.
 *
 *  Parameters:
 *      h [in]
 *          The half-precision floating point value to convert, stored in a
 *          16-bit unsigned integer field.
 *
 *  Returns:
 *      Returns the single-precision floating point value.
 *
 *  Comments:
 *      None.
 */
constexpr float HalfFloatToFloat(const std::uint16_t h)
{
    constexpr std::uint32_t nan_value = 0x7FC0'0000; // qNaN
    constexpr std::uint32_t inf_value = 0x7F80'0000;
    std::uint32_t bits{};
    std::uint32_t sign_bit{};
    std::uint32_t exponent{};
    std::uint32_t mantissa{};

    // Ensure values are the right number of octets
    static_assert(sizeof(float) == 4);
    static_assert(sizeof(bits) == 4);
    static_assert(sizeof(h) == 2);

    // Store the half float in a bit array 32-bits long
    bits = static_cast<std::uint32_t>(h);

    // Extract the sign bit, shifting into position
    sign_bit = (bits & 0x8000) << 16;

    // Extract the exponent, shifting all the way to the right
    exponent = (bits & 0x7c00) >> 10;

    // Extact the mantissa, shifting into final position
    mantissa = (bits & 0x03ff) << 13;

    // If this isn't an infinite value or NAN
    if (exponent != 0b0001'1111)
    {
        // If the exponent is non-zero, this is not a zero or subnormal number
        if (exponent)
        {
            // Adjust the exponent bias for single-precision floats
            exponent += 112;

            // Compose a float value from the components
            bits = sign_bit | (exponent << 23) | mantissa;
        }
        else
        {
            // A zero mantissa is encoded as all zeros (except sign bit)
            if (!mantissa)
            {
                bits = sign_bit;
            }
            else
            {
                // A subnormal number has an implicit leading 0, but this can be
                // promoted to a normal value that has an implicit leading 1 by
                // shifting bits, decremeting the exponent, and discarding the
                // resulting leading 1 bit

                // Get a copy of the original mantissa in the lower 10 bits
                std::uint32_t m = bits & 0x03ff;

                // Bit position of the Msb (9 <= b <= 0 in this case)
                std::uint32_t b = 0;

                // Find the mantissa's Msb bit position using a binary search,
                // looking only in the lower 10 bits
                if (m >= ((std::uint32_t) 1) << 8) b += 8, m >>= 8;
                if (m >= ((std::uint32_t) 1) << 4) b += 4, m >>= 4;
                if (m >= ((std::uint32_t) 1) << 2) b += 2, m >>= 2;
                if (m >= ((std::uint32_t) 1) << 1) b += 1, m >>= 1;

                // Adjust the mantissa (stripping leading 1 bit), which may
                // result in a zero-valued mantissa
                mantissa = (mantissa << (10 - b)) & 0x007f'ffff;

                // Set the exponent, adjusting the bias as appropriate
                exponent = 113 - (10 - b);

               // Compose a float value from the components
               bits = sign_bit | (exponent << 23) | mantissa;
            }
        }
    }
    else
    {
        // Infinite numbers have a zero mantissa (check original mantissa)
        if (!(bits & 0x03ff))
        {
            bits = sign_bit | inf_value;
        }
        else
        {
            // Must be a NaN value
            bits = sign_bit | nan_value;
        }
    }

    // Convert the bits to a floating point representation
    union
    {
        std::uint32_t i;
        float f;
    } u{};
    u.i = bits;

    return u.f;
}

} // namespace gs

#endif // HALF_FLOAT_CPP
