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
    wxImage im(wxSize(fi.get_width(), fi.get_height()), buf.get(), true);
    wxBitmap bm(im);
    dc.DrawBitmap(bm, 0, 0, false);
    m_txtPicInfo->SetLabelText(m_selected_fo->get_info(&fi));
}

MainFrame::MainFrame(AppIF *app) :
        MainFrameBase(nullptr),
        m_app(app),
        m_fos(app->get_fos()),
        m_classifiers(app->get_classifiers()),
        m_selected(0)
{
    for (auto fo : m_fos)
    {
        m_fo_selector->Append(fo->get_name());
    }
    for (auto classifier : m_classifiers)
    {
        m_classifier_selector->Append(classifier->get_name());
    }
    m_fo_selector->SetSelection(0);
    m_classifier_selector->SetSelection(0);
    m_selected_fo = m_fos[0];
}

void MainFrame::on_fo_selection( wxCommandEvent& event )
{
    int i = m_fo_selector->GetSelection();
    m_selected_fo = m_fos[i];
    m_selected = 0;
}

void MainFrame::on_classifier_selector( wxCommandEvent& event )
{
    int i = m_classifier_selector->GetSelection();
    m_selected_classifier = m_classifiers[i];
}

void MainFrame::on_train( wxCommandEvent& event )
{
    m_app->train();
}

MyApp::MyApp() :
    m_face_catalogue("/home/john/git/face-recognition/data/orl_faces_test"),
    m_mlp({10, 12, m_face_catalogue.get_num_classes()})
{
    m_face_catalogue.choose_training_sets(0.7);
    EigenFaces *eigenfaces = new EigenFaces(m_face_catalogue);
    m_fos.push_back(&m_face_catalogue);
    m_fos.push_back(eigenfaces);
    m_classifiers.push_back(&m_mlp);
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
    MainFrame* frame = new MainFrame(this);
    frame->Show(true);
    return true;
}

void MyApp::train()
{
    printf("Training...\n");
    for (int n = 0; n < 1000; n++)
    {
        for (uint32_t c = 0; c < m_face_catalogue.get_num_classes(); c++)
        {
            for (auto im : m_face_catalogue.get_set_of_class(c, FaceCatalogue::TRAINING_SET))
            {
                m_mlp.train(dynamic_cast<EigenFaces*>(m_fos[1])->project(*im, 10), c);
            }
        }
    }
    printf("Trained\n");

    printf("Testing...\n");
    for (uint32_t c = 0; c < m_face_catalogue.get_num_classes(); c++)
    {
        printf("Class %u...\n", c);
        for (auto im : m_face_catalogue.get_set_of_class(c, FaceCatalogue::TEST_SET))
        {
            uint32_t detected = m_mlp.classify(dynamic_cast<EigenFaces*>(m_fos[1])->project(*im, 10));
            printf("Detected %u\n", detected);
        }
    }
}

wxIMPLEMENT_APP(MyApp);
