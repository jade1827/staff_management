/*************************************************************************
#	 FileName	: client.c
#	 Author		: group
#	 Email		: group@gmail.com
#	 Created	: 2019年06月12日
 ************************************************************************/

#include<stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#include "common.h"


/**************************************
 *函数名：do_query
 *参   数：消息结构体
 *功   能：登陆
 ****************************************/
void do_admin_query(int sockfd,MSG *msg)
{
	//printf("------------%s-----------%d.\n",__func__,__LINE__);

	msg->msgtype=ADMIN_QUERY;
	send(sockfd,msg,sizeof(MSG),0);
	printf("staffno  usertype   name  passwd  age   phone	addr   work  date  level  salary\n");
	printf("=====================================================================================\n");
	while(1){
		recv(sockfd,msg,sizeof(MSG),0);
		if(strncmp(msg->recvmsg,"ADMIN_QUERY",11)==0){
			memset(msg->recvmsg,0,sizeof(msg->recvmsg));
			break;
		}
		printf("%d  %d  %s  %s %d %s %s  %s  %s  %d %lf \n",msg->info.no,msg->info.usertype,
				msg->info.name,msg->info.passwd,msg->info.age,msg->info.phone,
				msg->info.addr,msg->info.work,msg->info.date,msg->info.level,msg->info.salary);

	}
	return;
}

/**************************************
 *函数名：admin_modification
 *参   数：消息结构体
 *功   能：管理员修改
 ****************************************/
void do_admin_modification(int sockfd,MSG *msg)//管理员修改
{
//	printf("-------%s--------%d\n",__func__,__LINE__);
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

	
	if((send(sockfd,msg,sizeof(MSG),0)) <0 ){
		perror("faile to send ---- admin modify\n");
	}
	
	if((recv(sockfd,msg,sizeof(MSG),0)) <0 ){
		perror("faile to send ---- admin modify\n");
	}else{
		printf("%s\n",msg->recvmsg);
	}
	return ;
}


/**************************************
 *函数名：admin_adduser
 *参   数：消息结构体
 *功   能：管理员创建用户
 ****************************************/
void do_admin_adduser(int sockfd,MSG *msg)//管理员添加用户
{		
	//	printf("------------%s-----------%d.\n",__func__,__LINE__);

	msg->msgtype = ADMIN_ADDUSER;
	msg->usertype = ADMIN;


	printf("please infut staff nomber:\n");
	scanf("%d",&(msg->info.no));
	getchar();

	printf("please infut usertypr:(admin:1 user:2)\n");
	scanf("%d",&(msg->info.usertype));
	getchar();

	printf("please input username:\n");
	scanf("%s",msg->info.name);
	getchar();

	printf("please input password:\n");
	scanf("%s",msg->info.passwd);
	getchar();

	printf("please input age:\n");
	scanf("%d",&(msg->info.age));
	getchar();

	printf("please input user phonenum:\n");
	scanf("%s",msg->info.phone);
	getchar();

	printf("please input home address:\n");
	scanf("%s",msg->info.addr);
	getchar();

	printf("please input user's work:\n");
	scanf("%s",msg->info.work);
	getchar();

	printf("please input date:\n");
	scanf("%s",msg->info.date);
	getchar();

	printf("please input level\n");
	scanf("%d",&(msg->info.level));
	getchar();

	printf("please input salary\n");
	scanf("%lf",&(msg->info.salary));
	getchar();

	if(send(sockfd,msg,sizeof(MSG),0) < 0){
		printf("failed to send ---- admin_adduser");
	}

	if(recv(sockfd,msg,sizeof(MSG),0) < 0){
		printf("failed to reveive form server--- admin_adduser");	
	} 

	if(msg->flags == 1){
		printf("msg->recvmsg:%s . ,success\n",msg->recvmsg);
		msg->msgtype = ADMIN_LOGIN;
		admin_menu(sockfd,msg);
	}


}
/**************************************
 *函数名：admin_deluser
 *参   数：消息结构体
 *功   能：管理员删除用户
 ****************************************/
void do_admin_deluser(int sockfd,MSG *msg)//管理员删除用户
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



/**************************************
 *函数名：do_history
 *参   数：消息结构体
 *功   能：查看历史记录
 ****************************************/
void do_admin_history (int sockfd,MSG *msg)
{
	//printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype = ADMIN_HISTORY;
	msg->flags = 0;
	if ((send(sockfd,msg,sizeof(MSG),0)) < 0){
		printf("histroy send error\n");
	}

	while(msg->flags = 0){
		recv(sockfd,msg,sizeof(MSG),0);
		printf("msg->info:%s\n",msg->info.date);
	}
}


/**************************************
 *函数名：admin_menu
 *参   数：套接字、消息结构体
 *功   能：管理员菜单
 ****************************************/
void admin_menu(int sockfd,MSG *msg)
{
	int n;

	while(1)
	{
		printf("*************************************************************\n");
		printf("* 1：查询  2：修改 3：添加用户  4：删除用户  5：查询历史记录*\n");
		printf("* 6：退出													*\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
		case 1:
			do_admin_query(sockfd,msg);
			break;
		case 2:
			do_admin_modification(sockfd,msg);
			break;
		case 3:
			do_admin_adduser(sockfd,msg);
			break;
		case 4:
			do_admin_deluser(sockfd,msg);
			break;
		case 5:
			do_admin_history(sockfd,msg);
			break;
		case 6:
			msg->msgtype = QUIT;
			send(sockfd, msg, sizeof(MSG), 0);
			break;
			//close(sockfd);
			//exit(0);
		default:
			printf("您输入有误，请重新输入！\n");
		}
	}
}






/**************************************
 *函数名：do_query
 *参   数：消息结构体
 *功   能：登陆
 ****************************************/
void do_user_query(int sockfd,MSG *msg)
{
	//printf("------------%s-----------%d.\n",__func__,__LINE__);
	msg->msgtype = USER_QUERY;
	msg->usertype = USER;
	msg->flags = 0;
	printf("username:%s\n",msg->info.name);
	if(send(sockfd,msg,sizeof(MSG),0) < 0){
		perror("failed to send ---- do_user_query\n");
	}
	if(recv(sockfd,msg,sizeof(MSG),0) < 0){
		perror("failed to receive from server ---- do_user_query\n");
	}
	if(msg->flags == 1){
		printf("no:%d,type:%d,name:%s,pwd:%s,age:%d,phone:%s,addr:%s,work:%s,date;%s,level:%d,salary:%lf\n",\
				msg->info.no, msg->info.usertype, msg->info.name, msg->info.passwd, msg->info.age,\
				msg->info.phone, msg->info.addr, msg->info.work, msg->info.date,\
				msg->info.level, msg->info.salary);
	}
}



/**************************************
 *函数名：do_modification
 *参   数：消息结构体
 *功   能：修改
 ****************************************/
void do_user_modification(int sockfd,MSG *msg)
{
	msg->msgtype = USER_MODIFY;
	msg->usertype = USER;
	msg->flags = 0;
	//printf("------------%s-----------%d.\n",__func__,__LINE__);
	printf("please input your staffnum:\n");
	scanf("%d",&(msg->info.no));
	getchar();
	printf("please select which one you want to moddify:\n");
	printf("================================================\n");
	printf("--1.password--2.age--3.phone--4.addr--\n");
	printf("================================================\n");
	int num;
	scanf("%d",&num);
	getchar();
	
		switch(num){
		case 1:
			printf("please input new psw:\n");
			scanf("%s",msg->info.passwd);
			getchar();
			msg->flags = 1;
			if((send(sockfd,msg,sizeof(MSG),0)) < 0){
				perror("failed to send pwd----user update\n");
			}
			if(recv(sockfd,msg,sizeof(MSG),0) < 0){
				perror("failed reveive from serve --- user update\n");
			}
			if(msg->flags == 0){
				printf("update %s----new pwd:%s\n",msg->recvmsg,msg->info.passwd);
			}
			break;
		case 2:
			printf("please input new age:\n");
			scanf("%d",&(msg->info.age));
			getchar();
			msg->flags = 2;
			if((send(sockfd,msg,sizeof(MSG),0)) < 0){
				perror("failed to send age----user update\n");
			}
			if(recv(sockfd,msg,sizeof(MSG),0) < 0){
				perror("failed reveive from serve --- user update\n");
			}
			if(msg->flags == 0){
				printf("update %s----new age:%d\n",msg->recvmsg,msg->info.age);
			}
			break;
		case 3:
			printf("please input new phonenum:\n");
			scanf("%s",msg->info.phone);
			getchar();
			msg->flags = 3;
			if((send(sockfd,msg,sizeof(MSG),0)) < 0){
				perror("failed to send phone----user update\n");
			}
			if(recv(sockfd,msg,sizeof(MSG),0) < 0){
				perror("failed reveive from serve --- user update\n");
			}
			if(msg->flags == 0){
				printf("update %s----new phone:%s\n",msg->recvmsg,msg->info.phone);
			}
			break;
		case 4:
			printf("please input new addr:\n");
			scanf("%s",msg->info.addr);
			getchar();
			msg->flags = 4;
			if((send(sockfd,msg,sizeof(MSG),0)) < 0){
				perror("failed to send addr----user update\n");
			}
			if(recv(sockfd,msg,sizeof(MSG),0) < 0){
				perror("failed reveive from serve --- user update\n");
			}
			if(msg->flags == 0){
				printf("update %s----new addr:%s\n",msg->recvmsg,msg->info.addr);
			}
			break;
		default:
			printf("input error\n");
		}
}


/**************************************
 *函数名：user_menu
 *参   数：消息结构体
 *功   能：管理员菜单
 ****************************************/
void user_menu(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	int n;
	while(1)
	{
		printf("*************************************************************\n");
		printf("*************  1：查询  	2：修改		3：退出	 *************\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
		case 1:
			do_user_query(sockfd,msg);
			break;
		case 2:
			do_user_modification(sockfd,msg);
			break;
		case 3:
			msg->msgtype = QUIT;
			send(sockfd, msg, sizeof(MSG), 0);
			do_login(sockfd);
		//	break;
			//close(sockfd);
		//	exit(0);
		default:
			printf("您输入有误，请输入数字\n");
			break;
		}
	}
}




int admin_or_user_login(int sockfd,MSG *msg)
{
	printf("------------%s-----------%d.\n",__func__,__LINE__);
	//输入用户名和密码
	memset(msg->username, 0, NAMELEN);
	printf("请输入用户名：");
	scanf("%s",msg->username);
	getchar();

	memset(msg->passwd, 0, DATALEN);
	printf("请输入密码（6位）");
	scanf("%s",msg->passwd);
	getchar();

	//发送登陆请求
	send(sockfd, msg, sizeof(MSG), 0);
	//接受服务器响应
	recv(sockfd, msg, sizeof(MSG), 0);
	printf("msg->recvmsg :%s\n",msg->recvmsg);

	//判断是否登陆成功
	if(strncmp(msg->recvmsg, "OK", 2) == 0)
	{
		if(msg->usertype == ADMIN)
		{
			printf("亲爱的管理员，欢迎您登陆员工管理系统！\n");
			admin_menu(sockfd,msg);
		}
		else if(msg->usertype == USER)
		{
			printf("亲爱的用户，欢迎您登陆员工管理系统！\n");
			user_menu(sockfd,msg);
		}
	}
	else
	{
		printf("登陆失败！%s\n", msg->recvmsg);
		return -1;
	}

	return 0;
}


/************************************************
 *函数名：do_login
 *参   数：套接字、消息结构体
 *返回值：是否登陆成功
 *功   能：登陆
 *************************************************/
int do_login(int sockfd)
{	
	int n;
	MSG msg;

	while(1){
		printf("*************************************************************\n");
		printf("********  1：管理员模式    2：普通用户模式    3：退出********\n");
		printf("*************************************************************\n");
		printf("请输入您的选择（数字）>>");
		scanf("%d",&n);
		getchar();

		switch(n)
		{
		case 1:
			msg.msgtype  = ADMIN_LOGIN;
			msg.usertype = ADMIN;
			break;
		case 2:
			msg.msgtype =  USER_LOGIN;
			msg.usertype = USER;
			break;
		case 3:
			msg.msgtype = QUIT;
			if(send(sockfd, &msg, sizeof(MSG), 0)<0)
			{
				perror("do_login send");
				return -1;
			}
			close(sockfd);
			exit(0);
		default:
			printf("您的输入有误，请重新输入\n"); 
		}

		admin_or_user_login(sockfd,&msg);
	}

}


int main(int argc, const char *argv[])
{
	//socket->填充->绑定->监听->等待连接->数据交互->关闭 
	int sockfd;
	int acceptfd;
	ssize_t recvbytes,sendbytes;
	struct sockaddr_in serveraddr;
	struct sockaddr_in clientaddr;
	socklen_t addrlen = sizeof(serveraddr);
	socklen_t cli_len = sizeof(clientaddr);

	//创建网络通信的套接字
	sockfd = socket(AF_INET,SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket failed.\n");
		exit(-1);
	}
	printf("sockfd :%d.\n",sockfd); 

	//填充网络结构体
	memset(&serveraddr,0,sizeof(serveraddr));
	memset(&clientaddr,0,sizeof(clientaddr));
	serveraddr.sin_family = AF_INET;
	//	serveraddr.sin_port   = htons(atoi(argv[2]));
	//	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port   = htons(5001);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if(connect(sockfd,(const struct sockaddr *)&serveraddr,addrlen) == -1){
		perror("connect failed.\n");
		exit(-1);
	}

	do_login(sockfd);

	close(sockfd);

	return 0;
}



