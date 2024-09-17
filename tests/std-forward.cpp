#include <gtest/gtest.h>
#include <iostream>

// https://habr.com/ru/articles/568306/

using namespace std;


template<typename T>
void bar(const T& v) { cout << "bar<  by const lvalue ref  >" << endl; }

template<typename T>
void bar(T& v) { cout << "bar<  by lvalue ref  >" << endl; }

template<typename T>
void bar(T&& v) { cout << "bar<  by rvalue ref  >" << endl; }




// FUNCTION TEMPLATE forward
template <class _Ty>
[[nodiscard]] constexpr _Ty&& _forward(remove_reference_t<_Ty>& _Arg) noexcept
{
    cout << "_forward<  forward an lvalue as either an lvalue or an rvalue  >" << endl;
    return static_cast<_Ty&&>(_Arg);
}

template <class _Ty>
[[nodiscard]] constexpr _Ty&& _forward(remove_reference_t<_Ty>&& _Arg) noexcept
{
    static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
    cout << "_forward<  forward an rvalue as an rvalue  >" << endl;
    return static_cast<_Ty&&>(_Arg);
}




// FUNCTION TEMPLATE move
template <class _Ty>
[[nodiscard]] constexpr remove_reference_t<_Ty>&& _move(_Ty&& _Arg) noexcept
{
    cout << "_move<  forward _Arg as movable  >" << endl;
    return static_cast<remove_reference_t<_Ty>&&>(_Arg);
}




template<typename T>
void foo(T&& p)
{
    std::cout << "============== foo.0 ===============" << endl;
    bar(p);
    std::cout << "============== foo.1 ===============" << endl;
    bar(_move(p));
    std::cout << "============== foo.2 ===============" << endl;
    bar(_forward<T>(p));
    std::cout << "============== foo.3 ===============" << endl;
}




TEST(std_forward, lvalue)
{
    int i = 0;
    foo(i); // lvalue: T - int&, p - int&
}




TEST(std_forward, rvalue)
{
    foo(0); // rvalue: T - int, p - int&&
}
