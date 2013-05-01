#include "command.h"
#include "serial.h"
#include "constants.h"
#include "mod_string.h"


const cmd_t commands[] = SYSTEM_COMMANDS;

const char cmdPrompt[] = COMMAND_PROMPT;
const char welcomeMsg[] = WELCOME_MSG;
static int menuPadding = 0;

cmd_context commandContext;

static void set_command_context(Serial *serial, char *buffer, size_t bufferSize){
	commandContext.lineBuffer = buffer;
	commandContext.serial = serial;
	commandContext.lineBufferSize = bufferSize;
}

static void clear_command_context(){
	commandContext.lineBuffer = NULL;
	commandContext.serial = NULL;
	commandContext.lineBufferSize = 0;
}

static void show_help(Serial *serial){
	serial->put_s("Available Commands:");
	put_crlf(serial);
	put_crlf(serial);

	const cmd_t * cmd = commands;
	while (cmd->cmd != NULL){
		serial->put_s(cmd->cmd);
		int padding = menuPadding - strlen(cmd->cmd);
		while (padding-- > 0) serial->put_s(" ");
		serial->put_s(": ");
		serial->put_s(cmd->help);
		serial->put_s(" ");
		serial->put_s("Usage: ");
		serial->put_s(cmd->cmd);
		serial->put_s(" ");
		serial->put_s(cmd->paramHelp);
		put_crlf(serial);
		cmd++;
	}
}

static void calculateMenuPadding(){
	const cmd_t * cmd = commands;

	while (cmd->cmd != NULL){
		int len = strlen(cmd->cmd);
		if (len > menuPadding) menuPadding = len;
		cmd++;
	}
	menuPadding++;
}

void interactive_read_command(Serial *serial, char * buffer, size_t bufferSize){
	int bufIndex = 0;
    char c;
	while(bufIndex < bufferSize - 1){
		c = serial->get_c();
		if (c) {
			if ('\r' == c){
				break;
			}
			else if ('\b' == c){
				if (bufIndex > 0){
					bufIndex--;
					serial->put_c(c);
				}
			}
			else {
				serial->put_c(c);
				buffer[bufIndex++] = c;
			}
		}
	}
	serial->put_s("\n\r");
	buffer[bufIndex]='\0';
}

static void send_header(Serial *serial, unsigned int len){
	while (len-- > 0){
		serial->put_c('=');
	}
	put_crlf(serial);
}

static void show_welcome(Serial *serial){
	put_crlf(serial);
	size_t len = strlen(welcomeMsg);
	send_header(serial, len);
	serial->put_s(welcomeMsg);
	put_crlf(serial);
	send_header(serial, len);
	put_crlf(serial);
	show_help(serial);
}

void execute_command(Serial *serial, char * buffer){
	if (strlen(buffer) == 0){
		show_welcome(serial);
		return;
	}

	unsigned char argc, i = 0;
	char *argv[30];

	argv[i] = strtok(buffer, " ");

	do{
		argv[++i] = strtok(NULL, " ");
	}
	while (( i < 30) && (argv[i] != NULL));

	argc = i;
	const cmd_t * cmd = commands;

	while (cmd->cmd != NULL){
		if (!strcmp(argv[0], cmd->cmd)){
			cmd->func(serial, argc, argv);
			put_crlf(serial);
			break;
		}
		cmd++;
	}
	if (NULL == cmd->cmd){
		serial->put_s("Unknown Command- Press Enter for Help.");
		put_crlf(serial);
	}
}

void process_command(Serial *serial, char * buffer, size_t bufferSize) {

	interactive_read_command(serial, buffer, bufferSize);
	//this is not thread safe. if needed, throw a mutex around here
	set_command_context(serial, buffer, bufferSize);
	execute_command(serial, buffer);
	clear_command_context();
	serial->put_s(cmdPrompt);
	serial->put_s(" > ");
}

void put_commandOK(Serial *serial){
	serial->put_s(COMMAND_OK);
}

void put_commandParamError(Serial *serial, char *msg){
	serial->put_s(COMMAND_ERROR);
	serial->put_s("extended=\"");
	serial->put_s(msg);
	serial->put_s("\";");
}

void put_commandError(Serial *serial, int result){
	serial->put_s(COMMAND_ERROR);
	serial->put_s("code=");
	put_int(serial, result);
	serial->put_s(";");
}

void init_command(void){
	calculateMenuPadding();
}

cmd_context * get_command_context(){
	return &commandContext;
}

