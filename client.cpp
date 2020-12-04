#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>



int main( int argc, char ** argv )
{
	std::string const TITLE = "CLIENT : ";
	std::string const ERROR_TITLE = "CLIENT ERROR : ";

	std::string SERVER_IP = "127.0.0.1";
	int const DEFAULT_PORT = 1601;
	int const BUFFER_SIZE = 128;

	int sockfd = socket( AF_INET, SOCK_STREAM, 0 );

	if ( sockfd < 0 ) {
		std::cout << ERROR_TITLE << "establishing socket errror.\n";

		return 1;
	}

	std::cout << TITLE << "client socket has been successfully created.\n";

	struct sockaddr_in server_address;

	server_address.sin_port = htons( DEFAULT_PORT );
	server_address.sin_family = AF_INET;
	inet_pton( AF_INET, SERVER_IP.c_str(), &server_address.sin_addr );

	if ( connect(
		sockfd,
		( struct sockaddr * ) ( &server_address ),
		sizeof( server_address ) ) < 0 ) {
		std::cout << ERROR_TITLE << "connecting to server.\n";

		return 1;
	}

	std::cout << 
		TITLE <<
		"Connected to server " <<
		inet_ntoa( server_address.sin_addr ) <<
		" with port number " <<
		DEFAULT_PORT << 
		'\n';

	//std::cout << "Waiting for server confirmation...\n";

	char buffer[ BUFFER_SIZE ];
	while ( true ) {
		std::cout << TITLE;
		std::cin.getline( buffer, BUFFER_SIZE );
		send( sockfd, buffer, BUFFER_SIZE, 0 );

		if ( std::string( buffer ) == "exit" ) {
			std::cout << TITLE << "Client session terminated\n";

			break;
		}

		recv( sockfd, buffer, BUFFER_SIZE, 0 );
		std::cout << "Server : ";
		std::cout << buffer << '\n';
	}

	return 0;
}