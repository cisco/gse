/*
 *  gs_decoder.h
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

#ifndef GS_DECODER_H
#define GS_DECODER_H

#include <stdexcept>
#include <cstddef>
#include <string>
#include <vector>
#include "data_buffer.h"
#include "gs_types.h"
#include "gs_deserializer.h"

namespace gs
{

// DecoderException exception definition
class DecoderException : public std::runtime_error
{
    public:
        explicit DecoderException(const std::string &what_arg) :
            std::runtime_error(what_arg)
        {
        }

        explicit DecoderException(const char *what_arg) :
            std::runtime_error(what_arg)
        {
        }
};

// Game State Decoder object
class Decoder
{
    public:
        Decoder() = default;
        ~Decoder() = default;

        // Function to decode all objects found in the given buffer
        std::size_t Decode(DataBuffer &data_buffer, GSObjects &value);

        // Function to decode the next object from the given data buffer
        std::size_t Decode(DataBuffer &data_buffer, GSObject &value);

    protected:
        // Function to decode high-level objects
        std::size_t Decode(DataBuffer &data_buffer, Object1 &value);
        std::size_t Decode(DataBuffer &data_buffer, Head1 &value);
        std::size_t Decode(DataBuffer &data_buffer, Hand1 &value);
        std::size_t Decode(DataBuffer &data_buffer, Hand2 &value);
        std::size_t Decode(DataBuffer &data_buffer, Mesh1 &value);
        std::size_t Decode(DataBuffer &data_buffer, HeadIPD1 &value);
        std::size_t Decode(DataBuffer &data_buffer, UnknownObject &value);

        // Serialization functions for more complex types
        std::size_t Deserialize(DataBuffer &data_buffer,
                                Tag &value,
                                VarUint &raw_value);
        std::size_t Deserialize(DataBuffer &data_buffer, Loc1 &value);
        std::size_t Deserialize(DataBuffer &data_buffer, Loc2 &value);
        std::size_t Deserialize(DataBuffer &data_buffer, Norm1 &value);
        std::size_t Deserialize(DataBuffer &data_buffer, TextureUV1 &value);
        std::size_t Deserialize(DataBuffer &data_buffer, Rot1 &value);
        std::size_t Deserialize(DataBuffer &data_buffer, Rot2 &value);
        std::size_t Deserialize(DataBuffer &data_buffer, Transform1 &value);
        std::size_t Deserialize(DataBuffer &data_buffer, Thumb &value);
        std::size_t Deserialize(DataBuffer &data_buffer, Finger &value);

        // Deserialization function for a Blob type
        std::size_t Deserialize(DataBuffer &data_buffer, Blob &value)
        {
            return deserializer.Read(data_buffer, value);
        }

        // Deserialization function for vectors of any type
        template <typename T>
        std::size_t Deserialize(DataBuffer &data_buffer,
                                std::vector<T> &values);

        // Deserialization function for all other types
        template <typename T>
        std::size_t Deserialize(DataBuffer &data_buffer, T &value)
        {
            return deserializer.Read(data_buffer, value);
        }

        Deserializer deserializer;              // Deserializer object
};

} // namespace gs

#endif // GS_DECODER_H
