/**
Name:   ErrorCode.h
Author:  jjuiddong
Date:    3/15/2013

	Error Code
*/
#pragma once


namespace network { namespace error {

#define X_ERROR_CODE\
	X(ERR_SUCCESS, "ERR_SUCCESS")\
	\
	X(ERR_INTERNAL, "ERR_INTERNAL")\
	X(ERR_NOT_FOUND_GROUP, "ERR_NOT_FOUND_GROUP")\
	X(ERR_NOT_FOUND_USER, "ERR_NOT_FOUND_USER")\
	X(ERR_ALREADY_EXIST_USER, "ERR_ALREADY_EXIST_USER")\
	X(ERR_INVALID_USER, "ERR_INVALID_USER")\
	X(ERR_INVALID_CERTIFY_KEY, "ERR_INVALID_CERTIFY_KEY")\
	X(ERR_WAIT_ACK_PLAYER, "ERR_WAIT_ACK_PLAYER")\
	\
	X(ERR_MOVETOSERVER_INVALID_SERVERNAME, "ERR_MOVETOSERVER_INVALID_SERVERNAME")\
	X(ERR_MOVETOSERVER_NOT_FOUND_SERVER, "ERR_MOVETOSERVER_NOT_FOUND_SERVER")\
	\
	X(ERR_MOVEUSER_ALREADY_EXIST, "ERR_MOVEUSER_ALREADY_EXIST")\
	X(ERR_NO_CREATE_GROUP, "ERR_NO_CREATE_GROUP")\
	X(ERR_NOT_JOIN_GROUP, "ERR_NOT_JOIN_GROUP")\
	\
	X(ERR_GROUPLIST_NOT_FOUND_GROUP, "ERR_GROUPLIST_NOT_FOUND_GROUP")\
	\
	X(ERR_GROUPCREATE_NOT_FOUND_PARENT_GROUP, "ERR_GROUPCREATE_NOT_FOUND_PARENT_GROUP")\
	X(ERR_GROUPCREATE_PARENT_TERMINALNODE, "ERR_GROUPCREATE_PARENT_TERMINALNODE")\
	X(ERR_GROUPCREATE_NOR_MORE_CREATE_GROUP, "ERR_GROUPCREATE_NOR_MORE_CREATE_GROUP")\
	\
	X(ERR_GROUPJOIN_ALREADY_SAME_GROUP, "ERR_GROUPJOIN_ALREADY_SAME_GROUP")\
	X(ERR_GROUPJOIN_NOT_TERMINAL, "ERR_GROUPJOIN_NOT_TERMINAL")\
	\
	X(ERR_P2PCONNECTION_NO_MEMBER_IN_GROUP, "ERR_P2PCONNECTION_NO_MEMBER_IN_GROUP")\
	X(ERR_P2PCONNECTION_ALREADY_CONNECTED, "ERR_P2PCONNECTION_ALREADY_CONNECTED")\
	X(ERR_P2PCONNECTION_HOSTCLIENT_DISAPPEAR, "ERR_P2PCONNECTION_HOSTCLIENT_DISAPPEAR")\
	\
	X(ERR_REQSERVERINFO_NOTFOUND_SERVER, "ERR_REQSERVERINFO_NOTFOUND_SERVER")\
	\
	X(ERR_REQLOBBYIN_NOTFOUND_SERVER, "ERR_REQLOBBYIN_NOTFOUND_SERVER")

	
	// Use preprocessor to create the Enum
	typedef enum {
#define X(Enum, String)       Enum,
		X_ERROR_CODE
#undef X
	} ERROR_CODE;


	inline std::string ErrorCodeString(ERROR_CODE errorCode)
	{
		switch (errorCode)
		{
#define X(Enum, String)\
		case Enum: return String;
			X_ERROR_CODE
#undef X
		default: return " ";
		}
	}





/*
	enum ERROR_CODE
	{
		ERR_SUCCESS = 0,
		ERR_NO_ERROR = ERR_SUCCESS,

		// common
		ERR_INTERNAL,
		ERR_NOT_FOUND_GROUP,
		ERR_NOT_FOUND_USER,
		ERR_ALREADY_EXIST_USER,
		ERR_INVALID_USER,
		ERR_INVALID_CERTIFY_KEY,

		// basic protocol
		
			// MoveToServer
			ERR_MOVETOSERVER_INVALID_SERVERNAME,
			ERR_MOVETOSERVER_NOT_FOUND_SERVER,

			// MoveUser
			ERR_MOVEUSER_ALREADY_EXIST,


		// group
		ERR_NO_CREATE_GROUP,
		ERR_NOT_JOIN_GROUP,

		// group list
		ERR_GROUPLIST_NOT_FOUND_GROUP,		

		// create group
		ERR_GROUPCREATE_NOT_FOUND_PARENT_GROUP,
		ERR_GROUPCREATE_PARENT_TERMINALNODE,
		ERR_GROUPCREATE_NOR_MORE_CREATE_GROUP,

		// join group
		ERR_GROUPJOIN_ALREADY_SAME_GROUP,
		ERR_GROUPJOIN_NOT_TERMINAL,

		// p2p connect
		ERR_P2PCONNECTION_NO_MEMBER_IN_GROUP,
		ERR_P2PCONNECTION_ALREADY_CONNECTED,
		ERR_P2PCONNECTION_HOSTCLIENT_DISAPPEAR,
		

		// farm  server

		// request server info
		ERR_REQSERVERINFO_NOTFOUND_SERVER,

		
		// login server
		ERR_REQLOBBYIN_NOTFOUND_SERVER,

	};
/**/


}}
