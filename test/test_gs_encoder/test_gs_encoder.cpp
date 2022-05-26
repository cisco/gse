/*
 *  test_gs_encoder.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test to the Game State Encoder to ensure that objects
 *      are properly serialized into the data buffer.
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
#include <cstddef>
#include <variant>
#include <vector>
#include "gtest/gtest.h"
#include "gs_types.h"
#include "gs_encoder.h"
#include "data_buffer.h"

namespace {

    // The fixture for testing the Game State Encoder
    class GSEncoderTest : public ::testing::Test
    {
        public:
            GSEncoderTest() : data_buffer(1500)
            {
            }

            ~GSEncoderTest()
            {
            }

        protected:
            gs::Encoder encoder;
            gs::DataBuffer<> data_buffer;
    };

    // Test vector B.1
    TEST_F(GSEncoderTest, Test_Vector_B1)
    {
        std::vector<std::uint8_t> expected =
        {
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        gs::Head1 head{};

        head.id.value = 0;
        head.time = 0x0500;
        head.location.x = 1.1f;
        head.location.y = 0.2f;
        head.location.z = 30.0f;
        head.location.vx.value = 0.0f;
        head.location.vy.value = 0.0f;
        head.location.vz.value = 0.0f;
        head.rotation.si.value = 0.0f;
        head.rotation.sj.value = 0.0f;
        head.rotation.sk.value = 0.0f;
        head.rotation.ei.value = 0.0f;
        head.rotation.ej.value = 0.0f;
        head.rotation.ek.value = 0.0f;

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, head),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    };

    TEST_F(GSEncoderTest, Test_Vector_B1_ipd)
    {
        std::vector<std::uint8_t> expected =
        {
            0x01, 0x27, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,

            // ipd
            0xc0, 0x80, 0x02, 0x02, 0x42, 0x48
        };

        gs::Head1 head{};

        head.id.value = 0;
        head.time = 0x0500;
        head.location.x = 1.1f;
        head.location.y = 0.2f;
        head.location.z = 30.0f;
        head.location.vx.value = 0.0f;
        head.location.vy.value = 0.0f;
        head.location.vz.value = 0.0f;
        head.rotation.si.value = 0.0f;
        head.rotation.sj.value = 0.0f;
        head.rotation.sk.value = 0.0f;
        head.rotation.ei.value = 0.0f;
        head.rotation.ej.value = 0.0f;
        head.rotation.ek.value = 0.0f;
        head.ipd = gs::HeadIPD1{3.140625};

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, head),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    };

    // Test vector B.1, encoding as UnknownObject type
    TEST_F(GSEncoderTest, Test_Vector_B1_Unknown)
    {
        std::vector<std::uint8_t> expected =
        {
            0x20, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        gs::UnknownObject unknown{};

        // Set the tag value
        unknown.tag = {0x20};

        // Populate the expected buffer, skipping the length field as it
        // will be embodied in the Blob vector
        for (std::size_t i = 2; i < expected.size(); i++)
        {
            unknown.data.push_back(expected[i]);
        }

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, unknown),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    };

    // Test vector B.1, encoding as variant
    TEST_F(GSEncoderTest, Test_Vector_B1_Variant)
    {
        std::vector<std::uint8_t> expected =
        {
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        gs::Head1 head{};

        head.id.value = 0;
        head.time = 0x0500;
        head.location.x = 1.1f;
        head.location.y = 0.2f;
        head.location.z = 30.0f;
        head.location.vx.value = 0.0f;
        head.location.vy.value = 0.0f;
        head.location.vz.value = 0.0f;
        head.rotation.si.value = 0.0f;
        head.rotation.sj.value = 0.0f;
        head.rotation.sk.value = 0.0f;
        head.rotation.ei.value = 0.0f;
        head.rotation.ej.value = 0.0f;
        head.rotation.ek.value = 0.0f;

        // Store the object as a variant
        gs::GSObject object = head;

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, object),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    };

    // Test Mesh1 encoding
    TEST_F(GSEncoderTest, Test_Mesh1)
    {
        std::vector<std::uint8_t> expected =
        {
            // Mesh1 tag
            0xc0, 0x80, 0x00,

            // Octets to follow
            0x32,

            // Object ID
            0x1b,

            // Number of loc1 elements
            0x02,

            // Loc1 element
            0x3f, 0x80, 0x00, 0x00,
            0x40, 0x00, 0x00, 0x00,

            // Loc1 element
            0x40, 0x40, 0x00, 0x00,
            0x3f, 0x80, 0x00, 0x00,

            // Loc1 element
            0x40, 0x00, 0x00, 0x00,
            0x40, 0x40, 0x00, 0x00,

            // Number of normals
            0x03,

            // Norm1 element
            0x42, 0x48,
            0xBC, 0x00,
            0x7B, 0xFF,

            // Norm1 element
            0x42, 0x48,
            0xBC, 0x00,
            0x42, 0x48,

            // Norm1 element
            0x42, 0x48,
            0xBC, 0x00,
            0x7B, 0xFF,

            // Number of textures
            0x01,

            // Texture
            0x01, 0x80, 0x81,

            // Number of triangles
            0x00
        };

        gs::Mesh1 mesh{};

        mesh.id.value = 0x1b;
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});

        mesh.normals.push_back({{3.14f}, {-1.0f}, {65504.0f}});
        mesh.normals.push_back({{3.14f}, {-1.0f}, {3.14f}});
        mesh.normals.push_back({{3.14f}, {-1.0f}, {65504.0f}});

        mesh.textures.push_back({{1}, {129}});

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, mesh),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    }

    // Test encoding vector of variants
    TEST_F(GSEncoderTest, Test_Variant_Vector)
    {
        gs::GSObjects objects;

        std::vector<std::uint8_t> expected =
        {
            // Head1
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,

            // Mesh1 tag
            0xc0, 0x80, 0x00, 0x32, 0x1b, 0x02, 0x3f, 0x80,
            0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40,
            0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00,
            0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x03, 0x42,
            0x48, 0xBC, 0x00, 0x7B, 0xFF, 0x42, 0x48, 0xBC,
            0x00, 0x42, 0x48, 0x42, 0x48, 0xBC, 0x00, 0x7B,
            0xFF, 0x01, 0x01, 0x80, 0x81, 0x00,

            // Head1 - another copy
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        gs::Head1 head{};

        head.id.value = 0;
        head.time = 0x0500;
        head.location.x = 1.1f;
        head.location.y = 0.2f;
        head.location.z = 30.0f;
        head.location.vx.value = 0.0f;
        head.location.vy.value = 0.0f;
        head.location.vz.value = 0.0f;
        head.rotation.si.value = 0.0f;
        head.rotation.sj.value = 0.0f;
        head.rotation.sk.value = 0.0f;
        head.rotation.ei.value = 0.0f;
        head.rotation.ej.value = 0.0f;
        head.rotation.ek.value = 0.0f;

        // Assign Mesh values from above example
        gs::Mesh1 mesh;

        mesh.id.value = 0x1b;
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});

        mesh.normals.push_back({{3.14f}, {-1.0f}, {65504.0f}});
        mesh.normals.push_back({{3.14f}, {-1.0f}, {3.14f}});
        mesh.normals.push_back({{3.14f}, {-1.0f}, {65504.0f}});

        mesh.textures.push_back({{1}, {129}});

        // Store the object as a variant a few times
        objects.push_back(head);
        objects.push_back(mesh);
        objects.push_back(head);

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, objects),
                  std::make_pair(std::size_t(3), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    };

    // Test encoding vector of variants with a short buffer
    TEST_F(GSEncoderTest, Test_Variant_Vector_Too_Short)
    {
        gs::GSObjects objects;

        std::vector<std::uint8_t> expected =
        {
            // Head1
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,

            // Mesh1 tag
            0xc0, 0x80, 0x00, 0x32, 0x1b, 0x02, 0x3f, 0x80,
            0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40,
            0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00,
            0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x03, 0x42,
            0x48, 0xBC, 0x00, 0x7B, 0xFF, 0x42, 0x48, 0xBC,
            0x00, 0x42, 0x48, 0x42, 0x48, 0xBC, 0x00, 0x7B,
            0xFF, 0x01, 0x01, 0x80, 0x81, 0x00,

            // Head1 - another copy
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        gs::Head1 head{};

        head.id.value = 0;
        head.time = 0x0500;
        head.location.x = 1.1f;
        head.location.y = 0.2f;
        head.location.z = 30.0f;
        head.location.vx.value = 0.0f;
        head.location.vy.value = 0.0f;
        head.location.vz.value = 0.0f;
        head.rotation.si.value = 0.0f;
        head.rotation.sj.value = 0.0f;
        head.rotation.sk.value = 0.0f;
        head.rotation.ei.value = 0.0f;
        head.rotation.ej.value = 0.0f;
        head.rotation.ek.value = 0.0f;

        // Assign Mesh values from above example
        gs::Mesh1 mesh;

        mesh.id.value = 0x1b;
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});

        mesh.normals.push_back({{3.14f}, {-1.0f}, {65504.0f}});
        mesh.normals.push_back({{3.14f}, {-1.0f}, {3.14f}});
        mesh.normals.push_back({{3.14f}, {-1.0f}, {65504.0f}});

        mesh.textures.push_back({{1}, {129}});

        // Store the object as a variant a few times
        objects.push_back(head);
        objects.push_back(mesh);
        objects.push_back(head);

        // Define a short data buffer
        gs::DataBuffer<> db(100);

        // Check the expected encoded length, which should only be the
        // first too objects, aa the buffer is too short for all 3
        ASSERT_EQ(encoder.Encode(db, objects),
                  std::make_pair(std::size_t(2), std::size_t(89)));

        // Verify the buffer contents
        for (std::size_t i = 0; i < 35; i++)
        {
            ASSERT_EQ(db[i], expected[i]);
        }
    };

    // Test encoding Hand1
    TEST_F(GSEncoderTest, Test_Hand1)
    {
        std::vector<std::uint8_t> expected =
        {
            0x02, 0x22, 0x0c, 0x05, 0x00, 0x01, 0x3f, 0x8c,
            0xcc, 0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0,
            0x00, 0x00, 0x42, 0x48, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x42, 0x48
        };

        gs::Hand1 hand1{};

        hand1.id.value = 12;
        hand1.time = 0x0500;
        hand1.left = true;
        hand1.location.x = 1.1f;
        hand1.location.y = 0.2f;
        hand1.location.z = 30.0f;
        hand1.location.vx.value = 3.140625f;
        hand1.location.vy.value = 0.0f;
        hand1.location.vz.value = 0.0f;
        hand1.rotation.si.value = 0.0f;
        hand1.rotation.sj.value = 0.0f;
        hand1.rotation.sk.value = 0.0f;
        hand1.rotation.ei.value = 0.0f;
        hand1.rotation.ej.value = 0.0f;
        hand1.rotation.ek.value = 3.140625f;

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, hand1),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    }

    // Test encoding Hand2
    TEST_F(GSEncoderTest, Test_Hand2)
    {
        std::vector<std::uint8_t> expected =
        {
            // tag
            0xc0, 0x80, 0x01,

            // Length
            0x80, 0xb8,

            // id
            0x0c,

            // time
            0x05, 0x00,

            // left
            0x01,

            // location
            0x3f, 0x8c, 0xcc, 0xcd, 0x3e, 0x4c, 0xcc, 0xcd,
            0x41, 0xf0, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00,
            0x00, 0x00,

            // rotoration
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x42, 0x48,

            // wrist
            0x00, 0x00, 0x42, 0x48, 0x00, 0x00,

            // thumb
            0x00, 0x00, 0x42, 0x48, 0x00, 0x00, 0x00, 0x00,
            0x42, 0x48, 0x00, 0x00, 0x00, 0x00, 0x42, 0x48,
            0x00, 0x00, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00,

            // index
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            // middle
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            // ring
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            // pinky
            0x42, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        gs::Hand2 hand2{};

        hand2.id.value = 12;
        hand2.time = 0x0500;
        hand2.left = true;
        hand2.location.x = 1.1f;
        hand2.location.y = 0.2f;
        hand2.location.z = 30.0f;
        hand2.location.vx.value = 3.140625f;
        hand2.location.vy.value = 0.0f;
        hand2.location.vz.value = 0.0f;
        hand2.rotation.si.value = 0.0f;
        hand2.rotation.sj.value = 0.0f;
        hand2.rotation.sk.value = 0.0f;
        hand2.rotation.ei.value = 0.0f;
        hand2.rotation.ej.value = 0.0f;
        hand2.rotation.ek.value = 3.140625f;
        hand2.wrist.tx.value = 0.0f;
        hand2.wrist.ty.value = 3.140625f;
        hand2.wrist.tz.value = 0.0f;
        hand2.thumb.tip.tx.value = 0.0f;
        hand2.thumb.tip.ty.value = 3.140625f;
        hand2.thumb.tip.tz.value = 0.0f;
        hand2.thumb.ip.tx.value = 0.0f;
        hand2.thumb.ip.ty.value = 3.140625f;
        hand2.thumb.ip.tz.value = 0.0f;
        hand2.thumb.mcp.tx.value = 0.0f;
        hand2.thumb.mcp.ty.value = 3.140625f;
        hand2.thumb.mcp.tz.value = 0.0f;
        hand2.thumb.cmc.tx.value = 0.0f;
        hand2.thumb.cmc.ty.value = 3.140625f;
        hand2.thumb.cmc.tz.value = 0.0f;

        hand2.pinky.tip.tx.value = 3.140625f;

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, hand2),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    }

    // Test encoding Object1.
    TEST_F(GSEncoderTest, Test_Object1)
    {
        std::vector<std::uint8_t> expected =
        {
            // tag
            0x03,

            // Length
            0x21,

            // id
            0x0c,

            // time
            0x05, 0x00,

            // location
            0x3f, 0x80, 0x00, 0x00, 0x40, 0x00,
            0x00, 0x00, 0x40, 0x40, 0x00, 0x00,

            // rotation
            0x44, 0x00, 0x45, 0x00, 0x46, 0x00,

            // scale
            0x40, 0xE0, 0x00, 0x00, 0x41, 0x00,
            0x00, 0x00, 0x41, 0x10, 0x00, 0x00,
        };

        gs::Object1 object1{};

        object1.id.value = 12;
        object1.time = 0x0500;
        object1.position.x = 1.0f;
        object1.position.y = 2.0f;
        object1.position.z = 3.0f;
        object1.rotation.i.value = 4.0f;
        object1.rotation.j.value = 5.0f;
        object1.rotation.k.value = 6.0f;
        object1.scale.x = 7.0f;
        object1.scale.y = 8.0f;
        object1.scale.z = 9.0f;

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, object1),
            std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ(data_buffer[i], expected[i]);
        }
    }

} // namespace
