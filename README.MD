# cRainbowconsole

simply construct to put colorful string to console

## Table of contents
table of Contents(out of data)
- [cRainbowconsole](#crainbowconsole)
  - [Table of contents](#table-of-contents)
  - [Usage](#usage)
    - [Welcome to cRainbowconsole](#welcome-to-crainbowconsole)
    - [Build](#build)
      - [Coping the source](#coping-the-source)
    - [Data Struct](#data-struct)
    - [API](#api)
      - [Start your ccRainbowconcole progress](#start-your-ccrainbowconcole-progress)
        - [color](#color)
      - [format control token](#format-control-token)
  - [License](#license)
  - [Help](#help)
  - [Enjoy cRainbowconsole](#enjoy-crainbowconsole)
## Usage

### Welcome to cRainbowconsole

### Build
There are serval ways to incorporate cMENU into your projects

#### Coping the source
The entire library is only one C file and one Header file,you can just copy the `ccRainbow.h` and `ccRainbow.c` to your projects source and start using it.
You could include cMENU like this:
``` c
#include "ccRainbow.h"
```

cRainbowconsole is written in ANSI C (C89) in order to support as many platforms and compilers as possible.

### Data Struct
There is no struch you need to learn
### API
* There is only one core API `rainbow_print`
* data:2020/02/20 chinese support: If you want to output chinese char,you should use`chinese_support();`statament at the most top of your code.
* You can know the version of C-rainbowconsole by call `VERSION()`.
#### Start your ccRainbowconcole progress
You can just simply use ccRainbowconsole like `printf`
> The statament `rainbow_print("{red}hello {blue}world {green}%d",2020);` will output:
> ![demo.PNG](https://i.loli.net/2020/02/20/ruRc2hdLQWsV56b.png)

If you want to output '{' or '}'  you need to char "\\{"("\\}") or "\{"("\}")

##### color
- {red}
- {green}
- {blue}
- {yellow}
- {white}
- {clear}

#### format control token
From version:0.1.2 you could use all format_control_token in printf()
## License

MIT License

>  Copyright (c) 2009-2017 Dave Gamble and cJSON contributors
>
>  Permission is hereby granted, free of charge, to any person obtaining a copy
>  of this software and associated documentation files (the "Software"), to deal
>  in the Software without restriction, including without limitation the rights
>  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
>  copies of the Software, and to permit persons to whom the Software is
>  furnished to do so, subject to the following conditions:
>
>  The above copyright notice and this permission notice shall be included in
>  all copies or substantial portions of the Software.
>
>  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
>  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
>  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
>  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
>  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
>  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
>  THE SOFTWARE.

## Help
if your output did not colorful,you need to check here.
On OS windows,the color could only display on cmd\shell or other application that use win32.api
On OS [OS X,Linux,unix] you should check your application whether compiled on the os you are using.
Now ,the cRainbowconsole could not parser format control token the following token `[%m.n*]`
## Enjoy cRainbowconsole
- GuoZi (original author)
- 