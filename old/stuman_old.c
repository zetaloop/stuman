#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#define NO "\033[0m"
#define ITAL "\033[3m"
#define BOLD "\033[1m"
#define REV "\033[7m"
#define RED "\033[31m"
#define LN_UP "\033[1A"
#define LN_DOWN "\033[1B"
#define LN_HOME "\033[1K"
#define LN_CL "\033[0K"
#define CLS "\033[2J"
#define RST "\033[H"
#define HIDECUR "\033[?25l"
#define SHOWCUR "\033[?25h"
#define SAVECUR "\033[s"
#define RSTOCUR "\033[u"

int cursor_index = 0;

int getkey()
{
    int ch;
    ch = _getch();
    if (ch == 0 || ch == 224) // 方向键的特殊序列
    {
        ch = _getch(); // 读取方向键的实际键码
        switch (ch)
        {
        case 72:
            return 1;
        case 80:
            return 2;
        case 75:
            return 3;
        case 77:
            return 4;
        default:
            return -1;
        }
    }
    else if (ch == 13) // Enter键的键码
    {
        return 0;
    }
    return -1;
}

void bye()
{
    cursor_index = 7;
    main_UI();
    printf(SAVECUR RST BOLD "==== 成绩管理系统 [" RED "退出" NO BOLD "] ====\n\n " NO RSTRCUR);
    for (int i = 0; i < 12; i++)
    {
        Sleep((int)(((int)(1000 + i * 1000) - 200) / 200));
        printf(LN_CL LN_HOME LN_UP);
    }
    printf(RST "\n\n " REV);
    char x[] = "  系统已关闭 欢迎下次使用  ";
    for (int i = 0; i < sizeof(x); i++)
    {
        printf("%c", x[i]);
        Sleep(1);
    }
    Sleep(500);
    printf("\n" NO);
    exit(0);
}

void main_UI()
{
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [主页] ====\n\n" NO);
    if (cursor_index == 0)
    {
        printf(REV);
    }
    printf("  > 输入学生信息   \n" NO);
    if (cursor_index == 1)
    {
        printf(REV);
    }
    printf("  > 显示学生信息   \n" NO);
    if (cursor_index == 2)
    {
        printf(REV);
    }
    printf("  > 按学号查找学生信息   \n" NO);
    if (cursor_index == 3)
    {
        printf(REV);
    }
    printf("  > 插入一个学生信息   \n" NO);
    if (cursor_index == 4)
    {
        printf(REV);
    }
    printf("  > 删除一个学生信息   \n" NO);
    if (cursor_index == 5)
    {
        printf(REV);
    }
    printf("  > 按总分降序输出   \n" NO);
    if (cursor_index == 6)
    {
        printf(REV);
    }
    printf("  > 输出学生信息   \n" NO);
    if (cursor_index == 7)
    {
        printf(REV);
    }
    printf("  > 退出   \n" NO);
    printf(BOLD "\n方向键 选择\n回车键 确定\n其他键 退出" NO);
}

int enter()
{
    switch (cursor_index)
    {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        break;
    case 7:
        bye();
        break;
    }
}

int main() // 主UI循环
{
    system("chcp 65001"); // 输出改为 UTF-8 编码，如果您的编译器太老旧、还在用旧编码的话，请把这行去掉
    printf(CLS RST);
    int user = 0;
loop:
    main_UI();
    user = getkey();
    if (user == -1)
        bye();
    else if (user == 0)
        enter();
    else if (user == 1)
        cursor_index = (cursor_index + 7) % 8;
    else if (user == 2)
        cursor_index = (cursor_index + 1) % 8;
    else if (user == 3)
        bye();
    else if (user == 4)
        enter();
    goto loop;
}

/*
输入学生信息：添加学生
显示学生信息：显示学生
按学号查找学生信息：查找学生
插入一个学生信息：添加学生
删除一个学生信息：查找学生，删除学生
按总分降序输出：按总分降序，显示学生
退出
*/

/*（始终按学号排序）
数据查看（输出表格、排序）
数据导入
学生管理（增删改查）
关于
退出


*/