#include <string>
#include <thread>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
int sock = -1;

// signal handler
void sig_handler(int sig) {
  if (sock > 0) {
    if ( close(sock) < 0 ) {
      perror("close");
      exit(EXIT_FAILURE);
    }
  }
  std::cout << "Good Bye" << std::endl;
  exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <PORT> <FILE-DIR> "  << std::endl;
    return 1;
  }

  signal(SIGINT, sig_handler);
  signal(SIGQUIT, sig_handler);
  signal(SIGTERM, sig_handler);

  int port = std::stoi(argv[1]);
  std::string file_path = argv[2];
  
  // reference: https://www.geeksforgeeks.org/udp-server-client-implementation-c/

  // create socket fd
  if ( (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr, client_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  memset(&client_addr, 0, sizeof(client_addr));


  //server information
  server_addr.sin_family = AF_INET; // IPv4
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP address
  server_addr.sin_port = htons(port); // port number

  // bind socket
  if ( bind(sock, (sockaddr *)&server_addr, sizeof(server_addr)) < 0 ) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  socklen_t sock_len;
  int n;
  char buffer[BUFFER_SIZE];
  memset(&buffer, 0, sizeof(buffer));

  while (1) {
    sock_len = sizeof(client_addr);

    if ( (n = recvfrom(sock, (char *) buffer, BUFFER_SIZE, 0, (sockaddr *)&client_addr, &sock_len)) < 0 )
    {
      perror("recvfrom");
      memset(&buffer, 0, sizeof(buffer));
      continue;
    }

    std::cout << "Client: " << buffer << std::endl;

    // std::string hello_msg = "Hello, this is the server";
    // if ( (sendto(sock, hello_msg.c_str(), hello_msg.length(), 0, (sockaddr *)&client_addr, sock_len)) < 0)
    // {
    //   perror("sendto");
    //   continue;
    // }

  }

  return 0;
}
