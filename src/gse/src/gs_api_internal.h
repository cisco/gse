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
 */

#ifndef GS_API_INTERNAL_H
#define GS_API_INTERNAL_H

#include <memory>
#include <string>
#include <vector>
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
    std::unique_ptr<DataBuffer<>> data_buffer;  // DataBuffer object
    std::string error;                          // Text for last error
} GS_Encoder_Context_Internal;

// Internal decoder context
typedef struct
{
    gs::Decoder decoder;                        // gs::Decoder object
    std::unique_ptr<DataBuffer<>> data_buffer;  // DataBuffer object
    std::string error;                          // Text for last error
    std::vector<std::uint8_t *> allocations;    // Memory allocations
} GS_Decoder_Context_Internal;

// Functions to perform object conversion and serialization
int GSSerializeObject(GS_Encoder_Context_Internal &context,
                      const GS_Object &object);
int GSSerializeObject(GS_Encoder_Context_Internal& context,
                      const GS_Object1& object);
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
int GSDeserializeObject(GS_Decoder_Context_Internal& context,
                        const gs::Object1& decoded_object,
                        GS_Object& object);

#endif // GS_API_INTERNAL_H

} // namespace gs_api
