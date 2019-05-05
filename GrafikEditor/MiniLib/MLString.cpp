// MLString.cpp:                                         MiniLib V.4, 2004
// ------------
// Implementation of String, the MiniLib string class.
//========================================================================

#include <iostream>
#include <sstream>
using namespace std;

#include "MetaInfo.h"
#include "MLString.h"

namespace ML {


//=== global operator>> for String ===

  istream &operator>>(istream &is, String &s) {
    is >> s.cppStr;
    return is;
  } // operator>>


//=== class String ===

  String::String()
  : cppStr() {
    Register("String", "Object");
  } // String::String

  String::String(const char *s)
  : cppStr(s) {
    Register("String", "Object");
  } // String::String

  String::String(const string &s)
  : cppStr(s) {
    Register("String", "Object");
  } // String::String

  String::String(const String &s)
  : cppStr(s.AsString()) {
    Register("String", "Object");
  } // String::String

  String::String(int i) {
    Register("String", "Object");
    ostringstream os;
    os << i;
    cppStr = os.str();
  } // String::String

  String::String(double d) {
    Register("String", "Object");
    ostringstream os;
    os << d;
    cppStr = os.str();
  } // String::String

  String::~String() {
    // nothing to do
  } // String::~String


//--- overridden methods ---

  string String::AsString() const {
    return cppStr;
  } // String::AsString

  bool String::IsEqualTo(const Object *o) const {
    if (o->IsA("String")) {
      const String *strObj = dynamic_cast<const String*>(o);
      return cppStr == strObj->cppStr;
    } else {
      Error("invalid IsEuqalTo comparison of String to " + o->Class());
      return false; // not executed, as Error teminates program
    } // else
  } // String::IsEqualTo

  bool String::IsLessThan(const Object *o) const {
    if (o->IsA("String")) {
      const String *strObj = dynamic_cast<const String*>(o);
      return cppStr < strObj->cppStr;
    } else {
      Error("invalid IsLessThan comparison of String to " + o->Class());
      return false; // not executed, as Error teminates program
    } // else
  } // String::IsLessThan


//--- new methods ---

  String::operator const char*() const {
    return cppStr.c_str();
  } // String::operator const char*

  String::operator string() const {
    return cppStr;
  } // String::operator string*

  const char *String::AsCString() const {
    return cppStr.c_str();
  } // String::AsCString

  int String::Length(void) const {
    return static_cast<int>(cppStr.length());
  } // String::Length

  String &String::Append(const String &s) {
    cppStr += s.cppStr;
    return *this;
  } // String::Append


//===  global overloaded append operators for String ===

  String operator+(const String &s1, const string &s2) {
    String s = s1;
    s.Append(s2);
    return s;
  } // operator+

  String operator+(const String &s1, const char *s2) {
    String s = s1;
    s.Append(s2);
    return s;
  } // operator+

  String operator+(const String &s1, const String &s2) {
    String s = s1;
    s.Append(s2);
    return s;
  } // operator+

  String operator+(const String &s1, int i) {
    String s = s1;
    s.Append(String(i));
    return s;
  } // operator+

  String operator+(const String &s1, double d) {
    String s = s1;
    s.Append(String(d));
    return s;
  } // operator+


} // ML

//========================================================================
// end of file MLString.cpp
