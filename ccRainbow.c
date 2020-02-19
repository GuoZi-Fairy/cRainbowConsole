//TODO:json-parser:
/*
"[{"text":"","color":"red"},{"text":"world","color":"blue"}]"
"{\"text\":\" \",\"color\":\"red\"}"
"\"text\":\"hello\",\"color\":\"red\""
"\"text\":\"hello\""
"hello"
%d %c %s
可将上述类型解析为
text:hello world
color:red
实现:
    解析json，并建立数据类型
*/
#include <stdlib.h>
#include <stdio.h>
#define STDCAL(type) type __cdecl

typedef struct rainbowItem rItem;//"key":"value"
typedef struct rainbowJson rJson;//rItem

typedef struct error
{
    char* error_type;
    char* error_msg;
    unsigned int position;//为错误解析的索引
}error;

//__bool
typedef enum ccRanbow_boolea
{
    False,
    True
}boolean;
#define RAINBOW_PARSER const char*
typedef struct ranbowcontext
{
    RAINBOW_PARSER json;//表示当前指向的字符
    size_t index;
}rContext;
typedef struct ranbowpos //会对解析字符串的大小产生限制
{
    size_t start;//为第一个"在字符串中的索引
    size_t end;//为第二个"在字符串的索引
}rPos;

typedef struct rainbowItem
{
    char* key;
    char* value;//rainbowJson could only use string to be value
    rItem* next;//to the next rItem
}rItem;
typedef struct rainbowJson
{
    rItem* node;//to the fist Item of Json_obj
    rJson* next;
}rJson;
/********************************************************/
static STDCAL(void) rainbow_parse_whitespace(rContext* parser)
{
    RAINBOW_PARSER p = parser->json;
    while (*p=='\t'||*p==' '||*p=='\n'||*p=='\r')
    {
        p++;
        (parser->index)++;
    }
    parser->json = p;
}
/*******************************************************/
static STDCAL(void) rainbow_set_string(rItem* item,rPos* pos,const char* text)
{
    size_t len = (pos->end)-(pos->start)+1;//被获取的字符串的实际长度
    free(item->value);
    item->value = (char*)malloc(sizeof(char)*(len+1));
    char* p_item = item->value;
    const char* p_text = text + (pos->start)+1;//字符串中第一个字符 除去\"
    for(size_t i = (pos->start+1); i< (pos->end); i++, p_item++ , p_text++) *p_item = *p_text;
    *(p_item) = '\0';//set the end to \0
}
static STDCAL(void) rainbow_parse_string(rContext* parser)
{
}
/********************************************************/
static STDCAL(void) rContext_init(rContext* parser,const char* text)
{
    parser->json  = text;
    parser->index = 0;
}
/********************************************************/
static STDCAL(int) rainbow_parse(const char* text)
{
    rContext parser;
    rContext_init(&parser,text);
    rainbow_parse_whitespace(&parser);
    switch (*(parser.json))
    {
    case '\"':
        rainbow_parse_string(&parser);
        break;
    case '\0':
        return 0;
    default:
        break;
    }
}
/********************************************************/
#ifdef _WIN32
    //TODO: code for windows os
    #include <conio.h>
#endif

#ifdef __APPLE__||__linux
    //TODO:code for osX/linux
    #define RED 0x0004
#endif

//for test
int main(int argc, char const *argv[])
{
    rPos pos = {6,12};
    const char test[] = "hello \"world\",hello cctv, hello bilibili";
    rItem test_item;
    test_item.key = "test";
    test_item.value = (char*)malloc(sizeof(char)*100);
    rainbow_set_string(&test_item,&pos,test);
    printf("::%s::",test_item.value);
    
    return 0;
}
