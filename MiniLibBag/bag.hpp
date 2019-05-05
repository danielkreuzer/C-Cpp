#ifndef BAG_HPP
#define BAG_HPP

#include <MLCollection.h>

struct Bag final : ML::Collection
{
  public:
    Bag() { Register("Bag", "Collection"); };

    Bag(const Bag &) = delete;
    Bag(Bag &&) = delete;

    auto operator=(const Bag &) -> Bag & = delete;
    auto operator=(Bag &&) -> Bag & = delete;

    ~Bag();

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

    //own functions
    std::ostream & PrintWithAmount (std::ostream & out) const;
    //returns bag with amount of insertations

    struct Node : ML::Object
    {
      public:
        Node(ML::Object *obj) : value{obj}
        {
            Register("Bag::Node", "Object");
        }

        Node(const Node &) = delete;
        Node(Node &&) = delete;

        auto operator=(const Node &) -> Node & = delete;
        auto operator=(Node &&) -> Node & = delete;

        ~Node() = default;

        ML::Object *value{nullptr};
        int containings{1};
        Node *next{nullptr};
    } * head{nullptr}; // == Node * Head{nullptr}

    Node *ContainsPtr(ML::Object *o);

    struct BagIterator : ML::Iterator
    {
      public:
        BagIterator(const Node *node) : node{node}
        {
            Register("Bag::BagIterator", "Iterator");
        }

        BagIterator(const Iterator &) = delete;
        BagIterator(BagIterator &&) = delete;

        auto operator=(const BagIterator &) -> BagIterator & = delete;
        auto operator=(BagIterator &&) -> BagIterator & = delete;

        ~BagIterator() = default;

        auto Next() -> ML::Object * override;

      private:
        const Node *node{nullptr};
    };
};

#endif