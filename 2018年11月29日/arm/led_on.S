.text   //表示代码段
.global _start


_start:
    #将 GPA25，25设置位输出引脚
    ldr r1,=0x56000000
    ldr r0,=0xf9ffffff
    str r0,[r1]

    #点亮LED
    ldr r1,=0x56000004
    ldr r0,=0
    str r0,[r1]


#while(1)
halt:
    b halt
