/*
 * Server.h
 *
 *  Created on: Aug 15, 2023
 *      Author: Ronila
 */

#ifndef SERVER_H_
#define SERVER_H_

#define DFAULT_PORT 8080
#define LISTEN_BACKLOG 10
#define BUFFER_SIZE 1000000
#define HTTP_STATUS_CODE_LEN 100
#define HTTP_RESPONSE_BODY_LEN BUFFER_SIZE

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>

struct Server
{
	// Store server info
	int domain;
	int service;
	int protocol;
	u_long interface;
	int port;
	int backlog;

	// Store client info
	struct sockaddr_in address;

	// Store server socket
	int socket;

	char status_code[HTTP_STATUS_CODE_LEN];
	char response[HTTP_RESPONSE_BODY_LEN];

	void (*launch)(struct Server *server, _Bool show_routes);
};

struct Request_Parameters
{
	int *client_fd;
	struct ParameterArray *params;
	struct Route *route;
	struct Server *server;
};

enum http_status_code_e
{
	OK = 0,
	CREATED,
	BAD_REQUEST,
	UNAUTHORIZED,
	FORBIDDEN,
	NOT_FOUND,
	INTERNAL_ERROR,
	NOT_IMPLEMENTED
};

extern const char _status_code_text[8][50];

// Initialize web server
struct Server init_server(int domain, int service, int protocol, u_long interface, int port, int backlog,
		void (*launch)(struct Server *server, _Bool show_routes));

// Handle client web requests
void *handle_client_request(void *arg);

void set_http_status_code(struct Server *http_server, const enum http_status_code_e);

void set_http_response_body(struct Server *http_server, const char *body);

const char *get_file_extension(const char *file_name);

void build_http_response(char *file_path, char *file_ext, struct Server *http_server);

#endif /* SERVER_H_ */
