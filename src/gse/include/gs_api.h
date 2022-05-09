/*
 *  gs_api.h
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This file defines structures used with the C API for the Game State
 *      Encoder and Decoder objects.
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

#ifndef GS_API_H
#define GS_API_H

#include <stdint.h>
#include "gs_types.h"

#if defined(__linux__) || defined(__APPLE__)
#define EXPORT
#define CALL
#elif _WIN32
#define EXPORT __declspec(dllexport)
#define CALL __stdcall
#endif

extern "C"
{

// Primitive types
typedef uint64_t GS_VarUint;
typedef int64_t GS_VarInt;
typedef float GS_Float16;
typedef float GS_Float32;
typedef double GS_Float64;
typedef uint8_t GS_Uint8;
typedef uint16_t GS_Uint16;
typedef uint32_t GS_Uint32;
typedef uint64_t GS_Uint64;
typedef int8_t GS_Int8;
typedef int16_t GS_Int16;
typedef int32_t GS_Int32;
typedef int64_t GS_Int64;
typedef uint8_t GS_Boolean;
typedef uint8_t GS_Byte;
typedef char *GS_String;
typedef uint8_t *GS_Blob;

// Simple types (aliases)
typedef GS_String GS_TextureUrl1;
typedef GS_Uint8 GS_TextureRtpPT1;
typedef GS_Uint16 GS_Time1;
typedef GS_VarUint GS_ObjectID;
typedef GS_VarUint GS_Length;

// Tag type values for serializable objects
typedef enum
{
    GS_Tag_Invalid  = 0x00,
    GS_Tag_Head1    = 0x01,
    GS_Tag_Hand1    = 0x02,
    GS_Tag_Object1  = 0x03,
    GS_Tag_Mesh1    = 0x8000,
    GS_Tag_Hand2    = 0x8001,
    GS_Tag_HeadIPD1 = 0x8002
} GS_Tag;

// Complex types
typedef struct
{
    GS_Float32 x;
    GS_Float32 y;
    GS_Float32 z;
} GS_Loc1;

typedef struct
{
    GS_Float32 x;
    GS_Float32 y;
    GS_Float32 z;
    GS_Float16 vx;
    GS_Float16 vy;
    GS_Float16 vz;
} GS_Loc2;

typedef struct
{
    GS_Float16 x;
    GS_Float16 y;
    GS_Float16 z;
} GS_Norm1;

typedef struct
{
    GS_VarUint u;
    GS_VarUint v;
} GS_TextureUV1;

typedef struct
{
    GS_Float16 i;
    GS_Float16 j;
    GS_Float16 k;
} GS_Rot1;

typedef struct
{
    GS_Float16 si;
    GS_Float16 sj;
    GS_Float16 sk;
    GS_Float16 ei;
    GS_Float16 ej;
    GS_Float16 ek;
} GS_Rot2;

typedef struct
{
    GS_Float16 tx;
    GS_Float16 ty;
    GS_Float16 tz;
} GS_Transform1;

typedef struct {
    GS_ObjectID id;
    GS_Time1 time;
    GS_Loc1 position;
    GS_Rot1 rotation;
    GS_Loc1 scale;
    GS_Boolean parent_present;
    GS_ObjectID parent;
} GS_Object1;

typedef struct
{
    GS_Float16 ipd;
} GS_HeadIPD1;

typedef struct
{
    GS_ObjectID id;
    GS_Time1 time;
    GS_Loc2 location;
    GS_Rot2 rotation;
    GS_Boolean ipd_present;
    GS_HeadIPD1 ipd;
} GS_Head1;

typedef struct
{
    GS_ObjectID id;
    GS_VarUint num_vertices;
    GS_Loc1 *vertices;
    GS_VarUint num_normals;
    GS_Norm1 *normals;
    GS_VarUint num_textures;
    GS_TextureUV1 *textures;
    GS_VarUint num_triangles;
    GS_VarUint *triangles;
} GS_Mesh1;

typedef struct
{
    GS_ObjectID id;
    GS_Time1 time;
    GS_Boolean left;
    GS_Loc2 location;
    GS_Rot2 rotation;
} GS_Hand1;

typedef struct
{
    GS_Transform1 tip;
    GS_Transform1 ip;
    GS_Transform1 mcp;
    GS_Transform1 cmc;
} GS_Thumb;

typedef struct
{
    GS_Transform1 tip;
    GS_Transform1 dip;
    GS_Transform1 pip;
    GS_Transform1 mcp;
    GS_Transform1 cmc;
} GS_Finger;

typedef struct
{
    GS_ObjectID id;
    GS_Time1 time;
    GS_Boolean left;
    GS_Loc2 location;
    GS_Rot2 rotation;
    GS_Transform1 wrist;
    GS_Thumb thumb;
    GS_Finger index;
    GS_Finger middle;
    GS_Finger ring;
    GS_Finger pinky;
} GS_Hand2;

typedef struct
{
    GS_VarUint tag;
    GS_VarUint data_length;
    GS_Blob data;
} GS_UnknownObject;

typedef struct
{
    GS_VarUint type;
    union
    {
        GS_Object1 object1;
        GS_Head1 head1;
        GS_Hand1 hand1;
        GS_Mesh1 mesh1;
        GS_Hand2 hand2;
        GS_HeadIPD1 head_ipd1;
        GS_UnknownObject unknown_object;
    } u;
} GS_Object;

typedef struct
{
    void *opaque;
} GS_Encoder_Context;

typedef struct
{
    void *opaque;
} GS_Decoder_Context;

// Function prototypes for the public API
EXPORT int CALL GSEncoderInit(GS_Encoder_Context **gs_encoder_context,
                              unsigned char *buffer,
                              uint64_t buffer_length);
EXPORT int CALL GSEncodeObject(GS_Encoder_Context *gs_encoder_context,
                               GS_Object *object);
EXPORT size_t CALL GSEncoderDataLength(GS_Encoder_Context *gs_encoder_context);
EXPORT const char *CALL GetEncoderError(GS_Encoder_Context *gs_encoder_context);
EXPORT int CALL GSEncoderDestroy(GS_Encoder_Context *gs_encoder_context);

EXPORT int CALL GSDecoderInit(GS_Decoder_Context **gs_decoder_context,
                              unsigned char *buffer,
                              uint64_t data_length);
EXPORT int CALL GSDecodeObject(GS_Decoder_Context *gs_decoder_context,
                               GS_Object *object);
EXPORT const char *CALL GetDecoderError(GS_Decoder_Context *gs_decoder_context);
EXPORT int CALL GSDecoderDestroy(GS_Decoder_Context *gs_decoder_context);

} // extern C

#endif // GS_API_H
