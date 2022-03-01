/*
 *  test_gs_serializer.h
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test to various deserialization function.  It relies
 *      on proper functionality of the serializer, which should be tested
 *      independently of the deserializer.
 *
 *  Portability Issues:
 *      None.
 */

#include <cstring>
#include <cstdint>
#include <memory>
#include "gtest/gtest.h"
#include "gs_deserializer.h"
#include "gs_serializer.h"
#include "data_buffer.h"

namespace {

    // The fixture for testing floating point formats
    class GSDeserializerTest : public ::testing::Test
    {
        public:
            GSDeserializerTest() :
                data_buffer(std::make_unique<DataBuffer<>>(1500))
            {
            }

            ~GSDeserializerTest()
            {
            }

        protected:
            gs::Deserializer deserializer;
            gs::Serializer serializer;
            std::unique_ptr<DataBuffer<>> data_buffer;
    };

    /////////////////////////////////
    // ReadUint Tests
    /////////////////////////////////

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadUint8_0)
    {
        // Serialize the data
        gs::Uint8 v = 0x00;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Uint8 value = 0xff;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 127
    TEST_F(GSDeserializerTest, ReadUint8_127)
    {
        // Serialize the data
        gs::Uint8 v = 127;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Uint8 value = 0xff;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 255
    TEST_F(GSDeserializerTest, ReadUint8_255)
    {
        // Serialize the data
        gs::Uint8 v = 255;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Uint8 value = 0x00;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadUint16_0)
    {
        // Serialize the data
        gs::Uint16 v = 0;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Uint16 value = 0xffff;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 32767
    TEST_F(GSDeserializerTest, ReadUint16_32767)
    {
        // Serialize the data
        gs::Uint16 v = 32767;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Uint16 value = 0xffff;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 65535
    TEST_F(GSDeserializerTest, ReadUint16_65535)
    {
        // Serialize the data
        gs::Uint16 v = 65535;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Uint16 value = 0x0000;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadUint32_0)
    {
        // Serialize the data
        gs::Uint32 v = 0;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Uint32 value = 0xdeadbeef;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 2147483649
    TEST_F(GSDeserializerTest, ReadUint32_2147483649)
    {
        // Serialize the data
        gs::Uint32 v = 2147483649;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Uint32 value = 0xdeadbeef;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value DEADBEEF
    TEST_F(GSDeserializerTest, ReadUint32_DEADBEEF)
    {
        // Serialize the data
        gs::Uint32 v = 0xdeadbeef;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Uint32 value = 0xcafebabe;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadUint64_0)
    {
        // Serialize the data
        gs::Uint64 v = 0;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Uint64 value = 0xdeadbeefcafebabe;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 0xdeadbeefcafebabe
    TEST_F(GSDeserializerTest, ReadUint64_deadbeefcafebabe)
    {
        // Serialize the data
        gs::Uint64 v = 0xdeadbeefcafebabe;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Uint64 value = 0;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    /////////////////////////////////
    // ReadInt Tests
    /////////////////////////////////

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadInt8_0)
    {
        // Serialize the data
        gs::Int8 v = 0x00;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Int8 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 1
    TEST_F(GSDeserializerTest, ReadInt8_1)
    {
        // Serialize the data
        gs::Int8 v = 1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Int8 value = 0;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value -1
    TEST_F(GSDeserializerTest, ReadInt8_neg_1)
    {
        // Serialize the data
        gs::Int8 v = -1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Int8 value = 0;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 127
    TEST_F(GSDeserializerTest, ReadInt8_127)
    {
        // Serialize the data
        gs::Int8 v = 127;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Int8 value = 0;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value -128
    TEST_F(GSDeserializerTest, ReadInt8_neg_128)
    {
        // Serialize the data
        gs::Int8 v = -128;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Int8 value = 0;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadInt16_0)
    {
        // Serialize the data
        gs::Int16 v = 0x00;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Int16 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 1
    TEST_F(GSDeserializerTest, ReadInt16_1)
    {
        // Serialize the data
        gs::Int16 v = 1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Int16 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value -1
    TEST_F(GSDeserializerTest, ReadInt16_neg_1)
    {
        // Serialize the data
        gs::Int16 v = -1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Int16 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 32767
    TEST_F(GSDeserializerTest, ReadInt16_32767)
    {
        // Serialize the data
        gs::Int16 v = 32767;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Int16 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value -32768
    TEST_F(GSDeserializerTest, ReadInt16_neg_32768)
    {
        // Serialize the data
        gs::Int16 v = -32768;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Int16 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadInt32_0)
    {
        // Serialize the data
        gs::Int32 v = 0;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Int32 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 1
    TEST_F(GSDeserializerTest, ReadInt32_1)
    {
        // Serialize the data
        gs::Int32 v = 1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Int32 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value -1
    TEST_F(GSDeserializerTest, ReadInt32_neg_1)
    {
        // Serialize the data
        gs::Int32 v = -1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Int32 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 2147483647
    TEST_F(GSDeserializerTest, ReadInt32_2147483647)
    {
        // Serialize the data
        gs::Int32 v = INT32_MAX;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Int32 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value -2147483648
    TEST_F(GSDeserializerTest, ReadInt32_neg_2147483648)
    {
        // Serialize the data
        gs::Int32 v = INT32_MIN;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Int32 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 0
    TEST_F(GSDeserializerTest, ReadInt64_0)
    {
        // Serialize the data
        gs::Int64 v = 0;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Int64 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 1
    TEST_F(GSDeserializerTest, ReadInt64_1)
    {
        // Serialize the data
        gs::Int64 v = 1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Int64 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 1
    TEST_F(GSDeserializerTest, ReadInt64_neg_1)
    {
        // Serialize the data
        gs::Int64 v = -1;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Int64 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value 9223372036854775807
    TEST_F(GSDeserializerTest, ReadInt64_9223372036854775807)
    {
        // Serialize the data
        gs::Int64 v = INT64_MAX; // 9223372036854775807
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Int64 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize unsigned value -9223372036854775808
    TEST_F(GSDeserializerTest, ReadInt64_neg_9223372036854775808)
    {
        // Serialize the data
        gs::Int64 v = INT64_MIN; // -9223372036854775808;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Int64 value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    /////////////////////////////////
    // ReadVarUint Tests
    /////////////////////////////////

    // Serialize and deserialize VarUint 0
    TEST_F(GSDeserializerTest, ReadVarUint_0)
    {
        // Serialize the data
        gs::VarUint v{0};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 1
    TEST_F(GSDeserializerTest, ReadVarUint_1)
    {
        // Serialize the data
        gs::VarUint v{1};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 63
    TEST_F(GSDeserializerTest, ReadVarUint_63)
    {
        // Serialize the data
        gs::VarUint v{63};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 64
    TEST_F(GSDeserializerTest, ReadVarUint_64)
    {
        // Serialize the data
        gs::VarUint v{64};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 127
    TEST_F(GSDeserializerTest, ReadVarUint_127)
    {
        // Serialize the data
        gs::VarUint v{127};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 128
    TEST_F(GSDeserializerTest, ReadVarUint_128)
    {
        // Serialize the data
        gs::VarUint v{128};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 8191
    TEST_F(GSDeserializerTest, ReadVarUint_8191)
    {
        // Serialize the data
        gs::VarUint v{8191};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 8192
    TEST_F(GSDeserializerTest, ReadVarUint_8192)
    {
        // Serialize the data
        gs::VarUint v{8192};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 16383
    TEST_F(GSDeserializerTest, ReadVarUint_16383)
    {
        // Serialize the data
        gs::VarUint v{16383};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 16384
    TEST_F(GSDeserializerTest, ReadVarUint_16384)
    {
        // Serialize the data
        gs::VarUint v{16384};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 1048575
    TEST_F(GSDeserializerTest, ReadVarUint_1048575)
    {
        // Serialize the data
        gs::VarUint v{1048575};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 1048576
    TEST_F(GSDeserializerTest, ReadVarUint_1048576)
    {
        // Serialize the data
        gs::VarUint v{1048576};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 2097151
    TEST_F(GSDeserializerTest, ReadVarUint_2097151)
    {
        // Serialize the data
        gs::VarUint v{2097151};                 // Max 21-bit unsigned integer
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 2147483647
    TEST_F(GSDeserializerTest, ReadVarUint_2147483647)
    {
        // Serialize the data
        gs::VarUint v{2147483647UL};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 5);
        ASSERT_EQ(data_buffer->GetDataLength(), 5);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 5);
        ASSERT_EQ(data_buffer->GetReadLength(), 5);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 2147483648
    TEST_F(GSDeserializerTest, ReadVarUint_2147483648)
    {
        // Serialize the data
        gs::VarUint v{2147483648};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 5);
        ASSERT_EQ(data_buffer->GetDataLength(), 5);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 5);
        ASSERT_EQ(data_buffer->GetReadLength(), 5);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 4294967295
    TEST_F(GSDeserializerTest, ReadVarUint_4294967295)
    {
        // Serialize the data
        gs::VarUint v{UINT32_MAX};              // 4294967295
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 5);
        ASSERT_EQ(data_buffer->GetDataLength(), 5);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 5);
        ASSERT_EQ(data_buffer->GetReadLength(), 5);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 4294967296
    TEST_F(GSDeserializerTest, ReadVarUint_4294967296)
    {
        // Serialize the data
        gs::VarUint v{4294967296};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 9);
        ASSERT_EQ(data_buffer->GetDataLength(), 9);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 9);
        ASSERT_EQ(data_buffer->GetReadLength(), 9);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarUint 2^64-1
    TEST_F(GSDeserializerTest, ReadVarUint_18446744073709551615)
    {
        // Serialize the data
        gs::VarUint v{UINT64_MAX};              // 18446744073709551615
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 9);
        ASSERT_EQ(data_buffer->GetDataLength(), 9);

        // Read the data
        gs::VarUint value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 9);
        ASSERT_EQ(data_buffer->GetReadLength(), 9);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    /////////////////////////////////
    // ReadVarInt Tests
    /////////////////////////////////

    // Serialize and deserialize VarInt 0
    TEST_F(GSDeserializerTest, ReadVarInt_0)
    {
        // Serialize the data
        gs::VarInt v{0};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 1
    TEST_F(GSDeserializerTest, ReadVarInt_1)
    {
        // Serialize the data
        gs::VarInt v{1};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -1
    TEST_F(GSDeserializerTest, ReadVarInt_neg_1)
    {
        // Serialize the data
        gs::VarInt v{-1};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -64
    TEST_F(GSDeserializerTest, ReadVarInt_neg_64)
    {
        // Serialize the data
        gs::VarInt v{-64};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 63
    TEST_F(GSDeserializerTest, ReadVarInt_63)
    {
        // Serialize the data
        gs::VarInt v{63};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 64
    TEST_F(GSDeserializerTest, ReadVarInt_64)
    {
        // Serialize the data
        gs::VarInt v{64};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -65
    TEST_F(GSDeserializerTest, ReadVarInt_neg_65)
    {
        // Serialize the data
        gs::VarInt v{-65};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -8192
    TEST_F(GSDeserializerTest, ReadVarInt_neg_8192)
    {
        // Serialize the data
        gs::VarInt v{-8192};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 8191
    TEST_F(GSDeserializerTest, ReadVarInt_8191)
    {
        // Serialize the data
        gs::VarInt v{8191};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -8193
    TEST_F(GSDeserializerTest, ReadVarInt_neg_8193)
    {
        // Serialize the data
        gs::VarInt v{-8193};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 8192
    TEST_F(GSDeserializerTest, ReadVarInt_8192)
    {
        // Serialize the data
        gs::VarInt v{8192};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -1048576
    TEST_F(GSDeserializerTest, ReadVarInt_neg_1048576)
    {
        // Serialize the data
        gs::VarInt v{-1048576};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 1048575
    TEST_F(GSDeserializerTest, ReadVarInt_1048575)
    {
        // Serialize the data
        gs::VarInt v{1048575};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 3);
        ASSERT_EQ(data_buffer->GetDataLength(), 3);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 3);
        ASSERT_EQ(data_buffer->GetReadLength(), 3);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -1048577
    TEST_F(GSDeserializerTest, ReadVarInt_neg_1048577)
    {
        // Serialize the data
        gs::VarInt v{-1048577};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 5);
        ASSERT_EQ(data_buffer->GetDataLength(), 5);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 5);
        ASSERT_EQ(data_buffer->GetReadLength(), 5);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 1048576
    TEST_F(GSDeserializerTest, ReadVarInt_1048576)
    {
        // Serialize the data
        gs::VarInt v{1048576};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 5);
        ASSERT_EQ(data_buffer->GetDataLength(), 5);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 5);
        ASSERT_EQ(data_buffer->GetReadLength(), 5);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -2147483648
    TEST_F(GSDeserializerTest, ReadVarInt_neg_2147483648)
    {
        // Serialize the data
        gs::VarInt v{-2147483648ll};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 5);
        ASSERT_EQ(data_buffer->GetDataLength(), 5);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 5);
        ASSERT_EQ(data_buffer->GetReadLength(), 5);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 2147483647
    TEST_F(GSDeserializerTest, ReadVarInt_2147483647)
    {
        // Serialize the data
        gs::VarInt v{2147483647ll};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 5);
        ASSERT_EQ(data_buffer->GetDataLength(), 5);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 5);
        ASSERT_EQ(data_buffer->GetReadLength(), 5);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -2147483649
    TEST_F(GSDeserializerTest, ReadVarInt_neg_2147483649)
    {
        // Serialize the data
        gs::VarInt v{-2147483649ll};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 9);
        ASSERT_EQ(data_buffer->GetDataLength(), 9);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 9);
        ASSERT_EQ(data_buffer->GetReadLength(), 9);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 2147483648
    TEST_F(GSDeserializerTest, ReadVarInt_2147483648)
    {
        // Serialize the data
        gs::VarInt v{2147483648ll};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 9);
        ASSERT_EQ(data_buffer->GetDataLength(), 9);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 9);
        ASSERT_EQ(data_buffer->GetReadLength(), 9);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt -9223372036854775808
    TEST_F(GSDeserializerTest, ReadVarInt_neg_9223372036854775808)
    {
        // Serialize the data
        gs::VarInt v{INT64_MIN};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 9);
        ASSERT_EQ(data_buffer->GetDataLength(), 9);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 9);
        ASSERT_EQ(data_buffer->GetReadLength(), 9);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    // Serialize and deserialize VarInt 9223372036854775807
    TEST_F(GSDeserializerTest, ReadVarInt_9223372036854775807)
    {
        // Serialize the data
        gs::VarInt v{INT64_MAX};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 9);
        ASSERT_EQ(data_buffer->GetDataLength(), 9);

        // Read the data
        gs::VarInt value{10};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 9);
        ASSERT_EQ(data_buffer->GetReadLength(), 9);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    /////////////////////////////////
    // 16-bit Half Floats
    /////////////////////////////////

    // Serialize and deserialize half float
    TEST_F(GSDeserializerTest, ReadHalfFloat)
    {
        // Serialize the data
        gs::Float16 v{3.140625f};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 2);
        ASSERT_EQ(data_buffer->GetDataLength(), 2);

        // Read the data
        gs::Float16 value{10.0f};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 2);
        ASSERT_EQ(data_buffer->GetReadLength(), 2);

        // Compare the results
        ASSERT_EQ(v.value, value.value);
    };

    /////////////////////////////////
    // 32-bit Floats
    /////////////////////////////////

    // Serialize and deserialize 32-bit float
    TEST_F(GSDeserializerTest, ReadFloat)
    {
        // Serialize the data
        gs::Float32 v = 3.14159265f;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 4);
        ASSERT_EQ(data_buffer->GetDataLength(), 4);

        // Read the data
        gs::Float32 value = 10.0f;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 4);
        ASSERT_EQ(data_buffer->GetReadLength(), 4);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    /////////////////////////////////
    // 64-bit Floats
    /////////////////////////////////

    // Serialize and deserialize 64-bit float
    TEST_F(GSDeserializerTest, ReadDouble)
    {
        // Serialize the data
        gs::Float64 v = 3.141592653589793;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 8);
        ASSERT_EQ(data_buffer->GetDataLength(), 8);

        // Read the data
        gs::Float64 value = 10.0;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 8);
        ASSERT_EQ(data_buffer->GetReadLength(), 8);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    /////////////////////////////////
    // Boolean tests
    /////////////////////////////////

    // Serialize and deserialize Boolean value
    TEST_F(GSDeserializerTest, ReadBoolean_true)
    {
        // Serialize the data
        gs::Boolean v = true;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Boolean value = false;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize Boolean value
    TEST_F(GSDeserializerTest, ReadBoolean_false)
    {
        // Serialize the data
        gs::Boolean v = false;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Boolean value = true;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    /////////////////////////////////
    // Byte tests
    /////////////////////////////////

    // Serialize and deserialize individual bytes
    TEST_F(GSDeserializerTest, ReadByte_0)
    {
        // Serialize the data
        gs::Byte v = 0;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Byte value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize individual bytes
    TEST_F(GSDeserializerTest, ReadByte_128)
    {
        // Serialize the data
        gs::Byte v = 128;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Byte value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize individual bytes
    TEST_F(GSDeserializerTest, ReadByte_255)
    {
        // Serialize the data
        gs::Byte v = 255;
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::Byte value = 10;
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    /////////////////////////////////
    // String tests
    /////////////////////////////////

    // Serialize and deserialize String ""
    TEST_F(GSDeserializerTest, ReadString_empty)
    {
        gs::String v{};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1);
        ASSERT_EQ(data_buffer->GetDataLength(), 1);

        // Read the data
        gs::String value{};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1);
        ASSERT_EQ(data_buffer->GetReadLength(), 1);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize String "Hello"
    TEST_F(GSDeserializerTest, ReadString_Hello)
    {
        gs::String v{"Hello"};
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 6);
        ASSERT_EQ(data_buffer->GetDataLength(), 6);

        // Read the data
        gs::String value{};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 6);
        ASSERT_EQ(data_buffer->GetReadLength(), 6);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize String (longer)
    TEST_F(GSDeserializerTest, ReadString_longer)
    {
        gs::String v = "The quick brown fox jumps over the lazy dog. "
                       "The quick brown fox jumps over the lazy dog. "
                       "The quick brown fox jumps over the lazy dog.";
        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 136);
        ASSERT_EQ(data_buffer->GetDataLength(), 136);

        // Read the data
        gs::String value{};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 136);
        ASSERT_EQ(data_buffer->GetReadLength(), 136);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    /////////////////////////////////
    // Blob tests
    /////////////////////////////////

    // Serialize and deserialize Blob ""
    TEST_F(GSDeserializerTest, ReadBlob_Hello)
    {
        gs::String raw_data = "Hello";

        // Populate the Blob from the String
        gs::Blob v;
        for (auto c : raw_data) v.push_back(c);

        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 6);
        ASSERT_EQ(data_buffer->GetDataLength(), 6);

        // Read the data
        gs::Blob value{};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 6);
        ASSERT_EQ(data_buffer->GetReadLength(), 6);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize Blob (longer)
    TEST_F(GSDeserializerTest, ReadBlob_longer)
    {
        gs::String raw_data = "The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog.";

        // Populate the Blob from the String
        gs::Blob v;
        for (auto c : raw_data) v.push_back(c);

        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 136);
        ASSERT_EQ(data_buffer->GetDataLength(), 136);

        // Read the data
        gs::Blob value{};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 136);
        ASSERT_EQ(data_buffer->GetReadLength(), 136);

        // Compare the results
        ASSERT_EQ(v, value);
    };

    // Serialize and deserialize Blob (even longer)
    TEST_F(GSDeserializerTest, ReadBlob_even_longer)
    {
        // Populate the Blob from the String
        gs::Blob v;
        for (unsigned i = 0; i < 1000; i++) v.push_back(i % 256);

        std::size_t l = serializer.Write(data_buffer, v);
        ASSERT_EQ(l, 1002);
        ASSERT_EQ(data_buffer->GetDataLength(), 1002);

        // Read the data
        gs::Blob value{};
        ASSERT_EQ(deserializer.Read(data_buffer, value), 1002);
        ASSERT_EQ(data_buffer->GetReadLength(), 1002);

        // Compare the results
        ASSERT_EQ(v, value);
    };

} // namespace
