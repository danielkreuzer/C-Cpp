// MLString.h:                                           MiniLib V.4, 2004
// ----------
// Declaration of String, the MiniLib string class.
//========================================================================

#ifndef MLString_h
#define MLString_h

#include <iosfwd>
#include <string>

#include "MLObject.h"

namespace ML {


//=== class String ===

  class String: public Object {

    friend std::istream &operator>>(std::istream &is, String &s);
    // provides access to private data

  protected:

    std::string cppStr;

  public:

    String();
    String(const char        *s); // C string
    String(const std::string &s); // C++ std::string
    String(const String      &s); // MiniLib string
    String(      int         i);  // init. with digit sequence of i
    String(      double      d);  // init. with digit sequence of d

    virtual ~String();

//--- overridden methods ---

    virtual std::string AsString() const;
    // returns String value as std::string ;-)

    virtual bool IsEqualTo(const Object *o) const;
    // returns this->cppStr == obj->cppStr

    virtual bool IsLessThan(const Object *o) const;
    // returns this->cppStr < obj->cppStr

//--- new methods ---

    virtual operator const char*() const;
    // casts from String to const char*

    virtual operator std::string() const;
    // casts from String to std::string

    virtual const char *AsCString() const;
    // returns String value as C string

    virtual int Length(void) const;
    // returns length of String

    virtual String &Append(const String &s);
    // appends s to String

  }; // String


//=== global overloaded append operators for String ===

  String operator+(const String &s1, const char   *s2);
  String operator+(const String &s1, const std::string &s2);
  String operator+(const String &s1, const std::string &s2);
  String operator+(const String &s1, int            i);
  String operator+(const String &s1, double         d);

} // ML


#endif

//========================================================================
// end of file MLString.h
