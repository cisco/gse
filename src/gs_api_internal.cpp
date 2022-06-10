/*
 *  gs_api_internal.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module provides support function for converting data between
 *      the C and C++ structures used by the Game State Encoder and Decoder.
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

#include <algorithm>
#include <cstddef>
#include <cstring>
#include "gs_types.h"
#include "gs_api_internal.h"

namespace gs_api
{

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Loc1 data structure as a part of serialization (C to C++
 *      structures), copying from GS_Loc1 to gs::Loc1.
 *
 *  Parameters:
 *      gs_loc1 [in]
 *          C GS_Loc1 structure.
 *
 *      loc1 [out]
 *          C++ gs::Loc1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Loc1 &gs_loc1, gs::Loc1 &loc1)
{
    loc1.x = gs_loc1.x;
    loc1.y = gs_loc1.y;
    loc1.z = gs_loc1.z;
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Loc2 data structure as a part of serialization (C to C++
 *      structures), copying from GS_Loc2 to gs::Loc2.
 *
 *  Parameters:
 *      gs_loc2 [in]
 *          C GS_Loc2 structure.
 *
 *      loc2 [out]
 *          C++ gs::Loc2 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Loc2 &gs_loc2, gs::Loc2 &loc2)
{
    loc2.x = gs_loc2.x;
    loc2.y = gs_loc2.y;
    loc2.z = gs_loc2.z;
    loc2.vx.value = gs_loc2.vx;
    loc2.vy.value = gs_loc2.vy;
    loc2.vz.value = gs_loc2.vz;
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Rot1 data structure as a part of serialization (C to C++
 *      structures), copying from GS_Rot1 to gs::Rot1.
 *
 *  Parameters:
 *      gs_rot1 [in]
 *          C GS_Roc1 structure.
 *
 *      roc1 [out]
 *          C++ gs::Rot1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Rot1& gs_rot1, gs::Rot1& rot1)
{
    rot1.i.value = gs_rot1.i;
    rot1.j.value = gs_rot1.j;
    rot1.k.value = gs_rot1.k;
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Rot2 data structure as a part of serialization (C to C++
 *      structures), copying from GS_Rot2 to gs::Rot2.
 *
 *  Parameters:
 *      gs_rot2 [in]
 *          C GS_Roc2 structure.
 *
 *      roc2 [out]
 *          C++ gs::Rot2 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Rot2 &gs_rot2, gs::Rot2 &rot2)
{
    rot2.si.value = gs_rot2.si;
    rot2.sj.value = gs_rot2.sj;
    rot2.sk.value = gs_rot2.sk;
    rot2.ei.value = gs_rot2.ei;
    rot2.ej.value = gs_rot2.ej;
    rot2.ek.value = gs_rot2.ek;
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Norm1 data structure as a part of serialization (C to C++
 *      structures), copying from GS_Norm1 to gs::Norm1.
 *
 *  Parameters:
 *      gs_norm1 [in]
 *          C GS_Norm1 structure.
 *
 *      norm1 [out]
 *          C++ gs::Norm1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Norm1 &gs_norm1, gs::Norm1 &norm1)
{
    norm1.x.value = gs_norm1.x;
    norm1.y.value = gs_norm1.y;
    norm1.z.value = gs_norm1.z;
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the TextureUV1 data structure as a part of serialization (C to C++
 *      structures), copying from GS_TextureUV1 to gs::TextureUV1.
 *
 *  Parameters:
 *      gs_textureUV1 [in]
 *          C GS_TextureUV1 structure.
 *
 *      textureUV1 [out]
 *          C++ gs::TextureUV1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_TextureUV1 &gs_textureUV1,
                             gs::TextureUV1 &textureUV1)
{
    textureUV1.u.value = gs_textureUV1.u;
    textureUV1.v.value = gs_textureUV1.v;
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Transform1 data structure as a part of serialization (C to C++
 *      structures), copying from GS_Transform1 to gs::Transform1.
 *
 *  Parameters:
 *      gs_transform1 [in]
 *          C GS_Transform1 structure.
 *
 *      textureUV1 [out]
 *          C++ gs::Transform1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Transform1 &gs_transform1,
                             gs::Transform1 &transform1)
{
    transform1.tx.value = gs_transform1.tx;
    transform1.ty.value = gs_transform1.ty;
    transform1.tz.value = gs_transform1.tz;
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Thumb data structure as a part of serialization (C to C++
 *      structures), copying from GS_Thumb to gs::Thumb.
 *
 *  Parameters:
 *      gs_thumb [in]
 *          C GS_Thumb structure.
 *
 *      thumb [out]
 *          C++ gs::Thumb structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Thumb &gs_thumb, gs::Thumb &thumb)
{
    SerializeCopy(gs_thumb.tip, thumb.tip);
    SerializeCopy(gs_thumb.ip, thumb.ip);
    SerializeCopy(gs_thumb.mcp, thumb.mcp);
    SerializeCopy(gs_thumb.cmc, thumb.cmc);
}

/*
 *  SerializeCopy
 *
 *  Description:
 *      Copy the Finger data structure as a part of serialization (C to C++
 *      structures), copying from GS_Finger to gs::Finger.
 *
 *  Parameters:
 *      gs_finger [in]
 *          C GS_Finger structure.
 *
 *      finger [out]
 *          C++ gs::Finger structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void SerializeCopy(const GS_Finger &gs_finger, gs::Finger &finger)
{
    SerializeCopy(gs_finger.tip, finger.tip);
    SerializeCopy(gs_finger.dip, finger.dip);
    SerializeCopy(gs_finger.pip, finger.pip);
    SerializeCopy(gs_finger.mcp, finger.mcp);
    SerializeCopy(gs_finger.cmc, finger.cmc);
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Object &object)
{
    int result = -1;

    // Just return 0 if there is no space left in the buffer
    if (context.data_buffer.GetDataLength() ==
        context.data_buffer.GetBufferSize())
    {
        return 0;
    }

    // Serialize the object based on its type
    switch (object.type)
    {
        case GS_Tag_Invalid:
            context.error = std::string("Unable to serialize an Invalid object "
                                        "type");
            break;

        case GS_Tag_Object1:
            result = GSSerializeObject(context, object.u.object1);
            break;

        case GS_Tag_Head1:
            result = GSSerializeObject(context, object.u.head1);
            break;

        case GS_Tag_Hand1:
            result = GSSerializeObject(context, object.u.hand1);
            break;

        case GS_Tag_Mesh1:
            result = GSSerializeObject(context, object.u.mesh1);
            break;

        case GS_Tag_Hand2:
            result = GSSerializeObject(context, object.u.hand2);
            break;

        case GS_Tag_HeadIPD1:
            result = GSSerializeObject(context, object.u.head_ipd1);
            break;

        default:
            result = GSSerializeObject(context, object.u.unknown_object);
            break;
    }

    return result;
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Object1 &object)
{
    gs::Object1 object1{};

    // Copy the data from the C structure to C++ structure
    object1.id.value = object.id;
    object1.time = object.time;
    SerializeCopy(object.position, object1.position);
    SerializeCopy(object.rotation, object1.rotation);
    SerializeCopy(object.scale, object1.scale);
    if (object.parent_present) object1.parent = gs::ObjectID{object.parent};

    // Serialize the object
    auto [object_count,
          octet_count] = context.encoder.Encode(context.data_buffer, object1);

    return static_cast<int>(object_count);
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Head1 &object)
{
    gs::Head1 head1{};

    // Copy the data from the C structure to C++ structure
    head1.id.value = object.id;
    head1.time = object.time;
    SerializeCopy(object.location, head1.location);
    SerializeCopy(object.rotation, head1.rotation);
    if (object.ipd_present) head1.ipd = gs::HeadIPD1{object.ipd.ipd};

    // Serialize the object
    auto [object_count,
          octet_count] = context.encoder.Encode(context.data_buffer, head1);

    return static_cast<int>(object_count);
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Hand1 &object)
{
    gs::Hand1 hand1{};

    // Copy the data from the C structure to C++ structure
    hand1.id.value = object.id;
    hand1.time = object.time;
    hand1.left = (object.left ? true : false);
    SerializeCopy(object.location, hand1.location);
    SerializeCopy(object.rotation, hand1.rotation);

    // Serialize the object
    auto [object_count,
          octet_count] = context.encoder.Encode(context.data_buffer, hand1);

    return static_cast<int>(object_count);
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Mesh1 &object)
{
    gs::Mesh1 mesh1{};

    // Copy the data from the C structure to C++ structure
    mesh1.id.value = object.id;

    for (std::size_t i = 0; i < object.num_vertices; i++)
    {
        gs::Loc1 loc1;
        SerializeCopy(object.vertices[i], loc1);
        mesh1.vertices.push_back(loc1);
    }

    for (std::size_t i = 0; i < object.num_normals; i++)
    {
        gs::Norm1 norm1;
        SerializeCopy(object.normals[i], norm1);
        mesh1.normals.push_back(norm1);
    }

    for (std::size_t i = 0; i < object.num_textures; i++)
    {
        gs::TextureUV1 textureUV1;
        SerializeCopy(object.textures[i], textureUV1);
        mesh1.textures.push_back(textureUV1);
    }

    for (std::size_t i = 0; i < object.num_triangles; i++)
    {
        mesh1.triangles.push_back({object.triangles[i]});
    }

    // Serialize the object
    auto [object_count,
          octet_count] = context.encoder.Encode(context.data_buffer, mesh1);

    return static_cast<int>(object_count);
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Hand2 &object)
{
    gs::Hand2 hand2{};

    // Copy the data from the C structure to C++ structure
    hand2.id.value = object.id;
    hand2.time = object.time;
    hand2.left = (object.left ? true : false);
    SerializeCopy(object.location, hand2.location);
    SerializeCopy(object.rotation, hand2.rotation);
    SerializeCopy(object.wrist, hand2.wrist);
    SerializeCopy(object.thumb, hand2.thumb);
    SerializeCopy(object.index, hand2.index);
    SerializeCopy(object.middle, hand2.middle);
    SerializeCopy(object.ring, hand2.ring);
    SerializeCopy(object.pinky, hand2.pinky);

    // Serialize the object
    auto [object_count,
          octet_count] = context.encoder.Encode(context.data_buffer, hand2);

    return static_cast<int>(object_count);
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_UnknownObject &object)
{
    gs::UnknownObject unknown{};

    // Copy the data from the C structure to C++ structure
    unknown.tag.value = object.tag;
    unknown.data.insert(unknown.data.end(),
                        object.data,
                        object.data + object.data_length);

    // Serialize the object
    auto [object_count,
          octet_count] = context.encoder.Encode(context.data_buffer, unknown);

    return static_cast<int>(object_count);
}

/*
 *  GSSerializeObject
 *
 *  Description:
 *      This function will serialize the given object into the given buffer.
 *
 *  Parameters:
 *      context [in]
 *          The internal encoder context.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successfully serialized an object.
 *      0 no objects serialized due to insufficent buffer space.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_HeadIPD1 &object)
{
    gs::HeadIPD1 head_ipd1{};

    // Copy the data from the C structure to C++ structure
    head_ipd1.ipd.value = object.ipd;

    // Serialize the object
    auto [object_count,
          octet_count] = context.encoder.Encode(context.data_buffer, head_ipd1);

    return static_cast<int>(object_count);
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy Loc1 data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Loc1 to GS_Loc1.
 *
 *  Parameters:
 *      loc1 [in]
 *          C++ gs::Loc1 structure.
 *
 *      gs_loc1 [out]
 *          C GS_Loc1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Loc1 &loc1, GS_Loc1 &gs_loc1)
{
    gs_loc1.x = loc1.x;
    gs_loc1.y = loc1.y;
    gs_loc1.z = loc1.z;
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy Loc2 data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Loc2 to GS_Loc2.
 *
 *  Parameters:
 *      loc2 [in]
 *          C++ gs::Loc2 structure.
 *
 *      gs_loc2 [out]
 *          C GS_Loc2 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Loc2 &loc2, GS_Loc2 &gs_loc2)
{
    gs_loc2.x = loc2.x;
    gs_loc2.y = loc2.y;
    gs_loc2.z = loc2.z;
    gs_loc2.vx = loc2.vx.value;
    gs_loc2.vy = loc2.vy.value;
    gs_loc2.vz = loc2.vz.value;
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy Rot1 data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Rot1 to GS_Rot1.
 *
 *  Parameters:
 *      roc1 [in]
 *          C++ gs::Rot1 structure.
 *
 *      gs_rot1 [out]
 *          C GS_Roc1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Rot1& rot1, GS_Rot1& gs_rot1)
{
    gs_rot1.i = rot1.i.value;
    gs_rot1.j = rot1.j.value;
    gs_rot1.k = rot1.k.value;
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy Rot2 data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Rot2 to GS_Rot2.
 *
 *  Parameters:
 *      roc2 [in]
 *          C++ gs::Rot2 structure.
 *
 *      gs_rot2 [out]
 *          C GS_Roc2 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Rot2 &rot2, GS_Rot2 &gs_rot2)
{
    gs_rot2.si = rot2.si.value;
    gs_rot2.sj = rot2.sj.value;
    gs_rot2.sk = rot2.sk.value;
    gs_rot2.ei = rot2.ei.value;
    gs_rot2.ej = rot2.ej.value;
    gs_rot2.ek = rot2.ek.value;
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy Norm1 data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Norm1 to GS_Norm1.
 *
 *  Parameters:
 *      norm1 [in]
 *          C++ gs::Norm1 structure.
 *
 *      gs_norm1 [out]
 *          C GS_Norm1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Norm1 &norm1, GS_Norm1 &gs_norm1)
{
    gs_norm1.x = norm1.x.value;
    gs_norm1.y = norm1.y.value;
    gs_norm1.z = norm1.z.value;
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy TextureUV1 data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::TextureUV1 to GS_TextureUV1.
 *
 *  Parameters:
 *      textureUV1 [in]
 *          C++ gs::TextureUV1 structure.
 *
 *      gs_textureUV1 [out]
 *          C GS_TextureUV1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::TextureUV1 &textureUV1,
                               GS_TextureUV1 &gs_textureUV1)
{
    gs_textureUV1.u = textureUV1.u.value;
    gs_textureUV1.v = textureUV1.v.value;
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy Transform1 data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Transform1 to GS_Transform1.
 *
 *  Parameters:
 *      textureUV1 [in]
 *          C++ gs::Transform1 structure.
 *
 *      gs_transform1 [out]
 *          C GS_Transform1 structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Transform1 &transform1,
                               GS_Transform1 &gs_transform1)
{
    gs_transform1.tx = transform1.tx.value;
    gs_transform1.ty = transform1.ty.value;
    gs_transform1.tz = transform1.tz.value;
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy Thumb data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Thumb to GS_Thumb.
 *
 *  Parameters:
 *      thumb [in]
 *          C++ gs::Thumb structure.
 *
 *      gs_thumb [out]
 *          C GS_Thumb structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Thumb &thumb, GS_Thumb &gs_thumb)
{
    DeserializeCopy(thumb.tip, gs_thumb.tip);
    DeserializeCopy(thumb.ip, gs_thumb.ip);
    DeserializeCopy(thumb.mcp, gs_thumb.mcp);
    DeserializeCopy(thumb.cmc, gs_thumb.cmc);
}

/*
 *  DeserializeCopy
 *
 *  Description:
 *      Copy the Finger data structure as a part of deserialization (C++ to C
 *      structures), copying from gs::Finger to GS_Finger.
 *
 *  Parameters:
 *      finger [in]
 *          C++ gs::Finger structure.
 *
 *      gs_finger [out]
 *          C GS_Finger structure.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
constexpr void DeserializeCopy(const gs::Finger &finger,
                               GS_Finger &gs_finger)
{
    DeserializeCopy(finger.tip, gs_finger.tip);
    DeserializeCopy(finger.dip, gs_finger.dip);
    DeserializeCopy(finger.pip, gs_finger.pip);
    DeserializeCopy(finger.mcp, gs_finger.mcp);
    DeserializeCopy(finger.cmc, gs_finger.cmc);
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the next object in the data buffer
 *      into the object pointer.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      object [out]
 *          The object into which to deserialize from the buffer.
 *
 *  Returns:
 *      1 if successfully decoded an object.
 *      0 if successful, but no objects left in the buffer to decode.
 *      -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject(GS_Decoder_Context_Internal &context, GS_Object &object)
{
    int result = -1;
    gs::GSObject decoded_object{};

    // If there are no more objects to decode, return 0
    if (context.data_buffer.GetReadLength() >=
        context.data_buffer.GetDataLength())
    {
        return 0;
    }

    // Decode one object from the buffer
    context.decoder.Decode(context.data_buffer, decoded_object);

    // Zero the memory associated with the receiving object
    std::memset(&object, 0, sizeof(GS_Object));

    // Deserialize the object into the provided object pointer
    result = std::visit([&](const auto &decoded_object) -> int
                        {
                            return GSDeserializeObject(context,
                                                       decoded_object,
                                                       object);
                        },
                        decoded_object);

    // If the result of calling std::visit() is 0, set the result to 1
    if (result == 0) result = 1;

    return result;
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the gs::Head1 object into the GS_Object
 *      structure.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      head1 [in]
 *          The head1 object that should be deserialized into object.
 *
 *      object [out]
 *          The object into which data should be deserialized.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject([[maybe_unused]] GS_Decoder_Context_Internal &context,
                        const gs::Head1 &head1,
                        GS_Object &object)
{
    // Copy the data from the C++ structure to C structure
    object.type = GS_Tag_Head1;
    object.u.head1.id = head1.id.value;
    object.u.head1.time = head1.time;
    DeserializeCopy(head1.location, object.u.head1.location);
    DeserializeCopy(head1.rotation, object.u.head1.rotation);
    if (head1.ipd.has_value())
    {
        object.u.head1.ipd_present = true;
        object.u.head1.ipd.ipd = head1.ipd.value().ipd.value;
    }

    return 0;
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the gs::Hand1 object into the GS_Object
 *      structure.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      hand1 [in]
 *          The hand1 object that should be deserialized into object.
 *
 *      object [out]
 *          The object into which data should be deserialized.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject([[maybe_unused]] GS_Decoder_Context_Internal &context,
                        const gs::Hand1 &hand1,
                        GS_Object &object)
{
    // Copy the data from the C++ structure to C structure
    object.type = GS_Tag_Hand1;
    object.u.hand1.id = hand1.id.value;
    object.u.hand1.time = hand1.time;
    object.u.hand1.left = (hand1.left ? 1 : 0);
    DeserializeCopy(hand1.location, object.u.hand1.location);
    DeserializeCopy(hand1.rotation, object.u.hand1.rotation);

    return 0;
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the gs::Mesh1 object into the GS_Object
 *      structure.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      mesh1 [in]
 *          The mesh1 object that should be deserialized into object.
 *
 *      object [out]
 *          The object into which data should be deserialized.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::Mesh1 &mesh1,
                        GS_Object &object)
{
    // Copy the data from the C++ structure to C structure
    object.type = GS_Tag_Mesh1;
    object.u.mesh1.id = mesh1.id.value;

    // Get a count of items
    object.u.mesh1.num_vertices = mesh1.vertices.size();
    object.u.mesh1.num_normals = mesh1.normals.size();
    object.u.mesh1.num_textures = mesh1.textures.size();
    object.u.mesh1.num_triangles = mesh1.triangles.size();

    // Deserialize the verticies
    if (object.u.mesh1.num_vertices > 0)
    {
        // Allocate storage, storing the pointer for later deletion
        std::uint8_t *p = new std::uint8_t[sizeof(GS_Loc1) *
                                           object.u.mesh1.num_vertices];
        context.allocations.push_back(p);

        // Assign the pointer to the object
        object.u.mesh1.vertices = reinterpret_cast<GS_Loc1 *>(p);

        // Deserialize the data
        for (std::size_t i = 0; i < object.u.mesh1.num_vertices; i++)
        {
            DeserializeCopy(mesh1.vertices[i], object.u.mesh1.vertices[i]);
        }
    }

    // Deserialize the normals
    if (object.u.mesh1.num_normals > 0)
    {
        // Allocate storage, storing the pointer for later deletion
        std::uint8_t *p = new std::uint8_t[sizeof(GS_Norm1) *
                                           object.u.mesh1.num_normals];
        context.allocations.push_back(p);

        // Assign the pointer to the object
        object.u.mesh1.normals = reinterpret_cast<GS_Norm1 *>(p);

        // Deserialize the data
        for (std::size_t i = 0; i < object.u.mesh1.num_normals; i++)
        {
            DeserializeCopy(mesh1.normals[i], object.u.mesh1.normals[i]);
        }
    }

    // Deserialize the textures
    if (object.u.mesh1.num_textures > 0)
    {
        // Allocate storage, storing the pointer for later deletion
        std::uint8_t *p = new std::uint8_t[sizeof(GS_TextureUV1) *
                                           object.u.mesh1.num_textures];
        context.allocations.push_back(p);

        // Assign the pointer to the object
        object.u.mesh1.textures = reinterpret_cast<GS_TextureUV1 *>(p);

        // Deserialize the data
        for (std::size_t i = 0; i < object.u.mesh1.num_textures; i++)
        {
            DeserializeCopy(mesh1.textures[i], object.u.mesh1.textures[i]);
        }
    }

    // Deserialize the triangles
    if (object.u.mesh1.num_triangles > 0)
    {
        // Allocate storage, storing the pointer for later deletion
        std::uint8_t *p = new std::uint8_t[sizeof(GS_VarUint) *
                                           object.u.mesh1.num_triangles];
        context.allocations.push_back(p);

        // Assign the pointer to the object
        object.u.mesh1.triangles = reinterpret_cast<GS_VarUint *>(p);

        // Deserialize the data
        for (std::size_t i = 0; i < object.u.mesh1.num_triangles; i++)
        {
            object.u.mesh1.triangles[i] = mesh1.triangles[i].value;
        }
    }

    return 0;
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the gs::Hand2 object into the GS_Object
 *      structure.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      hand2 [in]
 *          The hand2 object that should be deserialized into object.
 *
 *      object [out]
 *          The object into which data should be deserialized.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject([[maybe_unused]] GS_Decoder_Context_Internal &context,
                        const gs::Hand2 &hand2,
                        GS_Object &object)
{
    // Copy the data from the C++ structure to C structure
    object.type = GS_Tag_Hand2;
    object.u.hand2.id = hand2.id.value;
    object.u.hand2.time = hand2.time;
    object.u.hand2.left = (hand2.left ? 1 : 0);
    DeserializeCopy(hand2.location, object.u.hand2.location);
    DeserializeCopy(hand2.rotation, object.u.hand2.rotation);
    DeserializeCopy(hand2.wrist, object.u.hand2.wrist);
    DeserializeCopy(hand2.thumb, object.u.hand2.thumb);
    DeserializeCopy(hand2.index, object.u.hand2.index);
    DeserializeCopy(hand2.middle, object.u.hand2.middle);
    DeserializeCopy(hand2.ring, object.u.hand2.ring);
    DeserializeCopy(hand2.pinky, object.u.hand2.pinky);

    return 0;
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the gs::HeadIPD1 object into the
 *      GS_Object structure.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      head_ipd1 [in]
 *          The head_ipd object that should be deserialized into object.
 *
 *      object [out]
 *          The object into which data should be deserialized.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject([[maybe_unused]] GS_Decoder_Context_Internal &context,
                        const gs::HeadIPD1 &head_ipd,
                        GS_Object &object)
{
    // Copy the data from the C++ structure to C structure
    object.type = GS_Tag_HeadIPD1;
    object.u.head_ipd1.ipd = head_ipd.ipd.value;

    return 0;
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the gs::Unknown object into the GS_Object
 *      structure.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      unknown [in]
 *          The unknown object that should be deserialized into object.
 *
 *      object [out]
 *          The object into which data should be deserialized.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::UnknownObject &unknown,
                        GS_Object &object)
{
    // Copy the data from the C++ structure to C structure
    object.type = unknown.tag.value;
    object.u.unknown_object.tag = unknown.tag.value;
    object.u.unknown_object.data_length = unknown.data.size();

    // Allocate storage, storing the pointer for later deletion
    object.u.unknown_object.data = new std::uint8_t[unknown.data.size()];
    context.allocations.push_back(object.u.unknown_object.data);

    // Copy the data
    std::copy(unknown.data.begin(),
              unknown.data.end(),
              object.u.unknown_object.data);

    return 0;
}

/*
 *  GSDeserializeObject
 *
 *  Description:
 *      This function will deserialize the gs::Object1 object into the GS_Object
 *      structure.
 *
 *  Parameters:
 *      context [in]
 *          The internal decoder context.
 *
 *      unknown [in]
 *          The unknown object that should be deserialized into object.
 *
 *      object [out]
 *          The object into which data should be deserialized.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int GSDeserializeObject([[maybe_unused]] GS_Decoder_Context_Internal &context,
                        const gs::Object1 &encoded,
                        GS_Object &object)
{
    // Copy the data from the C++ structure to C structure
    object.type = GS_Tag_Object1;
    object.u.object1.id = encoded.id.value;
    object.u.object1.time = encoded.time;
    DeserializeCopy(encoded.position, object.u.object1.position);
    DeserializeCopy(encoded.rotation, object.u.object1.rotation);
    DeserializeCopy(encoded.scale, object.u.object1.scale);
    if (encoded.parent.has_value())
    {
        object.u.object1.parent_present = true;
        object.u.object1.parent = encoded.parent.value().value;
    }

    return 0;
}

} // namespace gs_api
