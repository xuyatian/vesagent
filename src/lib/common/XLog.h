#pragma once

#include <string>

namespace vagt
{
    namespace log
    {
        enum XLogLevel
        {
            XLogTrace,
            XLogDebug,
            XLogInfo,
            XLogWarn,
            XLogErr,
            XLogCritical,
            XLogOff,
        };

        void init(const std::string& path, int logFileSize = 1048576 * 5, int logFileNum = 10);
        void setLevel(XLogLevel level);
    }
}
