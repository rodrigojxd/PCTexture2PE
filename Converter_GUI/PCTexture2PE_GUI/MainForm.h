#pragma once
//#include <Windows.h>
//#include <iostream>
#include <msclr/marshal_cppstd.h> // for convert System::String to std::string and vice-versa
#include "About.h"
//#include "TextureConverter.h"

//from ConverterDll. I know, it's not the best way
namespace ConverterFuncs
{
	int ConvertPack(const std::string& inPack, const std::string& packname, const std::string& output_folder);
	const std::string GetLog();
}

bool started = false;

namespace PCTexture2PE_GUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace msclr::interop;
	using namespace System::Threading;


	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::Button^  button_Start;
	private: System::Windows::Forms::FolderBrowserDialog^  outputFolderBrowser;
	private: System::Windows::Forms::Button^  inputFolderButton;
	private: System::Windows::Forms::Button^  outputFolderButton;
	private: System::Windows::Forms::TextBox^  inputFolderBox;
	private: System::Windows::Forms::TextBox^  outputFolderBox;
	private: System::Windows::Forms::Button^  aboutButton;
	private: System::Windows::Forms::TextBox^  packNameBox;
	private: System::Windows::Forms::TextBox^  logBox;
	private: System::Windows::Forms::OpenFileDialog^  inputFile;
	private: System::Windows::Forms::GroupBox^  settingsGroupBox;
	private: System::Windows::Forms::Label^  packNameLabel;
	private: System::Windows::Forms::Label^  outputFolderLabel;
	private: System::Windows::Forms::Label^  inputPackLabel;
	private: System::Windows::Forms::GroupBox^  progressGroupBox;


	private:



	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->button_Start = (gcnew System::Windows::Forms::Button());
			this->outputFolderBrowser = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->inputFolderButton = (gcnew System::Windows::Forms::Button());
			this->outputFolderButton = (gcnew System::Windows::Forms::Button());
			this->inputFolderBox = (gcnew System::Windows::Forms::TextBox());
			this->outputFolderBox = (gcnew System::Windows::Forms::TextBox());
			this->aboutButton = (gcnew System::Windows::Forms::Button());
			this->packNameBox = (gcnew System::Windows::Forms::TextBox());
			this->logBox = (gcnew System::Windows::Forms::TextBox());
			this->inputFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->settingsGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->packNameLabel = (gcnew System::Windows::Forms::Label());
			this->outputFolderLabel = (gcnew System::Windows::Forms::Label());
			this->inputPackLabel = (gcnew System::Windows::Forms::Label());
			this->progressGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->settingsGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// button_Start
			// 
			this->button_Start->BackColor = System::Drawing::SystemColors::Control;
			this->button_Start->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button_Start->Location = System::Drawing::Point(12, 145);
			this->button_Start->Name = L"button_Start";
			this->button_Start->Size = System::Drawing::Size(367, 28);
			this->button_Start->TabIndex = 0;
			this->button_Start->Text = L"Start";
			this->button_Start->UseVisualStyleBackColor = false;
			this->button_Start->Click += gcnew System::EventHandler(this, &MainForm::button_Start_Click);
			// 
			// outputFolderBrowser
			// 
			this->outputFolderBrowser->RootFolder = System::Environment::SpecialFolder::MyComputer;
			// 
			// inputFolderButton
			// 
			this->inputFolderButton->BackColor = System::Drawing::SystemColors::Control;
			this->inputFolderButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->inputFolderButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->inputFolderButton->ForeColor = System::Drawing::SystemColors::ControlText;
			this->inputFolderButton->Location = System::Drawing::Point(334, 18);
			this->inputFolderButton->Name = L"inputFolderButton";
			this->inputFolderButton->Size = System::Drawing::Size(25, 20);
			this->inputFolderButton->TabIndex = 13;
			this->inputFolderButton->Text = L"...";
			this->inputFolderButton->UseVisualStyleBackColor = false;
			this->inputFolderButton->Click += gcnew System::EventHandler(this, &MainForm::inputFolderButton_Click);
			// 
			// outputFolderButton
			// 
			this->outputFolderButton->BackColor = System::Drawing::SystemColors::Control;
			this->outputFolderButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->outputFolderButton->ForeColor = System::Drawing::SystemColors::ControlText;
			this->outputFolderButton->Location = System::Drawing::Point(334, 47);
			this->outputFolderButton->Name = L"outputFolderButton";
			this->outputFolderButton->Size = System::Drawing::Size(25, 20);
			this->outputFolderButton->TabIndex = 14;
			this->outputFolderButton->Text = L"...";
			this->outputFolderButton->UseVisualStyleBackColor = false;
			this->outputFolderButton->Click += gcnew System::EventHandler(this, &MainForm::outputFolderButton_Click);
			// 
			// inputFolderBox
			// 
			this->inputFolderBox->Location = System::Drawing::Point(79, 18);
			this->inputFolderBox->Name = L"inputFolderBox";
			this->inputFolderBox->Size = System::Drawing::Size(253, 20);
			this->inputFolderBox->TabIndex = 15;
			this->inputFolderBox->DoubleClick += gcnew System::EventHandler(this, &MainForm::TextBox_SelectAll);
			this->inputFolderBox->Leave += gcnew System::EventHandler(this, &MainForm::inputFolderBox_Leave);
			// 
			// outputFolderBox
			// 
			this->outputFolderBox->Location = System::Drawing::Point(79, 47);
			this->outputFolderBox->Name = L"outputFolderBox";
			this->outputFolderBox->Size = System::Drawing::Size(253, 20);
			this->outputFolderBox->TabIndex = 16;
			this->outputFolderBox->DoubleClick += gcnew System::EventHandler(this, &MainForm::TextBox_SelectAll);
			this->outputFolderBox->Leave += gcnew System::EventHandler(this, &MainForm::outputFolderBox_Leave);
			// 
			// aboutButton
			// 
			this->aboutButton->BackColor = System::Drawing::SystemColors::Control;
			this->aboutButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->aboutButton->Location = System::Drawing::Point(335, 404);
			this->aboutButton->Name = L"aboutButton";
			this->aboutButton->Padding = System::Windows::Forms::Padding(2, 0, 0, 0);
			this->aboutButton->Size = System::Drawing::Size(44, 22);
			this->aboutButton->TabIndex = 17;
			this->aboutButton->Text = L"About";
			this->aboutButton->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			this->aboutButton->UseVisualStyleBackColor = false;
			this->aboutButton->Click += gcnew System::EventHandler(this, &MainForm::aboutButton_Click);
			// 
			// packNameBox
			// 
			this->packNameBox->Location = System::Drawing::Point(79, 76);
			this->packNameBox->Name = L"packNameBox";
			this->packNameBox->Size = System::Drawing::Size(280, 20);
			this->packNameBox->TabIndex = 18;
			this->packNameBox->DoubleClick += gcnew System::EventHandler(this, &MainForm::TextBox_SelectAll);
			// 
			// logBox
			// 
			this->logBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->logBox->HideSelection = false;
			this->logBox->Location = System::Drawing::Point(12, 204);
			this->logBox->Multiline = true;
			this->logBox->Name = L"logBox";
			this->logBox->ReadOnly = true;
			this->logBox->ScrollBars = System::Windows::Forms::ScrollBars::Both;
			this->logBox->Size = System::Drawing::Size(367, 190);
			this->logBox->TabIndex = 19;
			this->logBox->WordWrap = false;
			// 
			// inputFile
			// 
			this->inputFile->Filter = L"pack.mcmeta|*.mcmeta";
			this->inputFile->InitialDirectory = L"Desktop";
			// 
			// settingsGroupBox
			// 
			this->settingsGroupBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->settingsGroupBox->Controls->Add(this->packNameBox);
			this->settingsGroupBox->Controls->Add(this->packNameLabel);
			this->settingsGroupBox->Controls->Add(this->outputFolderLabel);
			this->settingsGroupBox->Controls->Add(this->outputFolderBox);
			this->settingsGroupBox->Controls->Add(this->inputFolderBox);
			this->settingsGroupBox->Controls->Add(this->inputPackLabel);
			this->settingsGroupBox->Controls->Add(this->outputFolderButton);
			this->settingsGroupBox->Controls->Add(this->inputFolderButton);
			this->settingsGroupBox->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->settingsGroupBox->ForeColor = System::Drawing::SystemColors::ControlLight;
			this->settingsGroupBox->Location = System::Drawing::Point(12, 17);
			this->settingsGroupBox->Name = L"settingsGroupBox";
			this->settingsGroupBox->Size = System::Drawing::Size(367, 112);
			this->settingsGroupBox->TabIndex = 24;
			this->settingsGroupBox->TabStop = false;
			this->settingsGroupBox->Text = L"Settings";
			// 
			// packNameLabel
			// 
			this->packNameLabel->AutoSize = true;
			this->packNameLabel->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->packNameLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->packNameLabel->Location = System::Drawing::Point(7, 79);
			this->packNameLabel->Name = L"packNameLabel";
			this->packNameLabel->Size = System::Drawing::Size(63, 13);
			this->packNameLabel->TabIndex = 22;
			this->packNameLabel->Text = L"Pack Name";
			this->packNameLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// outputFolderLabel
			// 
			this->outputFolderLabel->AutoSize = true;
			this->outputFolderLabel->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->outputFolderLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->outputFolderLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->outputFolderLabel->Location = System::Drawing::Point(7, 50);
			this->outputFolderLabel->Name = L"outputFolderLabel";
			this->outputFolderLabel->Size = System::Drawing::Size(71, 13);
			this->outputFolderLabel->TabIndex = 21;
			this->outputFolderLabel->Text = L"Output Folder";
			this->outputFolderLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// inputPackLabel
			// 
			this->inputPackLabel->AutoSize = true;
			this->inputPackLabel->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->inputPackLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->inputPackLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->inputPackLabel->Location = System::Drawing::Point(7, 21);
			this->inputPackLabel->Name = L"inputPackLabel";
			this->inputPackLabel->Size = System::Drawing::Size(65, 13);
			this->inputPackLabel->TabIndex = 20;
			this->inputPackLabel->Text = L"MCPC Pack";
			this->inputPackLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// progressGroupBox
			// 
			this->progressGroupBox->ForeColor = System::Drawing::SystemColors::ControlLight;
			this->progressGroupBox->Location = System::Drawing::Point(12, 185);
			this->progressGroupBox->Name = L"progressGroupBox";
			this->progressGroupBox->Size = System::Drawing::Size(367, 209);
			this->progressGroupBox->TabIndex = 25;
			this->progressGroupBox->TabStop = false;
			this->progressGroupBox->Text = L"Progress";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(391, 438);
			this->Controls->Add(this->aboutButton);
			this->Controls->Add(this->button_Start);
			this->Controls->Add(this->logBox);
			this->Controls->Add(this->settingsGroupBox);
			this->Controls->Add(this->progressGroupBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->Text = L"PCTexture2PE";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->settingsGroupBox->ResumeLayout(false);
			this->settingsGroupBox->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		//convert std::string to System::String^
		String^ toSysString(const std::string& s)
		{
			return marshal_as< String^ >(s);
		}

		void SetTextBoxOnce()
		{
			String^ log = toSysString(ConverterFuncs::GetLog());
			int sz1 = log->Length;
			int sz2 = this->logBox->TextLength;
			if (sz1 > sz2)
			{
				log = log->Substring(sz2); //Removes the part of the log that is already in the box
				this->logBox->AppendText(log); //add th log and change the scroll to the end
			}
		}

		void RegisterLog()
		{
			this->logBox->Text = ""; //clean the logbox
			this->logBox->Update(); //force a update
			while (started)
			{
				this->Invoke(gcnew MethodInvoker(this, &MainForm::SetTextBoxOnce));
				Sleep(50);
			}
		}

		void convert()
		{
			String^ tmp = inputFile->FileName->Remove(inputFile->FileName->LastIndexOf("\\"));
			std::string pack = marshal_as< std::string >(tmp);
			std::string name = marshal_as< std::string >(packNameBox->Text);
			std::string output = marshal_as< std::string >(outputFolderBrowser->SelectedPath);
			ConverterFuncs::ConvertPack(pack, name, output);
			Sleep(300); //wait for the last SetTextBoxOnce
			started = false;
			System::Media::SystemSounds::Asterisk->Play();
		}

	private: System::Void button_Start_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (!started)
		{
			bool invalidpack = !System::IO::File::Exists(inputFile->FileName);
			bool invalidoutput = !System::IO::Directory::Exists(outputFolderBrowser->SelectedPath);
			bool invalidname = String::IsNullOrWhiteSpace(packNameBox->Text);

			if (invalidpack)
			{
				MessageBox::Show("Invalid MCPC Pack");
				return;
			}
			if (invalidoutput)
			{
				MessageBox::Show("Invalid Output Folder");
				return;
			}
			if (invalidname)
			{
				MessageBox::Show("Invalid Pack Name");
				return;
			}

			started = true;
			Thread^ tThread = gcnew Thread(gcnew ThreadStart(this, &MainForm::RegisterLog));
			tThread->Start();

			Thread^ nThread = gcnew Thread(gcnew ThreadStart(this, &MainForm::convert));
			nThread->Start();
		}
		else
		{
			MessageBox::Show("Process already running!");
		}
	}

	private: System::Void inputFolderButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		inputFile->ShowDialog();
		if (!String::IsNullOrWhiteSpace(inputFile->FileName))
		{
			inputFolderBox->Text = inputFile->FileName;
			String^ tmp = inputFile->FileName->Remove(inputFile->FileName->LastIndexOf("\\"));
			packNameBox->Text = tmp->Substring(tmp->LastIndexOf("\\") + 1);
		}
	}

	private: System::Void outputFolderButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		outputFolderBrowser->ShowDialog();
		if (!String::IsNullOrWhiteSpace(outputFolderBrowser->SelectedPath))
		{
			outputFolderBox->Text = outputFolderBrowser->SelectedPath;
		}
	}

	private: System::Void aboutButton_Click(System::Object^  sender, System::EventArgs^  e)
	{
		About ^ aboutF = gcnew About;
		aboutF->ShowDialog();
	}

	private: System::Void TextBox_SelectAll(System::Object^  sender, System::EventArgs^  e)
	{
		TextBox^ thisBox = safe_cast<TextBox^>(sender);
		thisBox->SelectAll();
	}

	//alert if click to close the app while is converting a pack
	private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
	{
		if (started)
		{
			if (MessageBox::Show("Converting a pack! Do you want to stop?",
				"PCTexture2PE", MessageBoxButtons::OKCancel,
				MessageBoxIcon::Warning)==System::Windows::Forms::DialogResult::OK)
			{
				Application::ExitThread();
			}
			else
			{
				e->Cancel = true;
			}
		}
	}

	//set the inputfile from inputFolderBox when click out the box
	private: System::Void inputFolderBox_Leave(System::Object^  sender, System::EventArgs^  e)
	{
		inputFile->FileName = inputFolderBox->Text;
	}

	//set the outputFolderBrowser from outputFolderBox when click out the box
	private: System::Void outputFolderBox_Leave(System::Object^  sender, System::EventArgs^  e)
	{
		outputFolderBrowser->SelectedPath = outputFolderBox->Text;
	}
};
}