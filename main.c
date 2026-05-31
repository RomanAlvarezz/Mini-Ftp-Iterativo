#include "arguments.h"
#include "logs.h"
#include "server.h"
#include "utils.h"
#include "signals.h"
#include <stdio.h>
#include <stdlib.h>     // EXIT_*
#include <string.h>
#include <unistd.h>     // for close()
#include <arpa/inet.h>  // for inet_ntoa()
#include <errno.h>

int main(int argc, char **argv) {
  struct arguments args;

  log_init();

  if (parse_arguments(argc, argv, &args) != 0)
    return EXIT_FAILURE;

  log_info("Starting server on %s:%d", args.address, args.port);

  int listen_fd = server_init(args.address, args.port);
  if (listen_fd < 0)
    return EXIT_FAILURE;

  setup_signals();

  while(1) {
    struct sockaddr_in client_addr;
    int new_socket = server_accept(listen_fd, &client_addr);
    if (new_socket < 0)
      continue;

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    log_info("Connection from %s:%d accepted",
         client_ip,
         ntohs(client_addr.sin_port));

    server_loop(new_socket);

    log_info("Connection from %s:%d closed",
         client_ip,
         ntohs(client_addr.sin_port));
  }

  // NEVER GO HERE
  close_fd(listen_fd, "listening socket");

  log_close();

  // https://en.cppreference.com/w/c/program/EXIT_status
  return EXIT_SUCCESS;
}
