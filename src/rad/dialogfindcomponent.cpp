#include "dialogfindcomponent.h"
#include <wx/sizer.h>


DialogFindComponent::DialogFindComponent(wxWindow* parent, const wxArrayString& componentsList,
                                         wxWindowID id, const wxString& title, const wxPoint& pos,
                                         const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style), m_componentsList{componentsList}
{
    this->SetSizeHints( wxSize( 250,400 ), wxDefaultSize );

    wxBoxSizer* bSizerMain;
    bSizerMain = new wxBoxSizer( wxVERTICAL );

    m_scrolledWindow = new wxScrolledWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHSCROLL|wxVSCROLL );
    m_scrolledWindow->SetScrollRate( 5, 5 );
    wxBoxSizer* bSizer2;
    bSizer2 = new wxBoxSizer( wxVERTICAL );

    m_textCtrlComponent = new wxTextCtrl( m_scrolledWindow, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
    bSizer2->Add( m_textCtrlComponent, 0, wxALL|wxEXPAND, 5 );

    m_listBoxComponents = new wxListBox( m_scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_SINGLE );
    bSizer2->Add( m_listBoxComponents, 1, wxALL|wxEXPAND, 5 );


    m_scrolledWindow->SetSizer( bSizer2 );
    m_scrolledWindow->Layout();
    bSizer2->Fit( m_scrolledWindow );
    bSizerMain->Add( m_scrolledWindow, 1, wxEXPAND | wxALL, 5 );

    m_textCtrlComponent->Bind(wxEVT_TEXT, &DialogFindComponent::OnTextCtrlComponent, this);
    m_listBoxComponents->Bind(wxEVT_LISTBOX_DCLICK, &DialogFindComponent::OnListBoxComponentsDClick,
                              this);

    if (m_componentsList.Count())
    {
        m_listBoxComponents->InsertItems(m_componentsList, 0);
    }

    this->SetSizer( bSizerMain );
    this->Layout();

    this->Centre( wxBOTH );
}

DialogFindComponent::~DialogFindComponent()
{

}

void DialogFindComponent::OnTextCtrlComponent(wxCommandEvent &event)
{
    wxString typed = m_textCtrlComponent->GetValue();

    m_componentsFinded.Clear();

    for (const auto& item : m_componentsList)
    {
        auto pos = item.Lower().find(typed.Lower());

        if (pos != wxNOT_FOUND)
        {
            m_componentsFinded.Add(item);
        }
    }

    m_listBoxComponents->Clear();

    if (m_componentsFinded.Count())
    {
        m_listBoxComponents->InsertItems(m_componentsFinded, 0);
    }
}

void DialogFindComponent::OnListBoxComponentsDClick(wxCommandEvent &event)
{
    m_chosenComponent = m_listBoxComponents->GetString(m_listBoxComponents->GetSelection());
    this->Close();
}

