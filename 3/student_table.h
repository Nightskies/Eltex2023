#ifndef STUDENT_TABLE_H
#define STUDENT_TABLE_H

#include "student.h"

#include <stddef.h>

typedef struct
{
    student_t** table;
    size_t size;
    size_t max_size;
} student_table_t;

student_table_t* student_table_new();
void student_table_delete(student_table_t* self);

void student_table_ctor(student_table_t* self, size_t max_size);
void student_table_file_ctor(student_table_t* self, const char* file_path);
void student_table_dtor(student_table_t* self);

void student_table_remove_record(student_table_t* self, void* target, student_field_t field_type);

void student_table_add_record_from_terminal(student_table_t* self);

void student_table_save(student_table_t* self, const char* file_path);

void student_table_sort(student_table_t* self, student_field_t field_type);

student_t* student_table_find(student_table_t* self, void* target, student_field_t field_type);

student_table_t* student_table_group_by(student_table_t* self, void* target, student_field_t field_type);

void student_table_print(student_table_t* self);

#endif // STUDENT_TABLE_H