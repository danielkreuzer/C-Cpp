#include <iostream>
#include <string>
#include "string_tree.hpp"

using namespace std::literals;

int main() {
    //create Nodes
    string_node * root = new string_node{"root"};
    string_node * first_layer_one = new string_node{"first_layer_one"};
    string_node * secound_layer_one = new string_node{"secound_layer_one"};
    string_node * secound_layer_two = new string_node{"secound_layer_two"};
    string_node * secound_layer_three = new string_node{"secound_layer_three"};
    string_node * third_layer_one = new string_node{"third_layer_one"};
    string_node * third_layer_two = new string_node{"third_layer_two"};
    string_node * third_layer_three = new string_node{"third_layer_three"};

    //create structure
    first_layer_one->add_node(secound_layer_three);
    first_layer_one->add_node(secound_layer_one);
    first_layer_one->add_node(secound_layer_two);
    
    secound_layer_one->add_node(third_layer_three);
    secound_layer_two->add_node(third_layer_one);
    secound_layer_two->add_node(third_layer_two);

    root->add_node(first_layer_one);

    //Test copy constructor only on node
    string_node test = * first_layer_one;
    std::cout << "\n Test copy on node only, copied first_layer_one node: \n" << test << std::endl;
    std::cout << "\n instances: " << object::get_instances() << std::endl;
    test.delete_node("secound_layer_one");
    std::cout << "\n Test Delete secound_layer_one from first_layer_one node: \n" << test << std::endl;
    std::cout << "\n instances: " << object::get_instances() << std::endl;
    string_node * test2{first_layer_one};
    std::cout << "\n proof that original first_layer_one is not changed, and copied without assign operator: \n" << *test2 << std::endl;
    std::cout << "\n instances: " << object::get_instances() << std::endl;
    
    //create trees
    tree_string * tree_one = new tree_string();

    std::cout << "\n is tree empty?: " << std::boolalpha << tree_one->isEmpty();
    tree_one->setRoot(root);
    std::cout << "\n is tree empty?: " << std::boolalpha << tree_one->isEmpty();

    std::cout << "\n first tree: \n" << *tree_one;
    std::cout << "\n instances: " << object::get_instances() << std::endl;
    
    //test copy semantics on tree
    tree_string tree_two{*tree_one};
    std::cout << "\n secound copied tree: \n" << tree_two << std::endl;;
    std::cout << "\n instances: " << object::get_instances() << std::endl;
    tree_two.delete_node("secound_layer_two");
    std::cout << "\n secound copied tree with deleted secound_layer_two: \n" << tree_two << std::endl;
    std::cout << "\n instances: " << object::get_instances() << std::endl;
    std::cout << "\n first tree again, proof that tree two is a deep copy: \n" << * tree_one << std::endl;
    std::cout << "\n instanceslast: " << object::get_instances() << std::endl;
    
}