#pragma once

/* 프로토콜 정의 */

enum protocol {
	HELLO = 0,
	MESSAGE,
	COMMAND
};

// Client -> Server
typedef struct PDUHello {
	char protocol_type; // HELLO
	char internal_ip[20];
} PDUHello;

// Server -> Client
typedef struct PDUMessage {
	UCHAR protocol_type; // MESSAGE
	WCHAR message[600];
} PDUMessage;

