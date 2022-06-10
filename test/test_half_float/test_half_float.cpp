/*
 *  test_half_float.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test to verify that conversion between IEEE floating
 *      point and half float values is performed correctly.
 *
 *  Portability Issues:
 *      None.
 *
 *  License:
 *      BSD 2-Clause License
 *
 *      Copyright (c) 2022, Cisco Systems
 *      All rights reserved.
 *
 *      Redistribution and use in source and binary forms, with or without
 *      modification, are permitted provided that the following conditions are
 *      met:
 *
 *      1. Redistributions of source code must retain the above copyright
 *         notice, this list of conditions and the following disclaimer.
 *
 *      2. Redistributions in binary form must reproduce the above copyright
 *         notice, this list of conditions and the following disclaimer in the
 *         documentation and/or other materials provided with the distribution.
 *
 *      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 *      IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 *      TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *      PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *      HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *      LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *      DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *      THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *      (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *      OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstring>
#include <cstdint>
#include <cmath>
#include "gtest/gtest.h"
#include "half_float.h"

namespace {

    // The fixture for testing floating point / half float conversion
    class HalfFloatTest : public ::testing::Test
    {
        protected:
            HalfFloatTest() : h{}, f{}
            {
                static_assert(sizeof(f) == 4);
                static_assert(sizeof(h) == 2);
            }

            ~HalfFloatTest() = default;

        protected:
            std::uint16_t h;
            float f;
    };

    /////////////////////////////////
    // Float to Half Float Tests
    /////////////////////////////////

    // Test float to half float value for 0
    TEST_F(HalfFloatTest, float_to_half_0)
    {
        f = 0.0f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0000'0000'0000'0000);
    };

    // Test float to half float value for 0
    TEST_F(HalfFloatTest, float_to_half_neg_0)
    {
        f = -0.0f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b1000'0000'0000'0000);
    };

    // Test float to half float value for 1
    TEST_F(HalfFloatTest, float_to_half_1)
    {
        f = 1.0f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0011'1100'0000'0000);
    };

    // Test float to half float value for -1
    TEST_F(HalfFloatTest, float_to_half_neg_1)
    {
        f = -1.0f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b1011'1100'0000'0000);
    };

    // Test float to half float value for 3.14
    TEST_F(HalfFloatTest, float_to_half_3_14)
    {
        f = 3.14f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0100'0010'0100'1000);
    };

    // Test float to half float value for 0.000000059604645
    TEST_F(HalfFloatTest, float_to_half_0_000000059604645)
    {
        f = 0.000000059604645f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0000'0000'0000'0001);
    };

    // Test float to half float value for 0.000060975552
    TEST_F(HalfFloatTest, float_to_half_0_000060975552)
    {
        f = 0.000060975552f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0000'0011'1111'1111);
    };

    // Test float to half float value for 0.00006103515625
    TEST_F(HalfFloatTest, float_to_half_0_00006103515625)
    {
        f = 0.00006103515625f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0000'0100'0000'0000);
    };

    // Test float to half float value for 0.33325195
    TEST_F(HalfFloatTest, float_to_half_0_33325195)
    {
        f = 0.33325195f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0011'0101'0101'0101);
    };

    // Test float to half float value for 0.99951172
    TEST_F(HalfFloatTest, float_to_half_0_99951172)
    {
        f = 0.99951172f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0011'1011'1111'1111);
    };

    // Test float to half float value for 1.00097656
    TEST_F(HalfFloatTest, float_to_half_1_00097656)
    {
        f = 1.00097656f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0011'1100'0000'0001);
    };

    // Test float to half float value for 65504
    TEST_F(HalfFloatTest, float_to_half_65504)
    {
        f = 65504.0f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0111'1011'1111'1111);
    };

    // Test float to half float value for infinity
    TEST_F(HalfFloatTest, float_to_half_infinity)
    {
        f = HUGE_VALF;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0111'1100'0000'0000);
    };

    // Test float to half float value for -infinity
    TEST_F(HalfFloatTest, float_to_half_neg_infinity)
    {
        f = -HUGE_VALF;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b1111'1100'0000'0000);
    };

    // Test float to half float value for -2
    TEST_F(HalfFloatTest, float_to_half_neg_2)
    {
        f = -2.0f;

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b1100'0000'0000'0000);
    };

    // Test float to half float value for NAN
    TEST_F(HalfFloatTest, float_to_half_NAN)
    {
        f = std::nanf("");

        h = gs::FloatToHalfFloat(f);

        // Compare the expected bit representation
        ASSERT_EQ(h, 0b0111'1110'0000'0000);
    };

    /////////////////////////////////
    // Half Float to Float Tests
    /////////////////////////////////

    // Test half float to float value for 0
    TEST_F(HalfFloatTest, half_to_float_0)
    {
        h = 0b0000'0000'0000'0000;

        f = gs::HalfFloatToFloat(h);

        ASSERT_EQ(f, 0.0);
    };

    // Test half float to float value for -0
    TEST_F(HalfFloatTest, half_to_float_neg_0)
    {
        h = 0b1000'0000'0000'0000;

        f = gs::HalfFloatToFloat(h);

        ASSERT_EQ(f, -0.0);
    };

    // Test half float to float value for 1
    TEST_F(HalfFloatTest, half_to_float_1)
    {
        h = 0b0011'1100'0000'0000;

        f = gs::HalfFloatToFloat(h);

        ASSERT_EQ(f, 1.0f);
    };

    // Test half float to float value for -1
    TEST_F(HalfFloatTest, half_to_float_neg_1)
    {
        h = 0b1011'1100'0000'0000;

        f = gs::HalfFloatToFloat(h);

        ASSERT_EQ(f, -1.0);
    };

    // Test half float to float value for 3.14
    TEST_F(HalfFloatTest, half_to_float_3_14)
    {
        h = 0b0100'0010'0100'1000;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0100'0000'0100'1001'0000'0000'0000'0000
        ASSERT_EQ(f, 3.140625);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0100'0010'0100'1000);
    };

    // Test half float to float value for 0.000000059604645
    TEST_F(HalfFloatTest, half_to_float_0_000000059604645)
    {
        h = 0b0000'0000'0000'0001;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0011'0011'1000'0000'0000'0000'0000'0000
        ASSERT_EQ(f, 5.9604644775390625E-8);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0000'0000'0000'0001);
    };

    // Test half float to float value for 0.000060975552
    TEST_F(HalfFloatTest, half_to_float_0_000060975552)
    {
        h = 0b0000'0011'1111'1111;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0011'1000'0111'1111'1100'0000'0000'0000
        ASSERT_EQ(f, 0.000060975551605224609375);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0000'0011'1111'1111);
    };

    // Test half float to float value for 0.00006103515625
    TEST_F(HalfFloatTest, half_to_float_0_00006103515625)
    {
        h = 0b0000'0100'0000'0000;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0011'1000'1000'0000'0000'0000'0000'0000
        ASSERT_EQ(f, 0.00006103515625);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0000'0100'0000'0000);
    };

    // Test half float to float value for 0.33325195
    TEST_F(HalfFloatTest, half_to_float_0_33325195)
    {
        h = 0b0011'0101'0101'0101;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0011'1110'1010'1010'1010'0000'0000'0000
        ASSERT_EQ(f, 0.333251953125);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0011'0101'0101'0101);
    };

    // Test half float to float value for 0.99951172
    TEST_F(HalfFloatTest, half_to_float_0_99951172)
    {
        h = 0b0011'1011'1111'1111;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0011'1111'0111'1111'1110'0000'0000'0000
        ASSERT_EQ(f, 0.99951171875);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0011'1011'1111'1111);
    };

    // Test half float to float value for 1.00097656
    TEST_F(HalfFloatTest, half_to_float_1_00097656)
    {
        h = 0b0011'1100'0000'0001;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0011'1111'1000'0000'0010'0000'0000'0000
        ASSERT_EQ(f, 1.0009765625);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0011'1100'0000'0001);
    };

    // Test half float to float value for 65504
    TEST_F(HalfFloatTest, half_to_float_65504)
    {
        h = 0b0111'1011'1111'1111;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0100'0111'0111'1111'1110'0000'0000'0000
        ASSERT_EQ(f, 65504.0);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0111'1011'1111'1111);
    };

    // Test half float to float value for infinity
    TEST_F(HalfFloatTest, half_to_float_infinity)
    {
        h = 0b0111'1100'0000'0000;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0111'1111'1000'0000'0000'0000'0000'0000
        ASSERT_EQ(f, HUGE_VALF);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0111'1100'0000'0000);
    };

    // Test half float to float value for negative infinity
    TEST_F(HalfFloatTest, half_to_float_neg_infinity)
    {
        h = 0b1111'1100'0000'0000;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0111'1111'1000'0000'0000'0000'0000'0000
        ASSERT_EQ(f, -HUGE_VALF);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b1111'1100'0000'0000);
    };

    // Test half float to float value for -2
    TEST_F(HalfFloatTest, half_to_float_neg_2)
    {
        h = 0b1100'0000'0000'0000;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b1100'0000'0000'0000'0000'0000'0000'0000
        ASSERT_EQ(f, -2.0);

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b1100'0000'0000'0000);
    };

    // Test half float to float value for NaN
    TEST_F(HalfFloatTest, half_to_float_NaN)
    {
        h = 0b0111'1110'0000'0000;

        f = gs::HalfFloatToFloat(h);

        // This would expand to 0b0111'1111'1100'0000'0000'0000'0000'0000
        ASSERT_TRUE(std::isnan(f));

        // Do the reverse and compare again
        h = gs::FloatToHalfFloat(f);
        ASSERT_EQ(h, 0b0111'1110'0000'0000);
    };

} // namespace
