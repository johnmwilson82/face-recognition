#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <string>
#include <map>
#include <eigen3/Eigen/Dense>
#include <wx/propgrid/property.h>
#include <wx/propgrid/props.h>
#include <wx/propgrid/propgridiface.h>
#include <memory>

#include "img_mgr.h"


#define REGISTER_PROJECTOR(klass) \
    class klass##Factory : public ProjectorFactory{ \
    public: \
        klass##Factory() \
        { \
            Projector::register_projector(#klass, this); \
        } \
        virtual Projector *create(const FaceCatalogue &fc, const wxPropertyGridInterface& props) const { \
            return new klass(fc, props); \
        } \
        virtual wxString get_name() const { \
            return klass::get_name(); \
        } \
        virtual std::vector<std::shared_ptr<wxPGProperty> > get_props() const { \
            return klass::get_props(); \
       } \
    }; \
    static klass##Factory global_##klass##Factory;

class Projector;

class ProjectorFactory
{
public:
    virtual Projector *create(const FaceCatalogue &fc, const wxPropertyGridInterface& props) const = 0;
    virtual wxString get_name() const = 0;
    virtual std::vector<std::shared_ptr<wxPGProperty>> get_props() const = 0;
    //virtual const wxPGAttributeStorage& get_props() = 0;
};

class Projector
{
protected:
    const FaceCatalogue &m_fc;
    uint32_t m_noutput_dims;
    static std::map<std::string, ProjectorFactory*> _factories;

public:
    virtual VectorXf project(const FaceImage& im, uint32_t dimensionality) = 0;
    virtual VectorXf project(const FaceImage& im) { return project(im, m_noutput_dims); }
    void set_noutput_dims(uint32_t noutput_dims) { printf("Set noutput dims = %u\n", noutput_dims); m_noutput_dims = noutput_dims; }

    static void register_projector(const std::string& name, ProjectorFactory *factory)
    {
        _factories[name] = factory;
    }

    static const std::map<std::string, ProjectorFactory*>& get_all_projectors()
    {
        return _factories;
    }

    uint32_t get_noutput_dims() { return m_noutput_dims; }
    Projector(const FaceCatalogue &fc, const wxPropertyGridInterface& props);

    virtual bool update_props(const wxPropertyGridInterface& props);

    static std::vector<std::shared_ptr<wxPGProperty> > get_props();

    virtual const MatrixXf& get_projection_matrix() const = 0;

    virtual ~Projector() {}
};
