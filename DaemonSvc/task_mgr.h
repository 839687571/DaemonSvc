#pragma once
#include <map>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>
#include "tdef.h"
#include "period_time.h"
#include "task_base.h"


class CTaskMgr : public boost::noncopyable
{
private:
    CTaskMgr(void);
    ~CTaskMgr(void);

public:
    static CTaskMgr& GetInstanceRef()
    {
        static CTaskMgr instance;
        return instance;
    }

public:
    typedef unsigned int TaskId;

    //diff�����
    TaskId add_time_point_task(const TaskFunc& f, const PeriodTime& period);
    TaskId add_time_interval_task(const TaskFunc& f, const DWORD interval_seconds);
    TaskId add_proc_non_exist_task(const TaskFunc& f, const tstring& proc_path, const DWORD interval_seconds);

    bool start_one(const TaskId id);
    void start_all(std::vector<TaskId>& failed_ids);

    void stop_one(const TaskId id);
    void stop_all();

    void delete_one(const TaskId id);
    void delete_all();

private:
    //����
    TaskId alloc_task_num_id();

private:
    typedef boost::shared_ptr<CTaskBase> TaskBasePtr;
    typedef std::map<TaskId, TaskBasePtr> TaskMap;
    TaskMap m_tasks;
    boost::mutex m_tasks_lock;
};









