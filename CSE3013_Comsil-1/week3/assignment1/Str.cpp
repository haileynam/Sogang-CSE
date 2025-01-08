#include "Str.h"
#include <iostream>
#include <cstring>

using namespace std;

Str::Str(int leng) // leng은 string의 길이.
{
    if (leng < 0)
        cout << "Length of String cannot be negative!" << endl;

    str = new char[leng + 1];
    str[0] = '\0';
    len = leng;
}

Str::Str(char *neyong) // neyong은 초기화할 내용이 들어감.
{
    len = strlen(neyong);
    str = new char[len + 1];
    strcpy(str, neyong);
    str[len]='\0';
}

Str::~Str() // 소멸자.
{
    delete[] str;
}

int Str::length() // string의 길이를 리턴하는 함수.
{
    return len;
}

char *Str::contents() // string의 내용을 리턴하는 함수.
{
    return str;
}

int Str::compare(class Str &a) // a의 내용과 strcmp.
{
    return strcmp(str, a.contents());
}

int Str::compare(char *a) // a의 내용과 strcmp.
{
    return strcmp(str, a);
}

void Str::operator = (char *a) // string의 값을 대입.
{
    if (strlen(a) < 0)
        cout << "NULL!" << endl;
    else 
    {
        delete[] str;
        len = strlen(a);
        str = new char[len + 1];
        strcpy(str, a);
        str[len] = '\0';
    }
}

void Str::operator = (class Str &a) // Str의 내용을 대입.
{
    if (a.length() < 0)
        cout << "NULL!" << endl;
    else 
    {
        delete[] str;
        len = a.length();
        str = new char[len + 1];
        strcpy(str, a.contents());
    }
}