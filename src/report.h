#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/propgrid/property.h>
#include <wx/propgrid/props.h>
#include <wx/propgrid/propgridiface.h>
#include <vector>
#include "result.h"

class Report
{
private:
    wxString m_str;

public:
    void add_props(const wxString& name, const wxPropertyGridInterface& props);
    void add_results(const std::vector<Result>& results);
    void save(const wxString& filename);
};
