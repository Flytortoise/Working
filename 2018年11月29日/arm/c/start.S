.text
.global _start


_start:
    /*设置内存：sp 栈 */
    ldr sp,=4096    /*Nand 启动，栈设为片内RAM顶部*/
    # ldr sp,=0x40000000+4096   #Nor启动
    
    /*调用main*/
    bl main


halt:
    b halt