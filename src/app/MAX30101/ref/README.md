<!--
 * @Author: Chengsen Dong 1034029664@qq.com
 * @Date: 2023-02-19 16:50:24
 * @LastEditors: Chengsen Dong 1034029664@qq.com
 * @LastEditTime: 2023-02-19 19:06:36
 * @FilePath: /SleepPanda/src/app/MAX30101/ref/README.md
 * @Description: 
 * Copyright (c) 2023 by ${git_name_email}(www.github.com/xddcore), All Rights Reserved. 
-->
**Ref:https://github.com/aa1ww-coop/SpO2A**

We refactored the above open source code (making it object-oriented programming) and fixed several bugs.

Thanks to the above open source code authors for their contributions, SleepPanda 19/02/2023

Compile command:
```
gcc  -Wall -pthread -o RD117piA RD117piA.cpp -lpigpio -lrt -lm `pkg-config --libs --cflags gtk+-3.0`
```