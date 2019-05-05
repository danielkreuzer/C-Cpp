#ifndef STRING_TREE_HPP
#define STRING_TREE_HPP

#include "string_treenode.hpp"
    class tree_string : public nary_tree {
        public:
            tree_string() = default;

            tree_string(tree_string && other) : nary_tree(std::move(other)) {

            }

            tree_string(tree_string & other) : nary_tree(other) {
                string_node * temp = new string_node;
                temp = dynamic_cast<string_node *>(other.getRoot())->cpy_nodes(nullptr);
                this->setRoot(temp);
            }

            std::ostream& print(std::ostream & = std::cout) const{
                return this->getRoot()->print();
            }

            bool delete_node(std::string const input){
                return dynamic_cast<string_node *>(this->getRoot())->delete_node(input);
            }

            tree_string & operator = ( tree_string const & other){
                if(this != &other) {
                    tree_string *temp = new tree_string;
                    temp->setRoot(other.getRoot()->cpy_nodes(nullptr));
                    return *temp;
                }
                return * this;
            }

            
    };
#endif