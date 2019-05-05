// MLCollection.cpp:                                     MiniLib V.4, 2004
// ----------------
// Implementaitons of abstract classes Collection and Iterator.
//========================================================================

#include <sstream>
#include <memory>
using namespace std;

#include "MLCollection.h"

namespace ML {


//=== class Collection ===


  Collection::Collection() {
    Register("Collection", "Object");
  } // Collection::Collection

  Collection::~Collection() {
    // nothing to do
  } // Collection::~Collection


//--- overridden method ---

  string Collection::AsString() const {
    ostringstream os;
    os << Size();
    string s(Class() + " with " + os.str() + " Elements: { ");
    unique_ptr<Iterator> it(this->NewIterator());
    Object *o = it->Next();
    bool first = true;
    while (o !=0 ) {
      if (first) {
        s += o->AsString();
        first = false;
      } else
        s += string(", ") + o->AsString();
      o = it->Next();
    } // while
    s += " }";
    return s;
  } // Collection::AsString


//--- new method ---

  void Collection::DeleteElements() {
    unique_ptr<Iterator> it(NewIterator());
    Object *o = it->Next();
    while (o != 0) {
      if (o->IsA("Collection")) { // delete its elements recursively
        Collection *c = dynamic_cast<Collection*>(o);
        c->DeleteElements();
      } // if
      delete o;
      o = it->Next();
    } // while
    Clear();
  } // Collection::DeleteElements


//=== class Iterator ===


  Iterator::Iterator() {
    Register("Iterator", "Object");
  } // Iterator::Iterrator

  Iterator::~Iterator() {
    // nothing to do
  } // Iterator::Iterrator


} // ML

//========================================================================
// end of file MLCollection.cpp
