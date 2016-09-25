#include "main_frame.h"

void MainFrame::on_paint( wxPaintEvent& event )
{
    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::on_prev( wxCommandEvent& event )
{
    if (m_selected == 0) m_selected = m_selected_fo->get_num_faces();
    m_selected--;

    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::on_next( wxCommandEvent& event )
{
    m_selected++;
    if (m_selected >= m_selected_fo->get_num_faces()) m_selected = 0;

    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::render_image(wxDC &dc)
{
    const FaceImage &fi = m_selected_fo->get_face(m_selected);
    std::unique_ptr<uint8_t[]> buf = fi.to_rgb_buffer();
    wxImage im(wxSize(fi.get_width(), fi.get_height()), buf.release());
    wxBitmap bm(im);
    dc.DrawBitmap(bm, 0, 0, false);
    m_txtPicInfo->SetLabelText(m_selected_fo->get_info(m_selected));
}

MainFrame::MainFrame(const std::vector<FaceObservable*> &fos) :
        MainFrameBase(nullptr),
        m_fos(fos),
        m_selected(0)
{
    for (auto fo : fos)
    {
        m_fo_selector->Append(fo->get_name());
    }
    m_fo_selector->SetSelection(m_selected);
    m_selected_fo = m_fos[m_selected];
}

void MainFrame::on_fo_selection( wxCommandEvent& event )
{
    int i = m_fo_selector->GetSelection();
    m_selected_fo = m_fos[i];
    m_selected = 0;
}

MyApp::MyApp() :
    m_face_catalogue("/home/john/git/face-recognition/data/orl_faces_test")
{
    m_face_catalogue.choose_training_sets(0.7);
    EigenFaces *eigenfaces = new EigenFaces(m_face_catalogue);
    m_fos.push_back(&m_face_catalogue);
    m_fos.push_back(eigenfaces);
}

MyApp::~MyApp()
{
    for (int i = 1; i < m_fos.size(); i++)
    {
        delete m_fos[i];
    }
}

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame(m_fos);
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);
