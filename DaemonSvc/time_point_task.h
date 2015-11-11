#pragma once
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

    typedef __int64 seconds_t;
    seconds_t get_last_should_execute_time();
    bool has_executed_after(const seconds_t& begin_time);
    static seconds_t get_local_time();

private:
    bool m_started;
    seconds_t m_last_executed_time;

    TaskFunc m_f;
    const PeriodTime m_period;

    boost::thread m_worker_thread;
    scoped_handle<> m_exit_event;
};

