#include "main_frame.h"

void MainFrame::on_paint( wxPaintEvent& event )
{
    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::on_prev( wxCommandEvent& event )
{
    m_selected--;
    //if (m_selected < 0) m_selected = m_face_catalogue.get_num_faces() - 1;
    if (m_selected < 0) m_selected = m_eigenfaces.get_num_faces() - 1;
    
    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::on_next( wxCommandEvent& event )
{
    m_selected++;
    //if (m_selected >= m_face_catalogue.get_num_faces()) m_selected = 0;
    if (m_selected >= m_eigenfaces.get_num_faces()) m_selected = 0;
    
    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::render_image(wxDC &dc)
{
    //const FaceImage &fi = m_face_catalogue.get_face(m_selected);
    const FaceImage &fi = m_eigenfaces.get_face(m_selected);
    std::unique_ptr<uint8_t[]> buf = fi.to_rgb_buffer();
    wxImage im(wxSize(fi.get_width(), fi.get_height()), buf.release());
    wxBitmap bm(im);
    dc.DrawBitmap(bm, 0, 0, false);
}

MyApp::MyApp() :
    m_face_catalogue("/home/john/git/face-recognition/data/orl_faces")
{
    m_face_catalogue.choose_training_sets(0.7);
    m_eigenfaces = new EigenFaces(m_face_catalogue);
} 

MyApp::~MyApp()
{
    delete m_eigenfaces;
}

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame(m_face_catalogue, *m_eigenfaces);
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
