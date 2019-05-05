#include "set.hpp"
#include <cassert>
#include <memory>

Set::~Set(){
    Clear();
}


int Set::Size() const {
    int result{0};
    std::unique_ptr<ML::Iterator> it{NewIterator()};
    while(it->Next()) ++result;
    return result;
}


void Set::Add(ML::Object * o) {
    //APEND
    assert(o);
    //check if already in list
    if(Set::Contains(o)){
        //do nothing
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

ML::Object *Set::Remove(ML::Object *o) {
    
    assert(o);
    Node * prev{nullptr}, * curr{head};
    for(; curr && !curr->value->IsEqualTo(o); curr = curr->next) prev = curr;

    if(!curr) return nullptr; //did not find 'o'
    else { //found 'o'
        if(!prev) head = curr->next; //'o' is head
        else prev->next = curr->next;//'o' is somewhere
        auto ptr{curr->value};
        delete curr;
        return ptr;
    }
}

bool Set::Contains(ML::Object *o) const {
    assert(o);

    std::unique_ptr<ML::Iterator> it{NewIterator()};
    for(auto obj{it->Next()}; obj; obj = it->Next()){
        if(obj->IsEqualTo(o)){
            return true;
        }
    }
    return false;
}

void Set::Clear() {
    while(head){
        std::unique_ptr<Node> ptr{head};
        head = head->next;
    }
}

ML::Iterator *Set::NewIterator() const {
    return new SetIterator{head};
}

auto Set::SetIterator::Next() -> ML::Object*{
    if(!node) return nullptr;
    const auto ptr{node->value};
    node = node->next;
    return ptr;
}

Set* Set::Difference(Set * other){
    Set * output = new Set();

    std::unique_ptr<ML::Iterator> it{NewIterator()};

    for (auto obj{it->Next()}; obj; obj = it->Next())
    {
        if(!(other->Contains(obj))){
            output->Add(obj);
        }
    }

    return output;
}

bool Set::Includes(Set * other){
    std::unique_ptr<ML::Iterator> itOther{other->NewIterator()};

    for (auto obj{itOther->Next()}; obj; obj = itOther->Next()) {
        if(!(this->Contains(obj))){
            return false;
        }
    }
    return true;
}

Set* Set::Intersection(Set * other){
    Set * output = new Set();

    std::unique_ptr<ML::Iterator> it{NewIterator()};

    for (auto obj{it->Next()}; obj; obj = it->Next())
    {
        if(other->Contains(obj)){
            output->Add(obj);
        }
    }

    return output;
}

void Set::Merge(Set * other){
    std::unique_ptr<ML::Iterator> itOther{other->NewIterator()};

    for (auto obj{itOther->Next()}; obj; obj = itOther->Next()) {
        this->Add(obj);
    }
}

Set* Set::SymmetricDifference(Set * other){
    Set * output = new Set();

    std::unique_ptr<ML::Iterator> it{NewIterator()};

    for (auto obj{it->Next()}; obj; obj = it->Next()) {
        if(!(other->Contains(obj))){
            output->Add(obj);
        }
    }

    std::unique_ptr<ML::Iterator> itOther{other->NewIterator()};

    for (auto obj{itOther->Next()}; obj; obj = itOther->Next()) {
        if (!(this->Contains(obj))){
            output->Add(obj);
        }
    }

    return output;
}

Set* Set::Union(Set * other){
    Set * output = new Set();

    std::unique_ptr<ML::Iterator> it{NewIterator()};

    for (auto obj{it->Next()}; obj; obj = it->Next()) {
        output->Add(obj);
    }

    std::unique_ptr<ML::Iterator> itOther{other->NewIterator()};

    for (auto obj{itOther->Next()}; obj; obj = itOther->Next()) {
        output->Add(obj);
    }

    return output;
}