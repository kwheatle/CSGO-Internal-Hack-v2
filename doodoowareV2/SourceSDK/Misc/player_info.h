#pragma once
typedef unsigned char byte;
typedef struct player_info_s
{
	char pad[0x10];
	char name[128];
	int	 userID;
	char guid[33];
	unsigned long friendsID;
	char friendsName[128];
	bool fakeplayer;
	bool ishltv;
	unsigned long customFiles[4];
	unsigned char filesDownloaded;
	byte buffer[200];
} player_info_t;