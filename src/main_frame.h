#include "main_frame_base.h"
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MainFrame : public MainFrameBase
{
public:
    MainFrame() : MainFrameBase(nullptr) {};

protected:
    virtual void on_paint( wxPaintEvent& event );
};

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};
