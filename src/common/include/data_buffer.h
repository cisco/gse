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
