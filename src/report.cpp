#include "report.h"
#include <wx/wfstream.h>
#include <wx/sstream.h>

void Report::add_props(const wxString& name, const wxPropertyGridInterface& props)
{
    m_str << name << " properties:\n";
    auto pi = props.GetIterator();
    while(!pi.AtEnd())
    {
        auto p = pi.GetProperty();
        m_str << p->GetName() << " = " << p->GetValueAsString() << "\n";
        pi.Next();
    }
    m_str << "\n";
}

void Report::add_results(const std::vector<Result>& results)
{
    m_str << "Results:\n";
    int total = 0;
    int total_correct = 0;

    for (auto res : results)
    {
        m_str << "Class " << res.get_class() << "\n";
        m_str << "Detections\n";
        auto detections = res.get_detections();

        for (auto d : detections)
        {
            m_str << d << " ";
        }
        total += detections.size();
        float accuracy = res.get_percent_accuracy();
        total_correct += detections.size() * accuracy / 100.0f;
        m_str << "\nPercent accuracy = " << accuracy << "%\n\n";
    }
    m_str << "Total accuracy = " << total_correct * 100.0f / total << "%\n";
}

void Report::save(const wxString& filename)
{
    wxFileOutputStream s(filename);
    if(!s.IsOk()) return;
    wxStringInputStream stream(m_str);
    s << stream;
    s.Close();
}
