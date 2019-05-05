#include "bag.hpp"
#include <MLString.h>

int main() {
    {
        Bag l;
        ML::String s[]{"s1","s2", "s3", "s4", "s5"};
        for(std::size_t i{0}; i < (sizeof(s)/sizeof(s[0]) -1); i++){
            l.Add(s+i);
        }
        for(std::size_t i{0}; i < (sizeof(s)/sizeof(s[0]) -1); i++){
            l.Add(s+i);
        }
        std::cout << l << std::endl;
        l.PrintWithAmount(std::cout);
        std::cout << std::endl;
        for(auto & e : s) {
            std::cout << e << " in l: " << std::boolalpha << l.Contains(&e) << std::endl;
        }
        ML::String s6{"s1"};
        std::cout << s6 << " in s6: " << std::boolalpha << l.Contains(&s6) << std::endl;
        for(auto & e : s){
            const auto ptr{l.Remove(&e)};
            if(ptr) {
                std::cout << "removed: " << *ptr << " remaining " << l << std::endl;
                l.PrintWithAmount(std::cout);
            } 
            else std::cout << "tried to remove: " << e << " but is not in list" << std::endl;
        }

        for(auto & e : s){
            const auto ptr{l.Remove(&e)};
            if(ptr) {
                std::cout << "removed: " << *ptr << " remaining " << l << std::endl;
                l.PrintWithAmount(std::cout);
            } 
            else std::cout << "tried to remove: " << e << " but is not in list" << std::endl;
        }
    } //destructor to see real writeMetaInfo

    WriteMetaInfo();
}