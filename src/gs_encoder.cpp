/*
 *  gs_encoder.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      The Game State Encoder is an object that will encode and game state
 *      data into a given data buffer.
 *
 *      The Encode functions will cause an EncoderException exception to
 *      be thrown if an attempt is made to serialize into a buffer
 *      where there is insufficent space.
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

#include <variant>
#include "gs_encoder.h"

namespace gs
{

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write a vector of GSObject object to the given
 *      buffer, appending the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The objects to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer, const GSObjects &value)
{
    std::size_t total_octets = 0;
    std::size_t object_count = 0;

    // Loop over the vector of objects, existing if buffer space is exhausted
    for (auto &object : value)
    {
        auto [count, octets] = Encode(data_buffer, object);
        if (count == 0) break;
        total_octets += octets;
        object_count += count;
    }

    return {object_count, total_octets};
}

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write a GSObject object to the given buffer,
 *      appending the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer, const GSObject &value)
{
    return std::visit([&](const auto &value) -> EncodeResult
                      {
                          return Encode(data_buffer, value);
                      },
                      value);
}

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write an Object1 object to the given buffer, appending
 *      the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer, const Object1 &value)
{
    std::size_t total_length{};
    Length data_length{};

    // Determine space required for this object
    data_length.value = Serialize(null_buffer, value.id) +
                        Serialize(null_buffer, value.time) +
                        Serialize(null_buffer, value.position) +
                        Serialize(null_buffer, value.rotation) +
                        Serialize(null_buffer, value.scale);

    if (value.parent.has_value())
    {
        data_length.value += Serialize(null_buffer, value.parent.value());
    }

    // Ensure the data buffer has sufficient space
    if ((data_buffer.GetDataLength() + Serialize(null_buffer, Tag::Object1) +
        Serialize(null_buffer, data_length) + data_length.value) >
        data_buffer.GetBufferSize())
    {
        return { 0, 0 };
    }

    // Serialize the object (evalation order matters)
    total_length = Serialize(data_buffer, Tag::Object1);
    total_length += Serialize(data_buffer, data_length);
    total_length += Serialize(data_buffer, value.id);
    total_length += Serialize(data_buffer, value.time);
    total_length += Serialize(data_buffer, value.position);
    total_length += Serialize(data_buffer, value.rotation);
    total_length += Serialize(data_buffer, value.scale);

    if (value.parent.has_value())
    {
        total_length += Serialize(data_buffer, value.parent.value());
    }

    return { 1, total_length };
}

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write a Head1 object to the given buffer, appending
 *      the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer, const Head1 &value)
{
    std::size_t total_length{};
    Length data_length{};

    // Determine space required for this object
    data_length.value = Serialize(null_buffer, value.id) +
                        Serialize(null_buffer, value.time) +
                        Serialize(null_buffer, value.location) +
                        Serialize(null_buffer, value.rotation);

    if (value.ipd.has_value())
    {
        data_length.value += Serialize(null_buffer, value.ipd.value());
    }

    // Ensure the data buffer has sufficient space
    if ((data_buffer.GetDataLength() + Serialize(null_buffer, Tag::Head1) +
         Serialize(null_buffer, data_length) + data_length.value) >
        data_buffer.GetBufferSize())
    {
        return {0, 0};
    }

    // Serialize the object (evalation order matters)
    total_length = Serialize(data_buffer, Tag::Head1);
    total_length += Serialize(data_buffer, data_length);
    total_length += Serialize(data_buffer, value.id);
    total_length += Serialize(data_buffer, value.time);
    total_length += Serialize(data_buffer, value.location);
    total_length += Serialize(data_buffer, value.rotation);

    if (value.ipd.has_value())
    {
        total_length += Serialize(data_buffer, value.ipd.value());
    }

    return {1, total_length};
}

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write a Hand1 object to the given buffer, appending
 *      the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer, const Hand1 &value)
{
    std::size_t total_length{};
    Length data_length{};

    // Determine space required for this object
    data_length.value = Serialize(null_buffer, value.id) +
                        Serialize(null_buffer, value.time) +
                        Serialize(null_buffer, value.left) +
                        Serialize(null_buffer, value.location) +
                        Serialize(null_buffer, value.rotation);

    // Ensure the data buffer has sufficient space
    if ((data_buffer.GetDataLength() + Serialize(null_buffer, Tag::Hand1) +
         Serialize(null_buffer, data_length) + data_length.value) >
        data_buffer.GetBufferSize())
    {
        return {0, 0};
    }

    // Serialize the object (evalation order matters)
    total_length = Serialize(data_buffer, Tag::Hand1);
    total_length += Serialize(data_buffer, data_length);
    total_length += Serialize(data_buffer, value.id);
    total_length += Serialize(data_buffer, value.time);
    total_length += Serialize(data_buffer, value.left);
    total_length += Serialize(data_buffer, value.location);
    total_length += Serialize(data_buffer, value.rotation);

    return {1, total_length};
}

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write a Hand2 object to the given buffer, appending
 *      the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer, const Hand2 &value)
{
    std::size_t total_length{};
    Length data_length{};

    // Determine space required for this object
    data_length.value = Serialize(null_buffer, value.id) +
                        Serialize(null_buffer, value.time) +
                        Serialize(null_buffer, value.left) +
                        Serialize(null_buffer, value.location) +
                        Serialize(null_buffer, value.rotation) +
                        Serialize(null_buffer, value.wrist) +
                        Serialize(null_buffer, value.thumb) +
                        Serialize(null_buffer, value.index) +
                        Serialize(null_buffer, value.middle) +
                        Serialize(null_buffer, value.ring) +
                        Serialize(null_buffer, value.pinky);

    // Ensure the data buffer has sufficient space
    if ((data_buffer.GetDataLength() + Serialize(null_buffer, Tag::Hand2) +
         Serialize(null_buffer, data_length) + data_length.value) >
        data_buffer.GetBufferSize())
    {
        return {0, 0};
    }

    // Serialize the object (evalation order matters)
    total_length = Serialize(data_buffer, Tag::Hand2);
    total_length += Serialize(data_buffer, data_length);
    total_length += Serialize(data_buffer, value.id);
    total_length += Serialize(data_buffer, value.time);
    total_length += Serialize(data_buffer, value.left);
    total_length += Serialize(data_buffer, value.location);
    total_length += Serialize(data_buffer, value.rotation);
    total_length += Serialize(data_buffer, value.wrist);
    total_length += Serialize(data_buffer, value.thumb);
    total_length += Serialize(data_buffer, value.index);
    total_length += Serialize(data_buffer, value.middle);
    total_length += Serialize(data_buffer, value.ring);
    total_length += Serialize(data_buffer, value.pinky);

    return {1, total_length};
}

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write a Mesh1 object to the given buffer, appending
 *      the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer, const Mesh1 &value)
{
    std::size_t total_length{};
    Length data_length{};

    // Determine space required for this object
    data_length.value = Serialize(null_buffer, value.id) +
                        Serialize(null_buffer, value.vertices) +
                        Serialize(null_buffer, value.normals) +
                        Serialize(null_buffer, value.textures) +
                        Serialize(null_buffer, value.triangles);

    // Ensure the data buffer has sufficient space
    if ((data_buffer.GetDataLength() + Serialize(null_buffer, Tag::Mesh1) +
         Serialize(null_buffer, data_length) + data_length.value) >
        data_buffer.GetBufferSize())
    {
        return {0, 0};
    }

    // Serialize the object (evalation order matters)
    total_length = Serialize(data_buffer, Tag::Mesh1);
    total_length += Serialize(data_buffer, data_length);
    total_length += Serialize(data_buffer, value.id);
    total_length += Serialize(data_buffer, value.vertices);
    total_length += Serialize(data_buffer, value.normals);
    total_length += Serialize(data_buffer, value.textures);
    total_length += Serialize(data_buffer, value.triangles);

    return {1, total_length};
}

/*
 *  Encoder::Encode
 *
 *  Description:
 *      This function will write an UnknownObject object to the given buffer,
 *      appending the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      A pair representing the number of objects and number of octets
 *      serialized onto the data buffer.  A value less than expected number of
 *      objects would indicate there was no more room for additional objects
 *      in the data buffer.
 *
 *  Comments:
 *      None.
 */
EncodeResult Encoder::Encode(DataBuffer &data_buffer,
                             const UnknownObject &value)
{
    std::size_t total_length{};

    // Ensure the data buffer has sufficient space
    if ((data_buffer.GetDataLength() + Serialize(null_buffer, value.tag) +
         Serialize(null_buffer, value.data)) > data_buffer.GetBufferSize())
    {
        return {0, 0};
    }

    total_length = Serialize(data_buffer, value.tag);
    total_length += Serialize(data_buffer, value.data);

    return {1, total_length};
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Tag value to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer, Tag value)
{
    VarUint tag{};

    switch (value)
    {
        case Tag::Invalid:
            tag.value = 0x00;
            break;

        case Tag::Head1:
            tag.value = 0x01;
            break;

        case Tag::Hand1:
            tag.value = 0x02;
            break;

        case Tag::Object1:
            tag.value = 0x03;
            break;

        case Tag::Mesh1:
            tag.value = 0x8000;
            break;

        case Tag::Hand2:
            tag.value = 0x8001;
            break;

        case Tag::HeadIPD1:
            tag.value = 0x8002;
            break;

        default:
            tag.value = 0x00;
            break;
    }

    // It is invalid to encode an "Invalid" tag type
    if (tag.value == 0) throw EncoderException("Invalid object tag value");

    return Serialize(data_buffer, tag);
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will write a Head1 object to the given buffer, appending
 *      the data to the end.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The object to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const HeadIPD1 &value)
{
    std::size_t total_length{};

    Length data_length{};

    // Determine space required for this object
    data_length.value = Serialize(null_buffer, value.ipd);

    // Serialize the object (evalation order matters)
    total_length = Serialize(data_buffer, Tag::HeadIPD1);
    total_length += Serialize(data_buffer, data_length);
    total_length += Serialize(data_buffer, value.ipd);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Loc1 structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Loc1 &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.x);
    total_length += Serialize(data_buffer, value.y);
    total_length += Serialize(data_buffer, value.z);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Loc2 structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Loc2 &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.x);
    total_length += Serialize(data_buffer, value.y);
    total_length += Serialize(data_buffer, value.z);
    total_length += Serialize(data_buffer, value.vx);
    total_length += Serialize(data_buffer, value.vy);
    total_length += Serialize(data_buffer, value.vz);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Norm1 structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Norm1 &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.x);
    total_length += Serialize(data_buffer, value.y);
    total_length += Serialize(data_buffer, value.z);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a TextureUV1 structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const TextureUV1 &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.u);
    total_length += Serialize(data_buffer, value.v);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Rot1 structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Rot1 &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.i);
    total_length += Serialize(data_buffer, value.j);
    total_length += Serialize(data_buffer, value.k);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Rot2 structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Rot2 &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.si);
    total_length += Serialize(data_buffer, value.sj);
    total_length += Serialize(data_buffer, value.sk);
    total_length += Serialize(data_buffer, value.ei);
    total_length += Serialize(data_buffer, value.ej);
    total_length += Serialize(data_buffer, value.ek);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Transform1 structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Transform1 &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.tx);
    total_length += Serialize(data_buffer, value.ty);
    total_length += Serialize(data_buffer, value.tz);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Thumb structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Thumb &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.tip);
    total_length += Serialize(data_buffer, value.ip);
    total_length += Serialize(data_buffer, value.mcp);
    total_length += Serialize(data_buffer, value.cmc);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a Finger structure to the data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer into which the value shall be written.
 *
 *      value [in]
 *          The data to serialize to the end of the DataBuffer.
 *
 *  Returns:
 *      The number of octets appended to the data buffer or would have been
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
inline std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                                      const Finger &value)
{
    std::size_t total_length{};

    total_length = Serialize(data_buffer, value.tip);
    total_length += Serialize(data_buffer, value.dip);
    total_length += Serialize(data_buffer, value.pip);
    total_length += Serialize(data_buffer, value.mcp);
    total_length += Serialize(data_buffer, value.cmc);

    return total_length;
}

/*
 *  Encoder::Serialize
 *
 *  Description:
 *      This function will serialize a vector of elements to the end of the
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
 *      appended if the data_buffer contains a zero-sized buffer.
 *
 *  Comments:
 *      None.
 */
template<typename T>
std::size_t Encoder::Serialize(DataBuffer &data_buffer,
                               const std::vector<T> &value)
{
    std::size_t total_length{};

    // Write out the number of vector elements that will follow
    VarUint size{value.size()};
    total_length = Serialize(data_buffer, size);

    // If the string is empty, just return
    if (value.empty()) return total_length;

    // Write each member of the vector in turn
    for (auto &item : value) total_length += Serialize(data_buffer, item);

    return total_length;
}

} // namespace gs
