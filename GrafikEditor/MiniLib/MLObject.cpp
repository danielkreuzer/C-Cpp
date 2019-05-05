// MLObject.cpp:                                         MiniLib V.4, 2004
// ------------
// Implementiaton of Object, the root class for all MiniLib classes.
//========================================================================

#include <iostream>
#include <new>
using namespace std;

#include "MetaInfo.h"
#include "MLObject.h"

#pragma warning (disable : 4702) // VC++: suppress unreachable code warnings

namespace ML {


//=== global operator<< supporting ALL MiniLib classes ===

  ostream &operator<<(ostream &out, const Object &o) {
    o.WriteAsString(out); // uses WriteAsString()
    return out;
  } // operator<<


//=== class Object ===

  Object::Object()
  : isDynamic(false), classIndex(-1) { // assume a static NONE object
    Register("Object", "NONE");
  } // Object::Object

  Object::~Object() {
    // nothing to do
  } // Object::Object


//--- meta information, no overrides ---

  void Object::Register(const char* className, const char* baseClassName) {
    MI_Register(className, baseClassName, static_cast<void*>(this));
  } // Object::Register

  string Object::Class() const {
    return MI_ClassNameOf(classIndex);
  } // Object::Class

  string Object::BaseClass() const {
    return MI_BaseClassNameOf(classIndex);
  } // Object::BaseClass

  bool Object::IsA(const string &className) const {
    return MI_IsEqualToOrDerivedFrom(Class(), className);
  } // Object::IsA


//---  generation of textual object representations ---

  string Object::AsString() const {
    return Class(); // simply name of class
  } // Object::AsString

  void Object::WriteAsString(ostream &out) const {
    out << AsString();
  } // Object::WriteAsString


//--- comparison method that may be overridden ---

  bool Object::IsEqualTo(const Object *o) const {
    return (this == o); // identical
  } // Object::IsEqualTo


 //--- comparison method that that has to be overridden ---

  bool Object::IsLessThan(const Object *o) const {
    // simple comparison of addresses would be possibe: this < o ?
    // but the result would be useless
    Error("invalid IsLessThan comparison of " + Class() + " with " + o->Class());
    return false; // not executed
  } // Object::IsLessThan


//--- creation and deletion of dynamic objects, no overrides ---

  void *Object::operator new(size_t objSize) {
    void *p = 0;
    try {
      p = ::operator new(objSize);
    } catch (bad_alloc /* &ba */) {
      Error("memory allocation failed in ::operator new(size_t), heap overflow");
    } // try/catch
    MI_RememberAddr(p); // prepare for registration of object in MI_Register
    return p;
  } // Object::operator new

  void Object::operator delete(void *p, size_t) {
    if (p != 0) {
      Object *o = static_cast<Object*>(p);
      MI_IncDeletionCounter(o->classIndex);
      ::operator delete(p);
    } // if
  } // Object::operator delete


} // ML

//========================================================================
// end of file MLObject.cpp
