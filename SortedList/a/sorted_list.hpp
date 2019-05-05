#ifndef sorted_list_hpp
#define sorted_list_hpp

#include <functional>
#include <iostream>
#include <ios>

using std::cerr;
using std::cout;
using std::endl;

namespace swe4 {
    template <typename T, typename C = std::less<T>>
    class sorted_list {
        private:
            struct Node {
                T data;
                Node *next;
                Node *prev;
            };
            Node *list;

            Node * newNode(const T &value) {
                Node *newNode = new Node();
                newNode->data = value;
                newNode->next = nullptr;
                newNode->prev = nullptr;
                return newNode;
            }

        public:
            sorted_list() {
                list = nullptr;
            }
            virtual ~sorted_list() {
                clear();
            }
            sorted_list(const sorted_list &other) {
                list = nullptr;
                Node * otherNode = other.list;
                Node * prevPtr = nullptr;
                while (otherNode != nullptr) {
                    Node * new_node = newNode(otherNode->data);
                    if(list == nullptr) {
                        list = new_node;
                        prevPtr = list;
                    } else {
                        new_node->prev = prevPtr;
                        prevPtr->next = new_node;
                        prevPtr = new_node;
                    }
                    otherNode = otherNode->next;
                }
            }
            sorted_list(sorted_list &&other) : list{std::move(other.list)} {

            }
            sorted_list& operator=(const sorted_list &other) {
                sorted_list tmp{other};
                std::swap(list, tmp.list);
                return *this;
            }
            sorted_list& operator=(sorted_list && other) {
                std::swap(list, other.list);
                return *this;
            }
            bool find(const T &value) const { // checks if value is in list
                Node * current = list;
                while (current != nullptr && current->data != value) {
                    current = current->next;
                }
                return current != nullptr;
            }
            void insert(const T &value) {     // inserts element into list
                if (!find(value)) {
                    C c = C();
                    if (list == nullptr) {
                        list = newNode(value);
                    } else if (!c(list->data, value)) {
                        Node * new_node = newNode(value);
                        new_node->next = list;
                        new_node->next->prev = new_node;
                        list = new_node;

                    } else {
                        Node * current = list;
                        while (current->next != nullptr && c(current->next->data, value)) {
                            current = current->next;
                        }
                        Node * new_node = newNode(value);
                        new_node->next = current->next;
                        if (current->next != nullptr) {
                            new_node->next->prev = new_node;
                        }
                        current->next = new_node;
                        new_node->prev = current;
                    }
                }
            }
            bool erase(const T &value) {     // removes element and returns true
                                            // if value was found in set1
                if(list == nullptr) {
                    return false;
                }
                if(list->data == value) {
                    Node * delete_current = list;
                    if (list->next != nullptr) {
                        list = list->next;
                        list->prev = nullptr;
                        delete delete_current;
                    } else {
                        list = nullptr;
                        delete delete_current;
                    }   
                    return true;
                }
                Node * current = list;
                while (current != nullptr && current->data != value) {
                    current = current->next;
                }
                if(current == nullptr) {
                    return false;
                }
                Node * delete_current = current;
                if(current->next != nullptr) {
                    if(current->prev != nullptr) {
                        current->next->prev = current->prev;
                    } else {
                        current->next->prev = nullptr;
                    }
                }

                if(current->prev != nullptr) {
                    if(current->next != nullptr) {
                        current->prev->next = current->next;
                    } else {
                        current->prev->next = nullptr;
                    }
                }

                delete delete_current;
                return true;
            }
            size_t size() const {            // returns the number of elements
                size_t output{0};
                Node * current = list;
                while (current != nullptr) {
                    current = current->next;
                    output++;
                }
                return output;
            }
            bool empty() const {             // checks if there are any elements
                return list == nullptr;
            }
            T &front() const {               // returns smallest element
                return list->data;
            }
            T &back() const {                // returns greatest element
                Node * back = list;               
                if(list != nullptr) {
                    while (back->next != nullptr) {
                        back = back->next;
                    }
                }
                return back->data;
            }
            void pop_front() {               // removes smallest element
                if(list != nullptr) {
                    Node * front = list;
                    list = front->next;
                    list->prev = nullptr;
                    delete front;
                } else {
                    cerr << "pop_front not possible" << endl;
                }   
            }
            void pop_back() {               // removes greatest element
                if(list != nullptr) {
                    Node * back = list;
                    while (back->next != nullptr) {
                        back = back->next;
                    }
                    if (back->prev != nullptr) {
                        back->prev->next = nullptr;
                    }

                    delete back;

                } else {
                    cerr << "pop_back not possible" << endl;
                }
            }
            void clear() {                   // empties list
                Node * current = list;
                list = nullptr;
                while (current != nullptr) {
                    Node * delete_current = current;
                    current = current->next;
                    delete delete_current;
                }
            }

            virtual void print(std::ostream& os) const {
                Node * current = list;
                while(current != nullptr) {
                    os << current->data << " ";
                    current = current->next;
                }
                os << endl;
            }
    };

}

#endif