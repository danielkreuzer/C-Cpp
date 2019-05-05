#include <iostream>
#include <utility>
#include <vector>
#include <cassert>
#include <cmath>

using std::cout;
using std::cin;
using std::cerr;
using std::endl;

class vector_template {
	private:
		std::vector<double> vec;
	
	public:
		//constructors
		vector_template() = delete;
		vector_template(double input, size_t size) {
			vec.assign(size, input);
		}
		vector_template(double * input, size_t size) {
			vec.assign(input, input + size);
		}
		//copy constructor
		vector_template(const vector_template & other){
			for(double elem : other.vec){
				vec.push_back(elem);
			}
		}
		//assignment operator
		vector_template & operator= (const vector_template & other) {
			auto tmp{other};
			vec = tmp.vec;
			return * this;
		}
		//arithmetic operations
		vector_template operator+(const vector_template& other){
			if(vec.size() != other.vec.size()){
				cerr << "ERROR: failure in operator +" << endl;
			}
			assert(vec.size() == other.vec.size());
			vector_template temp{other};
			size_t counter{0};
			for(double elem : vec) {
				temp.vec[counter] = temp.vec[counter] + elem;
				counter++;
			}
			return temp;
		}
		vector_template operator-(const vector_template& other){
			if(vec.size() != other.vec.size()){
				cerr << "ERROR: failure in operator -" << endl;
			}
			assert(vec.size() == other.vec.size());
			vector_template temp{other};
			size_t counter{0};
			for (double elem : vec)
			{
				temp.vec[counter] = temp.vec[counter] - elem;
				counter++;
			}
			return temp;
		}
		vector_template operator*(const double& skalar){
			vector_template temp(1, vec.size());
			size_t counter{0};
			for(double elem : vec) {
				temp.vec[counter] = elem * skalar;
				counter++;
			}
			return temp;
		}
		vector_template operator*(const vector_template& other){
			if(vec.size() != other.vec.size()){
				cerr << "ERROR: failure in operator *" << endl;
			}
			assert(vec.size() == other.vec.size());
			vector_template temp{other};
			size_t counter{0};
			for(double elem : vec){
				temp.vec[counter] = elem * temp.vec[counter];
				counter++;
			}
			return temp;
		}
		double amount(){
			double solution{0};
			for(double elem : vec){
				solution = solution + pow(elem, 2);
			}
			solution = sqrt(solution);
			return solution;
		}
		bool operator==(const vector_template& other){
			bool solution{true};
			if(vec.size() != other.vec.size()){
				solution = false;
			} else {
				size_t counter{0};
				for(double elem : vec){
					if(other.vec[counter] != elem){
						solution = false;
					}
					counter++;
				}
			}
			return solution;
		}
		friend std::ostream & operator<<(std::ostream & os, const vector_template & self) {
			os << "[ ";
			for (double elem : self.vec)
			{
				os << elem << " ";
			}
			os <<']' << endl;
			return os;
		}
		friend std::istream & operator>>(std::istream & is, vector_template & self) {
			double x;
			while (is >> x){ //while x is a double
				self.vec.push_back(x);
			}
			return is;
		}
};

class row_vector : public vector_template {
		using vector_template::vector_template;
		row_vector(const row_vector & other) : vector_template(other) {

		}
		row_vector & operator= (const row_vector & other) {
			return * this;
		}

};

class column_vector {

};

class matrix {
	matrix() {
		//TODO init als Einheitsmatrix
	}

	//TODO konstruktor mit fix wert
	//z.B. lauter 1 er

	//TODO koonstruktor für zweidimensionales feld
};

int main() {
	double arr[] {1, 2, 3};
	size_t arr_size = sizeof(arr) / sizeof(arr[0]);
	row_vector r1(arr, arr_size);
	row_vector r11(arr, arr_size);
	row_vector r2(1, 3);
	row_vector r3(1, 1);
	r2 = r2 + r1;
	r3 = r1 * 2;
	r11 = r1 * r1;
	cout << r1;
	cout << r11;
	cout << r2;
	cout << r3;
	cout << r3.amount() << endl;
	cout << std::boolalpha << (r1 == r1) << endl;
	row_vector r_in(1, 1);
	cin >> r_in;
	cout << r_in;
	return 0;
}
//make clean && make depend && make && ./myprog.exe