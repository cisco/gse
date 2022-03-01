/*
 *  octet_string.cpp
 *
 *  Copyright (C) 2018 - 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved.
 *
 *  Description:
 *      This file implements utility functions for OctetString.
 *
 *  Portability Issues:
 *      None.
 *
 */

#include "octet_string.h"
#include "data_buffer.h"

/*
 *  operator<< for OctetString
 *
 *  Description:
 *      This function will produce a hex dump of the contents of
 *      the OctetString object.
 *
 *  Parameters:
 *      o [in]
 *          Ostream object to which to direct output.
 *
 *      octet_string [in]
 *          The OctetString to output.
 *
 *  Returns:
 *      A reference to the stream being operated on.
 *
 *  Comments:
 *      None.
 */
std::ostream &operator<<(std::ostream &o, const OctetString &octet_string)
{
    DataBuffer<> buffer(
        const_cast<unsigned char *>(&octet_string[0]),
        octet_string.size(),
        octet_string.size());

    o << buffer;

    return o;
}
