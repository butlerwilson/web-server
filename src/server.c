#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <netdb.h>

// include all definations about server and functions define
#include "server.h"
#include "infoip.h"

#include "mysql_oper.h"

<<<<<<< HEAD
#include "message.h"

=======
>>>>>>> origin/master
// return errors to the broswer
void response_error_to_client(int cfd, int errno)
{
		char *errbuf = NULL;
		int fd, res;
		char *filename = NULL;
		struct stat st;
		char file_buff[FILE_MAX_SIZE];
		pthread_mutex_t lp;

		file_buff[0] = '\0';
		switch (errno) {
			// request page not found 404
			case 1:
<<<<<<< HEAD
				errbuf= err_404;
				filename = page_404;
				break;
			// request forbiden 403
			case 2:
				errbuf = err_403;
				filename = page_403;
				break;
			// unknow method 405
			case 3:
				errbuf = err_405;
				filename = page_405;
=======
				errbuf= "HTTP/1.1 404 Not Found\n";
				filename = "../www/_404.html";
				break;
			// request forbiden 403
			case 2:
				errbuf = "HTTP/1.1 403 Resquest forbiden\n";
				filename = "../www/_403.html";
				break;
			// unknow method 405
			case 3:
				errbuf = "HTTP/1.1 405 unknow method\n";
				filename = "../www/_405.html";
>>>>>>> origin/master
				break;
			// file too large
			case 4:
			// url too long
			case 5:
			// unsuport mime type
			case 6:
			// unsuport http protocol
<<<<<<< HEAD
				errbuf = err_505;
				filename = page_505;
				break;
			default:
				errbuf= err_404;
				filename = page_404;
=======
				errbuf = "HTTP/1.1 505 Unsupport http protocol\n";
				filename = "../www/_505.html";
				break;
			default:
				errbuf= "HTTP/1.1 404 Not Found\n";
				filename = "../www/_404.html";
>>>>>>> origin/master
		}
		strncpy(http_head_failed, errbuf, strlen(errbuf));
		fd = open(filename, O_RDONLY);
		res = fstat(fd, &st);
		if ((fd == -1) || (res == -1))
<<<<<<< HEAD
			strcpy(file_buff, err_fatal);
=======
			strcpy(file_buff, "<html><p>Fatel Error!</p><h>No reponse! please check your request.</h></html>");
>>>>>>> origin/master
		else
			read(fd, file_buff, res);
		if (pthread_mutex_init(&lp, NULL) == 0) {
//		    pthread_mutex_lock(&lp);
		    send(cfd, http_head_failed,strlen(http_head_failed),0);
		    send(cfd, file_buff, strlen(file_buff), 0);
//		    pthread_mutex_unlock(&lp);
		} else {
		    fprintf(stderr, "Dangerous visit...\n");
		    send(cfd, http_head_failed,strlen(http_head_failed),0);
		    send(cfd, file_buff, strlen(file_buff), 0);
		}
		pthread_mutex_destroy(&lp);
}

int is_http_protocol(char *const client_head)
{
	int flag = 0;
	char *post;

	post = strchr(client_head, '\n');
	if (strncmp(post - 9, "HTTP", 4))
		flag = 1;

	return flag;
}

// get the browser request and parese the type of web page
int get_client_request(char* const client_head, char* const cmd, int fd)
{
	int flag = 0;
	char *post, *curr;
	if (is_http_protocol(client_head)) {
		response_error_to_client(fd, 6);
	} else {
		// If it is not http protocol, we don't ignore it.
	}

	// usr is too large
	if (strlen(client_head) >= RECV_BUFF_SIZE)
		response_error_to_client(fd, 4);
	strcat(cmd, "..");
	curr = strchr(client_head, '/');
	post = strchr(client_head, '\n');
	strncat(cmd, curr, post - curr - 10);
	cmd[3 + post - curr - 10] = '\0';
	if (strcmp(cmd, "../") == 0) {
		strcpy(cmd, "../www/makefriends.html");
		cmd[strlen(cmd) + 1] = '\0';
	}
	// ensure the request is image or css, then mark the connect-type
	curr = cmd;
	curr += 2;	// jump the first two point
	post = strchr(curr, '.');
	if (!post)
		response_error_to_client(fd, 3);
	// if the request content images, the connect_type should replaced
	// by image/image_type
	post++;
	char ctype[30];
	int flags = 0;
	ctype[0] = '\0';
	if ((strncmp(post, "html", 4) == 0)) {
		flags = 0;
		post = strrchr(http_head_succeed, ':');
		post += 2;
		strcpy(post, "text/html\n\n");
	} else if ((strncmp(post, "gif", 3) == 0) || (strncmp(post,
		"png", 3) == 0)
			|| (strncmp(post, "jpg", 3)  ==  0)) {
		flags = 1;
		strncpy(ctype, "image/", 6);
	} else if (strncmp(post, "css", 3) == 0) {
		flags = 1;
		strncpy(ctype, "text/", 5);
	} else if (strncmp(post, "js", 2) == 0) {
		flags = 0;
		post = strrchr(http_head_succeed, ':');
		post += 2;
		strncpy(post, "text/javascript", 15);
	} else if (strncmp(post, "ico", 3) == 0) {
		flags = 0;
		post = strrchr(http_head_succeed, ':');
		post += 2;
		strncpy(post, "text/x-icon", 11);
	}
	if (flags) {
		strncat(ctype, post, 3);
		post = strrchr(http_head_succeed, ':');
		post += 2;	// jump the characters ": "
		strcat(ctype, "\n\n");
		strncpy(post, ctype, strlen(ctype));
	}

	return 0;
}

// deal user request and response
void deal_request_from_user(char *cmd)
{
	char *curr, *post, *p;
	char *method = NULL;
	char buff[1024] = "";
	int i = 1, flags = 0;

	// make the pointor to the second '/'
	curr = strrchr(cmd, '/');
	curr++;
	post = strchr(curr, '.');
	p = strchr(cmd, '?');
	if (p) {
		buff[0] = '(';
		p++;
		while (*p) {
			if (*p == '=') {
				buff[i++] = *p++;
				buff[i++] = '\'';
			} else if (*p == '&') {
				buff[i++] = '\'';
				buff[i++] = ',';
				p++;
			}
			if (*p == '&') {
				// mail address is none
				flags = 1;
				continue;
			}
			buff[i++] = *p++;
		}
		if (buff[i-2] == '\'')
			i--;
		buff[i] = '\''; buff[++i] = ')';
		if (strncmp(curr, "register", post - curr) == 0)
			method = "register";
		else if (strncmp(curr, "login", post - curr) == 0)
			method = "login";
		else if (strncmp(curr, "person", post - curr) == 0)
			method = "person";
		// database operation
		memset(cmd, '\0', sizeof(cmd));
		mysql_operation(buff, method);
		strcpy(cmd, buff);
	}
}

// base on the broswer's request and response
void response_to_client(int fd, char *filename)
{
	char replay[FILE_MAX_SIZE];
	int ffd;

	ffd = open(filename, O_RDONLY);
	if (ffd != -1) {
		struct stat st;
		if (fstat(ffd, &st) != -1) {
			read(ffd, replay, st.st_size);
			send(fd, http_head_succeed,
				strlen(http_head_succeed), 0);
			send(fd, replay, st.st_size, 0);
		} else
			response_error_to_client(fd, 1);
		close(ffd);
	} else {
		response_error_to_client(fd, 1);
	}
}

int accept_request_from_client()
{
        int client_fd, server_fd;
        struct sockaddr_in client_addr;
        struct sockaddr_in server_addr;
        int result = 0;
	int addr_len = sizeof(server_addr);
	static int procnum;
	static int bindcount;
	char curip[80];

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
		fprintf(stderr, "Fatel error, create server fd failed!\n");
		exit(1);
	}
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(PORT);
//	server_addr.sin_addr.s_addr = inet_addr(IP);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        result = bind(server_fd, (struct sockaddr *)&server_addr,
			sizeof(server_addr));
        if (result != 0) {
		close(server_fd);
		fprintf(stderr, "Bind net socket error!\n");
		exit(2);
	}
        listen(server_fd, 5);
	pthread_t p_thread;
	get_current_ip(curip);
	fprintf(stdout, "Server start succeed!\n");
	fprintf(stdout, "Current server port: %d\n", (PORT));
	fprintf(stdout, "Current server ip: %s\n", (IP));
	fprintf(stdout, "Current connect ip: %s\n", curip);
        while (1) {
                client_fd = accept(server_fd, (struct sockaddr *)
				&server_addr, &addr_len);
		if (procnum >= 100)
			sleep(1);
		if (client_fd != -1) {
			procnum++;
//			printf("Current connected num is: %d\n", procnum);
			// creaste a pthread to deal the request
			result = pthread_create(&p_thread, NULL,
					deal_request_from_client,
					(void *)(&client_fd));
			if (result != 0) {
				fprintf(stderr, "Create pthread failed");
				continue;
			}
			// like the wait function in fork()
			result = pthread_join(p_thread, NULL);
			if (result != 0)
				fprintf(stderr, "Pthread recycle failed!");
			else
				procnum -= 1;
		} else
                        fprintf(stderr,"Current request failed,wait...\n");
        }
	close(server_fd);

	return EXIT_SUCCESS;
}

void *deal_request_from_client(void *cfd)
{
	char buff[RECV_BUFF_SIZE] = "";
	char client_cmd[URL_SIZE] = "";
	char result[128] = "";
	int *fd;
	fd = (int *)cfd;
	memset(buff, '\0', sizeof(buff));
	if (read(*fd, buff, sizeof(buff))) {
		buff[RECV_BUFF_SIZE - 1] = '\0';
<<<<<<< HEAD
		get_client_request(buff, client_cmd, *fd);
=======
//time_t curtime = time(NULL);
//printf("Current time: %s\n", ctime(&curtime));
		get_client_request(buff, client_cmd, *fd);
fprintf(stdout, "Browser request:%s\n", buff);
>>>>>>> origin/master
		deal_request_from_user(client_cmd);
		response_to_client(*fd, client_cmd);
	}
	close(*fd);
//	pthread_exit(NULL);

	return NULL;
}

int main()
{

	accept_request_from_client();

        return 0;
}
