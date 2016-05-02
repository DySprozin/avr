## SD-card v.1.0 read/write on tiny-AVR (USI) ##

How to use it

1) Create new Project (or open exist one) in IAR Embedded Workbench
2) Add __flash feature in project's options: 

![enter image description here](http://anabot.ru/uploads/1462219068_iiflash.png)

3) Put SD_USI.h and SD_USI.c files in project's folder
4) Include SD_USI.h in main project's file (in main.c)
5) Add SC_USI.c in project (right-click on Workspace -> Add -> Add Files...)
6) Compiling and enjoing! ;)

Any questions — on sprozin@anabot.ru or dysprozin@yandex.ru
