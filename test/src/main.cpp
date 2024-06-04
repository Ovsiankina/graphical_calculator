#include "main.h"
#include "Frame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    MainFrame *MainFrame = new class MainFrame(wxT("Widgets communicate"));
    MainFrame->Show(true);

    return true;
}
