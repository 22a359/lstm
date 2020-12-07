#include "global.h"
mpz_class modNum;
int baseNum = 10;
mp_bitcnt_t randBit = 512;
mp_bitcnt_t eAndC = 32;
// string modNumStr = "40343";//小素数
string modNumStr = "618970019642690137449562111"; //大素数
char tDelim[2] = "-";
char mDelim[2] = ",";
char iDelim[2] = ":";
char checkMSG[3] = "ok";
int m1m1m1_counts = 5;
int m40m58m1_counts = 5;
int m40m40m1_counts = 5;
int m20m40m1_counts = 5;
int m1m20m1_counts = 5;
int m1m1m20_counts = 5;
int m20m20m1_counts = 5;
int m20m1m1_counts = 5;
int m20m1m40_counts = 5;
int m40m40m20_counts = 5;
int m40m20m1_counts = 5;
int m40m1m40_counts = 5;
int m40m1m58_counts = 5;
//将string转换为char*
char *stringToChar(string in_string)
{
    char *cstr = new char[in_string.size() + 1];
    strcpy(cstr, in_string.c_str());
    return cstr;
}
//在大素数范围内生成随机数
mpz_class randNumGen()
{
    mpz_class ans;
    mpz_t z, r;
    mpz_init(z);
    mpz_init(r);
    random_device rd;
    default_random_engine e(rd());
    gmp_randstate_t grt;
    gmp_randinit_mt(grt);
    gmp_randseed_ui(grt, e());
    mpz_urandomb(z, grt, randBit);
    mpz_mod(r, z, modNum.get_mpz_t());
    ans = mpz_class(r);
    mpz_clears(z, r, NULL);
    gmp_randclear(grt);
    return ans;
}