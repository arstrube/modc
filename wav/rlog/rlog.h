#pragma once

namespace rlog
{
    class RLogChannel;

    #define DEF_CHANNEL(string, level) (RLogChannel*)0

    inline void rLog(RLogChannel*, const char *, ...) {}

} // namespace rlog
