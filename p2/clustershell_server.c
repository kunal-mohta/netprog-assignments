#include "clustershell_server.h"

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include<errno.h>

#include "constants.h"
#include "parser.h"
#include "read_config.h"
#include "tcp_helpers.h"

int main(int argc, char **argv) {

	// Setup server
	int clnt_sock, serv_sock = serv_side_setup(__SERVER_PORT__);
	struct sockaddr_in clnt_addr;

	// Server in listening mode
	char **config_ips = read_config(__CONFIG_FILE__);

  while(true) {
		int clnt_len = sizeof(clnt_addr);

		if ((clnt_sock = accept(serv_sock, (struct sockaddr *) &clnt_addr, &clnt_len)) < 0) {
			printf("Error listening socket: %d\n", errno);
			exit(0);
		}

		printf("Handling client %s\n", inet_ntoa(clnt_addr.sin_addr));

		pid_t ch_handler = fork();

		if(ch_handler < 0) {
			printf("Error handling the forking. Exiting...\n");	
			exit(0);
		}

		else if(ch_handler == 0) {
			close(serv_sock);

			while (1) {
			char cmd_buf[__MAX_CMD_SIZE__];
			int n = read(clnt_sock, cmd_buf, __MAX_CMD_SIZE__);
			if(n < 0) exit(-1);
			cmd_buf[n] = '\0';
			printf("Received command: %s\n", cmd_buf);

			PARSE_OBJ pcmd = parse(cmd_buf);
			int cmd_it = 0;
			char input_buf[__MAX_OUT_SIZE__ + 1] = {0};
			int input_buf_size = 0;
			while(!PARSE_EMPTY(pcmd, cmd_it)) {
				char *cl = PARSE_GET_KEY(pcmd, cmd_it);
				char *in_cmd = PARSE_GET_VAL(pcmd, cmd_it);
				if(cl[1] == '*') {
				
				}
				else {
					int con_fd;				
					if(cl[0] != '-') {
						int cl_no = atoi(cl + 1);	
						con_fd = 	clnt_side_setup(config_ips[cl_no-1], __CLIENT_PORT__);
					}
					else {
						con_fd = clnt_side_setup(inet_ntoa(clnt_addr.sin_addr), __CLIENT_PORT__);	
					}
					char response[__MAX_CMD_SIZE__ + 1 + __MAX_OUT_SIZE__ + 1] = {0};
					strcpy(response, in_cmd);
					strcpy(response+strlen(in_cmd)+1, input_buf);
					int response_size = strlen(in_cmd)+1+strlen(input_buf)+1;

					printf("Sending cmd: %s\n", response);
					int nbytes = write(con_fd, response, response_size);
					if(nbytes != response_size) {
						printf("Error in writing. Exiting...\n");	
					}
					input_buf_size = read(con_fd, input_buf, __MAX_OUT_SIZE__+1);
					close(con_fd);
				}
				++cmd_it;
			}

			printf("Received output: %s\n", input_buf);
			write(clnt_sock, input_buf, input_buf_size);
			printf("reached here\n");
			}
			close(clnt_sock);
		}

		close(clnt_sock);

		/*while(true) {
			//sleep(2);
			//write(clnt_sock, "Hello world\n", 12);
		}*/
  }

	CONFIG_FREE(config_ips);
}