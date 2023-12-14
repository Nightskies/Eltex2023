#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>

#define SURNAME_SIZE 16
#define FACULTY_SIZE 8

typedef enum
{
    SURNAME,
    FACULTY,
    GROUP,
    RECORD_BOOK_NUMBER,

    FIELDS_NUMBER
} student_field_t;

typedef struct
{
    char surname[SURNAME_SIZE];
    char faculty[FACULTY_SIZE];
    unsigned group;
    unsigned record_book_number;
} student_t;

student_t* student_new();
void student_delete(student_t* self);

void student_terminal_ctor(student_t* self);
int student_file_ctor(student_t* self, FILE* stream);
void student_copy_ctor(student_t* self, const student_t* other);
void student_ctor(student_t* self);

void student_set_field(student_t* self, void* field, student_field_t field_type);

void student_print(student_t* self);

#endif // STUDENT_H