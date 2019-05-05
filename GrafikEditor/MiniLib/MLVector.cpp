// MLVector.cpp:                                         MiniLib V.4, 2004
// ------------
// Implementaion of classes Vector and VectorIterator.
//========================================================================

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

#include "MetaInfo.h"
#include "MLString.h"
#include "MLVector.h"

namespace ML {


//--- comparison function for qsort called in Vector::Sort ---

  static int ObjCmp(const void *p1, const void *p2) {
     Object *o1 = *(Object**)p1;
     Object *o2 = *(Object**)p2;
     if (o1->IsLessThan(o2))
       return -1;
     else if (o1->IsEqualTo(o2))
       return  0;
     else // o1->IsGreaterThan(*o2)
       return +1;
  } // ObjCmp


//=== class Vector ===


  void Vector::CheckIndex(int index) const {
    if ((index < 0) || (index >= curSize)) {
      String s("range error detected in Vector::CheckIndex, index ");
      s = s + index + " is not in [0 .. " + (curSize - 1) + "]";
      Error(s);
    } // if
  } // Vector::IsValid

  void Vector::IncreaseCapacity() {
    assert(curSize == curCap); // else no need for increase
    curCap *= 2;            // double curCap
    Object **newElements = new Object*[curCap];
    memcpy(newElements, elements, curSize * sizeof(Object*));
    delete[] elements;
    elements = newElements;
  } // Vector::IncreaseCapacity


  Vector::Vector(int curCap)
  : curCap(curCap) {
    Register("Vector", "Collection");
    curSize  = 0;
    elements = new Object*[curCap];
  } // Vector::Vector

  Vector::Vector(const Vector &v)
  : curCap(v.curCap) {
    Register("Vector", "Collection");
    curSize  = v.curSize;
    elements = new Object*[curCap];
    memcpy(elements, v.elements, curSize * sizeof(Object*));
  } // Vector::Vector

  Vector::~Vector() {
    delete[] elements;
  } // Vector::~Vector


//--- implementations of abstract methods ---

  int Vector::Size() const {
    return curSize;
  } // Vector::Size

  void Vector::Add(Object *o) {
    if (curSize == curCap)  // vector is full
      IncreaseCapacity();
    elements[curSize] = o;
    curSize++;
  } // Vector::Add

  Object *Vector::Remove(Object *o) {
    int index = IndexOf(o);
    if (index < 0)
      return 0; // element o is not in vector
    else
      return RemoveAt(index);
  } // Vector::Remove

  bool Vector::Contains(Object *o) const {
    return IndexOf(o) >= 0;
  } // Vector::Contains

  void Vector::Clear() {
    curSize = 0;
  } // Vector::Clear

  Iterator *Vector::NewIterator() const {
    return new VectorIterator(const_cast<Vector*>(this));
  } // Vector::Iterator


//--- new methods ---

  Vector& Vector::operator=(const Vector& v){
    if (this != &v) {
      if (curCap != v.curCap) {
        delete[] elements;
        curCap   = v.curCap;
        elements = new Object*[curCap];
      } // if
      curSize = v.curSize;
      memcpy(elements, v.elements, curSize * sizeof(Object*));
    } // if
    return *this;
  } // Vector::operator=

  Object *&Vector::operator[](int index) {
    CheckIndex(index);
    return elements[index];
  } // Vector::Put

  void Vector::SetAt(int index, Object *o) {
    this->operator[](index) = o;
  } // Vector::SetAt

  Object *Vector::GetAt(int index) const {
    return (const_cast<Vector*>(this))->operator[](index);
  } // Vector::GetAt

  int Vector::IndexOf(Object *o) const {
    for (int i = 0; i < curSize; i++) {
      if (elements[i]->IsEqualTo(o))
        return i;
    } // for
    return -1; // not found
  } // Vector::IndexOf

  void Vector::InsertAt(int index, Object *o) {
    CheckIndex(index);
    if (curSize == curCap)
      IncreaseCapacity();
    for (int i = curSize; i > index; i--) { // shift elements right
      elements[i] = elements[i - 1];
    } // for
    elements[index] = o;
    curSize++;
  } // Vector::InsertAt

  Object *Vector::RemoveAt(int index) {
    CheckIndex(index);
    Object *o = elements[index];
    for (int i = index; i < curSize - 1; i++) { // shift elements left
      elements[i] = elements[i + 1];
    } // for
    curSize--;
    return o;
  } // Vector::RemoveAt

  void Vector::Sort() {
    qsort(elements, curSize, sizeof(Object*), ObjCmp);
  } // Vector::Sort


//=== class VectorIterator ===


  VectorIterator::VectorIterator(Vector *v)
  : v(v), curIdx(0) {
    Register("VectorIterator", "Iterator");
  } // VectorIterator::VectorIterator

  VectorIterator::~VectorIterator() {
    // nothing to do
  } // VectorIterator::~VectorIterator


//--- implementation of abstract method ---

  Object *VectorIterator::Next() {
    Object *o = 0;
    while ( (curIdx < v->Size()) && (o == 0) ) {
      o = v->GetAt(curIdx);
      curIdx++;
    } // while
    return o;
  } // VectorIterator::Next


} // ML

//========================================================================
// end of file MLVector.cpp
