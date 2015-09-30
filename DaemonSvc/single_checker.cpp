#pragma once
#include "logger.h"
#include "single_checker.h"


CSingleChecker::CSingleChecker(void)
    : m_mutex(NULL)
{
}

CSingleChecker::~CSingleChecker(void)
{
    if (m_mutex)
    {
        CloseHandle(m_mutex);
        m_mutex = NULL;
    }
}

bool CSingleChecker::single(const tstring& mutex_name)
{
    if (m_mutex)//we created the mutex before
    {
        return true;
    }
    else
    {
        bool bSingle = false;
        do 
        {
            SECURITY_DESCRIPTOR sd;
            if (!InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION))
            {
                ErrorLogLastErr(CLastError(), "InitializeSecurityDescriptor fail");
                break;
            }
            if (!SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE))
            {
                ErrorLogLastErr(CLastError(), "SetSecurityDescriptorDacl fail");
                break;
            }

            SECURITY_ATTRIBUTES sa;
            sa.nLength =sizeof(SECURITY_ATTRIBUTES);
            sa.lpSecurityDescriptor = &sd;
            sa.bInheritHandle = TRUE;

            tstring mutex_global_name(TSTR("Global\\"));
            mutex_global_name += mutex_name;
            m_mutex = CreateMutex(&sa, FALSE, mutex_global_name.c_str());

            CLastError e;
            if (m_mutex)
            {
                //�����ɹ��Ҵ����ǡ��Ѵ��ڡ�����ȷȷʵʵ���Լ������ģ���ô��Ϊֻ��һ��ʵ�������У������˾��
                if (e.code() != ERROR_ALREADY_EXISTS)
                {
                    bSingle = true;
                }
                else
                {
                    CloseHandle(m_mutex);
                    m_mutex = NULL;
                }
            }
            else
            {
                //����ʧ���Ҵ����ǡ�Ȩ�޲��㡱�����ѱ����˴������Լ����ܴ򿪣����ȷ���������ʵ����������
                //if (e.code() == ERROR_ACCESS_DENIED)
                //{
                //}
                ErrorLogLastErr(e, TSTR("CreateMutex[%s] fail"), mutex_global_name.c_str());
            }

        } while (false);
        return bSingle;
    }
}



