int main()
{
    unsigned int *pGPACON = (unsigned int *)0x56000000;
    unsigned int *pGPADAT = (unsigned int *)0x56000004;

    *pGPACON &= 0xf9ffffff;
    *pGPADAT &= 0xf9ffffff;
    
    return 0;
}