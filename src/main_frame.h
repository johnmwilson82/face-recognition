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

class MyApp;

class MainFrame : public MainFrameBase
{
private:
    const std::vector<Classifier*> &m_classifiers;
    FaceObservable* m_selected_fo;
    Classifier* m_selected_classifier;
    uint32_t m_selected;
    void render_image(wxDC &dc);
    void populate_source_selector();
    void update_source_selector();
    MyApp *m_app;

public:
    MainFrame(MyApp* app);

protected:
    virtual void on_paint( wxPaintEvent& event );
    virtual void on_prev( wxCommandEvent& event );
    virtual void on_next( wxCommandEvent& event );
    virtual void on_classifier_selector( wxCommandEvent& event );
    virtual void on_train( wxCommandEvent& event );
    virtual void on_source_select( wxTreeListEvent& event );
    virtual void on_source_checked( wxTreeListEvent& event );
    virtual void on_autoselect_training( wxCommandEvent& event );
    virtual void on_projection_generate( wxCommandEvent& event );
    virtual void on_notebook_page( wxNotebookEvent& event );

};

class MyApp: public wxApp
{
private:
    std::vector<Classifier*> m_classifiers;
    FaceCatalogue m_face_catalogue;
    MLP m_mlp;

    std::shared_ptr<Projector> m_projector;

public:
    MyApp();
    ~MyApp();
    virtual bool OnInit();
    virtual bool OnExceptionInMainLoop();

    void train();
    const std::vector<Classifier*>& get_classifiers() { return m_classifiers; }
    FaceCatalogue& get_face_catalogue() { return m_face_catalogue; }
    template<typename T> void set_projector();
    FaceObservable* get_current_projector() { return dynamic_cast<FaceObservable*> (m_projector.get()); }

};
