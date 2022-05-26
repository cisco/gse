/*
 *  gs_api_internal.h
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

#ifndef GS_API_INTERNAL_H
#define GS_API_INTERNAL_H

#include <string>
#include <vector>
#include <cstdint>
#include "gs_api.h"
#include "gs_encoder.h"
#include "gs_decoder.h"
#include "data_buffer.h"

namespace gs_api
{

// Internal encoder context
typedef struct
{
    gs::Encoder encoder;                        // gs::Encoder object
    gs::DataBuffer<> data_buffer;               // DataBuffer object
    std::string error;                          // Text for last error
} GS_Encoder_Context_Internal;

// Internal decoder context
typedef struct
{
    gs::Decoder decoder;                        // gs::Decoder object
    gs::DataBuffer<> data_buffer;               // DataBuffer object
    std::string error;                          // Text for last error
    std::vector<std::uint8_t *> allocations;    // Memory allocations
} GS_Decoder_Context_Internal;

// Functions to perform object conversion and serialization
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Object &object);
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Object1 &object);
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Head1 &object);
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Hand1 &object);
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Mesh1 &object);
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Hand2 &object);
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_HeadIPD1 &object);
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_UnknownObject &object);

// Functions to perform object deserialization and conversion
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        GS_Object &object);
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::Head1 &decoded_object,
                        GS_Object &object);
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::Hand1 &decoded_object,
                        GS_Object &object);
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::Mesh1 &decoded_object,
                        GS_Object &object);
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::Hand2 &decoded_object,
                        GS_Object &object);
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::HeadIPD1 &decoded_object,
                        GS_Object &object);
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::UnknownObject &decoded_object,
                        GS_Object &object);
int GSDeserializeObject(GS_Decoder_Context_Internal &context,
                        const gs::Object1 &decoded_object,
                        GS_Object &object);

#endif // GS_API_INTERNAL_H

} // namespace gs_api
