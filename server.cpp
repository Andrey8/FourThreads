#include "backend.h"

#include <iostream>
#include <cstring>
#include <string>
#include <sstream>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <thread>



int main( int argc, char ** argv )
{
	int val1 = 0;
	int val2 = 0;
	bool isExit = false;

	Backend::Backend backend;

	std::thread thr3( [ & ]() { backend.LogInfoToFile( val1, val2, isExit ); } );
	


	std::string const ERROR_TITLE = "SERVER ERROR : ";
	std::string const TITLE = "SERVER : ";

	int const DEFAULT_PORT = 1601;
	int const BUFFER_SIZE = 128;

	int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

	if ( sockfd < 0 ) {
		std::cout << ERROR_TITLE << "establishing socket errror.\n";

		return 1;
	}

	std::cout << TITLE << "server socket has been successfully created.\n";
	std::cout << TITLE << "port : " << DEFAULT_PORT << '\n';

	struct sockaddr_in server_address;

	server_address.sin_port = htons( DEFAULT_PORT );
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons( INADDR_ANY );

	if ( bind( 
		sockfd, 
		( struct sockaddr * ) ( &server_address ),
		sizeof( server_address ) ) < 0 ) {
		std::cout << ERROR_TITLE << "binding.\n";

		return 1;
	}
	
	if ( listen( sockfd, 1 ) < 0 ) {
		std::cout << ERROR_TITLE << "listening.\n";
	}

	std::cout << TITLE << "Listening to clients...\n";

	socklen_t size = sizeof( server_address );
	int connfd = accept( 
		sockfd, 
		( struct sockaddr * ) ( &server_address ),
		&size );

	if ( connfd < 0 ) {
		std::cout << ERROR_TITLE << "Can't accepting client.\n";
	}

	std::cout << TITLE << "Client is connected\n";

	char buffer[ BUFFER_SIZE ];
	while ( true ) {
		recv( connfd, buffer, BUFFER_SIZE, 0 );

		std::string bufferString( buffer );
		if ( bufferString == "info" ) {
			std::cout << TITLE << "info request\n";

			std::stringstream ss;
			ss << 
				"VAL1 = " <<
				val1 <<
				", " <<
				"VAL2 = " <<
				val2 <<
				'\n';

			strcpy( buffer, ss.str().c_str() );

			send( connfd, buffer, BUFFER_SIZE, 0 );
		}
		else if ( bufferString == "exit" ) {
			isExit = true;

			break;
		}
	}

	thr3.join();

	close( sockfd );

	return 0;
}