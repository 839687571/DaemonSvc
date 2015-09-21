#pragma once
#include <Windows.h>
#include <boost/thread.hpp>
#include "task_common.h"
#include "task_base.h"


//���̲��������񣺽��̲�����ʱִ�У���ָ��������ѯ�����
class CProcNonExistTask : public CTaskBase
{
public:
    CProcNonExistTask(const TaskFunc& f, const tstring& proc_path, const DWORD interval_seconds);
    ~CProcNonExistTask(void);

public:
    bool is_started() const;
    bool start();
    void stop();

private:
    void worker_func();

private:
    bool m_started;

    TaskFunc m_f;
    const tstring m_proc_path;
    const DWORD m_interval_seconds;

    boost::thread m_worker_thread;
    HANDLE m_hExitEvent;
};
