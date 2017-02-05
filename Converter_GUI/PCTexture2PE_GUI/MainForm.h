#pragma once
//#include <Windows.h>
//#include <iostream>
#include <msclr/marshal_cppstd.h> // for convert System::String to std::string and vice-versa
#include "About.h"
#include "TextureConverter.h"
#include "zip_api.h"

bool started = false; //Indicates whether a conversion is running
int logcount = 0; //n of log lines printed on logbox

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
			this->inTypeCmbBox->SelectedIndex = 1; //.zip
			this->outTypeCmbBox->SelectedIndex = 1; //.zip
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
	private: System::ComponentModel::BackgroundWorker^  bkWorker_logbox;
	private: System::ComponentModel::BackgroundWorker^  bkWorker_converter;
	private: System::Windows::Forms::ComboBox^  inTypeCmbBox;
	private: System::Windows::Forms::ComboBox^  outTypeCmbBox;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	
	
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
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->outTypeCmbBox = (gcnew System::Windows::Forms::ComboBox());
			this->inTypeCmbBox = (gcnew System::Windows::Forms::ComboBox());
			this->packNameLabel = (gcnew System::Windows::Forms::Label());
			this->outputFolderLabel = (gcnew System::Windows::Forms::Label());
			this->inputPackLabel = (gcnew System::Windows::Forms::Label());
			this->progressGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->bkWorker_logbox = (gcnew System::ComponentModel::BackgroundWorker());
			this->bkWorker_converter = (gcnew System::ComponentModel::BackgroundWorker());
			this->settingsGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// button_Start
			// 
			this->button_Start->BackColor = System::Drawing::SystemColors::Control;
			this->button_Start->Cursor = System::Windows::Forms::Cursors::Hand;
			this->button_Start->Location = System::Drawing::Point(12, 151);
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
			this->inputFolderButton->Location = System::Drawing::Point(334, 54);
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
			this->outputFolderButton->Location = System::Drawing::Point(334, 80);
			this->outputFolderButton->Name = L"outputFolderButton";
			this->outputFolderButton->Size = System::Drawing::Size(25, 20);
			this->outputFolderButton->TabIndex = 14;
			this->outputFolderButton->Text = L"...";
			this->outputFolderButton->UseVisualStyleBackColor = false;
			this->outputFolderButton->Click += gcnew System::EventHandler(this, &MainForm::outputFolderButton_Click);
			// 
			// inputFolderBox
			// 
			this->inputFolderBox->Location = System::Drawing::Point(79, 54);
			this->inputFolderBox->Name = L"inputFolderBox";
			this->inputFolderBox->Size = System::Drawing::Size(253, 20);
			this->inputFolderBox->TabIndex = 15;
			this->inputFolderBox->DoubleClick += gcnew System::EventHandler(this, &MainForm::TextBox_SelectAll);
			this->inputFolderBox->Leave += gcnew System::EventHandler(this, &MainForm::inputFolderBox_Leave);
			// 
			// outputFolderBox
			// 
			this->outputFolderBox->Location = System::Drawing::Point(79, 80);
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
			this->packNameBox->Location = System::Drawing::Point(79, 106);
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
			this->inputFile->Filter = L"|*.zip";
			this->inputFile->InitialDirectory = L"Desktop";
			// 
			// settingsGroupBox
			// 
			this->settingsGroupBox->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->settingsGroupBox->Controls->Add(this->label2);
			this->settingsGroupBox->Controls->Add(this->label1);
			this->settingsGroupBox->Controls->Add(this->outTypeCmbBox);
			this->settingsGroupBox->Controls->Add(this->inTypeCmbBox);
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
			this->settingsGroupBox->Location = System::Drawing::Point(12, 7);
			this->settingsGroupBox->Name = L"settingsGroupBox";
			this->settingsGroupBox->Size = System::Drawing::Size(367, 132);
			this->settingsGroupBox->TabIndex = 24;
			this->settingsGroupBox->TabStop = false;
			this->settingsGroupBox->Text = L"Settings";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label2->Location = System::Drawing::Point(187, 23);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(66, 13);
			this->label2->TabIndex = 26;
			this->label2->Text = L"Output Type";
			this->label2->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label1->Location = System::Drawing::Point(8, 23);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(58, 13);
			this->label1->TabIndex = 25;
			this->label1->Text = L"Input Type";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// outTypeCmbBox
			// 
			this->outTypeCmbBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->outTypeCmbBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->outTypeCmbBox->FormattingEnabled = true;
			this->outTypeCmbBox->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Folder", L".zip", L".mcpack" });
			this->outTypeCmbBox->Location = System::Drawing::Point(260, 20);
			this->outTypeCmbBox->Name = L"outTypeCmbBox";
			this->outTypeCmbBox->Size = System::Drawing::Size(99, 24);
			this->outTypeCmbBox->TabIndex = 24;
			// 
			// inTypeCmbBox
			// 
			this->inTypeCmbBox->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->inTypeCmbBox->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->inTypeCmbBox->FormattingEnabled = true;
			this->inTypeCmbBox->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Folder", L".zip" });
			this->inTypeCmbBox->Location = System::Drawing::Point(79, 20);
			this->inTypeCmbBox->Name = L"inTypeCmbBox";
			this->inTypeCmbBox->Size = System::Drawing::Size(95, 24);
			this->inTypeCmbBox->TabIndex = 23;
			this->inTypeCmbBox->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::inTypeCmbBox_SelectedIndexChanged);
			// 
			// packNameLabel
			// 
			this->packNameLabel->AutoSize = true;
			this->packNameLabel->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->packNameLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->packNameLabel->Location = System::Drawing::Point(7, 109);
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
			this->outputFolderLabel->Location = System::Drawing::Point(7, 83);
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
			this->inputPackLabel->Location = System::Drawing::Point(7, 57);
			this->inputPackLabel->Name = L"inputPackLabel";
			this->inputPackLabel->Size = System::Drawing::Size(65, 13);
			this->inputPackLabel->TabIndex = 20;
			this->inputPackLabel->Text = L"MCPC Pack";
			this->inputPackLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// progressGroupBox
			// 
			this->progressGroupBox->ForeColor = System::Drawing::SystemColors::ControlLight;
			this->progressGroupBox->Location = System::Drawing::Point(11, 185);
			this->progressGroupBox->Name = L"progressGroupBox";
			this->progressGroupBox->Size = System::Drawing::Size(368, 209);
			this->progressGroupBox->TabIndex = 25;
			this->progressGroupBox->TabStop = false;
			this->progressGroupBox->Text = L"Progress";
			// 
			// bkWorker_logbox
			// 
			this->bkWorker_logbox->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainForm::bkWorker_logbox_DoWork);
			// 
			// bkWorker_converter
			// 
			this->bkWorker_converter->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainForm::bkWorker_converter_DoWork);
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
	private: String^ toSysString(const std::string& s)
	{
		return marshal_as< String^ >(s);
	}
	//convert System::String^ to std::string
	private: std::string toStdString(String^ s)
	{
		return marshal_as< std::string >(s);
	}

	private: System::Void button_Start_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (!started)
		{
			started = true;
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

			this->logBox->Text = ""; //clean the logbox
			ConverterFuncs::ClearLog();
			logcount = 0; //reset the counter

			this->bkWorker_logbox->RunWorkerAsync();
			this->bkWorker_converter->RunWorkerAsync();
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
			String^ sbl4search;
			if (this->inTypeCmbBox->SelectedIndex == 0) //input is a Folder
			{
				sbl4search = "\\"; //will remove the filename (pack.meta)
			}
			else //input is a .zip
			{
				sbl4search = "."; //will remove the ".zip"
			}
			String^ tmp = inputFile->FileName->Remove(inputFile->FileName->LastIndexOf(sbl4search));
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
	private: System::Void bkWorker_logbox_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
		const std::vector<std::string>* log = ConverterFuncs::GetLog();
		while (started)
		{
			int sz1 = log->size();
			if (sz1 > logcount) //If there is any new log
			{
				for (int i = logcount; i < sz1; ++i) //Looks at all the lines of the log that are not yet in the box
				{
					this->logBox->AppendText(toSysString(log->at(i) + "\r\n")); //add the log
					logcount++;
				}
			}
			Sleep(50);
		}
	}
	
#ifdef GetTempPath
#undef GetTempPath //System::IO::Path::GetTempPath() conflicts with that
#endif // GetTempPath
	private: System::Void bkWorker_converter_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
		
		String^ pack;
		String^ name = this->packNameBox->Text;
		String^ output = this->outputFolderBrowser->SelectedPath;
		String^ tmpOutput = System::IO::Path::GetTempPath() + "PCTexture2PE";
		System::IO::Directory::CreateDirectory(tmpOutput);
		
		if (this->inTypeCmbBox->SelectedIndex == 0) //folder
		{
			pack = this->inputFile->FileName->Remove(this->inputFile->FileName->LastIndexOf("\\")); //remove the "pack.meta"
		}
		else //.zip
		{
			pack = tmpOutput + "\\" + name + "_tmp";
			ConverterFuncs::Print("Extracting the zip file");
			zip_api::extractAll(toStdString(this->inputFile->FileName), toStdString(pack)); //extract to a temp folder
		}

		if (ConverterFuncs::ConvertPack(toStdString(pack), toStdString(name), toStdString(tmpOutput)))
		{
			if (this->outTypeCmbBox->SelectedIndex > 0) //.zip or .mcpack
			{
				String^ outFolder = tmpOutput + "\\" + name;
				String^ zipPath = output + "\\" + name;
				ConverterFuncs::Print("Compressing the zip file");
				if (System::IO::File::Exists(zipPath + this->outTypeCmbBox->Text))
				{ //if the file exists add "(1)", "(2)" etc like windows explorer
					if (MessageBox::Show("A file with the same name alread exists. Do you want to replace it?",
						"PCTexture2PE", MessageBoxButtons::YesNo,
						MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::No)
					{
						String^ tmp = zipPath;
						int errcount = 0;
						while (System::IO::File::Exists(tmp + this->outTypeCmbBox->Text))
						{
							tmp = zipPath;
							errcount++;
							tmp += " (" + errcount + ")";
						}
						zipPath = tmp;
					}
					else
					{
						System::IO::File::Delete(zipPath + this->outTypeCmbBox->Text);
					}
				}
				zip_api::compressAll(toStdString(outFolder),
					toStdString(zipPath + this->outTypeCmbBox->Text));
			}
			else
			{
				ConverterFuncs::Print("Moving the converted pack");
				String^ outfolder = output + "\\" + name;
				if (System::IO::Directory::Exists(outfolder))
				{
					if (MessageBox::Show("A folder with the same name alread exists. Do you want to replace it?",
						"PCTexture2PE", MessageBoxButtons::YesNo,
						MessageBoxIcon::Warning) == System::Windows::Forms::DialogResult::No)
					{
						String^ tmp = outfolder;
						int errcount = 0;
						while (System::IO::Directory::Exists(tmp))
						{
							tmp = outfolder;
							errcount++;
							tmp += " (" + errcount + ")";
						}
						outfolder = tmp;
					}
					else
					{
						System::IO::Directory::Delete(outfolder, true);
					}
				}
				System::IO::Directory::Move(tmpOutput + "\\" + name, outfolder);
			}
			ConverterFuncs::Print("Deleting temp files");
			System::IO::Directory::Delete(tmpOutput, true); //delete the tmp folder
			System::Media::SystemSounds::Asterisk->Play();
			ConverterFuncs::Print("Done!");
		}
		else
		{
			ConverterFuncs::Print("Faled to covert the pack. Check the files and try again");
		}
		Sleep(1000); //wait for the last log
		started = false;
	}
	private: System::Void inTypeCmbBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
		if (this->inTypeCmbBox->SelectedIndex == 0)
		{
			this->inputFile->Filter = L"pack.mcmeta|*.mcmeta";
		}
		else
		{
			this->inputFile->Filter = L"|*.zip";
		}
	}
};
}