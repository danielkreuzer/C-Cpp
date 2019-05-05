#include "sorted_list.hpp"

using std::cout;
using std::endl;
using std::cerr;



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
	return 0;
}
//make clean && make depend && make && ./myprog.exe > output.txt