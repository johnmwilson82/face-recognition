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
    const FaceCatalogue &m_face_catalogue;
    const EigenFaces &m_eigenfaces;
    uint32_t m_selected;
    void render_image(wxDC &dc);

public:
    MainFrame(const FaceCatalogue &fc, const EigenFaces &ef) : 
        MainFrameBase(nullptr),
        m_face_catalogue(fc),
        m_eigenfaces(ef),
        m_selected(0) {};

protected:
    virtual void on_paint( wxPaintEvent& event );
    virtual void on_prev( wxCommandEvent& event );
    virtual void on_next( wxCommandEvent& event );
};

class MyApp: public wxApp
{
private:
    FaceCatalogue m_face_catalogue;
    EigenFaces *m_eigenfaces;

public:
    MyApp();
    ~MyApp();
    virtual bool OnInit();
};
