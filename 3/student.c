#include "student.h"

#include <stdlib.h>
#include <string.h>

// Private functions

void student_set_surname(student_t* self, const char* surname)
{
    memcpy(self->surname, surname, SURNAME_SIZE);
}

void student_set_faculty(student_t* self, const char* faculty)
{
    memcpy(self->faculty, faculty, FACULTY_SIZE);
}

void student_set_group(student_t* self, unsigned group)
{
    self->group = group;
}

void student_set_record_book_number(student_t* self, unsigned record_book_number)
{
    self->record_book_number = record_book_number;
}

// Public functions
student_t* student_new()
{
    student_t* student = (student_t*)malloc(sizeof(student_t));
    if (student == NULL)
    {
        fprintf(stderr, "malloc return NULL\n");
        exit(EXIT_FAILURE);
    }

    return student;
}

void student_delete(student_t* self)
{
    if (self)
    {
        free(self);
    }
}

void student_terminal_ctor(student_t* self)
{
    printf("Enter surname:");
    scanf("%s", self->surname);
    printf("Enter faculty:");
    scanf("%s", self->faculty);
    printf("Enter group number:");
    scanf("%u", &self->group);
    printf("Enter record book number:");
    scanf("%u", &self->record_book_number);
}

int student_file_ctor(student_t* self, FILE* stream)
{
    return fscanf(stream, "%s %s %u %u", self->surname, self->faculty, &self->group, &self->record_book_number);
}

void student_copy_ctor(student_t* self, const student_t* other)
{
    memcpy(self, other, sizeof(student_t));
}

void student_ctor(student_t* self)
{
    memset(self, 0, sizeof(student_t));
}

void student_set_field(student_t* self, void* field, student_field_t field_type)
{
    switch (field_type)
    {
    case SURNAME:
        student_set_surname(self, (const char*)field);
        break;
    case FACULTY:
        student_set_faculty(self, (const char*)field);
        break;
    case GROUP:
        student_set_group(self, *(unsigned*)field);
        break;
    case RECORD_BOOK_NUMBER:
        student_set_record_book_number(self, *(unsigned*)field);
        break;
    }
}

void student_print(student_t* self)
{
    printf("Surname:%s\n", self->surname);
    printf("Faculty:%s\n", self->faculty);
    printf("Group:%u\n", self->group);
    printf("Read book number:%u\n\n", self->record_book_number);
}