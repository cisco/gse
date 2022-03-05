/*
 *  test_gs_api.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test to the Game State Decoder C API.
 *
 *  Portability Issues:
 *      None.
 */

#include <cstring>
#include <cstdint>
#include <vector>
#include "gtest/gtest.h"
#include "gs_api.h"

namespace {

    // The fixture for testing the Game State Decoder
    class GSAPITest : public ::testing::Test
    {
        public:
            GSAPITest()
            {
                std::memset(data_buffer, 0, sizeof(data_buffer));
            }

            ~GSAPITest()
            {
            }

            void PerformEncodeTest(std::vector<std::uint8_t> &expected,
                                   GS_Object *objects,
                                   unsigned num_objects = 1)
            {
                // Create the encoder context
                GS_Encoder_Context *context;
                ASSERT_EQ(GSEncoderInit(&context, data_buffer, 1500), 0);

                // Encode the objects
                for (std::size_t i = 0; i < num_objects; i++)
                {
                    ASSERT_EQ(GSEncodeObject(context, &objects[i]), 1);
                }

                // Check the encoded length
                ASSERT_EQ(GSEncoderDataLength(context), expected.size());

                // Destroy the encoder context
                ASSERT_EQ(GSEncoderDestroy(context), 0);

                // Verify the buffer contents
                for (std::size_t i = 0; i < expected.size(); i++)
                {
                    ASSERT_EQ(data_buffer[i], expected[i]);
                }
            }

            // Note: these tests assume encoding logic is correct
            void PerformDecodeTest(std::vector<std::uint8_t> &expected,
                                   unsigned num_objects = 1)
            {
                // Create the decoder context
                GS_Decoder_Context *context;
                ASSERT_EQ(GSDecoderInit(&context,
                                        &expected[0],
                                        expected.size()),
                          0);

                // Decode the three objects
                GS_Object *objects = new GS_Object[num_objects];
                for (std::size_t i = 0; i < num_objects; i++)
                {
                    ASSERT_EQ(GSDecodeObject(context, &objects[i]), 1);
                }

                // Re-encode the objects
                {
                    // Create the encoder context
                    GS_Encoder_Context *context;
                    ASSERT_EQ(GSEncoderInit(&context, data_buffer, 1500), 0);

                    // Encode the objects
                    for (std::size_t i = 0; i < num_objects; i++)
                    {
                        ASSERT_EQ(GSEncodeObject(context, &objects[i]), 1);
                    }

                    // Check the encoded length
                    ASSERT_EQ(GSEncoderDataLength(context), expected.size());

                    // Destroy the encoder context
                    ASSERT_EQ(GSEncoderDestroy(context), 0);

                    // Verify the buffer contents
                    for (std::size_t i = 0; i < expected.size(); i++)
                    {
                        ASSERT_EQ(data_buffer[i], expected[i]);
                    }
                }

                // Verify 0 return on trying to decode beyond last object
                ASSERT_EQ(GSDecodeObject(context, &objects[0]), 0);

                // Destroy the encoder context
                ASSERT_EQ(GSDecoderDestroy(context), 0);

                // Free the allocated memory
                delete [] objects;
            }

        protected:
            unsigned char data_buffer[1500];
    };

    TEST_F(GSAPITest, Test_Init_Destroy)
    {
        GS_Encoder_Context *context;

        GSEncoderInit(&context, data_buffer, sizeof(data_buffer));
        GSEncoderDestroy(context);
    };

    TEST_F(GSAPITest, Test_Encode_Head)
    {
        std::vector<std::uint8_t> expected =
        {
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        GS_Object object{};

        object.type = GS_Tag_Head1;
        object.u.head1.id = 0;
        object.u.head1.time = 0x0500;
        object.u.head1.location.x = 1.1f;
        object.u.head1.location.y = 0.2f;
        object.u.head1.location.z = 30.0f;
        object.u.head1.location.vx = 0.0f;
        object.u.head1.location.vy = 0.0f;
        object.u.head1.location.vz = 0.0f;
        object.u.head1.rotation.si = 0.0f;
        object.u.head1.rotation.sj = 0.0f;
        object.u.head1.rotation.sk = 0.0f;
        object.u.head1.rotation.ei = 0.0f;
        object.u.head1.rotation.ej = 0.0f;
        object.u.head1.rotation.ek = 0.0f;

        PerformEncodeTest(expected, &object);
    }

    TEST_F(GSAPITest, Test_Encode_Head_IPD)
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

        GS_Object object{};

        object.type = GS_Tag_Head1;

        object.u.head1.id = 0;
        object.u.head1.time = 0x0500;
        object.u.head1.location.x = 1.1f;
        object.u.head1.location.y = 0.2f;
        object.u.head1.location.z = 30.0f;
        object.u.head1.location.vx = 0.0f;
        object.u.head1.location.vy = 0.0f;
        object.u.head1.location.vz = 0.0f;
        object.u.head1.rotation.si = 0.0f;
        object.u.head1.rotation.sj = 0.0f;
        object.u.head1.rotation.sk = 0.0f;
        object.u.head1.rotation.ei = 0.0f;
        object.u.head1.rotation.ej = 0.0f;
        object.u.head1.rotation.ek = 0.0f;
        object.u.head1.ipd_present = true;
        object.u.head1.ipd.ipd = 3.140625;

        PerformEncodeTest(expected, &object);
    }

    // Test vector B.1, encoding as UnknownObject type
    TEST_F(GSAPITest, Test_Encode_Unknown)
    {
        std::vector<std::uint8_t> expected =
        {
            0x20, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        GS_Object object{};

        // Set the tag value
        object.type = 0x20;
        object.u.unknown_object.tag = object.type;
        object.u.unknown_object.data_length = expected.size() - 2;
        object.u.unknown_object.data = &expected[2];

        PerformEncodeTest(expected, &object);
    };

    // Test encoding multiple objects
    TEST_F(GSAPITest, Test_Encode_Multiple)
    {
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

        // Define 3 objects
        GS_Object objects[3]{};

        // Assign Head1 object
        GS_Object &head = objects[0];
        head.type = GS_Tag_Head1;
        head.u.head1.id = 0;
        head.u.head1.time = 0x0500;
        head.u.head1.location.x = 1.1f;
        head.u.head1.location.y = 0.2f;
        head.u.head1.location.z = 30.0f;
        head.u.head1.location.vx = 0.0f;
        head.u.head1.location.vy = 0.0f;
        head.u.head1.location.vz = 0.0f;
        head.u.head1.rotation.si = 0.0f;
        head.u.head1.rotation.sj = 0.0f;
        head.u.head1.rotation.sk = 0.0f;
        head.u.head1.rotation.ei = 0.0f;
        head.u.head1.rotation.ej = 0.0f;
        head.u.head1.rotation.ek = 0.0f;

        // Assign Mesh1 values from above example
        GS_Object &mesh = objects[1];
        mesh.type = GS_Tag_Mesh1;
        mesh.u.mesh1.id = 0x1b;
        GS_Loc1 vertices[2];
        vertices[0].x = 1.0f;
        vertices[0].y = 2.0f;
        vertices[0].z = 3.0f;
        vertices[1].x = 1.0f;
        vertices[1].y = 2.0f;
        vertices[1].z = 3.0f;
        mesh.u.mesh1.num_vertices = 2;
        mesh.u.mesh1.vertices = vertices;
        GS_Norm1 normals[3];
        normals[0].x = 3.14f;
        normals[0].y = -1.0f;
        normals[0].z = 65504.0f;
        normals[1].x = 3.14f;
        normals[1].y = -1.0f;
        normals[1].z = 3.14f;
        normals[2].x = 3.14f;
        normals[2].y = -1.0f;
        normals[2].z = 65504.0f;
        mesh.u.mesh1.num_normals = 3;
        mesh.u.mesh1.normals = normals;
        GS_TextureUV1 textures[1];
        textures[0].u = 1;
        textures[0].v = 129;
        mesh.u.mesh1.num_textures = 1;
        mesh.u.mesh1.textures = &textures[0];

        // The third object is just a copy of the first
        std::memcpy(&objects[2], &head, sizeof(GS_Object));

        PerformEncodeTest(expected, objects, 3);
    };

    // Test encoding Hand2
    TEST_F(GSAPITest, Test_Encode_Hand2)
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

        GS_Object object{};

        object.type = GS_Tag_Hand2;
        object.u.hand2.id = 12;
        object.u.hand2.time = 0x0500;
        object.u.hand2.left = true;
        object.u.hand2.location.x = 1.1f;
        object.u.hand2.location.y = 0.2f;
        object.u.hand2.location.z = 30.0f;
        object.u.hand2.location.vx = 3.140625f;
        object.u.hand2.location.vy = 0.0f;
        object.u.hand2.location.vz = 0.0f;
        object.u.hand2.rotation.si = 0.0f;
        object.u.hand2.rotation.sj = 0.0f;
        object.u.hand2.rotation.sk = 0.0f;
        object.u.hand2.rotation.ei = 0.0f;
        object.u.hand2.rotation.ej = 0.0f;
        object.u.hand2.rotation.ek = 3.140625f;
        object.u.hand2.wrist.tx = 0.0f;
        object.u.hand2.wrist.ty = 3.140625f;
        object.u.hand2.wrist.tz = 0.0f;

        object.u.hand2.thumb.tip.tx = 0.0f;
        object.u.hand2.thumb.tip.ty = 3.140625f;
        object.u.hand2.thumb.tip.tz = 0.0f;
        object.u.hand2.thumb.ip.tx = 0.0f;
        object.u.hand2.thumb.ip.ty = 3.140625f;
        object.u.hand2.thumb.ip.tz = 0.0f;
        object.u.hand2.thumb.mcp.tx = 0.0f;
        object.u.hand2.thumb.mcp.ty = 3.140625f;
        object.u.hand2.thumb.mcp.tz = 0.0f;
        object.u.hand2.thumb.cmc.tx = 0.0f;
        object.u.hand2.thumb.cmc.ty = 3.140625f;
        object.u.hand2.thumb.cmc.tz = 0.0f;

        object.u.hand2.pinky.tip.tx = 3.140625f;

        PerformEncodeTest(expected, &object);
    }

    TEST_F(GSAPITest, Test_Encode_Object1)
    {
        std::vector<std::uint8_t> expected = 
        {
            0x03, 0x1f, 0x01, 0x3f, 0x80, 0x00, 0x00, 0x40,
            0x00, 0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x44,
            0x00, 0x45, 0x00, 0x46, 0x00, 0x40, 0xe0, 0x00,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x41, 0x10, 0x00,
            0x00
        };

        GS_Object object{};

        object.type = GS_Tag_Object1;
        object.u.object1.id = 1;
        object.u.object1.position.x = 1.0f;
        object.u.object1.position.y = 2.0f;
        object.u.object1.position.z = 3.0f;
        object.u.object1.rotation.i = 4.0f;
        object.u.object1.rotation.j = 5.0f;
        object.u.object1.rotation.k = 6.0f;
        object.u.object1.scale.x = 7.0f;
        object.u.object1.scale.y = 8.0f;
        object.u.object1.scale.z = 9.0f;
        object.u.object1.parent_present = false;

        PerformEncodeTest(expected, &object);
    }

    //
    // Decoding functions rely on the fact the encoding tests pass,
    // as they work by decoding, then re-encoding, and comparing the encoding
    // results.
    //

    TEST_F(GSAPITest, Test_Decode_Head)
    {
        std::vector<std::uint8_t> expected =
        {
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        PerformDecodeTest(expected);
    }

    TEST_F(GSAPITest, Test_Decode_Head_IPD)
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

        PerformDecodeTest(expected);
    }

    TEST_F(GSAPITest, Test_Decode_Unknown)
    {
        std::vector<std::uint8_t> expected =
        {
            0x20, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        PerformDecodeTest(expected);
    }

    TEST_F(GSAPITest, Test_Decode_Multiple)
    {
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

        PerformDecodeTest(expected, 3);
    }

    TEST_F(GSAPITest, Test_Decode_Hand2)
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

        PerformDecodeTest(expected);
    }

    TEST_F(GSAPITest, Test_Decode_Object1)
    {
        std::vector<std::uint8_t> expected{
            // tag
            0x03,

            // Length
            0x1f,

            // id
            0x0c,

            // location
            0x3f, 0x80, 0x00, 0x00, 0x40, 0x00,
            0x00, 0x00, 0x40, 0x40, 0x00, 0x00,

            // rotation
            0x44, 0x00, 0x45, 0x00, 0x46, 0x00,

            // scale
            0x40, 0xE0, 0x00, 0x00, 0x41, 0x00,
            0x00, 0x00, 0x41, 0x10, 0x00, 0x00,
        };

        PerformDecodeTest(expected);
    }

} // namespace
