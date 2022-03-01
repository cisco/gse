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
 */

#ifndef GS_DECODER_H
#define GS_DECODER_H

#include <stdexcept>
#include <string>
#include <memory>
#include "data_buffer.h"
#include "gs_types.h"
#include "gs_deserializer.h"

namespace gs
{

// DecoderException exception definition
class DecoderException : public std::runtime_error
{
    public:
        DecoderException(const std::string &what_arg) :
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
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           GSObjects &value);

        // Function to decode the next object from the given data buffer
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           GSObject &value);

    protected:
        // Function to decode high-level objects
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           Object1 &value);
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           Head1 &value);
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           Hand1 &value);
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           Hand2 &value);
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           Mesh1 &value);
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           HeadIPD1 &value);
        std::size_t Decode(std::unique_ptr<DataBuffer<>> &data_buffer,
                           UnknownObject &value);

        // Serialization functions for more complex types
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Tag &value,
                                VarUint &raw_value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Loc1 &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Loc2 &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Norm1 &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                TextureUV1 &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Rot1 &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Rot2 &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Transform1 &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Thumb &value);
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                Finger &value);

        // Deserialization function for a Blob type
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                               Blob &value)
        {
            return deserializer.Read(data_buffer, value);
        }

        // Deserialization function for vectors of any type
        template <typename T>
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                std::vector<T> &values);

        // Deserialization function for all other types
        template <typename T>
        std::size_t Deserialize(std::unique_ptr<DataBuffer<>> &data_buffer,
                                T &value)
        {
            return deserializer.Read(data_buffer, value);
        }

        Deserializer deserializer;
        std::unique_ptr<DataBuffer<>> null_buffer;
};

} // namespace gs

#endif // GS_DECODER_H
