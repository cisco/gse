/*
 *  gs_decoder.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      The Game State Decoder is an object that will decode game state data
 *      extracted from a given data buffer.
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

#include "gs_decoder.h"

namespace gs
{

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will read all of the object from the given buffer,
 *      appending each object found to the GSObjects vector.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the objects shall be decoded.
 *
 *      value [out]
 *          The objects deserialized from the given DataBuffer.
 *
 *  Returns:
 *      Number of octets consumed from the data buffer.  Additionally, the
 *      GSObjects vector will contain any objects found.  An exception will be
 *      thrown if there is an error, in which case the vector should be
 *      considered invalid.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, GSObjects &value)
{
    std::size_t read_length{};

    // Loop until all data in the buffer is consumed, decoding objects serially
    while (data_buffer.GetReadLength() < data_buffer.GetDataLength())
    {
        value.push_back({});
        read_length += Decode(data_buffer, value.back());
    }

    return read_length;
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will read a single object from the given buffer.  This
 *      makes use of the DataBuffer's internal logic to determine where
 *      the last read ended.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the objects shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed from the data buffer when decoding
 *      the object.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, GSObject &value)
{
    Tag tag;
    VarUint raw_tag;
    std::size_t read_length;

    // Deserialize the object tag value
    read_length = Deserialize(data_buffer, tag, raw_tag);

    // Deserialization depends on the tag type
    switch (tag)
    {
        case Tag::Invalid:
            // If the tag is invalid and the raw value is zero, that's an error
            if (raw_tag.value == 0)
            {
                throw DecoderException("Cannot decode an invalid (0) tag type");
            }

            // Deserialize an UnknownObject, tag type value is in raw_tag
            {
                value = UnknownObject{};
                UnknownObject &unknown_object = std::get<UnknownObject>(value);
                unknown_object.tag = raw_tag;
                read_length += Decode(data_buffer, unknown_object);
            }
            break;

        case Tag::Head1:
            // Deserialize a Head1
            {
                value = Head1{};
                Head1 &head1 = std::get<Head1>(value);
                read_length += Decode(data_buffer, head1);
            }
            break;

        case Tag::Hand1:
            // Deserialize a Hand1
            {
                value = Hand1{};
                Hand1 &hand1 = std::get<Hand1>(value);
                read_length += Decode(data_buffer, hand1);
            }
            break;

        case Tag::Mesh1:
            // Deserialize a Mesh1
            {
                value = Mesh1{};
                Mesh1 &mesh1 = std::get<Mesh1>(value);
                read_length += Decode(data_buffer, mesh1);
            }
            break;

        case Tag::Hand2:
            // Deserialize a Hand2
            {
                value = Hand2{};
                Hand2 &hand2 = std::get<Hand2>(value);
                read_length += Decode(data_buffer, hand2);
            }
            break;

        case Tag::HeadIPD1:
            // Deserialize a HeadIPD1
            {
                value = HeadIPD1{};
                HeadIPD1 &head_ipd1 = std::get<HeadIPD1>(value);
                read_length += Decode(data_buffer, head_ipd1);
            }
            break;

        case Tag::Object1:
            // Deserialize an Object1.
            {
                value = Object1{};
                Object1& object1 = std::get<Object1>(value);
                read_length += Decode(data_buffer, object1);
            }
            break;
    }

    return read_length;
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will decode a Head1 object type from the data buffer.
 *      The tag value would have been read already, so this function reads
 *      the length field and balance of the octets.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the object shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, Head1 &value)
{
    VarUint length;
    std::size_t read_length;
    std::size_t length_field;

    // Read the object length
    length_field = read_length = Deserialize(data_buffer, length);

    if (!length.value) throw DecoderException("Invalid object length");

    // Read all of the required fields (evaluation order matters)
    read_length += Deserialize(data_buffer, value.id);
    read_length += Deserialize(data_buffer, value.time);
    read_length += Deserialize(data_buffer, value.location);
    read_length += Deserialize(data_buffer, value.rotation);

    // Are optional elements present?
    if ((read_length - length_field) < length.value)
    {
        // Attempt to decode the HeadIPD1 object
        GSObject object;
        read_length += Decode(data_buffer, object);

        // Ensure this is actually an HeadIPD1 object
        if (!std::holds_alternative<HeadIPD1>(object))
        {
            throw "Unexpected optional object type found decoding Head1";
        }

        // Assign the decoded object
        value.ipd = std::get<HeadIPD1>(object);

        // Discard any octets not understood
        if ((read_length - length_field) < length.value)
        {
            data_buffer.AdvanceReadLength(length.value - read_length);

            // Update the read_length
            read_length += length.value - (read_length - length_field);
        }
    }

    // Did we read more octets than we should have?
    if ((read_length - length_field) > length.value)
    {
        throw DecoderException("Encoded object length error");
    }

    return read_length;
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will decode a Hand1 object type from the data buffer.
 *      The tag value would have been read already, so this function reads
 *      the length field and balance of the octets.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the object shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, Hand1 &value)
{
    VarUint length;
    std::size_t read_length;
    std::size_t length_field;

    // Read the object length
    length_field = read_length = Deserialize(data_buffer, length);

    if (!length.value) throw DecoderException("Invalid object length");

    // Read all of the required fields (evaluation order matters)
    read_length += Deserialize(data_buffer, value.id);
    read_length += Deserialize(data_buffer, value.time);
    read_length += Deserialize(data_buffer, value.left);
    read_length += Deserialize(data_buffer, value.location);
    read_length += Deserialize(data_buffer, value.rotation);

    // Discard any octets not understood
    if ((read_length - length_field) < length.value)
    {
        data_buffer.AdvanceReadLength(length.value -
                                      (read_length - length_field));

        // Update the read_length
        read_length += length.value - (read_length - length_field);
    }

    // Did we read more octets than we should have?
    if ((read_length - length_field) > length.value)
    {
        throw DecoderException("Encoded object length error");
    }

    return read_length;
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will decode a Hand2 object type from the data buffer.
 *      The tag value would have been read already, so this function reads
 *      the length field and balance of the octets.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the object shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, Hand2 &value)
{
    VarUint length;
    std::size_t read_length;
    std::size_t length_field;

    // Read the object length
    length_field = read_length = Deserialize(data_buffer, length);

    if (!length.value) throw DecoderException("Invalid object length");

    // Read all of the required fields (evaluation order matters)
    read_length += Deserialize(data_buffer, value.id);
    read_length += Deserialize(data_buffer, value.time);
    read_length += Deserialize(data_buffer, value.left);
    read_length += Deserialize(data_buffer, value.location);
    read_length += Deserialize(data_buffer, value.rotation);
    read_length += Deserialize(data_buffer, value.wrist);
    read_length += Deserialize(data_buffer, value.thumb);
    read_length += Deserialize(data_buffer, value.index);
    read_length += Deserialize(data_buffer, value.middle);
    read_length += Deserialize(data_buffer, value.ring);
    read_length += Deserialize(data_buffer, value.pinky);

    // Discard any octets not understood
    if ((read_length - length_field) < length.value)
    {
        data_buffer.AdvanceReadLength(length.value -
                                      (read_length - length_field));

        // Update the read_length
        read_length += length.value - (read_length - length_field);
    }

    // Did we read more octets than we should have?
    if ((read_length - length_field) > length.value)
    {
        throw DecoderException("Encoded object length error");
    }

    return read_length;
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will decode a Mesh1 object type from the data buffer.
 *      The tag value would have been read already, so this function reads
 *      the length field and balance of the octets.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the object shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, Mesh1 &value)
{
    VarUint length;
    std::size_t read_length;
    std::size_t length_field;

    // Read the object length
    length_field = read_length = Deserialize(data_buffer, length);

    if (!length.value) throw DecoderException("Invalid object length");

    // Read all of the required fields (evaluation order matters)
    read_length += Deserialize(data_buffer, value.id);
    read_length += Deserialize(data_buffer, value.vertices);
    read_length += Deserialize(data_buffer, value.normals);
    read_length += Deserialize(data_buffer, value.textures);
    read_length += Deserialize(data_buffer, value.triangles);

    // Discard any octets not understood
    if ((read_length - length_field) < length.value)
    {
        data_buffer.AdvanceReadLength(length.value -
                                      (read_length - length_field));

        // Update the read_length
        read_length += length.value - (read_length - length_field);
    }

    // Did we read more octets than we should have?
    if ((read_length - length_field) > length.value)
    {
        throw DecoderException("Encoded object length error");
    }

    return read_length;
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will decode a HeadIPD1 object type from the data buffer.
 *      The tag value would have been read already, so this function reads
 *      the length field and balance of the octets.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the object shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, HeadIPD1 &value)
{
    VarUint length;
    std::size_t read_length;
    std::size_t length_field;

    // Read the object length
    length_field = read_length = Deserialize(data_buffer, length);

    if (!length.value) throw DecoderException("Invalid object length");

    // Read all of the required fields (evaluation order matters)
    read_length += Deserialize(data_buffer, value.ipd);

    // Discard any octets not understood
    if ((read_length - length_field) < length.value)
    {
        data_buffer.AdvanceReadLength(length.value -
                                      (read_length - length_field));

        // Update the read_length
        read_length += length.value - (read_length - length_field);
    }

    // Did we read more octets than we should have?
    if ((read_length - length_field) > length.value)
    {
        throw DecoderException("Encoded object length error");
    }

    return read_length;
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will decode an UnknownObject type from the data buffer.
 *      The tag value would have been read already, so this function reads
 *      the length field and balance of the octets, treating it as a Blob.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the object shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, UnknownObject &value)
{
    return Deserialize(data_buffer, value.data);
}

/*
 *  Decoder::Decode
 *
 *  Description:
 *      This function will decode an Object1 type from the data buffer.
 *      The tag value would have been read already, so this function reads
 *      the length field and balance of the octets, treating it as a Blob.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the object shall be decoded.
 *
 *      value [out]
 *          The object deserialized from the given DataBuffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Decode(DataBuffer<> &data_buffer, Object1 &value)
{
    VarUint length;
    std::size_t read_length;
    std::size_t length_field;

    // Read the object length
    length_field = read_length = Deserialize(data_buffer, length);

    if (!length.value) throw DecoderException("Invalid object length");

    // Read all of the required fields (evaluation order matters)
    read_length += Deserialize(data_buffer, value.id);
    read_length += Deserialize(data_buffer, value.time);
    read_length += Deserialize(data_buffer, value.position);
    read_length += Deserialize(data_buffer, value.rotation);
    read_length += Deserialize(data_buffer, value.scale);

    // Are optional elements present?
    if ((read_length - length_field) < length.value)
    {
        // Attempt to decode the Parent object
        ObjectID parent;
        read_length += Deserialize(data_buffer, parent);

        // Assign the decoded object
        value.parent = parent;

        // Discard any octets not understood
        if ((read_length - length_field) < length.value)
        {
            data_buffer.AdvanceReadLength(length.value - read_length);

            // Update the read_length
            read_length += length.value - (read_length - length_field);
        }
    }

    // Did we read more octets than we should have?
    if ((read_length - length_field) > length.value)
    {
        throw DecoderException("Encoded object length error");
    }

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize an object tag from the provided data
 *      buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The interpreted tag value read from the buffer.
 *
 *      raw_value [out]
 *          The raw tag value, which is useful in the case that the tag
 *          value read is unknown.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer,
                                 Tag &value,
                                 VarUint &raw_value)
{
    std::size_t read_length;
    VarUint tag_value{};

    // Read the tag value from the buffer
    read_length = Deserialize(data_buffer, tag_value);

    // Set the raw tag value to the value read
    raw_value = tag_value;

    switch (tag_value.value)
    {
        case 0x00:
            throw DecoderException("Invalid object tag in data buffer");
            break;

        case 0x01:
            value = Tag::Head1;
            break;

        case 0x02:
            value = Tag::Hand1;
            break;

        case 0x03:
            value = Tag::Object1;
            break;

        case 0x8000:
            value = Tag::Mesh1;
            break;

        case 0x8001:
            value = Tag::Hand2;
            break;

        case 0x8002:
            value = Tag::HeadIPD1;
            break;

        default:
            value = Tag::Invalid;
            break;
    }

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Loc1 structure from the provided data
 *      buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer, Loc1 &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.x);
    read_length += Deserialize(data_buffer, value.y);
    read_length += Deserialize(data_buffer, value.z);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Loc2 structure from the provided data
 *      buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer,
                                 Loc2 &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.x);
    read_length += Deserialize(data_buffer, value.y);
    read_length += Deserialize(data_buffer, value.z);
    read_length += Deserialize(data_buffer, value.vx);
    read_length += Deserialize(data_buffer, value.vy);
    read_length += Deserialize(data_buffer, value.vz);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Norm1 structure from the provided data
 *      buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer,
                                 Norm1 &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.x);
    read_length += Deserialize(data_buffer, value.y);
    read_length += Deserialize(data_buffer, value.z);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a TextureUV1 structure from the provided
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer, TextureUV1 &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.u);
    read_length += Deserialize(data_buffer, value.v);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Rot1 structure from the provided
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer,
                                 Rot1 &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.i);
    read_length += Deserialize(data_buffer, value.j);
    read_length += Deserialize(data_buffer, value.k);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Rot2 structure from the provided
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer, Rot2 &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.si);
    read_length += Deserialize(data_buffer, value.sj);
    read_length += Deserialize(data_buffer, value.sk);
    read_length += Deserialize(data_buffer, value.ei);
    read_length += Deserialize(data_buffer, value.ej);
    read_length += Deserialize(data_buffer, value.ek);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Transform1 structure from the provided
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer, Transform1 &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.tx);
    read_length += Deserialize(data_buffer, value.ty);
    read_length += Deserialize(data_buffer, value.tz);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Thumb structure from the provided
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer, Thumb &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.tip);
    read_length += Deserialize(data_buffer, value.ip);
    read_length += Deserialize(data_buffer, value.mcp);
    read_length += Deserialize(data_buffer, value.cmc);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a Finger structure from the provided
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The value read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer,
                                 Finger &value)
{
    std::size_t read_length{};

    read_length = Deserialize(data_buffer, value.tip);
    read_length += Deserialize(data_buffer, value.dip);
    read_length += Deserialize(data_buffer, value.pip);
    read_length += Deserialize(data_buffer, value.mcp);
    read_length += Deserialize(data_buffer, value.cmc);

    return read_length;
}

/*
 *  Decoder::Deserialize
 *
 *  Description:
 *      This function will deserialize a vector of elements from the provided
 *      data buffer.
 *
 *  Parameters:
 *      data_buffer [in]
 *          The data buffer from which the value shall be read.
 *
 *      value [out]
 *          The vector of elements read from the buffer.
 *
 *  Returns:
 *      The number of octets consumed in the data buffer.
 *
 *  Comments:
 *      None.
 */
template <typename T>
std::size_t Decoder::Deserialize(DataBuffer<> &data_buffer,
                                 std::vector<T> &values)
{
    std::size_t read_length;
    VarUint expected_vector_length;

    // Read the number of octets that will follow
    read_length = Deserialize(data_buffer, expected_vector_length);

    // If the vector is empty, just return
    if (expected_vector_length.value == 0) return read_length;

    // Deserialize each member of the vector from the buffer
    for (std::size_t i = 0; i < expected_vector_length.value; i++)
    {
        values.push_back({});
        read_length += Deserialize(data_buffer, values.back());
    }

    return read_length;
}

} // namespace gs
