#ifndef _COMMON_H_
#define _COMMON_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<errno.h>
#include<string.h>
#include<sqlite3.h>
#include<sys/wait.h>
#include<signal.h>
#include<time.h>
#include<pthread.h>
#include<sys/stat.h>

#define NAMELEN 16
#define DATALEN 128
#define STAFF_DATABASE "staff_manage_system.db"

#define USER_LOGIN  0x00000000
#define USER_MODIFY 0x00000001
#define USER_QUERY 0x00000002

#define ADMIN_LOGIN 0x10000000
#define ADMIN_MODIFY 0x10000001
#define ADMIN_ADDUSER 0x10000002
#define ADMIN_DELUSER 0x10000004
#define ADMIN_QUERY 0x10000008
#define ADMIN_HISTORY 0x10000010

#define QUIT 0x11111111

#define ADMIN 0
#define USER 1

typedef struct staff_info
{
	int no;
	int usertype;
	char name[NAMELEN];
	char passwd[8];
	int age;
	char phone[NAMELEN];
	char addr[DATALEN];
	char work[DATALEN];
	char date[DATALEN];
	int level;
	double salary;
}staff_info_t;

typedef struct
{
	int msgtype;
	int usertype;
	char username[NAMELEN];
	char passwd[8];
	char recvmsg[DATALEN];
	int flags;
	staff_info_t info;
}MSG;

int process_client_request(int acceptfd,MSG *msg);
int process_user_or_admin_login_request(int acceptfd,MSG *msg);
void do_login(int sockfd);

int admin_or_user_login(int sockfd,MSG *msg);

void admin_menu(int sockfd,MSG *msg);

#endif
