/*
 *  test_databuffer.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module will test the DataBuffer object, exercising the
 *      various functions and operators defined in that class.
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

#include <string.h>
#include <cstdint>
#include "data_buffer.h"
#include "gtest/gtest.h"

namespace {

    // The fixture for testing class DataBuffer
    class DataBufferTest : public ::testing::Test
    {
        protected:
            DataBufferTest() : data_buffer(1500)
            {
            }

            ~DataBufferTest() = default;

            gs::DataBuffer data_buffer;
    };

    // Test DataBuffer constructor
    TEST_F(DataBufferTest, Constructor)
    {
        ASSERT_EQ(data_buffer.GetDataLength(), 0);
        ASSERT_TRUE(data_buffer.Empty());
        ASSERT_NE(data_buffer.GetBufferPointer(), nullptr);
    }

    // Test DataBuffer constructor
    TEST_F(DataBufferTest, Constructor2)
    {
        // Create a new empty buffer
        gs::DataBuffer data_buffer;

        ASSERT_EQ(data_buffer.GetDataLength(), 0);
        ASSERT_TRUE(data_buffer.Empty());
        ASSERT_EQ(data_buffer.GetBufferPointer(), nullptr);

        // The following should throw an exception
        bool exception_thrown = false;
        try
        {
            data_buffer.AppendValue(std::uint32_t(0));
        }
        catch (const gs::DataBufferException&)
        {
            exception_thrown = true;
        }
        ASSERT_TRUE(exception_thrown);
    }

    // Test DataBuffer constructor with OctetString
    TEST_F(DataBufferTest, ConstructorOctetString)
    {
        gs::OctetString key =
        {
            0x58, 0x40, 0xDF, 0x6E, 0x29, 0xB0, 0x2A, 0XF1,
            0xAB, 0x49, 0x3B, 0x70, 0x5B, 0xF1, 0x6E, 0XA1,
            0xAE, 0x83, 0x38, 0xF4, 0xDC, 0xC1, 0x76, 0XA8
        };

        // Create a new DataBuffer, initializing it with the OctetString
        gs::DataBuffer db(key);

        ASSERT_EQ(db.GetDataLength(), key.size());

        ASSERT_EQ(memcmp(db.GetBufferPointer(), &key[0], key.size()), 0);
    }

    // Test DataBuffer constructor with existing buffer
    TEST_F(DataBufferTest, ConstructorExistingBuffer)
    {
        unsigned char key[] =
        {
            0x58, 0x40, 0xDF, 0x6E, 0x29, 0xB0, 0x2A, 0XF1,
            0xAB, 0x49, 0x3B, 0x70, 0x5B, 0xF1, 0x6E, 0XA1,
            0xAE, 0x83, 0x38, 0xF4, 0xDC, 0xC1, 0x76, 0XA8
        };

        // Create a new DataBuffer, initializing it they given buffer
        gs::DataBuffer db(key, sizeof(key), sizeof(key));

        ASSERT_EQ(db.GetDataLength(), sizeof(key));

        ASSERT_EQ(memcmp(db.GetBufferPointer(), key, sizeof(key)), 0);
    }

    // Test Buffer copy via assignment operator
    TEST_F(DataBufferTest, Copy)
    {
        std::uint32_t i = 0x01020304;
        std::uint32_t j = 0;
        gs::DataBuffer db(1);

        // Put some data into the DataBuffer
        data_buffer.SetDataLength(sizeof(std::uint32_t));
        data_buffer.SetValue(i, 0);

        // Assign the DataBuffer to the local one
        db = data_buffer;

        // Check the contents of the local DataBuffer
        db.GetValue(j, 0);

        // Same value should be inside
        ASSERT_EQ(i, j);

        // Should be the same length
        ASSERT_EQ(data_buffer.GetDataLength(), db.GetDataLength());

        // Should be different buffers
        ASSERT_NE(db.GetBufferPointer(), data_buffer.GetBufferPointer());
    }

    // Test Buffer copy via copy constructor
    TEST_F(DataBufferTest, Copy2)
    {
        std::uint32_t i = 0x01020304;
        std::uint32_t j = 0;

        // Put some data into the DataBuffer
        data_buffer.SetDataLength(sizeof(std::uint32_t));
        data_buffer.SetValue(i, 0);

        // Assign the DataBuffer to the local one
        gs::DataBuffer db(data_buffer);

        // Check the contents of the local DataBuffer
        db.GetValue(j, 0);

        // Same value should be inside
        ASSERT_EQ(i, j);

        // Should be the same length
        ASSERT_EQ(data_buffer.GetDataLength(), db.GetDataLength());

        // Should be different buffers
        ASSERT_NE(db.GetBufferPointer(), data_buffer.GetBufferPointer());
    }

    // Test data packet move constructor
    TEST_F(DataBufferTest, Move)
    {
        gs::OctetString raw_data = { 0x01, 0x02, 0x03, 0x04 };

        data_buffer.AppendValue(raw_data);

        gs::DataBuffer db(std::move(data_buffer));

        // Check for inequality
        ASSERT_TRUE(db != data_buffer);

        // The old data packet should be empty
        ASSERT_TRUE(data_buffer.Empty());

        // The new data packet should have 4 octets inside
        ASSERT_EQ(db.GetDataLength(), 4);
    }

    // Test DataBuffer (in)equality operators
    TEST_F(DataBufferTest, Equality)
    {
        std::uint32_t i = 0x01020304;
        std::uint32_t j = 0;
        gs::DataBuffer db(1);

        // Check that objects are equal to themselves
        ASSERT_TRUE(db == db);
        ASSERT_TRUE(data_buffer == data_buffer);

        // Put some data into the DataPacket
        data_buffer.SetDataLength(sizeof(std::uint32_t));
        data_buffer.SetValue(i, 0);

        // Check for inequality
        ASSERT_TRUE(db != data_buffer);

        db = data_buffer;

        // Check for equality
        ASSERT_TRUE(db == data_buffer);

        // Should be different buffers
        ASSERT_NE(db.GetBufferPointer(), data_buffer.GetBufferPointer());
    }

    // Test functions to get buffer pointers
    TEST_F(DataBufferTest, GetBufferPointer)
    {
        ASSERT_EQ(data_buffer.GetBufferPointer(),
                  data_buffer.GetMutableBufferPointer());
    }

    // Test data packet TakeBufferOwnership function
    TEST_F(DataBufferTest, TakeBufferOwnership)
    {
        unsigned char *p;
        unsigned char *q;
        std::uint32_t i = 0x01020304;

        // Put some data into the DataPacket
        data_buffer.SetDataLength(sizeof(std::uint32_t));
        data_buffer.SetValue(i, 0);

        // Get a pointer to the buffer
        p = data_buffer.GetMutableBufferPointer();

        // Take ownership, assigning to a different variable
        q = data_buffer.TakeBufferOwnership();

        // Should be the same pointer
        ASSERT_EQ(p, q);

        // The data packet should be empty now that we've taken the buffer
        ASSERT_TRUE(data_buffer.Empty());

        // Check to see it has no buffer now
        ASSERT_EQ(data_buffer.GetBufferPointer(), nullptr);

        // Free the memory (since we now own it)
        delete[] q;
    }

    // Test data packet SetBuffer function
    TEST_F(DataBufferTest, SetBuffer)
    {
        // Allocate some memory
        unsigned char *p = new unsigned char[700];

        // Ensure it's not null
        ASSERT_NE(p, nullptr);

        // Assign this buffer, telling the DataPacket to take ownership
        data_buffer.SetBuffer(p, 700, 0, true);

        // Does the data packet buffer match?
        ASSERT_EQ(data_buffer.GetBufferPointer(), p);
    }

    // Test to set/get the data length and Empty() function
    TEST_F(DataBufferTest, SetGetDataLength)
    {
        bool exception_thrown = false;

        // Data packet should be empty
        ASSERT_TRUE(data_buffer.Empty());

        // Set it to something value
        data_buffer.SetDataLength(32);
        ASSERT_EQ(32, data_buffer.GetDataLength());

        // Now it is not empty
        ASSERT_FALSE(data_buffer.Empty());

        try
        {
            // Try setting it to something larger than the buffer
            data_buffer.SetDataLength(2500);
        }
        catch (const gs::DataBufferException &)
        {
            exception_thrown = true;
        }
        ASSERT_TRUE(exception_thrown);
    }

    // Test set/get std::uint32_t
    TEST_F(DataBufferTest, SetValue32)
    {
        std::uint32_t i = 0x7F010203;
        std::uint32_t j;
        unsigned char *p;

        // Set the data length to something we can work with
        data_buffer.SetDataLength(16);
        data_buffer.SetValue(i, 8);

        // Check each octet for network byte order
        p = data_buffer.GetMutableBufferPointer() + 8;
        ASSERT_EQ(*(p + 0), 0x7f);
        ASSERT_EQ(*(p + 1), 0x01);
        ASSERT_EQ(*(p + 2), 0x02);
        ASSERT_EQ(*(p + 3), 0x03);

        // Get the value
        data_buffer.GetValue(j, 8);

        // Ensure the value matches
        ASSERT_EQ(i, j);
    }

    // Test set/get std::uint64_t
    TEST_F(DataBufferTest, SetValue64)
    {
        std::uint64_t i = 0x0102030405060708;
        std::uint64_t j;
        unsigned char *p;

        // Set the data length to something we can work with
        data_buffer.SetDataLength(24);
        data_buffer.SetValue(i, 8);

        // Check each octet for network byte order
        p = data_buffer.GetMutableBufferPointer() + 8;
        ASSERT_EQ(*(p + 0), 0x01);
        ASSERT_EQ(*(p + 1), 0x02);
        ASSERT_EQ(*(p + 2), 0x03);
        ASSERT_EQ(*(p + 3), 0x04);
        ASSERT_EQ(*(p + 4), 0x05);
        ASSERT_EQ(*(p + 5), 0x06);
        ASSERT_EQ(*(p + 6), 0x07);
        ASSERT_EQ(*(p + 7), 0x08);

        // Get the value
        data_buffer.GetValue(j, 8);

        // Ensure the value matches
        ASSERT_EQ(i, j);
    }

    // Test set/get std::uint16_t
    TEST_F(DataBufferTest, SetValue16)
    {
        std::uint16_t i = 0xA1B2;
        std::uint16_t j;
        unsigned char *p;

        // Set the data length to something we can work with
        data_buffer.SetDataLength(16);
        data_buffer.SetValue(i, 2);

        // Check each octet for network byte order
        p = data_buffer.GetMutableBufferPointer() + 2;
        ASSERT_EQ(*(p + 0), 0xA1);
        ASSERT_EQ(*(p + 1), 0xB2);

        // Get the value
        data_buffer.GetValue(j, 2);

        // Ensure the value matches
        ASSERT_EQ(i, j);
    }

    // Test set/get std::uint8_t
    TEST_F(DataBufferTest, SetValue8)
    {
        std::uint8_t i = 0xC1;
        std::uint8_t j;
        unsigned char *p;

        // Set the data length to something we can work with
        data_buffer.SetDataLength(16);
        data_buffer.SetValue(i, 3);

        // Check each octet for network byte order
        p = data_buffer.GetMutableBufferPointer() + 3;
        ASSERT_EQ(*(p + 0), 0xC1);

        // Get the value
        data_buffer.GetValue(j, 3);

        // Ensure the value matches
        ASSERT_EQ(i, j);
    }

    // Test set/get char data
    TEST_F(DataBufferTest, SetValueChar)
    {
        const char *data = "abcd";
        unsigned char dcopy[4];
        unsigned char *p;

        // Set the data length to something we can work with
        data_buffer.SetDataLength(16);
        data_buffer.SetValue(reinterpret_cast<const unsigned char *>(data),
                              4,
                              4);

        // Check each octet for network byte order
        p = data_buffer.GetMutableBufferPointer() + 4;
        ASSERT_EQ(*(p + 0), 'a');
        ASSERT_EQ(*(p + 1), 'b');
        ASSERT_EQ(*(p + 2), 'c');
        ASSERT_EQ(*(p + 3), 'd');

        // Get the value
        data_buffer.GetValue(dcopy, 4, 4);

        // Ensure the value matches
        ASSERT_EQ(memcmp(data, dcopy, 4), 0);
    }

    // Test setting OctetString
    TEST_F(DataBufferTest, SetValueOctetString)
    {
        gs::OctetString key =
        {
            0x58, 0x40, 0xDF, 0x6E, 0x29, 0xB0, 0x2A, 0XF1,
            0xAB, 0x49, 0x3B, 0x70, 0x5B, 0xF1, 0x6E, 0XA1,
            0xAE, 0x83, 0x38, 0xF4, 0xDC, 0xC1, 0x76, 0XA8
        };

        // Set the contents of the data buffer
        data_buffer.SetValue(key, 0);

        ASSERT_EQ(memcmp(data_buffer.GetBufferPointer(), &key[0], key.size()),
                  0);
    }

    // Test getting OctetString
    TEST_F(DataBufferTest, GetValueOctetString)
    {
        const char *data = "abcd";
        const unsigned char *p;
        gs::OctetString os;

        // Set the data length to something we can work with
        data_buffer.SetDataLength(16);
        data_buffer.SetValue(reinterpret_cast<const unsigned char *>(data),
                             0,
                             4);

        // Get the value
        data_buffer.GetValue(os, 0, 4);

        // This should have 4 octets inside
        ASSERT_EQ(os.size(), 4);

        // Get a pointer the buffer
        p = data_buffer.GetBufferPointer();

        // Ensure the value matches
        for (auto &c : os) ASSERT_EQ(c, *p++);
    }

    // Test for exceptions writing beyond buffer length
    TEST_F(DataBufferTest, DataLengthExceptions)
    {
        bool exception_thrown = false;
        unsigned char value = {};

        try
        {
            // 0..1499 should be valid, anything >= 1500 should fail
            data_buffer.SetValue(value, 1500);
        }
        catch (const gs::DataBufferException &)
        {
            exception_thrown = true;
        }
        ASSERT_TRUE(exception_thrown);
    }

    // Test append char data
    TEST_F(DataBufferTest, AppendValueChar)
    {
        const char *data0 = "abcd";
        const char *data1 = "efgh";
        unsigned char dcopy[4];
        unsigned char *p;

        // Append the octets in data
        data_buffer.AppendValue(reinterpret_cast<const unsigned char *>(data0),
                                4);

        // Ensure the data length is 4
        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        // Check each octet for network byte order
        p = data_buffer.GetMutableBufferPointer();
        ASSERT_EQ(*(p + 0), 'a');
        ASSERT_EQ(*(p + 1), 'b');
        ASSERT_EQ(*(p + 2), 'c');
        ASSERT_EQ(*(p + 3), 'd');

        data_buffer.AppendValue(reinterpret_cast<const unsigned char *>(data1),
                                4);

        // Ensure the data length is 8
        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        // Get the value
        data_buffer.GetValue(dcopy, 4, 4);

        // Ensure the value matches
        ASSERT_EQ(memcmp(data1, dcopy, 4), 0);
    }

    // Test append std::uint8_t
    TEST_F(DataBufferTest, AppendValue8)
    {
        std::vector<std::uint8_t> octets =
        {
            0xCC, 0xAF, 0xDE, 0xAD, 0xBE, 0xEF, 0xCA, 0x23
        };

        // Append the octets in data
        for (auto octet : octets) data_buffer.AppendValue(octet);

        // Validate the data length
        ASSERT_EQ(data_buffer.GetDataLength(),
                  octets.size() * sizeof(std::uint8_t));

        // Ensure the values in the buffer match
        for (int i = 0; i < octets.size(); i++)
        {
            std::uint8_t octet;
            data_buffer.GetValue(octet, i * sizeof(std::uint8_t));
            ASSERT_EQ(octet, octets[i]);
        }
    }

    // Test append std::uint16_t
    TEST_F(DataBufferTest, AppendValue16)
    {
        std::vector<std::uint16_t> values =
        {
            0xDEAD, 0xCC10, 0xBEEF, 0xCA16
        };

        // Append the octets in data
        for (auto value : values) data_buffer.AppendValue(value);

        // Validate the data length
        ASSERT_EQ(data_buffer.GetDataLength(),
                  values.size() * sizeof(std::uint16_t));

        // Ensure the values in the buffer match
        for (int i = 0; i < values.size(); i++)
        {
            std::uint16_t value;
            data_buffer.GetValue(value, i * sizeof(std::uint16_t));
            ASSERT_EQ(value, values[i]);
        }
    }

    // Test append std::uint32_t
    TEST_F(DataBufferTest, AppendValue32)
    {
        std::vector<std::uint32_t> values =
        {
            0xCAFEBABE, 0xCC10B4C7, 0x1A2B3C4D, 0xDEADBEEF, 0xCA16DEAF
        };

        // Append the octets in data
        for (auto value : values) data_buffer.AppendValue(value);

        // Validate the data length
        ASSERT_EQ(data_buffer.GetDataLength(),
                  values.size() * sizeof(std::uint32_t));

        // Ensure the values in the buffer match
        for (int i = 0; i < values.size(); i++)
        {
            std::uint32_t value;
            data_buffer.GetValue(value, i * sizeof(std::uint32_t));
            ASSERT_EQ(value, values[i]);
        }
    }

    // Test append std::uint64_t
    TEST_F(DataBufferTest, AppendValue64)
    {
        std::vector<std::uint64_t> values =
        {
            0x0001020304060708, 0x1011121314161718,
            0x2021222324262728, 0x3031323334363738,
            0x4041424344464748, 0x5051525354565758
        };

        // Append the octets in data
        for (auto value : values) data_buffer.AppendValue(value);

        // Validate the data length
        ASSERT_EQ(data_buffer.GetDataLength(),
                  values.size() * sizeof(std::uint64_t));

        // Ensure the values in the buffer match
        for (std::size_t i = 0; i < values.size(); i++)
        {
            std::uint64_t value;
            data_buffer.GetValue(value, i * sizeof(std::uint64_t));
            ASSERT_EQ(value, values[i]);
        }
    }

    // Test append float
    TEST_F(DataBufferTest, AppendValueFloat)
    {
        float value = 3.14159265f;

        data_buffer.AppendValue(value);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);

        ASSERT_EQ(data_buffer[0], 0x40);
        ASSERT_EQ(data_buffer[1], 0x49);
        ASSERT_EQ(data_buffer[2], 0x0f);
        ASSERT_EQ(data_buffer[3], 0xdb);
    }

    // Test append double
    TEST_F(DataBufferTest, AppendValueDouble)
    {
        double value = 3.141592653589793;

        data_buffer.AppendValue(value);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);

        ASSERT_EQ(data_buffer[0], 0x40);
        ASSERT_EQ(data_buffer[1], 0x09);
        ASSERT_EQ(data_buffer[2], 0x21);
        ASSERT_EQ(data_buffer[3], 0xfb);
        ASSERT_EQ(data_buffer[4], 0x54);
        ASSERT_EQ(data_buffer[5], 0x44);
        ASSERT_EQ(data_buffer[6], 0x2d);
        ASSERT_EQ(data_buffer[7], 0x18);
    }

    // Test read raw octets
    TEST_F(DataBufferTest, ReadValue)
    {
        const unsigned char buffer[] = "hello";
        unsigned char read_buffer[5];

        // Added 'hello' to the buffer
        data_buffer.AppendValue(buffer, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        // Read the data
        data_buffer.ReadValue(read_buffer, 5);

        ASSERT_EQ(data_buffer.GetReadLength(), 5);

        // Compare the read data
        for (std::size_t i = 0; i < 5; i++)
        {
            ASSERT_EQ(buffer[i], read_buffer[i]);
        }
    }

    // Test read string
    TEST_F(DataBufferTest, ReadString)
    {
        const unsigned char buffer[] = "hello";
        std::string str;

        // Added 'hello' to the buffer
        data_buffer.AppendValue(buffer, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        // Read the data
        data_buffer.ReadValue(str, 5);

        ASSERT_EQ(data_buffer.GetReadLength(), 5);
        ASSERT_EQ(str.length(), 5);

        // Compare the read data
        for (std::size_t i = 0; i < 5; i++)
        {
            ASSERT_EQ(buffer[i], str[i]);
        }
    }

    // Test read OctetString (vector)
    TEST_F(DataBufferTest, ReadOctetString)
    {
        const unsigned char buffer[] = "hello";
        gs::OctetString octets;

        // Added 'hello' to the buffer
        data_buffer.AppendValue(buffer, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        // Read the data
        data_buffer.ReadValue(octets, 5);

        ASSERT_EQ(data_buffer.GetReadLength(), 5);
        ASSERT_EQ(octets.size(), 5);

        // Compare the read data
        for (std::size_t i = 0; i < 5; i++)
        {
            ASSERT_EQ(buffer[i], octets[i]);
        }
    }

    // Test reading a single octet
    TEST_F(DataBufferTest, ReadOctet)
    {
        const unsigned char buffer[] = "hello";
        std::uint8_t octet;

        // Added 'hello' to the buffer
        data_buffer.AppendValue(buffer, 5);

        ASSERT_EQ(data_buffer.GetDataLength(), 5);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        // Read each octet and test
        for (std::size_t i = 0; i < 5; i++)
        {
            data_buffer.ReadValue(octet);
            ASSERT_EQ(data_buffer.GetReadLength(), i + 1);
            ASSERT_EQ(octet, buffer[i]);
        }
    }

    // Test reading a 16-bit integer
    TEST_F(DataBufferTest, ReadShortInt)
    {
        std::uint16_t value = 0x27AF;
        std::uint16_t read_value = 0x0000;

        // Added the value to the buffer
        data_buffer.AppendValue(value);

        ASSERT_EQ(data_buffer.GetDataLength(), 2);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        data_buffer.ReadValue(read_value);
        ASSERT_EQ(data_buffer.GetReadLength(), 2);

        ASSERT_EQ(read_value, value);
    }

    // Test reading a 32-bit integer
    TEST_F(DataBufferTest, ReadInt)
    {
        std::uint32_t value = 0xdeadbeef;
        std::uint32_t read_value = 0x00000000;

        // Added the value to the buffer
        data_buffer.AppendValue(value);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        data_buffer.ReadValue(read_value);

        ASSERT_EQ(data_buffer.GetReadLength(), 4);

        ASSERT_EQ(read_value, value);
    }

    // Test reading a 64-bit integer
    TEST_F(DataBufferTest, ReadLongInt)
    {
        std::uint64_t value = 0xdeadbeefcafebabe;
        std::uint64_t read_value = 0x0000000000000000;

        // Added the value to the buffer
        data_buffer.AppendValue(value);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        data_buffer.ReadValue(read_value);

        ASSERT_EQ(data_buffer.GetReadLength(), 8);

        ASSERT_EQ(read_value, value);
    }

    // Test reading a 32-bit floating point value
    TEST_F(DataBufferTest, ReadFloat)
    {
        float value = 3.14159f;
        float read_value = 0.0f;

        // Added the value to the buffer
        data_buffer.AppendValue(value);

        ASSERT_EQ(data_buffer.GetDataLength(), 4);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        data_buffer.ReadValue(read_value);

        ASSERT_EQ(data_buffer.GetReadLength(), 4);

        ASSERT_EQ(read_value, value);
    }

    // Test reading a 64-bit floating point value
    TEST_F(DataBufferTest, ReadDouble)
    {
        double value = 3.141592653589793;
        double read_value = 0.0;

        // Added the value to the buffer
        data_buffer.AppendValue(value);

        ASSERT_EQ(data_buffer.GetDataLength(), 8);
        ASSERT_EQ(data_buffer.GetReadLength(), 0);

        data_buffer.ReadValue(read_value);

        ASSERT_EQ(data_buffer.GetReadLength(), 8);

        ASSERT_EQ(read_value, value);
    }

    // Test mixed reads
    TEST_F(DataBufferTest, ReadMixedData)
    {
        std::string hello = "Hello, World!";
        std::uint16_t port = 5006;
        std::uint32_t value_32 = 0x01020304;
        std::uint64_t value_64 = 0xdeadbeefcafebabe;
        double value_double = 3.141592653589793;
        std::string fin = "The End!";

        // Added the values to the buffer
        data_buffer.AppendValue(hello);
        data_buffer.AppendValue(port);
        data_buffer.AppendValue(value_32);
        data_buffer.AppendValue(value_64);
        data_buffer.AppendValue(value_double);
        data_buffer.AppendValue(fin);

        std::string verify_hello;
        std::uint16_t verify_port;
        std::uint32_t verify_value_32;
        std::uint64_t verify_value_64;
        double verify_value_double;
        std::string verify_fin;

        // Read the data
        data_buffer.ReadValue(verify_hello, hello.size());
        data_buffer.ReadValue(verify_port);
        data_buffer.ReadValue(verify_value_32);
        data_buffer.ReadValue(verify_value_64);
        data_buffer.ReadValue(verify_value_double);
        data_buffer.ReadValue(verify_fin, fin.size());

        // Ensure all data is read
        ASSERT_EQ(data_buffer.GetDataLength(), data_buffer.GetReadLength());

        // Compare values
        ASSERT_EQ(hello, verify_hello);
        ASSERT_EQ(port, verify_port);
        ASSERT_EQ(value_32, verify_value_32);
        ASSERT_EQ(value_64, verify_value_64);
        ASSERT_EQ(value_double, verify_value_double);
        ASSERT_EQ(fin, verify_fin);
    }

    // Test mixed reads
    TEST_F(DataBufferTest, AdvanceRead)
    {
        std::string hello = "Hello, World!";
        std::uint16_t port = 5006;
        std::uint32_t value_32 = 0x01020304;
        std::uint64_t value_64 = 0xdeadbeefcafebabe;
        double value_double = 3.141592653589793;
        std::string fin = "The End!";

        // Added the values to the buffer
        data_buffer.AppendValue(hello);
        data_buffer.AppendValue(port);
        data_buffer.AppendValue(value_32);
        data_buffer.AppendValue(value_64);
        data_buffer.AppendValue(value_double);
        data_buffer.AppendValue(fin);

        std::string verify_hello;
        std::uint16_t verify_port;
        std::uint64_t verify_value_64;
        double verify_value_double;
        std::string verify_fin;

        // Read the data
        data_buffer.ReadValue(verify_hello, hello.size());
        data_buffer.ReadValue(verify_port);

        // Advance over the 32-bit field written
        data_buffer.AdvanceReadLength(sizeof(std::uint32_t));

        data_buffer.ReadValue(verify_value_64);
        data_buffer.ReadValue(verify_value_double);
        data_buffer.ReadValue(verify_fin, fin.size());

        // Ensure all data is read
        ASSERT_EQ(data_buffer.GetDataLength(), data_buffer.GetReadLength());

        // Compare values
        ASSERT_EQ(hello, verify_hello);
        ASSERT_EQ(port, verify_port);
        ASSERT_EQ(value_64, verify_value_64);
        ASSERT_EQ(value_double, verify_value_double);
        ASSERT_EQ(fin, verify_fin);
    }

} // namespace
