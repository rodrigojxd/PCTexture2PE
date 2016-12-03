#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
//using namespace System::Reflection;

[STAThread]

int main(cli::array <System::String^>^ args)
{
	//Application::EnableVisualStyles();
	//Application::SetCompatibleTextRenderingDefault(true);
	PCTexture2PE_GUI::MainForm form;
	Application::Run(%form);
	return 0;
}