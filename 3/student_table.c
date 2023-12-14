#include "student_table.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef int (*compare)(const void* a, const void* b);
compare qsort_compare_func[FIELDS_NUMBER] = {};
compare compare_func[FIELDS_NUMBER] = {};

/// Utility private functions

int utils_str_compare(const char* a, const char* b)
{
    return strcmp(a, b);
}

int utils_int_compare(const int a, const int b)
{
    if (a > b)
    {
        return 1;
    }
    else if (a < b)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/// Private functions

int _surname_compare(const void* a, const void* b)
{
    return utils_str_compare(((student_t*)a)->surname, ((student_t*)b)->surname);
}

int _faculty_compare(const void* a, const void* b)
{
    return utils_str_compare(((student_t*)a)->faculty, ((student_t*)b)->faculty);
}

int _group_compare(const void* a, const void* b)
{
    return utils_int_compare(((student_t*)a)->group, ((student_t*)b)->group);
}

int _record_book_number_compare(const void* a, const void* b)
{
    return utils_int_compare(((student_t*)a)->record_book_number, ((student_t*)b)->record_book_number);
}

int _qsort_surname_compare(const void* a, const void* b)
{
    return _surname_compare(*(student_t**)a, *((student_t**)b));
}

int _qsort_faculty_compare(const void* a, const void* b)
{
    return _faculty_compare(*(student_t**)a, *((student_t**)b));
}

int _qsort_group_compare(const void* a, const void* b)
{
    return _group_compare(*(student_t**)a, *((student_t**)b));
}

int _qsort_record_book_number_compare(const void* a, const void* b)
{
    return _record_book_number_compare(*(student_t**)a, *((student_t**)b));
}

void _student_table_realloc(student_table_t* self)
{
    if (self->max_size == 0)
        self->max_size = 1;

    self->table = (student_t**)realloc(self->table, sizeof(student_t*) * (self->max_size * 2));
    if (self->table == NULL)
    {
        fprintf(stderr, "realloc return NULL\n");
        exit(EXIT_FAILURE);
    }
    self->max_size *= 2;

    for (int i = self->size; i < self->max_size; i++)
    {
        self->table[i] = student_new();
    }
}

void _student_table_add_record(student_table_t* self, student_t* record)
{
    if (self->size < self->max_size)
    {
        student_copy_ctor(self->table[self->size++], record);
    }
}

// Public functions

student_table_t* student_table_new()
{
    student_table_t* new_table = (student_table_t*)malloc(sizeof(student_table_t));
    if (new_table == NULL)
    {
        fprintf(stderr, "malloc return NULL\n");
        exit(EXIT_FAILURE);
    }
    return new_table;
}

void student_table_delete(student_table_t* self)
{
    if (self)
    {
        free(self);
    }
}

void student_table_ctor(student_table_t* self, size_t max_size)
{
    self->table = (student_t**)calloc(max_size, sizeof(student_t*));
    if (self->table == NULL)
    {
        fprintf(stderr, "malloc return NULL\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < max_size; i++)
    {
        self->table[i] = student_new();
    }

    self->size = 0;
    self->max_size = max_size;

    compare_func[SURNAME] = _surname_compare;
    compare_func[FACULTY] = _faculty_compare;
    compare_func[GROUP] = _group_compare;
    compare_func[RECORD_BOOK_NUMBER] = _record_book_number_compare;

    qsort_compare_func[SURNAME] = _qsort_surname_compare;
    qsort_compare_func[FACULTY] = _qsort_faculty_compare;
    qsort_compare_func[GROUP] = _qsort_group_compare;
    qsort_compare_func[RECORD_BOOK_NUMBER] = _qsort_record_book_number_compare;
}

void student_table_dtor(student_table_t* self)
{
    if (self)
    {
        if (self->table)
        {
            for (int i = 0; i < self->max_size; i++)
            {
                student_delete(self->table[i]);
            }

            free(self->table);
        }
    }
}

void student_table_add_record_from_terminal(student_table_t* self)
{
    if (self->size == self->max_size)
    {
        _student_table_realloc(self);
    }

    student_terminal_ctor(self->table[self->size++]);
}

void student_table_file_ctor(student_table_t* self, const char* file_path)
{
    FILE* file = fopen(file_path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "File error\n");
        exit(EXIT_FAILURE);
    }

    student_t tmp;

    while (1)
    {
        int result = student_file_ctor(&tmp, file);
        if (result == EOF)
            break;

        if (self->size == self->max_size)
        {
            _student_table_realloc(self);
        }

        student_copy_ctor(self->table[self->size++], &tmp);
    }
}

void student_table_save(student_table_t* self, const char* file_path)
{
    FILE* file = fopen(file_path, "w");
    if (file == NULL)
    {
        fprintf(stderr, "fopen return NULL\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < self->size; i++)
    {
        fprintf(file, "%s\n%s\n%u\n%u\n\n", self->table[i]->surname, self->table[i]->faculty,
            self->table[i]->group, self->table[i]->record_book_number);
    }
}

void student_table_sort(student_table_t* self, student_field_t type)
{
    qsort(self->table, self->size, sizeof(student_t*), qsort_compare_func[type]);
}

student_t* student_table_find(student_table_t* self, void* target, student_field_t field_type)
{
    student_t* closest_left = NULL;
    student_t* closest_right = NULL;

    student_t tmp;
    student_ctor(&tmp);

    student_set_field(&tmp, target, field_type);

    for (int i = 0; i < self->size; i++)
    {
        int result = compare_func[field_type](self->table[i], &tmp);
        if (result == -1)
        {
            if (closest_left)
            {
                result = compare_func[field_type](closest_left, self->table[i]);
                if (result == -1)
                {
                    closest_left = self->table[i];
                }
            }
            else
            {
                closest_left = self->table[i];
            }
        }
        else if (result == 1)
        {
            if (closest_right)
            {
                result = compare_func[field_type](closest_right, self->table[i]);
                if (result == -1)
                {
                    closest_right = self->table[i];
                }
            }
            else
            {
                closest_right = self->table[i];
            }
        }
        else
        {
            return self->table[i];
        }
    }

    return closest_left ? closest_left : closest_right;
}

void student_table_remove_record(student_table_t* self, void* target, student_field_t field_type)
{
    student_t tmp;
    student_ctor(&tmp);

    student_set_field(&tmp, target, field_type);

    for (int i = 0; i < self->size; i++)
    {
        int result = compare_func[field_type](self->table[i], &tmp);
        if (result == 0)
        {
            self->table[i] = self->table[--self->size];
        }
    }
}

student_table_t* student_table_group_by(student_table_t* self, void* target, student_field_t field_type)
{
    student_table_t* new_table = student_table_new();
    student_table_ctor(new_table, self->size);

    student_t tmp;
    student_ctor(&tmp);

    student_set_field(&tmp, target, field_type);

    for (int i = 0; i < self->size; i++)
    {
        if (compare_func[field_type](self->table[i], &tmp) == 0)
        {
            _student_table_add_record(new_table, self->table[i]);
        }
    }
    return new_table;
}

void student_table_print(student_table_t* self)
{
    for (int i = 0; i < self->size; i++)
    {
        student_print(self->table[i]);
    }
}