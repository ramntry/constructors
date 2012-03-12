/**
 * Пример использования constructors, освещающий некоторые аспекты
 * работы виртуальных функций и особенности корректного удаления
 * полиморфных объектов.
 */

#include "constructors.h"

// Макрос, определяющий простой класс с конструктором и деструктором
#define NEW_CLASS(baseClass, className, virtualWord)                        \
class className : public baseClass                                          \
{                                                                           \
public:                                                                     \
    className(const char* _name = un)                                       \
    {                                                                       \
        printStatus(this, count() + 1, dc, _name);                          \
        count()++;                                                          \
        name = _name;                                                       \
    }                                                                       \
                                                                            \
    virtualWord ~className()                                                \
    {                                                                       \
        printStatus(this, count() - 1, ds, sc);                             \
        count()--;                                                          \
    }

//Ниже объявлена цепочка наследующих друг друга классов:
NEW_CLASS(Base, A, )
CLOSE_CLASS(A)

NEW_CLASS(A, B, virtual)
CLOSE_CLASS(B)

NEW_CLASS(B, C, )
CLOSE_CLASS(C)

NEW_CLASS(C, D, )
CLOSE_CLASS(D)


MAIN
    WIDTH = 17; // зададим подходящую ширину вывода для длинных имен наших объектов
    
    cout << "-------------------------------------------------------------------" << endl;

    A *aA = new A("A*->A, non virtual destructor");
    A *bA = new B("A*->B, explicitly defined virtual destructor");
    A *cA = new C("A*->C, ordinary destructor is still virtual");
    A *dA = new D("A*->D, and here too");

    delete aA;  // видно, что здесь для всех объеков вызываются
    delete bA;  // только деструктор класса A, так как он в нем объявлен
    delete cA;  // невиртуальным, а указатели на эти объекты имеют тип A*
    delete dA;  // Это некорректное удаление объектов.

    cout << "-------------------------------------------------------------------" << endl;

    B *bB = new B("B*->B, explicitly defined virtual destructor");
    B *cB = new C("B*->C, ordinary destructor is still virtual");
    B *dB = new D("B*->D, and here too");

    delete bB;  // Здесь же можно наблюдать, что благодаря виртуальности
    delete cB;  // деструктора вызывается напрямую именно нужный, в зависимости
    delete dB;  // от типа объекта, реально расположенного по указателю. Деструкторы
                // всех базовых классов он вызывает уже сам, как обычно, в порядке,
                // строго обратном порядку вызова конструкторов.
                
    cout << "-------------------------------------------------------------------" << endl;
 
    C *cC = new C("C*->C, ordinary destructor is still virtual");
    C *dC = new D("C*->D, and here too");

    delete cC;  // Важный момент: деструктор класа C вовсе не объявлен виртуальным,
    delete dC;  // однако он виртуальный, т. к. является перегрузкой виртуального в 
                // классе-предке.


    return 0;                                                                
ENDMAIN
