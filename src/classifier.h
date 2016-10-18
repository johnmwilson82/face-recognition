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

using Eigen::MatrixXf;
using Eigen::VectorXf;

#define REGISTER_CLASSIFIER(klass) \
    class klass##Factory : public ClassifierFactory{ \
    public: \
        klass##Factory() \
        { \
            Classifier::register_classifier(#klass, this); \
        } \
        virtual Classifier *create(uint32_t input_size, \
                                   uint32_t output_size, \
                                   const wxPropertyGridInterface& props) const { \
            return new klass(input_size, output_size, props); \
        } \
        virtual wxString get_name() const { \
            return klass::get_name(); \
        } \
        virtual std::vector<std::shared_ptr<wxPGProperty> > get_props() const { \
            return klass::get_props(); \
       } \
    }; \
    static klass##Factory global_##klass##Factory;

class Classifier;

class ClassifierFactory
{
public:
    virtual Classifier *create(uint32_t input_size,
                               uint32_t output_size,
                               const wxPropertyGridInterface& props) const = 0;
    virtual wxString get_name() const = 0;
    virtual std::vector<std::shared_ptr<wxPGProperty>> get_props() const = 0;
};

class Classifier
{
protected:
    static std::map<std::string, ClassifierFactory*> _factories;

public:
    virtual void train(const VectorXf &input, uint32_t exp_class) = 0;
    virtual uint32_t classify(const VectorXf &input) = 0;
    static void register_classifier(const std::string& name, ClassifierFactory *factory)
    {
        _factories[name] = factory;
    }

    static const std::map<std::string, ClassifierFactory*>& get_all_classifiers()
    {
        return _factories;
    }
};
