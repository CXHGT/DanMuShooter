#ifndef __SOURCE_UTILS_H__
#define __SOURCE_UTILS_H__

#include <vector>
#include <QPixmap>

namespace danMuGame::SourceUtils
{
    void SplitQPixmap(QPixmap &qPixmap, std::vector<QPixmap> &pixmapVec, int pixmapWidth, int pixmapHeight, int widthCount, int heightCount, int maxCount = -1);
}
#endif