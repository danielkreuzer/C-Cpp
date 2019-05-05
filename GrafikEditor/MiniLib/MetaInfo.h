// MetaInfo.h:                                           MiniLib V.4, 2004
// ----------
// Meta information (abstract data structure).
//========================================================================

#ifndef MetaInfo_h
#define MetaInfo_h

#include <iostream>
#include <string>


  enum ApplicationKind {consoleApplication, guiApplication};
  typedef void (*ShowMsgWindowPtr)(const char *title, const char *message);


//=== global variables ===

  extern ApplicationKind  applKind;
  extern ShowMsgWindowPtr showMsgWindow;


//=== public functions ===

  void WriteMetaInfo(std::ostream &out = std::cout);
  // writes meta info: class hierarchy and object counters

  void Error(const std::string &message);
  // writes error message to console or opens message window
  //   and terminates program execution


 //=== private functions (used within class Object only) ===

  void MI_RememberAddr(void *objAddr);
  // remembers address of object for later fixup in MI_Register

  void MI_Register(const std::string &className, const std::string &baseClassName,
                   void *objAddr);
  // registers class and increases object creation counter

  void MI_IncCreationCounter(int index);
  // increases object creation counter of class with index

  void MI_IncDeletionCounter(int index);
  // increases object deletion counter of class with index

  std::string MI_ClassNameOf(int index);
  // returns class name for class with index

  std::string MI_BaseClassNameOf(int index);
  // returns class name for base class of class with index

  bool MI_IsEqualToOrDerivedFrom(const std::string &className,
                                 const std::string &baseClassName);
  // returns true if there is an inheritance relationship between ...


#endif

//========================================================================
// end of file MetaInfo.h
