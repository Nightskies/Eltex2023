#include <stdio.h>
#include <string.h>

#define PASSWD_PATH "/etc/passwd"
#define LINE_SIZE 256
#define PASSWD_ATTRIB_NUM 7

char** get_passwd_tokens(char* str, char delim)
{
    static char* tokens[PASSWD_ATTRIB_NUM] = {};

    if (!str)
    {
        return NULL;
    }

    for (int i = 0; i < PASSWD_ATTRIB_NUM; i++)
    {
        if (*str == delim)
        {
            tokens[i] = NULL;
            ++str;
            continue;
        }

        tokens[i] = str;

        while (*str != '\0')
        {
            if (*str == delim || *str == '\n')
            {
                *str++ = '\0';
                break;
            }

            ++str;
        }
    }

    return tokens;
}

void print_real_name(const char* login)
{
    FILE* file = fopen(PASSWD_PATH, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Can't open file\n");
        return;
    }

    char str[LINE_SIZE] = {};
    while (!feof(file))
    {
        if (fgets(str, LINE_SIZE, file))
        {
            char** tokens = get_passwd_tokens(str, ':');

            if (strncmp(*tokens, login, sizeof(login)) == 0)
            {
                printf("%s\n", tokens[4]);
                break;
            }
        }

    }
    fclose(file);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "You need to enter your login\n");
        return -1;
    }

    print_real_name(argv[1]);

    return 0;
}