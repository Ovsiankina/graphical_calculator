#include "Panels.h"
#include <wx/event.h>
#include <wx/gtk/textctrl.h>
#include <wx/stattext.h>
// ======================== RIGHT ================================

RightPanel::RightPanel(wxPanel *parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(270, 150),
              wxBORDER_SUNKEN) { // paint_active false by default

  m_text = new wxStaticText(this, -1, wxT("0"), wxPoint(40, 60));
}

void RightPanel::PaintLines(std::vector<wxPoint> wxPoints) {
  m_points = wxPoints;
  Connect(wxEVT_PAINT, wxPaintEventHandler(RightPanel::OnPaint));
  Refresh();
}

void RightPanel::OnPaint(wxPaintEvent &WXUNUSED(event)) {
  wxPaintDC dc(this);
  wxSize window_size = GetSize();
  int screen_width = window_size.GetWidth();
  int screen_height = window_size.GetHeight();

  int xOffset = screen_width / 2;
  int yOffset = screen_height / 2;

  std::cout << "Screen size (from RightPanel): " << screen_width << " x "
            << screen_height << std::endl;

  for (wxPoint &point : m_points) {
    std::cout << std::endl
              << "OnPaint : " << "(" << point.y << ", " << point.x << ")";
  }

  try {
    dc.DrawLines(m_points.size(), &m_points[0], xOffset, yOffset);

  } catch (std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }
}
