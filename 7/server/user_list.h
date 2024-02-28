#ifndef USER_LIST_H
#define USER_LIST_H

#include <pthread.h>

typedef struct
{
    pthread_t tid;
    int socket;
} user_t;

typedef struct node
{
    user_t data;
    struct node *next;
} node_t;

typedef struct user_list
{
    node_t *head;
} user_list_t;

void user_list_push(user_list_t *list, user_t data);
void user_list_remove(user_list_t *list, int socket);

#endif // USER_LIST_H