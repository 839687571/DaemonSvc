//reference: http://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus
#pragma once


//need function "MiniDumpWriteDump" in Dbghelp.dll which is always existing on Windows XP and later version
//if you want to use it on Windows 2000, you can put Windows XP's Dbghelp.dll in your program dir
//dmp file name will end with ".dmp" in exe dir, see source file for details


namespace exception_catcher
{
    //some exception handlers work on per process, others work on per thread
    //per process: SEH exception, pure virtual function call, C++ new exception, runtime invalid parameter error, signal: SIGABRT, SIGINT, SIGTERM
    //per thread: terminate() call, unexpected() call, signal: SIGFPE, SIGILL, SIGSEGV
    void set_process_exception_handlers();
    void set_thread_exception_handlers();
}



/*

//
//
//�����޸���http://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus
//���������޸ģ�
//      1. ԭ������GetExceptionPointers��ʵ���Ǹ�����vc8.0���������Ǹ�����vc9.0
//      2. ԭ�������Ƕ�̬��ȡMiniDumpWriteDump�����ĵ�ַ���������Ǿ�̬������Dbghelp.dll
//  ��Ϊ���Ҳ��ԣ���ȻWindows2000��Dbghelp.dll��û��MiniDumpWriteDump�����������ǿ�����WindowsXPSP3��Dbghelp.dll����������dump�ļ���
//  ���ԣ�����ý�WindowsXPSP3��Dbghelp.dll�����Ӧ�ó���һͬ��������û�в��������汾��Dbghelp.dll�Ƿ�����Windows2000����
//
//

//����ʾ���е�SEH�쳣��һ�λ�������dmp�ļ�����һ����SIGSEGV�ģ��ڶ�����SEH�ġ�������ָʾ������ȷ���쳣���
//����ʾ���е�terminate���ӣ�һ�λ��������dmp�ļ�����һ����terminate�ģ��ڶ�����SIGABRT�ġ�����������ָʾ����unexpected()�ĵ���λ�ã�����
//����ʾ���е�unexpected���ӣ�һ�λ��������dmp�ļ�����һ����unexpected�ģ��ڶ�����terminate�ģ���������SIGABRT�ġ���ָʾ����unexpected()�ĵ���λ��
//����ʾ���еĴ��麯�������쳣��һ�λ��������dmp�ļ�����һ���Ǵ��麯�����õģ��ڶ�����SIGABRT�ġ���ָʾ���˴��麯�����õ�λ��
//����ʾ���еķǷ��������ã�һ�λ����һ��dmp�ļ���ָʾλ����ȷ
//����ʾ���е�new�쳣��һ�λ��������dmp�ļ�����һ����new�ģ�ָʾλ����ȷ���ڶ�����SEH�ģ�����ջ��AVbadcast->_onexit->_unlockexit->_unlock->LeaveCriticalSection
//����ʾ���е�SIGABRT�źŹ��ӣ�һ�λ����һ��dmp�ļ���ָʾλ����ȷ
//����ʾ���е�SIGFPE�źŹ��ӣ��Ҳ��Ե�ʱ�������䲢û�з��������쳣��Release�棩�����ܻ�Ҫ����ʲô�����������޷���֤�Ƿ��ܹ����񵽳����쳣
//����ʾ���е�SIGILL�źŹ��ӣ�һ�λ����һ��dmp�ļ���ָʾλ����ȷ
//����ʾ���е�SIGINT�źŹ��ӣ�һ�λ����һ��dmp�ļ���ָʾλ����ȷ
//����ʾ���е�SIGSEGV�źŹ��ӣ�һ�λ����һ��dmp�ļ���ָʾλ����ȷ
//����ʾ���е�SIGTERM�źŹ��ӣ�һ�λ����һ��dmp�ļ���ָʾλ����ȷ
//����ʾ���е�RaiseException�쳣���ڲ�ͬ��code�¿��ܲ�����ͬ������dmp�ļ�����������ջ���ǣ�_onexit->printf
//����ʾ���е�C++�����쳣��һ�λ����һ��dmp�ļ�����SEH�ģ��������͵Ĳ�ͬ����ջ��ͬ�����Ƕ�ûָ����ȷ�Ĵ����ϣ�����


//
//������ԭ�����е�ʾ�������Լ��޸ģ�
//


#include <float.h>
#include <stdio.h>
#include <signal.h>
#include <conio.h>
#include <stdlib.h>
#include <exception>
#include <Windows.h>
#include "logger.h"
#include "exception_catcher.h"



void sigfpe_test()
{ 
    // Code taken from http://www.devx.com/cplus/Article/34993/1954

    //Set the x86 floating-point control word according to what
    //exceptions you want to trap. 
    _clearfp(); //Always call _clearfp before setting the control
    //word
    //Because the second parameter in the following call is 0, it
    //only returns the floating-point control word
    unsigned int cw; 
    _controlfp_s(&cw, 0, 0); //Get the default control
    //word
    //Set the exception masks off for exceptions that you want to
    //trap.  When a mask bit is set, the corresponding floating-point
    //exception is //blocked from being generating.
    cw &=~(EM_OVERFLOW|EM_UNDERFLOW|EM_ZERODIVIDE|
        EM_DENORMAL|EM_INVALID);
    //For any bit in the second parameter (mask) that is 1, the 
    //corresponding bit in the first parameter is used to update
    //the control word.  
    unsigned int cwOriginal;
    _controlfp_s(&cwOriginal, cw, MCW_EM); //Set it.
    //MCW_EM is defined in float.h.
    //Restore the original value when done:
    //_controlfp(cwOriginal, MCW_EM);

    // Divide by zero

    float a = 1;
    float b = 0;
    float c = a/b;
    c; 
}

#define BIG_NUMBER 0x1fffffff
#pragma warning(disable: 4717) // avoid C4717 warning
int RecurseAlloc() 
{
    int *pi = new int[BIG_NUMBER];
    pi;
    RecurseAlloc();
    return 0;
}

class CDerived;
class CBase
{
public:
    CBase(CDerived *derived): m_pDerived(derived) {};
    ~CBase();
    virtual void function(void) = 0;

    CDerived * m_pDerived;
};

#pragma warning(disable:4355)
class CDerived : public CBase
{
public:
    CDerived() : CBase(this) {};   // C4355
    virtual void function(void) {};
};

CBase::~CBase()
{
    m_pDerived -> function();
}

int main()
{
    InitLog("", 0, LOG_DEBUG);

    exception_catcher::SetProcessExceptionHandlers();
    exception_catcher::SetThreadExceptionHandlers();

    printf("Choose an exception type:\n");
    printf("0 - SEH exception\n");
    printf("1 - terminate\n");
    printf("2 - unexpected\n");
    printf("3 - pure virtual method call\n");
    printf("4 - invalid parameter\n");
    printf("5 - new operator fault\n");	
    printf("6 - SIGABRT\n");
    printf("7 - SIGFPE\n");
    printf("8 - SIGILL\n");
    printf("9 - SIGINT\n");
    printf("10 - SIGSEGV\n");
    printf("11 - SIGTERM\n");
    printf("12 - RaiseException\n");
    printf("13 - throw C++ typed exception\n");
    printf("Your choice >  ");

    int ExceptionType = 0;
    scanf_s("%d", &ExceptionType);

    switch(ExceptionType)
    {
    case 0: // SEH
        {
            // Access violation
            int *p = 0;
#pragma warning(disable : 6011)   // warning C6011: Dereferencing NULL pointer 'p'
            *p = 0;
#pragma warning(default : 6011)   
        }
        break;
    case 1: // terminate
        {
            // Call terminate
            terminate();
        }
        break;
    case 2: // unexpected
        {
            // Call unexpected
            unexpected();
        }
        break;
    case 3: // pure virtual method call
        {
            // pure virtual method call
            CDerived derived;
        }
        break;
    case 4: // invalid parameter
        {      
            char* formatString;
            // Call printf_s with invalid parameters.
            formatString = NULL;
#pragma warning(disable : 6387)   // warning C6387: 'argument 1' might be '0': this does not adhere to the specification for the function 'printf'
            printf(formatString);
#pragma warning(default : 6387)   

        }
        break;
    case 5: // new operator fault
        {
            // Cause memory allocation error
            RecurseAlloc();
        }
        break;
    case 6: // SIGABRT 
        {
            // Call abort
            abort();
        }
        break;
    case 7: // SIGFPE
        {
            // floating point exception ( /fp:except compiler option)
            sigfpe_test();            
        }    
        break;
    case 8: // SIGILL 
        {
            raise(SIGILL);              
        }    
        break;
    case 9: // SIGINT 
        {
            raise(SIGINT);              
        }    
        break;
    case 10: // SIGSEGV 
        {
            raise(SIGSEGV);              
        }    
        break;
    case 11: // SIGTERM
        {
            raise(SIGTERM);            
        }
        break;
    case 12: // RaiseException 
        {
            // Raise noncontinuable software exception
            RaiseException(EXCEPTION_ACCESS_VIOLATION, EXCEPTION_NONCONTINUABLE, 0, NULL);        
        }
        break;
    case 13: // throw 
        {
            // Throw typed C++ exception.
            throw std::runtime_error("123");
        }
        break;
    default:
        {
            printf("Unknown exception type specified.");       
            _getch();
        }
        break;
    }

    system("pause");
    return 0;
}

*/

