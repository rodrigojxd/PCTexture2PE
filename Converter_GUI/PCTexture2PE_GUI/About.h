#pragma once

namespace PCTexture2PE_GUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for About
	/// </summary>
	public ref class About : public System::Windows::Forms::Form
	{
	public:
		About(void)
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
		~About()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  button10;
	private: System::Windows::Forms::Label^  copyRight;
	private: System::Windows::Forms::LinkLabel^  creatorLinkLabel;
	private: System::Windows::Forms::Label^  versionLabel;
	private: System::Windows::Forms::LinkLabel^  ForumLinkLabel;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(About::typeid));
			this->copyRight = (gcnew System::Windows::Forms::Label());
			this->creatorLinkLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->versionLabel = (gcnew System::Windows::Forms::Label());
			this->ForumLinkLabel = (gcnew System::Windows::Forms::LinkLabel());
			this->SuspendLayout();
			// 
			// copyRight
			// 
			this->copyRight->AutoSize = true;
			this->copyRight->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->copyRight->FlatStyle = System::Windows::Forms::FlatStyle::System;
			this->copyRight->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->copyRight->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->copyRight->Location = System::Drawing::Point(84, 194);
			this->copyRight->Name = L"copyRight";
			this->copyRight->Size = System::Drawing::Size(61, 17);
			this->copyRight->TabIndex = 1;
			this->copyRight->Text = L"(c) 2016";
			this->copyRight->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// creatorLinkLabel
			// 
			this->creatorLinkLabel->ActiveLinkColor = System::Drawing::SystemColors::ControlLight;
			this->creatorLinkLabel->AutoSize = true;
			this->creatorLinkLabel->Cursor = System::Windows::Forms::Cursors::Hand;
			this->creatorLinkLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->creatorLinkLabel->LinkColor = System::Drawing::SystemColors::ControlLightLight;
			this->creatorLinkLabel->Location = System::Drawing::Point(32, 23);
			this->creatorLinkLabel->Name = L"creatorLinkLabel";
			this->creatorLinkLabel->Size = System::Drawing::Size(157, 20);
			this->creatorLinkLabel->TabIndex = 11;
			this->creatorLinkLabel->TabStop = true;
			this->creatorLinkLabel->Text = L"Made by @rodrigojxd";
			this->creatorLinkLabel->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->creatorLinkLabel->VisitedLinkColor = System::Drawing::Color::White;
			this->creatorLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &About::creatorLinkLabel_LinkClicked);
			// 
			// versionLabel
			// 
			this->versionLabel->AutoSize = true;
			this->versionLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.5F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->versionLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->versionLabel->Location = System::Drawing::Point(65, 70);
			this->versionLabel->Name = L"versionLabel";
			this->versionLabel->Size = System::Drawing::Size(97, 16);
			this->versionLabel->TabIndex = 12;
			this->versionLabel->Text = L"Version: 1.0.2.0";
			// 
			// ForumLinkLabel
			// 
			this->ForumLinkLabel->ActiveLinkColor = System::Drawing::SystemColors::ControlLight;
			this->ForumLinkLabel->AutoSize = true;
			this->ForumLinkLabel->Cursor = System::Windows::Forms::Cursors::Hand;
			this->ForumLinkLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ForumLinkLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->ForumLinkLabel->LinkColor = System::Drawing::SystemColors::ControlLightLight;
			this->ForumLinkLabel->Location = System::Drawing::Point(9, 120);
			this->ForumLinkLabel->Name = L"ForumLinkLabel";
			this->ForumLinkLabel->Size = System::Drawing::Size(219, 17);
			this->ForumLinkLabel->TabIndex = 13;
			this->ForumLinkLabel->TabStop = true;
			this->ForumLinkLabel->Text = L"Official Topic On Minecraft Forum";
			this->ForumLinkLabel->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			this->ForumLinkLabel->VisitedLinkColor = System::Drawing::Color::White;
			this->ForumLinkLabel->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &About::ForumLinkLabel_LinkClicked);
			// 
			// About
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ControlDarkDark;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->ClientSize = System::Drawing::Size(234, 220);
			this->Controls->Add(this->ForumLinkLabel);
			this->Controls->Add(this->versionLabel);
			this->Controls->Add(this->creatorLinkLabel);
			this->Controls->Add(this->copyRight);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"About";
			this->Text = L"About";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void closeButton_Click(System::Object^  sender, System::EventArgs^  e) {
		Close();
	}
	private: System::Void creatorLinkLabel_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
		//ShellExecute(NULL, L"open", L"https://twitter.com/rodrigojxd", NULL, NULL, SW_SHOWNORMAL);
		Diagnostics::Process::Start("https://twitter.com/rodrigojxd");
	}
	private: System::Void ForumLinkLabel_LinkClicked(System::Object^  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^  e) {
		Diagnostics::Process::Start("http://www.minecraftforum.net/forums/minecraft-pocket-edition/mcpe-mods-tools/2756814-pctexture2pe-texture-converter-mcpc-to-mcpe-win10");
	}
	};
}
