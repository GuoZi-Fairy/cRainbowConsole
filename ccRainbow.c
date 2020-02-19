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
    white,
    red,
    clear
}rColor;
//__bool
typedef enum ccRainbow_boolea
{
    False,
    True
}boolean;

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
static STDCAL(void) rainbow_parse(const char* text);
static STDCAL(void) rainbow_count_token(rItem* item);
static STDCAL(void) rainbow_output(rItem* item);
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
static STDCAL(void) rainbow_parse(const char* text)
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
                    rainbow_output(&item);
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
    rainbow_output(&item);
    rcontext.stack = (char*)rainbow_stack_pop(&rcontext,rcontext.top);
    free(rcontext.stack);
    free(item.output);
}

#undef PUSH_THIS
/********************************************************/
static STDCAL(void) rainbow_output(rItem* item)
{
    char* ch = item->output;
    while (ch!='/0')
    {
        switch (*ch)
        {
            case '%':
                {
                    
                    break;
                }
        }
    }
    
}
/********************************************************/
#ifdef _WIN32
    //TODO: code for windows os
    
#elif __APPLE__||__linux
    //TODO:code for osX/linux
#endif

int main(int argc, char const *argv[])
{   
    const char* test = "{red}\\{\\}{white}hehe";
    rainbow_parse(test);
    return 0;
}
