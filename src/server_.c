#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

char http_head_succeed[64] =
	"HTTP/1.1 200 OK\n"
	"Connection: keep-alive\n"
	"Content-Type: text/html\n\n";

char http_head_failed[64] =
	"HTTP/1.1 200 OK\n"
	"Connection: keep-alive\n"
	"Content-Type: text/html\n\n";

int get_client_request(char client_head[512], char client_cmd[64]);
void response_err(char *replay, int fd, int errnum);
void response_to_client(int *fd, char *filename);
void child_proc();
void server_func();

int get_client_request(char* const client_head, char* const client_cmd)
{
	int flag = 0;
	char *post, *curr;
	strcat(client_cmd, "..");
	post = strchr(client_head, '\n');
	if (strncmp(post - 9, "HTTP", 4)) {
		flag = 1;
		client_cmd[0] = '\0';
		response_err("Not HTTP protocol!", 2, 2);
	}
	curr = strchr(client_head, '/');
	strncat(client_cmd, curr, post - curr - 10);
	if (strcmp(client_cmd, "../") == 0) {
		strcpy(client_cmd, "../www/main.html");
		client_cmd[strlen(client_cmd) + 1] = '\0';
		return flag;
	}
	client_cmd[3 + post - curr - 10] = '\0';

	return 0;
}

// return errors to the broswer
void response_err(char *replay, int fd, int errnum)
{
	if (errnum != 2) {
		send(fd, http_head_succeed, strlen(http_head_succeed), 0);
		strcpy(replay, "<html><h1>AN ERROR OCCURRED!</h1></html>");
		send(fd, replay, strlen(replay), 0);
		close(fd);
	} else if (errnum == 2 && fd == 2)
		fprintf(stderr, "%s", replay);
	exit(-1);
}

// base on the broswer's request and response
void response_to_client(int *fd, char *filename)
{
	char replay[1024] = "";
	FILE *fp;

	fp = fopen(filename, "r");
	if (fp) {
		send(*fd, http_head_succeed, strlen(http_head_succeed), 0);
		while (fread(replay, 1023, 1, fp)) {
			replay[1023] = '\0';
			send(*fd, replay, strlen(replay), 0);
		}
		fclose(fp);
	} else
		response_err("Command not found....", 2, 3);
}

void child_proc()
{
        int result = 0;
        int client_fd, server_fd;
        char buff[2048] = "";
	char client_cmd[64] = "";
        struct sockaddr_in client_addr;
        struct sockaddr_in server_addr;

        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0)
		response_err("Create server_fd failed......", 2, 2);
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(32700);
        server_addr.sin_addr.s_addr = inet_addr("192.168.1.6");

        result = bind(server_fd, (struct sockaddr *)&server_addr,
			sizeof(server_addr));
        if (result != 0)
		response_err("Bind failed--\n ",2, 2);
        listen(server_fd, 5);
	int addr_len = sizeof(server_addr);
	static int procnum;
        while (1) {
		pid_t pid;
                client_fd = accept(server_fd, (struct sockaddr *)&server_addr,
					&addr_len);
		if (procnum >= 100)
			sleep(1);
                if (client_fd != -1) {
			signal(SIGCHLD, NULL);
			pid = fork();
			procnum++;
			if (pid == 0) {
				printf("\nconnect succeed......\n");
				read(client_fd, buff, 1023); 
				buff[1023] = '\0';
				get_client_request(buff, client_cmd);
				response_to_client(&client_fd, client_cmd);
				close(client_fd);
				exit(0);
			} else if (pid != -1) {
				close(client_fd);
				exit(0);
			} else if (pid == -1)
			    response_err("Can't create process any more!", 2,2);
			procnum -= 2;
		} else
                        response_err("connect failed--", 2, 2);
        }
}

void server_func()
{
        pid_t pid;
        int res = 0;

        pid = fork();
        signal(SIGCHLD, NULL);
        switch (pid) {
        case -1:
                response_err("Create child process failed--", 2, 2);
        case 0:
                child_proc();
        }
}

int main()
{
        server_func();

        return 0;
}
