#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tcp_helpers.h"
#include "constants.h"
#include "subscriber_run_cmd.h"
#include "subscriber.h"

char *subscribed_topics[__MAX_TOPIC_COUNT__];
char subscribed_topics_len = 0;
int last_msg_id = 0;

int main (int argc, char *argv[]) {
	if (argc != 3) {
		printf("Incorrect number of arguments.\n");
		printf("Usage: ./sub.out <broker_ip> <broker_port>\n");
		exit(0);
	}
	char *broker_ip = argv[1];
	int broker_port = atoi(argv[2]);

	printf("Commands:\n");
	printf("-> %s <topic>\n", __SUB_CMD__);
	printf("-> %s <topic>\n", __SUB_RET_CMD__);
	printf("-> %s <topic>\n", __SUB_RETALL_CMD__);
	printf("\n\n");
	while (true) {
		printf(__PROMPT__);
		size_t cmd_size = __MAX_CMD_SIZE__+1;
		char *cmd_buf = malloc(sizeof(char) * cmd_size);
		ssize_t cmd_size_act = getline(&cmd_buf, &cmd_size, stdin);
		if(cmd_size_act  == -1 || cmd_size_act == 0 || cmd_size_act == 1 
				&& cmd_buf[0] == '\n') continue;
		cmd_buf[cmd_size_act-1] = 0;

		printf("%s\n", cmd_buf);
		
		/*int broker_sock_fd = clnt_side_setup(broker_ip, __DFL_PORT__);*/
		run_cmd (cmd_buf, broker_ip, broker_port);
	}
}
