#ifndef __NETWORK__
#define __NETWORK__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "matrix.h"

using namespace std;

const int BUF_SIZE = 2048;

class Network
{
public:
	eRole role;
	void init(eRole role, int port);
	bool mSend(mpz_class send1, mpz_class send2);
	bool mSend(Matrix matrix1, Matrix matrix2);
	bool mSend(array<Matrix, 5> array1, array<Matrix, 5> array2);
	bool mReceive(mpz_class &receive1, mpz_class &receive2);
	bool mReceive(Matrix &matrix1, Matrix &matrix2);
	bool mReceive(array<Matrix, 5> &array1, array<Matrix, 5> &array2);
	~Network();

private:
	MatrixTools networkTools;
	char *ipAddr = (char *)"127.0.0.1";
	int port;
	int sockSer, sockCli;
	struct sockaddr_in addrSer, addrCli;
	char recvSizeBuf[BUF_SIZE];
	char checkBuf[BUF_SIZE];
	char checkMSG[3] = "ok";
	char delim[2] = "|";
	char mDelim[2] = ",";
	char aDelim[2] = "-";
	bool mSend(int fd, string send_string);
	char *mReceive(int fd);
};

#endif