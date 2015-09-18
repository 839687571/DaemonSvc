#pragma once
#include <vector>
#include <map>
#include <Windows.h>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include "service_info.h"
#include "logger.h"


class CWin32Service : public boost::noncopyable
{
private:
    CWin32Service(void);
    ~CWin32Service(void);

public:
    static CWin32Service& GetInstanceRef()
    {
        static CWin32Service instance;
        return instance;
    }

public:
    bool Init(const ServiceInfo& info, const bool service_mode);

    typedef std::vector<tstring> ArgList;
    typedef boost::function<bool(const ArgList&)> StartingFunction;//ArgList��Ӧ�ó���������в���
    typedef boost::function<void(const ArgList&)> ServiceFunction;

    void RegisterStartingFunction(const StartingFunction& f)
    {
        m_startingfunc = f;
    }

    void RegisterRunningFunction(const ServiceFunction& f)
    {
        m_runningfunc = f;
    }

    void RegisterControlCodeFunction(const DWORD c, const ServiceFunction& f)
    {
        assert(SERVICE_CONTROL_INTERROGATE != c);
        m_ctrlfuncs[c] = f;
    }

    bool Go();

private:
    bool ReportStatus(const DWORD nState, const DWORD nWaitHintMS);
    void ServiceCtrl(const DWORD code);
    bool StartDispatcher();
    BOOL ConsoleCtrl(DWORD code);
    void ServiceMain(int argc, tchar * argv[]);

    static void WINAPI s_ServiceCtrl(DWORD code);
    static BOOL WINAPI s_ConsoleCtrl(DWORD code);
    static void WINAPI s_ServiceMain(int argc, tchar * argv[]);

private:
    ServiceInfo m_info;
    ArgList m_args;
    StartingFunction m_startingfunc;
    ServiceFunction m_runningfunc;
    bool m_service_mode;//�Ƿ����Է���ʽ���еġ����Ƿ���ʽ����ʱ��ReportStatus����Ϊ

    typedef std::map<DWORD, ServiceFunction> CtrlFuncs;
    CtrlFuncs m_ctrlfuncs;

    SERVICE_STATUS_HANDLE m_service_status_handle;//����״̬���
    SERVICE_STATUS m_service_status;//����״̬
};


