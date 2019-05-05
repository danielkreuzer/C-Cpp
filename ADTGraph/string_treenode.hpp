#ifndef STRING_TREENODE_HPP
#define STRING_TREENODE_HPP

#include "tree.hpp"

    class string_node : public nary_tree_node {
        public:
            string_node() = default;
            
            string_node(std::string input) : nary_tree_node{}, data{input} {
            }

            //copy Construktor
            string_node(const string_node & other) : nary_tree_node(other){
                this->data = other.data;
                this->set_p_parent(nullptr);
                if (other.p_sibling()){
                    this->set_p_sibling(other.p_sibling()->cpy_nodes(this));
                }

                if (other.p_son()) {
                    this->set_p_son(other.p_son()->cpy_nodes(this));
                }
            }

            //copy Assigment
            string_node & operator = (const string_node & other) {
                string_node * temp = new string_node{other};
                return * temp;
            }
            

            // deep copy function
            string_node * cpy_nodes(nary_tree_node * parentPtr) {
                string_node * newnode = new string_node;
                newnode->data = this->data;
                if(this->p_son()){
                    newnode->set_p_son(this->p_son()->cpy_nodes(newnode));
                }
                if(this->p_sibling()) {
                    newnode->set_p_sibling(this->p_sibling()->cpy_nodes(newnode));
                }
                if(parentPtr){
                    newnode->set_p_parent(parentPtr);
                }
                return newnode;

            }
            
            std::ostream &print(std::ostream &out = std::cout) const {
                static int offset = 0;
                
                for (int i = 0; i < offset; i++){
                    out << "--| ";
                }

                out << this->data << std::endl;

                if(this->p_son()){
                    offset++;
                    this->p_son()->print();
                    offset--;
                }
                
                if(this->p_sibling()) {
                    this->p_sibling()->print();
                }
                
                return out;
            }

            std::string getString() {
                return data;
            }
            
            bool delete_node(std::string const input) {
                string_node * son = dynamic_cast<string_node *>(this->p_son());
                string_node * current = nullptr;
                current = son;
                bool found = false;
                
                while (current != nullptr && current->data != input && !found) {
                   
                    if (current != nullptr) {
                        found = current->delete_node(input);
                    }
                    current = dynamic_cast<string_node *>(current->p_sibling());
                    if (found == false && current != nullptr) {
                        found = current->delete_node(input);
                    }
                    
                }
                if(current != nullptr){
                    delete current;
                    return true;
                }
                return false || found;
            }

            string_node (string_node &&){
                std::move(this);
            };

            string_node &operator=(string_node &&){
                return * std::move(this);
            }
        private:
                std::string data{""};
    };
#endif