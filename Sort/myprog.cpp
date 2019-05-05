#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <functional>
#include <chrono>
#include <algorithm>
#include <fstream>

using std::cout;
using std::cerr;
using std::endl;

namespace swe4 {
	template <typename RAIter>
	void shell_sort(RAIter first, RAIter last) {
		size_t n = std::distance(first, last);

		for(size_t gap = n/2; gap > 0; gap /= 2) {
			for(size_t i = gap; i < n; i += 1) {
				auto act_value = first[i];
				size_t j;
				for(j = i; j >= gap && first[j - gap] > act_value; j -= gap) {
					first[j] = first[j - gap];
				}
				first[j] = act_value;
			}
		}
	}

	template <typename RAIter, typename COMPAREfkt>
	void shell_sort(RAIter first, RAIter last, COMPAREfkt compare) {
		size_t n = 0;
		for(auto it = first; it != last; it++) {
			n++;
		}

		for(size_t gap = n/2; gap > 0; gap /= 2) {
			for(size_t i = gap; i < n; i += 1) {
				auto act_value = first[i];
				size_t j;
				for(j = i; j >= gap && compare(first[j - gap], act_value); j -= gap) {
					first[j] = first[j - gap];
				}
				first[j] = act_value;
			}
		}
	}

	template <typename RAIter>
	RAIter partition(RAIter first, RAIter last) {
		RAIter i = first - 1;
		RAIter it= first;

		while(it < last) {
			if(*it <= *last) {
				i++;
				std::swap(*i, *it);
			}
			it++;
		}
		std::swap(*(i + 1), *last);
		return ++i;
	}

	template <typename RAIter>
	void quick_sort(RAIter first, RAIter last) {
		if (std::distance(first, last) > 1) {
			RAIter pi = partition<RAIter>(first, last-1);
			quick_sort(first, pi);
			quick_sort(pi, last);
		}
	}

	template <typename RAIter, typename COMPAREfkt>
	RAIter partition_new(RAIter first, RAIter last, COMPAREfkt compare) {
		RAIter i = first - 1;
		RAIter it= first;

		while(it < last) {
			if(compare(*it, *last)) {
				i++;
				std::swap(*i, *it);
			}
			it++;
		}
		std::swap(*(i + 1), *last);
		return ++i;
	}

	template <typename RAIter, typename COMPAREfkt>
	void quick_sort(RAIter first, RAIter last, COMPAREfkt compare) {
		if (std::distance(first, last) > 1) {
			RAIter pi = partition_new<RAIter>(first, last-1, compare);
			quick_sort(first, pi, compare);
			quick_sort(pi, last, compare);
		}
	}

	//For testing
	template <typename RAIter>
	void print_vector(RAIter first, RAIter last) {
		for(auto it = first; it != last; it++) {
			cout << *it << '\t';
		}
		cout << endl;
	}

	std::vector<int> generateTest(size_t size) {
		std::vector<int> output;
		for(size_t i = 0; i < size; i++) {
			output.push_back(std::rand());
		}
		return output;
	}
}

int main() {
	//Test implementation
	cout << "- - - - - TEST Shellsort - - - - -" << endl;
	std::vector<int> testVector1 = swe4::generateTest(21);
	
	cout << "Random vector: " << endl;
	swe4::print_vector<std::vector<int>::iterator>(testVector1.begin(), testVector1.end());
	cout << "Sorted vector: " << endl;
	swe4::shell_sort<std::vector<int>::iterator>(testVector1.begin(), testVector1.end());
	swe4::print_vector<std::vector<int>::iterator>(testVector1.begin(), testVector1.end());
	cout << endl;

	std::vector<std::string> testVector2;
	testVector2.push_back("LLL");
	testVector2.push_back("RRR");
	testVector2.push_back("CCC");
	testVector2.push_back("AAA");
	testVector2.push_back("BBB");

	cout << "Random vector: " << endl;
	swe4::print_vector<std::vector<std::string>::iterator>(testVector2.begin(), testVector2.end());
	cout << "Sorted vector: " << endl;
	swe4::shell_sort<std::vector<std::string>::iterator>(testVector2.begin(), testVector2.end(), std::less_equal<std::string>());
	swe4::print_vector<std::vector<std::string>::iterator>(testVector2.begin(), testVector2.end());

	cout << "- - - - - TEST Quicksort - - - - -" << endl;
	std::vector<int> testVector3= swe4::generateTest(21);
	
	cout << "Random vector: " << endl;
	swe4::print_vector<std::vector<int>::iterator>(testVector3.begin(), testVector3.end());
	cout << "Sorted vector: " << endl;
	swe4::quick_sort<std::vector<int>::iterator>(testVector3.begin(), testVector3.end());
	swe4::print_vector<std::vector<int>::iterator>(testVector3.begin(), testVector3.end());
	cout << endl;

	std::vector<std::string> testVector4;
	testVector4.push_back("LLL");
	testVector4.push_back("RRR");
	testVector4.push_back("CCC");
	testVector4.push_back("AAA");
	testVector4.push_back("BBB");

	cout << "Random vector: " << endl;
	swe4::print_vector<std::vector<std::string>::iterator>(testVector4.begin(), testVector4.end());
	cout << "Sorted vector: " << endl;
	swe4::quick_sort<std::vector<std::string>::iterator>(testVector4.begin(), testVector4.end(), std::less_equal<std::string>());
	swe4::print_vector<std::vector<std::string>::iterator>(testVector4.begin(), testVector4.end());

	cout << "- - - - - **TIME COMPARISION** - - - - -" << endl;

	//Init test vectors
	std::vector<int> testVector11 = swe4::generateTest(100);
	std::vector<int> testVector21 = swe4::generateTest(1000);
	std::vector<int> testVector41 = swe4::generateTest(10000);
	std::vector<int> testVector51 = swe4::generateTest(100000);
	std::vector<int> testVector61 = swe4::generateTest(1000000);

	std::vector<int> testVector12 = testVector11;
	std::vector<int> testVector13 = testVector11;
	std::vector<int> testVector14 = testVector11;
	std::vector<int> testVector15 = testVector11;

	std::vector<int> testVector22 = testVector21;
	std::vector<int> testVector23 = testVector21;
	std::vector<int> testVector24 = testVector21;
	std::vector<int> testVector25 = testVector21;

	std::vector<int> testVector42 = testVector41;
	std::vector<int> testVector43 = testVector41;
	std::vector<int> testVector44 = testVector41;
	std::vector<int> testVector45 = testVector41;

	std::vector<int> testVector52 = testVector51;
	std::vector<int> testVector53 = testVector51;
	std::vector<int> testVector54 = testVector51;
	std::vector<int> testVector55 = testVector51;

	std::vector<int> testVector62 = testVector61;
	std::vector<int> testVector63 = testVector61;
	std::vector<int> testVector64 = testVector61;
	std::vector<int> testVector65 = testVector61;

	//messure time
	std::chrono::high_resolution_clock::time_point t111 = std::chrono::high_resolution_clock::now();
	std::sort(testVector11.begin(), testVector11.end());
	std::chrono::high_resolution_clock::time_point t112 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t121 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector12.begin(), testVector12.end());
	std::chrono::high_resolution_clock::time_point t122 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t131 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector13.begin(), testVector13.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t132 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t141 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector14.begin(), testVector14.end());
	std::chrono::high_resolution_clock::time_point t142 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t151 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector15.begin(), testVector15.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t152 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t211 = std::chrono::high_resolution_clock::now();
	std::sort(testVector11.begin(), testVector11.end());
	std::chrono::high_resolution_clock::time_point t212 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t221 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector22.begin(), testVector22.end());
	std::chrono::high_resolution_clock::time_point t222 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t231 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector23.begin(), testVector23.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t232 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t241 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector24.begin(), testVector24.end());
	std::chrono::high_resolution_clock::time_point t242 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t251 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector25.begin(), testVector25.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t252 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t311 = std::chrono::high_resolution_clock::now();
	std::sort(testVector41.begin(), testVector41.end());
	std::chrono::high_resolution_clock::time_point t312 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t321 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector42.begin(), testVector42.end());
	std::chrono::high_resolution_clock::time_point t322 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t331 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector43.begin(), testVector43.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t332 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t341 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector44.begin(), testVector44.end());
	std::chrono::high_resolution_clock::time_point t342 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t351 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector45.begin(), testVector45.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t352 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t411 = std::chrono::high_resolution_clock::now();
	std::sort(testVector51.begin(), testVector51.end());
	std::chrono::high_resolution_clock::time_point t412 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t421 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector52.begin(), testVector52.end());
	std::chrono::high_resolution_clock::time_point t422 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t431 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector53.begin(), testVector53.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t432 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t441 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector54.begin(), testVector54.end());
	std::chrono::high_resolution_clock::time_point t442 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t451 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector55.begin(), testVector55.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t452 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t511 = std::chrono::high_resolution_clock::now();
	std::sort(testVector61.begin(), testVector61.end());
	std::chrono::high_resolution_clock::time_point t512 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t521 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector62.begin(), testVector62.end());
	std::chrono::high_resolution_clock::time_point t522 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t531 = std::chrono::high_resolution_clock::now();
	swe4::shell_sort<std::vector<int>::iterator>(testVector63.begin(), testVector63.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t532 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t541 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector64.begin(), testVector64.end());
	std::chrono::high_resolution_clock::time_point t542 = std::chrono::high_resolution_clock::now();

	std::chrono::high_resolution_clock::time_point t551 = std::chrono::high_resolution_clock::now();
	swe4::quick_sort<std::vector<int>::iterator>(testVector65.begin(), testVector65.end(), std::less_equal<int>());
	std::chrono::high_resolution_clock::time_point t552 = std::chrono::high_resolution_clock::now();

	//write result in file
	std::ofstream myfile;
	myfile.open("result.csv");

	myfile << ";std::sort;shellsort;shellsort compFkt;quicksort; quicksort compFkt\n";
	myfile << "100;" << std::chrono::duration_cast<std::chrono::duration<double>>(t112 - t111).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t122 - t121).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t132 - t131).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t142 - t141).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t152 - t151).count() << ";\n";
	myfile << "1000;" << std::chrono::duration_cast<std::chrono::duration<double>>(t212 - t211).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t222 - t221).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t232 - t231).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t242 - t241).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t252 - t251).count() << ";\n";
	myfile << "10000;" << std::chrono::duration_cast<std::chrono::duration<double>>(t312 - t311).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t322 - t321).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t332 - t331).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t342 - t341).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t352 - t351).count() << ";\n";
	myfile << "100000;" << std::chrono::duration_cast<std::chrono::duration<double>>(t412 - t411).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t422 - t421).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t432 - t431).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t442 - t441).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t452 - t451).count() << ";\n";
	myfile << "1000000;" << std::chrono::duration_cast<std::chrono::duration<double>>(t512 - t511).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t522 - t521).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t532 - t531).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t542 - t541).count() << ";" << std::chrono::duration_cast<std::chrono::duration<double>>(t552 - t551).count() << ";\n";

	myfile.close();


	cout << "Test completed, in file result.csv you can see the needed time for the test!" << endl;

	return 0;
}
//make clean && make depend && make && ./myprog.exe > output.txt