#pragma once

namespace rlog
{
    class RLogNode;
    class StdioNode {
    public:
        StdioNode(int) {}
        void subscribeTo(RLogNode*) {}
    };
} // namespace rlog
