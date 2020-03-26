#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#define MAX_MSG 100

int main(void)
{
	int ret = -1;
	int msg_flags, msg_id;					/*msg_flags是msgget函数的参数*/
	int msg_sflags,msg_rflags;					/*发送和接受函数的参数*/
	key_t key;

	struct msgmbuf{							/*消息的缓冲区结构*/
		long mtype;
		char mtext[MAX_MSG];
		};

	struct msqid_ds msg_info;
	struct msgmbuf msg_mbuf;


	pid_t pid2=getpid();						/*求当前进程的pid*/
	printf("B进程pid: %d\n",pid2);

	key = ftok(".",'b');						/*产生key*/
	if(key != -1)								/*产生key成功*/
		printf("成功建立KEY\n");		
	else										/*产生key失败*/
		printf("建立KEY失败\n");	

	msg_flags = IPC_CREAT;								/*消息的类型*/
	msg_id = msgget(key, msg_flags|0666);				/*建立消息 一定记得权限的0666为8进制*/
	printf("msg id: %d\n",msg_id);
	if( -1 == msg_id)
	{
		printf("消息建立失败\n");
		return 0;		
	}

	/*发送终结另一进程的bash命令*/
	msg_sflags = IPC_NOWAIT;
	char buf2[128];
	sprintf(buf2,"kill %d",pid2);
	msg_mbuf.mtype = 10;

	memcpy(msg_mbuf.mtext,buf2,sizeof(buf2));		/*复制字符串*/
	ret = msgsnd(msg_id, &msg_mbuf, sizeof(buf2), msg_sflags);/*发送消息*/
	if( -1 == ret)
		printf("发送bash命令失败\n");	

	/*接收终结当前进程的bash命令 判断上一个发送消息的进程是否是本进程 是则循环 不是则跳出接收*/
	do{
		ret = msgctl(msg_id, IPC_STAT, &msg_info);
	}while(msg_info.msg_lspid==pid2);

	msg_rflags = IPC_NOWAIT|MSG_NOERROR;
	char buf1[128];
	ret = msgrcv(msg_id, &msg_mbuf, MAX_MSG,10,msg_rflags);		/*接收消息*/
	if( -1 == ret)
		printf("接收bash命令失败\n");		
	else
	{
		printf("接收bash命令成功，长度：%d\n",ret);
		strcpy(buf1,msg_mbuf.mtext);
	}


	/*消息队列之间传输信息 s是发送数据到消息队列 r是接收消息队列的数据 q是退出 默认2个进程都退出*/
	char ch;
	ch=getchar();
	while(ch!='q')
	{
		if(ch=='s')
		{
			msg_sflags = IPC_NOWAIT;
			char msg_send[MAX_MSG];
			scanf("%s",msg_send);
			msg_mbuf.mtype = 10;

			memcpy(msg_mbuf.mtext,msg_send,sizeof(msg_send));		/*复制字符串*/
			ret = msgsnd(msg_id, &msg_mbuf, sizeof(msg_send), msg_sflags);/*发送消息*/
			if( -1 == ret)
			{
				printf("发送消息失败\n");		
			}
		}
		else if(ch=='r')
		{
			msg_rflags = IPC_NOWAIT|MSG_NOERROR;
			ret = msgrcv(msg_id, &msg_mbuf, MAX_MSG,10,msg_rflags);		/*接收消息*/
			if( -1 == ret)
			{
				printf("接收消息失败\n");		
			}
			else
			{
				printf("接收消息成功，长度：%d\n",ret);
				printf("%s\n",msg_mbuf.mtext);	
			}
		}
		ch=getchar();
	}

	/*终止另一进程*/
	system(buf1);
	ret = msgctl(msg_id, IPC_RMID,NULL);				/*删除消息队列*/
	if(ret != -1)
	{
		printf("删除消息成功\n");
		return 0;		
	}
	
	return 0;
}