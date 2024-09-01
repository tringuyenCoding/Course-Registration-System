#ifndef CS162_PROJECT_FWD
#define CS162_PROJECT_FWD
#include "info.h"

struct Student;
struct Staff;
struct SchoolYear;

template <class T>
void deleteList(T *&head)
{
    while (head)
    {
        T *temp = head;
        head = head->next;
        delete temp;
    }
}

template <class T>
void reverseList(T *&head)
{
    T *previous = nullptr;

    while (head)
    {
        T *next = head->next;
        head->next = previous;
        previous = head;
        head = next;
    }

    head = previous;
}

#endif /* CS162_PROJECT_FWD */
