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
    if(m_selected_fo == nullptr)
        return;
    const FaceImage &fi = m_selected_fo->get_face(m_selected);
    std::unique_ptr<uint8_t[]> buf = fi.to_rgb_buffer();
    wxImage im(wxSize(fi.get_width(), fi.get_height()), buf.get(), true);
    wxBitmap bm(im);
    dc.DrawBitmap(bm, 0, 0, false);
    m_property_view->DeleteAllItems();
    for (auto item : m_selected_fo->get_info(m_selected))
    {
        m_property_view->AppendItem(item);
    }
}

void MainFrame::update_source_selector()
{
    const FaceCatalogue& fc = m_app->get_face_catalogue();

    for (wxTreeListItem item = m_source_selector->GetFirstItem();
         item.IsOk();
         item = m_source_selector->GetNextItem(item))
    {
        uint32_t index = reinterpret_cast<FaceImage*>(
                m_source_selector->GetItemData(item))->get_index();
        if (fc.get_info(index, "Train set").GetString().IsSameAs("Yes", false))
        {
            m_source_selector->CheckItem(item);
        }else
        {
            m_source_selector->CheckItem(item, wxCHK_UNCHECKED);
        }
    }
    for (wxTreeListItem item = m_source_selector->GetFirstItem();
         item.IsOk();
         item = m_source_selector->GetNextSibling(item))
    {
        if(m_source_selector->AreAllChildrenInState(item, wxCHK_UNCHECKED))
        {
            m_source_selector->CheckItem(item, wxCHK_UNCHECKED);
        }
        else
        {
            m_source_selector->CheckItem(item, wxCHK_CHECKED);
        }
    }
}

MainFrame::MainFrame(MyApp *app) :
        MainFrameBase(nullptr),
        m_app(app),
        m_classifiers(app->get_classifiers()),
        m_selected(0)
{
    for (auto classifier : m_classifiers)
    {
        m_classifier_selector->Append(classifier->get_name());
    }
    m_classifier_selector->SetSelection(0);
    m_selected_fo = &(m_app->get_face_catalogue());

    populate_source_selector();
    update_source_selector();
}

void MainFrame::populate_source_selector()
{
    const FaceCatalogue& fc = m_app->get_face_catalogue();
    for (int c = 0; c < fc.get_num_classes(); c++)
    {
        wxTreeListItem p = m_source_selector->AppendItem(
                m_source_selector->GetRootItem(),
                wxString::Format("Class %u", c),
                wxTreeListCtrl::NO_IMAGE,
                wxTreeListCtrl::NO_IMAGE,
                reinterpret_cast<wxClientData*> (fc.get_set_of_class(c)[0]));
        int i = 0;
        for (auto fi : fc.get_set_of_class(c))
        {

            m_source_selector->AppendItem(
                p,
                wxString::Format("Face %u", i++),
                wxTreeListCtrl::NO_IMAGE,
                wxTreeListCtrl::NO_IMAGE,
                reinterpret_cast<wxClientData*> (fi));
        }
    }
}


void MainFrame::on_source_select( wxTreeListEvent& event )
{
    auto item_data = m_source_selector->GetItemData(event.GetItem());
    if (!item_data) return;
    m_selected = reinterpret_cast<FaceImage*> (item_data)->get_index();
    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::on_source_checked( wxTreeListEvent& event )
{
    // Not implemented
    update_source_selector();
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

void MainFrame::on_autoselect_training( wxCommandEvent& event )
{
    wxString str = m_txt_training_prop->GetValue();
    double prop;
    if(str.ToDouble(&prop))
    {
        FaceCatalogue& fc = m_app->get_face_catalogue();
        fc.autoselect_training_sets(prop);
    }
    update_source_selector();
}

void MainFrame::on_projection_generate( wxCommandEvent& event )
{
    m_app->set_projector<EigenFaces>();
    m_selected_fo = m_app->get_current_projector();
    m_selected = 0;
    wxClientDC dc(m_draw_panel);
    render_image(dc);
}

void MainFrame::on_notebook_page( wxNotebookEvent& event )
{
    switch(event.GetSelection())
    {
    case 0: // Source
        m_selected_fo = &(m_app->get_face_catalogue());
        m_selected = 0;
        break;
    case 1: // Projector
        m_selected_fo = m_app->get_current_projector();
        m_selected = 0;
        break;

    default:
        m_selected_fo = &(m_app->get_face_catalogue());
        m_selected = 0;
        break;
    }
}

MyApp::MyApp() :
    m_face_catalogue("/home/john/git/face-recognition/data/orl_faces_test"),
    m_mlp({10, 12, m_face_catalogue.get_num_classes()})
{
    m_face_catalogue.autoselect_training_sets(0.7);
    m_classifiers.push_back(&m_mlp);
}

MyApp::~MyApp()
{
}

template<typename T> void MyApp::set_projector()
{
    m_projector = std::shared_ptr<Projector>(new EigenFaces(m_face_catalogue));
}

bool MyApp::OnInit()
{
    MainFrame* frame = new MainFrame(this);
    frame->Show(true);

    return true;
}

bool MyApp::OnExceptionInMainLoop()
{
    wxString error;
    try {
        throw; // Rethrow the current exception.
    } catch (const std::exception& e) {
        error = e.what();
    } catch ( ... ) {
        error = "unknown error.";
    }
    wxLogError("Unexpected exception has occurred: %s, the program will terminate.", error);
    // Exit the main loop and thus terminate the program.
    return false;
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
                m_mlp.train(m_projector->project(*im, 10), c);
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
            uint32_t detected = m_mlp.classify(m_projector->project(*im, 10));
            printf("Detected %u\n", detected);
        }
    }
}

wxIMPLEMENT_APP(MyApp);
