/*
 *  gs_types.h
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      The Game State types used with the Game State Encoder/Decder are defined
 *      within this header file.  These definitions represent the types
 *      used in the machine, not the representation on the wire.
 *
 *      Note that Float16 is actually a 32-bit floating point number.  When
 *      that type is encoded on the wire, though, it will be encoded as a
 *      half-precision floating point number.
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

#include <cstdint>
#include <vector>
#include <variant>
#include <string>
#include <optional>

#ifndef GS_TYPES_H
#define GS_TYPES_H

namespace gs
{
    // Primitive types
    struct VarUint { std::uint64_t value; };
    struct VarInt { std::int64_t value; };
    struct Float16 { float value; };
    typedef float Float32;
    typedef double Float64;
    typedef std::uint8_t Uint8;
    typedef std::uint16_t Uint16;
    typedef std::uint32_t Uint32;
    typedef std::uint64_t Uint64;
    typedef std::int8_t Int8;
    typedef std::int16_t Int16;
    typedef std::int32_t Int32;
    typedef std::int64_t Int64;
    typedef bool Boolean;
    typedef std::uint8_t Byte;

    // Basic types
    typedef std::string String;
    typedef std::vector<Byte> Blob;

    // Simple types (aliases)
    typedef String TextureUrl1;
    typedef Uint8 TextureRtpPT1;
    typedef Uint16 Time1;
    typedef VarUint ObjectID;
    typedef VarUint Length;

    // Tag type values for serializable objects
    enum class Tag
    {
        Invalid  = 0x00,
        Head1    = 0x01,
        Hand1    = 0x02,
        Object1  = 0x03,
        Mesh1    = 0x8000,
        Hand2    = 0x8001,
        HeadIPD1 = 0x8002
    };

    // Complex types
    struct Loc1
    {
        Float32 x;
        Float32 y;
        Float32 z;
    };

    struct Loc2
    {
        Float32 x;
        Float32 y;
        Float32 z;
        Float16 vy;
        Float16 vx;
        Float16 vz;
    };

    struct Norm1
    {
        Float16 x;
        Float16 y;
        Float16 z;
    };

    struct TextureUV1
    {
        VarUint u;
        VarUint v;
    };

    struct Rot1
    {
        Float16 i;
        Float16 j;
        Float16 k;
    };

    struct Rot2
    {
        Float16 si;
        Float16 sj;
        Float16 sk;
        Float16 ei;
        Float16 ej;
        Float16 ek;
    };

    struct Transform1
    {
        Float16 tx;
        Float16 ty;
        Float16 tz;
    };

    struct Object1
    {
        ObjectID id;
        Time1 time;
        Loc1 position;
        Rot1 rotation;
        Loc1 scale;
        std::optional<ObjectID> parent;
    };

    struct HeadIPD1
    {
        Float16 ipd;
    };

    struct Head1
    {
        ObjectID id;
        Time1 time;
        Loc2 location;
        Rot2 rotation;
        std::optional<HeadIPD1> ipd;
    };

    struct Mesh1
    {
        ObjectID id;
        std::vector<Loc1> vertices;
        std::vector<Norm1> normals;
        std::vector<TextureUV1> textures;
        std::vector<VarUint> triangles;
    };

    struct Hand1
    {
        ObjectID id;
        Time1 time;
        Boolean left;
        Loc2 location;
        Rot2 rotation;
    };

    struct Thumb
    {
        Transform1 tip;
        Transform1 ip;
        Transform1 mcp;
        Transform1 cmc;
    };

    struct Finger
    {
        Transform1 tip;
        Transform1 dip;
        Transform1 pip;
        Transform1 mcp;
        Transform1 cmc;
    };

    struct Hand2
    {
        ObjectID id;
        Time1 time;
        Boolean left;
        Loc2 location;
        Rot2 rotation;
        Transform1 wrist;
        Thumb thumb;
        Finger index;
        Finger middle;
        Finger ring;
        Finger pinky;
    };

    struct UnknownObject
    {
        VarUint tag;
        Blob data;
    };

    // Variant type that can contain any object type
    typedef std::variant<Head1,
                         Hand1,
                         Object1,
                         Mesh1,
                         Hand2,
                         HeadIPD1,
                         UnknownObject> GSObject;

    // Collection of objects
    typedef std::vector<GSObject> GSObjects;

} // namespace gs

#endif // GS_TYPES_H
