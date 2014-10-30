#ifndef BOOST_STRING_SPLIT_HPP
#define BOOST_STRING_SPLIT_HPP

#include <string>
#include <sstream>
#include <vector>

//using namespace std;

namespace boost {
    namespace algorithm {

        inline const std::vector<std::string>& split(
            std::vector<std::string>& Result,
            const std::string& Input,
            const std::string& Pred )
        {
            (void)Pred;
            const char delim = ':';
            std::stringstream ss(Input);
            std::string item;
            while (std::getline(ss, item, delim)) {
                Result.push_back(item);
            }
            return Result;
        }
        
        inline const std::string& is_any_of( const std::string& Set )
        {
            return Set; 
        }
       
    } // namespace algorithm
    
    // pull names to the boost namespace
    using algorithm::split;    
    using algorithm::is_any_of;

} // namespace boost

#endif  // BOOST_STRING_SPLIT_HPP
