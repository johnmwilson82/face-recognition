#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main_frame_base.h"
#include "img_mgr.h"
#include "eigenfaces.h"
#include "mlp.h"
#include <memory>
#include <list>

class MyApp;

class MainFrame : public MainFrameBase
{
private:
    FaceObservable* m_selected_fo;
    uint32_t m_selected;
    void render_image(wxDC &dc);
    void populate_source_selector();
    void update_source_selector();
    void set_projector_dimensions();
    void set_classifier_props();
    void set_projector_props();

    MyApp *m_app;

    std::list<std::shared_ptr<wxPGProperty> > m_classifier_props;
    std::list<std::shared_ptr<wxPGProperty> > m_projector_props;

public:
    MainFrame(MyApp* app);
    void append_classifier(ClassifierFactory* factory);
    void append_projector(ProjectorFactory* factory);
    void user_init();

protected:
    virtual void on_paint( wxPaintEvent& event );
    virtual void on_prev( wxCommandEvent& event );
    virtual void on_next( wxCommandEvent& event );
    virtual void on_train( wxCommandEvent& event );
    virtual void on_source_select( wxTreeListEvent& event );
    virtual void on_source_checked( wxTreeListEvent& event );
    virtual void on_autoselect_training( wxCommandEvent& event );
    virtual void on_projection_generate( wxCommandEvent& event );
    virtual void on_notebook_page( wxNotebookEvent& event );
    virtual void on_classifier_choice( wxCommandEvent& event );
    virtual void on_projector_choice( wxCommandEvent& event );
    virtual void on_projection_propgrid( wxPropertyGridEvent& event );

};

class MyApp: public wxApp
{
private:
    FaceCatalogue m_face_catalogue;

    std::shared_ptr<Projector> m_projector;

    ClassifierFactory* m_classifier_factory;
    ProjectorFactory* m_projector_factory;

    std::vector<std::shared_ptr<ClassifierFactory> > m_classifier_factories;

public:
    MyApp();
    ~MyApp();
    virtual bool OnInit();
    virtual bool OnExceptionInMainLoop();

    void train(const wxPropertyGridInterface& props);
    FaceCatalogue& get_face_catalogue() { return m_face_catalogue; }
    void set_projector(const wxPropertyGridInterface& props);
    Projector* get_current_projector() { return m_projector ? m_projector.get() : nullptr; }

    void set_classifier_factory(ClassifierFactory* factory) { m_classifier_factory = factory; }
    void set_projector_factory(ProjectorFactory* factory);
};
