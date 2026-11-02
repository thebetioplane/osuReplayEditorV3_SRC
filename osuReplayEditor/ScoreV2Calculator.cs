using System;
using System.Collections.Generic;

namespace osuReplayEditor
{
    internal static class ScoreV2Calculator
    {
        private const uint ModNoFail = 1;
        private const uint ModEasy = 2;
        private const uint ModHidden = 8;
        private const uint ModHardRock = 16;
        private const uint ModDoubleTime = 64;
        private const uint ModHalfTime = 256;
        private const uint ModNightcore = 512;
        private const uint ModFlashlight = 1024;
        private const uint ModRelax = 128;
        private const uint ModSpunOut = 4096;
        private const uint ModAutopilot = 8192;
        private const uint ModRandom = 2097152;

        public static long EstimateScore(IEnumerable<int> objectScores, uint mods)
        {
            if (objectScores == null)
                throw new ArgumentNullException("objectScores");

            var scores = new List<int>();
            foreach (int score in objectScores)
            {
                if (score != 0 && score != 50 && score != 100 && score != 300)
                    throw new ArgumentOutOfRangeException("objectScores", "Hit scores must be 0, 50, 100, or 300.");
                scores.Add(score);
            }

            int maxCombo = scores.Count;
            if (maxCombo == 0)
                return 0;

            double maxBaseScore = maxCombo * 300.0;
            double comboPortionMax = 0;
            for (int combo = 1; combo <= maxCombo; ++combo)
                comboPortionMax += 300 * (1 + combo / 10.0);

            double currentBaseScore = 0;
            double comboPortion = 0;
            int currentCombo = 0;
            int currentHits = 0;
            foreach (int score in scores)
            {
                ++currentHits;
                currentBaseScore += score;
                if (score == 0)
                {
                    currentCombo = 0;
                    continue;
                }

                ++currentCombo;
                comboPortion += score * (1 + currentCombo / 10.0);
            }

            double accuracy = currentBaseScore / maxBaseScore;
            double scoreV2 = (700000 * comboPortion / comboPortionMax +
                              300000 * Math.Pow(accuracy, 10) * currentHits / maxCombo) *
                             GetModMultiplier(mods);
            return (long)Math.Round(scoreV2);
        }

        private static double GetModMultiplier(uint mods)
        {
            double multiplier = 1.0;
            if ((mods & ModNoFail) != 0) multiplier *= 0.5;
            if ((mods & ModEasy) != 0) multiplier *= 0.5;
            if ((mods & ModHidden) != 0) multiplier *= 1.06;
            if ((mods & ModHardRock) != 0) multiplier *= 1.09;
            if ((mods & (ModDoubleTime | ModNightcore)) != 0) multiplier *= 1.12;
            else if ((mods & ModHalfTime) != 0) multiplier *= 0.3;
            if ((mods & ModFlashlight) != 0) multiplier *= 1.12;
            if ((mods & ModRelax) != 0) multiplier *= 0.1;
            if ((mods & ModSpunOut) != 0) multiplier *= 0.95;
            if ((mods & ModAutopilot) != 0) multiplier *= 0.1;
            if ((mods & ModRandom) != 0) multiplier *= 0.7;
            return multiplier;
        }
    }
}
