// MLApplication.cpp:                                  MiniLib V.4.5, 2009
// -----------------
// Implementation of class Application.
//========================================================================

#include <cstdlib>
#include <memory>
using namespace std;

#include "MetaInfo.h"
#include "MLApplication.h"

namespace ML {


  int
  /*file menu:*/
    newCommand, closeCommand, quitCommand,
  /*edit menu:*/
    undoCommand, cutCommand, copyCommand, pasteCommand, clearCommand,
  /*window menu:*/
    closeAllCommand,
  /*help menu:*/
    aboutCommand;


  void ShowMessageWindow(const char *title, const char *message) {
    OSB_ShowMessageWindow(title, message);
  } // ShowMessageWindow

  int NewMenuCommand(const char *menu, const char *cmd, char shortCut) {
    return OSB_NewMenuCommand(menu, cmd, shortCut);
  } // NewMenuCommand


  static void BuildFileAndEditMenus() {
  // file menu:
    newCommand =      NewMenuCommand("File",   "New",       'N');
    closeCommand =    NewMenuCommand("File",   "Close",     'W');
                      NewMenuCommand("File",   "-",         ' ');
    quitCommand =     NewMenuCommand("File",   "Quit",      'Q');
  // edit menu:
    undoCommand =     NewMenuCommand("Edit",   "Undo",      'Z');
                      NewMenuCommand("Edit",   "-",         ' ');
    cutCommand =      NewMenuCommand("Edit",   "Cut",       'X');
    copyCommand =     NewMenuCommand("Edit",   "Copy",      'C');
    pasteCommand =    NewMenuCommand("Edit",   "Paste",     'V');
    clearCommand =    NewMenuCommand("Edit",   "Clear",     ' ');
  } // Application::BuildFileAndEditMenus


  static void BuildWindowAndHelpMenus() {
  // window menu: Cascade, Tile, and Arrange icons in OSBridge
    closeAllCommand = NewMenuCommand("Window", "Close All", ' ');
  // help menu:
    aboutCommand =    NewMenuCommand("Help",   "About ...", ' ');
  } // BuildWindowAndHelpMenus


//=== calss Application ===


  void Application::AddWindow(Window *w) {
    openWindows->Add(w);
  } // Application::AddWindow

  void Application::RemoveWindow(Window *w) {
    openWindows->Remove(w);
  } // Application::RemoveWindow


	Application *Application::instance = 0;


	// handler function that gets invoked when events occur
	static void EventHandler(Event& e) {
		Application* app = Application::instance;
    if (app == 0)
			 Error("Event occurren when no Application object was instantiated");

    switch (e.kind) {
      case idleEvent:
				app->OnIdle();
        break;
      case buttonDownEvent:
        app->OnMousePressed(e.pos);
        break;
      case buttonReleaseEvent:
        app->OnMouseReleased(e.pos);
        break;
      case mouseMoveEvent:
        app->OnMouseMove(e.pos);
        break;
      case keyEvent:
        app->OnKey(e.key);
        break;
      case commandEvent:
        app->OnCommand(e.commandNr);
        break;
      case resizeEvent:
        app->WindowOf(e.wp)->OnResize();
        break;
      case redrawEvent:
        app->WindowOf(e.wp)->OnRedraw();
        break;
      case quitEvent:
        app->Quit();
        break;
      default:
        ; // nothing to do
    } // switch
  } // Application::DispatchEvent


  Application::Application(const string &applName)
  : applName(applName) {
    Register("Application", "Object");
    // check if singleton
    if (instance == 0)
      instance = this;
    else
      Error("invalid attempt to construct another Application object");
    // reset global variables defined in MetaInfo
    applKind      = guiApplication;
    showMsgWindow = &OSB_ShowMessageWindow;
    // finish object initialization
    openWindows = new Vector();
    // initialize MS Windows application
    OSB_InitApplication(const_cast<char*>(applName.c_str()));
  } // Application::Application

  Application::~Application() {
    instance = 0;
    delete openWindows;
    OSB_DestroyApplication();
  } //Application::~Application


//--- override to build application specific menus ---

  void Application::BuildMenus(){
    // implementations in derived classes call NewMenuCommand
  } // Application::BuildMenus


//--- run and quit mehtods ---

  void Application::Run() {
    BuildFileAndEditMenus();
    BuildMenus(); // call method in derived Application class
    BuildWindowAndHelpMenus();
    OSB_InstallMenuBar();
    OpenNewWindow();
    
		// invoke the main loop
    OSB_MainEventLoop(&EventHandler);
  } // Application::Run

  void Application::Quit() {
    OSB_RemoveMenuBar();
		CloseAllOpenWindows();
		OSB_QuitMainEventLoop();
  } // Application::Quit


//--- window hanlding methods ---

  void Application::OpenNewWindow() {
    (new Window("MiniLib Window"))->Open();
  } // Application::OpenNewWindow

  Window *Application::WindowOf(const WindowPeer *wp) {
    if (wp == 0)
      return 0;
    unique_ptr<Iterator> it(openWindows->NewIterator());
    Window *w = dynamic_cast<Window*>(it->Next());
    while (w != 0) {
      if (OSB_EqualWindowPeers(w->GetWindowPeer(), wp))
        return w;
      w = dynamic_cast<Window*>(it->Next());
    } // while
    return 0;
  } // Application::WindowOf

  Window *Application::TopWindow() {
    return WindowOf(OSB_ActiveWindowPeer());
  } // Application::TopWindow

   Iterator *Application::NewOpenWindowsIterator() const {
    return openWindows->NewIterator();
  } // Application::NewOpenWindowsIterator

  void Application::CloseAllOpenWindows() {
    int i = openWindows->Size() - 1;
    while (i >= 0) {
      Window *w = dynamic_cast<Window*>(openWindows->GetAt(i));
      delete w; // removes w from openWindows, closes and deletes it
      i--;
    } // while
  } // Application::CloseAllOpenWindows


 //--- event handling methods ---

  void Application::OnIdle() {
    unique_ptr<Iterator> it(openWindows->NewIterator());
    Window *w = dynamic_cast<Window*>(it->Next());
    while (w != 0) {
      w->OnIdle();
      w = dynamic_cast<Window*>(it->Next());
    } // while
  } // Application::OnIdle

  void Application::OnMousePressed(Point pos) {
    Window *w = TopWindow();
    if (w != 0)
      w->OnMousePressed(pos);
  } // Application::OnMousePressed

  void Application::OnMouseReleased(Point pos) {
    Window *w = TopWindow();
    if (w != 0)
      w->OnMouseReleased(pos);
  } // Application::OnMouseReleased

  void Application::OnMouseMove(Point pos) {
    Window *w = TopWindow();
    if (w != 0)
      w->OnMouseMove(pos);
  } // Application::OnMouseMove

  void Application::OnKey(char key) {
    Window *w = TopWindow();
    if (w != 0)
      w->OnKey(key);
  } // Application::OnKey

  void Application::OnCommand(int commandNr) {
    if (commandNr == quitCommand)
      Quit();
    else if (commandNr == closeAllCommand)
      CloseAllOpenWindows();
    else if (commandNr == aboutCommand) {
      string aboutMsg = applName + "\nbased on the famous application framework MiniLib" +
                                   "\n-- Version 4.5 (in C++), 2009";
      OSB_ShowMessageWindow("About Window", aboutMsg.c_str());
    } else if (commandNr == newCommand)
      OpenNewWindow();
    else {
      Window *w = TopWindow();
      if (w != 0)
        w->OnCommand(commandNr);
    } // else
  } // Application::OnCommand


} // ML

//========================================================================
// end of file MLApplication.cpp
