//
// Created by fpf on 2019-09-05.
//

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

// gcc cpuid.c -o cpuid
#define HWI_BUFFER_SIZE 255
#define cpuid(func, eax, ebx, ecx, edx)\
    __asm__ __volatile__ ("cpuid":\
    "=a" (eax),"=b" (ebx),"=c" (ecx),"=d" (edx):\
    "a" (func));
//asm statements： "cpuid"。汇编代码为——调用cpuid指令
//outputs："=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)。
//表示有4个输出参数——参数0为eax（绑定到变量a）、参数1为ebx（绑定到变量b）、参数2为ecx（绑定到变量c）、参数3为edx（绑定到变量d）。
//inputs："a" (func)。表示有1个输入参数——将变量func赋给参数0（eax）。
//registers-modified：（无）。
//因此，上述cpuid函数执行过程为：
//1. 将变量func的值赋给eax
//2. 执行cpuid指令
//3. 将返回的eax的值赋给a
//4. 将返回的ebx的值赋给b
//5. 将返回的ecx的值赋给c
//6. 将返回的edx的值赋给d


void getCPUID(char *cpuId)
{
    int a,b,c,d;
    char cpuid_buf[HWI_BUFFER_SIZE];
    memset((void*)cpuid_buf,0,HWI_BUFFER_SIZE);
    cpuid(0x80000004,a,b,c,d);
//    printf("%08X-%08X-%08X-%08X\n",a,b,c,d);
    sprintf(cpuid_buf,"%#010x %#010x %#010x %#010x",a,b,c,d);
    strcpy(cpuId,cpuid_buf);
}

//获取CPU 厂商
void getCPUCompany(char *cCom)
{
    char ComName[13];
    int a,b,c,d;
    int i,k;

    memset((void*)ComName,0,sizeof(ComName));
    cpuid(0,a,b,c,d);

    for(i = 0; b > 0; i++)
    {
        k = b;
        k = (k>>8);
        k = (k<<8);
        ComName[i]=b-k;
        b=(b>>8);
    }
    for(; d > 0; i++)
    {
        k = d;
        k = (k>>8);
        k = (k<<8);
        ComName[i]=d-k;
        d=(d>>8);
    }
    for(; c > 0; i++)
    {
        k = c;
        k = (k>>8);
        k = (k<<8);
        ComName[i]=c-k;
        c=(c>>8);
    }
    ComName[12]='\0';
    strcpy(cCom,ComName);
}

//获取CPU Family,Model,Stepping ID
void getCPUBaseParam(char *baseParam)
{
    char baseParamBuf[HWI_BUFFER_SIZE];
    unsigned long CPUBaseInfo;
    int a,b,c,d;

    memset((void*)baseParamBuf,0,HWI_BUFFER_SIZE);
    cpuid(1,a,b,c,d);

    CPUBaseInfo = a;
    sprintf(baseParamBuf,"Family:%ld Model:%ld Stepping ID:%ld",
            (CPUBaseInfo & 0x0F00)>>8,(CPUBaseInfo & 0xF0)>>4,CPUBaseInfo & 0xF);
    strcpy(baseParam,baseParamBuf);
}

//获取CPU Brand
void getCPUBrand(char *cBrand)
{
    int a,b,c,d,i;
    unsigned int cpu_brand_buf[13];
    int k = 0;

    memset((void*)cpu_brand_buf,0,sizeof(cpu_brand_buf));
    /**
    * eax == 0x800000000
    * 如果CPU支持Brand String，则在EAX中返 >= 0x80000004的值。
    */
    cpuid(0x80000000,a,b,c,d);
    if(a < 0x80000004)
    {
        printf("the cpu is not support\n");
        return;
    }

    for(i=0x80000002;i<=0x80000004;i++)
    {
        cpuid(i,a,b,c,d);
        cpu_brand_buf[k++]=a;
        cpu_brand_buf[k++]=b;
        cpu_brand_buf[k++]=c;
        cpu_brand_buf[k++]=d;
    }
    strcpy(cBrand,(char*)cpu_brand_buf);
    printf("CPU Brand:%s\n", (char *)cpu_brand_buf);
}

static void cpuInfo(unsigned int cpuinfo[4], unsigned int fn_id)
{
    unsigned int deax,debx,decx,dedx;
#ifdef WIN32
    __asm
    {
        mov eax,fn_id   ; 将参数赋值给eax
        cpuid             ; 执行cpuid指令
        mov deax,eax    ; 将寄存器值赋值给临时变量
        mov debx,ebx
        mov decx,ecx
        mov dedx,edx
    }

#elif defined(__GNUC__)
    __asm__ ("cpuid"
    :"=a"(deax),
    "=b"(debx),
    "=c"(decx),
    "=d"(dedx)
    :"a"(fn_id));
#endif
    cpuinfo[0]=deax;
    cpuinfo[1]=debx;
    cpuinfo[2]=dedx;
    cpuinfo[3]=decx;
}

int main(int argc,char** argv)
{
    unsigned int fn_id = 0;
    if(argc == 2){
        fn_id = atoi(argv[1]);
    }
    unsigned int cpu[4];
    cpuInfo(cpu,fn_id);
    printf("%08X-%08X-%08X-%08X\n",cpu[0],cpu[1],cpu[2],cpu[3]);

    //id
    char cpuid[13]  = {0};
    memcpy(cpuid, &cpu[1], 4);
    memcpy(cpuid+4, &cpu[2], 4);
    memcpy(cpuid+8, &cpu[3], 4);
    printf("%s\n", cpuid);

    //all
    char *cCom;
    cCom = (char *) malloc(sizeof(char) * 32);
    getCPUCompany(cCom);
    printf("%s\n", cCom);
    free(cCom);

    char *cpuId;
    cpuId = (char *) malloc(sizeof(char) * 32);
    getCPUID(cpuId);
    printf("%s\n", cpuId);
    free(cpuId);

    char *cBrand;
    cBrand = (char *) malloc(sizeof(char) * 32);
    getCPUBrand(cBrand);
    printf("%s\n", cBrand);
    free(cBrand);

    char *baseParam;
    baseParam = (char *) malloc(sizeof(char) * 32);
    getCPUBaseParam(baseParam);
    printf("%s\n", baseParam);
    free(baseParam);
    return 0;
}