using System;
using System.IO;
using System.Threading.Tasks;
using System.Net;
using System.Windows.Forms;

namespace osuReplayEditor.Maintenance
{
    static class Updater
    {
        //private static ResourceLocker IsRunning = new ResourceLocker();
        private static bool hasBeenRun = false;

        public static void Run(bool runInBackground)
        {
            string url = Config.mainConfig.UpdateUrl;
            if (url == null)
                return;
            if (hasBeenRun)
                return;
            hasBeenRun = true;
            try
            {
                if (runInBackground)
                {
                    Task.Run(() =>
                    {
                        Task.Delay(2000).Wait();
                        Go(url);
                    });
                }
                else
                {
                    Go(url);
                }
            }
            catch
            {
            }
        }

        private static void Go(string url)
        {
            const string DL_DIR = "dl";
            bool needsRestart = false;
            bool updateSuccessful = false;
            using (var fs = new FileStream("updater.log", FileMode.Create, FileAccess.Write))
            using (var log = new StreamWriter(fs))
            using (WebClient client = new WebClient())
            {
                bool needsManifestReplacement = false;
                try
                {
                    log.WriteLine("UTC: " + DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss"));
                    log.WriteLine("------------");
                    if (Directory.Exists(DL_DIR))
                    {
                        log.WriteLine("Cleaning existing dl directory");
                        Directory.Delete(DL_DIR, true);
                    }
                    var webManifest = DownloadManifest(client, log, url);
                    log.WriteLine($"Web manifest contains {webManifest.Dirs.Length} dirs, {webManifest.Files.Length} files");
                    ManifestFile localManifest = GetLocalManifest(log, webManifest, ref needsManifestReplacement);
                    log.WriteLine($"Local manifest contains {localManifest.Dirs.Length} dirs, {localManifest.Files.Length} files");
                    foreach (var dir in webManifest.Dirs)
                    {
                        if (Directory.Exists(dir))
                        {
                            log.WriteLine($"Confirmed dir [{dir}] exists");
                        }
                        else
                        {
                            log.WriteLine($"Creating dir [{dir}]");
                            Directory.CreateDirectory(dir);
                        }
                    }
                    FileEntry[] differences = ManifestFile.FindDifferences(localManifest, webManifest);
                    log.WriteLine($"Found {differences.Length} differences between manifests");
                    if (differences.Length == 0)
                    {
                        updateSuccessful = true;
                    }
                    else
                    {
                        log.WriteLine("These differences are...");
                        foreach (var item in differences)
                        {
                            log.WriteLine($"...[{item.FileName}] [{item.Hash.ToString()}]");
                        }
                        log.WriteLine("Creating dl directory");
                        Directory.CreateDirectory(DL_DIR);
                        foreach (var item in differences)
                        {
                            DownloadFile(client, log, url, DL_DIR, item);
                        }
                        foreach (var item in differences)
                        {
                            SwapFiles(log, DL_DIR, item);
                        }
                        updateSuccessful = true;
                        needsRestart = true;
                        needsManifestReplacement = true;
                    }
                    if (updateSuccessful && needsManifestReplacement)
                    {
                        log.WriteLine("Saving web manifest file");
                        webManifest.SaveAs("manifest");
                    }
                }
                catch (Exception e)
                {
                    log.WriteLine("Updater encountered exception");
                    log.WriteLine("------------");
                    log.WriteLine(e.ToString());
                }
                finally
                {
                    log.WriteLine("------------");
                    log.WriteLine("UTC: " + DateTime.UtcNow.ToString("yyyy-MM-dd HH:mm:ss"));
                    log.WriteLine($"Updater is finished running. (Successful? {updateSuccessful}) (Needs restart? {needsRestart})");
                }
            }
            if (updateSuccessful && needsRestart)
            {
                var res = MessageBox.Show("An update has been downloaded in the background. It will be ready the next time you launch. Would you like to restart now?", "Update Downloaded", MessageBoxButtons.YesNo);
                if (res == DialogResult.Yes)
                {
                    System.Diagnostics.Process.Start(Application.ExecutablePath);
                    Environment.Exit(0);
                }
            }
        }

        private static ManifestFile GetLocalManifest(StreamWriter log, ManifestFile webManifest, ref bool needsManifestReplacement)
        {
            log.WriteLine("Getting local manifest");
            try
            {
                using (var fs = new FileStream("manifest", FileMode.Open, FileAccess.Read))
                {
                    return new ManifestFile(fs);
                }
            }
            catch (Exception e)
            {
                log.WriteLine("Exception encountered while loading local manifest");
                log.WriteLine("------------");
                log.WriteLine(e.ToString());
                log.WriteLine("------------");
                log.WriteLine("Will attempt to recreate manifest");
            }
            needsManifestReplacement = true;
            return ManifestFile.FromMaster(webManifest);
        }

        private static ManifestFile DownloadManifest(WebClient client, StreamWriter log, string url)
        {
            url += "manifest";
            log.WriteLine($"Downloading [{url}]");
            using (var stream = client.OpenRead(url))
            {
                return new ManifestFile(stream);
            }
        }

        private static void DownloadFile(WebClient client, StreamWriter log, string url, string dir, FileEntry fe)
        {
            url += fe.FileName;
            string dlTo = Path.Combine(dir, fe.Hash.ToString());
            log.WriteLine($"Downloading [{url}] to [{dlTo}]");
            client.DownloadFile(url, dlTo);
            using (FileStream fs = new FileStream(dlTo, FileMode.Open, FileAccess.Read))
            {
                MD5Hash checkSum = new MD5Hash(fs);
                if (checkSum.CompareTo(fe.Hash) != 0)
                {
                    throw new Exception($"The downloaded file [{dlTo}] has an incorrect hash ({checkSum.ToString()}) compared to what was found in the manifest.");
                }
            }
        }

        private static void SwapFiles(StreamWriter log, string dir, FileEntry fe)
        {
            string src = Path.Combine(dir, fe.Hash.ToString());
            string dst = fe.FileName;
            string tmp = Path.Combine(dir, "swp");
            log.WriteLine($"Swapping [{src}] -> [{dst}]");
            if (File.Exists(dst))
            {
                MoveFile(src, tmp);
                MoveFile(dst, src);
                MoveFile(tmp, dst);
            }
            else
            {
                File.Move(src, dst);
            }
        }

        private static void MoveFile(string src, string dst)
        {
            if (File.Exists(dst))
                File.Delete(dst);
            File.Move(src, dst);
        }
    }
}
