using System;
using System.IO;
using System.Collections.Generic;

namespace osuReplayEditor
{
    enum ConfigValidation { None, File, Directory };
    class ConfigValue
    {
        public string Value;
        public readonly string DefaultValue;
        public readonly string Description;
        public readonly bool Required;
        public readonly ConfigValidation Validation;
        public ConfigValue(string defaultValue, string description, bool required, ConfigValidation validation)
        {
            Value = null;
            DefaultValue = defaultValue;
            Description = description;
            Required = required;
            Validation = validation;
        }
    }

    public class Config
    {
        public static Config mainConfig = new Config("config.txt");

        public bool LoadedFromFile { get; private set; }

        private const string K_OSU_DB_PATH = "osu_db_path";
        private const string K_SONG_DIR_PATH = "osu_song_folder";
        private const string K_REPLAY_DIR_PATH = "osu_replay_folder";
        private const string K_UPDATE_URL = "update_url";

        private readonly Dictionary<string, ConfigValue> pairs = new Dictionary<string, ConfigValue>();
        public readonly string FileName;

        public string OsuDbPath
        {
            get { return pairs[K_OSU_DB_PATH]?.Value; }
            set { pairs[K_OSU_DB_PATH].Value = value; }
        }

        public string SongDirPath
        {
            get { return pairs[K_SONG_DIR_PATH]?.Value; }
            set { pairs[K_SONG_DIR_PATH].Value = value; }
        }

        public string ReplayDirPath
        {
            get { return pairs[K_REPLAY_DIR_PATH]?.Value; }
            set { pairs[K_REPLAY_DIR_PATH].Value = value; }
        }

        public string UpdateUrl
        {
            get { return pairs[K_UPDATE_URL]?.Value; }
            set { pairs[K_UPDATE_URL].Value = value; }
        }

        private void WriteComment(StreamWriter writer, string msg)
        {
            writer.Write("# ");
            writer.WriteLine(msg);
        }

        public void CreateNewConfigFile()
        {
            using (var fstream = new FileStream(FileName, FileMode.Create, FileAccess.Write))
            using (var writer = new StreamWriter(fstream))
            {
                writer.WriteLine("# Settings file");
                writer.WriteLine("# Blank lines and lines starting with '#' are ignored");
                foreach (var entry in pairs)
                {
                    writer.WriteLine();
                    WriteComment(writer, entry.Value.Description);
                    switch (entry.Value.Validation)
                    {
                        case ConfigValidation.None:
                            break;
                        case ConfigValidation.File:
                            WriteComment(writer, "File must exist");
                            break;
                        case ConfigValidation.Directory:
                            WriteComment(writer, "Folder must exist");
                            break;
                    }
                    if (entry.Value.Required)
                    {
                        WriteComment(writer, "Required - Must have a value");
                        // since these values are almost guaranteed to be wrong until the user changes it
                        // only set the value to default value when it is required
                        entry.Value.Value = entry.Value.DefaultValue;
                    }
                    else
                    {
                        WriteComment(writer, "Not Required - Can be left blank or deleted");
                    }
                    writer.Write(entry.Key);
                    writer.Write(" = ");
                    writer.WriteLine(entry.Value.Value ?? entry.Value.DefaultValue);
                }
            }
        }

        private void ReadConfigFile()
        {
            using (var fstream = new FileStream(FileName, FileMode.Open, FileAccess.Read))
            using (var reader = new StreamReader(fstream))
            {
                int lineNum = 0;
                while (!reader.EndOfStream)
                {
                    ++lineNum;
                    string line = reader.ReadLine();
                    if (line.Length <= 1)
                        continue;
                    if (line[0] == '#')
                        continue;
                    int index = line.IndexOf('=');
                    if (index == -1)
                        continue;
                    string key = line.Substring(0, index).Trim();
                    string value = line.Substring(index + 1).Trim();
                    if (key.Length == 0 || value.Length == 0)
                        continue;
                    if (!pairs.ContainsKey(key))
                        throw new Exception($"Line {lineNum} - the specified key '{key}' is not valid");
                    pairs[key].Value = value;
                }
            }
        }

        public void Validate()
        {
            foreach (var kv in pairs)
            {
                if (kv.Value.Required && kv.Value.Value == null)
                    throw new Exception($"Item '{kv.Key}' ({kv.Value.Description}) is required but it was left blank or omitted");
                switch (kv.Value.Validation)
                {
                    case ConfigValidation.None:
                        break;
                    case ConfigValidation.File:
                        if (kv.Value.Value != null && !File.Exists(kv.Value.Value))
                            throw new Exception($"Item '{kv.Key}' ({kv.Value.Description}) - the file specified ({kv.Value.Value}) does not exist");
                        break;
                    case ConfigValidation.Directory:
                        if (kv.Value.Value != null)
                        {
                            if (!Directory.Exists(kv.Value.Value))
                                throw new Exception($"Item '{kv.Key}' ({kv.Value.Description}) - the folder specified ({kv.Value.Value}) does not exist");
                            char lastChar = kv.Value.Value[kv.Value.Value.Length - 1];
                            if (lastChar != '\\' && lastChar != '/')
                                kv.Value.Value += '\\';
                        }
                        break;
                }
            }
        }

        public Config(string fname)
        {
            LoadedFromFile = false;
            FileName = fname;
            pairs.Add(K_OSU_DB_PATH, new ConfigValue(@"C:\osu!\osu!.db", "Path to osu db", true, ConfigValidation.File));
            pairs.Add(K_SONG_DIR_PATH, new ConfigValue(@"C:\osu!\songs\", "Path to song folder", true, ConfigValidation.Directory));
            pairs.Add(K_REPLAY_DIR_PATH, new ConfigValue(@"C:\osu!\replays\", "Path to replay folder, used by \"quick load\"", false, ConfigValidation.Directory));
            pairs.Add(K_UPDATE_URL, new ConfigValue(@"https://raw.githubusercontent.com/thebetioplane/osuReplayEditorV3/master/", "Url for automatic updates", false, ConfigValidation.None));
            try
            {
                if (File.Exists(fname))
                {
                    ReadConfigFile();
                    Validate();
                    LoadedFromFile = true;
                }
            }
            catch (Exception e)
            {
                MainForm.ErrorMessageOwnerless("Error while parsing config:\n\n" + e.Message);
                Environment.Exit(1);
            }
        }
    }
}
