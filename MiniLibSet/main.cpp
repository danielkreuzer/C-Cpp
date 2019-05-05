#include "set.hpp"
#include <MLString.h>


int main() {
    {
        Set l;
        Set l2;
        ML::String s[]{"Item4Set1", "Item2Set1", "Item3Set1", "Item1Set1", "Item5Set1"};
        ML::String s2[]{"Item1Set1", "Item2Set2", "Item3Set2", "Item4Set2", "Item5Set2"};
        for(std::size_t i{0}; i < (sizeof(s)/sizeof(s[0]) -1); i++){
            l.Add(s+i);
        }
        for(std::size_t i{0}; i < (sizeof(s2)/sizeof(s2[0]) -1); i++){
            l2.Add(s2+i);
        }
        std::cout << l << std::endl;
        for(auto & e : s) {
            std::cout << e << " in l: " << std::boolalpha << l.Contains(&e) << std::endl;
        }
        ML::String s6{"Item1Set1"};
        std::cout << s6 << " in s6: " << std::boolalpha << l.Contains(&s6) << std::endl;
        std::cout << "Try to add s6 with value \"s1\" again" << std::endl;
        l.Add(&s6);
        std::cout << l << ", expected: same list as in beginning"<< std::endl;

        // Test mathematical set operations
        std::cout << std::endl;
        std::cout << "Test difference" << std::endl;
        std::cout << l << " \\ " << l2 << std::endl;
        Set * sP = l.Difference(&l2);
        std::cout << "Difference: " << *sP << std::endl << std::endl;

        std::cout << "Test intersection" << std::endl;
        std::cout << l << " /\\ " << l2 << std::endl;
        Set * sP1 = l.Intersection(&l2);
        std::cout << "Intersection: " << *sP1 << std::endl << std::endl;

        std::cout << "Test union" << std::endl;
        std::cout << l << " \\/ " << l2 << std::endl;
        Set * sP2 = l.Union(&l2);
        std::cout << "Union: " << *sP2 << std::endl << std::endl;

        std::cout << "Test symmetric difference" << std::endl;
        std::cout << l << " + " << l2 << std::endl;
        Set * sP3 = l.SymmetricDifference(&l2);
        std::cout << "Symmetric difference: " << *sP3 << std::endl << std::endl;

        std::cout << "Test includes" << std::endl;
        std::cout << l << " includes " << l2 << std::endl;
        std::cout << "Includes: " << std::boolalpha << l.Includes(&l2) << std::endl << std::endl;

        std::cout << "Test includes" << std::endl;
        std::cout << l << " includes " << l << std::endl;
        std::cout << "Includes: " << std::boolalpha << l.Includes(&l) << std::endl << std::endl;

        std::cout << "Test merge in first set" << std::endl;
        std::cout << l << " \\/ " << l2 << std::endl;
        l.Merge(&l2);
        std::cout << "Merged first List: " << l << std::endl << std::endl;

        //delete all
        for(auto & e : s){
            const auto ptr{l.Remove(&e)};
            if(ptr) std::cout << "removed: " << * ptr << " remaining " << l << std::endl;
            else std::cout << "tried to remove: " << e << " but is not in list" << std::endl;
        }
        for(auto & e : s2){
            const auto ptr{l.Remove(&e)};
            if(ptr) std::cout << "removed: " << * ptr << " remaining " << l << std::endl;
            else std::cout << "tried to remove: " << e << " but is not in list" << std::endl;
        }
        delete sP1;
        delete sP2;
        delete sP3;
        delete sP;

    } //destructor to see real writeMetaInfo

    WriteMetaInfo();
}