﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JJEngine
{
    public struct Vector3
    {
        public float X, Y, Z;
        public static Vector3 Zero = new Vector3(0.0f);
        public static Vector3 One = new Vector3(1, 1, 1);

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public override string ToString()
        {
            return $"Vector3 : [ {X}, {Y}, {Z} ]";
        }
    }
}
