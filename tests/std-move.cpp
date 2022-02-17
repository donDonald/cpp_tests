#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <thread>

//  https://ravesli.com/urok-192-std-move/
//      ! Как мы уже говорили на предыдущем уроке, вы должны оставлять объекты, ресурсы которых вы перемещаете, в четко определенном состоянии. В идеале это должно быть «нулевое состояние» (null/nullptr).
//      ! На этом этапе стоит сообщить, что std::move() как бы подсказывает компилятору, что нам больше не нужен этот объект (по крайней мере, в его текущем состоянии). Следовательно, вы не должны использовать std::move() с любым «постоянным» объектом, который вы не хотите изменять, и вам не следует ожидать, что объекты, которые используются с std::move(), останутся прежними.



//  https://ravesli.com/urok-191-konstruktor-peremeshheniya-i-operator-prisvaivaniya-peremeshheniem/
//      ! В C++11 добавили две новые функции для работы с семантикой перемещения: конструктор перемещения и оператор присваивания перемещением. В то время как цель семантики копирования состоит в том, чтобы выполнять копирование одного объекта в другой, цель семантики перемещения состоит в том, чтобы переместить владение ресурсами из одного объекта в другой (что менее затратно, чем выполнение операции копирования).

//      ! Определение конструктора перемещения и оператора присваивания перемещением выполняется аналогично определению конструктора копирования и оператора присваивания копированием. Однако, в то время как функции с копированием принимают в качестве параметра константную ссылку l-value, функции с перемещением принимают в качестве параметра неконстантную ссылку r-value.

//      ! Когда вызываются конструктор перемещения и оператор присваивания перемещением?
//      Конструктор перемещения и оператор присваивания перемещением вызываются, когда аргументом для создания или присваивания является r-value. Чаще всего этим r-value будет литерал или временное значение (временный объект).


//  Функция std::move() — это стандартная библиотечная функция, которая конвертирует передаваемый аргумент в r-value. Мы можем передать l-value в функцию std::move(), и std::move() вернет нам ссылку r-value. Для работы с std::move() нужно подключить заголовочный файл utility.

//  In particular, std::move produces an xvalue expression that identifies its argument t. It is exactly equivalent to a static_cast to an rvalue reference type.
//  static_cast<typename std::remove_reference<T>::type&&>(t)


//  ================================== std::swap ===============================
//  template<class T>
//  void swap(T& x, T& y)
//  {
//      T tmp { x }; // вызывает конструктор копирования
//      x = y; // вызывает оператор присваивания копированием
//      y = tmp; // вызывает оператор присваивания копированием
//  }

//  template<class T>
//  void swap(T& x, T& y)
//  {
//      T tmp { std::move(x) }; // вызывает конструктор перемещения
//      x = std::move(y); // вызывает оператор присваивания перемещением
//      y = std::move(tmp); // вызывает оператор присваивания перемещением
//  }


TEST(std_move, std_unique_ptr)
{
    using Type = std::unique_ptr<int>;

    EXPECT_EQ(false, std::is_copy_constructible<Type>::value);
    EXPECT_EQ(false, std::is_copy_assignable<Type>::value);

    EXPECT_EQ(true, std::is_move_constructible<Type>::value);
    EXPECT_EQ(true, std::is_move_assignable<Type>::value);
}


TEST(std_move, std_shared_ptr)
{
    using Type = std::shared_ptr<int>;

    EXPECT_EQ(true, std::is_copy_constructible<Type>::value);
    EXPECT_EQ(true, std::is_copy_assignable<Type>::value);

    EXPECT_EQ(true, std::is_move_constructible<Type>::value);
    EXPECT_EQ(true, std::is_move_assignable<Type>::value);
}


TEST(std_move, std_vector)
{
    using Type = std::vector<int>;

    EXPECT_EQ(true, std::is_copy_constructible<Type>::value);
    EXPECT_EQ(true, std::is_copy_assignable<Type>::value);

    EXPECT_EQ(true, std::is_move_constructible<Type>::value);
    EXPECT_EQ(true, std::is_move_assignable<Type>::value);
}


TEST(std_move, std_mutex)
{
    using Type = std::mutex;

    EXPECT_EQ(false, std::is_copy_constructible<Type>::value);
    EXPECT_EQ(false, std::is_copy_assignable<Type>::value);

    EXPECT_EQ(false, std::is_move_constructible<Type>::value);
    EXPECT_EQ(false, std::is_move_assignable<Type>::value);
}


TEST(std_move, std_thread)
{
    using Type = std::thread;

    EXPECT_EQ(false, std::is_copy_constructible<Type>::value);
    EXPECT_EQ(false, std::is_copy_assignable<Type>::value);

    EXPECT_EQ(true, std::is_move_constructible<Type>::value);
    EXPECT_EQ(true, std::is_move_assignable<Type>::value);
}


struct MovableType
{
    std::unique_ptr<std::string> _str;
};
TEST(std_move, MovableType)
{
    using Type = MovableType;

    EXPECT_EQ(false, std::is_copy_constructible<Type>::value);
    EXPECT_EQ(false, std::is_copy_assignable<Type>::value);

    EXPECT_EQ(true, std::is_move_constructible<Type>::value);
    EXPECT_EQ(true, std::is_move_assignable<Type>::value);

    MovableType v;
}


struct MovableAndCopyableType
{
    std::shared_ptr<std::string> _str;
    std::vector<int> _vec;
};
TEST(std_move, MovableAndCopyableType)
{
    using Type = MovableAndCopyableType;

    EXPECT_EQ(true, std::is_copy_constructible<Type>::value);
    EXPECT_EQ(true, std::is_copy_assignable<Type>::value);

    EXPECT_EQ(true, std::is_move_constructible<Type>::value);
    EXPECT_EQ(true, std::is_move_assignable<Type>::value);
}


class Str
{
    const char* _data;
    size_t _size;
public:
    Str()
        : _data(nullptr)
        , _size(0)
    {
    }

    Str(const char* szStr, size_t size)
        : _data(nullptr)
        , _size(size)
    {
        _data = new char[_size];
        memcpy((void*)_data, szStr, _size);
    }

    Str(const std::string& src)
        : _data(nullptr)
        , _size(src.size())
    {
        _data = new char[_size];
        memcpy((void*)_data, src.data(), _size);
    }

    ~Str()
    {
        delete []_data;
    }

    Str(Str&& src)
    {
        _data = src._data;
        _size = src._size;

        // To prevents the destructor from freeing resources (such as memory) multiple times
        src._data = nullptr;
        src._size = 0;
    }

    Str& operator=(Str&& src)
    {
        if(&src != this)
        {
            _data = src._data;
            _size = src._size;

            // To prevents the destructor from freeing resources (such as memory) multiple times
            src._data = nullptr;
            src._size = 0;
        }

        return *this;
    }

    std::size_t size() const
    {
        return _size;
    }

    std::string to_string() const
    {
        return std::string(_data, _data+_size);
    }
};


template <typename T>
constexpr bool is_lvalue(T&) {
    return true;
}

template <typename T>
constexpr bool is_lvalue(T&&) {
    return false;
}


TEST(std_move, example)
{
    using Type = Str;

    Type a("abc", 3);
    EXPECT_EQ("abc", a.to_string());
    EXPECT_EQ(3, a.size());
    EXPECT_EQ(true, is_lvalue(a));
    EXPECT_EQ(false, is_lvalue(std::move(a)));

    Type b = std::move(a);
    EXPECT_EQ("abc", b.to_string());
    EXPECT_EQ(3, b.size());
    EXPECT_EQ(true, is_lvalue(b));
    EXPECT_EQ(false, is_lvalue(std::move(b)));

    EXPECT_EQ("", a.to_string());
    EXPECT_EQ(0, a.size());
}

