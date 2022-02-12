using System;
using System.Windows.Forms;
using System.IO;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace osuReplayEditor
{
    static class Program
    {
        const int SUCCESS = 0;
        const int FAILURE = 1;
        [STAThread]
        static int Main(string[] args)
        {
            foreach (var arg in args)
            {
                if (arg == "-do_repair")
                {
                    doRepair();
                    return SUCCESS;
                }
                else if (arg == "-repair_textures")
                {
                    repairFiles("textures");
                    return SUCCESS;
                }
                else if (arg == "-build_manifest")
                {
                    return buildManifest();
                }
            }
            string[] requiredDlls = {
                "bass.dll", "bass_fx.dll", "ReplayEditor.dll"
            };
            foreach (var dllName in requiredDlls)
            {
                if (!File.Exists(dllName))
                {
                    repairFiles("DLLs");
                    return SUCCESS;
                }
            }
            Maintenance.Updater.Run(true);
            Main2();
            return SUCCESS;
        }

        private static int buildManifest()
        {
            try
            {
                List<string> dirs = new List<string>();
                List<string> files = new List<string>();
                var lines = File.ReadAllLines("manifest_files.txt");
                Directory.SetCurrentDirectory(Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location));
                bool readingFiles = false;
                foreach (var line in lines)
                {
                    if (line.Length == 0)
                    {
                        readingFiles = true;
                        continue;
                    }
                    if (line[0] == '#')
                        continue;
                    if (readingFiles)
                    {
                        files.Add(line);
                    }
                    else
                    {
                        dirs.Add(line);
                    }
                }
                Maintenance.ManifestFile manifest = new Maintenance.ManifestFile(dirs.ToArray(), files.ToArray());
                manifest.SaveAs("manifest");

            }
            catch (Exception e)
            {
                Console.WriteLine("Exception encountered");
                Console.WriteLine("-----------");
                Console.WriteLine(e.ToString());
                return FAILURE;
            }
            return SUCCESS;
        }

        private static void repairFiles(string missingWhat)
        {
            string url = Config.mainConfig.UpdateUrl;
            if (url == null)
            {
                MainForm.ErrorMessageOwnerless($"You are missing some {missingWhat}. Try redownloading (Auto-updates are OFF).");
                Environment.Exit(0);
                return;
            }
            var res = MessageBox.Show($"You are missing some {missingWhat}. Should I attempt to download them from <{url}>?", "Missing Files", MessageBoxButtons.YesNo);
            if (res != DialogResult.Yes)
            {
                Environment.Exit(0);
                return;
            }
            doRepair();
        }

        private static void doRepair()
        {
            File.Delete("manifest");
            Maintenance.Updater.Run(false);
        }

        [MethodImpl(MethodImplOptions.NoInlining)]
        private static void Main2()
        {
            AppDomain.CurrentDomain.UnhandledException += CurrentDomain_UnhandledException;
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }

        private static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
#if DEBUG
            Console.WriteLine("Crash !");
#else
            MessageBox.Show("The program has crashed, please review the contents of \"crash.log\" and include it on any bug report you fill out.", "Crash !");
#endif
            try
            {
                string d = DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss");
                File.WriteAllText("crash.log", "UTC: " + d + "\r\n" + e.ExceptionObject.ToString());
            }
            catch
            {
                Console.WriteLine("An exception occured while writing crash.log");
            }
        }
    }
}
