#include "bag.hpp"
#include <cassert>
#include <memory>

Bag::~Bag(){
    Clear();
}

int Bag::Size() const {
    int result{0};
    std::unique_ptr<ML::Iterator> it{NewIterator()};
    while(it->Next()) ++result;
    return result;
}

void Bag::Add(ML::Object * o) {
    //APEND
    assert(o);
    auto * check = ContainsPtr(o);
    if(check != nullptr){
        check->containings = check->containings + 1; 
    } else {
        auto node = new Node{o};
        if(!head) head = node;
        else {
            auto ptr{head};
            while (ptr->next) ptr = ptr->next;
            ptr->next = node;
        }
    }
}

ML::Object * Bag::Remove(ML::Object * o) {
    assert(o);
    Node * prev {nullptr}, * curr{head};
    for(; curr && !curr->value->IsEqualTo(o); curr = curr->next){
        prev = curr;
    }

    if(!curr) return nullptr;
    else {
        if(curr->containings > 1){
            curr->containings = curr->containings - 1;
            return curr->value;
        } else {
            if(!prev) head = curr->next; //'o' is head
            else prev->next = curr->next;//'o' is somewhere
            auto ptr{curr->value};
            delete curr;
            return ptr;
        }
    }
}

bool Bag::Contains(ML::Object *o) const {
    assert(o);

    std::unique_ptr<ML::Iterator> it{NewIterator()};
    for(auto obj{it->Next()}; obj; obj = it->Next()){
        if(obj == *&o){
            return true;
        }
    }
    return false;
}

Bag::Node * Bag::ContainsPtr(ML::Object * o) {
    assert(o);
    auto * ptr = this->head;
    while(ptr != nullptr){
        if(*&ptr->value == *&o){
            return ptr;
        } else {
            ptr = ptr->next;
        }
    }
    return nullptr;
}

void Bag::Clear() {
    while(head){
        std::unique_ptr<Node> ptr{head};
        head = head->next;
    }
}

ML::Iterator *Bag::NewIterator() const {
    return new BagIterator{head};
}

auto Bag::BagIterator::Next() -> ML::Object*{
    if(!node) return nullptr;
    const auto ptr{node->value};
    node = node->next;
    return ptr;
}

std::ostream & Bag::PrintWithAmount (std::ostream & out) const{
    out << "Bag contains: { ";
    auto * ptr = this->head;
    while(ptr != nullptr){
        out << ptr->containings;
        out << "x\"";
        out << ptr->value->AsString() + "\" ";
        ptr = ptr->next;
    }
    out << "} \n";
    return out;
}