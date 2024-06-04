#include "Panels.h"
#include <wx/wxprec.h>

class MainFrame : public wxFrame {

public:
  MainFrame(const wxString &title);
  void GetDimensions();

  LeftPanel *m_leftPanel;
  RightPanel *m_rightPanel;
  wxPanel *m_parent;
};
