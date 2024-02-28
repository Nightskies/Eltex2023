#include "fatal.h"
#include "user_list.h"

#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <signal.h>

#define MAX_USERS            5
#define MAX_LISTENERS        10
#define MAX_HOST_NAME_LENGTH 128
#define BUF_LENGTH           4096

static user_list_t list = {0};
static int num_users = 0;

static int socket_listen;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void get_uppercase_string(char *str, size_t length);
static void server_close(int sig);
static void* chat(void *arg);

int main(int argc, char* argv[])
{
    int err_code;
    pthread_t id;

    signal(SIGINT, server_close);

    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;

    struct addrinfo* bind_addr;
    err_code = getaddrinfo(0, "6665", &hint, &bind_addr);
    if (err_code < 0) {
        ERROR(gai_strerror(errno));
    }

    puts("Create socket...");
    socket_listen = socket(bind_addr->ai_family, bind_addr->ai_socktype, bind_addr->ai_protocol);
    if (socket_listen < 0) {
        ERROR(strerror(errno));
    }

    puts("Bind socket..");
    err_code = bind(socket_listen, bind_addr->ai_addr, bind_addr->ai_addrlen);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }
    freeaddrinfo(bind_addr);

    puts("Listening...");
    err_code = listen(socket_listen, MAX_LISTENERS);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

    for (;;) {
        user_t user;
        struct sockaddr_storage client_addr;
        socklen_t client_len = sizeof(struct sockaddr_storage);
        user.socket = accept(socket_listen, (struct sockaddr*)&client_addr, &client_len);
        if (user.socket < 0) {
            ERROR(strerror(errno));
        }

        err_code = pthread_mutex_lock(&mutex);
        if (err_code < 0) {
        	ERROR(strerror(errno));
        }
	
        char *msg = NULL;
    	if (num_users == MAX_USERS) {
            msg = "FULL";
            err_code = send(user.socket, msg, sizeof(msg), 0);
			if (err_code < 0) {
				ERROR(strerror(errno));
            }

            err_code = pthread_mutex_unlock(&mutex);
            if (err_code < 0) {
        	    ERROR(strerror(errno));
            }

			continue;
		} else {
            msg = "CONNECT";

            err_code = send(user.socket, msg, sizeof(msg), 0);
			if (err_code < 0) {
				ERROR(strerror(errno));
            }

            ++num_users;
        }
        
    	err_code = pthread_mutex_unlock(&mutex);
        if (err_code < 0) {
        	ERROR(strerror(errno));
        }
    	
        char addr[MAX_HOST_NAME_LENGTH] = {};
        err_code = getnameinfo((struct sockaddr*)&client_addr, client_len, addr, 
            MAX_HOST_NAME_LENGTH, 0, 0, NI_NUMERICHOST);
        if (err_code < 0) {
            ERROR(gai_strerror(errno));
        }
        printf("New connection from %s\n", addr);

        err_code = pthread_create(&id, NULL, chat, (void*)&user);
        if (err_code < 0) {
            ERROR(strerror(errno));
        }
    }

    return 0;
}


static void get_uppercase_string(char *str, size_t length)
{
    if (str == NULL) {
        return;
    }

    for (int i = 0; i < length; ++i) {
        str[i] = toupper(str[i]);
    }
}

static void* chat(void *arg)
{
    int err_code = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

	pthread_t tid = pthread_self();
    err_code = pthread_detach(tid);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

    user_t user = *(user_t*)arg;
    user.tid = tid;

    user_list_push(&list, user);

    char buf[BUF_LENGTH] = {};
    for (;;) {
        ssize_t rbytes = recv(user.socket, buf, BUF_LENGTH, 0);
        if (rbytes < 1) {
            if (rbytes == 0) { // Connection closed by client
                user_list_remove(&list, user.socket);
                err_code = pthread_mutex_lock(&mutex);
                if (err_code < 0) {
        	        ERROR(strerror(errno));
                }

                --num_users;

                err_code = pthread_mutex_unlock(&mutex);
                if (err_code < 0) {
        	        ERROR(strerror(errno));
                }

                err_code = close(user.socket);
                if (err_code < 0) {
                    ERROR(strerror(errno));
                }
                break;
            }
            ERROR(strerror(errno));
        }

        get_uppercase_string(buf, rbytes);

        ssize_t sbytes = send(user.socket, buf, rbytes, 0);
        if (sbytes < 0) {
            ERROR(strerror(errno));
        }
    }
}

static void server_close(int sig)
{
	puts("Server closing...");

    int err_code = pthread_mutex_lock(&mutex);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

    node_t *head = list.head;
    while (head) {
        err_code = pthread_cancel(head->data.tid);
        if (err_code < 0) {
            ERROR(strerror(errno));
        }
        head = head->next;
    }

    err_code = pthread_mutex_unlock(&mutex);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

    head = list.head;
    while (head) {
        node_t *next = head->next;
        free(head);
        head = next;
    }

	err_code = close(socket_listen);
    if (err_code < 0) {
        ERROR(strerror(errno));
    }

	exit(EXIT_SUCCESS);
}