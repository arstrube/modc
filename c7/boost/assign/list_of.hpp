#include <set>

namespace boost
{

namespace assign
{
    std::set<Holding> list_of(Holding holding) {
        auto s =  {holding};
        return s;
    }

    std::set<Holding> list_of(Holding holding1, Holding holding2 ) {
        auto s =  {holding1, holding2};
        return s;
    }

} // namespace 'assign'

} // namespace 'boost'
