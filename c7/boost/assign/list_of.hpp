#ifndef BOOST_ASSIGN_LIST_OF_HPP
#define BOOST_ASSIGN_LIST_OF_HPP

#include <set>

namespace boost
{
namespace assign_detail
{
    template<class T>
    class generic_list : public std::set<T>
    {
    public:
        generic_list& operator()(const T& u) {
            this->insert(u);
            return *this;
        }
    };
}
namespace assign
{
    template<class T>
    inline assign_detail::generic_list<T> list_of(const T& t) {
        return assign_detail::generic_list<T>()(t);
    }
} } // namespaces

#endif // BOOST_ASSIGN_LIST_OF_HPP
