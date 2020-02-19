//TODO:parser:
/*
"{red}str..%d.."
rule:
若读入'{' 弹出栈中所有元素解析并输出
若读入'\' 则检查是否为'\{'或'\}' 若是则不触发栈操作
若读入'\0' 则弹出栈中所有元素解析并输出并结束
PS:在没有读入'}'时不能弹出'{'
若解析'{' 一直读入字符至parser_color中 直到读入'}'
若解析'}' 则直接将后方所有元素抛入输出中
若解析到'%d/%c/%s/%f/...',则将参数以相同参数输出
若解析到'\}'或'\{' 则先替换为{/}再抛入输出中
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#define STDCAL(type) type __cdecl
/*************************************************************/
/*************************************************************/

typedef struct error
{
    char* error_type;
    char* error_msg;
}error;
typedef enum rainbow_color
{
    clear,
    white,
    red,
    green,
    blue,
    yellow,
}rColor;
#define RAINBOW_PARSER const char*

typedef struct ranbowcontext
{
    RAINBOW_PARSER context;//表示当前指向的字符
    char* stack;
    size_t size,top;
}rContext;
typedef struct rainbowItem
{
    char* output;
    rColor color;
}rItem;
static STDCAL(void*) rainbow_stack_push(rContext* parser,size_t size);
static STDCAL(void*) rainbow_stack_pop(rContext* parser,size_t size);
static STDCAL(void) rainbow_set_output(rContext* parser,rItem* item,char* stack,size_t size);
static STDCAL(rColor) rainbow_parser_color(char* color);
static STDCAL(void) rainbow_parse_stack(rContext* parser,rItem* item);
static STDCAL(void) rainbow_parse(const char* text,va_list arg);
static STDCAL(void) rainbow_count_token(rItem* item);
static STDCAL(void) rainbow_output(rItem* item,va_list arg);
static STDCAL(void) color_control(rColor color);
/********************************************************/
error color_end_token_error = {"color_parse_error","please check '}' at the end of string"};
error Invild_color_error = {"Invild color value","please check your color value"};
#define RAISE(ERROR) do{printf("ERROR:\n[%s]:%s\n",ERROR.error_type,ERROR.error_msg);system("pause");system("exit");}while(0)
/********************************************************/
static STDCAL(void) rainbow_context_init(rContext* parser,const char* text)
{
    parser->context = text;
    parser->stack = NULL;
    parser->size = 0;
    parser->top = 0;
}
/********************************************************/
#ifndef INIT_STACK_SIZE
#define INIT_STACK_SIZE 256
#endif
static STDCAL(void) forech_stack(rContext* parser)
{
    for (size_t i = 0; i < parser->top; i++)
    {
        printf("%lld:%c\n",i,parser->stack[i]);
    }
    
}
static STDCAL(void*) rainbow_stack_push(rContext* parser,size_t size)
{
    void* ret;//为值的位置
    assert(size>0);
    if ((parser->size)-(parser->top) <= size)//栈大小不足
    {
        if (parser->size == 0) parser->size=INIT_STACK_SIZE;
        while (parser->top+size>=parser->size)parser->size += parser->size>>1;
        parser->stack = (char*)realloc(parser->stack,parser->size);
    }
    ret = parser->stack+parser->top;
    parser->top+= size;
    return ret;
}
#define PUSH(PARSER,CH) do{*(char*)rainbow_stack_push(PARSER,sizeof(CH))=(CH);}while(0) //用于调用压栈操作
static STDCAL(void*) rainbow_stack_pop(rContext* parser,size_t size)
{
    void* ret;
    assert(parser->top >= size);
    parser->top -= size;
    ret = parser->stack+parser->top;
    return ret;
}
/********************************************************/
static STDCAL(void) rainbow_set_output(rContext* parser,rItem* item,char* stack,size_t size)//size为栈中需要拷贝的大小，stack为栈中需要拷贝的起始位置
{
    assert(parser->stack!=NULL);
    item->output = (char*)malloc(size+1);
    memcpy(item->output,stack,size);
    *(item->output+size)='\0';
}
static STDCAL(rColor) rainbow_parser_color(char* color)
{
    if(!strcmp(color,"red"))return red;
    else if(!strcmp(color,"clear"))return clear;
    else if(!strcmp(color,"white"))return white;
    else if(!strcmp(color,"blue"))return blue;
    else if(!strcmp(color,"green"))return green;
    else if(!strcmp(color,"yellow"))return yellow;
    else RAISE(Invild_color_error);
    return clear;
}
static STDCAL(void) rainbow_parse_stack(rContext* parser,rItem* item)
{
    assert(parser->size!=0);
    char* c = parser->stack;
    size_t size = 0;
    if(*c=='{')
    {
        char color[20];
        int i;
        c++;
        for(i = 0;(*c=='}' && *(c-1)=='\\' && i<20) || (*c!='}' && i<20);c++,i++)
        {
            if (size>=parser->size)RAISE(color_end_token_error);
            color[i] = *c;
            size += sizeof(*c);
        }
        size += 2;//添加'{'和'}'所占的内存
        color[i] = '\0';
        item->color = rainbow_parser_color(color);
        c++;
    }
    else item->color = clear;
    rainbow_set_output(parser,item,c,parser->top-size);
}
#define PUSH_THIS() PUSH(&rcontext,*rcontext.context);
// #define PUSH_THIS() do{PUSH(&rcontext,*rcontext.context);printf(">\'%c\' pushed\n",*rcontext.context);}while(0)
static STDCAL(void) rainbow_parse(const char* text,va_list arg)
{
    rContext rcontext;
    rainbow_context_init(&rcontext,text);
    while (*rcontext.context!='\0')
    {
        switch (*rcontext.context)
        {
            case '{':
                {
                    rItem item;
                if(rcontext.size!=0)
                {
                    rainbow_parse_stack(&rcontext,&item);
                    rainbow_output(&item,arg);
                    rcontext.stack = (char*)rainbow_stack_pop(&rcontext,rcontext.top);
                    PUSH_THIS();
                }
                else PUSH_THIS();
                break;
                }
            case '\\':
            {
                if(*(rcontext.context+1)=='{'||*(rcontext.context+1)=='}')
                {
                    PUSH_THIS();
                    rcontext.context++;
                    PUSH_THIS();
                }
                else PUSH_THIS();
                break;
            }
            default:
            {
                PUSH_THIS();
            }
        }
        rcontext.context++;
    }
    rItem item;
    rainbow_parse_stack(&rcontext,&item);
    rainbow_output(&item,arg);
    rcontext.stack = (char*)rainbow_stack_pop(&rcontext,rcontext.top);
    free(rcontext.stack);
    free(item.output);
}

#undef PUSH_THIS
/********************************************************/
#define VAG(type) va_arg(arg,type)
/*
穷举记录:
%d.x.o.e.g.p.u.s.c
%ld.lf
%+d.+f
%-d.-f
%lld
%%
*/
static STDCAL(void) rainbow_output(rItem* item,va_list arg)
{
    color_control(item->color);
    char* ch = item->output;
    while (*ch!='\0')
    {
        switch (*ch)
        {
            case '%':
            {
                ch++;
                switch (*ch)
                {
                case '\0':
                    putchar('%');
                    goto out;//用于退出多重分支
                    break;
                case '%':
                    putchar('%');
                case 'd':
                    printf("%d",VAG(int));
                    break;
                case 'o':
                    printf("%o",VAG(int));
                    break;
                case 'f':
                    printf("%f",VAG(double));
                    break;
                case 'x':
                    printf("%x",VAG(int));
                    break;
                case 'u':
                    printf("%u",VAG(int));
                    break;
                case 'e':
                    printf("%e",VAG(double));
                    break;
                case 'g':
                    printf("%g",VAG(double));
                    break;
                case 'p':
                    printf("%p",VAG(void*));
                    break;
                case 's':
                    printf("%s",VAG(char*));
                    break;
                case 'c':
                    printf("%c",VAG(char));
                    break;
                case '+':
                {
                    ch++;
                    switch (*ch)
                    {
                        case '\0':
                            putchar('%');
                            putchar('+');
                            goto out;
                        case 'd':
                            printf("%+d",VAG(int));
                            break;
                        case 'f':
                            printf("%+f",VAG(double));
                            break;
                        default:
                            putchar('%');
                            putchar('+');
                            break;
                    }
                }
                case '-':
                {
                    ch++;
                    switch (*ch)
                    {
                        case '\0':
                            putchar('%');
                            putchar('-');
                            goto out;
                        case 'd':
                            printf("%-d",VAG(int));
                            break;
                        case 'f':
                            printf("%-f",VAG(double));
                            break;
                    }
                }
                case 'l'://%lx...
                {
                    ch++;
                    switch (*ch)
                    {
                    case '\0':
                        putchar('%');
                        putchar('l');
                        goto out;//用于退出多重分支
                        break;
                    case 'd':
                        printf("%ld",VAG(long));
                        break;
                    case 'f':
                        printf("%lf",VAG(double));
                        break;
                    case 'l'://%llx
                    {
                        ch++;
                        switch (*ch)
                        {
                        case '\0':
                            putchar('%');
                            putchar('l');
                            putchar('l');
                            goto out;//用于退出多重分支
                            break;
                        case 'd':
                            printf("%lld",VAG(size_t));
                            break;
                        default:
                            break;
                        }
                    }
                    default:
                        putchar('%');
                        putchar('l');
                        putchar('l');
                        putchar(*ch);
                        break;
                    }
                }
                default:
                    putchar(*ch);
                    break;
                    }
                break;
            }
        default:
            putchar(*ch);
            break;
        }
        ch++;
    }
    out:
    color_control(clear);
}
/********************************************************/
#ifdef _WIN32
    //TODO: code for windows os
#include <windows.h>
static STDCAL(void) color_control(rColor color)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (color)
    {
    case red:
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED);
        break;
    case green:
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        break;
    case blue:
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        break;
    case yellow:
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
        break;
    case white:
    case clear:
        SetConsoleTextAttribute(handle, FOREGROUND_INTENSITY | FOREGROUND_BLUE |FOREGROUND_GREEN | FOREGROUND_RED);
        break;
    default:
        break;
    }
}
extern STDCAL(void) rainbow_print(const char* format,...)
{
    va_list vag;
    va_start(vag,format);
    rainbow_parse(format,vag);
}
#elif __APPLE__||__linux

#define RED printf("\033[31m") //红色字体
#define GREEN printf("\033[36m")//绿色字体
#define YELLOW printf("\033[33m")//黄色字体
#define BLUE printf("\033[34m")//蓝色字体
#define WHITE printf("\e[1;37m")
#define CLEAR printf("\033[0m")
static STDCAL(void) color_control(rColor color)
{
    switch (color)
    {
    case red:
        RED;
        break;
    case green:
        GREEN;
        break;
    case yellow:
        YELLOW;
        break;
    case blue:
        BLUE;
        break;
    case white:
        WHITE;
        break;
    case clear:
        CLEAR;
    default:
        break;
    }
}

#endif

int main(int argc, char const *argv[])
{   
    rainbow_print("{red}hello {blue}world {green}%d",2020);
    return 0;
}
