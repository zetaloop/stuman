/*
===========================
  仅支持最新配置、UTF-8 编码
  执意使用古代的编辑器与终端
  比如 DevCpp、CodeBlocks、
  console.exe (Win10 旧版)
  将会导致输出颜色或格式错误
===========================
推荐配置：
  系统: Windows 11+
  终端: Windows Terminal
  编辑器: VSCode, CLion
  编译器: GCC 10+
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include <locale.h>
#include <conio.h>
#include <windows.h>
#define sleep Sleep
#define NO "\033[0m"
#define ITAL "\033[3m"
#define BOLD "\033[1m"
#define UNDL "\033[4m"
#define REV "\033[7m"
#define RED "\033[31;1m"
#define BLUE "\033[34;1m"
#define GREEN "\033[32;1m"
#define LN_UP "\033[1A"
#define LN_DOWN "\033[1B"
#define LN_RIGHT "\033[1C"
#define LN_LEFT "\033[1D"
#define LN_HOME "\033[1K"
#define LN_CL "\033[0K"
#define CLS "\033[J\033[1J\033[2J\033[3J"
#define RST "\033[H"
#define HIDECUR "\033[?25l"
#define SHOWCUR "\033[?25h"
#define SAVECUR "\033[s"
#define RESTCUR "\033[u"
#define Tri "▲"
#define Revtri "▼"
#define CO_ID "学号"
#define CO_NAME "姓名"
#define CO_MATHS "高数"
#define CO_PHYSC "大物"
#define CO_CLANG "C语言"
#define CO_TOTAL "总分"
#define __ "   "
#define __x "  "
#define VERSION "v1.12"
#define MAX_NAMECHAR 100 // 姓名不应超过100字
#define MAX_NUMCHAR 32
#define MAX_LNCHAR 200

int cursor_index = 0;
int cursor_index_total = 0;
int table_force_refresh = 0;
int table_last_height = 0;
int table_last_pgnlen = 0;
int table_fast_refresh = 0;
int current_find_index = -1;
unsigned long long int current_find_id = 0;
char current_find_name[MAX_NAMECHAR] = {0};
int current_find_maths = 0;
int current_find_physc = 0;
int current_find_clang = 0;
int current_found = 0;

unsigned long long int *ID;
char (*name)[MAX_NAMECHAR];
int *maths;
int *physc;
int *clang;
int *total;
int colwidth[6];
int pageheight;
int datalen = 0;
int startln = 0;
int sortby = 1; // 123456列 +-升序降序
char path[MAX_PATH] = "新文件.csv";

#include "utils.c"
#include "printflim.c"
#include "tablemath.c"
#include "fileloader.c"

void transition(char *mode, int speed) // 转场效果, 以speed速度清除ln行
{
    int ln = dis2top() - 1;
    printf(SAVECUR RST BOLD "==== 成绩管理系统 [%s] ====\n\n " NO RESTCUR, mode);
    for (int i = 0; i < ln; i++)
    {
        if (speed != 0)
            sleep((int)(((int)(1000 + i * 1000) - 200) / 200 / speed));
        printf(LN_CL LN_HOME LN_UP);
    }
}

// int timelen = 0;
// void debugtime()
// {
//     char time[32];
//     SYSTEMTIME sys;
//     GetLocalTime(&sys);
//     snprintf(time, sizeof(time), "%2d.%03d ", sys.wSecond, sys.wMilliseconds);
//     printf(SAVECUR RST "\n");
//     for (int i = 0; i < timelen; i++)
//         printf(LN_RIGHT);
//     printf("%s" RESTCUR, time);
//     timelen += 7;
// }

void main_UI()
{
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [主页] ====\n\n" NO);
    if (cursor_index == 0)
        printf(REV);
    printf("  > 分数统计   \n" NO);
    if (cursor_index == 1)
        printf(REV);
    printf("  > 学生管理   \n" NO);
    if (cursor_index == 2)
        printf(REV);
    printf("  > 导入   \n" NO);
    if (cursor_index == 3)
        printf(REV);
    printf("  > 导出   \n" NO);
    if (cursor_index == 4)
        printf(REV);
    printf("  > 关于   \n" NO);
    if (cursor_index == 5)
        printf(REV);
    printf(RED "  > 退出   \n" NO);
    printf(BOLD "\n方向键 选择\n回车键 确定\n其他键 退出" NO);
}

void show_UI()
{
refresh:
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [统计] ====\n\n" NO);
    printf(GREEN "  > %s   \n" NO, path);

    // timelen = 0;
    // debugtime();

    if (cursor_index == 0)
        printf(REV);
    printlen(__, CO_ID, colwidth[0], sortby == 1 ? Tri __x NO : sortby == -1 ? Revtri __x NO
                                                                             : __ NO);
    if (cursor_index == 1)
        printf(REV);
    printlen(__, CO_NAME, colwidth[1], sortby == 2 ? Tri __x NO : sortby == -2 ? Revtri __x NO
                                                                               : __ NO);
    if (cursor_index == 2)
        printf(REV);
    printlen(__, CO_MATHS, colwidth[2], sortby == 3 ? Tri __x NO : sortby == -3 ? Revtri __x NO
                                                                                : __ NO);
    if (cursor_index == 3)
        printf(REV);
    printlen(__, CO_PHYSC, colwidth[3], sortby == 4 ? Tri __x NO : sortby == -4 ? Revtri __x NO
                                                                                : __ NO);
    if (cursor_index == 4)
        printf(REV);
    printlen(__, CO_CLANG, colwidth[4], sortby == 5 ? Tri __x NO : sortby == -5 ? Revtri __x NO
                                                                                : __ NO);
    if (cursor_index == 5)
        printf(REV);
    if (cursor_index == -1)
        printlen(REV RED BOLD "["__x, "返回", colwidth[5], __x "]" NO);
    else
        printlen(BLUE __, CO_TOTAL, colwidth[5], sortby == 6 ? Tri __x NO : sortby == -6 ? Revtri __x NO
                                                                                         : __ NO);
    char buf[MAX_NUMCHAR];
    int j = 0;
    int dis = dis2bottom();
    // debugtime();
    if (table_fast_refresh)
    {
        printf("\033[%dB", table_last_height + 6);
        // debugtime();
        table_fast_refresh = 0;
        goto over;
    }
    for (int i = startln; i < datalen && dis > 5; i++, j++, dis--)
    {
        printf("\n");
        printlen(__, llu2str(ID[i], buf), colwidth[0], __ NO);
        printlen(__, name[i], colwidth[1], __ NO);
        printlen(__, int2str(maths[i], buf), colwidth[2], __ NO);
        printlen(__, int2str(physc[i], buf), colwidth[3], __ NO);
        printlen(__, int2str(clang[i], buf), colwidth[4], __ NO);
        printlen(BLUE __, int2str(total[i], buf), colwidth[5], __ NO);
    }
    // debugtime();
    char pgn[MAX_NUMCHAR] = {0};
    if (datalen && pageheight > 0)
    {
        snprintf(pgn, sizeof(pgn), " %d/%d",
                 startln / pageheight + !!(startln % pageheight) + 1,
                 datalen / pageheight + !!(datalen % pageheight));
        printf(pgn);
    }
    // if (table_force_refresh)
    // {
    printfforce(BOLD "\n\n左右键 选择\n上下键 排序\n翻页键 翻页\n回车键 属性\n其他键 返回" NO);
    // printf("tableforcerefresh: %d", rand()); // Debug
    // table_force_refresh = 0;
    // goto over;
    // }
    if (table_last_height != j || table_last_pgnlen != strlen(pgn))
    {
        if (table_last_height > j)
            for (int i = j; i < table_last_height + 6; i++)
                printf("\n");
        table_force_refresh = 1;
        table_last_height = j;
        table_last_pgnlen = strlen(pgn);
        transition("统计", 0);
        goto refresh;
    }
over:
    // debugtime();
    1;
}

void manage_UI()
{
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [管理] ====\n\n" NO);
    if (cursor_index == 0)
        printf(REV);
    printf("  > 查找学号   \n" NO);
    if (cursor_index == 1)
        printf(REV);
    printf("  > 添加学生   \n" NO);
    if (cursor_index == 2)
        printf(REV);
    // printf("  > 筛选数据   \n" NO);
    // if (cursor_index == 3)
    //     printf(REV);
    printf("  > 返回   \n" NO);
    printf(BOLD "\n方向键 选择\n回车键 确定\n其他键 返回" NO);
}

void manage_find_UI()
{
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [学生] ====\n\n" NO);
    if (cursor_index == 0)
        printf(REV);
    printf("  > " CO_ID " %llu   \n" NO, current_find_id);
    if (cursor_index == 1)
        printf(REV);
    printf("  > " CO_NAME " %s   \n" NO, current_find_name);
    if (cursor_index == 2)
        printf(REV);
    printf("  > " CO_MATHS " %d   \n" NO, current_find_maths);
    if (cursor_index == 3)
        printf(REV);
    printf("  > " CO_PHYSC " %d   \n" NO, current_find_physc);
    if (cursor_index == 4)
        printf(REV);
    printf("  > " CO_CLANG " %d   \n" NO, current_find_clang);
    if (cursor_index == 5)
        printf(REV);
    printf(BLUE "\n  > 保存   \n" NO);
    if (cursor_index == 6)
        printf(REV);
    printf(RED "  > 删除   \n" NO);
    if (cursor_index == 7)
        printf(REV);
    printf("  > 返回   \n" NO);
    printf(BOLD "\n方向键 选择\n回车键 确定\n其他键 返回" NO);
}

void load_UI()
{
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [导入] ====\n\n" NO);
    if (cursor_index == 0)
        printf(REV);
    printf("  > CSV 文件   \n" NO);
    if (cursor_index == 1)
        printf(REV);
    printf("  > 生成演示数据   \n" NO);
    if (cursor_index == 2)
        printf(REV);
    printf("  > 返回   \n" NO);
    printf(BOLD "\n方向键 选择\n回车键 确定\n其他键 返回" NO);
}

void save_UI()
{
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [导出] ====\n\n" NO);
    if (cursor_index == 0)
        printf(REV);
    printf("  > CSV 文件   \n" NO);
    if (cursor_index == 1)
        printf(REV);
    printf("  > 返回   \n" NO);
    printf(BOLD "\n方向键 选择\n回车键 确定\n其他键 返回" NO);
}

void show()
{
    transition("统计", 0);
    cursor_index = 0;
    table_force_refresh = 1;
    int user = 0;
    int *detail_p;
    char buf[MAX_NAMECHAR];
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [统计] ====\n\n" NO);
    printf(REV "  > %s   \n" NO, path);
    pageheight = dis2bottom() - 5; // 减标题行减提示
    sleep(100);
loop:
    cursor_index_total = 7;
    show_UI();
    user = getkey_commonmove();
    if (user == -1) // esc
    {
    esc:
        cursor_index = -1;
        goto over;
    }
    else if (user == 0) // enter for detail
    {
        if (cursor_index == -1)
            goto esc;
        transition("属性", 0);
        printf(RST HIDECUR "\n\n");
        if (cursor_index == 0)
        {
            printf("["__x CO_ID __x "]" NO "\n");
            printflim("  > 个数   %d\n", datalen);
            printflim("  > 列宽   %d\n", colwidth[cursor_index]);
        }
        else if (cursor_index == 1)
        {
            printf("["__x CO_NAME __x "]" NO "\n");
            printflim("  > 个数   %d\n", datalen);
            printflim("  > 列宽   %d\n", colwidth[cursor_index]);
        }
        else
        {
            if (cursor_index == 2)
            {
                printf("["__x CO_MATHS __x "]" NO "\n");
                detail_p = maths;
            }
            else if (cursor_index == 3)
            {
                printf("["__x CO_PHYSC __x "]" NO "\n");
                detail_p = physc;
            }
            else if (cursor_index == 4)
            {
                printf("["__x CO_CLANG __x "]" NO "\n");
                detail_p = clang;
            }
            else if (cursor_index == 5)
            {
                printf(BLUE "[" __x CO_TOTAL __x "]" NO "\n");
                detail_p = total;
            }
            printflim("  > 个数   %d\n", datalen); // printflim 屏幕剩余空间不足时输出会截断
            printflim("  > 最大值 %s\n", _cmax(detail_p, buf));
            printflim("  > 最小值 %s\n", _cmin(detail_p, buf));
            printflim("  > 平均数 %s\n", _cmean(detail_p, buf));
            printflim("  > 中位数 %s\n", _cmedian(detail_p, buf));
            printflim("  > 众数   %s\n", _cmode(detail_p, buf));
            printflim("  > 方差   %s\n", _cvariance(detail_p, buf));
            printflim("  > 标准差 %s\n", _cstddev(detail_p, buf));
            printflim("  > 极差   %s\n", _crange(detail_p, buf));
            printflim("  > 偏度   %s\n", _cskewness(detail_p, buf));
            printflim("  > 峰度   %s\n", _ckurtosis(detail_p, buf));
            printflim("  > 列宽   %d\n", colwidth[cursor_index]);
        }
        printfforce(REV "\n  > 返回   " NO);
        getkey_any();
        transition("统计", 0);
        table_force_refresh = 1;
    }
    else if (user == 1) // ^, sort by
    {
        if (cursor_index == -1)
            goto esc;
        if (sortby != cursor_index + 1)
        {
            sortby = cursor_index + 1;
            if (sortby == 1)
                _sortbyid(0);
            else if (sortby == 2)
                _sortbyname(0);
            else
                _sortby(abs(sortby) == 3   ? maths
                        : abs(sortby) == 4 ? physc
                        : abs(sortby) == 5 ? clang
                                           : total,
                        0);
        }
    }
    else if (user == 2) // v, reverse sort by
    {
        if (cursor_index == -1)
            goto esc;
        if (sortby != -(cursor_index + 1))
        {
            sortby = -(cursor_index + 1);
            if (sortby == -1)
                _sortbyid(1);
            else if (sortby == -2)
                _sortbyname(1);
            else
                _sortby(abs(sortby) == 3   ? maths
                        : abs(sortby) == 4 ? physc
                        : abs(sortby) == 5 ? clang
                                           : total,
                        1);
        }
    }
    else if (user == 3) // <
    {
        cursor_index = (cursor_index + 1 + cursor_index_total - 1) % cursor_index_total - 1;
        table_fast_refresh = 1;
    }
    else if (user == 4) // >
    {
        cursor_index = (cursor_index + 1 + 1) % cursor_index_total - 1;
        table_fast_refresh = 1;
    }
    else if (user == 5) // pgup
    {
        if (startln - pageheight > 0)
            startln = startln - pageheight;
        else if (startln > 0)
            startln = 0;
        else
            goto loop;
    }
    else if (user == 6) // pgdn
    {
        if (startln + pageheight < datalen)
            startln = startln + pageheight;
        else
            goto loop;
    }
    goto loop;
over:
    transition("统计", 0);
    printf(RST HIDECUR);
    printf(BOLD "==== 成绩管理系统 [统计] ====\n\n" NO);
    printf(REV "  > %s   \n" NO, path);
    sleep(100);
    cursor_index = 0;
    transition("主页", 0);
}

void manage_find(int new); // 前置声明

void manage()
{
    transition("管理", 0);
    cursor_index = 0;
    int user = 0;
loop:
    cursor_index_total = 3;
    manage_UI();
    user = getkey_commonmove();
    if (user == -1) // esc
    {
    esc:
        goto over;
    }
    else if (user == 0) // enter
    {
    enter:
        switch (cursor_index)
        {
        case 0:
            manage_find(0);
            break;
        case 1:
            manage_find(1);
            break;
        case 2:
            goto esc;
        }
    }
    else if (user == 1) // ^
    {
        cursor_index = (cursor_index + cursor_index_total - 1) % cursor_index_total;
    }
    else if (user == 2) // v
    {
        cursor_index = (cursor_index + 1) % cursor_index_total;
    }
    else if (user == 3) // <
    {
        goto esc;
    }
    else if (user == 4) // >
    {
        goto enter;
    }
    else if (user == 5) // pgup
    {
        cursor_index = 0;
    }
    else if (user == 6) // pgdn
    {
        cursor_index = cursor_index_total - 1;
    }
    goto loop;
over:
    cursor_index = 1;
    transition("主页", 0);
};

void manage_find(int new)
{
    unsigned long long int id = 0;
    int index = 0;
    int user = 0;
    char tmp[MAX_NUMCHAR] = {0};
    if (new)
    {
        cursor_index = 0;
        current_found = 0;
        current_find_id = id = 0;
        strcpy(current_find_name, "(新学生)");
        current_find_maths = 0;
        current_find_physc = 0;
        current_find_clang = 0;
        printf(RST "\n\n\n\n\n\n\n\n");
        transition("学生", 0);
    }
    else
    {
        cursor_index = 7;
        printf(RST REV "\n\n  > 查找学号   " NO SHOWCUR " 学号: ");
        gets(tmp);
        if (sscanf(tmp, "%llu", &id) != 1)
            goto over;
        printf(NO HIDECUR "\n\n\n\n\n\n");
        transition("学生", 0);
        current_found = 0;
        current_find_id = id;
        strcpy(current_find_name, "(未找到该同学)");
        current_find_maths = 0;
        current_find_physc = 0;
        current_find_clang = 0;
        for (int i = 0; i < datalen; i++)
        {
            if (ID[i] == id)
            {
                index = i;
                current_found = 1;
                current_find_index = i;
                strcpy(current_find_name, name[i]);
                current_find_maths = maths[i];
                current_find_physc = physc[i];
                current_find_clang = clang[i];
                break;
            }
        }
    }
loop:
    cursor_index_total = 8;
    manage_find_UI();
    user = getkey_commonmove();
    if (user == -1) // esc
    {
    esc:
        goto over;
    }
    else if (user == 0) // enter
    {
    enter:
        switch (cursor_index)
        {
        case 0:
            printf(RST REV "\n\n  > " CO_ID " %llu   " NO " 改为: ", current_find_id);
            gets(tmp);
            if (sscanf(tmp, "%llu", &id) == 1)
                current_find_id = id;
            printf(NO RST HIDECUR "\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            transition("学生", 0);
            break;
        case 1:
            printf(RST REV "\n\n\n  > " CO_NAME " %s   " NO " 改为: ", current_find_name);
            getx(current_find_name, MAX_NAMECHAR);
            printf(NO RST HIDECUR "\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            transition("学生", 0);
            break;
        case 2:
            printf(RST REV "\n\n\n\n  > " CO_MATHS " %d   " NO " 改为: ", current_find_maths);
            gets(tmp);
            if (sscanf(tmp, "%d", &id) == 1)
                current_find_maths = id;
            printf(NO RST HIDECUR "\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            transition("学生", 0);
            break;
        case 3:
            printf(RST REV "\n\n\n\n\n  > " CO_PHYSC " %d   " NO " 改为: ", current_find_physc);
            gets(tmp);
            if (sscanf(tmp, "%d", &id) == 1)
                current_find_physc = id;
            printf(NO RST HIDECUR "\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            transition("学生", 0);
            break;
        case 4:
            printf(RST REV "\n\n\n\n\n\n  > " CO_CLANG " %d   " NO " 改为: ", current_find_clang);
            gets(tmp);
            if (sscanf(tmp, "%d", &id) == 1)
                current_find_clang = id;
            printf(NO RST HIDECUR "\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
            transition("学生", 0);
            break;
        case 5: // save
            if (current_found)
            {
                // del then add then sort
                for (int i = id; i < datalen - 1; i++)
                {
                    ID[i] = ID[i + 1];
                    strcpy(name[i], name[i + 1]);
                    maths[i] = maths[i + 1];
                    physc[i] = physc[i + 1];
                    clang[i] = clang[i + 1];
                    total[i] = total[i + 1];
                }
                ID[datalen - 1] = current_find_id;
                strcpy(name[datalen - 1], current_find_name);
                maths[datalen - 1] = current_find_maths;
                physc[datalen - 1] = current_find_physc;
                clang[datalen - 1] = current_find_clang;
                total[datalen - 1] = current_find_maths + current_find_physc + current_find_clang;
                _sortbyid(0);
            }
            else
            {
                // expand table and add to end then sort
                realloctable(datalen + 1);
                ID[datalen - 1] = current_find_id;
                strcpy(name[datalen - 1], current_find_name);
                maths[datalen - 1] = current_find_maths;
                physc[datalen - 1] = current_find_physc;
                clang[datalen - 1] = current_find_clang;
                total[datalen - 1] = current_find_maths + current_find_physc + current_find_clang;
                _sortbyid(0);
            }
            // 针对新的数据尝试扩展列表宽度colwidth
            if (strvlen(llu2str(current_find_id, tmp)) > colwidth[0])
                colwidth[0] = strvlen(llu2str(current_find_id, tmp));
            if (strvlen(current_find_name) > colwidth[1])
                colwidth[1] = strvlen(current_find_name);
            if (strvlen(int2str(current_find_maths, tmp)) > colwidth[2])
                colwidth[2] = strvlen(int2str(current_find_maths, tmp));
            if (strvlen(int2str(current_find_physc, tmp)) > colwidth[3])
                colwidth[3] = strvlen(int2str(current_find_physc, tmp));
            if (strvlen(int2str(current_find_clang, tmp)) > colwidth[4])
                colwidth[4] = strvlen(int2str(current_find_clang, tmp));
            if (strvlen(int2str(total[datalen - 1], tmp)) > colwidth[5])
                colwidth[5] = strvlen(int2str(total[datalen - 1], tmp));
            goto esc;
        case 6: // del
            if (current_found)
            {
                for (int i = index; i < datalen - 1; i++)
                {
                    ID[i] = ID[i + 1];
                    strcpy(name[i], name[i + 1]);
                    maths[i] = maths[i + 1];
                    physc[i] = physc[i + 1];
                    clang[i] = clang[i + 1];
                    total[i] = total[i + 1];
                }
                datalen--;
            }
            goto esc;
        case 7: // return
            goto esc;
        }
    }
    else if (user == 1) // ^
    {
        cursor_index = (cursor_index + cursor_index_total - 1) % cursor_index_total;
    }
    else if (user == 2) // v
    {
        cursor_index = (cursor_index + 1) % cursor_index_total;
    }
    else if (user == 3) // <
    {
        goto esc;
    }
    else if (user == 4) // >
    {
        goto enter;
    }
    else if (user == 5) // pgup
    {
        cursor_index = 0;
    }
    else if (user == 6) // pgdn
    {
        cursor_index = cursor_index_total - 1;
    }
    goto loop;
over:
    cursor_index = 0;
    transition("主页", 0);
};

void load()
{
    transition("导入", 0);
    cursor_index = 0;
    int user = 0;
loop:
    cursor_index_total = 3;
    load_UI();
    user = getkey_commonmove();
    if (user == -1) // esc
    {
    esc:
        goto over;
    }
    else if (user == 0) // enter
    {
    enter:
        switch (cursor_index)
        {
        case 0:
            if (load_csv())
                goto esc;
            break;
        case 1:
            if (load_demo())
                goto esc;
            break;
        case 2:
            goto esc;
        }
    }
    else if (user == 1) // ^
    {
        cursor_index = (cursor_index + cursor_index_total - 1) % cursor_index_total;
    }
    else if (user == 2) // v
    {
        cursor_index = (cursor_index + 1) % cursor_index_total;
    }
    else if (user == 3) // <
    {
        goto esc;
    }
    else if (user == 4) // >
    {
        goto enter;
    }
    else if (user == 5) // pgup
    {
        cursor_index = 0;
    }
    else if (user == 6) // pgdn
    {
        cursor_index = cursor_index_total - 1;
    }
    goto loop;
over:
    cursor_index = 2;
    transition("主页", 0);
}

void save()
{
    transition("导出", 0);
    cursor_index = 0;
    int user = 0;
loop:
    cursor_index_total = 2;
    save_UI();
    user = getkey_commonmove();
    if (user == -1) // esc
    {
    esc:
        goto over;
    }
    else if (user == 0) // enter
    {
    enter:
        switch (cursor_index)
        {
        case 0:
            if (save_csv())
                goto esc;
            break;
        case 1:
            goto esc;
        }
    }
    else if (user == 1) // ^
    {
        cursor_index = (cursor_index + cursor_index_total - 1) % cursor_index_total;
    }
    else if (user == 2) // v
    {
        cursor_index = (cursor_index + 1) % cursor_index_total;
    }
    else if (user == 3) // <
    {
        goto esc;
    }
    else if (user == 4) // >
    {
        goto enter;
    }
    else if (user == 5) // pgup
    {
        cursor_index = 0;
    }
    else if (user == 6) // pgdn
    {
        cursor_index = cursor_index_total - 1;
    }
    goto loop;
over:
    cursor_index = 3;
    transition("主页", 0);
}

void about()
{
    transition("关于", 0);
    printf(RST "\n\n");
    char x[] =
        BOLD "   成绩管理系统 " VERSION "\n" NO
             "   By zetaloop\n\n[";
    char y[] =
        "github.com/zetaloop/stuman";
    for (int i = 0; i < sizeof(x); i++)
    {
        printf("%c", x[i]);
        sleep(i % 2);
    }
    for (int i = 0; i < sizeof(y); i++)
    {
        printf(BLUE UNDL "%c" NO "]" LN_LEFT, y[i]);
        sleep(i % 2);
    }
    printf(NO "]");
    printf(REV "\n\n  > 返回   " NO);
    getkey_any();
    transition("主页", 0);
}

int main() // 主UI循环
{
    // sleep(999999); // Debug
    // setlocale(LC_ALL, "zh_CN.UTF-8");
    SetConsoleOutputCP(65001); // 输入输出改为 UTF-8 编码
    srand((unsigned)time(NULL));
    colwidth[0] = strvlen(CO_ID); // 初始化列宽
    colwidth[1] = strvlen(CO_NAME);
    colwidth[2] = strvlen(CO_MATHS);
    colwidth[3] = strvlen(CO_PHYSC);
    colwidth[4] = strvlen(CO_CLANG);
    colwidth[5] = strvlen(CO_TOTAL);
    ID = (unsigned long long int *)malloc(datalen * sizeof(unsigned long long int));
    name = (char(*)[MAX_NAMECHAR])malloc(datalen * MAX_NAMECHAR);
    maths = (int *)malloc(datalen * sizeof(int));
    physc = (int *)malloc(datalen * sizeof(int));
    clang = (int *)malloc(datalen * sizeof(int));
    total = (int *)malloc(datalen * sizeof(int));
    printf(CLS RST);
    int user = 0;
loop:
    cursor_index_total = 6;
    main_UI();
    user = getkey_commonmove();
    if (user == -1) // esc
    {
    esc:
        cursor_index = 5;
        main_UI();
        transition(RED "退出" NO BOLD, 1);
        printf(RST "\n\n " REV);
        char x[] = "  系统已关闭 欢迎下次使用  ";
        for (int i = 0; i < sizeof(x); i++)
        {
            printf("%c", x[i]);
            sleep(i % 2);
        }
        sleep(500);
        printf("\n" NO);
        exit(0);
    }
    else if (user == 0) // enter
    {
    enter:
        switch (cursor_index)
        {
        case 0:
            show();
            break;
        case 1:
            manage();
            break;
        case 2:
            load();
            break;
        case 3:
            save();
            break;
        case 4:
            about();
            break;
        case 5:
            goto esc;
        }
    }
    else if (user == 1) // ^
        cursor_index = (cursor_index + cursor_index_total - 1) % cursor_index_total;
    else if (user == 2) // v
        cursor_index = (cursor_index + 1) % cursor_index_total;
    else if (user == 3) // <
        goto esc;
    else if (user == 4) // >
        goto enter;
    else if (user == 5) // pgup
        cursor_index = 0;
    else if (user == 6) // pgdn
        cursor_index = cursor_index_total - 1;
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

/*（排序方式跟随分数统计表）
分数统计（输出表格、排序）
学生管理（增删改查）
导入
导出
关于
退出


*/
