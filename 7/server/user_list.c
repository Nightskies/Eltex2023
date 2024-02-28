#include "user_list.h"
#include "fatal.h"

#include <string.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void user_list_push(user_list_t *list, user_t data)
{
	int err_code = pthread_mutex_lock(&mutex);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

    node_t *new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) {
        ERROR("malloc return NULL");
    }
    new_node->data.socket = data.socket;
    new_node->data.tid = data.tid;
    new_node->next = list->head;
    list->head = new_node;

	err_code = pthread_mutex_unlock(&mutex);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }
}

void user_list_remove(user_list_t *list, int socket)
{
    node_t *head = NULL;

    int err_code = pthread_mutex_lock(&mutex);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

    if (list->head->data.socket == socket) {
        head = list->head;
        list->head = head->next;
    } else {
        node_t *prev = list->head;
        head = prev->next;
        while (head != NULL) {
            if (head->data.socket == socket){
                break;
            }
            prev = head;
            head = head->next;
        }

        if (head == NULL) {
            ERROR("Tried to delete a non-existent user");
        }

        prev->next = head->next;
    }

    err_code = pthread_mutex_unlock(&mutex);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }
    
    free(head);
}