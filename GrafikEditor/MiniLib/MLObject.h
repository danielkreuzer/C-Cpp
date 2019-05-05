// MLObject.h:                                           MiniLib V.4, 2004
// ----------
// Declaration of Object, the root class for all MiniLib classes.
//========================================================================

#ifndef MLObject_h
#define MLObject_h

#include <iostream>
#include <string>

#include "MetaInfo.h"


namespace ML {


//=== global operator<< supporting ALL MiniLib classes ===

  class Object; // declaration see below

  std::ostream &operator<<(std::ostream &out, const Object &o);
  // uses method Object::WriteAsString


//=== class Object ===

  class Object {        // treat like an abstract class

    friend void ::MI_Register(const std::string &, const std::string &, void *objAddr);
    // called from Register, provides access to private data

  private: // set in ::MI_Register

    bool isDynamic;     // true for objects allocated on the heap
    int  classIndex;    // index of object's class in MetaInfo

  protected:

    Object();           // prohibit object creation in clients, "abstract class"

  public:

    virtual ~Object();

//--- meta information, no overrides ---

    void Register(const char* className, const char* baseClassName);
    // registers class and its base class, increases class' creation counter
    //    has to be called in every constructor in every class

    std::string Class() const;
    // returns name of class for object

    std::string BaseClass() const;
    // returns name of base class for object

    bool IsA(const std::string &className) const;
    // returns whether object is at least of specified class

//--- generation of textual object representations ---

    virtual std::string AsString() const;
    // returns string representation for object

    virtual void WriteAsString(std::ostream &out = std::cout) const;
    // writes object to stream per default using AsString

//--- comparison method that may be overridden ---

    virtual bool IsEqualTo(const Object *o) const;
    // checks for identity: this == o ?

//--- comparison method that has to be overridden ---

    virtual bool IsLessThan(const Object *o) const;
    // writes error message if not overridden

//--- creation and deletion of dynamic objects, no overrides ---

    void* operator new(size_t objSize);
    // extension of ::operator new for object counting

    void operator delete(void *p, size_t objSize);
    // extension of ::operator delete for object counting

  }; // Object

} // ML


#endif

//========================================================================
// end of file MLObject.h

