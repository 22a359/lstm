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
// #define RELEASE false
#define RELEASE true

extern int baseNum;
extern string modNumStr;
extern mpz_class modNum;
extern mp_bitcnt_t eAndC;
extern mp_bitcnt_t randBit;
extern char tDelim[2];
extern char mDelim[2];
extern char iDelim[2];
extern char checkMSG[3];

extern int m1m1m1_counts;
extern int m40m58m1_counts;
extern int m40m40m1_counts;
extern int m20m40m1_counts;
extern int m1m20m1_counts;
extern int m1m1m20_counts;
extern int m20m20m1_counts;
extern int m20m1m1_counts;
extern int m20m1m40_counts;
extern int m40m40m20_counts;
extern int m40m20m1_counts;
extern int m40m1m40_counts;
extern int m40m1m58_counts;
extern time_t start, lastPoint;
extern int m1m1m1_num;
extern int mTriples_num[12];
extern int triples_num[13];
//将string转换为char*
char *stringToChar(string in_string);
//在大素数范围内生成随机数
mpz_class randNumGen();
void showTime();
void showTime(int flag);
#endif
