// MLWindow.cpp:                                         MiniLib V.4, 2004
// ------------
// Implementation of class Window.
//========================================================================

using namespace std;

#include "MLApplication.h"

#include "MLWindow.h"

namespace ML {


//=== Window ===


  Window::Window(const string &winTitle)
  : winTitle(winTitle), wp(0) { // no window peer yet, set in Open
    Register("Window", "Object");
 } // Window::Window

  Window::~Window() {
    Close(); // also destroys window peer
  } // Window::~Window


 //--- overridden method ---

  bool Window::IsEqualTo(const Object *o) const {
    return this == o;
  } // Window::IsEqualTo


 //--- new methods ... ---

  WindowPeer *Window::GetWindowPeer() {
    return wp;
  } // Window::GetWinWindow


//--- ... for window handling ---

  void Window::Open() {
    OSB_CreateNewWindow(const_cast<char*>(winTitle.c_str()), wp);
    Application::instance->AddWindow(this);
  } // Window::Open

  void Window::Close() {
    Application::instance->RemoveWindow(this);
    OSB_DestroyOldWindow(wp);
    wp = 0;
  } // Window::Close

  void Window::Update() {
    if (IsVisible()) {
      OSB_EraseContent(wp);
      Redraw();
    } // if
  } // Window::Update


  void Window::GetContent(Point &topLeft, int &w, int &h) {
    OSB_GetContent(wp, topLeft, w, h);
  } // Window::GetContent

  int Window::Width() {
    Point topLeft;
    int w, h;
    GetContent(topLeft, w, h);
    return w;
  } // Window::Width

  int Window::Height() {
    Point topLeft;
    int w, h;
    GetContent(topLeft, w, h);
    return h;
  } // Window::Height

  bool Window::IsVisible() {
    return OSB_IsVisible(wp);
  } // Window::IsVisible


  void Window::Redraw() {
    // nothing to do here: override to do drawing
  } // Window::Redraw


  void Window::InvalRectangle(Point topLeft, int w, int h) {
    OSB_InvalRect(wp, topLeft, w, h);
  } // Window::InvalRectangle

  void Window::InvalContent() {
    Point topLeft;
    int w, h;
    GetContent(topLeft, w, h);
    InvalRectangle(topLeft, w, h);
  } // Window::InvalContent


//--- ... for event handling ---

  void Window::OnIdle() {
    // nothing to do
  } // Window::OnIdle

  void Window::OnMousePressed(Point /* pos */) {
    // nothing to do
  } // Window::OnMousePressed

  void Window::OnMouseReleased(Point /* pos */) {
    // nothing to do
  } // Window::OnMouseReleased

  void Window::OnMouseMove(Point /* pos */) {
    // nothing to do
  } // Window::OnMouseMove

  void Window::OnKey(char /* key */) {
    // nothing to do
  } // Window::OnKey

  void Window::OnCommand(int commandNr) {
    if (commandNr == closeCommand) {
      Close();
      delete this;
    } // if
  } // Window::OnCommand

  void Window::OnRedraw() {
    if (IsVisible())
      Redraw();
  } // Window::OnRedraw

  void Window::OnResize() {
    if (IsVisible())
      Update();
  } // Window::OnResize


  void Window::GetMouseState(bool &buttonPressed, Point &pos) {
    OSB_GetMouseState(wp, buttonPressed, pos);
  } // Window::GetMouseState


//--- ... for drawing ---

  void Window::DrawDot(Point pos) {
    if (IsVisible())
      OSB_DrawDot(wp, pos);
  } // Window::DrawDot

  void Window::DrawLine(Point start, Point end, int t) {
    if (IsVisible())
      OSB_DrawLine(wp, start, end, t);
  } // Window::DrawLine

  void Window::DrawRectangle(Point topLeft, int w, int h, int t) {
    if (IsVisible())
      OSB_DrawRectangle(wp, topLeft, w, h, t, false);
  } // Window::DrawRect

  void Window::DrawFilledRectangle(Point topLeft, int w, int h) {
    if (IsVisible())
      OSB_DrawRectangle(wp, topLeft, w, h, 1, true);
  } // Window::DrawFilledRectangle

  void Window::DrawOval(Point topLeft, int w, int h, int t) {
    if (IsVisible())
      OSB_DrawOval(wp, topLeft, w, h, t, false);
  } // Window::DrawOval

  void Window::DrawFilledOval(Point topLeft, int w, int h) {
    if (IsVisible())
      OSB_DrawOval(wp, topLeft, w, h, 1, true);
  } // Window::DrawFilledOval

  void Window::DrawString(Point pos, const string &str, int size) {
    if (IsVisible())
      OSB_DrawString(wp, pos, str.c_str(), size);
  } // Window::DrawText


} // ML

//========================================================================
// end of file MLWindow.cpp
