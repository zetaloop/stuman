int dis2top() // 获取光标距离顶端的高度
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        return csbi.dwCursorPosition.Y;
    }
    return -1;
}

int dis2bottom() // 获取光标距离底端的高度
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        // free handle
        return csbi.srWindow.Bottom - csbi.dwCursorPosition.Y - 1;
    }
    return -1;
}

int printflim(const char *format, ...) // 输出，但是在窗口底部截止，防止历史记录缓存bug
{
    va_list args;
    va_start(args, format);

    // 首先计算格式化字符串需要的长度
    int length = _vscprintf(format, args) + 1; // 加1为了空字符
    char *formattedStr = (char *)malloc(length * sizeof(char));

    // 创建格式化字符串
    vsnprintf(formattedStr, length, format, args);
    va_end(args);

    // 检查可用行数并逐行打印
    int availableLines = dis2bottom();
    const char *lineStart = formattedStr;
    while (*lineStart && availableLines > 0)
    {
        const char *lineEnd = strchr(lineStart, '\n');
        if (lineEnd)
        {
            printf("%.*s", lineEnd - lineStart + 1, lineStart);
            lineStart = lineEnd + 1; // 移动到下一行的开头
        }
        else
        {
            printf("%s", lineStart); // 打印剩余的所有内容
            break;
        }
        availableLines--;
    }

    free(formattedStr); // 释放分配的内存
}

int printfforce(const char *format, ...) // 输出，但是整体贴靠到窗口底部
{
    va_list args;
    va_start(args, format);

    // 计算文本中的行数
    int totalLines = 1; // 至少有一行
    for (const char *p = format; *p; p++)
    {
        if (*p == '\n')
        {
            totalLines++;
        }
    }

    // 获取可用行数
    int availableLines = dis2bottom();

    // 如果可用行数不足以显示整个文本，则控制光标
    if (totalLines > availableLines)
    {
        printf("\033[J");
    }

    // 使用 vprintf 直接打印整个格式化的字符串
    vprintf(format, args);

    va_end(args);
    return 0;
}