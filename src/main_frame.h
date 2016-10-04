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

    MyApp *m_app;

    std::list<std::shared_ptr<wxPGProperty> > m_classifier_props;

public:
    MainFrame(MyApp* app);
    void append_classifier(ClassifierFactory* factory);
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
    virtual void on_dimensions_enter( wxCommandEvent& event );
    virtual void on_classifier_choice( wxCommandEvent& event );

};

class MyApp: public wxApp
{
private:
    FaceCatalogue m_face_catalogue;

    std::shared_ptr<Projector> m_projector;
    std::vector<std::shared_ptr<ClassifierFactory> > m_classifier_factories;

public:
    MyApp();
    ~MyApp();
    virtual bool OnInit();
    virtual bool OnExceptionInMainLoop();

    void train(const wxPropertyGridInterface& props);
    FaceCatalogue& get_face_catalogue() { return m_face_catalogue; }
    template<typename T> void set_projector();
    Projector* get_current_projector() { return m_projector.get(); }

};
