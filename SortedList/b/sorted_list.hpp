#ifndef sorted_list_hpp
#define sorted_list_hpp

#include <functional>
#include <iostream>
#include <ios>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cstdlib>

using std::cerr;
using std::cout;
using std::endl;

namespace swe4 {
    template <typename T, typename C = std::less<T>>
    class sorted_list {
        private:
            friend class sorted_list_iterator;
            struct Node {
                T data;
                Node *next;
                Node *prev;
            };
            Node *list;
            Node *dummy;

            Node * newNode(const T &value) {
                Node *newNode = new Node();
                newNode->data = value;
                newNode->next = nullptr;
                newNode->prev = nullptr;
                return newNode;
            }

        public:
            class sorted_list_iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
                private:
                    friend class sorted_list;
                    Node * it_node;
                public:
                    sorted_list_iterator() : it_node{nullptr} {
                        
                    }
                    sorted_list_iterator(Node * input) : it_node{input} {

                    }
                    sorted_list_iterator & operator++() {
                        it_node = it_node->next;
                        return * this;
                    }
                    sorted_list_iterator operator++(int) {
                        sorted_list_iterator tmp(it_node);
                        it_node = it_node->next;
                        return tmp;
                    }   	
                    sorted_list_iterator & operator--() {
                        it_node = it_node->prev;
                        return * this;
                    }
                    sorted_list_iterator operator--(int) {
                        sorted_list_iterator tmp(it_node);
                        it_node = it_node->prev;
                        return tmp;
                    }
                    bool operator==(const sorted_list_iterator &b) const {
                        return it_node->data == b.it_node->data;
                    }
                    bool operator!=(const sorted_list_iterator &b) const {
                        return it_node->data != b.it_node->data;
                    }
                    T & operator*() {
                        return it_node->data;
                    }
                    T * operator->() {
                        return &it_node->data;
                    }
                    sorted_list_iterator &operator=(const std::pair<sorted_list_iterator, bool> &b) {
                        it_node = std::get<sorted_list_iterator>(b).it_node;
                        return *this;
                    }
            };
            typedef typename std::iterator_traits<sorted_list_iterator>::value_type value_type;
            typedef typename std::reverse_iterator<sorted_list_iterator> reverse_iterator;
            sorted_list()
            {
                dummy = new Node();
                dummy->next = dummy;
                dummy->prev = dummy;
                list = dummy;
            }
            virtual ~sorted_list() {
                Node * current = list;
                list = nullptr;
                while (current != nullptr) {
                    Node * delete_current = current;
                    current = current->next;
                    delete delete_current;
                }
            }
            sorted_list(const sorted_list &other) {
                dummy = new Node();
                dummy->next = dummy;
                dummy->prev = dummy;
                list = dummy;
                Node * otherNode = other.list;
                while(otherNode != other.dummy) {
                    insert(otherNode->data);
                    otherNode = otherNode->next;
                }
            }
            sorted_list(sorted_list &&other) : list{std::move(other.list)}, dummy{std::move(other.dummy)} {

            }
            sorted_list& operator=(const sorted_list &other) {
                sorted_list tmp{other};
                std::swap(list, tmp.list);
                std::swap(dummy, tmp.dummy);
                return *this;
            }
            sorted_list& operator=(sorted_list && other) {
                std::swap(list, other.list);
                std::swap(dummy, other.dummy);
                return *this;
            }
            bool find(const T &value) const { // checks if value is in list
                Node * current = list;
                while (current != dummy && current->data != value) {
                    current = current->next;
                }
                return current != dummy;
            }

            Node * find_node(const T &value) { // checks if value is in list
                Node * current = list;
                while (current != dummy && current->data != value) {
                    current = current->next;
                }
                return current;
            }

            bool erase(const T &value) {     // removes element and returns true
                                            // if value was found in set1
                if(list == dummy) {
                    return false;
                }
                if(list->data == value) {
                    Node * delete_current = list;
                    if (list->next != dummy) {
                        list = list->next;
                        list->prev = dummy;
                        dummy->next = list;
                        delete delete_current;
                    } else {
                        list = dummy;
                        dummy->next = dummy;
                        dummy->prev = dummy;
                        delete delete_current;
                    }   
                    return true;
                }
                Node * current = list;
                while (current != dummy && current->data != value) {
                    current = current->next;
                }
                if(current == dummy) {
                    return false;
                }
                Node * delete_current = current;
                current->next->prev = current->prev;
                current->prev->next = current->next;
                if(current->prev == dummy) {
                    list = current->next;
                }

                delete delete_current;
                return true;
            }
            size_t size() const {            // returns the number of elements
                size_t output{0};
                Node * current = list;
                while (current != dummy) {
                    current = current->next;
                    output++;
                }
                return output;
            }
            bool empty() const {             // checks if there are any elements
                return list == dummy;
            }
            T &front() const {               // returns smallest element
                return dummy->next->data;
            }
            T &back() const {
                return dummy->prev->data;         // returns greatest element

            }
            void pop_front() {               // removes smallest element
                if(list != dummy) {
                    Node * front = list;
                    list = front->next;
                    list->prev = dummy;
                    dummy->next = list;
                    delete front;
                } else {
                    cerr << "pop_front not possible" << endl;
                }   
            }
            void pop_back() {               // removes greatest element
                if(list != dummy) {
                    Node * back = dummy->prev;
                    dummy->prev = back->prev;
                    back->prev->next = dummy;
                    if(back->prev == dummy) {
                        list = back->next;
                    }
                    delete back;
                } else {
                    cerr << "pop_back not possible" << endl;
                }
            }
            void clear() {                   // empties list
                Node * current = list;
                list = dummy;
                dummy->next = dummy;
                dummy->prev = dummy;
                while (current != dummy) {
                    Node * delete_current = current;
                    current = current->next;
                    delete delete_current;
                }
            }

            virtual void print(std::ostream& os) const {
                Node * current = list;
                while(current != dummy) {
                    os << current->data << " ";
                    current = current->next;
                }
                os << endl;
            }

            typedef sorted_list_iterator iterator;
            iterator begin() {
                iterator tmp(list);
                return tmp;
            }  // iterator to the beginning of list
            iterator end() {
                iterator tmp(dummy);
                return tmp;
            }   // iterator to the end of list
            reverse_iterator rbegin() {
                reverse_iterator tmp(dummy);
                return tmp;
            } // iterator to the beginning of the reversed list
            reverse_iterator rend() {
                reverse_iterator tmp(dummy->next);
                return tmp;
            }   // iterator to the end of the reversed list
            std::pair<iterator, bool> insert(const T &value) {
                if (!find(value)) {
                    C c = C();
                    if (list == dummy) {
                        list = newNode(value);
                        list->prev = dummy;
                        list->next = dummy;
                        dummy->prev = list;
                        dummy->next = list;
                        iterator tmp(list);
                        return std::make_pair(tmp, true);
                    } else if (!c(list->data, value)) {
                        Node * new_node = newNode(value);
                        new_node->next = list;
                        new_node->next->prev = new_node;
                        new_node->prev = dummy;
                        list = new_node;
                        dummy->next = list;
                        iterator tmp(new_node);
                        return std::make_pair(tmp, true);
                    } else {
                        Node * current = list;
                        while (current->next != dummy && c(current->next->data, value)) {
                            current = current->next;
                        }
                        Node * new_node = newNode(value);
                        new_node->next = current->next;
                        new_node->next->prev = new_node;
                        
                        current->next = new_node;
                        new_node->prev = current;
                        iterator tmp(new_node);
                        return std::make_pair(tmp, true);
                    }
                } else {
                    Node * tmp_node = find_node(value);
                    iterator tmp(tmp_node);
                    return std::make_pair(tmp, false);
                }
            }
            // insert value, returns a pair consisting of an iterator to the
            // inserted element (or to the element that prevented the insertion)
            // and a bool value set to true if the insertion took place.
            void erase(iterator first, iterator last) {
                while (first != last) {
                    T tmp = *first;
                    ++first;
                    erase(tmp);
                }
            }
            // removes all elements in range [first,last)
            template <class UnaryPredicate>
            void erase_if(UnaryPredicate p) {
                Node * current = list;
                while(current != dummy) {
                    T tmp = current->data;
                    current = current->next;
                    if(p(tmp)) {
                        erase(tmp);
                    }
                }
            }
            std::pair<iterator, bool> insert(sorted_list_iterator &it, const T &value) {
                (void)it;
                return insert(value);
            }

            // removes all elements satisfying predicate p
    };

}

#endif