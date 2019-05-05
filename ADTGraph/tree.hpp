#ifndef TREE_HPP
#define TREE_HPP

#include "treenode.hpp"

class nary_tree : public object {   // abstract base class for n-ary trees
   using inherited = object;

   public:
      virtual ~nary_tree () {  // destructor
            while(m_p_root && m_p_root->p_son()) {
                delete m_p_root->p_son();
            }
      }

      nary_tree & operator = (nary_tree const & other) {   // copy assignment
            if(this != &other) {
                delete this->m_p_root;
                this->m_p_root = other.m_p_root->cpy_nodes(nullptr);
            }
            return *this;
      }

      nary_tree & operator = (nary_tree &&) noexcept {   // move assignment
         return * std::move(this);
      }

      bool isEmpty() {
          return m_p_root == nullptr;
      }

      void setRoot(nary_tree_node * root){
          if(m_p_root){
              delete m_p_root;
          }
          this->m_p_root = root;
      }

      nary_tree_node * getRoot() const {
          return this->m_p_root;
      }

      

   protected:
      nary_tree () : inherited {} {   // default constructor
      }

      nary_tree (nary_tree const & other) : inherited {} {
            if(this != &other){
                if(m_p_root) {delete this->m_p_root;}
                this->m_p_root = other.m_p_root->cpy_nodes(nullptr);
            } // copy construction 
      }

      nary_tree (nary_tree && other) noexcept : inherited {} {   // move construction
            m_p_root = std::move(other.m_p_root);
            other.m_p_root = nullptr; //keep other in a valid state
      }

   private:
      nary_tree_node * m_p_root {nullptr};   // ownership semantics
};

#endif