// #include <windows.h>
// #include <stdio.h>
// #include <string.h>

int gbk2utf8(char *inGbk, char *outUtf8, int outSize)
{
    // 首先将 GBK 转换为宽字符
    int wcsLen = MultiByteToWideChar(936, 0, inGbk, -1, NULL, 0);
    wchar_t *wcs = (wchar_t *)malloc(wcsLen * sizeof(wchar_t));
    MultiByteToWideChar(936, 0, inGbk, -1, wcs, wcsLen);

    // 然后将宽字符转换为 UTF-8
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wcs, -1, outUtf8, outSize, NULL, NULL);
    free(wcs);
    return utf8Len; // 返回转换后的字符数
}

int getxxx(const char *cmd, char *path)
{
    SetConsoleOutputCP(936); // 只有 GBK 编码下默认对话框模板才显示中文

    SECURITY_ATTRIBUTES sa;
    HANDLE hRead, hWrite;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&hRead, &hWrite, &sa, 0))
    {
        return 0;
    }

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;

    // 转换 char 类型到宽字符字符串
    int cmdSize = MultiByteToWideChar(CP_UTF8, 0, cmd, -1, NULL, 0);
    wchar_t *wCmd = (wchar_t *)malloc(cmdSize * sizeof(wchar_t));
    MultiByteToWideChar(CP_UTF8, 0, cmd, -1, wCmd, cmdSize);

    if (!CreateProcessW(NULL, wCmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    {
        free(wCmd);
        CloseHandle(hWrite);
        CloseHandle(hRead);
        return 0;
    }

    // if (!CreateProcess(NULL, (LPSTR)cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
    // {
    //     free(wCmd);
    //     CloseHandle(hWrite);
    //     CloseHandle(hRead);
    //     return 0;
    // }

    free(wCmd);
    memset(path, 0, MAX_PATH);
    CloseHandle(hWrite);

    // 读取命令行输出（读取不需要转换）
    DWORD read;
    BOOL success = ReadFile(hRead, path, MAX_PATH, &read, NULL);
    path[read] = '\0'; // 确保字符串结束符
    CloseHandle(hRead);

    // // 读取命令行输出到宽字符缓冲区
    // wchar_t wPath[MAX_PATH];
    // DWORD read;
    // BOOL success = ReadFile(hRead, wPath, MAX_PATH, &read, NULL);
    // wPath[read] = L'\0'; // 确保宽字符字符串结束符
    // CloseHandle(hRead);

    // // 转换宽字符字符串到 char 类型
    // int pathSize = WideCharToMultiByte(CP_UTF8, 0, wPath, -1, NULL, 0, NULL, NULL);
    // WideCharToMultiByte(CP_UTF8, 0, wPath, -1, path, pathSize, NULL, NULL);

    SetConsoleOutputCP(65001);

    gbk2utf8(path, path, MAX_PATH); // 输出是 GBK 编码的，转为 UTF-8

    // 去除尾部空格
    int len = strlen(path);
    for (int i = len - 1; i >= 0; i--)
    {
        if (path[i] == '\n' || path[i] == '\r')
        {
            path[i] = '\0';
        }
        else
        {
            break;
        }
    }

    if (!success || read == 0 || strcmp(path, "") == 0)
    {
        return 0; // 用户取消选择或读取失败
    }

    // 等待进程结束
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 1;
}

int savedlg(char *path) // 传入的 path 应为默认路径，为空则不指定
{
    char initialDir[MAX_PATH] = {0};
    char fileName[MAX_PATH] = {0};
    char tmppath[MAX_PATH] = {0};

    // 查找路径中最后一个斜杠
    char *lastSlash = strrchr(path, '\\');
    if (lastSlash != NULL)
    {
        // 如果找到斜杠，分割目录和文件名
        strncpy(initialDir, path, lastSlash - path);
        initialDir[lastSlash - path] = '\0'; // 确保字符串以空字符结束
        strncpy(fileName, lastSlash + 1, sizeof(fileName));
        fileName[sizeof(fileName) - 1] = '\0';
    }
    else
    {
        // 如果没有斜杠，整个路径都是文件名
        strncpy(fileName, path, sizeof(fileName));
        fileName[sizeof(fileName) - 1] = '\0';
        // initialDir 已经初始化为 0，所以它将保持为空
    }

    char cmd[1024];
    snprintf(cmd, sizeof(cmd),
             "powershell.exe -Command \""
             "Add-Type -AssemblyName System.windows.forms; "
             "$dialog = New-Object System.Windows.Forms.SaveFileDialog; "
             "$dialog.Filter = 'UTF-8 CSV (*.csv)|*.csv|(*.*)|*.*'; "
             "$dialog.InitialDirectory = '%s'; "
             "$dialog.FileName = '%s'; "
             "if($dialog.ShowDialog() -eq [System.Windows.Forms.DialogResult]::OK){$dialog.FileName}else{''}\"",
             initialDir, fileName);
    int result = getxxx(cmd, tmppath);
    if (result)
    {
        strcpy(path, tmppath);
    }
    return result;
}

int opendlg(char *path)
{
    const char *cmd = "powershell.exe -Command \""
                      "Add-Type -AssemblyName System.windows.forms; "
                      "$dialog = New-Object System.Windows.Forms.OpenFileDialog; "
                      "$dialog.Filter = 'UTF-8 CSV (*.csv)|*.csv|(*.*)|*.*'; "
                      "$dialog.ShowDialog() | Out-Null; "
                      "$dialog.FileName\"";
    return getxxx(cmd, path);
}

// int main()
// {
//     char path[MAX_PATH] = "D:\\Script\\c语言课程设计\\xxx.csv";
//     if (savedlg(path))
//     {
//         printf("Selected File: %s\n", path);
//     }
//     else
//     {
//         printf("No file was selected or an error occurred.\n");
//     }
//     return 0;
// }
