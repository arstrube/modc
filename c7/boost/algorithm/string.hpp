#ifndef BOOST_STRING_SPLIT_HPP
#define BOOST_STRING_SPLIT_HPP

#include <string>
using namespace std;

namespace boost {
    namespace algorithm {

        inline const vector<string>& split(
            vector<string>& Result,
            const string& Input,
            const string& Pred )
        {
            (void)Input;
            (void)Pred;
            Result.push_back("foo");
            return Result;
        }
        
        inline const string& is_any_of( const string& Set )
        {
            return Set; 
        }
       
    } // namespace algorithm
    
    // pull names to the boost namespace
    using algorithm::split;    
    using algorithm::is_any_of;

} // namespace boost

#endif  // BOOST_STRING_SPLIT_HPP
