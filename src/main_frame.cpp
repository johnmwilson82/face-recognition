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
        m_selected(0)
{
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

void MainFrame::on_train( wxCommandEvent& event )
{
    m_app->train(*m_classifier_propgrid);
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
    set_projector_dimensions();
    m_selected_fo = dynamic_cast<FaceObservable*> (m_app->get_current_projector());
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
        m_selected_fo = dynamic_cast<FaceObservable*> (m_app->get_current_projector());
        m_selected = 0;
        break;

    default:
        m_selected_fo = &(m_app->get_face_catalogue());
        m_selected = 0;
        break;
    }
}

void MainFrame::on_dimensions_enter( wxCommandEvent& event )
{
    set_projector_dimensions();
}

void MainFrame::set_projector_dimensions()
{
    if(!m_app->get_current_projector()) return;
    unsigned long ndims;
    m_txt_projection_dimensions->GetValue().ToULong(&ndims);
    m_app->get_current_projector()->set_noutput_dims((uint32_t) ndims);
}

void MainFrame::append_classifier(ClassifierFactory* factory)
{
    m_classifier_choice->Append(factory->get_name(), (wxClientData*) factory);
}

void MainFrame::on_classifier_choice( wxCommandEvent& event )
{
    set_classifier_props();
}

void MainFrame::set_classifier_props()
{
    ClassifierFactory* factory = reinterpret_cast<ClassifierFactory*>(
        m_classifier_choice->GetClientObject(
            m_classifier_choice->GetSelection()));

    auto props = factory->get_props();
    m_classifier_props.clear();
    for (auto p: props)
    {
        m_classifier_props.push_back(p);
        m_classifier_propgrid->Append(p.get());
    }
}

void MainFrame::user_init()
{
    m_classifier_choice->SetSelection(0);
    set_classifier_props();
}

MyApp::MyApp() :
    m_face_catalogue("/home/john/git/face-recognition/data/orl_faces_test")
{
    m_face_catalogue.autoselect_training_sets(0.7);

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
    for (auto c : Classifier::get_all_classifiers())
    {
        frame->append_classifier(c.second);
    }
    frame->Show(true);
    frame->user_init();

    return true;
}

bool MyApp::OnExceptionInMainLoop()
{
    wxString error;
    try {
        throw;
    } catch (const std::exception& e) {
        error = e.what();
    } catch ( ... ) {
        error = "unknown error.";
    }
    wxLogError("Unexpected exception has occurred: %s, the program will terminate.", error);
    return false;
}

void MyApp::train(const wxPropertyGridInterface& props)
{
    MLP mlp(get_current_projector()->get_noutput_dims(),
            m_face_catalogue.get_num_classes(),
            props);

    printf("Training...\n");
    int num_iterations = props.GetPropertyByName(wxT("propIterations"))->GetValue().GetLong();
    for (int n = 0; n < num_iterations; n++)
    {
        for (uint32_t c = 0; c < m_face_catalogue.get_num_classes(); c++)
        {
            for (auto im : m_face_catalogue.get_set_of_class(c, FaceCatalogue::TRAINING_SET))
            {
                mlp.train(m_projector->project(*im), c);
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
            uint32_t detected = mlp.classify(m_projector->project(*im));
            printf("Detected %u\n", detected);
        }
    }
}

wxIMPLEMENT_APP(MyApp);
