Game State Encoder Library
--------------------------

This library will encode and decode data as per the specification
documented here: https://github.com/fluffy/draft-jennings-game-state-over-rtp.

To use the library, one instantiates an object of type gs::Encoder.
This object will accept any one of defined object types and encode that
by calling the gs::Serializer object.  The gs::Serializer object, in turn,
invokes functions in the DataBuffer object to serialize the primitive
data types.

Everything is serialized in network byte order (big endian), though the current
draft specifies use of little endian.

Deserialization of data is via the gs::Decoder object.  It accepts a DataBuffer
and all objects found in the buffer are deserialized into a vector of
GSObjects.  The gs::Decoder will utilize the gs::Serializer, which in turn
calls functions in the DataBuffer object to extract primitive types.

C Interface
-----------

The C interface has the following encoder (serialization) functions:

  * GSEncoderInit()
  * GSEncoderDataLength()
  * GSEncodeObject()
  * GetEncoderError()
  * GSEncoderDestroy()

One would call GSEncoderInit() with a pointer to the raw buffer and a buffer
length.  That will create a context that is used with other calls.

A GS_Object is populated and passed into GSEncodeObject(), which will return 1
on success, 0 if the buffer cannot hold more data, or -1 on error.  On error,
one can check the error string.  Calling the GSEncoderDataLength() will return
the number of octets serialized into the buffer.  GSEncodeObject() may be
called repeatedly until there is no more room in the buffer.

GSEEncoderDestroy() will destroy the encoder context and any associated data,
so be sure to call GSEncoderDataLength() before GSEEncoderDestroy().

The C interface has the following decoder (deserialization) functions:

  * GSDecoderInit()
  * GSDecodeObject()
  * GetDecoderError()
  * GSDecoderDestroy()

One would call GSDecoderInit() with a pointer to the buffer holding the
object(s) to deserialize and a buffer length.  That will create a context that
is used with other calls.

One then calls GSDecodeObject() with a pointer to a GS_Object type.  The
structured will be zero-initialized by this API call, so the caller need not
initialize it.  The result result will be 1 if successful, 0 if there are no
more objects to deserialize from the buffer, or -1 if there is an error.
On error, one can check the error string.  GSDecodeObject() may be called
repeatedly until there are no more objects to decode.

As a part of the decode process, some data may be dynamically allocated on the
heap with pointers assigned inside the GS_Object.  Pointers to any such data
are stored with the decoder context and will be freed when GSEDecoderDestroy()
is called.  One may assume pointers remain valid until GSEDecoderDestroy() is
called.

GSEDecoderDestroy() will destroy the decoder context and any associated data.
