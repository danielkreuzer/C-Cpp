// OSBridge2Win32.cpp:                                 MiniLib V.4.5, 2009
// ------------------
// Bridge to platform specific GUI operating system (MS Windows 32-bit).
//========================================================================

#include <cstring>
#include <cctype>
#include <fstream>
#include <fstream>
#include <set>
#include <vector>
#include <set>
#include <algorithm>

#include <Windows.h>
#include <Windowsx.h>

#include "MetaInfo.h"
#include "MLWindow.h"
#include "MLApplication.h"

using namespace std;
using namespace ML;

#include "OSBridge.h"



  const int firstMenuCmd   = 100;
  const int maxMenus       =  20;
  const int maxMenuNameLen =  31;
  const int firstChildId   = 300; // id of first child window of appl. window

  enum DrawColor {black, white};
  enum DrawMode  {addMode, invertMode};

  struct WindowPeer {
    HWND wnd; // handle to window
    HDC  dc;  // handle to display context
  }; // struct

  struct MenuItemDescr {
		MenuItemDescr(const string& command, char shortcut, int cmdId) : 
	                command(command), shortcut(shortcut), cmdId(cmdId) {}
	  string command;       // menu item text
    char   shortcut;      // command shortcut
    int    cmdId;         // unique id of menu item
	};

  struct MenuDescr {
		MenuDescr(const string& title) : title(title) {}
		bool operator==(const MenuDescr md) { return title == md.title; }

    string                title;         // menubar title
    vector<MenuItemDescr> items;
		set<char>             usedMnemonics;
  }; // struct


  //--- module golbals ---

  static HMENU     hMenuBar = 0;
  static HWND      appFrameWnd, appClientWnd;
  static HANDLE    hInstance, hPrevInst;

	static vector<MenuDescr> menus;
  static set<char>         usedMnemonics;
  static int               lastMenuCmd = 0;  
	static int               cascadeCommand, tileCommand, arrangeIconCommand;
  static bool              gButtonPressed, windowMenuFirst;


  //--- WinMain function ---

  extern int main(int argc, char* argv[]);

  int pascal WinMain(HINSTANCE hInst, HINSTANCE hPInst,
                     LPSTR /* lpCmdLine */, int /* nShowCmd */) {

    ofstream ofs("CoutCerr.txt");
    ofs << "CoutCerr.txt: file collecting all output via cout and cerr:" << endl;
    ofs << "----------------------------------------------------------"  << endl;
    ofs << endl;

    streambuf *coutBuffer = cout.rdbuf();
    cout.rdbuf(ofs.rdbuf()); // redirect output form cout to ofs
    streambuf *cerrBuffer = cerr.rdbuf();
    cerr.rdbuf(ofs.rdbuf()); // redirect output from cerr to ofs

    hInstance = hInst;
    hPrevInst = hPInst;

    int rc = main(0, 0); // call the "real" main function

    cout.rdbuf(coutBuffer);
    cerr.rdbuf(cerrBuffer);

    ofs.close();

    return rc;
  } // WinMain


  //--- assert correct operation result --

  static void AssertOperation(bool opSuccess, const char *operation) {
    char msg[256];
    if (!opSuccess) {
      strcpy_s(msg, "operation ");
      strcat_s(msg, operation);
      strcat_s(msg, " failed");
      Error(msg);
    } // if
  } /*AssertOperation*/

  //=== local window management utilities ===

  static LRESULT CALLBACK FrameWndProc(HWND wnd, UINT message,
                                       WPARAM wParam, LPARAM lParam) {
    switch (message) {
      case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
      default:
        ; // nothing to do
    } // switch
    return DefFrameProc(wnd, appClientWnd, message, wParam, lParam);
  } // FrameWndProc

  static LRESULT CALLBACK ChildWndProc(HWND wnd, UINT message,
                                       WPARAM wParam, LPARAM lParam) {
    Window    *w;
    WindowPeer wp;
    wp.wnd = wnd;
    switch (message) {
      case WM_PAINT:
        if (Application::instance != 0) {
          w = Application::instance->WindowOf(&wp);
          if (w != 0) {
            w->Update();
            ValidateRect(wp.wnd, 0);
            return 0;
          } // if
        } // if
        break;
      case WM_SIZE:
        if (Application::instance != 0) {
          w = Application::instance->WindowOf(&wp);
          if (w != 0)
            w->OnResize();
        } // if
        break;
      case WM_CLOSE:
        if (Application::instance != 0) {
          w = Application::instance->WindowOf(&wp);
          if (w != 0)
            delete w;
        } // if
        break;
      default:
        ; // nothing to do
    } // switch
    return DefMDIChildProc(wnd, message, wParam, lParam);
  } // ChildWndProc

  static void RegisterFrameWindowKind() {
    WNDCLASS wndClass;
    if (hPrevInst != 0)
      return;
    wndClass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc   = FrameWndProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = (struct HINSTANCE__ *)hInstance;
    wndClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = HBRUSH(COLOR_APPWORKSPACE);
    wndClass.lpszMenuName  = 0;
    wndClass.lpszClassName = "FrameWndKind";
    AssertOperation(RegisterClass(&wndClass) != 0, "RegisterClass");
  } // RegisterFrameWindowKind

  static void RegisterChildWindowKind() {
    WNDCLASS wndClass;
    if (hPrevInst != 0)
      return;
    wndClass.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc   = ChildWndProc;
    wndClass.cbClsExtra    = 0;
    wndClass.cbWndExtra    = 0;
    wndClass.hInstance     = (struct HINSTANCE__ *)hInstance;
    wndClass.hIcon         = LoadIcon(0, IDI_APPLICATION);
    wndClass.hCursor       = LoadCursor(0, IDC_ARROW);
    wndClass.hbrBackground = (struct HBRUSH__ *)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName  = 0;
    wndClass.lpszClassName = "ChildWndKind";
    AssertOperation(RegisterClass(&wndClass) != 0, "RegisterClass");
  } // RegisterChildWindowKind

  static HWND CreateFrameWindow(char *title) {
    HWND wnd;
    wnd = CreateWindow("FrameWndKind", title,
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
                        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                        HWND_DESKTOP, 0, (struct HINSTANCE__ *)hInstance, 0);
    return wnd;
  } // CreateFrameWindow

  static HWND CreateClientWindow(HWND frameWnd) {
    HWND wnd;
    CLIENTCREATESTRUCT clientCreate;
    clientCreate.hWindowMenu  = 0;
    clientCreate.idFirstChild = firstChildId;
    wnd = CreateWindow("MDICLIENT", 0,
                       WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
                       0, 0, 0, 0,
                       frameWnd, 0, (struct HINSTANCE__ *)hInstance, &clientCreate);
    ShowWindow(frameWnd, SW_SHOWNORMAL);
    UpdateWindow(frameWnd);
    return wnd;
  } // CreateClientWindow

  static HWND CreateChildWindow(char *title, HWND clientWnd) {
    HWND wnd;
    MDICREATESTRUCT mdiCreate;
    mdiCreate.szClass = "ChildWndKind";
    mdiCreate.szTitle = title;
    mdiCreate.hOwner  = hInstance;
    mdiCreate.x       = CW_USEDEFAULT;
    mdiCreate.y       = CW_USEDEFAULT;
    mdiCreate.cx      = CW_USEDEFAULT;
    mdiCreate.cy      = CW_USEDEFAULT;
    mdiCreate.style   = 0;
    mdiCreate.lParam  = 0;
    wnd = reinterpret_cast<HWND>(SendMessage(clientWnd, WM_MDICREATE, 0,
                                 reinterpret_cast<WPARAM>(&mdiCreate)));
    return wnd;
  } // CreateChildWindow


  //=== 1. application utilities ===

  void OSB_InitApplication(const char *applName) {
    RegisterFrameWindowKind();
    RegisterChildWindowKind();
    appFrameWnd     = CreateFrameWindow(const_cast<char*>(applName));
    appClientWnd    = CreateClientWindow(appFrameWnd);
    lastMenuCmd     = firstMenuCmd;
    windowMenuFirst = true;
    gButtonPressed  = false;
    SetTimer(0, 0, 1, 0); // to trigger idle events
  } // OSB_InitApplication

  void OSB_DestroyApplication() {
    CloseWindow(appFrameWnd);
    AssertOperation(::DestroyWindow(appFrameWnd) != 0, "DestroyWindow");
  } // OSB_DestroyApplication


  static int ShortCutCmdNr(short vKeyCode) {
    char ch;
    if ( (vKeyCode < 1) || (vKeyCode > 26) )
      return 0;
    ch = static_cast<char>(vKeyCode + 'A' - 1);

     for (unsigned int i=0; i<menus.size(); i++)
		 	 for (unsigned int j=0; j<menus[i].items.size(); j++)
				 if (menus[i].items[j].shortcut == ch)
					 return menus[i].items[j].cmdId;

			return 0;
		
			//for (int i = 0; i < menuBarItems.nr; i++) {
      //    for (int j = 0; j < menuItems[i].nr; j++) {
      //      if (menuItems[i].shortCuts[j] == ch)
      //        return menuItems[i].cmdId[j];
      //    } // for
      //  } // for
      // return 0;
  } // ShortCutCmdNr


  void OSB_MainEventLoop(void (*eventHandler)(Event& e)) {
    MSG        msg;
		Event      e;
    Window    *w;
    WindowPeer wp;
    int        cmdNr;
    bool       found;
		msg.hwnd = 0;
   
		do {
      found = false;
 
			GetMessage(&msg, 0, 0, 0);
      if (msg.message == WM_TIMER) {
        if (!InSendMessage() &&
            !PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
          // no other messages pending
          e.kind = idleEvent;
          found  = true;
        } // if
      } else {
        TranslateMessage(&msg);
        switch (msg.message) {
          case WM_LBUTTONDOWN:
						SetCapture(msg.hwnd);
            e.kind         = buttonDownEvent;
            e.pos.x        = GET_X_LPARAM(msg.lParam);
            e.pos.y        = GET_Y_LPARAM(msg.lParam);
            gButtonPressed = true;
            found          = true;
            break;
          case WM_LBUTTONUP:
						ReleaseCapture();
						e.kind         = buttonReleaseEvent;
            e.pos.x        = GET_X_LPARAM(msg.lParam);
            e.pos.y        = GET_Y_LPARAM(msg.lParam);
            gButtonPressed = false;
            found          = true;
            break;
          case WM_MOUSEMOVE:
            e.kind         = mouseMoveEvent;
            e.pos.x        = GET_X_LPARAM(msg.lParam);
            e.pos.y        = GET_Y_LPARAM(msg.lParam);
            found          = true;
            break;
          case WM_CHAR: {
            cmdNr = ShortCutCmdNr(static_cast<short>(msg.wParam));
            if (cmdNr == 0) {
              e.kind       = keyEvent;
              e.key        = static_cast<char>(msg.wParam);
            } else {
              e.kind       = commandEvent;
              e.commandNr  = cmdNr;
            } // else
            found          = true;
            break;
          } // case
          case WM_QUIT:
            e.kind         = quitEvent;
            e.exitCode     = (int)msg.wParam;
            found          = true;
            break;
          case WM_COMMAND:
            if (msg.wParam >= firstChildId) // activate child command
              DispatchMessage(&msg);
            else if (msg.wParam == static_cast<unsigned int>(cascadeCommand))
              SendMessage(appClientWnd, WM_MDICASCADE, 0, 0);
            else if (msg.wParam == static_cast<unsigned int>(tileCommand))
              SendMessage(appClientWnd, WM_MDITILE, 0, 0);
            else if (msg.wParam == static_cast<unsigned int>(arrangeIconCommand))
              SendMessage(appClientWnd, WM_MDIICONARRANGE, 0, 0);
            else {
              e.kind       = commandEvent;
              e.commandNr  = static_cast<long>(msg.wParam);
              found        = true;
            } // else
            break;
          default:
            DispatchMessage(&msg);
        } // else
      } // switch

			wp.wnd = msg.hwnd;
			w = Application::instance->WindowOf(&wp);
			if (w != 0)
				e.wp = w->GetWindowPeer();
			else
				e.wp = 0;

			if (found)
			  (*eventHandler)(e); // fire event

		} while (msg.message != WM_QUIT);
  } // OSB_GetNextEvent

	void OSB_QuitMainEventLoop() {
    PostQuitMessage(0);
	}

  //=== 2. menu management functions ===

	static void MarkMnemonic(string& menuText, set<char>& usedMnemonics) {
		// find first alphabetical character that is no already used as
		// a mnemonic
		unsigned int i;	 
		for (i=0; i<menuText.length() &&
							(! isalpha(menuText[i]) ||
							usedMnemonics.find(menuText[i]) != usedMnemonics.end());
				 i++);

		if (i < menuText.length()) {  // mnemonic char found?
			usedMnemonics.insert(menuText[i]);
			menuText.insert(i, "&"); 
	  }
	}

  static int InsertMenu(HMENU &hMenuBar, const string &menuTitle) {
		unsigned int pos;
		for (pos=0; pos<menus.size() && menus[pos].title != menuTitle; pos++);
		
		if (pos >= menus.size()) { // title not found
			 // insert menu entry
			menus.push_back(MenuDescr(menuTitle));

			string menuText = menuTitle;
			MarkMnemonic(menuText, usedMnemonics);
      
			AssertOperation(AppendMenu(hMenuBar, MF_ENABLED, lastMenuCmd, menuText.c_str()) != 0,
		                  "AppendMenu");
			lastMenuCmd++;
		}

		return pos;
  } // InsertMenu

	static int InsertMenuItem(HMENU &hMenu, MenuDescr &menu, const string &command, char shortcut) {
    if (command == "-") { // insert menu separator? 
       AssertOperation(AppendMenu(hMenu, MF_SEPARATOR, 0, 0) != 0,
                       "AppendMenu");
			 menu.items.push_back(MenuItemDescr("-", ' ', 0));
			 return menu.items.size() - 1;
		}
		    
		// find existing menu with this menuTitle
		unsigned int pos;
		for (pos=0; pos<menu.items.size() && menu.items[pos].command != command; pos++);
		
		if (pos >= menu.items.size()) { // title not found
			string cmdText = command;
			MarkMnemonic(cmdText, menu.usedMnemonics);

			shortcut = static_cast<char>(toupper(shortcut));
      if (isalpha(shortcut)) {
				cmdText.append("\tCtrl-");
				cmdText.append(1, shortcut);
      } 

			int cmdId = lastMenuCmd++;

			 // insert menu entry
			menu.items.push_back(MenuItemDescr(command, shortcut, cmdId));

			AssertOperation(AppendMenu(hMenu, MF_ENABLED, cmdId, cmdText.c_str()) != 0,
                        "AppendMenu");
		}
      
		return pos;
  } // InsertMenuItem

  int OSB_NewMenuCommand(const char *menuTitle, const char *cmd, char shortcut) {
    int menuBarPos, subMenuPos;
    HMENU hSubMenu;
    char menStr[256];

    if (appFrameWnd == 0)
      Error("menus allowed in an application only");
    
		if (hMenuBar == 0)
			hMenuBar = CreateMenu();

		if (windowMenuFirst && (strcmp(menuTitle, "Window") == 0)) {
      windowMenuFirst    = false;
      cascadeCommand     = OSB_NewMenuCommand("Window", "Cascade",       ' ');
      tileCommand        = OSB_NewMenuCommand("Window", "Tile",          ' ');
      arrangeIconCommand = OSB_NewMenuCommand("Window", "Arrange Icons", ' ');

    } // if
    
		menuBarPos = InsertMenu(hMenuBar, menuTitle);
    hSubMenu    = GetSubMenu(hMenuBar, menuBarPos);
    if (hSubMenu == 0)  {
      hSubMenu  = CreatePopupMenu();
      GetMenuString(hMenuBar, menuBarPos, menStr, 256, MF_BYPOSITION);
      ModifyMenu(hMenuBar, menuBarPos, MF_BYPOSITION | MF_POPUP,
                 reinterpret_cast<UINT_PTR>(hSubMenu),
                 menStr);
    } // if
    
		subMenuPos = InsertMenuItem(hSubMenu, menus[menuBarPos], cmd, shortcut);
    return GetMenuItemID(hSubMenu, subMenuPos);
  } // OSB_NewMenuCommand

  void OSB_InstallMenuBar() {
    AssertOperation(SetMenu(appFrameWnd, hMenuBar) != 0, "SetMenu");
    SendMessage(appClientWnd, WM_MDISETMENU, 0,
			MAKELPARAM(hMenuBar, GetSubMenu(hMenuBar, menus.size() - 2)));
    DrawMenuBar(appFrameWnd);
  } // OSB_InstallMenuBar

  void OSB_RemoveMenuBar() {
    // nothing to do
  } // OSB_RemoveMenuBar


  // === 3. window management utilities ===

  void OSB_GetMouseState(const WindowPeer *wp,
                         bool &buttonPressed, Point &pos) {
    MSG   msg;
    POINT p;
    buttonPressed = gButtonPressed;
    if (appFrameWnd == GetActiveWindow()) {
      SetCapture(wp->wnd);
      if (PeekMessage(&msg, 0, WM_LBUTTONUP, WM_LBUTTONUP, PM_NOREMOVE))
        buttonPressed = false;
      GetCursorPos(&p); // in screen coordinates
      ScreenToClient(wp->wnd, &p);
      ReleaseCapture();
      pos.x = p.x;
      pos.y = p.y;
    } // if
  } // OSB_GetMouseState

  void OSB_ShowMessageWindow(const char *title, const char *message) {
    MessageBox(GetFocus(), message, title, MB_OK);
  } // OSB_ShowMessageWindow

  bool OSB_EqualWindowPeers(const WindowPeer *wp1, const WindowPeer *wp2) {
    return ( (wp1 == wp2) ||
             ( (wp1 != 0) && (wp2 != 0) &&
               (wp1->wnd == wp2->wnd) ) );
  } // OSB_EqualWindowPeers

  void OSB_CreateNewWindow(char *title, WindowPeer *&wp) {
    if (wp == 0) {
      wp = new WindowPeer;
      wp->wnd = reinterpret_cast<HWND>(CreateChildWindow(title, appClientWnd));
    } // if
    wp->dc = GetDC(wp->wnd);
    ShowWindow(wp->wnd, SW_SHOWNORMAL);
    UpdateWindow(wp->wnd);
  } // OSB_CreateNewWindow

  void OSB_DestroyOldWindow(WindowPeer *&wp) {
    HWND oldWnd;
    if (wp != 0) {
      oldWnd  = wp->wnd;
      wp->wnd = 0;
      CloseWindow(oldWnd);
      SendMessage(appClientWnd, WM_MDIDESTROY, (WPARAM)oldWnd, 0);
      // delete wp; not necessary
    } // if
  } // OSB_DestroyOldWindow

  static WindowPeer *TempWindowPeer() {
    const int wpArrMax = 10;
    static WindowPeer wpArr[wpArrMax];
    static int wpArrNextFree = 0;
    wpArrNextFree = (wpArrNextFree + 1) % wpArrMax;
    return &(wpArr[wpArrNextFree]);
  } // TempWindowPeer

  WindowPeer *OSB_ActiveWindowPeer() {
    WindowPeer *wp;
    wp      = TempWindowPeer();
    wp->wnd = (HWND)SendMessage(appClientWnd, WM_MDIGETACTIVE, 0, 0);
    return wp;
  } // OSB_ActiveWindowPeer

  bool OSB_IsVisible(const WindowPeer *wp) {
    return (IsWindowVisible(wp->wnd) != 0);
  } // OSB_IsVisible

  void OSB_EraseContent(const WindowPeer *wp) {
    RECT   r;
    HANDLE brush;
    GetClientRect(wp->wnd, &r);
    brush = GetStockObject(WHITE_BRUSH);
    FillRect(wp->dc, &r, (struct HBRUSH__ *)brush);
  } // OSB_EraseContent

  void OSB_GetContent(const WindowPeer *wp,
                      Point &topLeft, int &w, int &h) {
    RECT r;
    GetClientRect(wp->wnd, &r);
    topLeft.x = r.left;
    topLeft.y = r.top;
    w         = r.right  - r.left;
    h         = r.bottom - r.top;
  } // OSB_GetContent

  void OSB_InvalRect(const WindowPeer *wp,
                     Point topLeft, int w, int h) {
    RECT r;
    SetRect(&r, topLeft.x, topLeft.y, topLeft.x + w, topLeft.y + h);
    InvalidateRect(wp->wnd, &r, false);
    UpdateWindow(wp->wnd);
  } // OSB_InvalRect


  // === 4. drawing utilities ===

  static unsigned long MsWindowsColorOf(DrawColor c) {
    switch (c) {
      case black:
        return RGB(  0,   0,   0);
      case white:
        return RGB(255, 255, 255);
      default:
        return RGB(127, 127, 127);
    } // switch
  } // MsWindowsColorOf

  void OSB_DrawDot(const WindowPeer *wp,
                   Point pos) {
    HPEN          linePen;
    int           width = 1;
    DrawMode      mode  = invertMode;
    unsigned long color = MsWindowsColorOf(black);
    int           normalPenStyle = GetROP2(wp->dc);
    // move cursor to pos
    MoveToEx(wp->dc, pos.x, pos.y, 0);
    // set drawing mode
    if (mode == invertMode)
      SetROP2(wp->dc, R2_NOT);
    else
      SetROP2(wp->dc, R2_COPYPEN);
    linePen = CreatePen(PS_SOLID, width, color);
    SelectObject(wp->dc, linePen);
    // draw
    LineTo(wp->dc, pos.x, pos.y);  // a dot is a line with length 1
    // restore previous settings
    SetROP2(wp->dc, normalPenStyle);
    SelectObject(wp->dc, GetStockObject(BLACK_PEN));
    DeleteObject(linePen);
  } // OSB_DrawDot

  void OSB_DrawLine(const WindowPeer *wp,
                    Point startPos, Point endPos, int t) {
    HPEN linePen;
    int           width = t;
    DrawMode      mode  = invertMode;
    unsigned long color = MsWindowsColorOf(black);
    int           normalPenStyle  = GetROP2(wp->dc);
    // move cursor to startPt
    MoveToEx(wp->dc, startPos.x, startPos.y, 0);
    // set drawing mode
    if (mode == invertMode)
      SetROP2(wp->dc, R2_NOT);
    else
      SetROP2(wp->dc, R2_COPYPEN);
    // SOLID because start and end are inside the line
    linePen = CreatePen(PS_SOLID, width, color);
    SelectObject(wp->dc, linePen);
    // draw
    LineTo(wp->dc,endPos.x, endPos.y);
    // restore previous settings
    SetROP2(wp->dc, normalPenStyle);
    SelectObject(wp->dc, GetStockObject(BLACK_PEN));
    DeleteObject(linePen);
  } // OSB_DrawLine

  void OSB_DrawRectangle(const WindowPeer *wp,
                         Point topLeft, int w, int h, int t, bool fill) {
    HPEN          linePen;
    HBRUSH        lineBrush;
    LOGBRUSH      lb;
    int           width = t;
    DrawMode      mode  = invertMode;
    unsigned long color = MsWindowsColorOf(black);
    int           normalPenStyle = GetROP2(wp->dc);
    lb.lbStyle = BS_HOLLOW;
    // set drawing mode
    if (mode == invertMode)
      SetROP2(wp->dc, R2_NOT);
    else
      SetROP2(wp->dc, R2_COPYPEN);
    linePen = CreatePen(PS_INSIDEFRAME, width, color);
    SelectObject(wp->dc, linePen);
    if (fill)
      lineBrush = CreateSolidBrush(color);// fill color same as outline color
    else
      lineBrush = CreateBrushIndirect(&lb);
    SelectObject(wp->dc, lineBrush);
    // draw
    Rectangle(wp->dc, topLeft.x , topLeft.y , topLeft.x + w , topLeft.y + h);
    // restore pervious settings
    SetROP2(wp->dc, normalPenStyle);
    SelectObject(wp->dc, GetStockObject(BLACK_PEN));
    DeleteObject(linePen);
    DeleteObject(lineBrush);
  } // OSB_DrawRectangle

   void OSB_DrawOval(const WindowPeer *wp,
                     Point topLeft, int w, int h, int t, bool fill) {
    HPEN          linePen;
    HBRUSH        lineBrush;
    LOGBRUSH      lb;
    int           width = t;
    DrawMode      mode  = invertMode;
    unsigned long color = MsWindowsColorOf(black);
    int           normalPenStyle = GetROP2(wp->dc);
    lb.lbStyle = BS_HOLLOW;
    // set drawing mode
    if (mode == invertMode)
      SetROP2(wp->dc, R2_NOT);
    else
      SetROP2(wp->dc, R2_COPYPEN);
    linePen = CreatePen(PS_INSIDEFRAME, width, color);
    SelectObject(wp->dc, linePen);
    if (fill)
      lineBrush = CreateSolidBrush(color);  // fill color same as outline color
    else
      lineBrush = CreateBrushIndirect(&lb);
    SelectObject(wp->dc, lineBrush);
    // draw
    Ellipse(wp->dc, topLeft.x , topLeft.y , topLeft.x + w, topLeft.y + h);
    // restore previous settings
    SetROP2(wp->dc, normalPenStyle);
    SelectObject(wp->dc, GetStockObject(BLACK_PEN));
    DeleteObject(linePen);
    DeleteObject(lineBrush);
  } // OSB_DrawOval

  void OSB_DrawString(const WindowPeer *wp,
                      Point pos, const char *str, int size) {
    HFONT   font;
    LOGFONT lf;
    POINT   pt;
    // clear font structure
    memset(&lf, 0, sizeof(LOGFONT));
    // convert coordiantes
    pt.y = GetDeviceCaps(wp->dc, LOGPIXELSY);
    pt.x = 0;
    DPtoLP(wp->dc, &pt, 1);
    // construct font
    lf.lfHeight     = -MulDiv(size, pt.y, 72); // set height
    lf.lfEscapement = 0;
    lf.lfWeight     = FW_NORMAL;        // FW_NORMAL, FW_BOLD
    lf.lfItalic     = false;            // true for italic
    strcpy_s(lf.lfFaceName, "Arial");     // Arial is standard font
    font = CreateFontIndirect(&lf);
    // draw
    SelectObject(wp->dc, font);
    SetTextColor(wp->dc, RGB(0, 0, 0));
    SetBkMode(wp->dc, TRANSPARENT);     // background should remain
    TextOut(wp->dc, pos.x, pos.y, str, static_cast<int>(strlen(str)));
    DeleteObject(font);
  } // OSB_DrawString


//========================================================================
// end of file OSBridge2Win32.cpp
