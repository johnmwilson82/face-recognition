#include "projector.h"

Projector::Projector(const FaceCatalogue &fc, const wxPropertyGridInterface& props) :
    m_fc(fc)
{
    update_props(props);
}

std::vector<std::shared_ptr<wxPGProperty> > Projector::get_props()
{
    auto prop1 = std::make_shared<wxIntProperty>(wxT("Output size"), wxT("propOutputSize"));
    prop1->SetValue(10);
    return {prop1};
}

bool Projector::update_props(const wxPropertyGridInterface& props)
{
    m_noutput_dims = props.GetPropertyByName(wxT("propOutputSize"))->GetValue().GetLong();
    printf("set noutput_dims = %d\n", m_noutput_dims);
}

std::map<std::string, ProjectorFactory*> Projector::_factories;
