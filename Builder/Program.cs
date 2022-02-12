using System;
using System.IO;
using System.Diagnostics;

namespace Builder
{
    class Program
    {
        static readonly string MSBUILD_PATH;
        static readonly string SLN_DIR;
#if USE_COMPRESSION
        static readonly string PETITE_PATH;
#endif
        static readonly string OUTPUT_DIR;
        static readonly string DISTRO_DIR;

        static Program()
        {
            string programFiles = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFilesX86);
            string userProfile = Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
            MSBUILD_PATH = programFiles + @"\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild.exe";
            SLN_DIR = userProfile + @"\source\repos\ReplayEditor\";
#if USE_COMPRESSION
            PETITE_PATH = SLN_DIR + @"petite24\petite.exe";
#endif
            OUTPUT_DIR = SLN_DIR + @"build_output\";
            DISTRO_DIR = SLN_DIR + @"distro\";
        }

        static int Main(string[] args)
        {
            int ret = 2;
            try
            {
                ret = Run() ? 0 : 1;
            }
            catch (Exception e)
            {
                Out("exception thrown");
                Console.WriteLine(e.ToString());
            }
            if (ret != 0)
            {
                Out("premature exit");
                Console.ReadKey();
            }
            return ret;
        }

        static bool Run()
        {
            if (FileNotFound(MSBUILD_PATH))
                return false;
            if (DirectoryNotFound(OUTPUT_DIR))
                return false;
            if (DirectoryNotFound(DISTRO_DIR))
                return false;
#if USE_COMPRESSION
            if (FileNotFound(PETITE_PATH))
                return false;
#endif
            if (!RunBuild())
                return false;
#if USE_COMPRESSION
            if (!RunPetite(OUTPUT_DIR + @"\ReplayEditor.dll"))
                return false;
#endif
            string[] moveToDistro = {
                "osuReplayEditor.exe",
                "ReplayEditor.dll"
            };
            foreach (var fname in moveToDistro)
            {
                string src = OUTPUT_DIR + "\\" + fname;
                string dst = DISTRO_DIR + "\\" + fname;
                File.Delete(dst);
                File.Move(src, dst);
            }
            if (!RunBuildManifest())
                return false;
            return true;
        }

        static bool FileNotFound(string path)
        {
            if (!File.Exists(path))
            {
                Out(string.Format("file not found: {0}", path));
                return true;
            }
            return false;
        }

        static bool DirectoryNotFound(string path)
        {
            if (!Directory.Exists(path))
            {
                Out(string.Format("directory not found: {0}", path));
                return true;
            }
            return false;
        }

        static bool RunProgram(string logName, string path, string args)
        {
            Out(string.Format("running {0}", logName));
            var psi = new ProcessStartInfo(path, args);
            psi.WorkingDirectory = SLN_DIR;
            psi.CreateNoWindow = true;
            psi.RedirectStandardOutput = true;
            psi.UseShellExecute = false;
            psi.WindowStyle = ProcessWindowStyle.Hidden;
            var p = Process.Start(psi);
            Console.WriteLine(p.StandardOutput.ReadToEnd());
            if (p.ExitCode != 0)
            {
                Out(string.Format("(run {0} failed with exit code {1})", logName, p.ExitCode));
                return false;
            }
            return true;
        }

        static bool RunBuild()
        {
            const string WHICH_PROJECTS = "osuReplayEditor;ReplayEditor";
            return RunProgram("msbuild", MSBUILD_PATH, string.Format("/nologo /v:quiet /m /p:OutDir={0};Configuration=Release;Platform=x64 /t:{1}", OUTPUT_DIR, WHICH_PROJECTS));
        }

#if USE_COMPRESSION
        static bool RunPetite(string target)
        {
            return RunProgram("petite", PETITE_PATH, string.Format("-1 -b0 -ts0 -y -v0 {0}", target));
        }
#endif
        static bool RunBuildManifest()
        {
            return RunProgram("ReplayEditorBuildManifest", DISTRO_DIR + "\\osuReplayEditor.exe", "-build_manifest");
        }

        static void Out(string msg)
        {
            Console.WriteLine(">> " + msg);
        }
    }
}
