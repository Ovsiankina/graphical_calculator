#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <wx/event.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

class LeftPanel : public wxPanel {
public:
  LeftPanel(wxPanel *parent);

  void OnMath(wxCommandEvent &event);
  void OnSliderScroll(wxCommandEvent &event);
  int calculateLinePoints(const std::string &expr, const int xMin,
                          const int xMax, int scale,
                          std::vector<wxPoint> *pwxPoint);
  std::set<char> detectVariables(const std::string &expr);
  double
  evaluateExpression(const std::string &expr,
                     const std::unordered_map<char, double> &variable_values);
  double applyOperator(double a, double b, char op);
  int precedence(char op);
  void OnPlus(wxCommandEvent &event);
  void OnMinus(wxCommandEvent &event);

  int count;
  std::string user_input;

  wxTextCtrl *m_textCtrl;
  wxButton *m_mathInput;
  wxButton *m_plus;
  wxButton *m_minus;
  wxPanel *m_parent;
  wxSlider *m_slider;
  wxStaticText *m_label;
};

class RightPanel : public wxPanel {
public:
  RightPanel(wxPanel *parent);

  void OnSetText(wxCommandEvent &event);
  void OnPaint(wxPaintEvent &event);
  void PaintLines(std::vector<wxPoint> wxPoints);
  void render(wxDC &dc);

  wxStaticText *m_text;
  std::vector<wxPoint> m_points;

  bool paint_active;
};

const int ID_MATH_INPUT = 101;
const int ID_SLIDER = 101;
const int ID_PLUS = 191;
const int ID_MINUS = 192;
