#define STDCAL(type) type __cdecl
extern STDCAL(void) rainbow_print(const char* format,...);
extern STDCAL(void) chinese_support(void);
#define __VERSION__ 0,1,2
#define VERSION() rainbow_print("{yellow}%d.%d.%d",__VERSION__)