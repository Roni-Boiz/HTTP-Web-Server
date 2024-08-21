/*
 * Server.c
 *
 *  Created on: Aug 15, 2023
 *      Author: Ronila
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "Server.h"
#include "Parameters.h"
#include "Routes.h"
#include "Response.h"

const char _status_code_text[8][50] = {
	"HTTP/1.1 200 OK\r\n",
	"HTTP/1.1 201 CREATED\r\n",
	"HTTP/1.1 400 Bad request\r\n",
	"HTTP/1.1 401 Unauthorized\r\n",
	"HTTP/1.1 403 Forbidden\r\n",
	"HTTP/1.1 404 Not found\r\n",
	"HTTP/1.1 500 Internal Error\r\n",
	"HTTP/1.1 501 Not implemented\r\n"
};


// SOCKET -> BIND -> LISTEN -> ACCEPT -> READ/WRITE
struct Server init_server(int domain, int service, int protocol, u_long interface, int port, int backlog,
		void (*launch)(struct Server *server, bool show_routes))
{
	struct Server server;

	// Server info/ Config server
	server.domain = domain;
	server.service = service;
	server.protocol = protocol;
	server.interface = interface;
	server.port = port;
	server.backlog = backlog;

	// Create server socket (a file descriptor that refers a end-point for communication) -> IPV4, TCP, 0
	server.socket = socket(domain, service, protocol);

	if(server.socket == 0)
	{
		perror("Failed to connect to socket...\n");
		exit(1);
	}
	printf("Socket created successfully...\n");

	// Client info/ Config socket address -> AF_INET, 80/8080, INADDR_ANY (address -> 127.0.0.1)
	server.address.sin_family = domain;
	server.address.sin_port = htons(port);
	server.address.sin_addr.s_addr = htonl(interface);

	// Bind socket to port accept client connection (bind name to socket) -> socket_fd, address struct, address length
	if((bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address))) < 0)
	{
		perror("Failed to bind socket...\n");
		exit(1);
	}
	printf("Socket bind to address successfully...\n");

	// Listen for connections on a socket as a passive(TCP) socket or active/passive(UDP) -> socket_fd, pending connections/queue length SOMAXCONN()128 10
	if(listen(server.socket, server.backlog) < 0)
	{
		perror("Failed to start listening...\n");
		exit(1);
	}
	printf("Server listening for connections...\nPort: %d\n", server.port);

	server.launch = launch;

	return server;
}

void *handle_client_request(void *_args)
{
	struct Request_Parameters* client_req_parameters = ((struct Request_Parameters *) _args);

//	char buffer[BUFFER_SIZE];
	char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

	// Read from file descriptor -> client_fd, pointer to contents want to read from client_fd as request, no.of bytes need to ready from the file descriptor into buffer
	ssize_t bytes_received = read(*client_req_parameters->client_fd, buffer, BUFFER_SIZE);
//	ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

	if (bytes_received < 0)
	{
		perror("Failed to read from buffer...\n");
	}
//	printf("%s\n", buffer);

	// parsing client socket header to get HTTP method, route
	char *method = "";
	char *urlRoute = "";

	char *client_http_header = strtok(buffer, "\n");
	printf("%s\n", client_http_header);

	char *header_token = strtok(client_http_header, " ");
	int header_parse_counter = 0;

	while (header_token != NULL)
	{
		switch (header_parse_counter)
		{
			case 0:
				method = header_token;
				//No break
			case 1:
				urlRoute = header_token;
				//No break
		}
		header_token = strtok(NULL, " ");
		header_parse_counter++;
	}

	printf("The method is %s\n", method);
	printf("The route is %s\n", urlRoute);

	if (strlen(urlRoute) > 0)
	{
		paramParse(client_req_parameters->params, urlRoute);

		printf("==== REQUEST PARAMETERS ====\n");
		for (size_t i = 0; i < client_req_parameters->params->n_members; ++i)
		{
			printf("KEY: %s\n", client_req_parameters->params->parameters[i].key);
			printf("VALUE: %s\n\n", client_req_parameters->params->parameters[i].value);
		}
		// do something with parameters and can clear them to re-use for the next request
		paramClear(client_req_parameters->params);
	}

//	char *response = "HTTP/1.1 200 OK\r\n"
//			"Date: Mon, 14 Aug 2023 00:00:00 GMT\r\n"
//			"Server: Apache/2.2.14 (win32)\r\n"
//			"Last-Modified: Tue, 15 Aug 2023 00:00:00 GMT\r\n"
//			"Content-Type: text/html\r\n\r\n"
//			"<html><body><h1>Hello, World!. Welcome to localhost...</h1></body></html>\r\n";

	char file_path[1024] = "";

	if (strstr(urlRoute, "/Styles/") != NULL)
	{
		strcat(file_path, "../Assets/Styles/index.css");
//		strcat(file_path, strcat("../Assets", urlRoute));
		set_http_status_code(client_req_parameters->server, OK);
	}
	else if (strstr(urlRoute, "/Scripts/") != NULL)
	{
		strcat(file_path, "../Assets/Scripts/index.js");
		set_http_status_code(client_req_parameters->server, OK);
	}
	else if (strstr(urlRoute, "/Images/") != NULL)
	{
		strcat(file_path, "../Assets/Images/man.png");
		set_http_status_code(client_req_parameters->server, OK);
	}
	else if (strstr(urlRoute, "/Files/") != NULL)
	{
		strcat(file_path, "../Files/resume_001.docx");
		set_http_status_code(client_req_parameters->server, OK);
	}
	else if (strstr(urlRoute, "/File/txt") != NULL)
	{
		strcat(file_path, "../Files/sample-text.txt");
		set_http_status_code(client_req_parameters->server, OK);
	}
	else if (strstr(urlRoute, "/File/pdf") != NULL)
	{
		strcat(file_path, "../Files/sample-pdf.pdf");
		set_http_status_code(client_req_parameters->server, OK);
	}
	else if (strstr(urlRoute, "/File/audio") != NULL)
	{
		strcat(file_path, "../Files/sample-audio.mp3");
		set_http_status_code(client_req_parameters->server, OK);
	}
	else if (strstr(urlRoute, "/File/video") != NULL)
	{
		strcat(file_path, "../Files/sample-video.mp4");
		set_http_status_code(client_req_parameters->server, OK);
	}
	else
	{
		struct Route *destination = search(client_req_parameters->route, urlRoute);
		strcat(file_path, "../Pages/");

		if (destination == NULL)
		{
			strcat(file_path, "404.html");
			set_http_status_code(client_req_parameters->server, NOT_FOUND);
		}
		else
		{
			strcat(file_path, destination->value);
			set_http_status_code(client_req_parameters->server, OK);
		}
	}
	printf("%s\n", file_path);

	// get file extension
	char file_ext[32];
	strcpy(file_ext, get_file_extension(file_path));

	// build HTTP response
	build_http_response(file_path, file_ext, client_req_parameters->server);

//	http_set_response_body(client_req_parameters->server, response_data);

//

//	// Write to file descriptor from buffer -> client_fd, pointer to contents want to write to client_fd as response, no.of bytes need to write to file descriptor from buffer
	ssize_t bytes_send = write(*client_req_parameters->client_fd, client_req_parameters->server->response, sizeof(client_req_parameters->server->response));
//	ssize_t bytes_send = send(*client_req_parameters->client_fd, client_req_parameters->server->response, sizeof(client_req_parameters->server->response), 0);

	if (bytes_send < 0)
	{
		perror("Failed to write to buffer...\n");
	}

	memset(client_req_parameters->server->status_code, '\0', sizeof(client_req_parameters->server->status_code));
	memset(client_req_parameters->server->response, '\0', sizeof(client_req_parameters->server->response));

	// Free memory used by parameters if pthread_create function used to call this function
//	free(client_req_parameters);

	return NULL;
}

const char *get_file_extension(const char *file_name) {
    const char *dot = strrchr(file_name, '.');
    if (!dot || dot == file_name) {
        return "";
    }
    return dot + 1;
}

const char *get_mime_type(const char *file_ext) {
    if (strcasecmp(file_ext, "html") == 0 || strcasecmp(file_ext, "htm") == 0)
    {
        return "text/html";
    }
    else if (strcasecmp(file_ext, "php") == 0)
	{
		return "text/html";
	}
    else if (strcasecmp(file_ext, "txt") == 0)
    {
        return "text/plain";
    }
    else if (strcasecmp(file_ext, "jpg") == 0 || strcasecmp(file_ext, "jpeg") == 0)
    {
        return "image/jpeg";
    }
    else if (strcasecmp(file_ext, "ico") == 0)
	{
		return "image/ico";
	}
    else if (strcasecmp(file_ext, "png") == 0)
    {
        return "image/png";
    }
    else if (strcasecmp(file_ext, "gif") == 0)
	{
		return "image/gif";
	}
    else if (strcasecmp(file_ext, "css") == 0)
	{
		return "text/css";
	}
    else if (strcasecmp(file_ext, "js") == 0)
	{
		return "application/javascript";
	}
    else if (strcasecmp(file_ext, "json") == 0)
	{
		return "application/json";
	}
    else if (strcasecmp(file_ext, "doc") == 0)
	{
		return "application/msword";
	}
    else if (strcasecmp(file_ext, "docx") == 0)
	{
		return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	}
    else if (strcasecmp(file_ext, "xls") == 0)
	{
		return "application/vnd.ms-excel";
	}
    else if (strcasecmp(file_ext, "ppt") == 0)
	{
		return "application/vnd.ms-powerpoint";
	}
    else if (strcasecmp(file_ext, "pptx") == 0)
	{
		return "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	}
    else if (strcasecmp(file_ext, "pdf") == 0)
	{
		return "application/pdf";
	}
    else if (strcasecmp(file_ext, "mp3") == 0)
	{
		return "audio/mpeg";
	}
    else if (strcasecmp(file_ext, "mp4") == 0)
	{
		return "video/mp4";
	}
    else
    {
        return "application/octet-stream";
    }
}

void set_http_status_code(struct Server *http_server, const enum http_status_code_e status_code)
{
	memset(http_server->status_code, 0, HTTP_STATUS_CODE_LEN);
	const char* _status = _status_code_text[status_code];
	memcpy(http_server->status_code, _status, strlen(_status));
}

void set_http_response_body(struct Server *http_server, const char* body)
{
	// prepend the status code before adding the body
	memset(http_server->response, 0, HTTP_RESPONSE_BODY_LEN);
	memcpy(http_server->response, http_server->status_code, strlen(http_server->status_code));

	// add response body (while accounting for the space we already took up by the status code
	// NOTE: the -5 is for the four extra \r\n\r\n characters and a null terminator
	size_t max_len = (size_t)HTTP_RESPONSE_BODY_LEN - strlen(http_server->status_code) - 5;
	strncat(http_server->response, body, max_len);
	strncat(http_server->response, "\r\n\r\n", 5);
}

void build_http_response(char* file_path, char *file_ext, struct Server *http_server)
{
    // build HTTP header
    const char *mime_type = get_mime_type(file_ext);
    char *header = (char *)malloc(BUFFER_SIZE * sizeof(char));
    snprintf(header, BUFFER_SIZE,
             "Content-Type: %s\r\n\r\n",
             mime_type);

    strncat(http_server->status_code, header, strlen(header));
    printf("%s", http_server->status_code);


    char *response_data = render_static_file(file_path);

    set_http_response_body(http_server, response_data);

    free(header);
}



