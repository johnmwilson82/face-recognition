///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  9 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __MAIN_FRAME_BASE_H__
#define __MAIN_FRAME_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/dataview.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/treelist.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/choice.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_draw_panel;
		wxDataViewListCtrl* m_property_view;
		wxDataViewColumn* m_property_name;
		wxDataViewColumn* m_property_value;
		wxButton* m_btn_prev;
		wxButton* m_btn_next;
		wxNotebook* m_notebook1;
		wxPanel* m_source_panel;
		wxTreeListCtrl* m_source_selector;
		wxButton* m_autoselect_training;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_txt_training_prop;
		wxPanel* m_projection_panel;
		wxStaticText* m_staticText2;
		wxTextCtrl* m_txt_projection_dimensions;
		wxButton* m_btn_projection_generate;
		wxPanel* m_mlp_panel;
		wxChoice* m_classifier_choice;
		wxPropertyGrid* m_classifier_propgrid;
		wxButton* m_train;
		
		// Virtual event handlers, overide them in your derived class
		virtual void on_init( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void on_paint( wxPaintEvent& event ) { event.Skip(); }
		virtual void on_prev( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_next( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_notebook_page( wxNotebookEvent& event ) { event.Skip(); }
		virtual void on_source_checked( wxTreeListEvent& event ) { event.Skip(); }
		virtual void on_source_select( wxTreeListEvent& event ) { event.Skip(); }
		virtual void on_autoselect_training( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_dimensions_enter( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_projection_generate( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_classifier_choice( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_train( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 662,463 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~MainFrameBase();
	
};

#endif //__MAIN_FRAME_BASE_H__
