#pragma once

namespace rlog
{
    class RLogNode;

    class StdioNode {
    public:
        StdioNode(int fileno) : fileno_(fileno){}
        void subscribeTo(RLogNode* /*channel*/) {}
    private:
        int fileno_;
    };
} // namespace rlog
