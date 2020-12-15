#include "global.h"

mpz_class modNum{"618970019642690137449562111", 10};
int baseNum = 10;
mp_bitcnt_t randBit = 512;
//mp_bitcnt_t eAndC = 32;
mp_bitcnt_t eAndC = 24;
//mpz_class eAndC{"4294967296", 10};//2^32
//mpz_class eAndC{"16777216", 10};//2^24
//string modNumStr = "17";//小素数
//string modNumStr = "40343";//小素数
//string modNumStr = "618970019642690137449562111"; //大素数
char tDelim[2] = "-";
char mDelim[2] = ",";
char iDelim[2] = ":";
char checkMSG[3] = "ok";
mpz_class sig0{"8388608", 10};
mpz_class sig1{"4184769", 10};
mpz_class sig2{"0", 10};
mpz_class sig3{"618970019642690137449252062", 10};
/* int m1m1m1_counts = 49626;
int m40m58m1_counts = 80;
int m40m40m1_counts = 472;
int m20m40m1_counts = 1;
int m1m20m1_counts = 1;
int m1m1m20_counts = 1;
int m20m20m1_counts = 1;
int m20m1m1_counts = 1;
int m20m1m40_counts = 1;
int m40m40m20_counts = 1;
int m40m20m1_counts = 1;
int m40m1m40_counts = 240;
int m40m1m58_counts = 80; */

int m1m1m1_counts = 1;
int m40m58m1_counts = 1;
int m40m40m1_counts = 1;
int m20m40m1_counts = 1;
int m1m20m1_counts = 1;
int m1m1m20_counts = 1;
int m20m20m1_counts = 1;
int m20m1m1_counts = 1;
int m20m1m40_counts = 1;
int m40m40m20_counts = 1;
int m40m20m1_counts = 1;
int m40m1m40_counts = 1;
int m40m1m58_counts = 1;

int m1m1m1_num = 0;
int mTriples_num[12] = {0};
int triples_num[13] = {m1m1m1_counts,
                       m40m58m1_counts,
                       m40m40m1_counts,
                       m20m40m1_counts,
                       m1m20m1_counts,
                       m1m1m20_counts,
                       m20m20m1_counts,
                       m20m1m1_counts,
                       m20m1m40_counts,
                       m40m40m20_counts,
                       m40m20m1_counts,
                       m40m1m40_counts,
                       m40m1m58_counts};
time_t start, lastPoint;

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
    mpz_class ans = 0;
    mpz_t z, r;
    mpz_inits(z, r, NULL);
    random_device rd;
    default_random_engine e(rd());
    gmp_randstate_t grt;
    gmp_randinit_mt(grt);
    gmp_randseed_ui(grt, e());
    mpz_urandomb(z, grt, randBit);
    mpz_mod(r, z, modNum.get_mpz_t());
    mpz_swap(ans.get_mpz_t(), r);
    mpz_clears(z, r, NULL);
    gmp_randclear(grt);
    return ans;
}

void showTime()
{
    time_t point = time(NULL);
    time_t allTime = (double) (point - start);
    time_t thisTime = (double) (point - lastPoint);
    cout << "Using time: " << thisTime / 86400 << "d " << thisTime / 3600 % 24 << "h " << thisTime / 60 % 60 << "m "
         << thisTime % 60 << "s | "
         << "Total time: " << allTime / 86400 << "d " << allTime / 3600 % 24 << "h " << allTime / 60 % 60 << "m "
         << allTime % 60 << "s" << endl;
    lastPoint = point;
}

void showTime(int flag)
{
    time_t point = time(NULL);
    time_t allTime = (double) (point - start);
    cout << "Total time: " << allTime / 86400 << "d " << allTime / 3600 % 24 << "h " << allTime / 60 % 60 << "m "
         << allTime % 60 << "s" << endl;
}

void mpz_print(mpz_class num, string out_word)
{
    if (LOG)
    {
        cout << out_word << ": ";
        cout << num.get_str() << endl;
    }
}
