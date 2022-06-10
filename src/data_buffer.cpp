/*
 *  data_buffer.cpp
 *
 *  Copyright (C) 2018 - 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved.
 *
 *  Description:
 *      This file implements the DataBuffer object, which provides functions to
 *      make it easy to read and write into a buffer.  Numeric values written
 *      to the buffer are stored in network byte order.  Likewise, numeric
 *      values read from the buffer are converted back to host byte order.
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
#include <string.h>
#include <iomanip>
#include <ctype.h>
#include <algorithm>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#endif
#include "data_buffer.h"

namespace gs
{

/*
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Constructor to create an empty DataBuffer object.
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
DataBuffer::DataBuffer() :
    buffer(nullptr),
    owns_buffer(false),
    buffer_size(0),
    data_length(0),
    read_length(0)
{
}

/*
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Constructor for the DataBuffer object to initialize all members
 *      to zero, setting the buffer size to the value given.
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
DataBuffer::DataBuffer(std::size_t buffer_size) :
    buffer(nullptr),
    owns_buffer(false),
    buffer_size(buffer_size),
    data_length(0),
    read_length(0)
{
    // Allocate the data buffer memory as requested
    AllocateBuffer();
}

/*
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Constructor for the DataBuffer object to assign a pre-existing
 *      buffer.  The DataBuffer does NOT take ownership of the buffer
 *      pointer.  When the object is destroyed, non-owned buffers are not freed.
 *      This form of construction exists to get the benefit of member
 *      functions to manipulate a pre-existing buffer without creating
 *      a new buffer.  This may help reduce memory copy operations.
 *
 *  Parameters:
 *      buffer [in]
 *          Pointer to buffer to use with the DataBuffer object.  This
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
DataBuffer::DataBuffer(unsigned char *buffer,
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
 *  DataBuffer::DataBuffer
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
DataBuffer::DataBuffer(const DataBuffer &other) :
    buffer(nullptr),
    owns_buffer(false),
    buffer_size(other.buffer_size),
    data_length(other.data_length),
    read_length(other.read_length)
{
    // Allocate memory as requested
    if (buffer_size)
    {
        // Allocate the data buffer memory as requested
        AllocateBuffer();

        // Copy the buffer contents
        std::memcpy(buffer, other.buffer, data_length);
    }
}

/*
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Move constructor for the DataBuffer object.
 *
 *  Parameters:
 *      other [in]
 *          The DataBuffer from which to move data.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
DataBuffer::DataBuffer(DataBuffer &&other) noexcept :
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
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Constructor for the DataBuffer object to create a data buffer holding
 *      the contents of the OctetString "data".
 *
 *  Parameters:
 *      data [in]
 *          The data to use to populate the Data Buffer
 *
 *      buffer_size [in]
 *          Size of the internal buffer to allocate.  If this value is 0,
 *          the buffer size will be set equal to data.size().  If it is greater
 *          than zero, it must be >= data.size().  The default is 0, meaning
 *          the buffer size matches the size of the data parameter.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      None.
 */
DataBuffer::DataBuffer(const OctetString &data, std::size_t buffer_size) :
    DataBuffer(buffer_size ? buffer_size : data.size())
{
    // Allocate memory as requested
    if (data.size() > this->buffer_size)
    {
        throw DataBufferException(
                        "Given buffer size is too small to hold the data");
    }

    // Allocate the data buffer memory as requested
    AllocateBuffer();

    // Copy the octets from the OctetString "data"
    AppendValue(data);
}

/*
 *  DataBuffer::~DataBuffer
 *
 *  Description:
 *      Destructor for the DataBuffer object.
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
DataBuffer::~DataBuffer()
{
    // Free any allocated buffer
    FreeBuffer();
}

/*
 *  DataBuffer::AllocateBuffer
 *
 *  Description:
 *      This function will allocate a buffer of the size in the buffer_size
 *      member variable using the MemoryManager (if valid) or from the heap
 *      otherwise.
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
void DataBuffer::AllocateBuffer()
{
    // Free any previously allocated buffer
    if (buffer) FreeBuffer();

    // if the buffer size is zero, do not allocate memory
    if (!buffer_size)
    {
        buffer = nullptr;
        return;
    }

    // Allocate memory for the buffer
    try
    {
        buffer = new unsigned char[buffer_size];
    }
    catch (const std::exception &e)
    {
        throw DataBufferException(e.what());
    }
    catch (...)
    {
        throw DataBufferException("Could not allocate buffer");
    }

    // Note buffer ownership
    owns_buffer = true;
}

/*
 *  DataBuffer::FreeBuffer
 *
 *  Description:
 *      Free the allocated memory buffer.
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
void DataBuffer::FreeBuffer()
{
    // Reset data length and read length values
    data_length = read_length = 0;

    // Do not attempt to free a buffer if one does not exist or is not owned
    if (!buffer || !owns_buffer)
    {
        buffer = nullptr;
        return;
    }

    // Free the allocated buffer
    delete[] buffer;

    // Reset the buffer pointer
    buffer = nullptr;
}

/*
 *  DataBuffer::operator=
 *
 *  Description:
 *      Assign one data buffer to another.  This will effectively
 *      copy the contents from the right to the left.
 *
 *  Parameters:
 *      other [in]
 *          The DataBuffer from which to copy data.
 *
 *  Returns:
 *      A reference to this DataBuffer.
 *
 *  Comments:
 *      The memory manager assigned to the DataBuffer during construction
 *      never changes.  Only the actual data is copied.  This allows buffers
 *      to be moved between DataBuffer objects where different parts of the
 *      application manage different memory pools.
 *
 *      Regardless if the copied-from DataBuffer owned its buffer or not,
 *      the copied-to DataBuffer will owns its buffer.
 */
DataBuffer &DataBuffer::operator=(const DataBuffer &other)
{
    // Do nothing if assigning to this
    if (this == &other) return *this;

    // If this object does not owns it's buffer, clear the contents before
    // copying
    if (!owns_buffer)
    {
        buffer = nullptr;
        buffer_size = 0;
    }

    // If the buffer sizes are not the same, re-allocate memory
    if (buffer_size != other.buffer_size)
    {
        // Set the buffer size to match
        buffer_size = other.buffer_size;

        // Allocate a new buffer of the right size
        AllocateBuffer();
    }

    // Copy the content of the other DataBuffer object
    data_length = other.data_length;
    if (data_length) std::memcpy(buffer, other.buffer, data_length);
    read_length = other.read_length;

    return *this;
}

/*
 *  DataBuffer::operator==
 *
 *  Description:
 *      Compare the equality of this and another DataBuffer.  This checks
 *      that the contents of their respective buffers have the same
 *      content by doing an octet-for-octet comparison.  For the purposes
 *      of equality, the buffer_size and read position are immaterial.
 *
 *  Parameters:
 *      other [in]
 *          The DataBuffer against which to compare.
 *
 *  Returns:
 *      True if equal, false if unequal.
 *
 *  Comments:
 *      None.
 */
bool DataBuffer::operator==(const DataBuffer &other) const
{
    if (this == &other) return true;
    if (data_length != other.data_length) return false;
    if (data_length == 0) return true;
    if ((buffer && !other.buffer) || (!buffer && other.buffer)) return false;
    if (!buffer) return true;
    if (!memcmp(buffer, other.buffer, data_length)) return true;

    return false;
}

/*
 *  DataBuffer::operator!=
 *
 *  Description:
 *      Compare the inequality of this and another DataBuffer.  This checks
 *      that the contents of their respective buffers have different
 *      content by doing an octet-for-octet comparison.
 *
 *  Parameters:
 *      other [in]
 *          The DataBuffer against which to compare.
 *
 *  Returns:
 *      True if unequal, false if equal.
 *
 *  Comments:
 *      None.
 */
bool DataBuffer::operator!=(const DataBuffer &other) const
{
    return !(*this == other);
}

/*
 *  DataBuffer::GetBufferPointer
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
DataBuffer::GetBufferPointer(std::size_t offset) const
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
 *  DataBuffer::GetMutableBufferPointer
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
unsigned char *DataBuffer::GetMutableBufferPointer(std::size_t offset)
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
 *  DataBuffer::TakeBufferOwnership
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
unsigned char *DataBuffer::TakeBufferOwnership()
{
    // Assign p to the current buffer
    unsigned char *p = buffer;

    // Reset the internal data buffer values
    buffer = nullptr;
    owns_buffer = false;
    buffer_size = 0;
    data_length = 0;
    read_length = 0;

    return p;
}

/*
 *  DataBuffer::SetBuffer
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
void DataBuffer::SetBuffer(unsigned char *new_buffer,
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
 *  DataBuffer::GetBufferSize
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
std::size_t DataBuffer::GetBufferSize() const
{
    return buffer_size;
}

/*
 *  DataBuffer::GetDataLength
 *
 *  Description:
 *      Get the length of the data stored in the buffer.  Note that if a buffer
 *      exists and data was inserted using SetValue(), that does not adjust the
 *      buffer's data length and, as such, this function would still
 *      indicate that the buffer is of zero length.  The data length is either
 *      set explicitly via SetDataLength() or automatically when AppendValue()
 *      functions are called.
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
std::size_t DataBuffer::GetDataLength() const
{
    return data_length;
}

/*
 *  DataBuffer::Empty
 *
 *  Description:
 *      Determine whether the DataBuffer is empty (i.e., the data length is
 *      zero).
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      True if the buffer is empty, false if it is not empty.
 *
 *  Comments:
 *      None.
 */
bool DataBuffer::Empty() const
{
    return (data_length == 0);
}

/*
 *  DataBuffer::SetDataLength
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
void DataBuffer::SetDataLength(std::size_t length)
{
    // Ensure there is an allocated buffer if length > 0
    if (!buffer && (length > 0))
    {
        throw DataBufferException("Cannot set non-zero data length as no "
                                  "buffer was allocated");
    }

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
 *  DataBuffer::GetReadLength
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
std::size_t DataBuffer::GetReadLength() const
{
    return read_length;
}

/*
 *  DataBuffer::ResetReadLength
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
void DataBuffer::ResetReadLength()
{
    read_length = 0;
}

/*
 *  DataBuffer::AdvanceReadLength
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
void DataBuffer::AdvanceReadLength(std::size_t count)
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
 *  DataBuffer::operator[]
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
unsigned char &DataBuffer::operator[](std::size_t offset)
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
 *  DataBuffer::operator[]
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
const unsigned char &DataBuffer::operator[](std::size_t offset) const
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
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(unsigned char *value,
                          std::size_t offset,
                          std::size_t length) const
{
    // If actually getting no data, just return
    if (length == 0) return;

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
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(std::uint8_t &value, std::size_t offset) const
{
    GetValue(static_cast<unsigned char *>(&value),
             offset,
             sizeof(std::uint8_t));
}

/*
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(std::uint16_t &value, std::size_t offset) const
{
    GetValue(reinterpret_cast<unsigned char *>(&value),
             offset,
             sizeof(std::uint16_t));
    value = ntohs(value);
}

/*
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(std::uint32_t &value, std::size_t offset) const
{
    GetValue(reinterpret_cast<unsigned char *>(&value),
             offset,
             sizeof(std::uint32_t));
    value = ntohl(value);
}

/*
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(std::uint64_t &value, std::size_t offset) const
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
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(float &value, std::size_t offset) const
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
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(double &value, std::size_t offset) const
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
 *  DataBuffer::GetValue
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
void DataBuffer::GetValue(OctetString &value,
                          std::size_t offset,
                          std::size_t length) const
{
    // Set the length to reach the end of the data if length was 0
    if (!length && (offset < data_length)) length = data_length - offset;

    // If the length is zero, just clear the octet string and return
    if (length == 0)
    {
        value.clear();
        return;
    }

    // Ensure the parameters will not result in an access violation
    if ((offset + length) > data_length)
    {
        throw DataBufferException(
                "Attempt to access memory beyond the end of the buffer");
    }

    // Copy the buffer contents or clear the OctetString as appropriate
    value.assign(buffer + offset, buffer + offset + length);
}

/*
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting an arbitrary
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
void DataBuffer::SetValue(const unsigned char *value,
                          std::size_t offset,
                          std::size_t length)
{
    // If actually writing no length, just return
    if (length == 0) return;

    // Ensure the parameters will not result in an access violation
    if (!buffer || ((offset + length) > buffer_size))
    {
        throw DataBufferException(
                    "Attempt to access memory beyond the end of the buffer");
    }

    // Copy the buffer contents
    std::memcpy(buffer + offset, value, length);
}

/*
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting a single octet.
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
void DataBuffer::SetValue(std::uint8_t value, std::size_t offset)
{
    SetValue(reinterpret_cast<const unsigned char *>(&value),
             offset,
             sizeof(std::uint8_t));
}

/*
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting a 16-bit value.
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
void DataBuffer::SetValue(std::uint16_t value, std::size_t offset)
{
    value = htons(value);
    SetValue(reinterpret_cast<const unsigned char *>(&value),
             offset,
             sizeof(std::uint16_t));
}

/*
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting a 32-bit value.
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
void DataBuffer::SetValue(std::uint32_t value, std::size_t offset)
{
    value = htonl(value);
    SetValue(reinterpret_cast<const unsigned char *>(&value),
             offset,
             sizeof(std::uint32_t));
}

/*
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting a 64-bit value.
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
void DataBuffer::SetValue(std::uint64_t value, std::size_t offset)
{
    // Set the high-order bits
    SetValue(static_cast<std::uint32_t>((value >> 32) & 0xffffffff), offset);

    // Set the low-order bits
    SetValue(static_cast<std::uint32_t>(value & 0xffffffff),
             offset + sizeof(std::uint32_t));
}

/*
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting a 32-bit floating
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
void DataBuffer::SetValue(float value, std::size_t offset)
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
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting a 64-bit floating
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
void DataBuffer::SetValue(double value, std::size_t offset)
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
 *  DataBuffer::SetValue
 *
 *  Description:
 *      Set the contents of the buffer at the given offset to the provided
 *      value.  This is an overloaded function for setting the contents of
 *      the OctetString.
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
void DataBuffer::SetValue(const OctetString &value, std::size_t offset)
{
    SetValue(&value[0], offset, value.size());
}

/*
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(const unsigned char *value, std::size_t length)
{
    std::size_t offset;

    // If appending no data, just return
    if (length == 0) return;

    // Ensure appending the data will not result in an buffer overflow
    if (!buffer || ((data_length + length) > buffer_size))
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
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(const char *value, std::size_t length)
{
    AppendValue(reinterpret_cast<const unsigned char *>(value), length);
}

/*
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(const std::string &value)
{
    AppendValue(value.c_str(), value.length());
}

/*
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(const OctetString &value)
{
    AppendValue(&value[0], value.size());
}

/*
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(std::uint8_t value)
{
    AppendValue(reinterpret_cast<const unsigned char *>(&value),
                sizeof(std::uint8_t));
}

/*
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(std::uint16_t value)
{
    value = htons(value);
    AppendValue(reinterpret_cast<const unsigned char *>(&value),
                sizeof(std::uint16_t));
}

/*
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(std::uint32_t value)
{
    value = htonl(value);
    AppendValue(reinterpret_cast<const unsigned char *>(&value),
                sizeof(std::uint32_t));
}

/*
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(std::uint64_t value)
{
    // Ensure appending the parameter will not result in an buffer overflow
    if (!buffer || ((data_length + sizeof(std::uint64_t)) > buffer_size))
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
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(float value)
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
 *  DataBuffer::AppendValue
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
void DataBuffer::AppendValue(double value)
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
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsequent reads will
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
void DataBuffer::ReadValue(unsigned char *value, std::size_t length)
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
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsequent reads will
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
void DataBuffer::ReadValue(char *value, std::size_t length)
{
    ReadValue(reinterpret_cast<unsigned char*>(value), length);
}

/*
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsequent reads will
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
void DataBuffer::ReadValue(std::string &value, std::size_t length)
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
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read the given number of octets from the data buffer.  The internal
 *      read length variable will be adjusted so that subsequent reads will
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
void DataBuffer::ReadValue(OctetString &value, std::size_t length)
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
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read a single octet from the data buffer.  The internal read length
 *      variable will be adjusted so that subsequent reads will be from the next
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
void DataBuffer::ReadValue(std::uint8_t &value)
{
    ReadValue(reinterpret_cast<unsigned char*>(&value), 1);
}

/*
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read a 16-bit unsigned integer from the data buffer.  The internal read
 *      length variable will be adjusted so that subsequent reads will be from
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
void DataBuffer::ReadValue(std::uint16_t &value)
{
    ReadValue(reinterpret_cast<unsigned char*>(&value), sizeof(value));
    value = ntohs(value);
}

/*
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read a 32-bit unsigned integer from the data buffer.  The internal read
 *      length variable will be adjusted so that subsequent reads will be from
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
void DataBuffer::ReadValue(std::uint32_t &value)
{
    ReadValue(reinterpret_cast<unsigned char*>(&value), sizeof(value));
    value = ntohl(value);
}

/*
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read a 64-bit unsigned integer from the data buffer.  The internal read
 *      length variable will be adjusted so that subsequent reads will be from
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
void DataBuffer::ReadValue(std::uint64_t &value)
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
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read a floating point value from the data buffer.  The internal read
 *      length variable will be adjusted so that subsequent reads will be from
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
void DataBuffer::ReadValue(float &value)
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
 *  DataBuffer::ReadValue
 *
 *  Description:
 *      Read a double-precision floating point value from the data buffer.  The
 *      internal read length variable will be adjusted so that subsequent reads
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
void DataBuffer::ReadValue(double &value)
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

} // namespace gs

/*
 *  operator<< for DataBuffer
 *
 *  Description:
 *      This function will produce a hex dump of the contents of
 *      the DataBuffer object's internal buffer.
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
                         const gs::DataBuffer &data_buffer)
{
    std::size_t counter = 0;                    // Character counter

    // Get the current stream flags
    auto flags = o.flags();

    // Set the fill character and store the old fill character
    auto fill = o.fill('0');

    // We want uppercase hex digits with zero-fill integers
    o << std::hex << std::uppercase;

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
            for (std::size_t i = 0; i < 16; i++)
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
        for (std::size_t i = 0; i < (16 - (counter % 16)); i++) o << "   ";
        o << " :";
        for (std::size_t i = 0; i < 16; i++)
        {
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

    // Restore the fill character
    o.fill(fill);

    return o;
}
