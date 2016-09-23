///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 22 2016)
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
	bSizer3->Add( m_draw_panel, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btn_prev = new wxButton( this, wxID_ANY, wxT("Prev"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btn_prev, 1, wxALL, 5 );
	
	m_btn_next = new wxButton( this, wxID_ANY, wxT("Next"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_btn_next, 1, wxALL, 5 );
	
	
	bSizer3->Add( bSizer4, 0, wxEXPAND, 5 );
	
	
	bSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( MainFrameBase::on_init ) );
	this->Connect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::on_paint ) );
	m_btn_prev->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_prev ), NULL, this );
	m_btn_next->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_next ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( MainFrameBase::on_init ) );
	this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( MainFrameBase::on_paint ) );
	m_btn_prev->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_prev ), NULL, this );
	m_btn_next->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( MainFrameBase::on_next ), NULL, this );
	
}