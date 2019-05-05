// MLApplication.h:                                      MiniLib V.4, 2004
// ---------------
// Declaration of class Application.
//========================================================================

#ifndef MLApplication_h
#define MLApplication_h

#include <string>

#include "OSBridge.h"
#include "MLObject.h"
#include "MLVector.h"
#include "MLWindow.h"


namespace ML {


//=== command numbers for default menus ===

  extern int
  /*file menu:*/
    newCommand, closeCommand, quitCommand,
  /*edit menu:*/
    undoCommand, cutCommand, copyCommand, pasteCommand, clearCommand,
  /*window menu:*/
    closeAllCommand,
  /*help menu:*/
    aboutCommand;


//=== functions to display message window and install menu command ===

  void ShowMessageWindow(const char *title, const char *message);

  int NewMenuCommand(const char *menu, const char *cmd, char shortCut);


//=== class Application ===

  class Application: public Object {

    friend void Window::Open();  // allows call to AddWindow
    friend void Window::Close(); // allows call to RemoveWindow

  private:

    std::string  applName;
    bool    running;
    Vector *openWindows;

    void AddWindow   (Window *w); // called form Window::Open()
    void RemoveWindow(Window *w); // called form Window::Close()
    void DispatchEvent(Event e);

  public:

    static Application* instance; // the one and only appl. object

    Application(const std::string &applName = "MiniLib Application");
    virtual ~Application();

//--- override to build application specific menus ---

    virtual void BuildMenus(); // using calls to NewMenuCommand

//--- run and quit methods ---

    virtual void Run();
    virtual void Quit();

//--- window handling methods ---

    virtual void OpenNewWindow();
    virtual Window *WindowOf(const WindowPeer *wp);
    virtual Window *TopWindow();
    virtual Iterator *NewOpenWindowsIterator() const;
    virtual void CloseAllOpenWindows();

//--- event handling methods ---

    virtual void OnIdle();
    virtual void OnMousePressed (Point pos);
    virtual void OnMouseReleased(Point pos);
    virtual void OnMouseMove    (Point pos);
    virtual void OnKey(char key);
    virtual void OnCommand(int commandNr);

  }; // Application

} // ML


#endif

//========================================================================
// end of file MLApplication.h

