// MLCollection.h:                                       MiniLib V.4, 2004
// --------------
// Declarations of abstract classes Collection and Iterator,
// the root classes for all MiniLib collection and iterator classes.
//========================================================================

#ifndef MLCollection_h
#define MLCollection_h

#include <iosfwd>

#include "MLObject.h"

namespace ML {


  class Iterator; // full declaration see below Collection


//=== class Collection ===

  class Collection: public Object { // abstract class

  public:

    Collection();
    virtual ~Collection();

//--- overridden method ---

    virtual std::string AsString() const;
    // returns textual representation of collection

//--- new abstract methods that have to be overridden ---

    virtual int Size() const = 0;
    // returns number of elements in collection

    virtual void Add(Object *o) = 0;
    // adds element o to collection

    virtual Object *Remove(Object *o) = 0;
    // removes first element == o and returns that element

    virtual bool Contains(Object *o) const = 0;
    // returns whether collection contains an element == o

    virtual void Clear() = 0;
    // removes all elements WITHOUT deleting them

    virtual Iterator *NewIterator() const = 0;
    // returns an iterator which has to be deleted after usage

//--- new method ---

    virtual void DeleteElements();
    // removes all elements (like Clear) AND deletes them

  }; // Collection


//=== class Iterator ===

  class Iterator: public Object { // abstract class

  public:

    Iterator();

    virtual ~Iterator();

//--- new abstract method ---

    virtual Object *Next() = 0;
    // returns next element or 0 if "end of" collection reached

  }; // Iterator

} // ML


#endif

//========================================================================
// end of file MLCollection.h
