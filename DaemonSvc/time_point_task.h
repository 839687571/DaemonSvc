#pragma once
#include "task_common.h"
#include "task_base.h"


//ʱ���������ÿ�졢ÿ�ܡ�ÿ�µ�ָ��ʱ��ִ��
class CTimePointTask : public CTaskBase
{
public:
    CTimePointTask(const TaskFunc& f, const PERIOD_TYPE& type, const TaskTime& tm);
    ~CTimePointTask(void);

public:
    bool is_started() const;
    bool start();
    void stop();

private:
    bool m_started;
};
