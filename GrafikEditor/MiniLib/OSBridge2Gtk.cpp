// OSBridge2GTK.cpp:                                   MiniLib V.4.5, 2009
// ----------------
// Bridge to platform specific GUI operating system (Unix GTK).
//========================================================================


#ifdef WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#include <windows.h>
#endif

#include <cstring>
#include <cctype>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <fstream>
#include <set>
#include <vector>
#include <set>
#include <algorithm>

#include "MetaInfo.h"
#include "MLApplication.h"
#include "MLWindow.h"

#include "OSBridge.h"

using namespace std;
using namespace ML;

	enum DrawMode  {addMode, invertMode};

	struct WindowPeer {
		GtkWidget*     window;
		GtkWidget*     drawingArea;
		GtkWidget*     menuBar;
		GtkAccelGroup* accelGroup;
		GdkGCValues    drawingGCValues;
	}; // struct

	//--- one menubar structure (e.g., for File, Edit, etc.) ---

	struct MenuItemDescr {
		MenuItemDescr(const string& command, const string& mCommand, 
									char shortcut, int cmdId) : 
			command(command), commandWithMnemonic(mCommand),
			shortcut(shortcut), cmdId(cmdId) {}

		string command;       // menu item text
		string commandWithMnemonic;
		char   shortcut;      // command shortcut
		int    cmdId;         // unique id of menu item
	};

	struct MenuDescr {
		MenuDescr(const string& title, const string& mTitle) : 
			title(title), titleWithMnemonic(mTitle) {}

		bool operator==(const string& t) { return title == t; }

		string                title;         // menubar title
		string                titleWithMnemonic;
		vector<MenuItemDescr> items;
		set<char>             usedMnemonics;
	}; // struct

	//--- module golbals ---
	
	static vector<MenuDescr> menus;
	static set<char>         usedMnemonics;

	static int lastMenuCmd = 100;  // must be greater than 0

	static gint timerId = 0;

	static void (*eventHandler)(Event& e) = 0;
	static std::set<WindowPeer*> windowData;

	// foreward delcarations
	static void CommandCallback(GtkWidget* widget, gpointer data);
	static gboolean TimeoutCallback(gpointer data);


#ifdef WIN32
	//--- WinMain function ---

	static HANDLE    hInstance, hPrevInst;

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

#endif

	//=== 1. application utilities ===

	void OSB_InitApplication(const char* /* applName */) {
		gtk_init(0, 0);

		// invoke timerout function every 10 milliseconds.
		timerId = g_timeout_add(10, TimeoutCallback, NULL); 
	} // OSB_InitApplication

	void OSB_DestroyApplication() {
		g_source_remove(timerId);
	} // OSB_DestroyApplication

	void OSB_MainEventLoop(void (*eh)(Event& e)) {
		eventHandler = eh;
		gtk_main();
	} // OSB_MainEventLoop

	void OSB_QuitMainEventLoop() {
		gtk_main_quit();
	} // OSB_QuitMainEventLoop()


	//=== 2. menu management utilitiess ===

	static void MarkMnemonic(string& menuText, set<char>& usedMnemonics) {
		unsigned int i;
	 
		// find first alphabetical character that is no already used as
		// a mnemonic
		for (i=0; i<menuText.length() &&
							(! isalpha(menuText[i]) ||
							usedMnemonics.find(menuText[i]) != usedMnemonics.end());
				 i++);

		if (i < menuText.length()) {  // mnemonic char found?
			usedMnemonics.insert(menuText[i]);
			menuText.insert(i, "_"); 
		}
	}

	static GtkWidget* InsertMenu(GtkWidget* menuBar, const string& menuTitle) {
		GtkWidget* menu = gtk_menu_new();
		GtkWidget* itemsContainer = gtk_menu_item_new_with_mnemonic(menuTitle.c_str());
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(itemsContainer), menu);
		gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), itemsContainer);

		return menu;
	}

	static void InsertMenuItem(GtkWidget* menu,  GtkAccelGroup* accelGroup, 
														 const string& cmd, char shortcut, int cmdNo) {
		GtkWidget* menuItem = 0;
		if (cmd == "-")
			menuItem = gtk_separator_menu_item_new();
		else
			menuItem = gtk_menu_item_new_with_mnemonic(cmd.c_str());
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

		if (shortcut != ' ')
			gtk_widget_add_accelerator(menuItem, "activate", accelGroup, shortcut,
																 GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
			
		if (cmd != "-")
			g_signal_connect(G_OBJECT(menuItem), "activate", 
											 G_CALLBACK(CommandCallback), GINT_TO_POINTER(cmdNo));
	} // InsertMenuItem

	int OSB_NewMenuCommand(const char* menuTitle, const char* cmd, char shortcut) {
		// look for title
		string title = menuTitle;
		string command = cmd;

		vector<MenuDescr>::iterator menuIt = find(menus.begin(), menus.end(), title);

		if (menuIt == menus.end()) { // title not found
			string mTitle = title;
			MarkMnemonic(mTitle, usedMnemonics);
			menus.push_back(MenuDescr(title, mTitle));
			menuIt = menus.end() - 1;
		}

		int cmdId = command == "-" ? 0 : lastMenuCmd++;
		string mCmd = cmd;
		MarkMnemonic(mCmd, menuIt->usedMnemonics); 
		menuIt->items.push_back(MenuItemDescr(cmd, mCmd, shortcut, cmdId));

		return cmdId;
	} // OSB_NewMenuCommand

	void OSB_InstallMenuBar() {
	} // OSB_InstallMenuBar

	void OSB_RemoveMenuBar() {
	} // OSB_RemoveMenuBar


	//=== 3. window management utilities ===

	void OSB_GetMouseState(const WindowPeer* wp,
												 bool& buttonPressed, Point& pos) {
		GdkModifierType mask;
		gdk_window_get_pointer(wp->drawingArea->window, &pos.x, &pos.y, &mask);
		buttonPressed = (mask & GDK_BUTTON1_MASK) != 0;
	} // OSB_GetMouseState

	void OSB_ShowMessageWindow(const char* winTitle, const char* message) {
		GtkDialogFlags flags = GtkDialogFlags(GTK_DIALOG_MODAL);
		GtkWidget* dialog = gtk_dialog_new_with_buttons(winTitle, NULL, flags, 
													GTK_STOCK_OK, GTK_RESPONSE_ACCEPT, NULL);
		gtk_window_set_position(GTK_WINDOW(dialog), GTK_WIN_POS_CENTER_ALWAYS);

		GtkWidget* label = gtk_label_new(message);
		gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 120);

		gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), label);
		gtk_widget_show_all(dialog);

		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy(dialog);
	} // OSB_ShowMessageWindow

	
	// GTK event handler

	bool OSB_EqualWindowPeers(const WindowPeer* wp1, const WindowPeer* wp2) {
		return ( (wp1 == wp2) ||
					 ( (wp1 != 0) && (wp2 != 0) && (wp1->window == wp2->window)));
	} // OSB_EqualWindowPeers

	static void CommandCallback(GtkWidget* /* widget */, gpointer data) {
		Event e;
		e.kind       = commandEvent;
		e.commandNr  = GPOINTER_TO_INT(data);
		eventHandler(e);
	} // CommandCallBack


	static gboolean ButtonPressCallback(GtkWidget* /* widget */, GdkEventButton* evt, gpointer /* data */) {
		if (evt->button == 1) {
			Event e;
			e.kind  = buttonDownEvent;
			e.pos.x = (int)evt->x;
			e.pos.y = (int)evt->y;
			eventHandler(e);
		}

		return TRUE;
	}

	static gboolean ButtonReleaseCallback(GtkWidget* /* widget */, GdkEventButton* evt, gpointer /* data */) {
		if (evt->button == 1) {
			Event e;
			e.kind  = buttonReleaseEvent;
			e.pos.x = (int)evt->x;
			e.pos.y = (int)evt->y;
			eventHandler(e);
		}

		return TRUE;
	}

	static gboolean ButtonMotionCallback(GtkWidget* /* widget */, GdkEventMotion* evt, gpointer /* data */) {
		Event e;
		e.kind  = mouseMoveEvent;
		e.pos.x = (int)evt->x;
		e.pos.y = (int)evt->y;
		eventHandler(e);

		return TRUE;
	}

	static gboolean ExposeCallback(GtkWidget* /* widget */, GdkEventExpose* /* evt */, gpointer data) {
		if (Application::instance != 0) {
			WindowPeer* wp = (WindowPeer*)data;
			Window* w = Application::instance->WindowOf(wp);
			if (w != 0)
				w->Update();
		} // if
		
		return TRUE;
	}

	static gboolean ResizeCallback(GtkWidget* /* widget */, gpointer data) {
		if (Application::instance != 0) {
			WindowPeer* wp = (WindowPeer*)data;
			Window* w = Application::instance->WindowOf(wp);
			if (w != 0)
				w->OnResize();
		} // if

		return TRUE;
	}

	static gboolean KeyPressCallback(GtkWindow* /* window */, GdkEventKey* evt, gpointer /* data */) {
		Event e;
		e.kind = keyEvent;
		e.key  = static_cast<char>(evt->keyval);
		eventHandler(e);

		return TRUE;
	}

	static gboolean DeleteWindowCallback(GtkWidget* /* widget */, GdkEvent* /* evt */, gpointer data) {
		if (Application::instance != 0) {
			WindowPeer* wp = (WindowPeer*)data;
			Window* w = Application::instance->WindowOf(wp);
			if (w != 0)
				delete w;
		} // if

		return TRUE;
	}

	static gboolean TimeoutCallback(gpointer /* data */) {
		Event e;
		e.kind = idleEvent;
		eventHandler(e);

		return TRUE;
	}

	void OSB_CreateNewWindow(char* title, WindowPeer*& wp) {
		if (wp == 0)
			wp = new WindowPeer;

		wp->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

		gtk_window_set_title(GTK_WINDOW(wp->window), title);
		gtk_window_set_default_size(GTK_WINDOW(wp->window), 600, 400);
		gtk_window_set_position(GTK_WINDOW(wp->window), GTK_WIN_POS_CENTER_ON_PARENT);
		gtk_container_set_border_width(GTK_CONTAINER (wp->window), 0);

		wp->drawingArea = gtk_event_box_new();

		GtkWidget* vbox = gtk_vbox_new(FALSE, 0);
		gtk_container_add(GTK_CONTAINER(wp->window), vbox);

		wp->accelGroup = gtk_accel_group_new();
		gtk_window_add_accel_group(GTK_WINDOW(wp->window), wp->accelGroup);

		//insert main menu bar
		wp->accelGroup = gtk_accel_group_new();
		gtk_window_add_accel_group(GTK_WINDOW(wp->window), wp->accelGroup);
		wp->menuBar = gtk_menu_bar_new();
	
		for (unsigned int i=0; i<menus.size(); i++) {
			GtkWidget* menu = InsertMenu(wp->menuBar, menus[i].titleWithMnemonic);

			for (unsigned int j=0; j<menus[i].items.size(); j++) {
				InsertMenuItem(menu, wp->accelGroup, 
											 menus[i].items[j].commandWithMnemonic, 
											 menus[i].items[j].shortcut, 
											 menus[i].items[j].cmdId);
			}

			windowData.insert(wp);
		}

		// create top level window layout
		gtk_box_pack_start(GTK_BOX(vbox), wp->menuBar, FALSE, FALSE, 0);
		gtk_box_pack_end(GTK_BOX (vbox), wp->drawingArea, TRUE, TRUE, 0);

		gtk_widget_show_all(wp->window);

		// init drawing graphics context

		GdkGC* gc = wp->window->style->fg_gc[GTK_STATE_NORMAL];

		GdkGCValues oldGCValues;
		gdk_gc_get_values(gc, &oldGCValues);

		GdkColor pen_col;
		gdk_color_parse("black", &pen_col);
		gdk_gc_set_rgb_fg_color(gc, &pen_col);
		gdk_gc_set_function(gc, GDK_INVERT);
		gdk_gc_get_values(gc, &wp->drawingGCValues);

		gdk_gc_set_values(gc, &oldGCValues, 
											(GdkGCValuesMask) (GDK_GC_FOREGROUND | GDK_GC_FUNCTION));

		g_signal_connect (G_OBJECT(wp->window), "delete_event",
				G_CALLBACK (DeleteWindowCallback), wp);
		g_signal_connect_after(G_OBJECT(wp->window), "key_press_event",
				G_CALLBACK (KeyPressCallback), wp);
		g_signal_connect (G_OBJECT(wp->drawingArea), "button_press_event",
				G_CALLBACK (ButtonPressCallback), NULL);
		g_signal_connect (G_OBJECT(wp->drawingArea), "button_release_event",
			G_CALLBACK (ButtonReleaseCallback), NULL);
		g_signal_connect (G_OBJECT(wp->drawingArea), "motion_notify_event",
				G_CALLBACK (ButtonMotionCallback), NULL);
		g_signal_connect (G_OBJECT(wp->drawingArea), "expose_event",
				G_CALLBACK (ExposeCallback), wp);
		g_signal_connect (G_OBJECT(wp->drawingArea), "configure_event",
				G_CALLBACK (ResizeCallback), wp);

		gtk_widget_add_events(wp->drawingArea, GDK_BUTTON_PRESS_MASK
				| GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK
				| GDK_POINTER_MOTION_HINT_MASK | GDK_KEY_PRESS_MASK);
	} // OSB_CreateNewWindow

	void OSB_DestroyOldWindow(WindowPeer*& wp) {
		if (wp) {
			gtk_widget_destroy(wp->window);
			windowData.erase(wp);

			if (windowData.size() == 0) { // if all windows have been closed.
				Event e;
				e.kind     = quitEvent;
				e.exitCode = 0;
				eventHandler(e);
			}

			delete wp;
		} // if
		wp = 0;
	} // OSB_DestroyOldWindow

	WindowPeer* OSB_ActiveWindowPeer() {
		set<WindowPeer*>::iterator it;
		for (it = windowData.begin(); it != windowData.end(); ++it)
			if (gtk_window_has_toplevel_focus(GTK_WINDOW((*it)->window)))
				return *it;

		return 0;
	} // OSB_ActiveWindowPeer

	bool OSB_IsVisible(const WindowPeer* /* wp */) {
		return true; // TODO?
	} // OSB_IsVisible

	void OSB_EraseContent(const WindowPeer* wp) {
		gdk_window_clear(wp->window->window);
	} // OSB_EraseContent

	void OSB_GetContent(const WindowPeer *wp, Point &topLeft, int &w, int &h) {
		topLeft.x = 0;
		topLeft.y = 0;
		gdk_drawable_get_size(wp->drawingArea->window, &w, &h);
	} // OSB_GetContent

	void OSB_InvalRect(const WindowPeer* wp, Point /* topLeft */, int w, int h) {
		GdkRectangle rect;
		rect.x      = 0;
		rect.y      = 0;
		rect.width  = w;
		rect.height = h;
		gdk_window_invalidate_rect(wp->window->window, &rect, TRUE);
	} // OSB_InvalRect


	//=== 4. drawing utilties ===

	static GdkGC* GetDrawingGC(const WindowPeer* wp, int lineWidth, GdkGCValues& oldGCValues) {
		GdkGC* gc = wp->drawingArea->style->fg_gc[GTK_STATE_NORMAL];
		gdk_gc_get_values(gc, &oldGCValues);
		GdkGCValues gcValues = wp->drawingGCValues;
		gcValues.line_width = lineWidth;
		gdk_gc_set_values(gc, &gcValues,
											(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_FUNCTION | GDK_GC_LINE_WIDTH));
		return gc;
	}

	static void ResetDrawingGC(GdkGC* gc, GdkGCValues& oldGCValues) {
		gdk_gc_set_values(gc, &oldGCValues,
											(GdkGCValuesMask)(GDK_GC_FOREGROUND | GDK_GC_FUNCTION | GDK_GC_LINE_WIDTH));
	}

	void OSB_DrawDot(const WindowPeer* wp, Point pos) {
		GdkGCValues oldGCValues;
		GdkGC* gc = GetDrawingGC(wp, 1, oldGCValues);
		gdk_draw_point(wp->drawingArea->window, gc, pos.x, pos.y);
		ResetDrawingGC(gc, oldGCValues);
	} // OSB_DrawDot

	void OSB_DrawLine(const WindowPeer* wp,
										Point start, Point end, int t) {
		GdkGCValues oldGCValues;
		GdkGC* gc = GetDrawingGC(wp, t, oldGCValues);
		gdk_draw_line(wp->drawingArea->window, gc, start.x, start.y, end.x, end.y);
		ResetDrawingGC(gc, oldGCValues);
	} // OSB_DrawLine

	void OSB_DrawRectangle(const WindowPeer* wp,
												 Point topLeft, int w, int h, int t, bool fill) {
	 // catch negative width and height
	 if (w < 0) {
		 w = -w;
		 topLeft.x -= w;
	 } // if
	 if (h < 0) {
		 h = -h;
		 topLeft.y -= h;
	 } // if

		GdkGCValues oldGCValues;
		GdkGC* gc = GetDrawingGC(wp, t, oldGCValues);
		gdk_draw_rectangle(wp->drawingArea->window, gc, fill, topLeft.x, topLeft.y, w, h);
		ResetDrawingGC(gc, oldGCValues);
	} // OSB_DrawRectangle

	void OSB_DrawOval(const WindowPeer* wp,
										Point topLeft, int w, int h, int t, bool fill) {
		// catch negative width and height
		if (w < 0) {
			w = -w;
			topLeft.x -= w;
		} // if
		if (h < 0) {
			h = -h;
			topLeft.y -= h;
		} // if

		GdkGCValues oldGCValues;
		GdkGC* gc = GetDrawingGC(wp, t, oldGCValues);
		gdk_draw_arc(wp->drawingArea->window, gc, fill, topLeft.x, topLeft.y, w, h, 0, 360*64);
		ResetDrawingGC(gc, oldGCValues);
	} // OSB_DrawOval

	void OSB_DrawString(const WindowPeer* wp,
											Point pos, const char* str, int size) {
		GdkGCValues oldGCValues;
		GdkGC* gc = GetDrawingGC(wp, 1, oldGCValues);

		PangoLayout* layout = gtk_widget_create_pango_layout(wp->drawingArea, str);
		PangoFontDescription* fontDescr = pango_font_description_new();
		// pango_font_description_set_family(fontDescr, "monospace");
		pango_font_description_set_absolute_size(fontDescr, size*PANGO_SCALE);
		pango_layout_set_font_description(layout, fontDescr);

		gdk_draw_layout(wp->drawingArea->window, gc, pos.x, pos.y, layout);
		
		pango_font_description_free(fontDescr);
		ResetDrawingGC(gc, oldGCValues);
	} // OSB_DrawString


//========================================================================
// end of file OSBridge2X11.cpp
