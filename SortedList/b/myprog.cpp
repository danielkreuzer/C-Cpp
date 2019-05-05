#include "sorted_list.hpp"

using std::cout;
using std::endl;
using std::cerr;

bool isFive(int x) {
	return x == 5;
}

class isSmaller {
	public:
	 inline bool operator() (const int& x, const int& y){
        return (x < y);
    }
};


int main() {
	cout << "----------Test implementaion----------" << endl;
	swe4::sorted_list<int> test1;
	cout << "* Test if test1 empty" << endl;
	cout << std::boolalpha << test1.empty() << endl;
	test1.insert(6);
	test1.insert(5);
	test1.insert(5);
	test1.insert(3);
	cout << "* Test insert & print test1" << endl;
	test1.print(cout);
	cout << "* Test if test1 empty" << endl;
	cout << std::boolalpha << test1.empty() << endl;
	cout << "* Test if 3 in test1" << endl;
	cout << std::boolalpha << test1.find(3) <<endl;
	cout << "* Test if 10 in test1" << endl;
	cout << std::boolalpha << test1.find(10) << endl;
	cout << "* Test number of nodes in test1" << endl;
	cout << test1.size() << endl;
	cout << "* Test show front in test1" << endl;
	cout << test1.front() << endl;
	cout << "* Test show back in test1" << endl;
	cout << test1.back() << endl;
	cout << "* Test pop front and back and show test1 afterwards" << endl;
	test1.pop_back();
	test1.pop_front();
	test1.print(cout);
	test1.insert(6);
	test1.insert(5);
	test1.insert(3);
	cout << "* Test insert & print test1" << endl;
	test1.print(cout);
	cout << "* Test copy test1 in test2" << endl;
	swe4::sorted_list<int> test2{test1};
	test2.print(cout);
	cout << "* Test copy test1 in test3 with assign operator" << endl;
	swe4::sorted_list<int> test3 = test1;
	test3.print(cout);
	cout << "* Test earase 5 in test1" << endl;
	cout << std::boolalpha << test1.erase(5) << endl;
	test1.print(cout);
	cout << "* Test clear test1" << endl;
	test1.clear();
	test1.print(cout);
	cout << "* Test if test1 empty" << endl;
	cout << std::boolalpha << test1.empty() << endl;
	cout << "* Test if test2 empty (Should not be empty after deepcopy)" << endl;
	cout << std::boolalpha << test2.empty() << endl;
	cout << "* Test if test3 empty (Should not be empty after deepcopy)" << endl;
	cout << std::boolalpha << test3.empty() << endl;
	swe4::sorted_list<int, std::greater_equal<int>> test4;
	test4.insert(6);
	test4.insert(5);
	test4.insert(5);
	test4.insert(3);
	cout << "* Test insert & print test4 with custom compare function" << endl;
	test4.print(cout);

	

	cout << "----------Test iterator----------" << endl;
	cout << "* Test for with iterators test4" << endl;
	swe4::sorted_list<int, std::greater_equal<int>>::sorted_list_iterator it1;
	for(it1 = test4.begin(); it1 != test4.end(); it1++) {
		cout << *it1 << endl;
	}
	cout << "* Test range erase, erase from beginn to last-1 test4" << endl;
	test4.erase(test4.begin(), --test4.end());
	test4.print(cout);

	cout << "* Test remove_if, remove all 5 in test3" << endl;
	test3.erase_if([](int x){return x == 5;});
	test3.print(cout);

	cout << "* Test copy test3 in test5" << endl;
	swe4::sorted_list<int> test5;
	test3.print(cout);
	copy(test3.begin(), test3.end(), std::inserter(test5, test5.begin()));
	test5.print(cout);
	cout << "* Test reverse test3" << endl;
	std::reverse(test3.begin(), test3.end());
	test3.print(cout);

	cout << "* Test reverse output with rbegin, rend" << endl;
	cout << "Origin: ";
	test5.print(cout);
	swe4::sorted_list<int>::reverse_iterator it2;
	for(it2 = test5.rbegin(); it2 != test5.rend(); it2++) {
		cout << *it2 << endl;
	}

	cout << "* Test remove_if if value = 5" << endl;
	cout << "Origin: ";
	test2.print(cout);
	std::remove_if(test2.begin(), test2.end(), [](int x){return x == 5;});
	test2.print(cout);

	cout << "* Test transform test5 every 6 in 5 in test6" << endl;
	swe4::sorted_list<int> test6;
	cout << "Origin: ";
	test5.print(cout);
	std::transform(test5.begin(), test5.end(), std::inserter(test6, test6.begin()), [](int x)-> int{if(x==6){return 5;}else{return x;}});
	test6.print(cout);

	cout << "* Test insert_iterator test5 to test6 by copy" << endl;
	test6.insert(89);
	cout << "Origin1: ";
	test5.print(cout);
	cout << "Origin2: ";
	test6.print(cout);
	std::copy(test5.begin(), test5.end(), std::inserter<swe4::sorted_list<int>>(test6, std::next(test6.begin())));
	test6.print(cout);

	cout << "* Test with custom compare test21" << endl;
	swe4::sorted_list<int, isSmaller> test21;
	for(size_t i = 0; i < 4; i++) {
		test21.insert(std::rand());
	}
	test21.print(cout);

	cout << "* Test insert_iterator test21 to test22 by copy" << endl;
	swe4::sorted_list<int, isSmaller> test22;
	for(size_t i = 0; i < 3; i++) {
		test22.insert(std::rand());
	}
	cout << "Origin1: ";
	test21.print(cout);
	cout << "Origin2: ";
	test22.print(cout);
	std::copy(test21.begin(), test21.end(), std::inserter<swe4::sorted_list<int, isSmaller>>(test22, std::next(test22.begin())));
	test22.print(cout);
	return 0;
}
//make clean && make depend && make && ./myprog.exe > output.txt