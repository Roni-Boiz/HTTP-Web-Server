/*
 * TestConnection.c
 *
 *  Created on: Aug 15, 2023
 *      Author: Ronila
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <getopt.h>

#include "Server.h"
#include "Routes.h"
#include "Parameters.h"
#include "Response.h"


struct HTTP_Argparser
{
     int port;
     int backlog;
     _Bool  show_routes;
};

void argument_parser ( int argc  , char * const  *argv  , struct HTTP_Argparser  *http_argparse )
{
    if(argc == 1)
        return;

    int  option_parser = 0;
    struct  option  http_arguments_options [] = {
        {"version"        , no_argument        ,  0 , 'v'},
        {"port"           , required_argument  ,  0 , 'p'},
        {"backlog" , required_argument  ,  0 , 'b'},
        {"show-routes"    , no_argument        ,  0 , 'r'},
        {0, 0, 0, 0}
    };

    while ((option_parser  = getopt_long(argc, argv , "vp:b:r", http_arguments_options, 0)) != -1 )
    {
        switch (option_parser)
        {
            case 'v' :
                fprintf(stdout  , "HTTPWebServer version 1.0 \n") ;
                break ;
            case 'p' :
                 http_argparse->port = strtol(optarg , NULL , 10) ;
                break ;
            case 'b':
                 http_argparse->backlog = strtol(optarg , NULL , 10) ;
                break;
            case 'r':
               http_argparse->show_routes = true;
               break ;
            default :
               fprintf(stderr, "Unrecognized flags argument %s \n" ,argv[optind]);
               exit(1) ;
        }
    }

}

void launch(struct Server *server, _Bool show_routes)
{
	// Registering available Routes
	struct Route * route = initRoute("/", "index.html");
	addRoute(&route, "/home", "index.html");
	addRoute(&route, "/about", "about.html");
	addRoute(&route, "/service", "service.html");
	addRoute(&route, "/blog", "blog.html");
	addRoute(&route, "/contact", "contact.html");
	addRoute(&route, "/File/txt", "sample-text.txt");
	addRoute(&route, "/File/pdf", "sample-pdf.pdf");
	addRoute(&route, "/File/audio", "sample-audio.mp3");
	addRoute(&route, "/File/video", "sample-video.mp4");

	if(show_routes)
	{
		printf("=========ALL AVAILABLE ROUTES========\n");
		// display all available routes
		inorder(route);
	}

	int address_length = sizeof(server->address);

	int *client_fd = malloc(sizeof(int));

	// create dynamic parameters array
	struct ParameterArray* parameters = paramInit(10);

	struct Request_Parameters* req_parameters = malloc (sizeof (struct Request_Parameters));


	while(1)
	{
		printf("==== WAITING FOR CONNECTION ====\n");

		// Accept client connection (get first connection from the queue of listening socket and create a new socket and file descriptor for other connections) original socket_fd, socket address, address length
		*client_fd = accept(server->socket, (struct sockaddr *)&server->address, (socklen_t*)&address_length);
		if(*client_fd < 0)
		{
			perror("Failed to accept socket...\n");
			continue;
		}

		req_parameters->client_fd = client_fd;
		req_parameters->params = parameters;
		req_parameters->route = route;
		req_parameters->server = server;

		// Create a new thread to handle client request
//		pthread_t thread_id;
//		pthread_create(&thread_id, NULL, handle_client_request, (void *)req_parameters);
//		pthread_detach(thread_id);
		handle_client_request((void *)req_parameters);

		close(*client_fd);
	}

	close(server->socket);

}

int main(int argc, char* argv[])
{
	//! Argument  handler
	struct HTTP_Argparser  http_argp   = {
		.port        = DFAULT_PORT,
		.backlog = LISTEN_BACKLOG,
		false
	};

	argument_parser(argc, argv,  &http_argp) ;


	// AF_INET: IPV4
	// SOCK_STREAM: TCP
	// INADDR_ANY: accept connections from any interface
	// DFAULT_PORT: connecting to port 8080 (localhost)
	// LISTEN_BACKLOG: Maximum number of pending connections
	struct Server server = init_server(AF_INET, SOCK_STREAM, 0, INADDR_ANY, http_argp.port, http_argp.backlog, launch);

	server.launch(&server, http_argp.show_routes);
}
