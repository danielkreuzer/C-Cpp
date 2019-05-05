// OSBridge.h:                                         MiniLib V.4.5, 2009
// ----------
// Bridge to platform specific GUI operating system.
//========================================================================

#ifndef OSBridge_h
#define OSBridge_h

#ifdef __BORLANDC__
  typedef unsigned int WHandle;
  extern WHandle hInstance, hPrevInst;
  #define main SubMain
#endif


//--- possible kinds of events ---

  enum EventKind {idleEvent,
                  buttonDownEvent, buttonReleaseEvent, mouseMoveEvent,
                  keyEvent,
                  commandEvent,
                  redrawEvent, resizeEvent,
                  quitEvent};

//--- point within a window in pixel coordinates ---

  struct Point {
    int x, y;
  }; // Point

//--- plattform dependent window descritption ---

  struct WindowPeer; // opaque type

//--- event data structure ---

  struct Event {
    EventKind   kind;
    WindowPeer *wp;
    Point       pos;        // for button(Down|Release)Event, mouseMoveEvent
    int         commandNr;  // for commandEvent
    char        key;        // for charEvent
    int         exitCode;   // for quitEvent
  }; // Event


//--- 1. application utilities ---

  void OSB_InitApplication(const char *applName);
  // initializes window system and creates GUI application

  void OSB_DestroyApplication();
  // terminates application

  // endless loop that makes callbacks to the event handler function
  void OSB_MainEventLoop(void (*eventHandler)(Event& e));
  
	// interrupts OSB_MainEventLoop
	void OSB_QuitMainEventLoop();

//--- 2. menu management utilities ---

  int OSB_NewMenuCommand(const char *menu, const char *cmd, char shortCut);
  // creates new command in menu and returns its command number

  void OSB_InstallMenuBar();
  // attatches menu bar to application window

  void OSB_RemoveMenuBar();
  // removes menu bar from application window


//--- 3. window management utilities ---

  void OSB_GetMouseState(const WindowPeer *wp,
                         bool &buttonPressed, Point &pos);
  // returns if mouse button is pressed and mouse postion

  void OSB_ShowMessageWindow(const char *title, const char *message);
  // opens modal dialog window with message and OK button

  bool OSB_EqualWindowPeers(const WindowPeer *wp1, const WindowPeer *wp2);
  // compares two window peers and returns true if they are equal

  void OSB_CreateNewWindow(char *title, WindowPeer *&wp);
  // creates and opens new window

  void OSB_DestroyOldWindow(WindowPeer *&wp);
  // closes and destroys old window

  WindowPeer *OSB_ActiveWindowPeer();
  // returns active windows peer or 0

  bool OSB_IsVisible(const WindowPeer *wp);
  // returns true if window is visible, iconized windows are invisible

  void OSB_EraseContent(const WindowPeer *wp);
  // erases content of window

  void OSB_GetContent(const WindowPeer *wp, Point &topLeft, int &w, int &h);
  // calculates content rectangle of window

  void OSB_InvalRect(const WindowPeer *wp, Point topLeft, int w, int h);
  // invalidates content of rectangle


//--- 4. drawing utilities ---

  void OSB_DrawDot(const WindowPeer *wp,
                   Point pos);
  // draws dot at position pos

  void OSB_DrawLine(const WindowPeer *wp,
                    Point startPos, Point endPos, int t);
  // draws line from start to end with thickness

  void OSB_DrawRectangle(const WindowPeer *wp,
                         Point topLeft, int w, int h, int t, bool fill);
  // draws (filled) rectangle from topLeft with width, height, and thickness

  void OSB_DrawOval(const WindowPeer *wp,
                    Point topLeft, int w, int h, int t, bool fill);
  // draws (filled) oval from topLeft with width, height, and thickness

  void OSB_DrawString(const WindowPeer *wp,
                      Point pos, const char *str, int size);
  // draws string with fontsize at position


#endif

//========================================================================
// end of file OSBridge.h


