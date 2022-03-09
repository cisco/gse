/*
 *  data_buffer_interface.cpp
 *
 *  Copyright (C) 2018 - 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved.
 *
 *  Description:
 *      This file implements the DataBufferInterface object, which provides
 *      functions to make it easy to read and write into a buffer.  Values are
 *      written to or read from the buffer in network byte order.
 *
 *      Note that this is an abstract base class that lacks the functionality
 *      to allocate memory, assign objects to another, or compare for equality.
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
#include <stdexcept>
#include <string.h>
#include <ostream>
#include <iomanip>
#include <ctype.h>
#ifdef _WIN32
#define NOMINMAX
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include "data_buffer_interface.h"

/*
 *  DataBufferInterface::DataBufferInterface
 *
 *  Description:
 *      Constructor for the DataBufferInterface object to initialize all
 *      members to zero.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
DataBufferInterface::DataBufferInterface() :
    buffer(nullptr),
    owns_buffer(false),
    buffer_size(0),
    data_length(0),
    read_length(0)
{
}

/*
 *  DataBufferInterface::DataBufferInterface
 *
 *  Description:
 *      Constructor for the DataBufferInterface object to initialize all members
 *      to zero, setting the buffer size to the value given.  This base
 *      class does not allocate any memory.
 *
 *  Parameters:
 *      buffer_size [in]
 *          Size of the internal buffer to allocate.  This must be greater
 *          than zero.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
DataBufferInterface::DataBufferInterface(std::size_t buffer_size) :
    buffer(nullptr),
    owns_buffer(false),
    buffer_size(buffer_size),
    data_length(0),
    read_length(0)
{
}

/*
 *  DataBufferInterface::DataBufferInterface
 *
 *  Description:
 *      Constructor for the DataBufferInterface object to assign a pre-existing
 *      buffer.  The DataBufferInterface does NOT take ownership of the buffer
 *      pointer.  When the object is destroyed, non-owned buffers are not freed.
 *      This form of construction exists to get the benefit of member
 *      functions to manipulate a pre-existing buffer without creating
 *      a new buffer.  This may help reduce memory copy operations.
 *
 *  Parameters:
 *      buffer [in]
 *          Pointer to buffer to use with the DataBufferInterface object.  This
 *          may be a nullptr.
 *
 *      buffer_size [in]
 *          Size of the buffer.  This must be greater than zero unless buffer
 *          is a nullptr.
 *
 *      data_length [in]
 *          The length of the usable data within the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
DataBufferInterface::DataBufferInterface(unsigned char *buffer,
                              std::size_t buffer_size,
                              std::size_t data_length) :
    buffer(buffer),
    owns_buffer(false),
    buffer_size(buffer_size),
    data_length(data_length),
    read_length(0)
{
    // Do not allow a null buffer that is claimed to be non-zero
    if (buffer && !buffer_size)
    {
        throw DataBufferException("Invalid buffer pointer or buffer size");
    }

    // Make sure the data length is not larger than the buffer size
    if (data_length > buffer_size)
    {
        throw DataBufferException("Data length larger than the buffer length");
    }
}

/*
 *  DataBufferInterface::DataBufferInterface
 *
 *  Description:
 *      Copy constructor for the DataBuffer object.
 *
 *  Parameters:
 *      other [in]
 *          The DataBuffer from which to copy data.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      Only internal non-buffer variables are copied.  The derived class
 *      is responsible for actually copying the buffer contents.
 */
DataBufferInterface::DataBufferInterface(const DataBufferInterface &other) :
    buffer(nullptr),
    owns_buffer(false),
    buffer_size(other.buffer_size),
    data_length(other.data_length),
    read_length(other.read_length)
{
}

/*
 *  DataBufferInterface::DataBufferInterface
 *
 *  Description:
 *      Move constructor for the DataBufferInterface object.
 *
 *  Parameters:
 *      other [in]
 *          The DataBufferInterface from which to move data.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
DataBufferInterface::DataBufferInterface(DataBufferInterface &&other) noexcept :
    buffer(other.buffer),
    owns_buffer(other.owns_buffer),
    buffer_size(other.buffer_size),
    data_length(other.data_length),
    read_length(other.read_length)
{
    // Clear the other objects buffer information
    other.buffer = nullptr;
    other.owns_buffer = false;
    other.buffer_size = 0;
    other.data_length = 0;
    other.read_length = 0;
}

/*
 *  DataBufferInterface::~DataBufferInterface
 *
 *  Description:
 *      Destructor for the DataBufferInterface object.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
DataBufferInterface::~DataBufferInterface()
{
}

/*
 *  DataBufferInterface::GetBufferPointer
 *
 *  Description:
 *      Return a const pointer into the data buffer at the given offset.
 *
 *  Parameters:
 *      offset [in]
 *          Offset into the data buffer (default is 0)
 *
 *  Returns:
 *      A pointer to the internal data buffer at the given offset or nullptr
 *      if the buffer does not exist of the offset is beyond the end of
 *      the actual buffer.
 *
 *  Comments:
 *      None.
 */
const unsigned char *
DataBufferInterface::GetBufferPointer(std::size_t offset) const
{
    if (buffer && (offset < buffer_size))
    {
        return buffer + offset;
    }
    else
    {
        return nullptr;
    }
}

/*
 *  DataBufferInterface::GetMutableBufferPointer
 *
 *  Description:
 *      Return a pointer into the data buffer at the given offset.
 *
 *  Parameters:
 *      offset [in]
 *          Offset into the data buffer (default is 0)
 *
 *  Returns:
 *      A pointer to the internal data buffer at the given offset.
 *
 *  Comments:
 *      None.
 */
unsigned char *DataBufferInterface::GetMutableBufferPointer(std::size_t offset)
{
    if (buffer && (offset < buffer_size))
    {
        return buffer + offset;
    }
    else
    {
        return nullptr;
    }
}

/*
 *  DataBufferInterface::TakeBufferOwnership
 *
 *  Description:
 *      Transfer ownership of the internal data buffer to the caller.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      A pointer to the internal data buffer.
 *
 *  Comments:
 *      The internal buffer MUST have been allocated using the same type of
 *      allocator, else there is a risk that memory will not be freed properly.
 */
unsigned char *DataBufferInterface::TakeBufferOwnership()
{
    // Assign p to the current buffer
    unsigned char *p = buffer;

    // Reset the internal data buffer values
    buffer = nullptr;
    owns_buffer = false;
    buffer_size = 0;
    data_length = 0;

    return p;
}

/*
 *  DataBufferInterface::SetBuffer
 *
 *  Description:
 *      Set the internal data buffer to the one provided.
 *
 *  Parameters:
 *      new_buffer [in]
 *          New buffer to use in place of any existing buffer.
 *
 *      new_buffer_size [in]
 *          Size of the buffer.
 *
 *      new_data_length [in]
 *          The length of the usable data within the buffer.
 *
 *      take_ownership [in]
 *          True if this object is to take ownership of the buffer.  If
 *          this is true, the buffer must have been allocated by
 *          the MemoryManager since that will be called to free the memory.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      The given buffer MUST be allocated using the same type of allocator,
 *      else there is a risk that memory will not be freed properly.
 */
void DataBufferInterface::SetBuffer(unsigned char *new_buffer,
                                    std::size_t new_buffer_size,
                                    std::size_t new_data_length,
                                    bool take_ownership)
{
    // Make sure the data length is not larger than the buffer size
    if (new_data_length > new_buffer_size)
    {
        throw DataBufferException("Data length larger than the buffer");
    }

    // Free existing data buffer
    FreeBuffer();

    // Initialize values
    buffer = new_buffer;
    buffer_size = new_buffer_size;
    data_length = new_data_length;
    owns_buffer = take_ownership;
}

/*
 *  DataBufferInterface::GetBufferSize
 *
 *  Description:
 *      Return the size of the underlying data buffer.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      The size in octets of the underlying data buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t DataBufferInterface::GetBufferSize() const
{
    return buffer_size;
}

/*
 *  DataBufferInterface::SetDataLength
 *
 *  Description:
 *      Set the length of the data in the buffer.
 *
 *  Parameters:
 *      length [in]
 *          The length of the data.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetDataLength(std::size_t length)
{
    // Ensure the length value does not exceed the buffer size
    if (length > buffer_size)
    {
        throw DataBufferException("Data length larger than the buffer size");
    }

    // Set the data length
    data_length = length;

    // Adjust the read_length if data_length is shortened
    read_length = std::min(data_length, read_length);
}

/*
 *  DataBufferInterface::GetDataLength
 *
 *  Description:
 *      Get the length of the data stored in the buffer.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Length of the data stored in the internal buffer.
 *
 *  Comments:
 *      None.
 */
std::size_t DataBufferInterface::GetDataLength() const
{
    return data_length;
}

/*
 *  DataBufferInterface::GetReadLength
 *
 *  Description:
 *      Get the length of the data that has been read.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Length of the number of octets that have been read.
 *
 *  Comments:
 *      None.
 */
std::size_t DataBufferInterface::GetReadLength() const
{
    return read_length;
}

/*
 *  DataBufferInterface::ResetReadLength
 *
 *  Description:
 *      Reset the read length to zero.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ResetReadLength()
{
    read_length = 0;
}

/*
 *  DataBufferInterface::AdvanceReadLength
 *
 *  Description:
 *      Update the read length value to efficiently skip octets when performing
 *      ReadValue() calls.
 *
 *  Parameters:
 *      count [in]
 *          Count of octets to skip.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AdvanceReadLength(std::size_t count)
{
    if ((read_length + count) > data_length)
    {
        throw DataBufferException(
                    "Attempt to advance read length beyond data length");
    }

    // Advance the read length forward
    read_length += count;
}

/*
 *  DataBufferInterface::operator[]
 *
 *  Description:
 *      Return a reference to an octet in the data buffer at the specified
 *      location.
 *
 *  Parameters:
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      A reference to the octet in the data buffer at the specified location.
 *
 *  Comments:
 *      This function will not increase the data length value.  If indexing
 *      into the buffer to insert a value, the caller must call SetDataLength()
 *      if the intent is to change the data length.
 */
unsigned char &DataBufferInterface::operator[](std::size_t offset)
{
    // Ensure the parameters will not result in an access violation
    if (offset >= buffer_size)
    {
        throw DataBufferException(
                    "Attempted memory access beyond the end of the buffer");
    }

    // Return a reference into the buffer
    return buffer[offset];
}

/*
 *  DataBufferInterface::operator[]
 *
 *  Description:
 *      Return a reference to an octet in the data buffer at the specified
 *      location.
 *
 *  Parameters:
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      A reference to the octet in the data buffer at the specified location.
 *
 *  Comments:
 *      This function will not increase the data length value.  If indexing
 *      into the buffer to insert a value, the caller must call SetDataLength()
 *      if the intent is to change the data length.
 */
const unsigned char &DataBufferInterface::operator[](std::size_t offset) const
{
    // Ensure the parameters will not result in an access violation
    if (offset >= buffer_size)
    {
        throw DataBufferException(
                    "Attempted memory access beyond the end of the buffer");
    }

    // Return a reference into the buffer
    return buffer[offset];
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the contents in the buffer at the specified offset.
 *      This is an overloaded function for looking at an arbitrary number
 *      of octets.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(unsigned char *value,
                                   std::size_t offset,
                                   std::size_t length) const
{
    // Ensure the parameters will not result in an access violation
    if ((offset + length) > data_length)
    {
        throw DataBufferException(
                    "Attempted memory access beyond the end of the buffer");
    }

    // Copy the buffer contents
    std::memcpy(value, buffer + offset, length);
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the octet in the buffer at the specified offset.
 *      This is an overloaded function for looking at a single octet.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(std::uint8_t &value,
                                   std::size_t offset) const
{
    GetValue(static_cast<unsigned char *>(&value),
             offset,
             sizeof(std::uint8_t));
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the contents in the buffer at the specified offset.
 *      This is an overloaded function for looking at a 16-bit value.
 *      This function will convert from network byte order to host byte order.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.  The buffer contents will be returned in host order.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(std::uint16_t &value,
                                   std::size_t offset) const
{
    GetValue(reinterpret_cast<unsigned char *>(&value),
             offset,
             sizeof(std::uint16_t));
    value = ntohs(value);
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the contents in the buffer at the specified offset.
 *      This is an overloaded function for looking at a 32-bit value.
 *      This function will convert from network byte order to host byte order.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.  The buffer contents will be returned in host order.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(std::uint32_t &value,
                                   std::size_t offset) const
{
    GetValue(reinterpret_cast<unsigned char *>(&value),
             offset,
             sizeof(std::uint32_t));
    value = ntohl(value);
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the contents in the buffer at the specified offset.
 *      This is an overloaded function for looking at a 64-bit value.
 *      This function will convert from network byte order to host byte order.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.  The buffer contents will be returned in host order.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(std::uint64_t &value,
                                   std::size_t offset) const
{
    std::uint32_t value_32;

    // Get the high-order bits
    GetValue(value_32, offset);
    value = static_cast<std::uint64_t>(value_32) << 32;

    // Get the low-order bits
    GetValue(value_32, offset + sizeof(std::uint32_t));
    value |= value_32;
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the contents in the buffer at the specified offset.
 *      This is an overloaded function for looking at a 32-bit IEEE floating
 *      point value.  This function will convert from network byte order to
 *      host byte order.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.  The buffer contents will be returned in host order.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(float &value, std::size_t offset) const
{
    std::uint32_t value_32;

    // Ensure the field sizes are 32-bits
    static_assert(sizeof(value) == 4, "expected float to be 32 bits");
    static_assert(sizeof(value_32) == 4, "expected uint32_t to be 32 bits");

    // Get the value as an integer, which will perform byte-order swaps
    GetValue(value_32, offset);

    // Copy the bit array into the return value
    std::memcpy(&value, &value_32, sizeof(value));
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the contents in the buffer at the specified offset.
 *      This is an overloaded function for looking at a 64-bit IEEE floating
 *      point value.  This function will convert from network byte order to
 *      host byte order.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.  The buffer contents will be returned in host order.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(double &value, std::size_t offset) const
{
    std::uint64_t value_64;

    // Ensure the field sizes are 64-bits
    static_assert(sizeof(value) == 8, "expected double to be 64 bits");
    static_assert(sizeof(value_64) == 8, "expected uint64_t to be 64 bits");

    // Get the value as an integer, which will perform byte-order swaps
    GetValue(value_64, offset);

    // Copy the bit array into the return value
    std::memcpy(&value, &value_64, sizeof(value));
}

/*
 *  DataBufferInterface::GetValue
 *
 *  Description:
 *      Peek at the contents in the buffer at the specified offset.
 *      This is an overloaded function for looking at a 32-bit value.
 *
 *  Parameters:
 *      value [out]
 *          The parameter to hold the contents of the buffer at the specified
 *          offset.  The buffer contents will be returned in host order.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *      length [in]
 *          The number of octets to copy from the buffer.  If the length is 0,
 *          then all of the octets in the buffer from the offset to the end
 *          of the buffer are returned.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::GetValue(OctetString &value,
                                   std::size_t offset,
                                   std::size_t length) const
{
    // Set the length to reach the end of the data if length was 0
    if (!length && (offset < data_length)) length = data_length - offset;

    // Ensure the parameters will not result in an access violation
    if ((offset + length) > data_length)
    {
        throw DataBufferException(
                "Attempt to access memory beyond the end of the buffer");
    }

    // Copy the buffer contents or clear the OctetString as appropriate
    if (length > 0)
    {
        value.assign(buffer + offset, buffer + offset + length);
    }
    else
    {
        value.clear();
    }
}

/*
 *  DataBufferInterface::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to be the provided
 *      value.  This is an overloaded function for setting at an arbitrary
 *      number of octets.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to assign to the buffer at the
 *          specified offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *      length [in]
 *          The number of octets to write into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetValue(const unsigned char *value,
                                   std::size_t offset,
                                   std::size_t length)
{
    // Ensure the parameters will not result in an access violation
    if ((offset + length) > buffer_size)
    {
        throw DataBufferException(
                    "Attempt to access memory beyond the end of the buffer");
    }

    // Copy the buffer contents
    if (length) std::memcpy(buffer + offset, value, length);
}

/*
 *  DataBufferInterface::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to be the provided
 *      value.  This is an overloaded function for setting at a single octet.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to assign to the buffer at the
 *          specified offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetValue(std::uint8_t value, std::size_t offset)
{
    SetValue(reinterpret_cast<const unsigned char *>(&value),
             offset,
             sizeof(std::uint8_t));
}

/*
 *  DataBufferInterface::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to be the provided
 *      value.  This is an overloaded function for setting at a 16-bit value.
 *      This function will convert from host byte order to network byte order.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to assign to the buffer at the
 *          specified offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetValue(std::uint16_t value, std::size_t offset)
{
    value = htons(value);
    SetValue(reinterpret_cast<const unsigned char *>(&value),
             offset,
             sizeof(std::uint16_t));
}

/*
 *  DataBufferInterface::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to be the provided
 *      value.  This is an overloaded function for setting at a 32-bit value.
 *      This function will convert from host byte order to network byte order.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to assign to the buffer at the
 *          specified offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetValue(std::uint32_t value, std::size_t offset)
{
    value = htonl(value);
    SetValue(reinterpret_cast<const unsigned char *>(&value),
             offset,
             sizeof(std::uint32_t));
}

/*
 *  DataBufferInterface::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to be the provided
 *      value.  This is an overloaded function for setting at a 64-bit value.
 *      This function will convert from host byte order to network byte order.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to assign to the buffer at the
 *          specified offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetValue(std::uint64_t value, std::size_t offset)
{
    // Set the high-order bits
    SetValue(static_cast<std::uint32_t>((value >> 32) & 0xffffffff), offset);

    // Set the low-order bits
    SetValue(static_cast<std::uint32_t>(value & 0xffffffff),
             offset + sizeof(std::uint32_t));
}

/*
 *  DataBufferInterface::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to be the provided
 *      value.  This is an overloaded function for setting at a 32-bit floating
 *      point value.  This function will convert from host byte order to
 *      network byte order.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to assign to the buffer at the
 *          specified offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetValue(float value, std::size_t offset)
{
    std::uint32_t value_32;

    // Ensure the field sizes are 32-bits
    static_assert(sizeof(value) == 4, "expected float to be 32 bits");
    static_assert(sizeof(value_32) == 4, "expected uint32_t to be 32 bits");

    // Copy the floating point value into a 32-bit bit array
    std::memcpy(&value_32, &value, sizeof(value_32));

    // Store the value in the buffer using network byte order
    SetValue(value_32, offset);
}

/*
 *  DataBufferInterface::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to be the provided
 *      value.  This is an overloaded function for setting at a 64-bit floating
 *      point value.  This function will convert from host byte order to
 *      network byte order.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to assign to the buffer at the
 *          specified offset.
 *
 *      offset [in]
 *          The offset into the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::SetValue(double value, std::size_t offset)
{
    std::uint64_t value_64;

    // Ensure the field sizes are 64-bits
    static_assert(sizeof(value) == 8, "expected double to be 64 bits");
    static_assert(sizeof(value_64) == 8, "expected uint64_t to be 64 bits");

    // Copy the floating point value into a 64-bit bit array
    std::memcpy(&value_64, &value, sizeof(value_64));

    // Store the value in the buffer using network byte order
    SetValue(value_64, offset);
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given octets to the end of the existing data.  The data
 *      length is increased by the given number of octets.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the octets to be appended to the
 *          data buffer.
 *
 *      length [in]
 *          The number of octets to be appended.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(const unsigned char *value,
                                      std::size_t length)
{
    std::size_t offset;

    // Ensure appending the data will not result in an buffer overflow
    if ((data_length + length) > buffer_size)
    {
        throw DataBufferException(
                    "Attempt to access memory beyond the end of the buffer");
    }

    // Set the offset to be equal to the current data length
    offset = data_length;

    // Update the data length
    data_length += length;

    // Now put the value into the buffer
    SetValue(value, offset, length);
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given octets to the end of the existing data.  The data
 *      length is increased by the given number of octets.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the octets to be appended to the
 *          data buffer.
 *
 *      length [in]
 *          The number of octets to be appended.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(const char *value, std::size_t length)
{
    AppendValue(reinterpret_cast<const unsigned char *>(value), length);
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given string to the end of the existing data.  The data
 *      length is increased by the given number of octets.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the string to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(const std::string &value)
{
    AppendValue(value.c_str(), value.length());
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given OctetString to the end of the existing data.  The data
 *      length is increased by the given number of octets.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the OctetString to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(const OctetString &value)
{
    AppendValue(&value[0], value.size());
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given value to the end of the existing data.  If the
 *      request would result in writing beyond the end of the buffer, an
 *      exception will be thrown.  The data length is increased by the size
 *      of the value type.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(std::uint8_t value)
{
    AppendValue(reinterpret_cast<const unsigned char *>(&value),
                sizeof(std::uint8_t));
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given value to the end of the existing data.  The data
 *      length is increased by the size of the value type.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(std::uint16_t value)
{
    value = htons(value);
    AppendValue(reinterpret_cast<const unsigned char *>(&value),
                sizeof(std::uint16_t));
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given value to the end of the existing data.  The data
 *      length is increased by the size of the value type.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(std::uint32_t value)
{
    value = htonl(value);
    AppendValue(reinterpret_cast<const unsigned char *>(&value),
                sizeof(std::uint32_t));
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given value to the end of the existing data.  The data
 *      length is increased by the size of the value type.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(std::uint64_t value)
{
    // Ensure appending the parameter will not result in an buffer overflow
    if ((data_length + sizeof(std::uint64_t)) > buffer_size)
    {
        throw DataBufferException(
                    "Attempt to access memory beyond the end of the buffer");
    }

    // Set the high-order bits
    AppendValue(static_cast<std::uint32_t>((value >> 32) & 0xffffffff));

    // Set the low-order bits
    AppendValue(static_cast<std::uint32_t>(value & 0xffffffff));
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given value to the end of the existing data.  The data
 *      length is increased by the size of the value type.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(float value)
{
    std::uint32_t value_32;

    // Ensure the field sizes are 32-bits
    static_assert(sizeof(value) == 4, "expected float to be 32 bits");
    static_assert(sizeof(value_32) == 4, "expected uint32_t to be 32 bits");

    // Copy the floating point value into a 32-bit bit array
    std::memcpy(&value_32, &value, sizeof(value_32));

    // Append the value using network byte order
    AppendValue(value_32);
}

/*
 *  DataBufferInterface::AppendValue
 *
 *  Description:
 *      Append the given value to the end of the existing data.  The data
 *      length is increased by the size of the value type.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the value to be appended to the
 *          data buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::AppendValue(double value)
{
    std::uint64_t value_64;

    // Ensure the field sizes are 64-bits
    static_assert(sizeof(value) == 8, "expected double to be 64 bits");
    static_assert(sizeof(value_64) == 8, "expected uint64_t to be 64 bits");

    // Copy the floating point value into a 32-bit bit array
    std::memcpy(&value_64, &value, sizeof(value_64));

    // Append the value using network byte order
    AppendValue(value_64);
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsquent reads will
 *      be from the next position in the data buffer.  This function is limited
 *      by the length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the octets into which data should be
 *          placed.
 *
 *      length [in]
 *          The number of octets to read from the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(unsigned char *value, std::size_t length)
{
    // Do nothing if the length is 0
    if (!length) return;

    // Ensure appending the data will not result in an buffer overflow
    if ((read_length + length) > data_length)
    {
        throw DataBufferException(
                    "Attempt to read beyond the end of the data");
    }

    std::memcpy(value, buffer + read_length, length);

    // Increment the read_length value
    read_length += length;
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsquent reads will
 *      be from the next position in the data buffer.  This function is limited
 *      by the length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the octets into which data should be
 *          placed.
 *
 *      length [in]
 *          The number of octets to read from the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(char *value, std::size_t length)
{
    ReadValue(reinterpret_cast<unsigned char*>(value), length);
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsquent reads will
 *      be from the next position in the data buffer.  This function is limited
 *      by the length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the octets into which data should be
 *          placed.  The octets are appended to the contents of the string.
 *
 *      length [in]
 *          The number of octets to read from the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(std::string &value, std::size_t length)
{
    // Do nothing if the length is 0
    if (!length) return;

    // Ensure appending the data will not result in an buffer overflow
    if ((read_length + length) > data_length)
    {
        throw DataBufferException(
                    "Attempt to read beyond the end of the data");
    }

    // Insert the new elements
    value.insert(value.end(),
                 buffer + read_length,
                 buffer + read_length + length);

    // Increment the read_length value
    read_length += length;
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsquent reads will
 *      be from the next position in the data buffer.  This function is limited
 *      by the length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter holding the octets into which data should be
 *          placed.  The octets are appended to the contents of the vector.
 *
 *      length [in]
 *          The number of octets to read from the buffer.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(OctetString &value, std::size_t length)
{
    // Do nothing if the length is 0
    if (!length) return;

    // Ensure appending the data will not result in an buffer overflow
    if ((read_length + length) > data_length)
    {
        throw DataBufferException(
                    "Attempt to read beyond the end of the data");
    }

    // Insert the new elements
    value.insert(value.end(),
                 buffer + read_length,
                 buffer + read_length + length);

    // Increment the read_length value
    read_length += length;
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read a single octet from the data buffer.  The internal read length
 *      variable will be adjusted so that subsquent reads will be from the next
 *      position in the data buffer.  This function is limited by the length of
 *      the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter into which a value shall be placed.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(std::uint8_t &value)
{
    ReadValue(reinterpret_cast<unsigned char*>(&value), 1);
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read a 16-bit unsigned integer from the data buffer.  The internal read
 *      length variable will be adjusted so that subsquent reads will be from
 *      the next position in the data buffer.  This function is limited by the
 *      length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter into which a value shall be placed.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(std::uint16_t &value)
{
    ReadValue(reinterpret_cast<unsigned char*>(&value), sizeof(value));
    value = ntohs(value);
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read a 32-bit unsigned integer from the data buffer.  The internal read
 *      length variable will be adjusted so that subsquent reads will be from
 *      the next position in the data buffer.  This function is limited by the
 *      length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter into which a value shall be placed.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(std::uint32_t &value)
{
    ReadValue(reinterpret_cast<unsigned char*>(&value), sizeof(value));
    value = ntohl(value);
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read a 64-bit unsigned integer from the data buffer.  The internal read
 *      length variable will be adjusted so that subsquent reads will be from
 *      the next position in the data buffer.  This function is limited by the
 *      length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter into which a value shall be placed.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(std::uint64_t &value)
{
    // Ensure appending the data will not result in an buffer overflow
    if ((read_length + sizeof(value)) > data_length)
    {
        throw DataBufferException(
                    "Attempt to read beyond the end of the data");
    }

    // Get the value
    GetValue(value, read_length);

    // Update the read length
    read_length += sizeof(value);
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read a floating point value from the data buffer.  The internal read
 *      length variable will be adjusted so that subsquent reads will be from
 *      the next position in the data buffer.  This function is limited by the
 *      length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter into which a value shall be placed.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(float &value)
{
    // Ensure appending the data will not result in an buffer overflow
    if ((read_length + sizeof(value)) > data_length)
    {
        throw DataBufferException(
                    "Attempt to read beyond the end of the data");
    }

    // Get the value
    GetValue(value, read_length);

    // Update the read length
    read_length += sizeof(value);
}

/*
 *  DataBufferInterface::ReadValue
 *
 *  Description:
 *      Read a double-precision floating point value from the data buffer.  The
 *      internal read length variable will be adjusted so that subsquent reads
 *      will be from the next position in the data buffer.  This function is
 *      limited by the length of the data, not the length of the buffer.
 *
 *  Parameters:
 *      value [in]
 *          The parameter into which a value shall be placed.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
void DataBufferInterface::ReadValue(double &value)
{
    // Ensure appending the data will not result in an buffer overflow
    if ((read_length + sizeof(value)) > data_length)
    {
        throw DataBufferException(
                    "Attempt to read beyond the end of the data");
    }

    // Get the value
    GetValue(value, read_length);

    // Update the read length
    read_length += sizeof(value);
}

/*
 *  operator<< for DataBufferInterface
 *
 *  Description:
 *      This function will produce a hex dump of the contents of
 *      the DataBufferInterface object's internal buffer.
 *
 *  Parameters:
 *      o [in]
 *          Ostream object to which to direct output.
 *
 *      data_buffer [in]
 *          The data buffer to output.
 *
 *  Returns:
 *      A reference to the stream being operated on.
 *
 *  Comments:
 *      None.
 */
std::ostream &operator<<(std::ostream &o,
                         const DataBufferInterface &data_buffer)
{
    unsigned counter = 0;                       // Character counter

    // Get the current stream flags
    std::ios_base::fmtflags flags(o.flags());

    // We want uppercase hex digits with zero-fill integers
    o << std::hex << std::setfill('0') << std::uppercase;

    // Get a pointer to the start of the buffer and buffer length
    const unsigned char *p = data_buffer.GetBufferPointer();
    std::size_t length = data_buffer.GetDataLength();

    // Iterate over the data buffer
    for (std::size_t j = 0; j < length; j++)
    {
        if (!(counter % 16))
        {
            if (counter) o << std::endl;
            o << std::setw(8) << counter;
            o << ":";
        }
        o << " " << std::setw(2) << int(*p);
        p++;
        counter++;
        if (!(counter % 16))
        {
            o << " :";
            for (unsigned i = 0; i < 16; i++)
            {
                if (isprint(*(p - 16 + i)))
                {
                    o << *(p - 16 + i);
                }
                else
                {
                    o << ".";
                }
            }
            o << ":";
        }
    }

    // Create the last line of output
    if (counter % 16)
    {
        for (unsigned i = 0; i < (16 - (counter % 16)); i++) o << "   ";
        o << " :";
        for (unsigned i = 0; i < 16; i++) {
            if (i < (counter % 16))
            {
                if (isprint(*(p - (counter % 16) + i)))
                {
                    o << *(p - (counter % 16) + i);
                }
                else
                {
                    o << ".";
                }
            }
            else
            {
                o << " ";
            }
        }
        o << ":";
    }

    // Produce a final new line only if there was any output
    if (counter) o << std::endl;

    // Restore the ostream flags
    o.flags(flags);

    return o;
}
