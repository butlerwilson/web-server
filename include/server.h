#ifndef _SERVER_H_
#define _SERVER_H_

#define RECV_BUFF_SIZE 3072
#define SEND_BUFF_SIZE 1024000
#define URL_SIZE 2048

#define TIME_OUT 10

// the generally status of http head
#define RESQ_OK 200
#define RESQ_FORB 403
#define RESQ_NOT_FOUND 404
#define UNKNOW_METHOD 405
#define FILE_TOO_LARGE 413
#define URL_TOO_LONG 414
#define UNSUPPORT_MIME_TYPE 415
#define UNSUPPORT_HTTP_VERSION 505

#define FILE_MAX_SIZE 1048576	//the max file size

#define PORT 32766	// port
#define IP "192.168.1.111" // server ip

//  succeed send http head
char http_head_succeed[] =
	"HTTP/1.1 200 OK\n"
	 "Connection: keep-alive\n"
	 "Content-Type: text/html\n\n";

// failed to send http _head
char http_head_failed[] =
	"HTTP/1.1 404 Not Found\n"
	"Connection: keep-alive\n"
	"Content-Type: text/html\n\n";

// finction declaration

// judge the protocol is http
extern int is_http_protocol(char *const client_head);

// get the command from browser request
extern int get_client_request(char *const client_head, char *const cmd, int fd);

// reponse errors to client
extern void response_error_to_client(int fd, int errno);

// reponse to client
extern void reponse_to_client(int *fd, char *filename);

// the main loop for server
extern int accept_request_from_client();

// deal the request from user
extern void *deal_request_from_client(void *cfd);

#endif
