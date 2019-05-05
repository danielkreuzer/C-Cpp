#ifndef OBJ_HPP
#define OBJ_HPP

#include <iostream>
#include <string>
#include <utility>

class object
{ // abstract base class for a singly-rooted class hierarchy
  private:
    static auto &get_c_instances()
    { // encapsulates class member
        static int c_instances{0};
        return c_instances;
    }

  public:
    static auto const &get_instances()
    {
        return get_c_instances();
    }

    virtual ~object()
    {
        --get_c_instances();
    }

    object &operator=(object const &) = default; // copy assignment
    object &operator=(object &&) = default;      // move assignment

    virtual std::ostream& print(std::ostream &out = std::cout) const = 0;

  protected:
    object()
    { // default constructor
        ++get_c_instances();
    }

    object(object const &)
    { // copy construction
        ++get_c_instances();
    }

    object(object &&) noexcept
    { // move construction
        ++get_c_instances();
    }
};

inline auto &operator<<(std::ostream &lhs, object &rhs)
{
    return rhs.print(lhs);
}

#endif