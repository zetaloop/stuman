
void malloctable(int len)
{
    free(ID);
    free(name);
    free(maths);
    free(physc);
    free(clang);
    free(total);
    datalen = len;
    ID = (unsigned long long int *)malloc(datalen * sizeof(unsigned long long int));
    name = (char(*)[MAX_NAMECHAR])malloc(datalen * MAX_NAMECHAR);
    maths = (int *)malloc(datalen * sizeof(int));
    physc = (int *)malloc(datalen * sizeof(int));
    clang = (int *)malloc(datalen * sizeof(int));
    total = (int *)malloc(datalen * sizeof(int));
}

void realloctable(int len)
{
    if (len <= datalen)
    {
        return;
    }
    ID = (unsigned long long int *)realloc(ID, len * sizeof(unsigned long long int));
    name = (char(*)[MAX_NAMECHAR])realloc(name, len * MAX_NAMECHAR);
    maths = (int *)realloc(maths, len * sizeof(int));
    physc = (int *)realloc(physc, len * sizeof(int));
    clang = (int *)realloc(clang, len * sizeof(int));
    total = (int *)realloc(total, len * sizeof(int));
    datalen = len;
}

FILE *fopen_utf8(const char *filename_utf8, const char *mode)
{
    wchar_t wfilename[MAX_PATH];
    wchar_t wmode[20];
    MultiByteToWideChar(CP_UTF8, 0, filename_utf8, -1, wfilename, MAX_PATH);
    MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, 20);
    return _wfopen(wfilename, wmode);
}

// ==================== 高级字符串功能函数 ====================

int strvlen(char *str) // 计算字符串的视觉长度
{
    int length = 0;
    unsigned char c;
    for (int i = 0; str[i] != '\0'; i++)
    {
        c = (unsigned char)str[i];
        if (c < 128)
            length += 1; // ASCII字符，视为英文字符占1格
        else if ((c & 0xc0) != 0x80)
            length += 2; // 非ASCII字符的起始字节，视为中文字符占2格
    }
    return length;
}

char *int2str(int num, char *str)
{
    // char *str = (char *)malloc(MAX_NUMCHAR * sizeof(char));
    sprintf(str, "%d", num);
    return str;
}

char *llu2str(unsigned long long int num, char *str)
{
    // char *str = (char *)malloc(MAX_NUMCHAR * sizeof(char));
    sprintf(str, "%llu", num);
    return str;
}

char *double2str(double num, int precision, char *str)
{
    // char *str = (char *)malloc(MAX_NUMCHAR * sizeof(char));
    sprintf(str, "%.*lf", precision, num);
    return str;
}

void printlen(char *pre, char *str, int len, char *aft) // 补齐空格输出
{
    printf("%s%s%*s%s", pre, str, len - strvlen(str), "", aft);
}

void sprintlen(char *buf, char *pre, char *str, int len, char *aft) // 补齐空格输出
{
    // sprintf(buf + strlen(buf), "%s%s%*s%s", pre, str, len - strvlen(str), "", aft);
    // TODO: use raw char instead of str functions
    strcat(buf, pre);
    strcat(buf, str);
    int len1 = strlen(buf);
    for (int i = 0; i < len - strvlen(str); i++)
    {
        buf[len1 + i] = ' ';
    }
    buf[len1 + len - strvlen(str)] = '\0';
    strcat(buf, aft);
}

void getx(char *buffer, int bufferSize)
{
    wchar_t wstr[1024];
    DWORD charsRead = 0;

    // 读取宽字符字符串
    ReadConsoleW(GetStdHandle(STD_INPUT_HANDLE), wstr, 1023, &charsRead, NULL);
    wstr[charsRead] = L'\0'; // 确保字符串以空字符结尾

    // 移除可能的换行符
    if (wstr[charsRead - 1] == L'\n')
    {
        wstr[charsRead - 1] = L'\0';
        charsRead--;
        if (wstr[charsRead - 1] == L'\r')
        {
            wstr[charsRead - 1] = L'\0';
        }
    }

    // 获取转换后的字符串所需的大小
    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);

    // 转换宽字符字符串到 UTF-8 多字节字符串
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer, requiredSize, NULL, NULL);
}

// void _getinput_DEMO()
// {
//     char str[128] = {0};
//     getx(str, sizeof(str));
//     printf("%s\nlength: %d\n", str, strvlen(str));
// };

// ==================== 键盘读取函数 ====================

int getkey_commonmove()
{
    int ch;
    ch = _getch();
    if (ch == 0 | ch == 224) // 方向键的特殊序列
    {
        ch = _getch(); // 读取方向键的实际键码
        switch (ch)
        {
        case 72: // ^
            return 1;
        case 80: // v
            return 2;
        case 75: // <
            return 3;
        case 77: // >
            return 4;
        case 73: // PgUp
            return 5;
        case 81: // PgDn
            return 6;
        default:
            return -1;
        }
    }
    else if (ch == 13) // Enter
    {
        return 0;
    }
    return -1;
}

void getkey_any()
{
    int ch = _getch();
    if (ch == 0 | ch == 224)
        _getch();
}
