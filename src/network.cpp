#include "network.h"
using namespace std;
//网络初始化
void Network::init(eRole role, int port)
{
    this->role = role;
    this->port = port;
    //创建socket
    if ((this->sockSer = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("Socket error : %s\n", strerror(errno));
        exit(1);
    }
    //填充套接字地址结构，包括地址族，ip和端口号
    bzero(&this->addrSer, sizeof(struct sockaddr_in));
    inet_aton(ipAddr, &(this->addrSer.sin_addr));
    this->addrSer.sin_family = AF_INET;
    this->addrSer.sin_port = htons(port);
    int opt = SO_REUSEADDR;
    setsockopt(this->sockSer, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // 接收缓冲区
    int nRecvBuf = 256 * 1024; //设置为256K
    setsockopt(this->sockSer, SOL_SOCKET, SO_RCVBUF, (const char *)&nRecvBuf, sizeof(int));
    // 发送缓冲区
    int nSendBuf = 256 * 1024; //设置为256K
    setsockopt(this->sockSer, SOL_SOCKET, SO_SNDBUF, (const char *)&nSendBuf, sizeof(int));
    getsockopt(this->sockSer, SOL_SOCKET, SO_RCVBUF, (char *)&nRecvBuf, (socklen_t *)sizeof(int));
    cout << "nRecvBuf:" << nRecvBuf << endl;
    if (this->role == SERVER)
    {
        //绑定
        if (bind(sockSer, (struct sockaddr *)(&this->addrSer), sizeof(struct sockaddr)) == -1)
        {
            printf("Bind error : %s\n", strerror(errno));
            exit(1);
        }
        // 监听
        if (listen(sockSer, 1) == -1)
        {
            printf("Listen error : %s\n", strerror(errno));
            exit(1);
        }
        //接受
        socklen_t naddr = sizeof(struct sockaddr_in);
        if ((this->sockCli = accept(this->sockSer, (struct sockaddr *)(&this->addrCli), &naddr)) == -1)
        {
            printf("Accept error%s\n", strerror(errno));
            exit(1);
        }
    }
    else
    {
        //连接
        int times = 1;
        while (connect(sockSer, (struct sockaddr *)&this->addrSer, sizeof(struct sockaddr)) == -1)
        {
            sleep(times++);
            if (times > 5)
            {
                printf("Connect error : %s\n", strerror(errno));
                exit(1);
            }
        }
    }
}
//发送一个string
bool Network::mSend(int fd, string send_string)
{
    char *cstr = new char[send_string.size() + 1];
    strcpy(cstr, send_string.c_str());
    size_t send_size = send_string.size() + 1; //需要发送的数据大小
    string send_size_string = to_string(send_size);
    char *c_send_size_str = new char[send_size_string.size() + 1];
    strcpy(c_send_size_str, send_size_string.c_str());
    if (send(fd, c_send_size_str, send_size_string.size() + 1, 0) == -1)
    { //告知对方需要准备的缓冲区大小
        printf("Prewrite error : %s\n", strerror(errno));
        exit(1);
    }
    if (recv(fd, this->checkBuf, BUF_SIZE, 0) == -1 && !(strcmp(this->checkBuf, c_send_size_str)))
    { //接收确认信息
        printf("Check error : %s\n", strerror(errno));
        exit(1);
    }
    ssize_t send_num;
    if ((send_num = send(fd, cstr, send_size, 0)) == -1)
    { //发送数据
        printf("Write error : %s\n", strerror(errno));
        exit(1);
    }
    cout << "send num is: " << send_size << " and sended: " << send_num << endl;
    // cout << "send:\n" << cstr << endl;
    delete[] cstr;
    delete[] c_send_size_str;
    return true;
}
//接收一个char指针
char *Network::mReceive(int fd)
{
    if (recv(fd, this->recvSizeBuf, BUF_SIZE, 0) == -1)
    { //接收缓冲区尺寸
        printf("Preread error : %s\n", strerror(errno));
        exit(1);
    }
    if (send(fd, this->recvSizeBuf, BUF_SIZE, 0) == -1)
    { //发送确认信息
        printf("Check error : %s\n", strerror(errno));
        exit(1);
    }
    size_t recv_size = atol(this->recvSizeBuf); //接收缓冲区尺寸
    char *cstr = new char[recv_size];           //接收缓冲区
    string recv_temp;
    ssize_t recv_num, remain_num = recv_size;
    int flag = 0;
    while (remain_num > 0)
    {
        if ((recv_num = recv(fd, cstr, recv_size, 0)) == -1 || recv_num == 0)
        { //接收数据
            printf("Read error : %s\n", strerror(errno));
            exit(1);
        }
        remain_num -= recv_num;
        recv_temp += cstr;
        if (remain_num > 0)
            flag++;
        cout << "recv num is: " << recv_num << " size is: " << recv_size << " and remain is: " << remain_num << endl;
    }
    if (flag)
        strcpy(cstr, recv_temp.c_str());
    // cout << "recv:\n" << cstr << endl;
    return cstr;
}
//发送两个mpz_class数
bool Network::mSend(mpz_class send1, mpz_class send2)
{
    int fd = (this->role == SERVER) ? this->sockCli : sockSer;
    string send_one = send1.get_str();
    string send_two = send2.get_str();
    string send_string = send_one + "|" + send_two;
    this->mSend(fd, send_string);
    return true;
}
//接收两个mpz_class数
bool Network::mReceive(mpz_class &receive1, mpz_class &receive2)
{
    int fd = (this->role == SERVER) ? this->sockCli : sockSer;
    char *cstr = this->mReceive(fd);
    char *recv1 = strtok(cstr, this->delim);
    char *recv2 = strtok(NULL, this->delim);
    receive1 = mpz_class(recv1, baseNum);
    receive2 = mpz_class(recv2, baseNum);
    delete[] cstr;
    return true;
}
//发送两个矩阵
bool Network::mSend(Matrix matrix1, Matrix matrix2)
{
    int fd = (this->role == SERVER) ? this->sockCli : sockSer;
    string matrix1_string, matrix2_string;
    string matrix1_string_row = to_string(matrix1.row);
    string matrix1_string_col = to_string(matrix1.col);
    matrix1_string = matrix1_string_row + this->mDelim + matrix1_string_col + this->mDelim;
    for (int mRow = 0; mRow < matrix1.row; mRow++)
    {
        for (int mCol = 0; mCol < matrix1.col; mCol++)
        {
            string temp = matrix1.matrix[mRow][mCol].get_str();
            matrix1_string += temp;
            if ((mRow < matrix1.row - 1) || (mCol < matrix1.col - 1))
                matrix1_string += this->mDelim;
        }
    }
    string matrix2_string_row = to_string(matrix2.row);
    string matrix2_string_col = to_string(matrix2.col);
    matrix2_string = matrix2_string_row + this->mDelim + matrix2_string_col + this->mDelim;
    for (int mRow = 0; mRow < matrix2.row; mRow++)
    {
        for (int mCol = 0; mCol < matrix2.col; mCol++)
        {
            string temp = matrix2.matrix[mRow][mCol].get_str();
            matrix2_string += temp;
            if ((mRow < matrix2.row - 1) || (mCol < matrix2.col - 1))
                matrix2_string += this->mDelim;
        }
    }
    string send_string = matrix1_string + this->delim + matrix2_string;
    this->mSend(fd, send_string);
    return true;
}
//接收两个矩阵
bool Network::mReceive(Matrix &matrix1, Matrix &matrix2)
{
    int fd = (this->role == SERVER) ? this->sockCli : sockSer;
    char *cstr = this->mReceive(fd);

    char *recv1 = strtok(cstr, this->delim);
    char *recv2 = strtok(NULL, this->delim);

    char *pch;
    int counts = 0, row, col;
    pch = strtok(recv1, this->mDelim);
    if (pch == NULL)
    {
        printf("Reveice matrix error!(1) : %s\n", strerror(errno));
        exit(1);
    }
    int matrix_row = atoi(pch);
    pch = strtok(NULL, this->mDelim);
    if (pch == NULL)
    {
        printf("Reveice matrix error!(2) : %s\n", strerror(errno));
        exit(1);
    }
    int matrix_col = atoi(pch);

    Matrix matrix_one(M_NORMAL, 0, matrix_row, matrix_col);
    for (int i = 0; i < matrix_row; i++)
    {
        for (int j = 0; j < matrix_col; j++)
        {
            pch = strtok(NULL, this->mDelim);
            long temp = atol(pch);
            matrix_one.change(i, j, temp);
        }
    }

    counts = 0;
    pch = strtok(recv2, this->mDelim);
    if (pch == NULL)
    {
        printf("Reveice matrix error!(3) : %s\n", strerror(errno));
        exit(1);
    }
    matrix_row = atoi(pch);
    pch = strtok(NULL, this->mDelim);
    if (pch == NULL)
    {
        printf("Reveice matrix error!(4) : %s\n", strerror(errno));
        exit(1);
    }
    matrix_col = atoi(pch);
    Matrix matrix_two(M_NORMAL, 0, matrix_row, matrix_col);
    for (int i = 0; i < matrix_row; i++)
    {
        for (int j = 0; j < matrix_col; j++)
        {
            pch = strtok(NULL, this->mDelim);
            long temp = atol(pch);
            matrix_two.change(i, j, temp);
        }
    }

    this->networkTools.mCopy(matrix_one, matrix1);
    this->networkTools.mCopy(matrix_two, matrix2);
    delete[] cstr;
    return true;
}
//发送两个矩阵数组
bool Network::mSend(array<Matrix, 5> array1, array<Matrix, 5> array2)
{
    int fd = (this->role == SERVER) ? this->sockCli : sockSer;
    array<array<Matrix, 5>, 2> mArray{array1, array2};
    string array1_string, array2_string;
    string array_string[2] = {array1_string, array2_string};
    string send_string;
    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < 5; i++)
        {
            string matrix1_string_row = to_string(mArray[k][i].row);
            string matrix1_string_col = to_string(mArray[k][i].col);
            array_string[k] += (matrix1_string_row + this->mDelim + matrix1_string_col + this->mDelim);
            for (int mRow = 0; mRow < mArray[k][i].row; mRow++)
            {
                for (int mCol = 0; mCol < mArray[k][i].col; mCol++)
                {
                    string temp = mArray[k][i].matrix[mRow][mCol].get_str();
                    array_string[k] += temp;
                    if ((mRow < mArray[k][i].row - 1) || (mCol < mArray[k][i].col - 1))
                        array_string[k] += this->mDelim;
                }
            }
            if (i < 4)
                array_string[k] += this->aDelim;
        }
        send_string += array_string[k];
        if (k < 1)
            send_string += this->delim;
    }
    this->mSend(fd, send_string);
    return true;
}
//接收两个矩阵数组
bool Network::mReceive(array<Matrix, 5> &array1, array<Matrix, 5> &array2)
{
    int fd = (this->role == SERVER) ? this->sockCli : sockSer;
    char *cstr = this->mReceive(fd);
    char *cArray1 = strtok(cstr, this->delim);
    char *cArray2 = strtok(NULL, this->delim);
    char *arrays[2] = {cArray1, cArray2};
    Matrix cmatrix1, cmatrix2, cmatrix3, cmatrix4, cmatrix5;
    Matrix cmatrix6, cmatrix7, cmatrix8, cmatrix9, cmatrix10;
    array<Matrix, 5> mArray1 = {cmatrix1, cmatrix2, cmatrix3, cmatrix4, cmatrix5};
    array<Matrix, 5> mArray2 = {cmatrix6, cmatrix7, cmatrix8, cmatrix9, cmatrix10};
    array<array<Matrix, 5>, 2> mArrays{mArray1, mArray2};
    for (int k = 0; k < 2; k++)
    {
        char *matrix1 = strtok(arrays[k], this->aDelim);
        if (matrix1 == NULL)
        {
            printf("Reveice matrix array error!(-1) : %s\n", strerror(errno));
            exit(1);
        }
        char *matrix2 = strtok(NULL, this->aDelim);
        if (matrix2 == NULL)
        {
            printf("Reveice matrix array error!(-2) : %s\n", strerror(errno));
            exit(1);
        }
        char *matrix3 = strtok(NULL, this->aDelim);
        if (matrix3 == NULL)
        {
            printf("Reveice matrix array error!(-3) : %s\n", strerror(errno));
            exit(1);
        }
        char *matrix4 = strtok(NULL, this->aDelim);
        if (matrix4 == NULL)
        {
            printf("Reveice matrix array error!(-4) : %s\n", strerror(errno));
            exit(1);
        }
        char *matrix5 = strtok(NULL, this->aDelim);
        if (matrix5 == NULL)
        {
            printf("Reveice matrix array error!(-5) : %s\n", strerror(errno));
            exit(1);
        }
        char *matrixs[5] = {matrix1, matrix2, matrix3, matrix4, matrix5};
        for (int t = 0; t < 5; t++)
        {
            char *pch;
            int counts = 0, row = 0, col = 0;
            pch = strtok(matrixs[t], this->mDelim);
            if (pch == NULL)
            {
                printf("Reveice matrix array error!(1) : %s\n", strerror(errno));
                exit(1);
            }
            int matrix_row = atoi(pch);
            pch = strtok(NULL, this->mDelim);
            if (pch == NULL)
            {
                printf("Reveice matrix array error!(2) : %s\n", strerror(errno));
                exit(1);
            }
            int matrix_col = atoi(pch);

            if (matrix_row <= 0 || matrix_col <= 0)
            {
                printf("Parsing error!");
                exit(1);
            }
            Matrix matrix(M_NORMAL, 0, matrix_row, matrix_col);
            for (int i = 0; i < matrix_row; i++)
            {
                for (int j = 0; j < matrix_col; j++)
                {
                    pch = strtok(NULL, this->mDelim);
                    if (pch == NULL)
                    {
                        cout << "Exchange error!" << endl;
                        exit(1);
                    }
                    long temp = atol(pch);
                    matrix.change(i, j, temp);
                }
            }
            this->networkTools.mCopy(matrix, mArrays[k][t]);
            // mArrays[k][t] = matrix;
        }
    }
    array1 = mArrays[0];
    array2 = mArrays[1];
    delete[] cstr;
    return true;
}
//关闭套接字
Network::~Network()
{
    int t = 10;
    if (sockSer)
        close(sockSer);
    if (sockCli)
        close(sockCli);
}
