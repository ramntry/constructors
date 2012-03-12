#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

int WIDTH = 12; // переменная, управляющая шириной текстовых полей при выводе сообщений
                // для вывода в стандартной 80-колоночной консоли установите значение 0

// Макрос, необходимый для работы скрипта constructors - он обеспечивает согласование
// номеров строк исходного кода main() и генерируемого ими вывода
#define PRINT_LINE_NUMBER(num) cout << endl << setw(6) << right << num << "  ";

// Макросы, которыми следуем обрамлять тело main() для корректной работы скрипта
// constructors
#define MAIN int main(int argc, char **argv) {
#define ENDMAIN }

using namespace std;

// Преобразование целого числа в STL строку
string itos(int value)
{
    stringstream ss;
    ss << value;
    return ss.str();
}

// Истинное значение подавляет вывод диагностических сообщений
// из констукторов, оператора присваивания и деструктора класса Base
bool supressBase = true;

// Таблица удобных в использовании строковых констант
const char* sc = "[SCRATCH]";  // Используется для обозначения неинициализированной памяти
const char* un = "[UNNAMED]";
const char* dc = "default constructor";
const char* cp = "copy constructor";
const char* op = "operator =";
const char* ds = "destructor";
// Генератор типичного суффикса для имени копии объекта, состоящего
// из уникального идентификатора блока памяти, отведенного под объект
// и слова copy
string cf(int memId)
{
    return ":" + itos(memId) + " copy";
}

// Базовый класс, наследование от которого необходимо для корректной работы
class Base
{
public:
    // Каждый из конструкторов, занимая ранее не инициализированную память,
    // присваивает ей уникальный номер memId, уникальность которого поддерживается
    // инкрементированим статического поля ids. Каждый объект в системе помимо
    // константного поля memId, не меняющегося при присваиваниях (operator =),
    // имеет собственное имя. Будем считать, что имя еще неинициализированной
    // памяти sc ("[SCRATCH]")
    Base(const char* _name = un)
        : name(sc)
        , memId(ids++)
    {
        // Рапорт о происходящем: в каком объекте, как происходящее повлияет на
        // количество экземпляров данного класса, в каком методе находится
        // управление (dc = "default constructor"), каким станет имя объекта
        // по завершении метода.
        printStatus(this, count() + 1, dc, _name);
        count()++;    // честно выполняем
        name = _name; // обещанное
    }

    Base(Base const& src)
        : name(sc)
        , memId(ids++)
    {
        printStatus(this, count() + 1, cp, src.name + cf(src.memId));
        count()++;
        name = src.name + cf(src.memId);
    }

    Base &operator =(Base const& src)
    {
        printStatus(this, count(), op, src.name + cf(src.memId));
        name = src.name + cf(src.memId);
    }

    ~Base()
    {
        printStatus(this, count() - 1, ds, sc);
        count()--;
    }

protected:
    string name;
    const int memId;

private:
    static int ids;

friend
    void printStatus(Base *obj, int newCount, const char* method, string newName);

// Макрос, позволяющий быстро доопределять классы, производные от Base, образом,
// восполняющим невозможность в C++ определять виртуальные поля. Проблема связана
// с тем, что поля _className и _count статические, но индивидуальные для каждого
// класса (что ясно), однако хотелось бы их адекватно полиморфно менять.
#define CLOSE_CLASS(ClassName)                       \
    public:                                          \
        virtual const char* className()              \
        { return _className; }                       \
                                                     \
        virtual int &count()                         \
        { return _count; }                           \
                                                     \
    private:                                         \
        static const char* _className;               \
        static int _count;                           \
    };                                               \
    const char* ClassName::_className = #ClassName;  \
    int ClassName::_count = 0;
CLOSE_CLASS(Base)

int Base::ids = 0;

void printStatus(Base *obj, int newCount, const char* method, string newName)
{
    if (supressBase && obj->className() == "Base")
        return;
    cout << setw(8 + WIDTH) << right << obj->className()
         << "[" << setw(2) << right << obj->count() << " -> " << setw(2) << left << newCount << "]: "
         << setw(20) << left << method << "("
         << setw(11 + 2*WIDTH) << right << obj->name << ":" << setw(3) << left << obj->memId <<  " -> "
         << setw(11 + 2*WIDTH) << left << newName << ")"
         << endl;
}

