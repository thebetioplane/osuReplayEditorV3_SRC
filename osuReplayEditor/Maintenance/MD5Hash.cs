using System;
using System.IO;

namespace osuReplayEditor.Maintenance
{
    class MD5Hash
    {
        public byte[] Hash { get; private set; }
        public MD5Hash(Stream stream)
        {
            using (var m = System.Security.Cryptography.MD5.Create())
            {
                Hash = m.ComputeHash(stream);
            }
        }

        private MD5Hash()
        {
            Hash = new byte[16];
        }

        public static MD5Hash CreateEmpty()
        {
            return new byte[] { 0x68, 0xb3, 0x29, 0xda, 0x98, 0x93, 0xe3, 0x40, 0x99, 0xc7, 0xd8, 0xad, 0x5c, 0xb9, 0xc9, 0x40 };
        }

        public static implicit operator MD5Hash(byte[] aob)
        {
            if (aob.Length != 16)
                throw new ArgumentException("MD5 hashes must have a length of 16 bytes");
            MD5Hash obj = new MD5Hash();
            for (int i = 0; i < 16; ++i)
                obj.Hash[i] = aob[i];
            return obj;
        }

        public static implicit operator byte[] (MD5Hash obj)
        {
            return obj.Hash;
        }

        public int CompareTo(MD5Hash other)
        {
            if (other == null)
                return -1;
            for (int i = 0; i < 16; ++i)
            {
                int diff = Hash[i] - other.Hash[i];
                if (diff != 0)
                    return diff;
            }
            return 0;
        }

        private const string HEX_CHARS = "0123456789ABCDEF";

        public override string ToString()
        {
            var sb = new System.Text.StringBuilder(32);
            for (int i = 0; i < 16; ++i)
            {
                sb.Append(HEX_CHARS[(Hash[i] >> 4) & 0xf]);
                sb.Append(HEX_CHARS[Hash[i] & 0xf]);
            }
            return sb.ToString();
        }
    }
}
