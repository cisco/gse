/*
 *  gs_api.cpp
 *
 *  Copyright (C) 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved
 *
 *  Description:
 *      This module provides a C API for interacting with the Game State
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

#include <cstring>
#include "gs_encoder.h"
#include "gs_decoder.h"
#include "gs_types.h"
#include "data_buffer.h"
#include "gs_api.h"
#include "gs_api_internal.h"

extern "C"
{

// Define the encoder context type
typedef struct GS_Encoder_Context
{
    gs_api::GS_Encoder_Context_Internal context;
} GS_Encoder_Context;

// Define the decoder context type
typedef struct GS_Decoder_Context
{
    gs_api::GS_Decoder_Context_Internal context;
} GS_Decoder_Context;

/*
 *  GSEncoderInit
 *
 *  Description:
 *      This function will initialize internal structures to prepare to
 *      serialize data to the given data buffer.
 *
 *  Parameters:
 *      gs_encoder_context [out]
 *          A pointer to the encoder context that will be returned upon
 *          successful initialization.
 *
  *      buffer [in]
 *          A pointer to a data buffer to which objects will be serialized.
 *
 *      buffer_length [in]
 *          The number of octets in the data buffer.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      The created context will retain a reference to the provided buffer
 *      until the context is destroyed.
 */
int CALL GSEncoderInit(GS_Encoder_Context **gs_encoder_context,
                       unsigned char *buffer,
                       uint64_t buffer_length)
{
    int result = -1;

    // Initialize the gs_encoder_context
    *gs_encoder_context = nullptr;

    // Reject the request if the buffer length is zero
    if (buffer_length == 0) return result;

    try
    {
        // Create the encoder context
        *gs_encoder_context = new GS_Encoder_Context{
            {
                gs::Encoder(),
                gs::DataBuffer(buffer, buffer_length, 0),
                {}
            }
        };

        // Indicate success
        result = 0;
    }
    catch (...)
    {
        // Nothing we can really do
    }

    // If there was an error, destroy any allocated data
    if (result && *gs_encoder_context) delete *gs_encoder_context;

    return result;
}

/*
 *  GSEncodeObject
 *
 *  Description:
 *      This function will serialize the given object into the buffer provided
 *      when GSEncoderInit() was called.
 *
 *  Parameters:
 *      gs_encoder_context [in]
 *          A pointer to the encoder context created during initialization.
 *
 *      object [in]
 *          The object to serialize into the buffer.
 *
 *  Returns:
 *      1 if successful.
 *      0 if the buffer lacks space to encode the object.
 *      -1 if there is any other.
 *
 *      If an error occurs GSEncoderError() may provide a string explaining why.
 *
 *  Comments:
 *      None.
 */
int CALL GSEncodeObject(GS_Encoder_Context *gs_encoder_context,
                        GS_Object *object)
{
    int result = -1;
    size_t data_length = SIZE_MAX;

    // If a null pointer is provided, return an error
    if (!gs_encoder_context) return result;

    try
    {
        // Clear the error text
        gs_encoder_context->context.error.clear();

        // Take note of the data length so that it can be restored on error
        data_length = gs_encoder_context->context.data_buffer.GetDataLength();

        // Serialize the object onto the buffer
        result = gs_api::GSSerializeObject(gs_encoder_context->context,
                                           *object);
    }
    catch (const std::exception &e)
    {
        // Take note of the error text, if any
        gs_encoder_context->context.error = e.what();

        // Restore the data length on failure, if we have a value
        if (data_length != SIZE_MAX)
        {
            gs_encoder_context->context.data_buffer.SetDataLength(data_length);
        }
    }
    catch (...)
    {
        // Clear the error sting
        gs_encoder_context->context.error.clear();

        // Restore the data length on failure, if we have a value
        if (data_length != SIZE_MAX)
        {
            gs_encoder_context->context.data_buffer.SetDataLength(data_length);
        }
    }

    return result;
}

/*
 *  GSEncoderDataLength
 *
 *  Description:
 *      Return the length of the encoded object(s) in the data buffer.
 *
 *  Parameters:
 *      gs_encoder_context [out]
 *          A pointer to the encoder context.
 *
 *  Returns:
 *      The length of the encoded data buffer or SIZE_MAX if there was an error.
 *
 *  Comments:
 *      None.
 */
size_t CALL GSEncoderDataLength(GS_Encoder_Context *gs_encoder_context)
{
    // If a null pointer is provided, return an error
    if (!gs_encoder_context) return SIZE_MAX;

    try
    {
        return gs_encoder_context->context.data_buffer.GetDataLength();
    }
    catch (const std::exception &e)
    {
        // Is the context available? If so, note the error reason.
        gs_encoder_context->context.error = e.what();
    }
    catch (...)
    {
        // Nothing we can really do, but clear the error string
        gs_encoder_context->context.error.clear();
    }

    return SIZE_MAX;
}

/*
 *  GetEncoderError
 *
 *  Description:
 *      Return the error text (if any) produced as a result of the last
 *      error relating to this context.
 *
 *  Parameters:
 *      gs_encoder_context [in]
 *          A pointer to the encoder context created during initialization.
 *
 *  Returns:
 *      A NULL-terminated string containing the text of the last error.
 *      If an error occurs making this call, NULL is returned.
 *
 *  Comments:
 *      None.
 */
const char * CALL GetEncoderError(GS_Encoder_Context *gs_encoder_context)
{
    const char *error_string = nullptr;

    // If a null pointer is provided, return an error
    if (!gs_encoder_context) return error_string;

    try
    {
        // Take note of the data length so that it can be restored on error
        error_string = gs_encoder_context->context.error.c_str();
    }
    catch (const std::exception &e)
    {
        // Is the context available?
        gs_encoder_context->context.error = e.what();
    }
    catch (...)
    {
        // Nothing we can really do, but clear the error string
        gs_encoder_context->context.error.clear();
    }

    return error_string;
}

/*
 *  GSEncoderDestroy
 *
 *  Description:
 *      This function will destroy the structures created by GSEncoderInit
 *      and release internal references to the raw buffer.
 *
 *  Parameters:
 *      gs_encoder_context [in]
 *          A pointer to the encoding context created during initialization.
 *          Upon return, the context will no longer be valid.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int CALL GSEncoderDestroy(GS_Encoder_Context *gs_encoder_context)
{
    int result = -1;

    // If a null pointer is provided, return an error
    if (!gs_encoder_context) return result;

    try
    {
        // Destroy the context object
        delete gs_encoder_context;

        // Indicate success
        result = 0;
    }
    catch (...)
    {
        // Nothing we can really do
    }

    return result;
}

/*
 *  GSDecoderInit
 *
 *  Description:
 *      This function will initialize internal structures to prepare to
 *      deserialize data from a data buffer.
 *
 *  Parameters:
 *      gs_decoder_context [out]
 *          A pointer to the decoder context that will be returned upon
 *          successful initialization.
 *
 *      buffer [in]
 *          A pointer to a data buffer from which objects will be deserialized.
 *
 *      data_length [in]
 *          The number of octets in the data buffer to deserialize.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      The created context will retain a reference to the provided buffer
 *      until the context is destroyed.
 */
int CALL GSDecoderInit(GS_Decoder_Context **gs_decoder_context,
                       unsigned char *buffer,
                       uint64_t data_length)
{
    int result = -1;

    // Initialize the gs_decoder_context
    *gs_decoder_context = nullptr;

    // Reject the request if the buffer length is zero
    if (data_length == 0) return result;

    try
    {
        *gs_decoder_context = new GS_Decoder_Context{
            {
                gs::Decoder(),
                gs::DataBuffer(buffer, data_length, data_length),
                {},
                {}
            }
        };

        // Indicate success
        result = 0;
    }
    catch (...)
    {
        // Nothing we can really do
    }

    // If there was an error, destroy any allocated data
    if (result && *gs_decoder_context) delete *gs_decoder_context;

    return result;
}

/*
 *  GSDecodeObject
 *
 *  Description:
 *      This function will decode an object from the data buffer and return
 *      it to the caller.
 *
 *  Parameters:
 *      gs_decoder_context [in]
 *          A pointer to the decoder context created during initialization.
 *
 *      object [in/out]
 *          This is a pointer to a GS_Object object.  It will be
 *          zero-initialized by this function and then populated with values,
 *          some of which will be pointers that will be dynamically allocated.
 *          Those dynamic memory allocations will be freed when
 *          GSDecoderDestroy() is called.
 *
 *  Returns:
 *      1 if successfully decoded an object.
 *      0 if successful, but no objects left in the buffer to decode.
 *      -1 if there is an error.
 *
 *      If an error occurs GSDecoderError() may provide a string explaining why.
 *
 *  Comments:
 *      None.
 */
int CALL GSDecodeObject(GS_Decoder_Context *gs_decoder_context,
                        GS_Object *object)
{
    int result = -1;

    // If a null pointer is provided, return an error
    if (!gs_decoder_context) return result;

    try
    {
        // Clear the error text
        gs_decoder_context->context.error.clear();

        // Deserialize the object into the provided object pointer
        result = gs_api::GSDeserializeObject(gs_decoder_context->context,
                                             *object);
    }
    catch (const std::exception &e)
    {
        // Is the context available?
        gs_decoder_context->context.error = e.what();
    }
    catch (...)
    {
        // Nothing we can really do, but clear the error string
        gs_decoder_context->context.error.clear();
    }

    return result;
}

/*
 *  GetDecoderError
 *
 *  Description:
 *      Return the error text (if any) produced as a result of the last
 *      error relating to this context.
 *
 *  Parameters:
 *      gs_decoder_context [in]
 *          A pointer to the decoder context created during initialization.
 *
 *  Returns:
 *      A NULL-terminated string containing the text of the last error.
 *      If an error occurs making this call, NULL is returned.
 *
 *  Comments:
 *      None.
 */
const char * CALL GetDecoderError(GS_Decoder_Context *gs_decoder_context)
{
    const char *error_string = nullptr;

    // If a null pointer is provided, return an error
    if (!gs_decoder_context) return error_string;

    try
    {
        // Take note of the data length so that it can be restored on error
        error_string = gs_decoder_context->context.error.c_str();
    }
    catch (const std::exception &e)
    {
        // Is the context available?
        gs_decoder_context->context.error = e.what();
    }
    catch (...)
    {
        // Nothing we can really do, but clear the error string
        gs_decoder_context->context.error.clear();
    }

    return error_string;
}

/*
 *  GSDecoderDestroy
 *
 *  Description:
 *      This function will destroy the structures created by GSDecoderInit
 *      and release internal references to the raw buffer.
 *
 *  Parameters:
 *      gs_decoder_context [in]
 *          A pointer to the decoder context created during initialization.
 *          Upon return, the context will no longer be valid.
 *
 *  Returns:
 *      0 if successful, -1 if there is an error.
 *
 *  Comments:
 *      None.
 */
int CALL GSDecoderDestroy(GS_Decoder_Context *gs_decoder_context)
{
    int result = -1;

    // If a null pointer is provided, return an error
    if (!gs_decoder_context) return result;

    try
    {
        // Free all of the allocated memory
        for (auto p : gs_decoder_context->context.allocations) delete[] p;

        // Destroy the context object
        delete gs_decoder_context;

        // Indicate success
        result = 0;
    }
    catch (...)
    {
        // Nothing we can really do
    }

    return result;
}

} // extern C
