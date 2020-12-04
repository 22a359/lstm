#include "global.h"
mpz_class modNum;
int baseNum = 10;
mp_bitcnt_t randBit = 512;
mpz_class eAndC = 4294967296;
// string modNumStr = "40343";//小素数
string modNumStr = "618970019642690137449562111"; //大素数
char tDelim[2] = "-";
char mDelim[2] = ",";
char iDelim[2] = ":";
char checkMSG[3] = "ok";

//将string转换为char*
char *stringToChar(string in_string)
{
    char *cstr = new char[in_string.size() + 1];
    strcpy(cstr, in_string.c_str());
    return cstr;
}
