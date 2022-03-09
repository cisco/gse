/*
 *  gs_deserializer.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      The Game State Deserializer is an object used by the Game State Encoder
 *      to deserialize data from a given buffer.
 *
 *  Portability Issues:
 *      The C++ float and double types are assumed to be implemented following
 *      IEEE-754 specification.
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
#include "gs_deserializer.h"
#include "half_float.h"

namespace gs
{

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read an unsigned integer of fixed-size from
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Uint8 &value) const
{
    data_buffer->ReadValue(value);

    return sizeof(Uint8);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read an unsigned integer of fixed-size from
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Uint16 &value) const
{
    data_buffer->ReadValue(value);

    return sizeof(Uint16);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read an unsigned integer of fixed-size from
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Uint32 &value) const
{
    data_buffer->ReadValue(value);

    return sizeof(Uint32);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read an unsigned integer of fixed-size from
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Uint64 &value) const
{
    data_buffer->ReadValue(value);

    return sizeof(Uint64);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a signed integer of fixed-size from the
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Int8 &value) const
{
    data_buffer->ReadValue(reinterpret_cast<Uint8&>(value));

    return sizeof(Int8);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a signed integer of fixed-size from the
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t  Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Int16 &value) const
{
    data_buffer->ReadValue(reinterpret_cast<Uint16&>(value));

    return sizeof(Int16);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a signed integer of fixed-size from the
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t  Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Int32 &value) const
{
    data_buffer->ReadValue(reinterpret_cast<Uint32&>(value));

    return sizeof(Int32);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a signed integer of fixed-size from the
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The unsigned integer to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t  Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Int64 &value) const
{
    data_buffer->ReadValue(reinterpret_cast<Uint64&>(value));

    return sizeof(Int64);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a VarUint from the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The VarUint to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t  Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                                VarUint &value) const
{
    std::uint8_t octet;

    // Read the first octet
    data_buffer->ReadValue(octet);

    // Is this a single octet value?
    if ((octet & 0b1000'0000) == 0)
    {
        value.value = octet & 0b0111'1111;

        return sizeof(std::uint8_t);
    }

    // Is this a two-octet value?
    if ((octet & 0b1100'0000) == 0b1000'0000)
    {
        // Store the lower bits
        value.value = octet & 0b0011'1111;

        // Read the next octet
        data_buffer->ReadValue(octet);
        value.value = (value.value << 8) | octet;

        return sizeof(std::uint16_t);
    }

    // Is this a three-octet value?
    if ((octet & 0b1110'0000) == 0b1100'0000)
    {
        std::uint16_t lower_bits;

        // Store the lower bits
        value.value = octet & 0b0001'1111;

        // Read the next 16 bits
        data_buffer->ReadValue(lower_bits);

        // Update the value
        value.value = (value.value << 16) | lower_bits;

        return sizeof(std::uint8_t) + sizeof(std::uint16_t);
    }

    // Do we have a 32-bit value stored?
    if (octet == 0b1110'0001)
    {
        std::uint32_t lower_bits;

        // Read the 32-bit value
        data_buffer->ReadValue(lower_bits);

        // Assign the return value
        value.value = lower_bits;

        return sizeof(std::uint8_t) + sizeof(std::uint32_t);
    }

    // Do we have a 64-bit value stored?
    if (octet == 0b1110'0010)
    {
        // Read the 64-bit value
        data_buffer->ReadValue(value.value);

        return sizeof(std::uint8_t) + sizeof(std::uint64_t);
    }

    // We have an invalid VarUint
    throw DeserializerException("Invalid VarUint in the data buffer");
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a VarInt from the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The VarInt to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               VarInt &value) const
{
    std::uint8_t octet;

    // Read the first octet
    data_buffer->ReadValue(octet);

    // Is this a single octet value?
    if ((octet & 0b1000'0000) == 0)
    {
        value.value = octet & 0b0111'1111;

        // Check the sign bit, ensure proper negative result
        if (octet & 0b0100'0000) value.value |= 0xffff'ffff'ffff'ff80;

        return sizeof(std::uint8_t);
    }

    // Is this a two-octet value?
    if ((octet & 0b1100'0000) == 0b1000'0000)
    {
        // Store the lower bits
        value.value = octet & 0b0011'1111;

        // Check the sign bit, ensure proper negative result
        if (octet & 0b0010'0000) value.value |= 0xffff'ffff'ffff'ffc0;

        // Read the next octet
        data_buffer->ReadValue(octet);
        value.value = (value.value << 8) | octet;

        return sizeof(std::uint16_t);
    }

    // Is this a three-octet value?
    if ((octet & 0b1110'0000) == 0b1100'0000)
    {
        std::uint16_t lower_bits;

        // Store the lower bits
        value.value = octet & 0b0001'1111;

        // Check the sign bit, ensure proper negative result
        if (octet & 0b0001'0000) value.value |= 0xffff'ffff'ffff'ffe0;

        // Read the next two octets
        data_buffer->ReadValue(lower_bits);
        value.value = (value.value << 16) | lower_bits;

        return sizeof(std::uint8_t) + sizeof(std::uint16_t);
    }

    // Do we have a 32-bit value stored?
    if (octet == 0b1110'0001)
    {
        std::uint32_t lower_bits;

        // Read the 32-bit value
        data_buffer->ReadValue(lower_bits);

        // Assign the return value
        value.value = lower_bits;

        // Check the sign bit, ensure proper negative result
        if (lower_bits & 0x8000'0000) value.value |= 0xffff'ffff'0000'0000;

        return sizeof(std::uint8_t) + sizeof(std::uint32_t);
    }

    // Do we have a 64-bit value stored?
    if (octet == 0b1110'0010)
    {
        // Read the 64-bit value
        data_buffer->ReadValue(reinterpret_cast<Uint64&>(value.value));

        return sizeof(std::uint8_t) + sizeof(std::uint64_t);
    }

    // We have an invalid VarUint
    throw DeserializerException("Invalid VarInt in the data buffer");
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a Float16 from the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The Float16 to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Float16 &value) const
{
    std::uint16_t half_float;

    // Read the 16-bit value
    data_buffer->ReadValue(half_float);

    // Convert the value to a float
    value.value = HalfFloatToFloat(half_float);

    return sizeof(std::uint16_t);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a 32-bit floating point value from the data
 *      buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The floating point variable to receive the value read from the
 *          buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Float32 &value) const
{
    // Read the 32-bit value
    data_buffer->ReadValue(value);

    return sizeof(std::uint32_t);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a 64-bit double precision floating point value
 *      from the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The double precision floating point variable to receive the value
 *          read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Float64 &value) const
{
    // Read the 64-bit value
    data_buffer->ReadValue(value);

    return sizeof(std::uint64_t);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a boolean value from the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The Boolean variable to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Boolean &value) const
{
    // Read the 64-bit value
    data_buffer->ReadValue(reinterpret_cast<std::uint8_t&>(value));

    return sizeof(std::uint8_t);
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a String value from the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The String variable to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               String &value) const
{
    std::size_t read_length;
    VarUint length;

    // Read the length of the String
    read_length = Read(data_buffer, length);

    // Read the actual String
    data_buffer->ReadValue(value, length.value);

    // Update the read length
    read_length += value.length();

    return read_length;
}

/*
 *  Deserializer::Read
 *
 *  Description:
 *      This function will read a Blob value from the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The Blob variable to receive the value read from the buffer.
 *
 *  Returns:
 *      Number of octets read from the buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Deserializer::Read(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Blob &value) const
{
    std::size_t read_length;
    VarUint length;

    // Read the length of the Blob
    read_length = Read(data_buffer, length);

    // Read the actual Blob
    data_buffer->ReadValue(value, length.value);

    // Update the read length
    read_length += value.size();

    return read_length;
}

} // namespace gs
