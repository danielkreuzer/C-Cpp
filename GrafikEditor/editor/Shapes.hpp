#ifndef SHAPES_HPP
#define SHAPES_HPP

#include <cassert>
#include <MLWindow.h>
#include <memory>

enum class Shapes {Line, Oval, Rectangle,};

struct Shape : ML::Object {
        Shape(const Point & pos, int lineWidth) : pos1{pos}, pos2{pos}, lineWidth{lineWidth} {
            Register("Shape", "Object");
            assert(lineWidth > 0);
        }

        Shape(const Shape &) = delete;
        Shape(Shape &&) = delete;

        auto operator=(const Shape &) -> Shape & = delete;
        auto operator=(Shape &&) -> Shape & = delete;

        ~Shape() = default;

        void MoveEnd(ML::Window * window, const Point & pos){
            if((pos.x != pos2.x) || (pos.y != pos2.y)){
                InvertInWindow(window);
                pos2 = pos;
                InvertInWindow(window);
            }
        }

        virtual void InvertInWindow(ML::Window * window) const = 0;
        static auto create(Shapes shape, const Point & pos, int lineWidth) -> std::unique_ptr<Shape>;
        //virtual auto Clone() const -> std::unique_ptr<Shape> = 0;
        //virtual void SelectInWindow(ML::Window * window) const = 0;
    protected:
        Point pos1;
        Point pos2;
        const int lineWidth;
};

#endif