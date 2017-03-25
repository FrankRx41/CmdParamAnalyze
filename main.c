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
#define MAXSWITCHPARAM  3
#define MAXOTHERPARAM   2
struct{
    char * switchparamshort[MAXSWITCHPARAM];
    char * switchparamlong[MAXSWITCHPARAM];
    int option[MAXSWITCHPARAM];
    char * otherparamshort[MAXOTHERPARAM];
    char * otherparamslong[MAXOTHERPARAM];
    int otheroption[MAXOTHERPARAM];
}option = {
"q"     ,"h"    ,"create",
"quite" ,"help" ,"create",
0       ,0      ,0      ,
//其他參數
"of"    ,"w"    ,
"offset","width",
0       ,0      ,
};
#define GetSwitchParamtesLong(x)    (option.switchparamlong[x])
#define GetSwitchParamtesShort(x)   (option.switchparamshort[x])
#define GetOtherParamtesLong(x)     (option.otherparamslong[x])
#define GetOtherParamtesShort(x)    (option.otherparamshort[x])
#define SetOption(x)            option.option[x]=1
#define SetOtherOption(num,x)   option.otheroption[x] = num
#define GetSwitchParamMaxNum()  MAXSWITCHPARAM
#define GetOtherParamMaxNum()   MAXOTHERPARAM

#define SWITCH_OPTION_QUITE     0
#define SWITCH_OPTION_HELP      1
#define SWITCH_OPTION_CREATE    2
#define IsOption(x)             (option.option[x]==1)

#define OTHER_OPTION_OFFSET     0
#define OTHER_OPTION_WIDTH      1
#define GetOption(x)            (option.otheroption[x])

/*******************************************************************************
*******************************************************************************/

//本宏定義可以放置到自己的標準庫中
#define StringCompare(s1,s2)    ((*(s1))==(*(s2))&&strcmp((s1),(s2))==0)
int StringCompareHead(char * s1,char * s2,int len)
{
    for(int i=0;i<len;i++)
    {
        if(s1[i]!=s2[i])return 0;
    }
    return 1;
}
//-1表示失敗
int GetIntegerFromStr(char *str)
{
    int len = strlen(str);
    int sum = 0;
    for(int i=0;i<len;i++){
        if(!(str[i]>='0' && str[i]<='9'))return -1;
        sum*=10;
        sum+=str[i]-'0';
    }
    return sum;
}

//該函數返回所需要設置的值
//-1目前保留
//-2錯誤
int AnalysetheParamete(char *param)
{
    printf("[%-12s]\t",param);
    if(param[0]=='-' || param[0]=='/')
    {
        for(int i=0;i<GetSwitchParamMaxNum();i++)
        {
            printf("[%s][%s][%s]\t",param+1,GetSwitchParamtesLong(i),GetSwitchParamtesShort(i));
            if(StringCompare(param+1,GetSwitchParamtesLong(i)) ||
               StringCompare(param+1,GetSwitchParamtesShort(i)))
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
        //printf("param is no head with '-' and '/'\n");
        //param is no head with '-' and '/'
        //支持新的參數類型，比如 offset=10 之類的設置
        for(int i=0;i<GetOtherParamMaxNum();i++){
            int len;
            len = strlen(GetOtherParamtesLong(i));
            if(StringCompareHead(param,GetOtherParamtesLong(i),len)){
                if(*(param+len)!='=')
                {
                    printf("'=' error\n");   
                    return -2;
                }
                if(GetOption(i)!=0)
                {
                    printf("had set yet2\n");   
                    return -2;
                }
                int num = GetIntegerFromStr(param+len+1);
                SetOtherOption(num,i);
                printf("(%d) successful!!\n",num);
                return -1;
            }
            len = strlen(GetOtherParamtesShort(i));
            if(StringCompareHead(param,GetOtherParamtesShort(i),len))
            {
                if(*(param+len)!='=')
                {
                    printf("'=' error\n");
                    return -2;
                }
                if(GetOption(i)!=0)
                {
                    printf("had set yet2\n");
                    return -2;
                }
                int num = GetIntegerFromStr(param+len+1);
                SetOtherOption(num,i);
                printf("(%d) successful!!\n",num);
                return -1;
            }
        }
        //unknow param
        printf("false\n");
        return -2;
    }
}

//注意：不要將第一個參數，即程序名傳入該函數
//e.g.AnalyseParametes(argc-1,argv+1)
void AnalyseParametes(int argnum,char **arglist)
{
    for(int i=0;i<argnum;i++)
    {
        int set = AnalysetheParamete(arglist[i]);
        if(set==-2 || set==-1)
        {
            continue;
        }
        if(!IsOption(set))
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
    if(IsOption(SWITCH_OPTION_QUITE)){
        printf("OPTION_QUITE\n");
    }
    if(IsOption(SWITCH_OPTION_HELP)){
        printf("OPTION_HELP\n");
    }
    if(IsOption(SWITCH_OPTION_CREATE)){
        printf("OPTION_CREATE\n");
    }
}

void CheckAnalyseParametesEnter()
{
    char *str[] = {"123abcABC,","haha-=[]\"","987","123","8","-q","-quite","-create","/q"," q","of=123","width=099","offset=2"};
    AnalyseParametes(13,str);
}

void CheckStringCompareBegin(char *str1,char *str2)
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
    char str1[MAX][256] = {"123abcABC,","haha-=[]\"","987","123","8","-q","-quite","-create","/q"," q"};
    char str2[MAX][256] = {"123abcABC,","haha-=[]\"","887","1234","98"};
    for(int i=0;i<MAX;i++)
    {
        CheckStringCompareBegin(str1[i],str2[i]);
    }
}

void CheckStringCompareHeadBegin(char *str1,char *str2,int len){
    printf("[%-10s] [%-10s] (%2d)\t",str1,str2,len);
    if(StringCompareHead(str1,str2,len))
    {
        printf("pass\n");
    }
    else
    {
        printf("false\n");
    }
}
void CheckStringCompareHeadEnter(){
    #define MAX 10
    char str1[MAX][256] = {"123abcABC,","haha-=[]\"","889","123","8","-q","-quite","-create","/q"," q"};
    char str2[MAX][256] = {"123abcABC,","haha-=[]\"","887","1234","98","","-q","__A_B"};
    int     len[MAX]    = {9           ,9           ,2    ,3    ,1  ,1   ,2   ,5};
    for(int i=0;i<MAX;i++)
    {
        CheckStringCompareHeadBegin(str1[i],str2[i],len[i]);
    }
}

void CheckGetIntegerFromStrBegin(char *str)
{
    printf("[%-10s]\t",str);
    int sum = GetIntegerFromStr(str);
    if(sum!=-1)
    {
        printf("%d\n",sum);
    }
    else
    {
        printf("false\n");
    }
}
void CheckGetIntegerFromStrEnter()
{
    #define MAX 10
    char str[MAX][256] = {"123abcABC,","haha-=[]\"","889","123","8","-q","-quite","-create","/q"," q"};
    for(int i=0;i<MAX;i++)
    {
        CheckGetIntegerFromStrBegin(str[i]);
    }
}

int main(int argc,char **argv)
{
    //CheckStringCompareEnter();
    CheckAnalyseParametesEnter();
    //CheckStringCompareHeadEnter();
    //CheckGetIntegerFromStrEnter();
    AnalyseParametes(argc-1,argv+1);
    CheckOption();
    for(int i=0;i<GetSwitchParamMaxNum();i++){
        printf("[%d]",GetOption(i));
    }
    return 0;
}