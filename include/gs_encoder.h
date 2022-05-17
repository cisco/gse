/*
 *  gs_encoder.h
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

#ifndef GS_ENCODER_H
#define GS_ENCODER_H

#include <utility>
#include <stdexcept>
#include <string>
#include <cstddef>
#include "data_buffer.h"
#include "gs_types.h"
#include "gs_serializer.h"

namespace gs
{

// EncoderException exception definition
class EncoderException : public std::runtime_error
{
    public:
        explicit EncoderException(const std::string &what_arg) :
            std::runtime_error(what_arg)
        {
        }

        explicit EncoderException(const char *what_arg) :
        std::runtime_error(what_arg)
        {
        }
};

// Return result from Encode() calls, which is a count of objects and
// count of octets serialized
typedef std::pair<std::size_t, std::size_t> EncodeResult;

// Game State Encoder object
class Encoder
{
    public:
        Encoder() = default;
        ~Encoder() = default;

        // Function to encode a vector of objects
        EncodeResult Encode(DataBuffer<> &data_buffer, const GSObjects &value);

        // Function to encode a single object
        EncodeResult Encode(DataBuffer<> &data_buffer, const GSObject &value);

        // Function to encode high-level objects
        EncodeResult Encode(DataBuffer<> &data_buffer, const Object1 &value);
        EncodeResult Encode(DataBuffer<> &data_buffer, const Head1 &value);
        EncodeResult Encode(DataBuffer<> &data_buffer, const Hand1 &value);
        EncodeResult Encode(DataBuffer<> &data_buffer, const Hand2 &value);
        EncodeResult Encode(DataBuffer<> &data_buffer, const Mesh1 &value);
        EncodeResult Encode(DataBuffer<> &data_buffer,
                            const UnknownObject &value);

    protected:
        // Serialization functions for more complex types
        std::size_t Serialize(DataBuffer<> &data_buffer, Tag value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const HeadIPD1 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const Loc1 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const Loc2 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const Norm1 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer,
                              const TextureUV1 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const Rot1 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const Rot2 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer,
                              const Transform1 &value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const Thumb &value);
        std::size_t Serialize(DataBuffer<> &data_buffer, const Finger &value);

        // Serialization function for a Blob type
        std::size_t Serialize(DataBuffer<> &data_buffer, const Blob &value)
        {
            return serializer.Write(data_buffer, value);
        }

        // Serialization function for vectors of any type
        template <typename T>
        std::size_t Serialize(DataBuffer<> &data_buffer,
                              const std::vector<T> &values);

        // Serialization function for all other types
        template <typename T>
        std::size_t Serialize(DataBuffer<> &data_buffer, const T &value)
        {
            return serializer.Write(data_buffer, value);
        }

        Serializer serializer;                  // Serializer object
        DataBuffer<> null_buffer;               // Used to compute encoding size
};

} // namespace gs

#endif // GS_ENCODER_H
