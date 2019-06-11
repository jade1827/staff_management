#include"common.h"

void do_admin_query(int sockfd,MSG *msg)
{
	printf("-----%s------%d\n",__func__,__LINE__);
//	printf("************************************\n");
//	printf("***1:按姓名查询        2:")

}
void do_admin_modification(int sockfd,MSG *msg)
{
	printf("-------%s--------%d\n",__func__,__LINE__);
//	memset(msg->msgtype,0,sizeof(int));
	memset(msg->recvmsg,0,DATALEN);
	memset(msg->info.name,0,NAMELEN);
	memset(msg->username,0,NAMELEN);
	msg->msgtype = ADMIN_MODIFY;
	printf("*************************************************************************************\n");
	printf("1.no 2.usertype 3.name 4.passwd 5.age 6.phone 7.addr 8.work 9.date 10.level 11.salary\n");
	printf("*************************************************************************************\n");

	printf("请输入修改员工的姓名：");
	scanf("%s",msg->info.name);
	getchar();

	printf("请输入修改哪一项：");
	scanf("%d",&msg->flags);
	getchar();

	printf("请输入修改后的值：");
	scanf("%s",msg->recvmsg);
	getchar();

	send(sockfd,msg,sizeof(MSG),0);
	
	recv(sockfd,msg,sizeof(MSG),0);

	printf("%s\n",msg->recvmsg);

	return ;
}

void do_admin_deluser(int sockfd,MSG *msg)
{
	printf("------%s--------%d\n",__func__,__LINE__);
	memset(msg->info.name,0,NAMELEN);
	msg->msgtype = ADMIN_DELUSER;
	printf("请输入要删除的用户名：");
	scanf("%s",msg->info.name);
	send(sockfd,msg,sizeof(MSG),0);

	recv(sockfd,msg,sizeof(MSG),0);
	printf("recvmsg:%s\n",msg->recvmsg);
}

void do_login(int sockfd)
{
	int n;
	MSG msg;

	while(1)
	{
		printf("*************************************\n");
		printf("*****1:admin    2:user    3:quit*****\n");
		printf("*************************************\n");
		printf("please input your choose(number) : ");
		scanf("%d",&n);
		getchar();
		
		switch(n)
		{
		case 1:
			msg.msgtype = ADMIN_LOGIN;
			msg.usertype = ADMIN;
			break;
		case 2:
			msg.msgtype = USER_LOGIN;
			msg.usertype = USER;
			break;
		case 3:
			msg.msgtype = QUIT;
			if(send(sockfd,&msg,sizeof(MSG),0) < 0)
			{
				perror("do_login send");
				return ;
			}
			close(sockfd);
			exit(0);
		default:
			printf("input error\n");
		}
	admin_or_user_login(sockfd,&msg);
	
	}
}

int admin_or_user_login(int sockfd,MSG *msg)
{
	printf("-------------%s------------%d\n",__func__,__LINE__);
	memset(msg->username,0,NAMELEN);
	printf("please input user name: ");
	scanf("%s",msg->username);
	getchar();

	memset(msg->passwd,0,DATALEN);
	printf("please input password: ");
	scanf("%s",msg->passwd);
	getchar();

	send(sockfd,msg,sizeof(MSG),0);

	recv(sockfd,msg,sizeof(MSG),0);

	printf("msg->recvmsg :%s\n",msg->recvmsg);

	if(strncmp(msg->recvmsg,"OK",2) == 0)
	{
		if(msg->usertype == ADMIN)
		{
			printf("dear admin,welcome login in system\n");
			admin_menu(sockfd,msg);
		}
		else if(msg->usertype == USER)
		{
			printf("dear user,welcome login in system\n");
//			user_menu(sockfd,msg);
		}
	}
	else
	{
		printf("login failed\n");
		return -1;
	}

	return 0;
}

void admin_menu(int sockfd,MSG *msg)
{
	int n;
	while(1)
	{
		printf("*********************************************************\n");
		printf("1:查询 2:修改 3:添加用户 4:删除用户 5:查询历史记录 6:退出\n");
		printf("*********************************************************\n");
		printf("please input your choose: ");
		scanf("%d",&n);
		getchar();
		
		switch(n)
		{
			/*case 1:
				do_admin_query(sockfd,msg);
				break;*/
			case 2:
				do_admin_modification(sockfd,msg);
				break;/*
			case 3:
				do_admin_adduser(sockfd,msg);
				break;*/
			case 4:
				do_admin_deluser(sockfd,msg);
				break;
			/*case 5:
				do_admin_history(sockfd,msg);
				break;
			case 6:
				msg->msgtype = QUIT;
				send(sockfd,msg,sizeof(MSG),0);
				close(sockfd);
				exit(0);*/
			default:
				printf("input error\n");
		}
	}
}

int main(int argc,const char *argv[])
{
	char go[5] = {'\0'};
	int sockfd;
	int acceptfd;
	ssize_t recvbytes,sendbytes;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);

	sockfd = socket(AF_INET,SOCK_STREAM,0);

	if(sockfd == -1)
	{
		perror("socket failed\n");
		exit(-1);
	}
	printf("sockfd : %d\n",sockfd);

	memset(&serveraddr,0,addrlen);
	memset(&clientaddr,0,cli_len);

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(sockfd,(const struct sockaddr *)&serveraddr,addrlen) == -1)
	{
		perror("connect failed\n");
		exit(-1);
	}
	
/*	while(1)
	{
	scanf("%s",go);
	getchar();

	send(sockfd,&go,sizeof(go),0);
	}        test communicate success or failed
*/

	do_login(sockfd);

	close(sockfd);

	return 0;
}

