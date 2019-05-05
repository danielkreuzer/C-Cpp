// MLWindow.h:                                           MiniLib V.4, 2004
// ----------
// Declaration of class Window.
//========================================================================

#ifndef MLWindow_h
#define MLWindow_h

#include <string>

#include "OSBridge.h"
#include "MLObject.h"


namespace ML {


//=== class Window ===

  class Window: public Object {

  private:

    std::string      winTitle;
    WindowPeer *wp;

  public:

    Window(const std::string& winTitle = "MiniLib Window");
    virtual ~Window();

 //--- overridden method ---

    virtual bool IsEqualTo(const Object *o) const;
    // returns whether both Windows are identical

 //--- new methods ...

    WindowPeer *GetWindowPeer();

 //--- ... for window handling ---

    virtual void Open();
    virtual void Close();
    virtual void Update();

    virtual void GetContent(Point &topLeft, int &w, int &h);
    virtual int  Width();        // returns same w as GetContent
    virtual int  Height();       // returns same h as GetContent
    virtual bool IsVisible();

    virtual void Redraw();       // override to do drawing

    virtual void InvalRectangle(Point topLeft, int w, int h);
    virtual void InvalContent(); // InvalRectangle for whole content

//--- ... for event handling ---

    virtual void OnIdle();
    virtual void OnMousePressed (Point pos);
    virtual void OnMouseReleased(Point pos);
    virtual void OnMouseMove    (Point pos);
    virtual void OnKey(char key);
    virtual void OnCommand(int commandNr);
    virtual void OnRedraw();
    virtual void OnResize();

    virtual void GetMouseState(bool &buttonPressed, Point &pos);

 //--- ... for drawing: w = width, h = height, t = thickness ---

    void DrawDot(Point pos);
    void DrawLine(Point start, Point end, int t = 1);
    void DrawRectangle(Point topLeft, int w, int h, int t = 1);
    void DrawFilledRectangle(Point topLeft, int w, int h);
    void DrawOval(Point topLeft, int w, int h, int t = 1);
    void DrawFilledOval(Point topLeft, int w, int h);
    void DrawString(Point pos, const std::string &str, int size = 10);

  }; // Window

} // ML


#endif

//========================================================================
// end of file MLWindow.h
