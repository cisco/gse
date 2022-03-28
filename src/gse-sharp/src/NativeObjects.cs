/*
 *  BSD 2-Clause License
 *
 *  Copyright (c) 2022, Cisco Systems
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

using System;
using System.Runtime.InteropServices;
using System.Linq;

// GSE Types.
using VarUint = System.UInt64;
using ObjectId = System.UInt64;
using Time1 = System.UInt16;
using Bool = System.Byte;
using GSHalf = System.Single;
using System.Collections.Generic;

namespace gs.sharp
{
    public interface IObject
    {
        ObjectId ID { get; }
    }

    public readonly struct Head1 : IObject
    {
        public ObjectId ID => id;
        public bool IPDPresent => Convert.ToBoolean(ipdPresent);

        private readonly ObjectId id;
        public readonly Time1 Time;
        public readonly Loc2 Location;
        public readonly Rot2 Rotation;
        private readonly Bool ipdPresent;
        public readonly HeadIPD1 IPD;

        public Head1(ObjectId id, Time1 time, Loc2 location, Rot2 rotation, GSHalf? ipd = null) : this()
        {
            this.id = id;
            Time = time;
            Location = location;
            Rotation = rotation;
            if (ipd.HasValue)
            {
                ipdPresent = Convert.ToByte(true);
                IPD = new HeadIPD1(ipd.Value);
            }
        }
    }

    public readonly struct Object1 : IObject
    {
        public ObjectId ID => id;

        private readonly ObjectId id;
        public readonly Time1 Time;
        public readonly Loc1 Location;
        public readonly Rot1 Rotation;
        public readonly Loc1 Scale;
        public readonly Bool HasParent;
        public readonly ObjectId Parent;

        public Object1(ObjectId id, Time1 time, Loc1 location, Rot1 rotation, Loc1 scale, ObjectId? parent = null) : this()
        {
            this.id = id;
            Time = time;
            Location = location;
            Rotation = rotation;
            Scale = scale;
            if (parent.HasValue)
            {
                HasParent = Convert.ToByte(parent.HasValue);
                Parent = parent.Value;
            }
        }
    }

    public readonly struct Hand1 : IObject
    {
        public ObjectId ID => id;

        private readonly ObjectId id;
        public readonly Time1 Time;
        public readonly Bool Left;
        public readonly Loc2 Location;
        public readonly Rot2 Rotation;
    }

    internal readonly struct Mesh1Ptr : IObject, IDisposable
    {
        public ObjectId ID => id;

        private readonly ObjectId id;
        public readonly VarUint NumVertices;
        public readonly IntPtr Vertices;
        public readonly VarUint NumNormals;
        public readonly IntPtr Normals;
        public readonly VarUint NumTextures;
        public readonly IntPtr Textures;
        public readonly VarUint NumTriangles;
        public readonly IntPtr Triangles;

        private readonly GCHandle _vertices;
        private readonly GCHandle _normals;
        private readonly GCHandle _textures;
        private readonly GCHandle _triangles;

        public Mesh1Ptr(Mesh1 mesh)
        {
            id = mesh.ID;

            NumVertices = mesh.NumVertices;
            _vertices = GCHandle.Alloc(mesh.Vertices, GCHandleType.Pinned);
            Vertices = _vertices.AddrOfPinnedObject();

            NumNormals = mesh.NumNormals;
            _normals = GCHandle.Alloc(mesh.Normals, GCHandleType.Pinned);
            Normals = _normals.AddrOfPinnedObject();

            NumTextures = mesh.NumTextures;
            _textures = GCHandle.Alloc(mesh.Textures, GCHandleType.Pinned);
            Textures = _textures.AddrOfPinnedObject();

            NumTriangles = mesh.NumTriangles;
            _triangles = GCHandle.Alloc(mesh.Triangles, GCHandleType.Pinned);
            Triangles = _triangles.AddrOfPinnedObject();
        }

        public void Dispose()
        {
            _vertices.Free();
            _normals.Free();
            _textures.Free();
            _triangles.Free();
        }
    }

    public readonly struct Mesh1 : IObject
    {
        public ObjectId ID => id;

        private readonly ObjectId id;
        public readonly VarUint NumVertices;
        public readonly Loc1[] Vertices;
        public readonly VarUint NumNormals;
        public readonly Norm1[] Normals;
        public readonly VarUint NumTextures;
        public readonly TextureUV1[] Textures;
        public readonly VarUint NumTriangles;
        public readonly VarUint[] Triangles;

        public Mesh1(ObjectId id, Loc1[] vertices, Norm1[] normals, TextureUV1[] textures, VarUint[] triangles)
        {
            this.id = id;

            // Vertices.
            NumVertices = vertices != null ? (VarUint)vertices.Length : 0;
            Vertices = vertices;

            // Normals.
            NumNormals = normals != null ? (VarUint)normals.Length : 0;
            Normals = normals;

            // Textures.
            NumTextures = textures != null ? (VarUint)textures.Length : 0;
            Textures = textures;

            // Triangles.
            NumTriangles = triangles != null ? (VarUint)triangles.Length : 0;
            Triangles = triangles;
        }

        internal Mesh1(Mesh1Ptr pointers)
        {
            id = pointers.ID;

            // Marshalling.
            int offset;

            // Marshal vertices.
            NumVertices = pointers.NumVertices;
            Vertices = new Loc1[NumVertices];
            offset = 0;
            for (int i = 0; i < (int)NumVertices; i++)
            {
                Vertices[i] = Marshal.PtrToStructure<Loc1>(pointers.Vertices + offset);
                offset += Marshal.SizeOf<Loc1>();
            }

            // Normals.
            NumNormals = pointers.NumNormals;
            Normals = new Norm1[NumNormals];
            offset = 0;
            for (int i = 0; i < (int)NumNormals; i++)
            {
                Normals[i] = Marshal.PtrToStructure<Norm1>(pointers.Normals + offset);
                offset += Marshal.SizeOf<Norm1>();
            }

            // Textures.
            NumTextures = pointers.NumTextures;
            Textures = new TextureUV1[NumTextures];
            offset = 0;
            for (int i = 0; i < (Int32)NumTextures; i++)
            {
                Textures[i] = Marshal.PtrToStructure<TextureUV1>(pointers.Textures + offset);
                offset += Marshal.SizeOf<TextureUV1>();
            }

            // Triangles.
            NumTriangles = pointers.NumTriangles;
            Triangles = new VarUint[NumTriangles];
            offset = 0;
            for (int i = 0; i < (UInt32)NumTriangles; i++)
            {
                Triangles[i] = Marshal.PtrToStructure<VarUint>(pointers.Triangles + offset);
                offset += Marshal.SizeOf<VarUint>();
            }
        }
    }

    public readonly struct Hand2 : IObject
    {
        public ObjectId ID => id;

        private readonly ObjectId id;
        public readonly Time1 Time;
        public readonly Bool Left;
        public readonly Loc2 Location;
        public readonly Rot2 Rotation;
        public readonly Transform1 Wrist;
        public readonly Thumb Thumb;
        public readonly Finger Index;
        public readonly Finger Middle;
        public readonly Finger Ring;
        public readonly Finger Pinky;

        public Hand2(ObjectId id, Time1 time, bool isLeftHand, Loc2 location, Rot2 rotation, Transform1 wrist, Thumb thumb, Finger index, Finger middle, Finger ring, Finger pinky)
        {
            this.id = id;
            Time = time;
            Left = Convert.ToByte(isLeftHand);
            Location = location;
            Rotation = rotation;
            Wrist = wrist;
            Thumb = thumb;
            Index = index;
            Middle = middle;
            Ring = ring;
            Pinky = pinky;
        }
    }

    public readonly struct UnknownObject
    {
        public readonly VarUint Tag;
        public readonly VarUint DataLength;
        public readonly IntPtr Data;

        public UnknownObject(VarUint tag, VarUint dataLength, IntPtr data)
        {
            Tag = tag;
            DataLength = dataLength;
            Data = data;
        }
    }

    [StructLayout(LayoutKind.Explicit)]
    internal struct GSObject
    {
        [FieldOffset(0)]
        public VarUint Type;

        [FieldOffset(sizeof(VarUint))]
        public Head1 Head1;

        [FieldOffset(sizeof(VarUint))]
        public Hand1 Hand1;

        [FieldOffset(sizeof(VarUint))]
        public Mesh1Ptr Mesh1;

        [FieldOffset(sizeof(VarUint))]
        public Hand2 Hand2;

        [FieldOffset(sizeof(VarUint))]
        public HeadIPD1 HeadIPD1;

        [FieldOffset(sizeof(VarUint))]
        public UnknownObject UnknownObject;

        [FieldOffset(sizeof(VarUint))]
        public Object1 Object1;

        public GSObject(Head1 head) : this()
        {
            Type = (VarUint)Tag.Head1;
            Head1 = head;
        }

        public GSObject(Hand1 hand) : this()
        {
            Type = (VarUint)Tag.Hand1;
            Hand1 = hand;
        }

        public GSObject(Mesh1Ptr mesh) : this()
        {
            Type = (VarUint)Tag.Mesh1;
            Mesh1 = mesh;
        }

        public GSObject(Hand2 hand) : this()
        {
            Type = (VarUint)Tag.Hand2;
            Hand2 = hand;
        }

        public GSObject(HeadIPD1 head) : this()
        {
            Type = (VarUint)Tag.HeadIPD1;
            HeadIPD1 = head;
        }

        public GSObject(UnknownObject obj) : this()
        {
            // TODO: Is this correct.
            Type = (VarUint)Tag.Unknown;
            UnknownObject = obj;
        }

        public GSObject(Object1 obj) : this()
        {
            Type = (VarUint)Tag.Object1;
            Object1 = obj;
        }
    }

    /// <summary>
    /// Helper factory for GSObject creation.
    /// </summary>
    internal class GSObjectFactory
    {
        /// <summary>
        /// Wrap the given into a <see cref="GSObject"/>.
        /// </summary>
        /// <typeparam name="T">Type of object to wrap.</typeparam>
        /// <param name="obj">Instance of object to wrap.</param>
        /// <returns>GSObject.</returns>
        /// <exception cref="ArgumentException">Unsupported object type.</exception>
        public static GSObject Make<T>(T obj)
        {
            if (obj is Head1 head1)
            {
                return new GSObject(head1);
            }
            else if (obj is Hand1 hand1)
            {
                return new GSObject(hand1);
            }
            else if (obj is Mesh1 mesh)
            {
                return new GSObject(new Mesh1Ptr(mesh));
            }
            else if (obj is Hand2 hand2)
            {
                return new GSObject(hand2);
            }
            else if (obj is UnknownObject unknown)
            {
                return new GSObject(unknown);
            }
            else if (obj is Object1 @object)
            {
                return new GSObject(@object);
            }
            else
            {
                throw new ArgumentException("Unexpected object type: " + typeof(T), nameof(obj));
            }
        }
    }
}
