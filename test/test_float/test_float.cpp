/*
 *  test_float.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test to verify that floating point numbers are
 *      represented internally as per IEEE-754.  If these tests fail, then
 *      code in GSE that assumes the IEEE-754 format needs additional work
 *      to accommodate this computing platform.
 *
 *  Portability Issues:
 *      None.
 */

#include <cstring>
#include "gtest/gtest.h"

namespace {

    // The fixture for testing floating point formats
    class FloatTest : public ::testing::Test
    {
        protected:
            FloatTest()
            {
            }

            ~FloatTest()
            {
            }
    };

    // Test that float is a proper Binary32 format
    TEST_F(FloatTest, Binary32)
    {
        float f = 3.14159265f;

        // Ensure a float is 32 bits
        ASSERT_EQ(sizeof(f), 4);

        // Create a 32-bit unsigned integer
        std::uint32_t i;

        // Ensure the integer is, indeed, 4 octets
        ASSERT_EQ(sizeof(i), sizeof(f));

        // Copy the float into the unsigned integer memory
        std::memcpy(&i, &f, sizeof(i));

        // Compare the expected bit representation
        ASSERT_EQ(i, 0x40490fdb);
    };

    // Test that float is a proper Binary64 format
    TEST_F(FloatTest, Binary64)
    {
        double f = 3.141592653589793;

        // Ensure a float is 32 bits
        ASSERT_EQ(sizeof(f), 8);

        // Create a 32-bit unsigned integer
        std::uint64_t i;

        // Ensure the integer is, indeed, 4 octets
        ASSERT_EQ(sizeof(i), sizeof(f));

        // Copy the float into the unsigned integer memory
        std::memcpy(&i, &f, sizeof(i));

        // Compare the expected bit representation
        ASSERT_EQ(i, 0x400921fb54442d18);
    };

} // namespace
