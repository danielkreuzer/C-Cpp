#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <map>
#include <sstream>
#include <iomanip>

using std::cerr;
using std::cout;
using std::cin;
using std::endl;

//normalize input
inline bool is_punct(char c) {
	return std::ispunct(c);
}
inline bool is_digit(char c) {
	return std::isdigit(c);
}
inline char to_lower(char c) {
	return static_cast<char>(std::tolower(c));
}
std::string normalize(std::string word) {
	//remove not needed char
	std::string::iterator new_end = std::remove_if(begin(word), end(word), is_punct); //auto = std::string::iterator
	new_end = std::remove_if(begin(word), new_end, is_digit);
	//shorten word
	word.erase(new_end, end(word));
	//lower word
	std::transform(begin(word), end(word), begin(word), to_lower);
	return word;
}

class wordset {
	private:
		//wordset
		std::multiset<std::string> words;

		std::multiset<std::string> union_multiset(std::multiset<std::string> &other) {
			std::multiset<std::string> sunion;
			sunion = words;
			sunion.insert(other.begin(), other.end());
			return sunion;
		}
	public:
		void insert(std::ifstream &in) {
			std::transform(std::istream_iterator<std::string> (in), std::istream_iterator<std::string>(), std::inserter(words, begin(words)), normalize);
		}
		
		void compare(wordset &other) {
			wordset cunion;
			wordset cintersect;

			//For multisets, std::set_union works wrong!
			//std::set_union(words.begin(), words.end(), other.words.begin(), other.words.end(), inserter(cunion.words, cunion.words.begin()));
			cunion.words = union_multiset(other.words);
			std::set_intersection(words.begin(), words.end(), other.words.begin(), other.words.end(), inserter(cintersect.words, cintersect.words.begin()));

			cout << "------ COMPARE REPORT ------" << endl;
			cout << "- Nr. of words in file1: " << endl << "- - " << words.size() << endl;
			cout << "- Nr. of words in file2: " << endl << "- - " << other.words.size() << endl;
			cout << "- Sum of words in both files: " << endl << "- - " << cunion.words.size() << endl;
			cout << "- Nr. of equal words: " << endl << "- - " << cintersect.words.size() << endl;
			cout << "- Percentage of equal words to sum of words:" << endl << "- - - "<< (((double(cintersect.words.size()) * 100.0) / double(cunion.words.size()))*2.0) << "%" << " = equality key figure" << endl;

			/* //DEBUGING
			cout << endl << endl <<"Union" << endl << endl;
			cunion.print_set();
			cout << endl << endl <<"Intersect" << endl << endl;
			cintersect.print_set();
			*/
		}

		//for testing
		void print_set() {
			std::copy(begin(words), end(words), std::ostream_iterator<std::string>(cout, "\n"));
		}
		
};


int main(int argc, char * argv[]) {

	//Check input
	if(argc < 2) {
		cerr << "usage: " << argv[0] << " <filename1> <filename2>" << endl;
		return 1;
	}

	std::ifstream fin1(argv[1]);
	std::ifstream fin2(argv[2]);
	bool check = false;
	if (!fin1) {
		cerr << "file <" << argv[1] << "> does not exist." << endl;
		check = true;
	}
	if (!fin2) {
		cerr << "file <" << argv[2] << "> does not exist." << endl;
		check = true;
	}
	if (check) {
		return 1;
	}

	//Generate wordset by using std::set
	wordset file1;
	wordset file2;

	file1.insert(fin1);
	file2.insert(fin2);

	//Compare function compares in both directions

	/* //DEBUGING
	cout << endl << "Text1: " << endl;
	file1.print_set();
	cout << endl << "Text2: " << endl;
	file2.print_set();
	cout << endl << endl;
	*/
	cout << "*Compare file1 with file2" << endl;
	file1.compare(file2);
	cout << endl << "*Compare file2 with file1" << endl;
	file2.compare(file1);
	cout << endl << "*Compare file2 with file2" << endl;
	file2.compare(file2);
	cout << endl << "*Compare file1 with file1" << endl;
	file1.compare(file1);


	return 0;
}
//make clean && make depend && make && ./myprog.exe
//make clean && make depend && make && ./myprog.exe test1.txt test2.txt
//make clean && make depend && make && ./myprog.exe test1.txt test2.txt > output.txt