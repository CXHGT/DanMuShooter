#include "SourceUtils.h"

namespace danMuGame::SourceUtils
{
    void SplitQPixmap(QPixmap &qPixmap, std::vector<QPixmap> &pixmapVec, int pixmapWidth, int pixmapHeight, int widthCount, int heightCount, int maxCount)
    {
        int count = 0;
        for (int y = 0; y < heightCount; ++y)
        {
            for (int x = 0; x < widthCount; ++x)
            {
                pixmapVec.push_back(qPixmap.copy(x * pixmapWidth, y * pixmapWidth, pixmapWidth, pixmapWidth));
                ++count;
                if (maxCount > 0 && count >= maxCount)
                {
                    return;
                }
            }
        }
    }
}