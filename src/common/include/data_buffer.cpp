/*
 *  data_buffer.cpp
 *
 *  Copyright (C) 2018 - 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved.
 *
 *  Description:
 *      This module implements a DataBuffer object, which provides functions to
 *      make it easy to read and write into a buffer.  Integer values written
 *      or read are stored in the buffer in network byte order.
 *
 *      Note that this file is included in the data_buffer.h file since it
 *      contains all of the template-based function definitions.
 *
 *  Portability Issues:
 *      None.
 */

#ifndef DATA_BUFFER_CPP
#define DATA_BUFFER_CPP

#include <cstring>
#include <stdexcept>
#include <string.h>
#include <ostream>
#include <iomanip>
#include <ctype.h>
#include <utility>
#include "data_buffer.h"

/*
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Constructor for the DataBuffer object to initialize all members to
 *      zero.
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
template <typename Alloc>
DataBuffer<Alloc>::DataBuffer() :
    DataBufferInterface(),
    allocator()
{
}

/*
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Constructor for the DataBuffer object to initialize all members to
 *      zero, setting the buffer size to the value given, and allocating
 *      a buffer of the requested size.
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
template <typename Alloc>
DataBuffer<Alloc>::DataBuffer(std::size_t buffer_size) :
    DataBufferInterface(buffer_size),
    allocator()
{
    // Allocate the data buffer memory as requested
    AllocateBuffer();
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
template <typename Alloc>
DataBuffer<Alloc>::DataBuffer(const OctetString &data,
                              std::size_t buffer_size) :
    DataBufferInterface(buffer_size ? buffer_size : data.size()),
    allocator()
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
 *  DataBuffer::DataBuffer
 *
 *  Description:
 *      Constructor for the DataBuffer object to assign a pre-existing
 *      buffer.  The DataBuffer does NOT take ownership of the buffer pointer.
 *      When the object is destroyed, non-owned buffers are not freed.
 *      This form of construction exists to get the benefit of member
 *      functions to manipulate a pre-existing buffer without creating
 *      a new buffer.  This may help reduce memory copy operations.
 *
 *  Parameters:
 *      buffer [in]
 *          Pointer to buffer to use with the DataBuffer object.  This may be
 *          a nullptr.
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
template <typename Alloc>
DataBuffer<Alloc>::DataBuffer(unsigned char *buffer,
                              std::size_t buffer_size,
                              std::size_t data_length) :
    DataBufferInterface(buffer, buffer_size, data_length),
    allocator()
{
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
 *      Regardless of whether the original DataBuffer owned its data buffer
 *      or not, the copy will always own its buffer.
 */
template <typename Alloc>
DataBuffer<Alloc>::DataBuffer(const DataBuffer<Alloc> &other) :
    DataBufferInterface(other),
    allocator()
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
template <typename Alloc>
DataBuffer<Alloc>::DataBuffer(DataBuffer<Alloc> &&other) noexcept :
    DataBufferInterface(std::move(other)),
    allocator()
{
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
template <typename Alloc>
DataBuffer<Alloc>::~DataBuffer()
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
template <typename Alloc>
void DataBuffer<Alloc>::AllocateBuffer()
{
    // Free any previously allocated buffer
    FreeBuffer();

    // if the buffer size is zero, do not allocate memory
    if (!buffer_size)
    {
        buffer = nullptr;
        return;
    }

    // Allocate memory for the buffer
    try
    {
        buffer = allocator.allocate(buffer_size);
    }
    catch (const std::exception &e)
    {
        data_length = buffer_size = 0;
        throw DataBufferException(e.what());
    }
    catch (...)
    {
        data_length = buffer_size = 0;
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
template <typename Alloc>
void DataBuffer<Alloc>::FreeBuffer()
{
    // Do not attempt to free the buffer if we do not own it
    if (!buffer || !owns_buffer)
    {
        buffer = nullptr;
        return;
    }

    // Free the allocated buffer
    allocator.deallocate(buffer, buffer_size);

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
template <typename Alloc>
DataBuffer<Alloc> &DataBuffer<Alloc>::operator=(const DataBuffer<Alloc> &other)
{
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

    return *this;
}

/*
 *  DataBuffer::operator==
 *
 *  Description:
 *      Compare the equality of this and another DataBuffer.  This checks
 *      that the contents of their respective buffers have the same
 *      content by doing an octet-for-octet comparison.  For the purposes
 *      of equality, the buffer_size is immaterial.
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
template <typename Alloc>
bool DataBuffer<Alloc>::operator==(const DataBuffer<Alloc> &other) const
{
    if (allocator != other.allocator) return false;
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
template <typename Alloc>
bool DataBuffer<Alloc>::operator!=(const DataBuffer<Alloc> &other) const
{
    return !(*this == other);
}

#endif // DATA_BUFFER_CPP
