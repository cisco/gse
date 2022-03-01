using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace gs.sharp.test;

[TestClass]
public class EncoderTests
{
    private const int BUFFER_SIZE = 1500;

    [TestMethod]
    public void TestInitDestroy() => new Encoder(BUFFER_SIZE).Dispose();

    [TestMethod]
    public void TestEncodeHead()
    {
        var expected = new List<byte>
        {
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };
        var location = new Loc2(1.1f, 0.2f, 30, 0, 0, 0);
        var head = new Head1(id: 0, time: 0x0500, location, rotation: new Rot2());
        PerformEncodeTest(expected, new Head1[] { head });
    }

    [TestMethod]
    public void TestEncodeIPD()
    {
        var expected = new List<byte>
        {
            0x01, 0x27, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,

            // ipd
            0xc0, 0x80, 0x02, 0x02, 0x42, 0x48
        };

        var location = new Loc2(1.1f, 0.2f, 30, 0, 0, 0);
        var rotation = new Rot2();
        var head = new Head1(id: 0, time: 0x0500, location, rotation, ipd: 3.140625f);
        PerformEncodeTest(expected, new Head1[] { head });
    }

    [TestMethod]
    public void TestEncodeHand2()
    {
        var expected = new List<byte>
        {
            // tag
            0xc0, 0x80, 0x01,

            // Length
            0x80, 0xb8,

            // id
            0x0c,

            // time
            0x05, 0x00,

            // left
            0x01,

            // location
            0x3f, 0x8c, 0xcc, 0xcd, 0x3e, 0x4c, 0xcc, 0xcd,
            0x41, 0xf0, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00,
            0x00, 0x00,

            // rotoration
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x42, 0x48,

            // wrist
            0x00, 0x00, 0x42, 0x48, 0x00, 0x00,

            // thumb
            0x00, 0x00, 0x42, 0x48, 0x00, 0x00, 0x00, 0x00,
            0x42, 0x48, 0x00, 0x00, 0x00, 0x00, 0x42, 0x48,
            0x00, 0x00, 0x00, 0x00, 0x42, 0x48, 0x00, 0x00,

            // index
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            // middle
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            // ring
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            // pinky
            0x42, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        var location = new Loc2(1.1f, 0.2f, 30.0f, 3.140625f, 0, 0);
        var rotation = new Rot2(0, 0, 0, 0, 0, 3.140625f);
        var wrist = new Transform1(0, 3.140625f, 0);
        var transform = new Transform1(0, 3.140625f, 0);
        var thumb = new Thumb(transform, transform, transform, transform);
        var transformZero = new Transform1();
        var tip = new Transform1(3.140625f, 0, 0);
        var pinky = new Finger(tip, transformZero, transformZero, transformZero, transformZero);
        var hand2 = new Hand2(
            id: 12,
            time: 0x0500,
            isLeftHand: true,
            location,
            rotation,
            wrist,
            thumb,
            new Finger(),
            new Finger(),
            new Finger(),
            pinky);
        PerformEncodeTest(expected, new Hand2[] { hand2 });
    }

    [TestMethod]
    public void TestEncodeUnknown()
    {
        // Prep.
        var expected = new List<byte>
        {
            0x20, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };
        var datasize = expected.Count - 2;
        IntPtr ptr = Marshal.AllocHGlobal(datasize);
        Marshal.Copy(expected.ToArray(), 2, ptr, datasize);
        var obj = new UnknownObject((ulong)Tag.Unknown, (ulong)datasize, ptr);

        // Encode.
        PerformEncodeTest(expected, new UnknownObject[] { obj });
    }

    [TestMethod]
    public void TestEncodeMultiple()
    {
        var expected = new List<byte>
        {
            // Head1
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00,

            // Mesh1 tag
            0xc0, 0x80, 0x00, 0x32, 0x1b, 0x02, 0x3f, 0x80,
            0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x40, 0x40,
            0x00, 0x00, 0x3f, 0x80, 0x00, 0x00, 0x40, 0x00,
            0x00, 0x00, 0x40, 0x40, 0x00, 0x00, 0x03, 0x42,
            0x48, 0xBC, 0x00, 0x7B, 0xFF, 0x42, 0x48, 0xBC,
            0x00, 0x42, 0x48, 0x42, 0x48, 0xBC, 0x00, 0x7B,
            0xFF, 0x01, 0x01, 0x80, 0x81, 0x00,

            // Head1 - another copy
            0x01, 0x21, 0x00, 0x05, 0x00, 0x3f, 0x8c, 0xcc,
            0xcd, 0x3e, 0x4c, 0xcc, 0xcd, 0x41, 0xf0, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00
        };

        // Head 1.
        var location = new Loc2(1.1f, 0.2f, 30, 0, 0, 0);
        var head1 = new Head1(0, 0x0500, location, new Rot2());

        // Mesh1.
        var loc = new Loc1(1, 2, 3);
        var vertices = new Loc1[2] { loc, loc };
        var normals = new Norm1[3]
        {
            new Norm1(3.14f, -1, 65504),
            new Norm1(3.14f, -1, 3.14f),
            new Norm1(3.14f, -1, 65504),
        };
        var textures = new TextureUV1[1] { new TextureUV1(1, 129) };
        var mesh = new Mesh1(0x1b, vertices, normals, textures, null);

        // The third object is just a copy of the first.
        var copied = head1;

        // Create the encoder.
        Encoder encoder = null;
        byte[] encoded = null;
        try
        {
            encoder = new Encoder(BUFFER_SIZE);
            Assert.IsNotNull(encoder);

            // Encode the objects.
            encoder.Encode(head1);
            encoder.Encode(mesh);
            encoder.Encode(copied);

            // Check the encoded length.
            var dataLength = encoder.GetDataLength();
            Assert.AreEqual(expected.Count, dataLength);

            // Copy the managed buffer to verify.
            encoded = new byte[BUFFER_SIZE];
            Marshal.Copy(encoder.DataBuffer, encoded, 0, BUFFER_SIZE);
        }
        finally
        {
            // Destroy the encoder.
            encoder?.Dispose();
        }

        // Verify the buffer contents.
        for (var i = 0; i < expected.Count; i++)
        {
            Assert.AreEqual(expected[i], encoded[i]);
        }
    }

    private static void PerformEncodeTest<T>(List<byte> expected, IEnumerable<T> objects) where T : unmanaged
    {
        // Create the encoder.
        Encoder encoder = null;
        byte[] encoded = null;
        try
        {
            encoder = new Encoder(BUFFER_SIZE);
            Assert.IsNotNull(encoder);

            // Encode the objects.
            foreach (T obj in objects)
            {
                encoder.Encode(obj);
            }

            // Check the encoded length.
            var dataLength = encoder.GetDataLength();
            Assert.AreEqual(expected.Count, dataLength);

            // Copy the managed buffer to verify.
            encoded = new byte[BUFFER_SIZE];
            Marshal.Copy(encoder.DataBuffer, encoded, 0, BUFFER_SIZE);
        }
        finally
        {
            // Destroy the encoder.
            encoder?.Dispose();
        }

        // Verify the buffer contents.
        for (var i = 0; i < expected.Count; i++)
        {
            Assert.AreEqual(expected[i], encoded[i]);
        }
    }
}