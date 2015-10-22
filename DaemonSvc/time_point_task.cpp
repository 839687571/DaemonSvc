#include <cassert>
#include <Windows.h>
#include <boost/bind.hpp>
#include "logger.h"
#include "time_point_task.h"


CTimePointTask::CTimePointTask(const TaskFunc& f, const PeriodTime& period)
    : m_started(false)
    , m_f(f)
    , m_period(period)
{
    memset(&m_last_execute_time, 0, sizeof(m_last_execute_time));
}

CTimePointTask::~CTimePointTask(void)
{
    if (m_started)
    {
        stop();
    }
}

bool CTimePointTask::is_started() const
{
    return m_started;
}

bool CTimePointTask::start()
{
    if (m_started)
    {
        return true;
    }
    else
    {
        m_exit_event.reset(CreateEvent(NULL, TRUE, FALSE, NULL));
        if (!m_exit_event.valid())
        {
            ErrorLogLastErr("CreateEvent for notify time point task thread exit fail");
        }
        else
        {
            try
            {
                m_worker_thread = boost::thread(boost::bind(&CTimePointTask::worker_func, this));
                m_started = true;
            }
            catch (boost::thread_resource_error& e)
            {
                ErrorLog("create time point task worker thread fail, error: %s", e.what());
            }
        }

        return m_started;
    }
}

void CTimePointTask::stop()
{
    if (m_started)
    {
        assert(m_exit_event.valid());

        SetEvent(m_exit_event.get_ref());
        if (m_worker_thread.joinable())
        {
            m_worker_thread.join();
        }

        m_started = false;
    }
}

void CTimePointTask::worker_func()
{
    InfoLog("time point task worker thread func begin");
    ErrorLog("time point task not implemented");

    while (true)
    {
        SYSTEMTIME systime = {0};
        GetLocalTime(&systime);

        //������һ��Ӧִ�е�ʱ��
        //����ǰʱ�����һ��Ӧִ��ʱ��֮����û�б�ִ�й������ϴ�ִ��ʱ���Ƿ�����һ��Ӧִ��ʱ��֮��
        //���У��������һ�εȴ�
        //��û�У����ж��Ƿ�����Χ�ڣ�����ǰʱ��-��һ��Ӧִ��ʱ�� �Ƿ� <= ���
        //���ڣ���ִ�У���������һ��ִ��ʱ��
        //�����ڣ��������һ�εȴ�

        //�ȴ�ʱ�䣺�ȴ����ʱ���һ�룬����һ�����10���ӣ�����10���Ӽ�

        //todo
        const DWORD wait_result = WaitForSingleObject(m_exit_event.get_ref(), INFINITE);
        if (WAIT_OBJECT_0 == wait_result)
        {
            InfoLog("got exit notify");
            break;
        }
    }

    InfoLog("time point task worker thread func end");
}

