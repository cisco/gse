/*
 *  octet_string.h
 *
 *  Copyright (C) 2018 - 2022
 *  Cisco Systems, Inc.
 *  All Rights Reserved.
 *
 *  Description:
 *      This file also defines the OctetString type and utility functions.
 *
 *  Portability Issues:
 *      None.
 */

#ifndef OCTET_STRING_H

#include <ostream>
#include <vector>
#include <cstdint>

// OctetString to store a vector of octets
typedef std::vector<std::uint8_t> OctetString;

// Produce a hex dump for OctetStrings using DataBuffer for assistance
std::ostream &operator<<(std::ostream &o, const OctetString &octet_string);

#endif // OCTET_STRING_H
