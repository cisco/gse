/*
 *  gs_serializer.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      The Game State Serializer is an object used by the Game State Encoder
 *      to serialize data into a given buffer.
 *
 *  Portability Issues:
 *      The C++ float and double types are assumed to be implemented following
 *      IEEE-754 specification.
 */

#include <cstring>
#include "gs_serializer.h"
#include "half_float.h"

namespace gs
{

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the unsigned integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Uint8 value) const
{
    if (data_buffer) data_buffer->AppendValue(value);

    return sizeof(Uint8);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the unsigned integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Uint16 value) const
{
    if (data_buffer) data_buffer->AppendValue(value);

    return sizeof(Uint16);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the unsigned integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Uint32 value) const
{
    if (data_buffer) data_buffer->AppendValue(value);

    return sizeof(Uint32);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the unsigned integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Uint64 value) const
{
    if (data_buffer) data_buffer->AppendValue(value);

    return sizeof(Uint64);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the signed integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Int8 value) const
{
    if (data_buffer) data_buffer->AppendValue(static_cast<Uint8>(value));

    return sizeof(Int8);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the signed integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Int16 value) const
{
    if (data_buffer)
    {
        data_buffer->AppendValue(static_cast<Uint16>(value));
    }

    return sizeof(Int16);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the signed integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Int32 value) const
{
    if (data_buffer)
    {
        data_buffer->AppendValue(static_cast<Uint32>(value));
    }

    return sizeof(Int32);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write the signed integer of fixed-size to
 *      the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The unsigned integer value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Int64 value) const
{
    if (data_buffer)
    {
        data_buffer->AppendValue(static_cast<Uint64>(value));
    }

    return sizeof(Int64);
}

/*
 *  Serializer::WriteVarUint
 *
 *  Description:
 *      This function will write the unsigned integer as a variable-length
 *      integer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The 64-bit VarUint value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      VarUint is encoded as follows:
 *          [a] 0b + 7 bits for unsigned integer in the range 0 to 127
 *          [b] 10b + 14 bits for unsigned integer in the range 0 to 16383
 *          [c] 110b + 21 bits for unsigned integer 0 to 2097151
 *          [d] 11100001b + 4 octets for a 32-bit unsigned integer
 *          [e] 11100010b + 8 octets for a 64-bit unsigned integer
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              const VarUint &value) const
{
    // [a] Produce a 7-bit unsigned integer
    if (value.value <= 0x7f)
    {
        if (data_buffer)
        {
            data_buffer->AppendValue(static_cast<std::uint8_t>(value.value));
        }

        return sizeof(std::uint8_t);
    }

    // [b] Produce a 14-bit unsigned integer
    if (value.value <= 0x3fff)
    {
        if (data_buffer)
        {
            data_buffer->AppendValue(
                static_cast<std::uint16_t>(value.value | 0x8000));
        }

        return sizeof(std::uint16_t);
    }

    // [c] Produce a 21-bit unsigned integer
    if (value.value <= 0x001f'ffff)
    {
        std::uint32_t i = static_cast<std::uint32_t>(value.value) | 0x00c0'0000;
        if (data_buffer)
        {
            data_buffer->AppendValue(static_cast<std::uint8_t>(i >> 16));
            data_buffer->AppendValue(static_cast<std::uint16_t>(i & 0xffff));
        }

        return sizeof(std::uint8_t) + sizeof(std::uint16_t);
    }

    // [d] Prduce a 32-bit unsigned integer
    if (value.value <= 0xffff'ffff)
    {
        if (data_buffer)
        {
            data_buffer->AppendValue(static_cast<std::uint8_t>(0b1110'0001));
            data_buffer->AppendValue(static_cast<std::uint32_t>(value.value));
        }

        return sizeof(std::uint8_t) + sizeof(std::uint32_t);
    }

    // [e] Produce a 64-bit unsigned integer
    if (data_buffer)
    {
        data_buffer->AppendValue(static_cast<std::uint8_t>(0b1110'0010));
        data_buffer->AppendValue(static_cast<std::uint64_t>(value.value));
    }

    return sizeof(std::uint8_t) + sizeof(std::uint64_t);
}

/*
 *  Serializer::WriteVarInt
 *
 *  Description:
 *      This function will write the signed integer as a variable-length
 *      integer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The 64-bit VarInt value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      VarInt is encoded as follows:
 *          [a] 0b + 7 bits for signed integer in the range -64 to +63
 *          [b] 10b + 14 bits for signed integer in the range -8192 to 8191
 *          [c] 110b + 21 bits for signed integer -1048576 to 1048575
 *          [d] 11100001b + 4 octets for a 32-bit signed integer
 *          [e] 11100010b + 8 octets for a 64-bit signed integer
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                             const VarInt &value) const
{
    // [a] Produce a 7-bit signed integer
    if (((value.value & 0xffff'ffff'ffff'ffc0) == 0xffff'ffff'ffff'ffc0) ||
        ((value.value & 0xffff'ffff'ffff'ffc0) == 0x0000'0000'0000'0000))
    {
        if (data_buffer)
        {
            data_buffer->AppendValue(
                static_cast<std::uint8_t>(value.value & 0x7f));
        }

        return sizeof(std::uint8_t);
    }

    // [b] Produce a 14-bit signed integer
    if (((value.value & 0xffff'ffff'ffff'e000) == 0xffff'ffff'ffff'e000) ||
        ((value.value & 0xffff'ffff'ffff'e000) == 0x0000'0000'0000'0000))
    {
        if (data_buffer)
        {
            data_buffer->AppendValue(
                static_cast<std::uint16_t>((value.value & 0x3fff) | 0x8000));
        }

        return sizeof(std::uint16_t);
    }

    // [c] Produce a 21-bit signed integer
    if (((value.value & 0xffff'ffff'fff0'0000) == 0xffff'ffff'fff0'0000) ||
        ((value.value & 0xffff'ffff'fff0'0000) == 0x0000'0000'0000'0000))
    {
        if (data_buffer)
        {
            std::uint32_t i = (value.value & 0x001f'ffff) | 0x00c0'0000;
            data_buffer->AppendValue(static_cast<std::uint8_t>(i >> 16));
            data_buffer->AppendValue(static_cast<std::uint16_t>(i & 0xffff));
        }

        return sizeof(std::uint8_t) + sizeof(std::uint16_t);
    }

    // [d] Prduce a 32-bit signed integer
    if (((value.value & 0xffff'ffff'8000'0000) == 0xffff'ffff'8000'0000) ||
        ((value.value & 0xffff'ffff'8000'0000) == 0x0000'0000'0000'0000))
    {
        if (data_buffer)
        {
            data_buffer->AppendValue(static_cast<std::uint8_t>(0b1110'0001));
            data_buffer->AppendValue(
                static_cast<std::uint32_t>(value.value & 0xffff'ffff));
        }

        return sizeof(std::uint8_t) + sizeof(std::uint32_t);
    }

    // [e] Produce a 64-bit signed integer
    if (data_buffer)
    {
        data_buffer->AppendValue(static_cast<std::uint8_t>(0b1110'0010));
        data_buffer->AppendValue(static_cast<std::uint64_t>(value.value));
    }

    return sizeof(std::uint8_t) + sizeof(std::uint64_t);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will take a 32-bit floating point value and serialize it
 *       as a 16-bit half floating point value per IEEE 754 at the end of the
 *       given data buffer.  See
 *       https://en.wikipedia.org/wiki/Half-precision_floating-point_format.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The floating point value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              const Float16 &value) const
{
    if (data_buffer)
    {
        data_buffer->AppendValue(FloatToHalfFloat(value.value));
    }

    return sizeof(std::uint16_t);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will take a 32-bit floating point value and serialize it
 *      at the end of the given data buffer.  The floating point value is
 *      assumed to conform to IEEE 754.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The single-precision floating point value to write to the data
 *          buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Float32 value) const
{
    static_assert(sizeof(value) == 4, "expected Float32 to be 32 bits");

    if (data_buffer)
    {
        data_buffer->AppendValue(value);
    }

    return sizeof(std::uint32_t);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will take a 64-bit floating point value and serialize it
 *      at the end of the given data buffer.  The floating point value is
 *      assumed to conform to IEEE 754.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The double-precision floating point value to write to the data
 *          buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Float64 value) const
{
    static_assert(sizeof(value) == 8, "expected Float64 to be 64 bits");

    if (data_buffer)
    {
        data_buffer->AppendValue(value);
    }

    return sizeof(std::uint64_t);
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write a Boolean value to the end of the specified
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The Boolean value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              Boolean value) const
{
    if (value)
    {
        if (data_buffer) data_buffer->AppendValue(static_cast<std::uint8_t>(1));

        return sizeof(std::uint8_t);
    }

    if (data_buffer) data_buffer->AppendValue(static_cast<std::uint8_t>(0));

    return sizeof(std::uint8_t);
}

/*
 *  Write
 *
 *  Description:
 *      This function will write a String to the end of the specified data
 *      buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The String value to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              const String &value) const
{
    std::size_t total_length{};

    // Write out the buffer length as a VarUint
    total_length = Write(data_buffer, VarUint{value.size()});

    // If the string is empty, just return
    if (value.empty()) return total_length;

    // Write out the actual string content if there is a string
    if (data_buffer) data_buffer->AppendValue(value);

    // Update the octet count
    total_length += value.length();

    return total_length;
}

/*
 *  Serializer::Write
 *
 *  Description:
 *      This function will write a vector of elements to the end of the
 *      specified data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The vector of values to write to the data buffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer is a nullptr value.
 *
 *  Comments:
 *      None.
 */
std::size_t Serializer::Write(std::unique_ptr<DataBuffer<>> &data_buffer,
                              const Blob &value) const
{
    std::size_t total_length{};

    // Write out the number of Blob bytes that follow
    total_length = Write(data_buffer, VarUint{value.size()});

    // If the string is empty, just return
    if (value.empty()) return total_length;

    // Write out the actual string content if there is a string
    if (data_buffer) data_buffer->AppendValue(value);

    // Update the octet count
    total_length += value.size();

    return total_length;
}

} // namespace gs
