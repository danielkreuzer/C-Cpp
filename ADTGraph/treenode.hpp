#ifndef TREENODE_HPP
#define TREENODE_HPP

#include "obj.hpp"

class nary_tree_node : public object {   // abstract base class for n-ary tree nodes
   using inherited = object;

   public:
      virtual ~nary_tree_node (){
            //delete for one node and not the stuctore below/above
            //to keep the tree valid (no leaks) the ptr-structure has to be rebuild
            if(this->m_p_son != nullptr) {
                  if(this->m_p_parent != nullptr) {
                        this->m_p_son->m_p_parent = this->m_p_parent;
                        if(this->m_p_parent->m_p_son == this) {
                              this->m_p_parent->m_p_son = this->m_p_son;
                        }
                        if(this->m_p_parent->m_p_sibling == this) {
                              this->m_p_parent->m_p_sibling = this->m_p_son;
                        }
                  }
                  
                  nary_tree_node * temp = this->m_p_son;
                  if(temp && temp->m_p_sibling){
                        while (temp->m_p_sibling != nullptr) {
                              temp = temp->m_p_sibling;
                        }
                        if(this->m_p_sibling) {
                              temp->m_p_sibling = this->m_p_sibling;
                        }
                  }
            }
            
      };   // destructor

      nary_tree_node & operator = (nary_tree_node const & other) = default;   // copy assignment
      
      nary_tree_node & operator = (nary_tree_node && other) = default;
      
      //getter/setter
      auto *  p_son() const {
            return m_p_son;
      }

      auto * p_sibling () const { 
         return m_p_sibling;
      }

      auto * p_parent() const{
            return m_p_parent;
      }

      void set_p_sibling(nary_tree_node * sibling) {
            this->m_p_sibling = sibling;
      }

      void set_p_son(nary_tree_node * son){
            this->m_p_son = son;
      }

      void set_p_parent(nary_tree_node * parent){
            this->m_p_parent = parent;
      }

      void add_node(nary_tree_node * newnode){
            if(this->m_p_son) {
                  newnode->m_p_parent = this; 
                  newnode->m_p_sibling = this->m_p_son;
                  this->m_p_son->m_p_parent = newnode;
                  this->m_p_son = newnode;
            } else {
                  newnode->m_p_parent = this;
                  this->m_p_son = newnode;
            }
            nr_of_nodes++;
      }

      virtual nary_tree_node * cpy_nodes(nary_tree_node * parentPtr) = 0;

   protected:
      nary_tree_node () = default;   // default constructor

      explicit nary_tree_node (nary_tree_node * const p_son, nary_tree_node * const p_sibling)   // additional constructor
         : inherited   {}
         , m_p_sibling {p_sibling}
         , m_p_son     {p_son} {
      }

      nary_tree_node (nary_tree_node const &) = default;   // copy construction
      nary_tree_node (nary_tree_node && other) = default;       // move construction

   private:
     
      nary_tree_node * m_p_parent {nullptr};
      nary_tree_node * m_p_sibling {nullptr};   // no ownership
      nary_tree_node * m_p_son     {nullptr};   // no ownership
      int nr_of_nodes{1};
};

#endif