#include <vector>
#include <Windows.h>
#include "boost_algorithm_string.h"
#include "process_creator.h"
#include "process_scanner.h"
#include "logger.h"
#include "cmd_run_as.h"


RUN_AS_TYPE cast_run_as_type_from_string(const std::string& s)
{
    std::string s_lower = boost::algorithm::to_lower_copy(s);
    boost::algorithm::trim(s_lower);

    if (s_lower == "all")
    {
        return AS_ALL_LOGON_USERS;
    }
    else if (s_lower == "first")
    {
        return AS_LOGON_USER;
    }
    else if (s_lower == "local")
    {
        return AS_LOCAL;
    }
    else
    {
        ErrorLog("can not cast string[%s] to RUN_AS_TYPE", s_lower.c_str());
        return AS_UNKNOWN;
    }
}

std::string cast_run_as_type_to_string(const RUN_AS_TYPE& run_as)
{
    std::string s;
    switch (run_as)
    {
    case AS_ALL_LOGON_USERS:
        s = "all";
        break;

    case AS_LOGON_USER:
        s = "first";
        break;

    case AS_LOCAL:
        s = "local";
        break;

    default:
        ErrorLog("unknown RUN_AS_TYPE: %d", run_as);
        s = "unknown";
        break;
    }
    return s;
}

bool cmd_run_as(const tstring& command,
                const RUN_AS_TYPE& as_type,
                const bool show_window /*= true*/)
{
    InfoLog("begin exec");

    bool execute_success = false;
    const unsigned short sw_flag = show_window ? SW_SHOWNORMAL : SW_HIDE;

    std::vector<HANDLE> processes;

    switch (as_type)
    {
    case AS_LOCAL:
        {
            DWORD created_pid = 0;
            HANDLE hProcess = ProcessCreator::create_process_in_local_context(command,
                created_pid, CREATE_NEW_CONSOLE, TSTR(""), sw_flag);
            if (hProcess)
            {
                InfoLog("create_process_in_local_context success, pid=%lu", created_pid);
                processes.push_back(hProcess);
            }
            else
            {
                ErrorLog("create_process_in_local_context fail");
            }
        }
        break;

    case AS_LOGON_USER:
    case AS_ALL_LOGON_USERS:
        {
            std::vector<DWORD> pids;
            find_pids_by_path(TSTR("explorer.exe"), pids);
            if (pids.empty())
            {
                ErrorLog("can not find any explorer.exe");
            }
            else
            {
                for (std::vector<DWORD>::const_iterator iter_pid = pids.begin();
                    iter_pid != pids.end();
                    ++iter_pid)
                {
                    InfoLog("explorer.exe pid=%lu", *iter_pid);
                    DWORD created_pid = 0;
                    HANDLE hProcess = ProcessCreator::create_process_as_same_token(*iter_pid,
                        command, created_pid, CREATE_NEW_CONSOLE, TSTR(""), sw_flag);
                    if (hProcess)
                    {
                        InfoLog("create_process_as_same_token success, pid=%lu", created_pid);
                        processes.push_back(hProcess);
                        if (as_type == AS_LOGON_USER)
                        {
                            break;
                        }
                    }
                    else
                    {
                        ErrorLog(TSTR("create_process_as_same_token fail, pid=%lu, cmd=[%s]"),
                            *iter_pid, command.c_str());
                    }
                }
            }

            if (processes.empty())
            {
                ErrorLog("no new process in user context was created");
            }
        }
        break;

    default:
        ErrorLog("unknown run_as type: %d", as_type);
        break;
    }

    if (!processes.empty())
    {
        //����ź�3�룬���ؿ���������ػ�����ȥ��
        WaitForMultipleObjects(processes.size(), &processes[0], TRUE, 3 * 1000);
        for (std::vector<HANDLE>::iterator iter_process = processes.begin();
            iter_process != processes.end();
            ++iter_process)
        {
            CloseHandle(*iter_process);
            *iter_process = NULL;
        }

        execute_success = true;
    }
    else
    {
        execute_success = false;
    }

    InfoLog("end exec with %s", execute_success ? "success" : "fail");
    return execute_success;
}


