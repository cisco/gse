#pragma warning disable IDE0049 // Ignore name simplification warnings so native type sizes are explicit.

using System;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace gs.sharp
{
    /// <summary>
    /// Interface to libgse decoder.
    /// </summary>
    public class Decoder : IDisposable
    {
        /// <summary>
        /// Provides a view into the underlying decoder buffer.
        /// </summary>
        public IntPtr DataBuffer { get; private set; }

        private readonly DecoderContextHandle _context;
        private readonly bool _managedBuffer = false;
        private bool _disposedValue;

        /// <summary>
        /// Create a new decoder with the given buffer size.
        /// </summary>
        /// <param name="bufferSize">Size of the buffer to create in bytes.</param>
        /// <exception cref="InvalidOperationException">Underlying decoder failure.</exception>
        public Decoder(int bufferSize, IntPtr? buffer = null)
        {
            if (buffer == null)
            {
                // Create & manage our own buffer.
                DataBuffer = Marshal.AllocHGlobal(bufferSize);
                _managedBuffer = true;
            }
            else
            {
                // External buffer.
                DataBuffer = buffer.Value;
            }

            var returnCode = NativeMethods.GSDecoderInit(out IntPtr handle, DataBuffer, (System.UInt64)bufferSize);
            _context = new DecoderContextHandle(handle);
            if (returnCode != 0)
            {
                throw new InvalidOperationException($"GSDecoderInit returned {returnCode}");
            }
        }

        /// <summary>
        /// Attempt to fetch a decoded object.
        /// </summary>
        /// <returns>Tuple of object and type, if any available.</returns>
        public (object decoded, Type type)? Decode()
        {
            // Attempt decode.
            int decoded = NativeMethods.GSDecodeObject(_context, out GSObject gsObject);

            // Check return type.
            switch (decoded)
            {
                case -1:
                    // TODO: Try and get error.
                    throw new InvalidOperationException("Decode failed");
                case 0:
                    return null;
                case 1:
                    break;
                default:
                    throw new InvalidOperationException("Unexpected decode return call: " + decoded);
            }

            // Return the decoded object.
            Tag tag = (Tag)gsObject.Type;
            switch (tag)
            {
                case Tag.Object1:
                    return (gsObject.Object1, typeof(Object1));
                case Tag.Head1:
                    return (gsObject.Head1, typeof(Head1));
                case Tag.Hand1:
                    return (gsObject.Hand1, typeof(Hand1));
                case Tag.Hand2:
                    return (gsObject.Hand2, typeof(Hand2));
                case Tag.Mesh1:
                    return (gsObject.Mesh1, typeof(Mesh1));
                case Tag.Unknown:
                    return (gsObject.UnknownObject, typeof(UnknownObject));
                default:
                    throw new InvalidOperationException("Unknown decoded object type");
            }
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposedValue)
            {
                if (disposing)
                {
                    _context?.Dispose();
                    if (_managedBuffer)
                    {
                        // Free the buffer if managed directly.
                        Marshal.FreeHGlobal(DataBuffer);
                    }
                }
                _disposedValue = true;
            }
        }

        public void Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }

        private class DecoderContextHandle : SafeHandleZeroOrMinusOneIsInvalid
        {
            public DecoderContextHandle(IntPtr handle) : base(true) => SetHandle(handle);
            public DecoderContextHandle() : base(true) { }
            protected override bool ReleaseHandle() => NativeMethods.GSDecoderDestroy(handle) == 0;
        }

        private class NativeMethods
        {
            private const string GSE_LIB_NAME = "gse";

            [DllImport(GSE_LIB_NAME)]
            public static extern int GSDecoderInit(out IntPtr context, IntPtr buffer, System.UInt64 length);

            [DllImport(GSE_LIB_NAME)]
            public static extern int GSDecodeObject(DecoderContextHandle context, out GSObject gsObjects);

            [DllImport(GSE_LIB_NAME)]
            public static extern string GetDecoderError(DecoderContextHandle context);

            [DllImport(GSE_LIB_NAME)]
            internal static extern int GSDecoderDestroy(IntPtr context);
        }
    }
}

#pragma warning restore IDE0049