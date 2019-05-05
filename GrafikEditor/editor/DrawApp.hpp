#ifndef DRAWAPP_HPP
#define DRAWAPP_HPP

#include <cassert>
#include <memory>

#include <MLVector.h>
#include <MLApplication.h>
#include "Shapes.hpp"
#include <string>

struct DrawApp final : ML::Application {
        DrawApp() : ML::Application{"DrawApp"} { Register("DrawApp", "Application"); }

        DrawApp(const DrawApp &) = delete;
        DrawApp(DrawApp &&) = delete;

        auto operator=(const DrawApp &)-> DrawApp & = delete;
        auto operator=(DrawApp &&) -> DrawApp & = delete;

        ~DrawApp() = default;

        void OpenNewWindow() override {
            DrawWindow * Window = new DrawWindow();
            Window->Open();
            Window->DrawSidebar();
            //(new DrawWindow)->Open(); //life time internally managed!
        }

        void BuildMenus() override {
            ML::Application::BuildMenus();
            lineCommand = ML::NewMenuCommand("Shape", "Line", 'L');
            rectangleCommand = ML::NewMenuCommand("Shape", "Rectangle", 'R');
            ovalCommand = ML::NewMenuCommand("Shape", "Oval", 'O');

            for(auto i{0}; i < lineWidthCount; ++i) {
                lineWidthCommand[i] = ML::NewMenuCommand("LineWidth", std::to_string(i * 2 + 1).c_str(), ' ');
            }
        }
    private:
        static int lineCommand, rectangleCommand, ovalCommand;
        static const int lineWidthCount{10};
        static int lineWidthCommand[lineWidthCount];
        struct DrawWindow final : ML::Window {
                DrawWindow() : ML::Window{"Draw Window"} {
                    Register("DrawWindow", "Window");
                }

                DrawWindow(const DrawWindow &) = delete;
                DrawWindow(DrawWindow &&) = delete;

                auto operator=(const DrawWindow &) -> DrawWindow & = delete;
                auto operator=(DrawWindow &&) -> DrawWindow & = delete;

                ~DrawWindow(){
                    shapes.DeleteElements();
                    Close();
                }

                void Redraw() override {
                    std::unique_ptr<ML::Iterator> it { shapes.NewIterator() };
                    for(auto obj = it->Next(); obj; obj = it->Next()) {
                        auto shape{dynamic_cast<Shape *>(obj)};
                        assert(shape);
                        shape->InvertInWindow(this);
                    }
                    if(newShape) newShape->InvertInWindow(this);
                }
                void OnMousePressed(Point pos) override {
                    if(pos.x > 71){
                            newShape = Shape::create(nextShape.shape, pos, nextShape.lineWidth);
                        
                    } else {
                        if (pos.y >= 0 && pos.y <= 65) {
                            if(selectedOnToolbar) ChangeSelection(selectedOnToolbar);
                            ChangeSelection(1);
                            selectedOnToolbar = 1;

                        } else if (pos.y > 65 && pos.y <= 130) {
                            if(selectedOnToolbar) ChangeSelection(selectedOnToolbar);
                            ChangeSelection(2);
                            selectedOnToolbar = 2;
                            nextShape.shape = Shapes::Line;
                        } else if (pos.y > 130 && pos.y <= 195) {
                            if(selectedOnToolbar) ChangeSelection(selectedOnToolbar);
                            ChangeSelection(3);
                            selectedOnToolbar = 3;
                            nextShape.shape = Shapes::Rectangle;
                        } else if (pos.y > 195 && pos.y <= 260) {
                            if(selectedOnToolbar) ChangeSelection(selectedOnToolbar);
                            ChangeSelection(4);
                            selectedOnToolbar = 4;
                            nextShape.shape = Shapes::Oval;
                        } else {
                            // do nothing
                        }
                    }
                }
                void OnMouseMove(Point pos) override{
                    if(pos.x > 71){
                        if(newShape) newShape->MoveEnd(this, pos);
                    }
                }
                void OnMouseReleased(Point pos) override{
                    if(pos.x > 71){
                        if(newShape) {
                            newShape->MoveEnd(this, pos);
                            shapes.Add(newShape.release());
                            newShape = nullptr;
                        }
                    } else {
                        if(newShape) {
                            pos.x = 71;
                            newShape->MoveEnd(this, pos);
                            shapes.Add(newShape.release());
                            newShape = nullptr;
                        }
                    }
                }

                void OnCommand(int commandNr) override{
                    if(commandNr == ML::clearCommand) {
                        shapes.DeleteElements();
                        newShape = nullptr;
                        InvalContent();
                        DrawSidebar();
                        selectedOnToolbar = 0;
                        return;
                    }
                    if(commandNr == lineCommand) {
                        nextShape.shape = Shapes::Line;
                        return;
                    }
                    if(commandNr == rectangleCommand) {
                        nextShape.shape = Shapes::Rectangle;
                        return;
                    }
                    if(commandNr == ovalCommand) {
                        nextShape.shape = Shapes::Oval;
                        return;
                    }
                    for(auto i {0}; i < lineWidthCount; ++i) {
                        if(commandNr == lineWidthCommand[i]){
                            nextShape.lineWidth = i * 2 + 1;
                            return;
                        }
                    }


                    ML::Window::OnCommand(commandNr);
                }

                void DrawSidebar() {
                    // sideline
                    nextShape.shape = Shapes::Line;
                    Point p1;
                    p1.x = 70;
                    p1.y = 0;
                    Point p2;
                    p2.x = 70;
                    p2.y = 265;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    //endline
                    nextShape.shape = Shapes::Line;
                    p1.x = 0;
                    p1.y = 265;
                    p2.x = 70;
                    p2.y = 265;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    //four rectangles
                    nextShape.shape = Shapes::Rectangle;
                    p1.x = 5;
                    p1.y = 5;
                    p2.x = 65;
                    p2.y = 65;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    p1.x = 5;
                    p1.y = 70;
                    p2.x = 65;
                    p2.y = 130;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    p1.x = 5;
                    p1.y = 135;
                    p2.x = 65;
                    p2.y = 195;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    p1.x = 5;
                    p1.y = 200;
                    p2.x = 65;
                    p2.y = 260;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    //draw Selection button
                    nextShape.shape = Shapes::Line;
                    p1.x = 10;
                    p1.y = 59;
                    p2.x = 54;
                    p2.y = 15;
                    newShape = Shape::create(nextShape.shape, p1, 2);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    p1.x = 44;
                    p1.y = 15;
                    p2.x = 54;
                    p2.y = 15;
                    newShape = Shape::create(nextShape.shape, p1, 2);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    p1.x = 54;
                    p1.y = 25;
                    p2.x = 54;
                    p2.y = 15;
                    newShape = Shape::create(nextShape.shape, p1, 2);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    //draw Line button
                    p1.x = 10;
                    p1.y = 124;
                    p2.x = 54;
                    p2.y = 80;
                    newShape = Shape::create(nextShape.shape, p1, 2);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    //draw Rectangle button
                    nextShape.shape = Shapes::Rectangle;
                    p1.x = 15;
                    p1.y = 145;
                    p2.x = 55;
                    p2.y = 185;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                    //draw Oval button
                    nextShape.shape = Shapes::Oval;
                    p1.x = 10;
                    p1.y = 205;
                    p2.x = 60;
                    p2.y = 255;
                    newShape = Shape::create(nextShape.shape, p1, 3);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                }
                
                void ChangeSelection(int selected){
                    nextShape.shape = Shapes::Rectangle;
                    Point p1;
                    Point p2;
                    switch (selected) {
                        case 1:
                            p1.x = 5;
                            p1.y = 5;
                            p2.x = 65;
                            p2.y = 65;
                            break;
                        case 2:
                            p1.x = 5;
                            p1.y = 70;
                            p2.x = 65;
                            p2.y = 130;
                            break;
                        case 3:
                            p1.x = 5;
                            p1.y = 135;
                            p2.x = 65;
                            p2.y = 195;
                            break;
                        case 4:
                            p1.x = 5;
                            p1.y = 200;
                            p2.x = 65;
                            p2.y = 260;
                            break;
                        default:
                            //do nothing;
                            break;
                    }

                    newShape = Shape::create(nextShape.shape, p1, 2);
                    newShape->MoveEnd(this, p2);
                    shapes.Add(newShape.release());
                }
            private:
                ML::Vector shapes;
                std::unique_ptr<Shape> newShape;
                int selectedOnToolbar{0};
                struct {
                    Shapes shape{Shapes::Line};
                    int lineWidth{1};
                } nextShape;
        };
};


#endif