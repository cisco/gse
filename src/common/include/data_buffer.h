/*
 *  data_buffer.h
 *
 *  Copyright (C) 2018 - 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved.
 *
 *  Description:
 *      Header file for the DataBuffer object, which provides functions to
 *      make it easy to read and write into a buffer.  Integer values written
 *      or read are stored in the buffer in network byte order.
 *
 *      The default allocator is std::allocator, though a custom allocator
 *      may be specified.  However, any allocator used must allocate
 *      unsigned char types since that is the underlying data buffer type.
 *
 *      This file also defines the OctetString type.
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

#ifndef DATA_BUFFER_H
#define DATA_BUFFER_H

#include <cstdint>
#include <chrono>
#include <memory>
#include "data_buffer_interface.h"

// DataBuffer object declaration
template <typename Alloc = std::allocator<unsigned char>>
class DataBuffer : public DataBufferInterface
{
    public:
        DataBuffer();
        DataBuffer(std::size_t buffer_size);
        DataBuffer(unsigned char *buffer,
                   std::size_t buffer_size,
                   std::size_t data_length);
        DataBuffer(const OctetString &data,
                   std::size_t buffer_size = 0);
        DataBuffer(const DataBuffer &other);    // Copy constructor
        DataBuffer(DataBuffer &&other) noexcept;// Move constructor
        virtual ~DataBuffer();

        // Functions to assign and compare DataBuffer objects
        DataBuffer& operator=(const DataBuffer &other);
        bool operator==(const DataBuffer &other) const;
        bool operator!=(const DataBuffer &other) const;

    protected:
        virtual void AllocateBuffer();
        virtual void FreeBuffer();

        Alloc allocator;                        // Memory allocator
};

// Define a shared pointer type for the DataBuffer object
typedef std::shared_ptr<DataBuffer<>> DataBufferPointer;

// Pull in all of the template function definitions
#include "data_buffer.cpp"

#endif // DATA_BUFFER_H
