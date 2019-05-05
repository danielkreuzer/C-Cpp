#include "Shapes.hpp"

namespace {
    struct Line final : Shape {
        Line(const Point & pos, int lineWidth) : Shape{pos, lineWidth} {Register("Line", "Shape");}

        void InvertInWindow(ML::Window * window) const override {
            window->DrawLine(pos1, pos2, lineWidth);
        }
        /*
        void SelectInWindow(ML::Window * window) const override {
            window->DrawRectangle({pos1.x - 2, pos1.y -2}, 5, 5);
            window->DrawRectangle({pos2.x + 2, pos2.y +2}, 5, 5);
        }

        auto Clone() const -> std::unique_ptr<Shape> override {
            auto ptr{std::make_unique<Line>(pos1, lineWidth)};
            ptr->pos2 = pos2;
            return ptr;
        }*/
    };

    struct Rectangle final : Shape
    {
        Rectangle(const Point &pos, int lineWidth) : Shape{pos, lineWidth} { Register("Line", "Shape"); }

        void InvertInWindow(ML::Window *window) const override
        {
            window->DrawRectangle(pos1, pos2.x - pos1.x, pos2.y - pos1.y, lineWidth);
        }
        /*
        void SelectInWindow(ML::Window * window) const override {
            //not implemented
        }

        auto Clone() const -> std::unique_ptr<Shape> override {
            //not implemented
            return nullptr;
        }*/
    };

    struct Oval final : Shape {
        Oval(const Point & pos, int lineWidth) : Shape{pos, lineWidth} {Register("Line", "Shape");}

        void InvertInWindow(ML::Window * window) const override {
            window->DrawOval(pos1, pos2.x - pos1.x, pos2.y - pos1.y, lineWidth);
        }
        /*
        void SelectInWindow(ML::Window * window) const override {
            //not implemented
        }

        auto Clone() const -> std::unique_ptr<Shape> override {
            //not implemented
            return nullptr;
        }*/
    };
}

auto Shape::create(Shapes shape, const Point & pos, int lineWidth) -> std::unique_ptr<Shape> {
    switch(shape) {
        case Shapes::Line:          return std::make_unique<Line>(pos, lineWidth);
        case Shapes::Rectangle:     return std::make_unique<Rectangle>(pos, lineWidth);
        case Shapes::Oval:          return std::make_unique<Oval>(pos, lineWidth);
        default:
            Error("Unknown Shape requesetet");
            return {}; //never reached!
    }
}