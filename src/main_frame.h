#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main_frame_base.h"
#include "img_mgr.h"
#include "eigenfaces.h"
#include "mlp.h"

class AppIF
{
public:
    virtual void train() = 0;
    virtual const std::vector<FaceObservable*>& get_fos() = 0;
    virtual const std::vector<Classifier*>& get_classifiers() = 0;
};

class MainFrame : public MainFrameBase
{
private:
    const std::vector<FaceObservable*> &m_fos;
    const std::vector<Classifier*> &m_classifiers;
    FaceObservable* m_selected_fo;
    Classifier* m_selected_classifier;
    uint32_t m_selected;
    void render_image(wxDC &dc);
    AppIF *m_app;

public:
    MainFrame(AppIF* app);

protected:
    virtual void on_paint( wxPaintEvent& event );
    virtual void on_prev( wxCommandEvent& event );
    virtual void on_next( wxCommandEvent& event );
    virtual void on_fo_selection( wxCommandEvent& event );
    virtual void on_classifier_selector( wxCommandEvent& event );
    virtual void on_train( wxCommandEvent& event );
};

class MyApp: public wxApp, public AppIF
{
private:
    std::vector<FaceObservable*> m_fos;
    std::vector<Classifier*> m_classifiers;
    FaceCatalogue m_face_catalogue;
    MLP m_mlp;

public:
    MyApp();
    ~MyApp();
    virtual bool OnInit();
    virtual void train();
    virtual const std::vector<FaceObservable*>& get_fos() { return m_fos; }
    virtual const std::vector<Classifier*>& get_classifiers() { return m_classifiers; }
};
