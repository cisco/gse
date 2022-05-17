![build](https://github.com/cisco/gse/actions/workflows/cmake.yml/badge.svg?branch=main)

Game State Encoder Library
--------------------------

This library will encode and decode data as per the specification
documented here: https://github.com/fluffy/draft-jennings-game-state-over-rtp.

Building
-------

```bash
cmake -S . -B build ; cmake --build build --parallel
```

Integrating In Other Projects
-----------------------------

The following is an example CMakeLists.txt file that can be used to include
this library in other software package builds.

```cmake
# Enable fetching content
include(FetchContent)

# Fetch the Game State Encoder Library
FetchContent_Declare(
    gse
    GIT_REPOSITORY  https://github.com/cisco/gse.git
    GIT_TAG         origin/main
)

# Make the library available
FetchContent_MakeAvailable(gse)
```

C++ Interface
-------------

The core of the library is written in C++ and a C interface exists (discussed
below) to facilitate using the library with other languages.

The two main C++ objects are:

  * gs::Encoder
  * gs::Decoder

These objects only need to be instantiated once.  They are stateless and
thread-safe, as long as no two threads write or read from the same `DataBuffer`
simultaneously.

The objects to encode are defined in the header file `gs_types.h` and
should align with the Internet Draft referenced above.  For example, there is
a structure called `gs::Hand1`.  To encode a `gs::Hand1` object, one would
populate the structure with the desired values and then call `gs::Encoder`'s
`Encode()` function, passing it a `DataBuffer` object into which the
object should be serialized.  The `DataBuffer` can allocate a buffer
or accept a user-provided buffer (and length) when it is instantiated.
The total length of the encoded object(s) can be determined by calling
`DataBuffer`'s `GetDataLength()` function.

Multiple objects can be encoded into the same `DataBuffer`.  Each call
to `Encode()` will result in the next object being appended to previously
serialized objects in the `DataBuffer`.

Likewise, multiple objects may be deserialized from the same `DataBuffer`.
To decode a buffer full of objects received over a network, for example,
one would create a `DataBuffer` object having a pointer to the start of the
encoded data.  Then, `gs::Decoder`'s `Decode()` function would be called.
The `Decode()` function accepts a variant `gs::GSObject` (and called
repetitively until there are no further objects) or a vector of variants
`gs::GSObjects` (requiring a single call to decode the entire buffer) as the
second parameter into which the decoded object(s) will be written.

For examples of how to use these objects, see the unit test code in
test/test_gs_encoder and test/test_gs_decoder or the C API code.

Note that the objects `gs::Serializer` and `gs::Deserializer` exist to
facilitate serialization and deserialization of various simpler data types
into and out of the `DataBuffer`.  One does not use those object directly.
Rather, they are used indirectly by `gs::Encoder` and `gs::Decoder`.

In the event of an error, the `gs::Encoder` may throw an exception of
type `gs::EncoderException` if an attempt is made to encode an invalid object
or `DataBufferException` if there is a problem with the `DataBuffer`, though
an effort was made to detect such issues to avoid such exceptions from being
thrown.  Likewise, `gs::Decoder` may throw an exception of the type
`DataBufferException` or `gs::DecoderException` if there is an error reading
from the data before or decoding the data buffer.

C Interface
-----------

The C interface has the following encoder (serialization) functions:

  * GSEncoderInit()
  * GSEncoderDataLength()
  * GSEncodeObject()
  * GetEncoderError()
  * GSEncoderDestroy()

One would call `GSEncoderInit()` with a pointer to the raw buffer and a buffer
length.  That will create a context that is used with other calls.

A `GS_Object` is populated and passed into `GSEncodeObject()`, which will
return 1 on success, 0 if the buffer cannot hold more data, or -1 on error.
On error, one can check the error string.  Calling the `GSEncoderDataLength()`
will return the number of octets serialized into the buffer.  `GSEncodeObject()`
may be called repeatedly until there is no more room in the buffer.

`GSEEncoderDestroy()` will destroy the encoder context and any associated data,
so be sure to call `GSEncoderDataLength()` before `GSEEncoderDestroy()`.

The C interface has the following decoder (deserialization) functions:

  * GSDecoderInit()
  * GSDecodeObject()
  * GetDecoderError()
  * GSDecoderDestroy()

One would call `GSDecoderInit()` with a pointer to the buffer holding the
object(s) to deserialize and a buffer length.  That will create a context that
is used with other calls.

One then calls `GSDecodeObject()` with a pointer to a `GS_Object` type.  The
structured will be zero-initialized by this API call, so the caller need not
initialize it.  The result result will be 1 if successful, 0 if there are no
more objects to deserialize from the buffer, or -1 if there is an error.
On error, one can check the error string.  `GSDecodeObject()` may be called
repeatedly until there are no more objects to decode.

As a part of the decode process, some data may be dynamically allocated on the
heap with pointers assigned inside the `GS_Object`.  Pointers to any such data
are stored with the decoder context and will be freed when `GSEDecoderDestroy()`
is called.  One may assume pointers remain valid until `GSEDecoderDestroy()` is
called.

`GSEDecoderDestroy()` will destroy the decoder context and any associated data.
