#include "main_frame.h"
#include "img_mgr.h"

void MainFrame::on_paint( wxPaintEvent& event )
{
    FaceImage fi(std::string("/home/john/git/face-recognition/data/orl_faces/s1/1.pgm"), 0);
    uint8_t *buf;
    uint32_t size;
    fi.to_rgb_buffer(&buf, &size);
    wxImage im(wxSize(fi.get_width(), fi.get_height()), buf);
    //delete buf;

    wxBitmap bm(im);
    wxClientDC dc(m_draw_panel);
    dc.DrawBitmap( bm, 0, 0, false);
}

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
