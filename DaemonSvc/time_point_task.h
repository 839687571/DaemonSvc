#pragma once
#include <Windows.h>
#include <boost/thread.hpp>
#include "scoped_handle.h"
#include "task_base.h"
#include "period_time.h"



//ʱ���������ÿ�졢ÿ�ܡ�ÿ�µ�ָ��ʱ��ִ��
class CTimePointTask : public CTaskBase
{
public:
    //deviation_minutes�����
    CTimePointTask(const TaskFunc& f, const PeriodTime& period);
    ~CTimePointTask(void);

public:
    bool is_started() const;
    bool start();
    void stop();

private:
    void worker_func();

private:
    bool m_started;
    SYSTEMTIME m_last_execute_time;

    TaskFunc m_f;
    const PeriodTime m_period;

    boost::thread m_worker_thread;
    scoped_handle<false> m_exit_event;
};
