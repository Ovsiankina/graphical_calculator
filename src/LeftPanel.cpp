#include "Frame.h"
#include <iostream>
#include <set>
#include <sstream>       // for std::istringstream
#include <stack>         // for std::stack
#include <unordered_map> // for std::unordered_map
#include <vector>
#include <wx/event.h>
#include <wx/gtk/textctrl.h>
#include <wx/stattext.h>
// ======================== LEFT ================================

LeftPanel::LeftPanel(wxPanel *parent)
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
  count = 0;
  m_parent = parent;

  // == Math dialog box ==
  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  // button
  m_mathInput = new wxButton(this, ID_MATH_INPUT, "Enter a math equation");
  vbox->Add(m_mathInput, 0, wxALL | wxCENTER, 10);
  Connect(ID_MATH_INPUT, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(LeftPanel::OnMath));
  // text input box (dialog box)
  m_textCtrl =
      new wxTextCtrl(this, ID_MATH_INPUT, "", wxPoint(-1, -1), wxSize(300, 30));
  vbox->Add(m_textCtrl, 0, wxALL | wxCENTER, 10);
  // Automatic sizer of the dialog box
  this->SetSizer(vbox);

  // == SLIDER ==
  m_slider =
      new wxSlider(this, ID_SLIDER, 0, -100, 100, wxPoint(30, 40),
                   wxSize(250, wxDefaultCoord), wxSL_HORIZONTAL | wxSL_LABELS);
  Connect(ID_SLIDER, wxEVT_SLIDER,
          wxCommandEventHandler(LeftPanel::OnSliderScroll));
  m_label =
      new wxStaticText(this, ID_SLIDER, "Slider Value: 0", wxPoint(30, 100));

  // == PLUS AND MINUS BUTTON
  m_plus = new wxButton(this, ID_PLUS, wxT("+"), wxPoint(10, 300));
  Connect(ID_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(LeftPanel::OnPlus));

  m_minus = new wxButton(this, ID_MINUS, wxT("-"), wxPoint(10, 400));
  Connect(ID_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
          wxCommandEventHandler(LeftPanel::OnMinus));
}

void LeftPanel::OnMath(wxCommandEvent &WXUNUSED(event)) {
  user_input = m_textCtrl->GetValue().ToStdString();
  // just to showcase how it works
  std::cout << "OnButtonClick :" << user_input << std::endl;

  int xMin = -100;
  int xMax = 100 + 1;

  // TODO: Scale is a GUI slider
  int scale = 10; // Scale the parabola for better visibility

  wxSize window_size = GetSize();
  int screen_width = window_size.GetWidth();
  int screen_height = window_size.GetHeight();

  std::cout << "user_input: " << user_input << std::endl;
  std::cout << "Screen size : " << screen_width << " x " << screen_height
            << std::endl;

  // Evaluate expression and generate points
  std::vector<wxPoint> wxPoints;

  calculateLinePoints(user_input, xMin, xMax, scale, &wxPoints);

  // Send points to right panel
  MainFrame *rightPanel = (MainFrame *)m_parent->GetParent();
  rightPanel->m_rightPanel->PaintLines(wxPoints);
}

int LeftPanel::calculateLinePoints(const std::string &expr, const int xMin,
                                   const int xMax, int scale,
                                   std::vector<wxPoint> *pwxPoint) {

  // Ask the user for a mathematical expression
  std::cout << "User input: " << expr;

  std::vector<wxPoint> points;

  try {
    // Detect all variables in the expression
    std::set<char> variables = detectVariables(expr);

    // Evaluate the expression 100 times with all variables incrementing each
    // time
    for (int i = xMin; i < xMax; ++i) {
      std::unordered_map<char, double> variable_values;
      for (char var : variables) {
        variable_values[var] = static_cast<double>(i);
      }
      double result = evaluateExpression(expr, variable_values);
      points.push_back(wxPoint(i, static_cast<int>(result)));
    }

    // Output the results for debugging
    for (wxPoint &point : points) {
      point.x *= scale;
      point.y *= scale * -1; // otherwise graph is reversed on y axis
      std::cout << "# ";
    }
    std::cout << std::endl;

    // update pwxPoints with the calculated points
    *pwxPoint = points;

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

std::set<char> LeftPanel::detectVariables(const std::string &expr) {
  std::set<char> variables;
  for (char c : expr) {
    if (std::isalpha(c)) {
      variables.insert(c);
    }
  }
  if (variables.empty()) {
    throw std::runtime_error("No variables found in the expression");
  }
  return variables;
}

// Function to perform the Shunting Yard algorithm and evaluate the expression
double LeftPanel::evaluateExpression(
    const std::string &expr,
    const std::unordered_map<char, double> &variable_values) {
  std::stack<double> values;
  std::stack<char> ops;
  std::istringstream input(expr);
  char token;

  while (input >> token) {
    if (std::isdigit(token)) {
      input.putback(token);
      double value;
      input >> value;
      values.push(value);
    } else if (token == '(') {
      ops.push(token);
    } else if (token == ')') {
      while (!ops.empty() && ops.top() != '(') {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOperator(a, b, op));
      }
      if (!ops.empty())
        ops.pop();
    } else if (std::isalpha(token)) {
      if (variable_values.find(token) != variable_values.end()) {
        values.push(variable_values.at(token));
      } else {
        throw std::runtime_error("Variable not defined");
      }
    } else if (std::string("+-*/^").find(token) != std::string::npos) {
      while (!ops.empty() && precedence(ops.top()) >= precedence(token)) {
        double b = values.top();
        values.pop();
        double a = values.top();
        values.pop();
        char op = ops.top();
        ops.pop();
        values.push(applyOperator(a, b, op));
      }
      ops.push(token);
    }
  }

  while (!ops.empty()) {
    double b = values.top();
    values.pop();
    double a = values.top();
    values.pop();
    char op = ops.top();
    ops.pop();
    values.push(applyOperator(a, b, op));
  }

  return values.top();
}

void LeftPanel::OnSliderScroll(wxCommandEvent &WXUNUSED(event)) {
  int slider_value = m_slider->GetValue();
  // just to showcase how it works
  m_label->SetLabel(wxString::Format("Slider Value: %d", slider_value));
}

double LeftPanel::applyOperator(double a, double b, char op) {

  switch (op) {

  case '+':
    return (a + b);
  case '-':
    return (a - b);
  case '*':
    return (a * b);
  case '/':
    return (a / b);
  case '^':
    return std::pow(a, b);
  default:
    throw std::runtime_error("Invalid operator");
  }
}

// Function to determine the precedence of an operator
int LeftPanel::precedence(char op) {
  if (op == '+' || op == '-')
    return 1;
  if (op == '*' || op == '/')
    return 2;
  if (op == '^')
    return 3;
  return 0;
}

// == Plus and Minus buttons
void LeftPanel::OnPlus(wxCommandEvent &WXUNUSED(event)) {
  count++;

  MainFrame *rightPanel = (MainFrame *)m_parent->GetParent();
  rightPanel->m_rightPanel->m_text->SetLabel(
      wxString::Format(wxT("%d"), count));
}

void LeftPanel::OnMinus(wxCommandEvent &WXUNUSED(event)) {
  count--;

  MainFrame *rightPanel = (MainFrame *)m_parent->GetParent();
  rightPanel->m_rightPanel->m_text->SetLabel(
      wxString::Format(wxT("%d"), count));
}
