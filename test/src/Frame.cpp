#include "Frame.h"

MainFrame::MainFrame(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(2000, 1000))
{
  m_parent = new wxPanel(this, wxID_ANY);

  wxBoxSizer *hbox = new wxBoxSizer(wxHORIZONTAL);

  m_leftPanel = new LeftPanel(m_parent);
  m_rightPanel = new RightPanel(m_parent);

  hbox->Add(m_leftPanel, 1, wxEXPAND | wxALL, 5);
  hbox->Add(m_rightPanel, 1, wxEXPAND | wxALL, 5);

  m_parent->SetSizer(hbox);

  this->Centre();
}

void MainFrame::GetDimensions(){
  
  wxSize window_size = GetSize();
  int screen_width = window_size.GetWidth();
  int screen_height = window_size.GetHeight();

}
