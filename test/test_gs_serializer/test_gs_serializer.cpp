/*
 *  test_gs_serializer.h
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test to various serialization function.  It relies
 *      on proper functionality of the DataBuffer object.
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
#include <cstddef>
#include "gtest/gtest.h"
#include "gs_serializer.h"
#include "data_buffer.h"

namespace {

    // The fixture for testing floating point formats
    class GSSerializerTest : public ::testing::Test
    {
        public:
            GSSerializerTest() : data_buffer(1500)
            {
            }

            ~GSSerializerTest()
            {
            }

        protected:
            gs::Serializer serializer;
            gs::DataBuffer<> data_buffer;
    };

    /////////////////////////////////
    // WriteUint Tests
    /////////////////////////////////

    // Serialize unsigned value 0
    TEST_F(GSSerializerTest, WriteUint8_0)
    {
        gs::Uint8 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    // Serialize unsigned value 127
    TEST_F(GSSerializerTest, WriteUint8_127)
    {
        gs::Uint8 value = 127;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x7f);
    };

    // Serialize unsigned value 255
    TEST_F(GSSerializerTest, WriteUint8_255)
    {
        gs::Uint8 value = 255;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0xff);
    };

    // Serialize unsigned value 0
    TEST_F(GSSerializerTest, WriteUint16_0)
    {
        gs::Uint16 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
    };

    // Serialize unsigned value 32767
    TEST_F(GSSerializerTest, WriteUint16_32767)
    {
        gs::Uint16 value = 32767;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x7f);
        ASSERT_EQ(data_buffer[1], 0xff);
    };

    // Serialize unsigned value 65535
    TEST_F(GSSerializerTest, WriteUint16_65535)
    {
        gs::Uint16 value = 65535;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0xff);
        ASSERT_EQ(data_buffer[1], 0xff);
    };

    // Serialize unsigned value 0
    TEST_F(GSSerializerTest, WriteUint32_0)
    {
        gs::Uint32 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x00);
    };

    // Serialize unsigned value 2147483649
    TEST_F(GSSerializerTest, WriteUint32_2147483649)
    {
        gs::Uint32 value = 2147483649;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x80);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x01);
    };

    // Serialize unsigned value DEADBEEF
    TEST_F(GSSerializerTest, WriteUint32_DEADBEEF)
    {
        gs::Uint32 value = 0xdeadbeef;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0xde);
        ASSERT_EQ(data_buffer[1], 0xad);
        ASSERT_EQ(data_buffer[2], 0xbe);
        ASSERT_EQ(data_buffer[3], 0xef);
    };

    // Serialize unsigned value 0
    TEST_F(GSSerializerTest, WriteUint64_0)
    {
        gs::Uint64 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x00);
        ASSERT_EQ(data_buffer[4], 0x00);
        ASSERT_EQ(data_buffer[5], 0x00);
        ASSERT_EQ(data_buffer[6], 0x00);
        ASSERT_EQ(data_buffer[7], 0x00);
    };

    // Serialize unsigned value 0xdeadbeefcafebabe
    TEST_F(GSSerializerTest, WriteUint64_deadbeefcafebabe)
    {
        gs::Uint64 value = 0xdeadbeefcafebabe;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0xde);
        ASSERT_EQ(data_buffer[1], 0xad);
        ASSERT_EQ(data_buffer[2], 0xbe);
        ASSERT_EQ(data_buffer[3], 0xef);
        ASSERT_EQ(data_buffer[4], 0xca);
        ASSERT_EQ(data_buffer[5], 0xfe);
        ASSERT_EQ(data_buffer[6], 0xba);
        ASSERT_EQ(data_buffer[7], 0xbe);
    };

    /////////////////////////////////
    // WriteInt Tests
    /////////////////////////////////

    // Serialize signed value 0
    TEST_F(GSSerializerTest, WriteInt8_0)
    {
        gs::Int8 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    // Serialize signed value 1
    TEST_F(GSSerializerTest, WriteInt8_1)
    {
        gs::Int8 value = 1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x01);
    };

    // Serialize signed value -1
    TEST_F(GSSerializerTest, WriteInt8_neg_1)
    {
        gs::Int8 value = -1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x0FF);
    };

    // Serialize signed value 127
    TEST_F(GSSerializerTest, WriteInt8_127)
    {
        gs::Int8 value = 127;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x7f);
    };

    // Serialize signed value -128
    TEST_F(GSSerializerTest, WriteInt8_neg_128)
    {
        gs::Int8 value = -128;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x80);
    };

    // Serialize signed value 0
    TEST_F(GSSerializerTest, WriteInt16_0)
    {
        gs::Int16 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
    };

    // Serialize signed value 1
    TEST_F(GSSerializerTest, WriteInt16_1)
    {
        gs::Int16 value = 1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x01);
    };

    // Serialize signed value -1
    TEST_F(GSSerializerTest, WriteInt16_neg_1)
    {
        gs::Int16 value = -1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x0FF);
        ASSERT_EQ(data_buffer[1], 0x0FF);
    };

    // Serialize signed value 32767
    TEST_F(GSSerializerTest, WriteInt16_32767)
    {
        gs::Int16 value = 32767;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x7f);
        ASSERT_EQ(data_buffer[1], 0xff);
    };

    // Serialize signed value -32768
    TEST_F(GSSerializerTest, WriteInt16_neg_32768)
    {
        gs::Int16 value = -32768;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x080);
        ASSERT_EQ(data_buffer[1], 0x000);
    };

    // Serialize signed value 0
    TEST_F(GSSerializerTest, WriteInt32_0)
    {
        gs::Int32 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x00);
    };

    // Serialize signed value 1
    TEST_F(GSSerializerTest, WriteInt32_1)
    {
        gs::Int32 value = 1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x01);
    };

    // Serialize signed value -1
    TEST_F(GSSerializerTest, WriteInt32_neg_1)
    {
        gs::Int32 value = -1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0xff);
        ASSERT_EQ(data_buffer[1], 0xff);
        ASSERT_EQ(data_buffer[2], 0xff);
        ASSERT_EQ(data_buffer[3], 0xff);
    };

    // Serialize signed value 2147483647
    TEST_F(GSSerializerTest, WriteInt32_2147483647)
    {
        gs::Int32 value = 2147483647;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x7f);
        ASSERT_EQ(data_buffer[1], 0xff);
        ASSERT_EQ(data_buffer[2], 0xff);
        ASSERT_EQ(data_buffer[3], 0xff);
    };

    // Serialize signed value -2147483648
    TEST_F(GSSerializerTest, WriteInt32_neg_2147483648)
    {
        gs::Int32 value = INT32_MIN;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x80);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x00);
    };

    // Serialize signed value 0
    TEST_F(GSSerializerTest, WriteInt64_0)
    {
        gs::Int64 value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x00);
        ASSERT_EQ(data_buffer[4], 0x00);
        ASSERT_EQ(data_buffer[5], 0x00);
        ASSERT_EQ(data_buffer[6], 0x00);
        ASSERT_EQ(data_buffer[7], 0x00);
    };

    // Serialize signed value 1
    TEST_F(GSSerializerTest, WriteInt64_1)
    {
        gs::Int64 value = 1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0x00);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x00);
        ASSERT_EQ(data_buffer[4], 0x00);
        ASSERT_EQ(data_buffer[5], 0x00);
        ASSERT_EQ(data_buffer[6], 0x00);
        ASSERT_EQ(data_buffer[7], 0x01);
    };

    // Serialize signed value -1
    TEST_F(GSSerializerTest, WriteInt64_neg_1)
    {
        gs::Int64 value = -1;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0xff);
        ASSERT_EQ(data_buffer[1], 0xff);
        ASSERT_EQ(data_buffer[2], 0xff);
        ASSERT_EQ(data_buffer[3], 0xff);
        ASSERT_EQ(data_buffer[4], 0xff);
        ASSERT_EQ(data_buffer[5], 0xff);
        ASSERT_EQ(data_buffer[6], 0xff);
        ASSERT_EQ(data_buffer[7], 0xff);
    };

    // Serialize signed value 9223372036854775807
    TEST_F(GSSerializerTest, WriteInt64_9223372036854775807)
    {
        gs::Int64 value = 9223372036854775807;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0x7f);
        ASSERT_EQ(data_buffer[1], 0xff);
        ASSERT_EQ(data_buffer[2], 0xff);
        ASSERT_EQ(data_buffer[3], 0xff);
        ASSERT_EQ(data_buffer[4], 0xff);
        ASSERT_EQ(data_buffer[5], 0xff);
        ASSERT_EQ(data_buffer[6], 0xff);
        ASSERT_EQ(data_buffer[7], 0xff);
    };

    // Serialize signed value -9223372036854775808
    TEST_F(GSSerializerTest, WriteInt64_neg_9223372036854775808)
    {
        gs::Int64 value = INT64_MIN; // -9223372036854775808;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0x80);
        ASSERT_EQ(data_buffer[1], 0x00);
        ASSERT_EQ(data_buffer[2], 0x00);
        ASSERT_EQ(data_buffer[3], 0x00);
        ASSERT_EQ(data_buffer[4], 0x00);
        ASSERT_EQ(data_buffer[5], 0x00);
        ASSERT_EQ(data_buffer[6], 0x00);
        ASSERT_EQ(data_buffer[7], 0x00);
    };

    /////////////////////////////////
    // WriteVarUint Tests
    /////////////////////////////////

    // Serialize 64-bit VarUint 0
    TEST_F(GSSerializerTest, WriteVarUint_0)
    {
        gs::VarUint value{0};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    // Serialize 64-bit VarUint 1
    TEST_F(GSSerializerTest, WriteVarUint_1)
    {
        gs::VarUint value{1};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b00000001);
    };

    // Serialize 64-bit VarUint 63
    TEST_F(GSSerializerTest, WriteVarUint_63)
    {
        gs::VarUint value{63};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b00111111);
    };

    // Serialize 64-bit VarUint 64
    TEST_F(GSSerializerTest, WriteVarUint_64)
    {
        gs::VarUint value{64};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b01000000);
    };

    // Serialize 64-bit VarUint 127
    TEST_F(GSSerializerTest, WriteVarUint_127)
    {
        gs::VarUint value{127};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b01111111);
    };

    // Serialize 64-bit VarUint 128
    TEST_F(GSSerializerTest, WriteVarUint_128)
    {
        gs::VarUint value{128};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10000000);
        ASSERT_EQ(data_buffer[1], 0b10000000);
    };

    // Serialize 64-bit VarUint 8191
    TEST_F(GSSerializerTest, WriteVarUint_8191)
    {
        gs::VarUint value{8191};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10011111);
        ASSERT_EQ(data_buffer[1], 0b11111111);
    };

    // Serialize 64-bit VarUint 8192
    TEST_F(GSSerializerTest, WriteVarUint_8192)
    {
        gs::VarUint value{8192};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10100000);
        ASSERT_EQ(data_buffer[1], 0b00000000);
    };

    // Serialize 64-bit VarUint 16383
    TEST_F(GSSerializerTest, WriteVarUint_16383)
    {
        gs::VarUint value{16383};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10111111);
        ASSERT_EQ(data_buffer[1], 0b11111111);
    };

    // Serialize 64-bit VarUint 16384
    TEST_F(GSSerializerTest, WriteVarUint_16384)
    {
        gs::VarUint value{16384};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11000000);
        ASSERT_EQ(data_buffer[1], 0b01000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
    };

    // Serialize 64-bit VarUint 1048575
    TEST_F(GSSerializerTest, WriteVarUint_1048575)
    {
        gs::VarUint value{1048575};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11001111);
        ASSERT_EQ(data_buffer[1], 0b11111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
    };

    // Serialize 64-bit VarUint 1048576
    TEST_F(GSSerializerTest, WriteVarUint_1048576)
    {
        gs::VarUint value{1048576};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11010000);
        ASSERT_EQ(data_buffer[1], 0b00000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
    };

    // Serialize 64-bit VarUint 2097151
    TEST_F(GSSerializerTest, WriteVarUint_2097151)
    {
        gs::VarUint value{2097151};             // Max 21-bit unsigned integer
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11011111);
        ASSERT_EQ(data_buffer[1], 0b11111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
    };

    // Serialize 64-bit VarUint 2147483647
    TEST_F(GSSerializerTest, WriteVarUint_2147483647)
    {
        gs::VarUint value{2147483647};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);

        ASSERT_EQ(data_buffer[0], 0b11100001);
        ASSERT_EQ(data_buffer[1], 0b01111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
        ASSERT_EQ(data_buffer[3], 0b11111111);
        ASSERT_EQ(data_buffer[4], 0b11111111);
    };

    // Serialize 64-bit VarUint 2147483648
    TEST_F(GSSerializerTest, WriteVarUint_2147483648)
    {
        gs::VarUint value{2147483648};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);

        ASSERT_EQ(data_buffer[0], 0b11100001);
        ASSERT_EQ(data_buffer[1], 0b10000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
        ASSERT_EQ(data_buffer[3], 0b00000000);
        ASSERT_EQ(data_buffer[4], 0b00000000);
    };

    // Serialize 64-bit VarUint 4294967295
    TEST_F(GSSerializerTest, WriteVarUint_4294967295)
    {
        gs::VarUint value{UINT32_MAX};          // 4294967295
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);

        ASSERT_EQ(data_buffer[0], 0b11100001);
        ASSERT_EQ(data_buffer[1], 0b11111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
        ASSERT_EQ(data_buffer[3], 0b11111111);
        ASSERT_EQ(data_buffer[4], 0b11111111);
    };

    // Serialize 64-bit VarUint 4294967296
    TEST_F(GSSerializerTest, WriteVarUint_4294967296)
    {
        gs::VarUint value{4294967296};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 9);

        ASSERT_EQ(data_buffer.GetDataLength(), 9);

        ASSERT_EQ(data_buffer[0], 0b11100010);
        ASSERT_EQ(data_buffer[1], 0b00000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
        ASSERT_EQ(data_buffer[3], 0b00000000);
        ASSERT_EQ(data_buffer[4], 0b00000001);
        ASSERT_EQ(data_buffer[5], 0b00000000);
        ASSERT_EQ(data_buffer[6], 0b00000000);
        ASSERT_EQ(data_buffer[7], 0b00000000);
        ASSERT_EQ(data_buffer[8], 0b00000000);
    };

    // Serialize 64-bit VarUint 2^64-1
    TEST_F(GSSerializerTest, WriteVarUint_18446744073709551615)
    {
        gs::VarUint value{UINT64_MAX};          // 18446744073709551615
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 9);

        ASSERT_EQ(data_buffer.GetDataLength(), 9);

        ASSERT_EQ(data_buffer[0], 0b11100010);
        ASSERT_EQ(data_buffer[1], 0b11111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
        ASSERT_EQ(data_buffer[3], 0b11111111);
        ASSERT_EQ(data_buffer[4], 0b11111111);
        ASSERT_EQ(data_buffer[5], 0b11111111);
        ASSERT_EQ(data_buffer[6], 0b11111111);
        ASSERT_EQ(data_buffer[7], 0b11111111);
        ASSERT_EQ(data_buffer[8], 0b11111111);
    };

    /////////////////////////////////
    // WriteVarInt Tests
    /////////////////////////////////

    // Serialize 64-bit VarInt 0
    TEST_F(GSSerializerTest, WriteVarInt_0)
    {
        gs::VarInt value{0};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    // Serialize 64-bit VarInt 1
    TEST_F(GSSerializerTest, WriteVarInt_1)
    {
        gs::VarInt value{1};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b00000001);
    };

    // Serialize 64-bit VarInt -1
    TEST_F(GSSerializerTest, WriteVarInt_neg_1)
    {
        gs::VarInt value{-1};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b01111111);
    };

    // Serialize 64-bit VarInt -64
    TEST_F(GSSerializerTest, WriteVarInt_neg_64)
    {
        gs::VarInt value{-64};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b01000000);
    };

    // Serialize 64-bit VarInt 63
    TEST_F(GSSerializerTest, WriteVarInt_63)
    {
        gs::VarInt value{63};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0b00111111);
    };

    // Serialize 64-bit VarInt 64
    TEST_F(GSSerializerTest, WriteVarInt_64)
    {
        gs::VarInt value{64};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10000000);
        ASSERT_EQ(data_buffer[1], 0b01000000);
    };

    // Serialize 64-bit VarInt -65
    TEST_F(GSSerializerTest, WriteVarInt_neg_65)
    {
        gs::VarInt value{-65};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10111111);
        ASSERT_EQ(data_buffer[1], 0b10111111);
    };

    // Serialize 64-bit VarInt -8192
    TEST_F(GSSerializerTest, WriteVarInt_neg_8192)
    {
        gs::VarInt value{-8192};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10100000);
        ASSERT_EQ(data_buffer[1], 0b00000000);
    };

    // Serialize 64-bit VarInt 8191
    TEST_F(GSSerializerTest, WriteVarInt_8191)
    {
        gs::VarInt value{8191};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0b10011111);
        ASSERT_EQ(data_buffer[1], 0b11111111);
    };

    // Serialize 64-bit VarInt -8193
    TEST_F(GSSerializerTest, WriteVarInt_neg_8193)
    {
        gs::VarInt value{-8193};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11011111);
        ASSERT_EQ(data_buffer[1], 0b11011111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
    };

    // Serialize 64-bit VarInt 8192
    TEST_F(GSSerializerTest, WriteVarInt_8192)
    {
        gs::VarInt value{8192};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11000000);
        ASSERT_EQ(data_buffer[1], 0b00100000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
    };

    // Serialize 64-bit VarInt -1048576
    TEST_F(GSSerializerTest, WriteVarInt_neg_1048576)
    {
        gs::VarInt value{-1048576};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11010000);
        ASSERT_EQ(data_buffer[1], 0b00000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
    };

    // Serialize 64-bit VarInt 1048575
    TEST_F(GSSerializerTest, WriteVarInt_1048575)
    {
        gs::VarInt value{1048575};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 3);

        ASSERT_EQ(data_buffer.GetDataLength(), 3);

        ASSERT_EQ(data_buffer[0], 0b11001111);
        ASSERT_EQ(data_buffer[1], 0b11111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
    };

    // Serialize 64-bit VarInt -1048577
    TEST_F(GSSerializerTest, WriteVarInt_neg_1048577)
    {
        gs::VarInt value{-1048577};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);

        ASSERT_EQ(data_buffer[0], 0b11100001);
        ASSERT_EQ(data_buffer[1], 0b11111111);
        ASSERT_EQ(data_buffer[2], 0b11101111);
        ASSERT_EQ(data_buffer[3], 0b11111111);
        ASSERT_EQ(data_buffer[4], 0b11111111);
    };

    // Serialize 64-bit VarInt 1048576
    TEST_F(GSSerializerTest, WriteVarInt_1048576)
    {
        gs::VarInt value{1048576};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);

        ASSERT_EQ(data_buffer[0], 0b11100001);
        ASSERT_EQ(data_buffer[1], 0b00000000);
        ASSERT_EQ(data_buffer[2], 0b00010000);
        ASSERT_EQ(data_buffer[3], 0b00000000);
        ASSERT_EQ(data_buffer[4], 0b00000000);
    };

    // Serialize 64-bit VarInt -2147483648
    TEST_F(GSSerializerTest, WriteVarInt_neg_2147483648)
    {
        gs::VarInt value{INT32_MIN};            // -2147483648
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);

        ASSERT_EQ(data_buffer[0], 0b11100001);
        ASSERT_EQ(data_buffer[1], 0b10000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
        ASSERT_EQ(data_buffer[3], 0b00000000);
        ASSERT_EQ(data_buffer[4], 0b00000000);
    };

    // Serialize 64-bit VarInt 2147483647
    TEST_F(GSSerializerTest, WriteVarInt_2147483647)
    {
        gs::VarInt value{INT32_MAX};            // 2147483647
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);

        ASSERT_EQ(data_buffer[0], 0b11100001);
        ASSERT_EQ(data_buffer[1], 0b01111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
        ASSERT_EQ(data_buffer[3], 0b11111111);
        ASSERT_EQ(data_buffer[4], 0b11111111);
    };

    // Serialize 64-bit VarInt -2147483649
    TEST_F(GSSerializerTest, WriteVarInt_neg_2147483649)
    {
        gs::VarInt value{-2147483649ll};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 9);

        ASSERT_EQ(data_buffer.GetDataLength(), 9);

        ASSERT_EQ(data_buffer[0], 0b11100010);
        ASSERT_EQ(data_buffer[1], 0b11111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
        ASSERT_EQ(data_buffer[3], 0b11111111);
        ASSERT_EQ(data_buffer[4], 0b11111111);
        ASSERT_EQ(data_buffer[5], 0b01111111);
        ASSERT_EQ(data_buffer[6], 0b11111111);
        ASSERT_EQ(data_buffer[7], 0b11111111);
        ASSERT_EQ(data_buffer[8], 0b11111111);
    };

    // Serialize 64-bit VarInt 2147483648
    TEST_F(GSSerializerTest, WriteVarInt_2147483648)
    {
        gs::VarInt value{2147483648};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 9);

        ASSERT_EQ(data_buffer.GetDataLength(), 9);

        ASSERT_EQ(data_buffer[0], 0b11100010);
        ASSERT_EQ(data_buffer[1], 0b00000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
        ASSERT_EQ(data_buffer[3], 0b00000000);
        ASSERT_EQ(data_buffer[4], 0b00000000);
        ASSERT_EQ(data_buffer[5], 0b10000000);
        ASSERT_EQ(data_buffer[6], 0b00000000);
        ASSERT_EQ(data_buffer[7], 0b00000000);
        ASSERT_EQ(data_buffer[8], 0b00000000);
    };

    // Serialize 64-bit VarInt -9223372036854775808
    TEST_F(GSSerializerTest, WriteVarInt_neg_9223372036854775808)
    {
        gs::VarInt value{INT64_MIN};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 9);

        ASSERT_EQ(data_buffer.GetDataLength(), 9);

        ASSERT_EQ(data_buffer[0], 0b11100010);
        ASSERT_EQ(data_buffer[1], 0b10000000);
        ASSERT_EQ(data_buffer[2], 0b00000000);
        ASSERT_EQ(data_buffer[3], 0b00000000);
        ASSERT_EQ(data_buffer[4], 0b00000000);
        ASSERT_EQ(data_buffer[5], 0b00000000);
        ASSERT_EQ(data_buffer[6], 0b00000000);
        ASSERT_EQ(data_buffer[7], 0b00000000);
        ASSERT_EQ(data_buffer[8], 0b00000000);
    };

    // Serialize 64-bit VarInt 9223372036854775807
    TEST_F(GSSerializerTest, WriteVarInt_9223372036854775807)
    {
        gs::VarInt value{INT64_MAX};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 9);

        ASSERT_EQ(data_buffer.GetDataLength(), 9);

        ASSERT_EQ(data_buffer[0], 0b11100010);
        ASSERT_EQ(data_buffer[1], 0b01111111);
        ASSERT_EQ(data_buffer[2], 0b11111111);
        ASSERT_EQ(data_buffer[3], 0b11111111);
        ASSERT_EQ(data_buffer[4], 0b11111111);
        ASSERT_EQ(data_buffer[5], 0b11111111);
        ASSERT_EQ(data_buffer[6], 0b11111111);
        ASSERT_EQ(data_buffer[7], 0b11111111);
        ASSERT_EQ(data_buffer[8], 0b11111111);
    };

    /////////////////////////////////
    // 16-bit Half Floats
    /////////////////////////////////

    // Test serializing a 16-bit float
    TEST_F(GSSerializerTest, WriteHalfFloat)
    {
        gs::Float16 value{3.14f};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 2);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);

        ASSERT_EQ(data_buffer[0], 0x42);
        ASSERT_EQ(data_buffer[1], 0x48);
    };

    /////////////////////////////////
    // 32-bit Floats
    /////////////////////////////////

    // Test serializing a 32-bit float
    TEST_F(GSSerializerTest, WriteFloat32)
    {
        float value = 3.14159265f;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 4);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x40);
        ASSERT_EQ(data_buffer[1], 0x49);
        ASSERT_EQ(data_buffer[2], 0x0f);
        ASSERT_EQ(data_buffer[3], 0xdb);
    };

    /////////////////////////////////
    // 64-bit Floats
    /////////////////////////////////

    // Test serializing a 64-bit float
    TEST_F(GSSerializerTest, WriteFloat64)
    {
        double value = 3.141592653589793;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 8);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0x40);
        ASSERT_EQ(data_buffer[1], 0x09);
        ASSERT_EQ(data_buffer[2], 0x21);
        ASSERT_EQ(data_buffer[3], 0xfb);
        ASSERT_EQ(data_buffer[4], 0x54);
        ASSERT_EQ(data_buffer[5], 0x44);
        ASSERT_EQ(data_buffer[6], 0x2d);
        ASSERT_EQ(data_buffer[7], 0x18);
    };

    /////////////////////////////////
    // Boolean tests
    /////////////////////////////////

    // Serialize boolean true
    TEST_F(GSSerializerTest, WriteBoolean_true)
    {
        gs::Boolean value = true;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x01);
    };

    // Serialize boolean false
    TEST_F(GSSerializerTest, WriteBoolean_false)
    {
        gs::Boolean value = false;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    /////////////////////////////////
    // Byte tests
    /////////////////////////////////

    // Serialize Byte 0
    TEST_F(GSSerializerTest, WriteByte_0)
    {
        gs::Byte value = 0;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    // Serialize Byte 128
    TEST_F(GSSerializerTest, WriteByte_128)
    {
        gs::Byte value = 128;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x80);
    };

    // Serialize Byte 255
    TEST_F(GSSerializerTest, WriteByte_255)
    {
        gs::Byte value = 255;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0xff);
    };

    /////////////////////////////////
    // String tests
    /////////////////////////////////

    // Serialize String ""
    TEST_F(GSSerializerTest, WriteString_empty)
    {
        gs::String value{};
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    // Serialize String "Hello"
    TEST_F(GSSerializerTest, WriteString_hello)
    {
        gs::String value = "Hello";
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 6);

        ASSERT_EQ(data_buffer.GetDataLength(), 6);

        ASSERT_EQ(data_buffer[0], 0x05);
        ASSERT_EQ(data_buffer[1], 'H');
        ASSERT_EQ(data_buffer[2], 'e');
        ASSERT_EQ(data_buffer[3], 'l');
        ASSERT_EQ(data_buffer[4], 'l');
        ASSERT_EQ(data_buffer[5], 'o');
    };

    // Serialize String that's a little longer
    TEST_F(GSSerializerTest, WriteString_longer)
    {
        gs::String value = "The quick brown fox jumps over the lazy dog. "
                           "The quick brown fox jumps over the lazy dog. "
                           "The quick brown fox jumps over the lazy dog.";
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 136);

        ASSERT_EQ(data_buffer.GetDataLength(), 136);

        ASSERT_EQ(data_buffer[0], 0b10000000);
        ASSERT_EQ(data_buffer[1], 0b10000110);
        for (std::size_t i = 0; i < value.size(); i++)
        {
            ASSERT_EQ(data_buffer[i + 2],
                      static_cast<unsigned char>(value[i]));
        }
    };

    /////////////////////////////////
    // Blob tests
    /////////////////////////////////

    // Serialize Blob ""
    TEST_F(GSSerializerTest, WriteBlob_empty)
    {
        gs::Blob value;
        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(data_buffer.GetDataLength(), 1);

        ASSERT_EQ(data_buffer[0], 0x00);
    };

    // Serialize Blob "Hello"
    TEST_F(GSSerializerTest, WriteBlob_hello)
    {
        gs::Blob value;
        gs::String raw_data = "Hello";

        for (auto c : raw_data) value.push_back(c);

        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 6);

        ASSERT_EQ(data_buffer.GetDataLength(), 6);

        ASSERT_EQ(data_buffer[0], 0x05);
        ASSERT_EQ(data_buffer[1], 'H');
        ASSERT_EQ(data_buffer[2], 'e');
        ASSERT_EQ(data_buffer[3], 'l');
        ASSERT_EQ(data_buffer[4], 'l');
        ASSERT_EQ(data_buffer[5], 'o');
    };

    // Serialize Blob that's a little longer
    TEST_F(GSSerializerTest, WriteBlob_longer)
    {
        gs::Blob value;
        gs::String raw_data = "The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog. "
                              "The quick brown fox jumps over the lazy dog.";

        for (auto c : raw_data) value.push_back(c);

        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 136);

        ASSERT_EQ(data_buffer.GetDataLength(), 136);

        // Check the VarUint length
        ASSERT_EQ(data_buffer[0], 0b10000000);
        ASSERT_EQ(data_buffer[1], 0b10000110);

        // Check the blob data
        for (std::size_t i = 0; i < value.size(); i++)
        {
            ASSERT_EQ(data_buffer[i + 2],
                      static_cast<unsigned char>(value[i]));
        }
    };

    // Serialize Blob that's even longer
    TEST_F(GSSerializerTest, WriteBlob_even_longer)
    {
        gs::Blob value;

        for (unsigned i = 0; i < 1000; i++) value.push_back(i % 256);

        std::size_t l = serializer.Write(data_buffer, value);

        ASSERT_EQ(l, 1002);

        ASSERT_EQ(data_buffer.GetDataLength(), 1002);

        // Check the VarUint length
        ASSERT_EQ(data_buffer[0], 0b10000011);
        ASSERT_EQ(data_buffer[1], 0b11101000);

        // Check the blob data
        for (std::size_t i = 0; i < value.size(); i++)
        {
            ASSERT_EQ(data_buffer[i + 2],
                      static_cast<unsigned char>(i % 256));
        }
    };

} // namespace
