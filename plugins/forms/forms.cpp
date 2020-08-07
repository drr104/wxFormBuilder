///////////////////////////////////////////////////////////////////////////////
//
// wxFormBuilder - A Visual Dialog Editor for wxWidgets.
// Copyright (C) 2005 José Antonio Hurtado
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// Written by
//   José Antonio Hurtado - joseantonio.hurtado@gmail.com
//   Juan Antonio Ortega  - jortegalalmolda@gmail.com
//
// Wizard component written by
//   Andrea Zanellato - zanellato.andrea@gmail.com
//
///////////////////////////////////////////////////////////////////////////////
#include <plugin.h>
#include <xrcconv.h>

#include <forms/wizard.h>
#include <wx/aui/aui.h>

// Event handler for events generated by controls in this plugin
class ComponentEvtHandler : public wxEvtHandler
{
private:
	IManager* m_manager;

public:
	ComponentEvtHandler(IManager* manager) : m_manager(manager) {
	}

protected:
	void OnTool( wxCommandEvent& event );

	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE( ComponentEvtHandler, wxEvtHandler )
	EVT_TOOL( wxID_ANY, ComponentEvtHandler::OnTool )
END_EVENT_TABLE()

// TO-DO: The "Form" type component should be drawn in the designer, so that,
//        for instance, a dark panel could be drawn for a wxFrame (N.B. ??)
class FrameFormComponent : public ComponentBase
{
public:
	wxObject* Create(IObject* /*obj*/, wxObject* parent) override {
		wxPanel *panel = new wxPanel((wxWindow *)parent, wxID_ANY);
		panel->SetBackgroundColour(wxColour(50,50,50));
		return panel;
	}

	ticpp::Element* ExportToXrc(IObject* obj) override {
		ObjectToXrcFilter xrc(obj, wxT("wxFrame"), obj->GetPropertyAsString(wxT("name")));
		xrc.AddWindowProperties();
		xrc.AddProperty( wxT("title"), wxT("title"), XRC_TYPE_TEXT);
		if ( !obj->IsNull( wxT("center") ) )
		{
			xrc.AddPropertyValue( wxT("centered"), wxT("1") );
		}
		xrc.AddProperty( wxT("aui_managed"), wxT("aui_managed"), XRC_TYPE_BOOL);
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc(ticpp::Element* xrcObj) override {
		XrcToXfbFilter filter(xrcObj, wxT("Frame"));
		filter.AddWindowProperties();
		filter.AddProperty( wxT("title"), wxT("title"), XRC_TYPE_TEXT);
		filter.AddProperty(wxT("centered"), wxT("center"), XRC_TYPE_BITLIST);
		filter.AddProperty( wxT("aui_managed"), wxT("aui_managed"), XRC_TYPE_BOOL);
		return filter.GetXfbObject();
	}
};

class PanelFormComponent : public ComponentBase
{
public:
	wxObject* Create(IObject* /*obj*/, wxObject* parent) override {
		wxPanel *panel = new wxPanel((wxWindow *)parent, wxID_ANY);
		return panel;
	}

	ticpp::Element* ExportToXrc(IObject* obj) override {
		ObjectToXrcFilter xrc(obj, wxT("wxPanel"), obj->GetPropertyAsString(wxT("name")));
		xrc.AddWindowProperties();
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc(ticpp::Element* xrcObj) override {
		XrcToXfbFilter filter(xrcObj, wxT("Panel"));
		filter.AddWindowProperties();
		return filter.GetXfbObject();
	}
};

class DialogFormComponent : public ComponentBase
{
public:
	wxObject* Create(IObject* /*obj*/, wxObject* parent) override {
		wxPanel *panel = new wxPanel((wxWindow *)parent, wxID_ANY);
		return panel;
	}

	ticpp::Element* ExportToXrc(IObject* obj) override {
		ObjectToXrcFilter xrc(obj, wxT("wxDialog"), obj->GetPropertyAsString(wxT("name")));
		xrc.AddWindowProperties();
		xrc.AddProperty( wxT("title"), wxT("title"), XRC_TYPE_TEXT);
		if ( !obj->IsNull( wxT("center") ) )
		{
			xrc.AddPropertyValue( wxT("centered"), wxT("1") );
		}
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc(ticpp::Element* xrcObj) override {
		XrcToXfbFilter filter(xrcObj, wxT("Dialog"));
		filter.AddWindowProperties();
		filter.AddProperty( wxT("title"), wxT("title"), XRC_TYPE_TEXT);
		filter.AddProperty(wxT("centered"), wxT("center"), XRC_TYPE_BITLIST);
		return filter.GetXfbObject();
	}
};

class MenuBarFormComponent : public ComponentBase
{
public:
	wxObject* Create(IObject* obj, wxObject* /*parent*/) override {
		wxMenuBar *mb = new wxMenuBar(obj->GetPropertyAsInteger(wxT("style")) |
			obj->GetPropertyAsInteger(wxT("window_style")));
		return mb;
	}

	ticpp::Element* ExportToXrc(IObject* obj) override {
		ObjectToXrcFilter xrc(obj, wxT("wxMenuBar"), obj->GetPropertyAsString(wxT("name")));
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc(ticpp::Element* xrcObj) override {
		XrcToXfbFilter filter(xrcObj, wxT("MenuBar"));
		return filter.GetXfbObject();
	}
};

class ToolBarFormComponent : public ComponentBase
{
public:
	wxObject* Create(IObject* obj, wxObject* parent) override {
		wxToolBar *tb = new wxToolBar((wxWindow*)parent, wxID_ANY,
			obj->GetPropertyAsPoint(wxT("pos")),
			obj->GetPropertyAsSize(wxT("size")),
			obj->GetPropertyAsInteger(wxT("style")) | obj->GetPropertyAsInteger(wxT("window_style")) | wxTB_NOALIGN | wxTB_NODIVIDER | wxNO_BORDER);

		if (!obj->IsNull(wxT("bitmapsize")))
			tb->SetToolBitmapSize(obj->GetPropertyAsSize(wxT("bitmapsize")));
		if (!obj->IsNull(wxT("margins")))
		{
			wxSize margins(obj->GetPropertyAsSize(wxT("margins")));
			tb->SetMargins(margins.GetWidth(), margins.GetHeight());
		}
		if (!obj->IsNull(wxT("packing")))
			tb->SetToolPacking(obj->GetPropertyAsInteger(wxT("packing")));
		if (!obj->IsNull(wxT("separation")))
			tb->SetToolSeparation(obj->GetPropertyAsInteger(wxT("separation")));

		tb->PushEventHandler(new ComponentEvtHandler(GetManager()));

		return tb;
	}

	void Cleanup(wxObject* obj) override
	{
		auto* window = wxDynamicCast(obj, wxToolBar);
		if (window)
		{
			window->PopEventHandler(true);
		}
	}

	void OnCreated(wxObject* wxobject, wxWindow* /*wxparent*/) override {
		wxToolBar* tb = wxDynamicCast( wxobject, wxToolBar );
		if ( NULL == tb )
		{
			// very very strange
			return;
		}

		size_t count = GetManager()->GetChildCount( wxobject );
		for ( size_t i = 0; i < count; ++i )
		{
			wxObject* child = GetManager()->GetChild( wxobject, i );
			IObject* childObj = GetManager()->GetIObject( child );
			if ( wxT("tool") == childObj->GetClassName() )
			{
				tb->AddTool( 	wxID_ANY,
								childObj->GetPropertyAsString( wxT("label") ),
								childObj->GetPropertyAsBitmap( wxT("bitmap") ),
								wxNullBitmap,
								(wxItemKind)childObj->GetPropertyAsInteger( wxT("kind") ),
								childObj->GetPropertyAsString( wxT("help") ),
								wxEmptyString,
								child
							);
			}
			else if ( wxT("toolSeparator") == childObj->GetClassName() )
			{
				tb->AddSeparator();
			}
			else
			{
				wxControl* control = wxDynamicCast( child, wxControl );
				if ( NULL != control )
				{
					tb->AddControl( control );
				}
			}
		}
		tb->Realize();

	}

	ticpp::Element* ExportToXrc(IObject* obj) override {
		ObjectToXrcFilter xrc(obj, wxT("wxToolBar"), obj->GetPropertyAsString(wxT("name")));
		xrc.AddWindowProperties();
		xrc.AddProperty(wxT("bitmapsize"), wxT("bitmapsize"), XRC_TYPE_SIZE);
		xrc.AddProperty(wxT("margins"), wxT("margins"), XRC_TYPE_SIZE);
		xrc.AddProperty(wxT("packing"), wxT("packing"), XRC_TYPE_INTEGER);
		xrc.AddProperty(wxT("separation"), wxT("separation"), XRC_TYPE_INTEGER);
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc(ticpp::Element* xrcObj) override {
		XrcToXfbFilter filter(xrcObj, wxT("ToolBar"));
		filter.AddWindowProperties();
		filter.AddProperty(wxT("bitmapsize"), wxT("bitmapsize"), XRC_TYPE_SIZE);
		filter.AddProperty(wxT("margins"), wxT("margins"), XRC_TYPE_SIZE);
		filter.AddProperty(wxT("packing"), wxT("packing"), XRC_TYPE_INTEGER);
		filter.AddProperty(wxT("separation"), wxT("separation"), XRC_TYPE_INTEGER);
		return filter.GetXfbObject();
	}
};

void ComponentEvtHandler::OnTool( wxCommandEvent& event )
{
	//FIXME: Same as above

	wxToolBar* tb = wxDynamicCast( event.GetEventObject(), wxToolBar );
	if ( NULL == tb )
	{
		// very very strange
		return;
	}

	wxObject* wxobject = tb->GetToolClientData( event.GetId() );
	if ( NULL != wxobject )
	{
		m_manager->SelectObject( wxobject );
	}
}

class WizardFormComponent : public ComponentBase
{
public:
	wxObject* Create(IObject*, wxObject* parent) override {
		wxPanel *wizard = new wxPanel( (wxWindow *)parent );
		return wizard;
	}

	void OnSelected(wxObject*, IManager*) {
        wxLogDebug( wxT("Wizard OnSelected") );
    }

	ticpp::Element* ExportToXrc(IObject* obj) override {
		ObjectToXrcFilter xrc( obj, wxT("wxWizard"), obj->GetPropertyAsString( wxT("name") ) );
		xrc.AddWindowProperties();
		xrc.AddProperty( wxT("title"), wxT("title"), XRC_TYPE_TEXT );
		if ( !obj->IsNull( wxT("center") ) )
			xrc.AddPropertyValue( wxT("centered"), wxT("1") );

		if ( !obj->IsNull( wxT("bitmap") ) )
			xrc.AddProperty( wxT("bitmap"), wxT("bitmap"), XRC_TYPE_BITMAP );

		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc(ticpp::Element* xrcObj) override {
		XrcToXfbFilter filter( xrcObj, wxT("Wizard") );
		filter.AddWindowProperties();
		filter.AddProperty( wxT("title"),    wxT("title"),  XRC_TYPE_TEXT );
		filter.AddProperty( wxT("centered"), wxT("center"), XRC_TYPE_BITLIST );
		filter.AddProperty( wxT("bitmap"),   wxT("bitmap"), XRC_TYPE_BITMAP );
		return filter.GetXfbObject();
	}
};

class WizardPageComponent : public ComponentBase
{
public:
	wxObject* Create(IObject* /*obj*/, wxObject* parent) override {
		WizardPageSimple *page = new WizardPageSimple( ( Wizard * )parent );
		return page;
	}
/*
    void OnSelected( wxObject* wxobject, IManager* manager )
	{
        wxLogDebug( wxT("WizardPageSimple OnSelected") );
		// Get actual page - first child
		wxObject* page = manager->GetChild( wxobject, 0 );
		if ( !page )
		{
			return;
		}

		Wizard* wizard = wxDynamicCast( manager->GetParent( wxobject ), Wizard );
		if ( wizard )
		{
			for ( size_t i = 0; i < wizard->GetPageCount(); ++i )
			{
				if ( wizard->GetPage( i ) == page )
				{
					// Prevent infinite event loop
					wxEvtHandler* wizardEvtHandler  = wizard->PopEventHandler();
					wxEvtHandler* vobjEvtHandler    = wizard->PopEventHandler();

					// Select Page
                    wizard->SetSelection( i );

					// Restore event handling
					wizard->PushEventHandler( vobjEvtHandler );
					wizard->PushEventHandler( wizardEvtHandler );
				}
			}
		}
	}
*/
	ticpp::Element* ExportToXrc(IObject* obj) override {
        ObjectToXrcFilter xrc( obj, wxT("wxWizardPageSimple"), obj->GetPropertyAsString( wxT("name") ) );
        xrc.AddWindowProperties();
		if ( !obj->IsNull( wxT("bitmap") ) )
			xrc.AddProperty( wxT("bitmap"), wxT("bitmap"), XRC_TYPE_BITMAP );
		return xrc.GetXrcObject();
	}

	ticpp::Element* ImportFromXrc(ticpp::Element* xrcObj) override {
        XrcToXfbFilter filter( xrcObj, wxT("WizardPageSimple") );
        filter.AddWindowProperties();
		filter.AddProperty( wxT("bitmap"), wxT("bitmap"), XRC_TYPE_BITMAP );
		return filter.GetXfbObject();
	}
};

BEGIN_LIBRARY()

// forms aren't considered as windows !
ABSTRACT_COMPONENT("Frame", FrameFormComponent)
ABSTRACT_COMPONENT("Panel", PanelFormComponent)
ABSTRACT_COMPONENT("Dialog",DialogFormComponent)

ABSTRACT_COMPONENT( "Wizard", WizardFormComponent )
MACRO(wxWIZARD_EX_HELPBUTTON)

WINDOW_COMPONENT( "WizardPageSimple", WizardPageComponent )

ABSTRACT_COMPONENT("MenuBar",MenuBarFormComponent)
WINDOW_COMPONENT("ToolBar",ToolBarFormComponent)

// wxWindow style macros
MACRO(wxBORDER_DEFAULT)
MACRO(wxBORDER_SIMPLE)
MACRO(wxBORDER_DOUBLE)
MACRO(wxBORDER_SUNKEN)
MACRO(wxBORDER_RAISED)
MACRO(wxBORDER_STATIC)
MACRO(wxBORDER_THEME)
MACRO(wxBORDER_NONE)
MACRO(wxTRANSPARENT_WINDOW)
MACRO(wxTAB_TRAVERSAL)
MACRO(wxWANTS_CHARS)
MACRO(wxVSCROLL)
MACRO(wxHSCROLL)
MACRO(wxALWAYS_SHOW_SB)
MACRO(wxCLIP_CHILDREN)
MACRO(wxFULL_REPAINT_ON_RESIZE)
MACRO(wxNO_FULL_REPAINT_ON_RESIZE)
MACRO(wxWS_EX_VALIDATE_RECURSIVELY)
MACRO(wxWS_EX_BLOCK_EVENTS)
MACRO(wxWS_EX_TRANSIENT)
MACRO(wxWS_EX_CONTEXTHELP)
MACRO(wxWS_EX_PROCESS_IDLE)
MACRO(wxWS_EX_PROCESS_UI_UPDATES)

SYNONYMOUS(wxSIMPLE_BORDER, wxBORDER_SIMPLE)
SYNONYMOUS(wxDOUBLE_BORDER, wxBORDER_DOUBLE)
SYNONYMOUS(wxSUNKEN_BORDER, wxBORDER_SUNKEN)
SYNONYMOUS(wxRAISED_BORDER, wxBORDER_RAISED)
SYNONYMOUS(wxSTATIC_BORDER, wxBORDER_STATIC)
SYNONYMOUS(wxNO_BORDER, wxBORDER_NONE)

// wxFrame style macros
MACRO(wxDEFAULT_FRAME_STYLE)
MACRO(wxICONIZE)
MACRO(wxCAPTION)
MACRO(wxMINIMIZE)
MACRO(wxMINIMIZE_BOX)
MACRO(wxMAXIMIZE)
MACRO(wxMAXIMIZE_BOX)
MACRO(wxCLOSE_BOX)
MACRO(wxSTAY_ON_TOP)
MACRO(wxSYSTEM_MENU)
MACRO(wxRESIZE_BORDER)
MACRO(wxFRAME_TOOL_WINDOW)
MACRO(wxFRAME_NO_TASKBAR)
MACRO(wxFRAME_FLOAT_ON_PARENT)
MACRO(wxFRAME_SHAPED)
MACRO(wxFRAME_EX_CONTEXTHELP)
MACRO(wxFRAME_EX_METAL)

// wxDialog style macros
MACRO(wxCAPTION)
MACRO(wxDEFAULT_DIALOG_STYLE)
MACRO(wxRESIZE_BORDER)
MACRO(wxSYSTEM_MENU)
MACRO(wxCLOSE_BOX)
MACRO(wxMAXIMIZE_BOX)
MACRO(wxMINIMIZE_BOX)
MACRO(wxSTAY_ON_TOP)
MACRO(wxDIALOG_NO_PARENT)

//wxDialog
MACRO(wxBOTH)
SYNONYMOUS(1,wxBOTH)

// wxMenuBar
MACRO(wxMB_DOCKABLE)

// wxToolBar
MACRO(wxTB_FLAT)
MACRO(wxTB_DOCKABLE)
MACRO(wxTB_HORIZONTAL)
MACRO(wxTB_VERTICAL)
MACRO(wxTB_TEXT)
MACRO(wxTB_NOICONS)
MACRO(wxTB_NODIVIDER)
MACRO(wxTB_NOALIGN)
MACRO(wxTB_HORZ_LAYOUT)
MACRO(wxTB_HORZ_TEXT)

// wxAuiManager
MACRO(wxAUI_MGR_ALLOW_FLOATING)
MACRO(wxAUI_MGR_ALLOW_ACTIVE_PANE)
MACRO(wxAUI_MGR_TRANSPARENT_DRAG)
MACRO(wxAUI_MGR_TRANSPARENT_HINT)
MACRO(wxAUI_MGR_VENETIAN_BLINDS_HINT)
MACRO(wxAUI_MGR_RECTANGLE_HINT)
MACRO(wxAUI_MGR_HINT_FADE)
MACRO(wxAUI_MGR_NO_VENETIAN_BLINDS_FADE)
MACRO(wxAUI_MGR_LIVE_RESIZE)
MACRO(wxAUI_MGR_DEFAULT)

END_LIBRARY()
