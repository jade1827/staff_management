#include"common.h"

sqlite3 *db;

void process_admin_modify_request(int acceptfd,MSG *msg)
{
	char *errmsg;
	char temp[10] = {0};
	printf("------%s-------%d\n",__func__,__LINE__);
	char sql[DATALEN] = {0};
	printf("modify user: %s\n",msg->info.name);
	switch(msg->flags)
	{
		case 1:
			strcpy(temp,"staffno");
			break;
		case 2:
			strcpy(temp,"usertype");
			break;
		case 3:
			strcpy(temp,"name");
			break;
		case 4:
			strcpy(temp,"passwd");
			break;
		case 5:
			strcpy(temp,"age");
			break;
		case 6:
			strcpy(temp,"phone");
			break;
		case 7:
			strcpy(temp,"addr");
			break;
		case 8:
			strcpy(temp,"work");
			break;
		case 9:
			strcpy(temp,"date");
			break;
		case 10:
			strcpy(temp,"level");
			break;
		case 11:
			strcpy(temp,"salary");
			break;
		default:
			printf("input error");
	}

	sprintf(sql,"update usrinfo set '%s' = '%s' where name = '%s';",temp,msg->recvmsg,msg->info.name);

	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK)
		printf("update failed,%s\n",errmsg);
	strcpy(msg->recvmsg,"modify ok");
	send(acceptfd,msg,sizeof(MSG),0);


	return;
}


int process_admin_deluser_request(int acceptfd,MSG *msg)
{
	char *errmsg;
	printf("-------%s---------%d\n",__func__,__LINE__);
	char sql[DATALEN] = {0};
	printf("delete user: %s\n",msg->info.name);

	sprintf(sql,"delete from usrinfo where name = '%s';",msg->info.name);

	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK)
		printf("delete failed,%s\n",errmsg);

	strcpy(msg->recvmsg,"delete ok");
	send(acceptfd,msg,sizeof(MSG),0);
	return 0;
}

int process_user_or_admin_login_request(int acceptfd,MSG *msg)
{
	printf("----------%s---------%d\n",__func__,__LINE__);
	char sql[DATALEN] = {0};
	char *errmsg;
	char **result;
	int nrow,ncolumn;

	msg->info.usertype = msg->usertype;
	strcpy(msg->info.name,msg->username);
	strcpy(msg->info.passwd,msg->passwd);

	printf("usrtype: %#x---usrname: %s---passwd: %s\n",msg->info.usertype,msg->info.name,msg->info.passwd);
	sprintf(sql,"select * from usrinfo where usertype = %d and name='%s' and passwd='%s';",msg->info.usertype,msg->info.name,msg->info.passwd);

	if(sqlite3_get_table(db,sql,&result,&nrow,&ncolumn,&errmsg) != SQLITE_OK)
		printf("---%s\n",errmsg);
	else
	{
		if(nrow == 0)
		{
			strcpy(msg->recvmsg,"name or passwd failed\n");
			send(acceptfd,msg,sizeof(MSG),0);
		}
		else
		{
			strcpy(msg->recvmsg,"OK");
			send(acceptfd,msg,sizeof(MSG),0);
		}
	}
    return 0;

}
int process_client_request(int acceptfd,MSG *msg)
{
	printf("--------%s-------%d\n",__func__,__LINE__);
	switch(msg->msgtype)
	{
		case USER_LOGIN:
		case ADMIN_LOGIN:
			process_user_or_admin_login_request(acceptfd,msg);
			break;
/*		case USER_MODIFY:
			process_user_modify_request(acceptfd,msg);
			break;
		case USER_QUERY:
			process_user_query_request(acceptfd,msg);
			break;_*/
		case ADMIN_MODIFY:
			process_admin_modify_request(acceptfd,msg);
			break;
	/*	case ADMIN_ADDUSER:
			process_Admin_adduser_request(acceptfd,msg);
			break;*/
		case ADMIN_DELUSER:
			process_admin_deluser_request(acceptfd,msg);
			break;/*
		case ADMIN_QUERY:
			process_admin_query_request(acceptfd,msg);
			break;
		case QUIT:
			process_client_quit_request(acceptfd,msg);
			break;*/
		default:
			break;
	}
}



int main(int argc,const char *argv[])
{
	//char come[8] = {0};
	int sockfd;
	int acceptfd;
	ssize_t recvbytes;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);

	MSG msg;
	char *errmsg;

	if(sqlite3_open(STAFF_DATABASE,&db) != SQLITE_OK)
	{
		printf("%s\n",sqlite3_errmsg(db));
	}
	else
		printf("database open success\n");

	if(sqlite3_exec(db,"create table usrinfo(staffno integer,usertype integer,name text,passwd text,age integer,phone text,addr text,date text,level integer,salary REAL);",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("%s\n",errmsg);
	}
	else
		printf("create usrinfo table success\n");

	if(sqlite3_exec(db,"create table historyinfo(time text,name text,words text);",NULL,NULL,&errmsg) != SQLITE_OK)
	{
		printf("$s\n",errmsg);
	}
	else
		printf("create historyinfo table success\n");

	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd == -1)
	{
		perror("socket failed\n");
		exit(-1);
	}
	printf("sockfd :%d\n",sockfd);

	int b_reuse = 1;
	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&b_reuse,sizeof(int));

	memset(&serveraddr,0,sizeof(serveraddr));
	memset(&clientaddr,0,sizeof(clientaddr));

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	if(bind(sockfd,(const struct sockaddr *)&serveraddr,addrlen) == -1)
	{
		printf("bind failed\n");
		exit(-1);
	}
	
	if(listen(sockfd,10) == -1)
	{
		printf("listen failed\n");
		exit(-1);
	}

	fd_set readfds,tempfds;

	FD_ZERO(&readfds);
	FD_ZERO(&tempfds);

	FD_SET(sockfd,&readfds);

	int nfds = sockfd;
	int retval;
	int i = 0;

	while(1)
	{
		tempfds = readfds;
		retval = select(nfds+1,&tempfds,NULL,NULL,NULL);
		for(i = 0 ; i < nfds+1 ; i++)
		{
			if(FD_ISSET(i,&tempfds))
			{
				if(i == sockfd)
				{
					acceptfd = accept(sockfd,(struct sockaddr *)&clientaddr,&cli_len);
					if(acceptfd == -1)
					{
						printf("accept failed\n");
						exit(-1);
					}
					printf("ip : %s\n",inet_ntoa(clientaddr.sin_addr));
					FD_SET(acceptfd,&readfds);
					nfds = nfds>acceptfd?nfds:acceptfd;
				}
				else
				{
					recvbytes = recv(i,&msg,sizeof(msg),0);
					printf("msg.type: %#x\n",msg.msgtype);
					if(recvbytes == -1)
					{
						printf("recv failed\n");
					 	continue;
					}
					else if(recvbytes == 0)
					{
						printf("peer shutdown\n");
						close(i);
						FD_CLR(i,&readfds);
					}
					else
					{
						process_client_request(i,&msg);
					}
				}
			}
		}
	}
	close(sockfd);
	
	return 0;
}

