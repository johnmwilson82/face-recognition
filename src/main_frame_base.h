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
#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxDialog 
{
	private:
	
	protected:
		wxChoice* m_fo_selector;
		wxPanel* m_draw_panel;
		wxStaticText* m_txtPicInfo;
		wxButton* m_btn_prev;
		wxButton* m_btn_next;
		
		// Virtual event handlers, overide them in your derived class
		virtual void on_init( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void on_paint( wxPaintEvent& event ) { event.Skip(); }
		virtual void on_fo_selection( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_prev( wxCommandEvent& event ) { event.Skip(); }
		virtual void on_next( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 285,463 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~MainFrameBase();
	
};

#endif //__MAIN_FRAME_BASE_H__
