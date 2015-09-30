#pragma once
#include "logger.h"
#include "single_checker.h"


CSingleChecker::CSingleChecker(void)
{
}

CSingleChecker::~CSingleChecker(void)
{
}

bool CSingleChecker::single(const tstring& mutex_name)
{
    if (m_mutex.valid())//we created the mutex before
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
                ErrorLogLastErr("InitializeSecurityDescriptor fail");
                break;
            }
            if (!SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE))
            {
                ErrorLogLastErr("SetSecurityDescriptorDacl fail");
                break;
            }

            SECURITY_ATTRIBUTES sa;
            sa.nLength =sizeof(SECURITY_ATTRIBUTES);
            sa.lpSecurityDescriptor = &sd;
            sa.bInheritHandle = TRUE;

            tstring mutex_global_name(TSTR("Global\\"));
            mutex_global_name += mutex_name;
            m_mutex.reset(CreateMutex(&sa, FALSE, mutex_global_name.c_str()));

            CLastErrorFormat e;
            if (m_mutex.valid())
            {
                //�����ɹ��Ҵ����ǡ��Ѵ��ڡ�����ȷȷʵʵ���Լ������ģ���ô��Ϊֻ��һ��ʵ�������У������˾��
                if (e.code() != ERROR_ALREADY_EXISTS)
                {
                    bSingle = true;
                }
                else
                {
                    m_mutex.destory();
                }
            }
            else
            {
                //����ʧ���Ҵ����ǡ�Ȩ�޲��㡱�����ѱ����˴������Լ����ܴ򿪣����ȷ���������ʵ����������
                //if (e.code() == ERROR_ACCESS_DENIED)
                //{
                //}
                ErrorLogLastErrEx(e, TSTR("CreateMutex[%s] fail"), mutex_global_name.c_str());
            }

        } while (false);
        return bSingle;
    }
}



