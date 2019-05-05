// MetaInfo.cpp:                                         MiniLib V.4, 2004
// ------------
// Meta information (abstract data structure).
//========================================================================

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
using namespace std;

#include "MLObject.h"
using namespace ML;

#include "MetaInfo.h"

//--- constants

  const int    errorIndex =  -1;    // for non-existing classes or none
  const int    maxClasses = 100;    // max. number of classes
  const string none       = "NONE"; // non-existing base class of Object


//--- type for representation of information for classes ---

  struct Class {
    string     className;        // name of class
    int        baseClassIndex;   // index of base class in array classes
    int        objsCreated;      // number of dynamic objects created
    int        objsDeleted;      // number of dynamic objects deleted
  }; // Class


//--- modul global data ---

  // init. both globals, possibly reset in Application::Application
  ApplicationKind  applKind      = consoleApplication;
  ShowMsgWindowPtr showMsgWindow = 0;


//--- data for collection meta information on classes their objects ---

  static int   nrOfClasses = 0;     // number of classes registered
  static Class classes[maxClasses]; // classes in [0 .. nrOfClasses - 1]
  static void* newObjAddr  = 0;     // address of newly allocated object


//--- utility functions used only within this module ---

  static int IndexOf(const string &className) {
    if (className == none)
      return errorIndex;
    int i = nrOfClasses - 1;
    while ( (i >= 0) && (classes[i].className != className) ) {
      i--;
    } // while
    return i; // if not found, returns errorIndex == -1
  } // IndexOf

  static void WriteMetaInfoRec(ostream &out, int baseClassIndex, int indent,
                               int &objectsStillAlive) {
    int aliveSum = 0;
    for (int i = 0; i < nrOfClasses; i++) {
      if (classes[i].baseClassIndex == baseClassIndex) {
        int space = 23 - indent;
        out << " " << string(indent, ' ');
        if ((int)classes[i].className.length() > space) { // name to long, trancate
          out << classes[i].className.substr(0, space - 3);
          out << ".. ";
       } else {
         out << classes[i].className;
         out << string(space - classes[i].className.length(), ' ');
       } // else
        out << "| " <<  setw(7) << classes[i].objsCreated;
        out << " | " << setw(7) << classes[i].objsDeleted;
        int alive = classes[i].objsCreated - classes[i].objsDeleted;
        out << " | " << setw(11);
        if (alive < 0)
          out << alive << " ERROR";
        else { // alive >= 0
          out << alive;
          aliveSum += alive;
        } // else
        out << endl;
        WriteMetaInfoRec(out, i, indent + 2, alive);
        aliveSum += alive;
      } // if
    } // for
    objectsStillAlive = aliveSum;
  } // WriteMetaInfoRec


//=== "public" functions ===

  void WriteMetaInfo(ostream &out) {
    int objectsStillAlive = 0;
    out << endl;
    out << endl;
    out << "==========================================================" << endl;
    out << " Meta information for MiniLib application                 " << endl;
    out << "------------------------+---------------------------------" << endl;
    out << " Class hierarchy        | Number of dynamic objects       " << endl;
    out << "                        +---------+---------+-------------" << endl;
    out << "                        | created | deleted | still alive " << endl;
    out << "------------------------+---------+---------+-------------" << endl;
    WriteMetaInfoRec(out, errorIndex, 0, objectsStillAlive); // start with none
    out << "------------------------+---------+---------+-------------" << endl;
    out << " Number of classes: " << setw(2) << nrOfClasses << "  | Summary: ";
    if (objectsStillAlive == 0)
      out << "all objects deleted";
    else
      out << objectsStillAlive << " object(s) still alive";
    out << endl;
    out << "==========================================================" << endl;
    out << endl;
  } // WriteMetaInfo

  void Error(const string &message) {
    if ( (applKind == consoleApplication) ||
          showMsgWindow == 0) {
      cerr << endl << endl << "*** ERROR: " << message << endl;
      cerr << "\a"; // beep
      cerr << "enter [CR] to continue ...";
      char ch;
      cin.get(ch);
    } else { // (applKind == guiApplication) &&
             // (showMsgWindow != 0)
      (*showMsgWindow)("*** ERROR ***", message.c_str());
    } // if
    exit(1);
  } // Error


//=== "private" functions (used within class Object only) ===

  void MI_RememberAddr(void *objAddr) {
    if (objAddr == 0)
      Error("invalid object addresss in MetaInfo.MI_RememberAddr");
    newObjAddr = objAddr;
  } // MI_RememberAddr

  void MI_Register(const string &className, const string &baseClassName,
                   void *objAddr) {
    int baseClassIndex = IndexOf(baseClassName);
    if ( (baseClassIndex == errorIndex) && (baseClassName != none) )
      Error(string("base class ") + baseClassName +
            " of class " + className + " not registered in MetaInfo");
    int classIndex = IndexOf(className);
    if (classIndex == errorIndex) { // new class
      if (nrOfClasses == maxClasses)
        Error("too many classes registered in MetaInfo");
      classIndex = nrOfClasses;
      nrOfClasses++;
      classes[classIndex].className      = className;
      classes[classIndex].baseClassIndex = baseClassIndex;
      // objsCreated and objsDeleted already set to 0
    } // if
    // newObjAddr has been set in Object::operator new
    Object *obj = (Object*)objAddr;
    if (newObjAddr == objAddr) {
      obj->isDynamic = true;
      newObjAddr = 0;
    } // if
    if (obj->isDynamic) {
      if (baseClassIndex != errorIndex)
        classes[baseClassIndex].objsCreated--;
      classes[classIndex].objsCreated++;
    } // if
    obj->classIndex = classIndex;
  } // MI_Register

  void MI_IncCreationCounter(int index) {
    if (index >= 0 && index < nrOfClasses)
      classes[index].objsCreated++;
    else
      Error("invald class index in MetaInfo");
  } // MI_IncCreationCounter

  void MI_IncDeletionCounter(int index) {
    if (index >= 0 && index < nrOfClasses)
      classes[index].objsDeleted++;
    else
      Error("invald class index in MetaInfo");
  } // MI_IncDeletionCounter

  string MI_ClassNameOf(int index)  {
    if (index == errorIndex)
      return none;
    else if ( index >= 0 && index <= nrOfClasses)
      return classes[index].className;
    else {
      Error("invalid class index in MetaInfo.MI_ClassNameOf");
      return string(); // not executed, as Error teminates program
    } // else
  } // MI_ClassNameOf

  string MI_BaseClassNameOf(int index)  {
    if (index > 0 && index < nrOfClasses)
      return MI_ClassNameOf(classes[index].baseClassIndex);
    else {
      Error("invalid class index in MetaInfo.MI_BaseClassNameOf");
      return string(); // not executed, as Error teminates program
    } // else
  } // MI_BaseClassNameOf

  string MI_BaseClassNameOf(const string &className)  {
    int classIndex = IndexOf(className);
    if (classIndex == errorIndex)
      Error("class not registered in MetaInfo");
    return MI_BaseClassNameOf(classIndex);
  } // MI_BaseClassNameOf

  bool MI_IsEqualToOrDerivedFrom(const string &className,
                                 const string &baseClassName) {

    int classIndex = IndexOf(className);
    if (classIndex == errorIndex)
      Error("class " + className + " not registered in MetaInfo");

    int baseClassIndex = IndexOf(baseClassName);
    // Attention: when baseClassIndex == errorIndex there are two possibilites
    //   1. this class exists, but no objects have been created up to now 
    //   2. there is no class with this name
    // let's be pessimistic, we assume the second possibility
    if (baseClassIndex == errorIndex)
      Error("class " + baseClassName + " not registered in MetaInfo");

    if (classIndex == baseClassIndex)
      return true;
    while ( classIndex != errorIndex &&
            classes[classIndex].className != baseClassName )
      classIndex = classes[classIndex].baseClassIndex;
    return classIndex != errorIndex;
  } // MI_IsEqualToOrDerivedFrom


//========================================================================
// end of file MetaInfo.cpp
