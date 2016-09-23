#pragma once
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "main_frame_base.h"
#include "img_mgr.h"
#include "eigenfaces.h"

class MainFrame : public MainFrameBase
{
private:
    const std::vector<FaceObservable*> &m_fos;
    FaceObservable* m_selected_fo;
    uint32_t m_selected;
    void render_image(wxDC &dc);

public:
    MainFrame(const std::vector<FaceObservable*> &fos);

protected:
    virtual void on_paint( wxPaintEvent& event );
    virtual void on_prev( wxCommandEvent& event );
    virtual void on_next( wxCommandEvent& event );
    virtual void on_fo_selection( wxCommandEvent& event );
};

class MyApp: public wxApp
{
private:
    std::vector<FaceObservable*> m_fos;
    FaceCatalogue m_face_catalogue;

public:
    MyApp();
    ~MyApp();
    virtual bool OnInit();
};
