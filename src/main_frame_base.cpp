///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 17 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "main_frame_base.h"

///////////////////////////////////////////////////////////////////////////

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_draw_panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	bSizer3->Add( m_draw_panel, 2, wxEXPAND | wxALL, 5 );
	
	m_property_view = new wxDataViewListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_property_name = m_property_view->AppendTextColumn( wxT("Name") );
	m_property_value = m_property_view->AppendTextColumn( wxT("Value") );
	bSizer3->Add( m_property_view, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btn_prev = new wxButton( this, wxID_ANY, wxT("Prev"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btn_prev, 1, wxALL, 5 );
	
	m_btn_next = new wxButton( this, wxID_ANY, wxT("Next"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btn_next, 1, wxALL, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook1 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_source_panel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_source_selector = new wxTreeListCtrl( m_source_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_CHECKBOX|wxTL_SINGLE );
	m_source_selector->AppendColumn( wxT("Training Set Selection"), wxCOL_WIDTH_DEFAULT, wxALIGN_LEFT, wxCOL_RESIZABLE );
	
	bSizer11->Add( m_source_selector, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	m_autoselect_training = new wxButton( m_source_panel, wxID_ANY, wxT("Autoselect training"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_autoselect_training, 0, wxALL, 5 );
	
	m_staticText1 = new wxStaticText( m_source_panel, wxID_ANY, wxT("Proportion"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	bSizer8->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_txt_training_prop = new wxTextCtrl( m_source_panel, wxID_ANY, wxT("0.7"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer8->Add( m_txt_training_prop, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer11->Add( bSizer8, 0, wxEXPAND, 5 );
	
	
	m_source_panel->SetSizer( bSizer11 );
	m_source_panel->Layout();
	bSizer11->Fit( m_source_panel );
	m_notebook1->AddPage( m_source_panel, wxT("Source"), true );
	m_projection_panel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_projector_choiceChoices;
	m_projector_choice = new wxChoice( m_projection_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_projector_choiceChoices, 0 );
	m_projector_choice->SetSelection( 0 );
	bSizer9->Add( m_projector_choice, 0, wxALL|wxEXPAND, 5 );
	
	m_projector_propgrid = new wxPropertyGrid(m_projection_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer9->Add( m_projector_propgrid, 1, wxALL|wxEXPAND, 5 );
	
	m_projection_generate = new wxButton( m_projection_panel, wxID_ANY, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer9->Add( m_projection_generate, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_projection_panel->SetSizer( bSizer9 );
	m_projection_panel->Layout();
	bSizer9->Fit( m_projection_panel );
	m_notebook1->AddPage( m_projection_panel, wxT("Projection"), false );
	m_mlp_panel = new wxPanel( m_notebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	wxArrayString m_classifier_choiceChoices;
	m_classifier_choice = new wxChoice( m_mlp_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_classifier_choiceChoices, 0 );
	m_classifier_choice->SetSelection( 0 );
	bSizer12->Add( m_classifier_choice, 0, wxALL|wxEXPAND, 5 );
	
	m_classifier_propgrid = new wxPropertyGrid(m_mlp_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer12->Add( m_classifier_propgrid, 1, wxALL|wxEXPAND, 5 );
	
	m_train = new wxButton( m_mlp_panel, wxID_ANY, wxT("Train"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer12->Add( m_train, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_mlp_panel->SetSizer( bSizer12 );
	m_mlp_panel->Layout();
	bSizer12->Fit( m_mlp_panel );
	m_notebook1->AddPage( m_mlp_panel, wxT("MLP"), false );
	
	bSizer10->Add( m_notebook1, 1, wxEXPAND | wxALL, 5 );
	
	
	bSizer2->Add( bSizer10, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( MainFrameBase::on_init ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::on_paint ) );
	m_btn_prev->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_prev ), NULL, this );
	m_btn_next->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_next ), NULL, this );
	m_notebook1->Connect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MainFrameBase::on_notebook_page ), NULL, this );
	m_source_selector->Connect( wxEVT_TREELIST_ITEM_CHECKED, wxTreeListEventHandler( MainFrameBase::on_source_checked ), NULL, this );
	m_source_selector->Connect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( MainFrameBase::on_source_select ), NULL, this );
	m_autoselect_training->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_autoselect_training ), NULL, this );
	m_projector_choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrameBase::on_projector_choice ), NULL, this );
	m_projector_propgrid->Connect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( MainFrameBase::on_projection_propgrid ), NULL, this );
	m_projection_generate->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_projection_generate ), NULL, this );
	m_classifier_choice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrameBase::on_classifier_choice ), NULL, this );
	m_train->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_train ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( MainFrameBase::on_init ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::on_paint ) );
	m_btn_prev->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_prev ), NULL, this );
	m_btn_next->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_next ), NULL, this );
	m_notebook1->Disconnect( wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( MainFrameBase::on_notebook_page ), NULL, this );
	m_source_selector->Disconnect( wxEVT_TREELIST_ITEM_CHECKED, wxTreeListEventHandler( MainFrameBase::on_source_checked ), NULL, this );
	m_source_selector->Disconnect( wxEVT_TREELIST_SELECTION_CHANGED, wxTreeListEventHandler( MainFrameBase::on_source_select ), NULL, this );
	m_autoselect_training->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_autoselect_training ), NULL, this );
	m_projector_choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrameBase::on_projector_choice ), NULL, this );
	m_projector_propgrid->Disconnect( wxEVT_PG_CHANGED, wxPropertyGridEventHandler( MainFrameBase::on_projection_propgrid ), NULL, this );
	m_projection_generate->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_projection_generate ), NULL, this );
	m_classifier_choice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MainFrameBase::on_classifier_choice ), NULL, this );
	m_train->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_train ), NULL, this );
	
}
