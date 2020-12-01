#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "matrix.h"

using namespace std;

const int BUF_SIZE = 2048;

class client
{
public:
	int port = 1090;
	char *ipAddr = (char*)"127.0.0.1";
	int sockSer;
	struct sockaddr_in addrSer, addrCli;

	char sendbuf[BUF_SIZE];
	char inputbuf[BUF_SIZE];
	char recvbuf[BUF_SIZE];

	void run()
	{
		//创建socket
		if ((this->sockSer = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			printf("Socket error : %s\n", strerror(errno));
			exit(1);
		}
		bzero(&this->addrSer, sizeof(struct sockaddr_in));
		inet_aton(this->ipAddr, &(this->addrSer.sin_addr));
		this->addrSer.sin_family = AF_INET;
		this->addrSer.sin_port = htons(port);
		while (connect(sockSer, (struct sockaddr *)&this->addrSer, sizeof(struct sockaddr)) == -1)
		{
			sleep(50000);
		}
	}

	void recv1(Matrix *x)
	{
		int i, j, k;
		for (k = 0; k < 5; k++)
		{
			for (i = 0; i < x[k].row; i++)
			{
				for (j = 0; j < x[k].col; j++)
				{
					string a;
					recv(sockSer, recvbuf, sizeof(recvbuf), 0);
					while (strlen(recvbuf) == 0)
					{
						sleep(10000);
						recv(sockSer, recvbuf, sizeof(recvbuf), 0);
					}
					strcpy(&a[0], recvbuf);
					x[k].matrix[i][j] = mpz_class(a);

					memset(sendbuf, 0, 2048 * sizeof(char));
					memset(recvbuf, 0, 2048 * sizeof(char));
					memset(&a[0], 0, a.length() * sizeof(char));
				}
			}
		}
	}

	void send1(Matrix *x)
	{
		int i, j, k;
		for (k = 0; k < 5; k++)
		{
			for (i = 0; i < x[k].row; i++)
			{
				for (j = 0; j < x[k].col; j++)
				{
					string a = x[k].matrix[i][j].get_str();
					strcpy(sendbuf, &a[0]);
					send(sockSer, sendbuf, sizeof(sendbuf), 0);
					memset(sendbuf, 0, 2048 * sizeof(char));
					memset(recvbuf, 0, 2048 * sizeof(char));
					memset(&a[0], 0, a.length() * sizeof(char));
				}
			}
		}
	}

	void recv2(Matrix &x)
	{
		int i, j, k;
		for (i = 0; i < x.row; i++)
		{
			for (j = 0; j < x.col; j++)
			{
				string a;
				recv(sockSer, recvbuf, sizeof(recvbuf), 0);
				while (strlen(recvbuf) == 0)
				{
					sleep(10000);
					recv(sockSer, recvbuf, sizeof(recvbuf), 0);
				}
				strcpy(&a[0], recvbuf);
				x.matrix[i][j] = mpz_class(a);

				memset(sendbuf, 0, 2048 * sizeof(char));
				memset(recvbuf, 0, 2048 * sizeof(char));
				memset(&a[0], 0, a.length() * sizeof(char));
			}
		}
	}

	void send2(Matrix &x)
	{
		int i, j, k;
		for (i = 0; i < x.row; i++)
		{
			for (j = 0; j < x.col; j++)
			{
				string a = x.matrix[i][j].get_str();
				strcpy(sendbuf, &a[0]);
				send(sockSer, sendbuf, sizeof(sendbuf), 0);
				memset(sendbuf, 0, 2048 * sizeof(char));
				memset(recvbuf, 0, 2048 * sizeof(char));
				memset(&a[0], 0, a.length() * sizeof(char));
			}
		}
	}

	void recv3(mpz_class &x)
	{
		string a;
		recv(sockSer, recvbuf, sizeof(recvbuf), 0);
		while (strlen(recvbuf) == 0)
		{
			sleep(10000);
			recv(sockSer, recvbuf, sizeof(recvbuf), 0);
		}
		strcpy(&a[0], recvbuf);
		x = mpz_class(a);

		memset(sendbuf, 0, 2048 * sizeof(char));
		memset(recvbuf, 0, 2048 * sizeof(char));
		memset(&a[0], 0, a.length() * sizeof(char));
	}

	void send3(mpz_class &x)
	{
		string a = x.get_str();
		strcpy(sendbuf, &a[0]);
		send(sockSer, sendbuf, sizeof(sendbuf), 0);
		memset(sendbuf, 0, 2048 * sizeof(char));
		memset(recvbuf, 0, 2048 * sizeof(char));
		memset(&a[0], 0, a.length() * sizeof(char));
	}

	void clean()
	{
		if (sockSer)
			close(sockSer);
		//WSACleanup();
	}
	~client(){
		this->clean();
	}
};
