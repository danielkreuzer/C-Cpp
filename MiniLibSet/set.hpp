#ifndef SET_HPP
#define SET_HPP

#include <MLCollection.h>

struct Set final : ML::Collection {
    public:
        Set() {Register("Set", "Collection");};

        Set(const Set &) = delete;
        Set(Set &&) = delete;

        auto operator=(const Set &) ->Set & = delete;
        auto operator=(Set &&) ->Set& = delete;

        ~Set();

        //---Copied from MLCollection---
        //---make concrete class from virtual class---

        int Size() const override;
        // returns number of elements in collection

        void Add(ML::Object *o) override;
        // adds element o to collection

        ML::Object *Remove(ML::Object *o) override;
        // removes first element == o and returns that element

        bool Contains(ML::Object *o) const override;
        // returns whether collection contains an element == o

        void Clear() override;
        // removes all elements WITHOUT deleting them

        ML::Iterator *NewIterator() const override;
        // returns an iterator which has to be deleted after usage

        //--- mathematical set operations ---
        //

        Set* Difference(Set * other);

        bool Includes(Set * other);

        Set* Intersection(Set * other);

        void Merge(Set * other);

        Set* SymmetricDifference(Set * other);

        Set* Union(Set * other);
    
        struct Node : ML::Object
        {
            public:
            Node(ML::Object * obj) : value{obj} {
                Register("Set::Node", "Object");
            }

            Node(const Node &) = delete;
            Node(Node &&) = delete;

            auto operator=(const Node &) -> Node & = delete;
            auto operator=(Node &&) -> Node & = delete;

            ~Node() = default;

            ML::Object * value{nullptr};
            Node * next{nullptr};
        } * head {nullptr}; // == Node * Head{nullptr}

        struct SetIterator : ML::Iterator {
            public:
                SetIterator(const Node * node) : node{node} {
                    Register("Set::SetIterator", "Iterator");
                }

                SetIterator(const Iterator &) = delete;
                SetIterator(SetIterator &&) = delete;

                auto operator=(const SetIterator &) -> SetIterator & = delete;
                auto operator=(SetIterator &&) -> SetIterator & = delete;

                ~SetIterator() = default;

                auto Next() -> ML::Object * override;

            private:
                const Node * node{nullptr};
        };

};


#endif