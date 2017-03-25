/***
*Author:    FrankRx
*Readme:
這是一個簡單的解析命令行參數的模板，一種參數解析的方法
*******************************************************************************/
#include <stdio.h>
#include <windows.h>
/*******************************************************************************
在這裡設置各種不同的參數
這裡設不設置結構體都不重要
結構體只是為了以後更好的擴展

把這文件放到.h中，將下面的宏定義改為函數，即可做到封裝
shortparam表示短參數 longparam表示長參數 ｢他們對應同一個設置｣
每個參數對應一個int，每個參數可以擁有一個簡寫
*對於特殊的情況，如 -? -h -help 這樣子的非設置類參數，
*不建議在這裡進行設置，而是應該在檢測出來的第一刻就進行處理

增加新的參數時，請記住更改MAXPARAM的值
如果新的參數沒有簡寫，請設置簡寫和全寫相同
*******************************************************************************/
#define MAXPARAM  3
struct{
    char * shortparam[MAXPARAM];
    char * longparam[MAXPARAM];
    int option[MAXPARAM];
}option = {
"q"     ,"h"    ,"create",
"quite" ,"help" ,"create",
0       ,0      ,0      ,
};
#define GetLongParamtesList(x)  option.longparam[x]
#define GetShortParamtesList(x) option.shortparam[x]
#define GetOption(x)            option.option[x]
#define SetOption(x)            option.option[x]=1
#define GetParamMaxNum()        MAXPARAM

#define OPTION_QUITE    0
#define OPTION_HELP     1
#define OPTION_CREATE   2
#define IsOption(x)     (option.option[x]==1)
/*******************************************************************************
*******************************************************************************/

//本宏定義可以放置到自己的標準庫中
#define StringCompare(s1,s2)    ((*(s1))==(*(s2))&&strcmp((s1),(s2))==0)

//該函數返回所需要設置的值
//-1目前保留
//-2錯誤
int AnalysetheParamete(char *param)
{
    printf("[%-12s]\t",param);
    if(param[0]=='-' || param[0]=='/')
    {
        for(int i=0;i<GetParamMaxNum();i++)
        {
            printf("[%s][%s][%s]\t",param+1,GetLongParamtesList(i),GetShortParamtesList(i));
            if(StringCompare(param+1,GetLongParamtesList(i)) ||
               StringCompare(param+1,GetShortParamtesList(i)))
            {
                return i;
            }
        }
        //unknow param
        printf("false\n");
        return -2;
    }
    else
    {
        printf("param is no head with '-' and '/'\n");
        //param is no head with '-' and '/'
        //TODO:支持新的參數類型，比如 offset=10 之類的設置
        //由於要對目前的數據結構進行大幅度的更改，暫時先不考慮
        return -1;
    }
}

//注意：不要將第一個參數，即程序名傳入該函數
//e.g.AnalyseParametes(argc-1,argv+1)
void AnalyseParametes(int argnum,char **arglist)
{
    for(int i=0;i<argnum;i++)
    {
        int set = AnalysetheParamete(arglist[i]);
        if(set<-2)
        {
            continue;
        }
        if(!GetOption(set))
        {
            SetOption(set);
            printf("setsuccessful\n");
        }
        else
        {
            printf("have set yet\n");
        }
    }
}

void CheckOption()
{
    if(IsOption(OPTION_QUITE)){
        printf("OPTION_QUITE\n");
    }
    if(IsOption(OPTION_HELP)){
        printf("OPTION_HELP\n");
    }
    if(IsOption(OPTION_CREATE)){
        printf("OPTION_CREATE\n");
    }
}

void CheckAnalyseParametesEntry()
{
    char *str[] ={"123abcABC,","haha-=[]\"","987","123","8","-q","-quite","-create","/q"," q"};
    AnalyseParametes(10,str);
}

int CheckStringCompareBegin(char *str1,char *str2)
{
    printf("[%-10s] [%-10s]\t",str1,str2);
    if(StringCompare(str1,str2))
    {
        printf("pass\n");
    }
    else
    {
        printf("false\n");
    }
}
void CheckStringCompareEnter()
{
    #define MAX 10
    char str1[MAX][256] ={"123abcABC,","haha-=[]\"","987","123","8","-q","-quite","-create","/q"," q"};
    char str2[MAX][256] ={"123abcABC,","haha-=[]\"","887","1234","98"};
    for(int i=0;i<MAX;i++)
    {
        CheckStringCompareBegin(str1[i],str2[i]);
    }
}

int main(int argc,char **argv)
{
    //CheckStringCompareEnter();
    CheckAnalyseParametesEntry();
    AnalyseParametes(argc-1,argv+1);
    CheckOption();
    for(int i=0;i<GetParamMaxNum();i++){
        printf("[%d]",GetOption(i));
    }
    return 0;
}