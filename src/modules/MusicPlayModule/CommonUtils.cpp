#include "CommonUtils.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdio>
using namespace std;

namespace CommonUtils
{

    QString formatNum(int num)
    {
        if (num >= 100)
        {
            return QString::number(num);
        }
        stringstream s;
        s << setw(2) << setfill('0') << num;
        string ss;
        s >> ss;
        return QString::fromStdString(ss);
    }

    QString formatDuration(int duration)
    {
        char buf[7];
        int d = duration / 1000;
        int min = d / 60;
        int sec = d % 60;

        sprintf(buf, "%02d:%02d", min, sec);
        return QString(buf);
    }
}
