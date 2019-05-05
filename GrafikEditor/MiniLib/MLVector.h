// MLVector.h:                                           MiniLib V.4, 2004
// ----------
// Declaration of classes Vector and VectorIterator.
//========================================================================

#ifndef MLVector_h
#define MLVector_h

#include "MLObject.h"
#include "MLCollection.h"

namespace ML {


  class VectorIterator; // full declaration see below Collection


//=== class Vector ===

  class Vector: public Collection {

  private:

    int      curCap;   // cur. capacity = space for elements
    int      curSize;  // cur. number of elements <= curCap
    Object **elements; // dyn. allocated array of Object pointers

    void CheckIndex(int index) const;
    void IncreaseCapacity();

  public:

    Vector(int capacity = 10);
    Vector(const Vector &v);

    virtual ~Vector();

//--- overridden abstract methods ---

    virtual int Size() const;
    // returns number of elements in collection

    virtual void Add(Object *o);
    // adds element at right end

    virtual Object *Remove(Object *o);
    // removes first element == o, shifts rest to the left
    // and returns removed element

    virtual bool Contains(Object *o) const;
    // returns whether collection contains an element == o

    virtual void Clear();
    // removes all elements WITHOUT deleting them

    virtual Iterator *NewIterator() const;
    // returns a vector iterator which has to be deleted after usage

//--- new methods ---

    Vector& operator=(const Vector& v);
    // assigns a to this

    virtual Object *&operator[](int index);
    // index operator

    virtual void    SetAt(int index, Object *o);
    virtual Object *GetAt(int index) const;

    virtual int IndexOf(Object *o) const;
    // returns first index for element == o or -1

    virtual void InsertAt(int index, Object *o);
    // inserts object at given index, shifts rest to the right

    virtual Object *RemoveAt(int index);
    // removes object at index and returns removed object

    virtual void Sort();
    // sorts elements

  }; // Vector


//=== class VectorIterator ===

  class VectorIterator: public Iterator {

    friend Iterator *Vector::NewIterator() const;

  private:

    Vector *v;        // vector to iterate over
    int     curIdx;   // current index in v

    VectorIterator(Vector *v);

  public:

    virtual ~VectorIterator();

//--- implementation of abstract method ---

    virtual Object *Next();
    // returns next element or 0 if "end of" collection reachen

  }; // VectorIterator

} // ML


#endif

//========================================================================
// end of file MLVector.h
