#ifndef COMMONUTIL_H
#define COMMONUTIL_H
#include <QString>

namespace CommonUtils
{
    // 格式化数字，前面补0
    QString formatNum(int num);
    // 格式化歌曲时长
    QString formatDuration(int duration);
};
#endif // COMMONUTIL_H
