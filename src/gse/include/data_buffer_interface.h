/*
 *  data_buffer_interface.h
 *
 *  Copyright (C) 2018 - 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved.
 *
 *  Description:
 *      Header file for the DataBufferInterface object, which provides functions
 *      to make it easy to read and write into a buffer.  Values are written to
 *      or read from the buffer in network byte order.
 *
 *      Note that this is an abstract base class that lacks the functionality
 *      to allocate memory, assign objects to another, or compare for equality.
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

#ifndef DATA_BUFFER_INTERFACE_H
#define DATA_BUFFER_INTERFACE_H

#include <cstdint>
#include <cstddef>
#include <chrono>
#include <memory>
#include "octet_string.h"

/*
 * DataBufferException
 *
 * This class defines an exception object that will be thrown if the
 * DataBufferInterface (or derived objects) cannot allocate memory, an attempt
 * is made to write beyond the buffer, or an attempt is made to read beyond
 * the data length.
 */
class DataBufferException : public std::runtime_error
{
    public:
        DataBufferException(const std::string &what_arg) :
            std::runtime_error(what_arg) {}
};

// DataBufferInterface object declaration
class DataBufferInterface
{
    public:
        DataBufferInterface();
        DataBufferInterface(std::size_t buffer_size);
        DataBufferInterface(unsigned char *buffer,
                            std::size_t buffer_size,
                            std::size_t data_length);
        DataBufferInterface(const DataBufferInterface &other);
        DataBufferInterface(DataBufferInterface &&other) noexcept;
        virtual ~DataBufferInterface();

        // Functions to get access to the underlying data buffer
        unsigned char *GetMutableBufferPointer(std::size_t offset = 0);
        const unsigned char *GetBufferPointer(std::size_t offset = 0) const;

        // Transfer ownership of the internal buffer to the caller
        unsigned char *TakeBufferOwnership();

        // Set the buffer to the specified address
        void SetBuffer(unsigned char *new_buffer,
                       std::size_t new_buffer_size,
                       std::size_t data_length,
                       bool take_ownership);

        // Function to return the data buffer size
        std::size_t GetBufferSize() const;

        // Functions to get/set the content length or read index
        std::size_t GetDataLength() const;
        void SetDataLength(std::size_t length);
        inline bool Empty() const { return (data_length == 0); }
        std::size_t GetReadLength() const;
        void ResetReadLength();
        void AdvanceReadLength(std::size_t count);

        // Functions to get contents of the internal buffer; these do not
        // adjust the read data length variable that keeps track of where to
        // perform subsequent read operations
        unsigned char &operator[](std::size_t offset);
        const unsigned char &operator[](std::size_t offset) const;
        void GetValue(unsigned char *value,
                      std::size_t offset,
                      std::size_t length) const;
        void GetValue(std::uint8_t &value, std::size_t offset) const;
        void GetValue(std::uint16_t &value, std::size_t offset) const;
        void GetValue(std::uint32_t &value, std::size_t offset) const;
        void GetValue(std::uint64_t &value, std::size_t offset) const;
        void GetValue(float &value, std::size_t offset) const;
        void GetValue(double &value, std::size_t offset) const;
        void GetValue(OctetString &value,
                      std::size_t offset = 0,
                      std::size_t length = 0) const;

        // Functions to set the contents of a internal buffer
        void SetValue(const unsigned char *value,
                      std::size_t offset,
                      std::size_t length);
        void SetValue(std::uint8_t value, std::size_t offset);
        void SetValue(std::uint16_t value, std::size_t offset);
        void SetValue(std::uint32_t value, std::size_t offset);
        void SetValue(std::uint64_t value, std::size_t offset);
        void SetValue(float value, std::size_t offset);
        void SetValue(double value, std::size_t offset);

        // Functions to append to the contents of a internal buffer,
        // adjusting the data length value as necessary
        void AppendValue(const unsigned char *value, std::size_t length);
        void AppendValue(const char *value, std::size_t length);
        void AppendValue(const std::string &value);
        void AppendValue(const OctetString &value);
        void AppendValue(std::uint8_t value);
        void AppendValue(std::uint16_t value);
        void AppendValue(std::uint32_t value);
        void AppendValue(std::uint64_t value);
        void AppendValue(float value);
        void AppendValue(double value);

        // Functions to read to the contents of a internal buffer,
        // adjusting the read data length value as it moves
        void ReadValue(unsigned char *value, std::size_t length);
        void ReadValue(char *value, std::size_t length);
        void ReadValue(std::string &value, std::size_t length);
        void ReadValue(OctetString &value, std::size_t length);
        void ReadValue(std::uint8_t &value);
        void ReadValue(std::uint16_t &value);
        void ReadValue(std::uint32_t &value);
        void ReadValue(std::uint64_t &value);
        void ReadValue(float &value);
        void ReadValue(double &value);

    protected:
        virtual void AllocateBuffer() = 0;
        virtual void FreeBuffer() = 0;

        unsigned char *buffer;                  // Raw data buffer
        bool owns_buffer;                       // Buffer owned by this object?
        std::size_t buffer_size;                // Size of the allocated buffer
        std::size_t data_length;                // Length of data in buffer
        std::size_t read_length;                // Number of octets read
};

// Define a shared pointer type for the DataBufferInterface object
typedef std::shared_ptr<DataBufferInterface> DataBufferInterfacePointer;

// Produce a hex dump for data buffer contents
std::ostream &operator<<(std::ostream &o,
                         const DataBufferInterface &data_buffer);

#endif // DATA_BUFFER_INTERFACE_H
