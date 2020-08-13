#ifndef DIALOGFINDCOMPONENT_H
#define DIALOGFINDCOMPONENT_H

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/dialog.h>
#include <wx/scrolwin.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>


class DialogFindComponent : public wxDialog
{
public:
    DialogFindComponent(wxWindow* parent, const wxArrayString &componentsList,
                        wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString,
                        const wxPoint& pos = wxDefaultPosition,
                        const wxSize& size = wxSize( 250,400 ), long style = wxDEFAULT_DIALOG_STYLE);

    ~DialogFindComponent();

    wxString GetSelected() {return m_chosenComponent;}

private:
    void OnTextCtrlComponent(wxCommandEvent& event);
    void OnListBoxComponentsDClick(wxCommandEvent& event);

    wxScrolledWindow* m_scrolledWindow;
    wxTextCtrl* m_textCtrlComponent;
    wxListBox* m_listBoxComponents;

    const wxArrayString& m_componentsList;
    wxArrayString m_componentsFinded;
    wxString m_chosenComponent = wxEmptyString;
};

#endif // DIALOGFINDCOMPONENT_H
