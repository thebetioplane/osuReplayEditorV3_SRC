using System;
using System.IO;
using System.Collections.Generic;

namespace osuReplayEditor.Maintenance
{
    struct FileEntry
    {
        public string FileName;
        public MD5Hash Hash;
        public FileEntry(string fileName, MD5Hash hash)
        {
            FileName = fileName;
            Hash = hash;
        }
    }

    class ManifestFile
    {
        private const int MAGIC = 1136341535;
        public string[] Dirs;
        public FileEntry[] Files;

        public static ManifestFile FromMaster(ManifestFile master)
        {
            ManifestFile ret = new ManifestFile();
            ret.Dirs = master.Dirs.Clone() as string[];
            if (ret.Dirs == null)
                throw new Exception("cloned dirs array is null");
            ret.Files = new FileEntry[master.Files.Length];
            for (int i = 0; i < master.Files.Length; ++i)
            {
                string fname = master.Files[i].FileName;
                if (File.Exists(fname))
                {
                    using (var fs = new FileStream(fname, FileMode.Open, FileAccess.Read))
                    {
                        ret.Files[i] = new FileEntry(fname, new MD5Hash(fs));
                    }
                }
                else
                {
                    ret.Files[i] = new FileEntry(fname, MD5Hash.CreateEmpty());
                }
            }
            return ret;
        }

        private ManifestFile()
        {
        }

        public ManifestFile(string[] dirs, string[] files)
        {
            Dirs = dirs.Clone() as string[];
            if (Dirs == null)
                throw new Exception("cloned dirs array is null");
            Files = new FileEntry[files.Length];
            for (int i = 0; i < files.Length; ++i)
            {
                if (!File.Exists(files[i]))
                {
                    throw new FileNotFoundException("Trying to make hash for file", files[i]);
                }
                using (var fs = new FileStream(files[i], FileMode.Open, FileAccess.Read))
                {
                    Files[i] = new FileEntry(files[i], new MD5Hash(fs));
                }
            }
        }

        public ManifestFile(Stream stream)
        {
            using (BinaryReader reader = new BinaryReader(stream))
            {
                int magic = reader.ReadInt32();
                if (magic != MAGIC)
                    throw new Exception("Manifest file: magic number does not match");
                int count = reader.ReadInt32();
                Dirs = new string[count];
                for (int i = 0; i < count; ++i)
                {
                    Dirs[i] = reader.ReadString();
                }
                count = reader.ReadInt32();
                Files = new FileEntry[count];
                for (int i = 0; i < count; ++i)
                {
                    string name = reader.ReadString();
                    MD5Hash hash = reader.ReadBytes(16);
                    Files[i] = new FileEntry(name, hash);
                }
            }
        }

        public void SaveAs(string fname)
        {
            using (FileStream fs = new FileStream(fname, FileMode.Create, FileAccess.Write))
            using (BinaryWriter writer = new BinaryWriter(fs))
            {
                writer.Write(MAGIC);
                writer.Write(Dirs.Length);
                foreach (var dir in Dirs)
                {
                    writer.Write(dir);
                }
                writer.Write(Files.Length);
                foreach (FileEntry item in Files)
                {
                    writer.Write(item.FileName);
                    byte[] hash = item.Hash;
                    writer.Write(hash);
                }
            }
        }

        public bool HasEntry(FileEntry entry)
        {
            foreach (FileEntry fe in Files)
            {
                if (fe.FileName == entry.FileName && fe.Hash.CompareTo(entry.Hash) == 0)
                    return true;
            }
            return false;
        }

        public static FileEntry[] FindDifferences(ManifestFile current, ManifestFile goal)
        {
            List<FileEntry> ret = new List<FileEntry>();
            foreach (FileEntry item in goal.Files)
            {
                if (!current.HasEntry(item))
                    ret.Add(item);
            }
            return ret.ToArray();
        }
    }
}
