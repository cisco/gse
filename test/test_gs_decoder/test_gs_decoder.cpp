/*
 *  test_gs_decoder.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test to the Game State Decoder to ensure that objects
 *      are properly serialized into the data buffer.
 *
 *  Portability Issues:
 *      None.
 */

#include <cstring>
#include <cstdint>
#include <memory>
#include <variant>
#include <vector>
#include "gtest/gtest.h"
#include "gs_types.h"
#include "gs_encoder.h"
#include "gs_decoder.h"
#include "data_buffer.h"

namespace {

    // The fixture for testing the Game State Decoder
    class GSDecoderTest : public ::testing::Test
    {
        public:
            GSDecoderTest() : data_buffer(std::make_unique<DataBuffer<>>(1500))
            {
            }

            ~GSDecoderTest()
            {
            }

        protected:
            gs::Encoder encoder;
            gs::Decoder decoder;
            gs::GSObjects decoded_objects;
            std::unique_ptr<DataBuffer<>> data_buffer;
    };

    // Test vector B.1
    TEST_F(GSDecoderTest, Test_Vector_B1)
    {
        // PORTION TAKEN FROM GAME STATE ENCODER TEST

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
            ASSERT_EQ((*data_buffer)[i], expected[i]);
        }

        // PORTION TO TEST THE GAME STATE DECODER

        // Ensure we are at the start of the buffer
        ASSERT_EQ(data_buffer->GetReadLength(), 0);

        // Decode the data buffer
        ASSERT_EQ(decoder.Decode(data_buffer, decoded_objects), data_buffer->GetDataLength());

        // We should have found a single object
        ASSERT_EQ(decoded_objects.size(), 1);

        // Verify that what we got is a Head1 object
        ASSERT_TRUE(std::holds_alternative<gs::Head1>(decoded_objects.front()));

        // Verify the decoded value is the same as the encoded value
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).id.value, head.id.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).time, head.time);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.x, head.location.x);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.y, head.location.y);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.z, head.location.z);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.vx.value, head.location.vx.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.vy.value, head.location.vy.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.vz.value, head.location.vz.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.si.value, head.rotation.si.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.sj.value, head.rotation.sj.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.sk.value, head.rotation.sk.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.ei.value, head.rotation.ei.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.ej.value, head.rotation.ej.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.ek.value, head.rotation.ek.value);
        ASSERT_FALSE(std::get<gs::Head1>(decoded_objects.front()).ipd.has_value());
    };

    TEST_F(GSDecoderTest, Test_Vector_B1_ipd)
    {
        // PORTION TAKEN FROM GAME STATE ENCODER TEST

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
            ASSERT_EQ((*data_buffer)[i], expected[i]);
        }

        // PORTION TO TEST THE GAME STATE DECODER

        // Ensure we are at the start of the buffer
        ASSERT_EQ(data_buffer->GetReadLength(), 0);

        // Decode the data buffer
        ASSERT_EQ(decoder.Decode(data_buffer, decoded_objects), data_buffer->GetDataLength());

        // We should have found a single object
        ASSERT_EQ(decoded_objects.size(), 1);

        // Verify that what we got is a Head1 object
        ASSERT_TRUE(std::holds_alternative<gs::Head1>(decoded_objects.front()));

        // Verify the decoded value is the same as the encoded value
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).id.value, head.id.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).time, head.time);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.x, head.location.x);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.y, head.location.y);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.z, head.location.z);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.vx.value, head.location.vx.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.vy.value, head.location.vy.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).location.vz.value, head.location.vz.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.si.value, head.rotation.si.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.sj.value, head.rotation.sj.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.sk.value, head.rotation.sk.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.ei.value, head.rotation.ei.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.ej.value, head.rotation.ej.value);
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).rotation.ek.value, head.rotation.ek.value);
        ASSERT_TRUE(std::get<gs::Head1>(decoded_objects.front()).ipd.has_value());
        ASSERT_EQ(std::get<gs::Head1>(decoded_objects.front()).ipd.value().ipd.value, head.ipd.value().ipd.value);
    };

    // Test vector B.1, encoding as UnknownObject type
    TEST_F(GSDecoderTest, Test_Vector_B1_Unknown)
    {
        // PORTION TAKEN FROM GAME STATE ENCODER TEST

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
            ASSERT_EQ((*data_buffer)[i], expected[i]);
        }

        // PORTION TO TEST THE GAME STATE DECODER

        // Ensure we are at the start of the buffer
        ASSERT_EQ(data_buffer->GetReadLength(), 0);

        // Decode the data buffer
        ASSERT_EQ(decoder.Decode(data_buffer, decoded_objects),
                  data_buffer->GetDataLength());

        // We should have found a single object
        ASSERT_EQ(decoded_objects.size(), 1);

        // Verify that what we got is a Head1 object
        ASSERT_TRUE(std::holds_alternative<gs::UnknownObject>(decoded_objects.front()));

        // Verify the decoded object contents
        gs::UnknownObject &decoded_object = std::get<gs::UnknownObject>(decoded_objects.front());
        ASSERT_EQ(decoded_object.data.size(), expected.size() - 2);
        for (std::size_t i = 2; i < expected.size(); i++)
        {
            ASSERT_EQ(decoded_object.data[i - 2], expected[i]);
        }
    };

    // Test Mesh1 encoding
    TEST_F(GSDecoderTest, Test_Mesh1)
    {
        // PORTION TAKEN FROM GAME STATE ENCODER TEST

        std::vector<std::uint8_t> expected =
        {
            // Mesh1 tag
            0xc0, 0x80, 0x00,

            // Octets to follow
            0x35,

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
            0x02,

            // Triangles
            0x02, 0x80, 0x82
        };

        gs::Mesh1 mesh{};

        mesh.id.value = 0x1b;
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});
        mesh.vertices.push_back({1.0f, 2.0f, 3.0f});

        mesh.normals.push_back({{3.140625f}, {-1.0f}, {65504.0f}});
        mesh.normals.push_back({{3.140625f}, {-1.0f}, {3.140625f}});
        mesh.normals.push_back({{3.140625f}, {-1.0f}, {65504.0f}});

        mesh.textures.push_back({{1}, {129}});

        mesh.triangles.push_back({2});
        mesh.triangles.push_back({130});

        // Check the expected encoded length
        ASSERT_EQ(encoder.Encode(data_buffer, mesh),
                  std::make_pair(std::size_t(1), expected.size()));

        // Verify the buffer contents
        for (std::size_t i = 0; i < expected.size(); i++)
        {
            ASSERT_EQ((*data_buffer)[i], expected[i]);
        }

        // PORTION TO TEST THE GAME STATE DECODER

        // Ensure we are at the start of the buffer
        ASSERT_EQ(data_buffer->GetReadLength(), 0);

        // Decode the data buffer
        ASSERT_EQ(decoder.Decode(data_buffer, decoded_objects), data_buffer->GetDataLength());

        // We should have found a single object
        ASSERT_EQ(decoded_objects.size(), 1);

        // Verify that what we got is a Mesh1 object
        ASSERT_TRUE(std::holds_alternative<gs::Mesh1>(decoded_objects.front()));

        // Verify the decoded value is the same as the encoded value
        ASSERT_EQ(std::get<gs::Mesh1>(decoded_objects.front()).id.value, mesh.id.value);

        gs::Mesh1 &mesh_decoded = std::get<gs::Mesh1>(decoded_objects.front());

        ASSERT_EQ(mesh.vertices.size(), mesh_decoded.vertices.size());
        for (std::size_t i = 0; i < mesh.vertices.size(); i++)
        {
            ASSERT_EQ(mesh.vertices[i].x, mesh_decoded.vertices[i].x);
            ASSERT_EQ(mesh.vertices[i].y, mesh_decoded.vertices[i].y);
            ASSERT_EQ(mesh.vertices[i].z, mesh_decoded.vertices[i].z);
        }

        ASSERT_EQ(mesh.normals.size(), mesh_decoded.normals.size());
        for (std::size_t i = 0; i < mesh.normals.size(); i++)
        {
            ASSERT_EQ(mesh.normals[i].x.value, mesh_decoded.normals[i].x.value);
            ASSERT_EQ(mesh.normals[i].y.value, mesh_decoded.normals[i].y.value);
            ASSERT_EQ(mesh.normals[i].z.value, mesh_decoded.normals[i].z.value);
        }

        ASSERT_EQ(mesh.textures.size(), mesh_decoded.textures.size());
        for (std::size_t i = 0; i < mesh.textures.size(); i++)
        {
            ASSERT_EQ(mesh.textures[i].u.value, mesh_decoded.textures[i].u.value);
            ASSERT_EQ(mesh.textures[i].v.value, mesh_decoded.textures[i].v.value);
        }

        ASSERT_EQ(mesh.triangles.size(), mesh_decoded.triangles.size());
        for (std::size_t i = 0; i < mesh.triangles.size(); i++)
        {
            ASSERT_EQ(mesh.triangles[i].value, mesh_decoded.triangles[i].value);
            ASSERT_EQ(mesh.triangles[i].value, mesh_decoded.triangles[i].value);
        }
    }

    // Test encoding vector of variants
    TEST_F(GSDecoderTest, Test_Variant_Vector)
    {
        // PORTION TAKEN FROM GAME STATE ENCODER TEST

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
            ASSERT_EQ((*data_buffer)[i], expected[i]);
        }

        // PORTION TO TEST THE GAME STATE DECODER

        // Ensure we are at the start of the buffer
        ASSERT_EQ(data_buffer->GetReadLength(), 0);

        // Decode the data buffer
        ASSERT_EQ(decoder.Decode(data_buffer, decoded_objects), data_buffer->GetDataLength());

        // We should have found three object
        ASSERT_EQ(decoded_objects.size(), 3);

        // Check that we got the objects we expected
        ASSERT_TRUE(std::holds_alternative<gs::Head1>(decoded_objects[0]));
        ASSERT_TRUE(std::holds_alternative<gs::Mesh1>(decoded_objects[1]));
        ASSERT_TRUE(std::holds_alternative<gs::Head1>(decoded_objects[2]));
    };

    // Test encoding Hand1
    TEST_F(GSDecoderTest, Test_Hand1)
    {
        // PORTION TAKEN FROM GAME STATE ENCODER TEST

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
            ASSERT_EQ((*data_buffer)[i], expected[i]);
        }

        // PORTION TO TEST THE GAME STATE DECODER

        // Ensure we are at the start of the buffer
        ASSERT_EQ(data_buffer->GetReadLength(), 0);

        // Decode the data buffer
        ASSERT_EQ(decoder.Decode(data_buffer, decoded_objects), data_buffer->GetDataLength());

        // We should have found a single object
        ASSERT_EQ(decoded_objects.size(), 1);

        // Verify that what we got is a Mesh1 object
        ASSERT_TRUE(std::holds_alternative<gs::Hand1>(decoded_objects.front()));

        // Verify the decoded value is the same as the encoded value
        ASSERT_EQ(std::get<gs::Hand1>(decoded_objects.front()).id.value, hand1.id.value);

        gs::Hand1 &hand_decoded = std::get<gs::Hand1>(decoded_objects.front());

        ASSERT_EQ(hand_decoded.time, hand1.time);
        ASSERT_EQ(hand_decoded.left, hand1.left);
        ASSERT_EQ(hand_decoded.location.x, hand1.location.x);
        ASSERT_EQ(hand_decoded.location.y, hand1.location.y);
        ASSERT_EQ(hand_decoded.location.z, hand1.location.z);
        ASSERT_EQ(hand_decoded.location.vx.value, hand1.location.vx.value);
        ASSERT_EQ(hand_decoded.location.vy.value, hand1.location.vy.value);
        ASSERT_EQ(hand_decoded.location.vz.value, hand1.location.vz.value);
        ASSERT_EQ(hand_decoded.rotation.si.value, hand1.rotation.si.value);
        ASSERT_EQ(hand_decoded.rotation.sj.value, hand1.rotation.sj.value);
        ASSERT_EQ(hand_decoded.rotation.sk.value, hand1.rotation.sk.value);
        ASSERT_EQ(hand_decoded.rotation.ei.value, hand1.rotation.ei.value);
        ASSERT_EQ(hand_decoded.rotation.ej.value, hand1.rotation.ej.value);
        ASSERT_EQ(hand_decoded.rotation.ek.value, hand1.rotation.ek.value);
    }

    // Test encoding Hand1
    TEST_F(GSDecoderTest, Test_Hand2)
    {
        // PORTION TAKEN FROM GAME STATE ENCODER TEST

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
            ASSERT_EQ((*data_buffer)[i], expected[i]);
        }

        // PORTION TO TEST THE GAME STATE DECODER

        // Ensure we are at the start of the buffer
        ASSERT_EQ(data_buffer->GetReadLength(), 0);

        // Decode the data buffer
        ASSERT_EQ(decoder.Decode(data_buffer, decoded_objects),
                  data_buffer->GetDataLength());

        // We should have found a single object
        ASSERT_EQ(decoded_objects.size(), 1);

        // Verify that what we got is a Mesh1 object
        ASSERT_TRUE(std::holds_alternative<gs::Hand2>(decoded_objects.front()));

        // Verify the decoded value is the same as the encoded value
        ASSERT_EQ(std::get<gs::Hand2>(decoded_objects.front()).id.value,
                  hand2.id.value);

        gs::Hand2 &hand_decoded = std::get<gs::Hand2>(decoded_objects.front());

        ASSERT_EQ(hand_decoded.time, hand2.time);
        ASSERT_EQ(hand_decoded.left, hand2.left);
        ASSERT_EQ(hand_decoded.location.x, hand2.location.x);
        ASSERT_EQ(hand_decoded.location.y, hand2.location.y);
        ASSERT_EQ(hand_decoded.location.z, hand2.location.z);
        ASSERT_EQ(hand_decoded.location.vx.value, hand2.location.vx.value);
        ASSERT_EQ(hand_decoded.location.vy.value, hand2.location.vy.value);
        ASSERT_EQ(hand_decoded.location.vz.value, hand2.location.vz.value);
        ASSERT_EQ(hand_decoded.rotation.si.value, hand2.rotation.si.value);
        ASSERT_EQ(hand_decoded.rotation.sj.value, hand2.rotation.sj.value);
        ASSERT_EQ(hand_decoded.rotation.sk.value, hand2.rotation.sk.value);
        ASSERT_EQ(hand_decoded.rotation.ei.value, hand2.rotation.ei.value);
        ASSERT_EQ(hand_decoded.rotation.ej.value, hand2.rotation.ej.value);
        ASSERT_EQ(hand_decoded.rotation.ek.value, hand2.rotation.ek.value);


        ASSERT_EQ(hand_decoded.wrist.tx.value, hand2.wrist.tx.value);
        ASSERT_EQ(hand_decoded.wrist.ty.value, hand2.wrist.ty.value);
        ASSERT_EQ(hand_decoded.wrist.tz.value, hand2.wrist.tz.value);

        ASSERT_EQ(hand_decoded.thumb.tip.tx.value, hand2.thumb.tip.tx.value);
        ASSERT_EQ(hand_decoded.thumb.tip.ty.value, hand2.thumb.tip.ty.value);
        ASSERT_EQ(hand_decoded.thumb.tip.tz.value, hand2.thumb.tip.tz.value);

        ASSERT_EQ(hand_decoded.thumb.ip.tx.value, hand2.thumb.ip.tx.value);
        ASSERT_EQ(hand_decoded.thumb.ip.ty.value, hand2.thumb.ip.ty.value);
        ASSERT_EQ(hand_decoded.thumb.ip.tz.value, hand2.thumb.ip.tz.value);

        ASSERT_EQ(hand_decoded.thumb.mcp.tx.value, hand2.thumb.mcp.tx.value);
        ASSERT_EQ(hand_decoded.thumb.mcp.ty.value, hand2.thumb.mcp.ty.value);
        ASSERT_EQ(hand_decoded.thumb.mcp.tz.value, hand2.thumb.mcp.tz.value);

        ASSERT_EQ(hand_decoded.thumb.cmc.tx.value, hand2.thumb.cmc.tx.value);
        ASSERT_EQ(hand_decoded.thumb.cmc.ty.value, hand2.thumb.cmc.ty.value);
        ASSERT_EQ(hand_decoded.thumb.cmc.tz.value, hand2.thumb.cmc.tz.value);

        ASSERT_EQ(hand_decoded.pinky.tip.tx.value, hand2.pinky.tip.tx.value);
        ASSERT_EQ(hand_decoded.pinky.tip.ty.value, hand2.pinky.tip.ty.value);
        ASSERT_EQ(hand_decoded.pinky.tip.tz.value, hand2.pinky.tip.tz.value);
    }

} // namespace
