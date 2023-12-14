#include "student_table.h"

#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 4096

char buf[BUFFER_SIZE] = {};

student_t* menu_find(student_table_t* table, const char* msg);

int main()
{
    student_table_t* table = student_table_new();
    student_table_ctor(table, 10);

    unsigned item = 0u;
    student_t* s = NULL;

    student_table_t* new_table = NULL;

    while (1)
    {
        printf("Menu\n");
        printf("1) Add new record\n");
        printf("2) Remove record\n");
        printf("3) Sort\n");
        printf("4) Print table\n");
        printf("5) Find record\n");
        printf("6) Change record\n");
        printf("7) Group by\n");
        printf("8) Load table from file\n");
        printf("9) Save table to file\n");
        printf("10) Exit\n");

        scanf("%u", &item);
        system("clear");
        switch (item)
        {
        case 1:
            student_table_add_record_from_terminal(table);
            break;
        case 2:
            s = menu_find(table, "Find record to delete by");
            student_table_remove_record(table, s);
            break;
        case 3:
            printf("Sort by\n");
            printf("1) Surname\n");
            printf("2) Faculty\n");
            printf("3) Group\n");
            printf("4) Record book number\n");
            scanf("%u", &item);
            switch (item)
            {
            case 1:
                student_table_sort(table, SURNAME);
                break;
            case 2:
                student_table_sort(table, FACULTY);
                break;
            case 3:
                student_table_sort(table, GROUP);
                break;
            case 4:
                student_table_sort(table, RECORD_BOOK_NUMBER);
                break;
            default:
                break;
            }
            break;
        case 4:
            student_table_print(table);
            printf("Table size[%zu] Table capacity[%zu]\n", table->size, table->max_size);
            printf("Enter q to back in menu\n");
            while (getchar() != 'q')
            {
            }
            break;
        case 5:
            s = menu_find(table, "Find by");
            system("clear");
            student_print(s);
            printf("Enter q to back in menu\n");
            while (getchar() != 'q')
            {
            }
            break;
        case 6:
            s = menu_find(table, "Find record to change by");
            int f = 1;
            while (f)
            {
                system("clear");
                student_print(s);
                printf("What change?\n");
                printf("1) Surname\n");
                printf("2) Faculty\n");
                printf("3) Group\n");
                printf("4) Record book number\n");
                printf("5) Back in menu\n");
                scanf("%u", &item);
                system("clear");
                switch (item)
                {
                case 1:
                    printf("Enter surname:");
                    scanf("%s", buf);
                    student_set_field(s, buf, SURNAME);
                    break;
                case 2:
                    printf("Enter faculty:");
                    scanf("%s", buf);
                    student_set_field(s, buf, FACULTY);
                    break;
                case 3:
                    printf("Enter group:");
                    scanf("%u", &item);
                    student_set_field(s, &item, GROUP);
                    break;
                case 4:
                    printf("Enter record book number:");
                    scanf("%u", &item);
                    student_set_field(s, &item, RECORD_BOOK_NUMBER);
                    break;
                case 5:
                    f = 0;
                    break;
                default:
                    break;
                }
            }
            break;
        case 7:
            printf("Group by\n");
            printf("1) Surname\n");
            printf("2) Faculty\n");
            printf("3) Group\n");
            printf("4) Record book number\n");
            scanf("%u", &item);
            system("clear");
            switch (item)
            {
            case 1:
                printf("Enter surname:");
                scanf("%s", buf);
                new_table = student_table_group_by(table, buf, SURNAME);
                break;
            case 2:
                printf("Enter faculty:");
                scanf("%s", buf);
                new_table = student_table_group_by(table, buf, FACULTY);
                break;
            case 3:
                printf("Enter group:");
                scanf("%u", &item);
                new_table = student_table_group_by(table, &item, GROUP);
                break;
            case 4:
                printf("Enter record book number:");
                scanf("%u", &item);
                new_table = student_table_group_by(table, &item, RECORD_BOOK_NUMBER);
                break;
            default:
                break;
            }
            system("clear");
            student_table_print(new_table);
            printf("Enter q to back in menu\n");
            while (getchar() != 'q')
            {
            }
            student_table_dtor(new_table);
            student_table_delete(new_table);
            break;
        case 8:
            if (table->size > 0)
            {
                printf("Table already has elements\n");
                printf("1) Rewrite\n");
                printf("2) Add to the end\n");
                scanf("%u", &item);
                if (item == 1)
                {
                    table->size = 0;
                }
                system("clear");
            }
            printf("Enter file path:");
            scanf("%s", buf);
            student_table_file_ctor(table, buf);
            break;
        case 9:
            printf("Enter file path:");
            scanf("%s", buf);
            student_table_save(table, buf);
            break;
        case 10:
            student_table_dtor(table);
            student_table_delete(table);
            return 0;
        default:
            break;
        }

        system("clear");
    }
}

student_t* menu_find(student_table_t* table, const char* msg)
{
    student_t* s = NULL;
    unsigned item = 0u;

    printf("%s\n", msg);
    printf("1) Surname\n");
    printf("2) Faculty\n");
    printf("3) Group\n");
    printf("4) Record book number\n");
    scanf("%u", &item);
    system("clear");
    switch (item)
    {
    case 1:
        printf("Enter surname:");
        scanf("%s", buf);
        s = student_table_find(table, buf, SURNAME);
        break;
    case 2:
        printf("Enter faculty:");
        scanf("%s", buf);
        s = student_table_find(table, buf, FACULTY);
        break;
    case 3:
        printf("Enter group:");
        scanf("%u", &item);
        s = student_table_find(table, &item, GROUP);
        break;
    case 4:
        printf("Enter record book number:");
        scanf("%u", &item);
        s = student_table_find(table, &item, RECORD_BOOK_NUMBER);
        break;
    default:
        break;
    }

    return s;
}