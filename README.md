# Pascal_interpritator

Интерпретатор паскаля. На вход интерпретатора подаётся файл, содержащий программу на языке паскаль. Интерпретатор обрабатывает полученную на вход програму,
если программа написана некорректно, то выдает ошибку(и объяснение этой ошибки(номер строки, этап выполения программы и тд), так же как и стандарный компилятор паскаля), если программа написана корректно, то выдает результат её выполенения.

Предложено 5 тестовых программ на языке паскаль, которые тестирую основные функции интерпретатора(арифметические операции, условные операторы, работу с типами, циклы).

Запуск:

g++ -std=c++14 -Wall -o main main.cpp

./main file



Что реализованно:

Тип boolean, 

Тип integer, 

Тип string,  

Оператор чтения read() для string и integer, 

Оператор вывода на экран write() всех типов, 

Арифмитические операции + - /(целочисленное) * (без унарных), 

Логические операции and or not ,

Операция сложения строк, 

Операции сравнения <> >= <= < > =, 

Оператор условия if then else, 

Оператор условия if then, 

Оператор цикла while do, 

Оператор составной begin end, 

Значаения всех переменных по умолчанию 0-int false-bool ""-string

Грамматика языка прилагается (фото Syntax.jpg)


Добавлены 5 тестов:
test0 - {Tested if else block}

test1 - Вводится N>0 c клавиатуры выводит сумму от 1 до N

test2 - {write  5 to 1}

test3 - Вводится строка (имя) выводит Your name is ...(имя)

test4 - {test operations} Выводит значание выражений x:=10*10-50+(1+1)/2; {==51} и true and true {true} 

Желаемая оценка 5

Компилятор:
g++ -std=c++14 
