namespace osuReplayEditor.Maintenance
{
    class ResourceLocker
    {
        private bool used = false;
        private readonly object lockette = new object();

        public bool Grab()
        {
            lock (lockette)
            {
                if (used)
                    return false;
                used = true;
                return true;
            }
        }

        public void Release()
        {
            lock (lockette)
            {
                used = false;
            }
        }
    }
}
