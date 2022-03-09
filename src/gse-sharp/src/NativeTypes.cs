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

using VarUint = System.UInt64;
using GSHalf = System.Single;

namespace gs.sharp
{
    public enum Tag : VarUint
    {
        Invalid = 0x00,
        Head1 = 0x01,
        Hand1 = 0x02,
        Object1 = 0x03,
        Mesh1 = 0x8000,
        Hand2 = 0x8001,
        HeadIPD1 = 0x8002,
        Unknown = 0x20,
    }

    public readonly struct Loc1
    {
        public readonly float X;
        public readonly float Y;
        public readonly float Z;

        public Loc1(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public readonly struct Loc2
    {
        public readonly float X;
        public readonly float Y;
        public readonly float Z;
        public readonly GSHalf VX;
        public readonly GSHalf VY;
        public readonly GSHalf VZ;

        public Loc2(Loc1 xyz, GSHalf vx, GSHalf vy, GSHalf vz) : this(xyz.X, xyz.Y, xyz.Z, vx, vy, vz) { }

        public Loc2(float x, float y, float z, GSHalf vx, GSHalf vy, GSHalf vz)
        {
            X = x;
            Y = y;
            Z = z;
            VX = vx;
            VY = vy;
            VZ = vz;
        }
    }

    public readonly struct Norm1
    {
        public readonly GSHalf X;
        public readonly GSHalf Y;
        public readonly GSHalf Z;

        public Norm1(GSHalf x, GSHalf y, GSHalf z)
        {
            X = x;
            Y = y;
            Z = z;
        }
    }

    public readonly struct TextureUV1
    {
        public readonly VarUint u;
        public readonly VarUint v;

        public TextureUV1(VarUint u, VarUint v)
        {
            this.u = u;
            this.v = v;
        }
    }

    public readonly struct Rot1
    {
        public readonly GSHalf i;
        public readonly GSHalf j;
        public readonly GSHalf k;

        public Rot1(GSHalf i, GSHalf j, GSHalf k)
        {
            this.i = i;
            this.j = j;
            this.k = k;
        }
    }

    public readonly struct Rot2
    {
        public readonly GSHalf si;
        public readonly GSHalf sj;
        public readonly GSHalf sk;
        public readonly GSHalf ei;
        public readonly GSHalf ej;
        public readonly GSHalf ek;

        public Rot2(GSHalf si, GSHalf sj, GSHalf sk, GSHalf ei, GSHalf ej, GSHalf ek)
        {
            this.si = si;
            this.sj = sj;
            this.sk = sk;
            this.ei = ei;
            this.ej = ej;
            this.ek = ek;
        }
    }

    public readonly struct Transform1
    {
        public readonly GSHalf tx;
        public readonly GSHalf ty;
        public readonly GSHalf tz;

        public Transform1(GSHalf tx, GSHalf ty, GSHalf tz)
        {
            this.tx = tx;
            this.ty = ty;
            this.tz = tz;
        }
    }

    public readonly struct HeadIPD1
    {
        public readonly GSHalf IPD;
        public HeadIPD1(GSHalf ipd) => IPD = ipd;
    }

    public readonly struct Thumb
    {
        public readonly Transform1 Tip;
        public readonly Transform1 Ip;
        public readonly Transform1 Mcp;
        public readonly Transform1 Cmc;

        public Thumb(Transform1 tip, Transform1 ip, Transform1 mcp, Transform1 cmc)
        {
            Tip = tip;
            Ip = ip;
            Mcp = mcp;
            Cmc = cmc;
        }
    }

    public readonly struct Finger
    {
        public readonly Transform1 Tip;
        public readonly Transform1 Dip;
        public readonly Transform1 Pip;
        public readonly Transform1 Mcp;
        public readonly Transform1 Cmc;

        public Finger(Transform1 tip, Transform1 dip, Transform1 pip, Transform1 mcp, Transform1 cmc)
        {
            Tip = tip;
            Dip = dip;
            Pip = pip;
            Mcp = mcp;
            Cmc = cmc;
        }
    }
}
