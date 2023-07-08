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
        static readonly string BUILD_LABEL;

        static Program()
        {
            string programFiles = Environment.GetFolderPath(Environment.SpecialFolder.ProgramFiles);
            MSBUILD_PATH = programFiles + @"\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\amd64\MSBuild.exe";
            SLN_DIR = Environment.CurrentDirectory;
#if USE_COMPRESSION
            PETITE_PATH = SLN_DIR + @"petite24\petite.exe";
#endif
            OUTPUT_DIR = Path.Combine(SLN_DIR, @"build_output\");
            DISTRO_DIR = Path.Combine(SLN_DIR, @"distro\");
            BUILD_LABEL = "Test";
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
                string src = Path.Combine(OUTPUT_DIR, fname);
                string dst = Path.Combine(DISTRO_DIR, fname);
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
                Out($"file not found: {path}");
                return true;
            }
            return false;
        }

        static bool DirectoryNotFound(string path)
        {
            if (!Directory.Exists(path))
            {
                Out($"directory not found: {path}");
                return true;
            }
            return false;
        }

        static bool RunProgram(string logName, string path, string args)
        {
            Out($"running {logName}");
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
                Out($"(run {logName} failed with exit code {p.ExitCode})");
                return false;
            }
            return true;
        }

        static bool RunBuild()
        {
            const string WHICH_PROJECTS = "osuReplayEditor;ReplayEditor";
            return RunProgram("msbuild", MSBUILD_PATH, $"/nologo /t:Rebuild /v:quiet /m /p:OutDir={OUTPUT_DIR};Configuration=Release;Platform=x64 /t:{WHICH_PROJECTS}");
        }

#if USE_COMPRESSION
        static bool RunPetite(string target)
        {
            return RunProgram("petite", PETITE_PATH, $"-1 -b0 -ts0 -y -v0 {target}");
        }
#endif
        static bool RunBuildManifest()
        {
            return RunProgram("ReplayEditorBuildManifest", Path.Combine(DISTRO_DIR, "osuReplayEditor.exe"), "-build_manifest");
        }

        static void Out(string msg)
        {
            Console.WriteLine($">> {msg}");
        }
    }
}
