#pragma once
#include <vector>
#include <boost/noncopyable.hpp>
#include "tdef.h"
#include "config_info.h"
#include "xml.h"


class CConfigLoader : public boost::noncopyable
{
public:
    CConfigLoader(const tstring& file_path);
    ~CConfigLoader(void);

    typedef TimeIntervalTaskInfo ti_info;
    typedef TimePointTaskInfo tp_info;
    typedef ProcNonExistTaskInfo pne_info;

    typedef std::vector<ti_info> ti_info_list;
    typedef std::vector<tp_info> tp_info_list;
    typedef std::vector<pne_info> pne_info_list;

    void get(ti_info_list& infos) const;
    void get(tp_info_list& infos) const;
    void get(pne_info_list& infos) const;

private:
    void load(const tstring& file_path);

    static bool parse_common_info(xml_node_ptr pnode, CommonInfo& ci);

    static bool parse_one_info(xml_node_ptr pnode, ti_info& info);
    static bool parse_one_info(xml_node_ptr pnode, tp_info& info);
    static bool parse_one_info(xml_node_ptr pnode, pne_info& info);

    static void parse_all_infos(xml_doc_ptr pdoc, ti_info_list& infos);
    static void parse_all_infos(xml_doc_ptr pdoc, tp_info_list& infos);
    static void parse_all_infos(xml_doc_ptr pdoc, pne_info_list& infos);

private:
    ti_info_list m_ti_infos;
    tp_info_list m_tp_infos;
    pne_info_list m_pne_infos;
};
