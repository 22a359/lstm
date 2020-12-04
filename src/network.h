#ifndef __NETWORK__
#define __NETWORK__
#include "matrix.h"

using namespace std;

const int BUF_SIZE = 2048;

class Network
{
public:
	eRole role;
	void init(eRole role, int port);						 //初始化
	bool mSend(string send_string);							 //发送一个string
	bool mSend(mpz_class send);								 //发送mpz_class数
	bool mSend(Matrix matrix);								 //发送矩阵
	bool mSend(array<Matrix, 5> array);						 //发送矩阵数组
	bool mReceive(string &recv_string);						 //接收一个string
	bool mReceive(mpz_class &receive);						 //接收mpz_class数
	bool mReceive(Matrix &matrix);							 //接收矩阵
	bool mReceive(array<Matrix, 5> &array);					 //接收矩阵数组
	bool serialization(mpz_class num, string &num_ser);		 //序列化
	bool serialization(Matrix matrix, string &matrix_ser);	 //序列化
	bool deserialization(string num_ser, mpz_class &num);	 //反序列化
	bool deserialization(string matrix_ser, Matrix &matrix); //反序列化
	~Network();
	char checkMSG[3] = "ok";

private:
	MatrixTools networkTools;
	char *ipAddr = (char *)"127.0.0.1";
	int port;
	int sockSer, sockCli;
	struct sockaddr_in addrSer, addrCli;
	char recvSizeBuf[BUF_SIZE];
	char checkBuf[BUF_SIZE];

	bool mSend(int fd, string send_string);
	bool mReceive(int fd, string &recv_string);
};

#endif