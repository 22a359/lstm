#ifndef __GLOBAL__
#define __GLOBAL__

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <array>
#include <cassert>
#include <stack>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <gmp.h>
#include <gmpxx.h>

using namespace std;

#define eRole bool
#define SERVER true
#define CLIENT false
#define M_NORMAL true
#define M_DIAGONAL false
#define M_GAUSS true

extern int baseNum;
extern string modNumStr;
extern mpz_class modNum;
extern mpz_class eAndC;
extern mp_bitcnt_t randBit;
extern char tDelim[2];
extern char mDelim[2];
extern char iDelim[2];
extern char checkMSG[3];

//将string转换为char*
char *stringToChar(string in_string);

#endif
