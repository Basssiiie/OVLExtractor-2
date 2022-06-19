#pragma once
#include <Windows.h>
#include <vcclr.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <bitset>

//#include <d3d9.h>
//#include <d3dx9.h>

#include "OVLReader.h"
#include "SFStructs.h"

using namespace System::IO;
using namespace System::Diagnostics;

#pragma warning(disable:4244) // FPOS_T to LONG
#pragma warning(disable:4996) // WCSTOMBS

namespace OverlayExtractor2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	OVLReader OVL = OVLReader("null");
	std::vector<LinkedFiles> lf;
	ulong indexunique = 0;
	PreResolved resolvedsymbols;

	char* SIDTypes[45] = {
		"tree",
		"plant",
		"bush",
		"flowers",
		"fence",
		"wall misc",
		"path lamp",
		"scenery small",
		"scenery medium",
		"scenery large",
		"scenery anamatronic",
		"scenery misc",
		"support middle",
		"support top",
		"support bottom",
		"support bottom extra",
		"support girder",
		"support cap",
		"ride track",
		"path",
		"main entrance",
		"object",
		"guest inject",
		"ride",
		"ride entrance",
		"ride exit",
		"keep clear fence",
		"stall",
		"ride event",
		"firework",
		"litter bin",
		"bench",
		"sign",
		"photo point",
		"wall straight",
		"wall roof",
		"wall corner",
		"water cannon",
		"pool piece",
		"pool object",
		"changing room",
		"laser dome",
		"water jet",
		"terrain piece",
		"particle effect"
	};

	char* SIDSizeTypes[9] = { 
		"Full Tile",
		"Path Edge (inner)",
		"Path Edge (outer)",
		"Wall",
		"Quarter Tile",
		"Half Tile",
		"Path Center",
		"Corner",
		"Path Edge (join)" 
	};

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(array<System::String^>^ args)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			MessageBox::Show("WARNING!\r\n\r\nUse this tool at your own risc. You are fully responsible for what you do with this tool, you are reliable for whatever you do with it. Only use it for educational methods, DO NOT USE IT FOR ILLEGAL ACTIONS!");
			formtitle = "Overlay inspector public edition";
			if (args->Length > 0)
			{
				if (!args[0]->Contains(".ovl"))
				{
					MessageBox::Show("Error: File is no OVL");
					return;
				}
				std::string filepathandname;
				std::string safefilename;
				String^ result;
				if (args[0]->Contains(".unique.ovl"))
				{
					ToString(args[0]->Replace(".unique.ovl", ""), filepathandname);
					result = Path::GetFileName(args[0]->Replace(".unique.ovl", ".common.ovl"));
				}
				else
				{
					ToString(args[0]->Replace(".common.ovl", ""), filepathandname);
					result = Path::GetFileName(args[0]);
				}
				ToString(result, safefilename);
				OVL = OVLReader(filepathandname);
				OVL.safefilename = safefilename;
				this->Text = formtitle + " | " + result;
				if (!OVL.IsValid())
				{
					std::string err = OVL.GetLastReadingError();
					MessageBox::Show("Error: Could not open Overlay.\r\r\n" + gcnew String(err.c_str()));
					return;
				}

				itemlist->Items->Clear();
				itemlist->BeginUpdate();
				indexunique = 0;
				for (ulong j = 0; j < 2; j++)
				{
					lf = OVL.GetRawLinkedFiles(j);
					for (ulong i = 0; i < lf.size(); i++)
					{
						String^ str;
						std::string symbol = OVL.GetStringFromOffset(lf[i].symbolresolve.stringpointer);
						Loader ldr = OVL.GetLoaderByID(lf[i].loaderreference.loadernumber, C_OVL);
						str = gcnew String(ldr.tag.c_str()) + "  |  " + gcnew String(symbol.c_str());

						/*if(symbol == "STRINGNOTFOUND")
						{
						Debug::WriteLine("Stringpointer " + lf[i].loaderreference.symbolstructpointer + gcnew String(ldr.tag.c_str()));
						}*/
						itemlist->Items->Add(str);
						if (j == 0)
						{
							indexunique = i;
						}
					}
				}
				itemlist->EndUpdate();
				DoAfterLoading();
			}
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ openOVLToolStripMenuItem;
	private: System::Windows::Forms::ListBox^ itemlist;

	private: System::Windows::Forms::ToolStripMenuItem^ dumpOVLToolStripMenuItem;
	private: System::Windows::Forms::GroupBox^ groupBox1;
	private: System::Windows::Forms::CheckBox^ do_showincomingrelocations;
	private: System::Windows::Forms::CheckBox^ do_resolves;
	private: System::Windows::Forms::ToolStripMenuItem^ commonToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ uniqueToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ bothToolStripMenuItem;
	private: System::Windows::Forms::ListBox^ stringlist;


	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ListBox^ loaderlist;

	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button6;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::ListBox^ symbolresolvelist;
	private: System::Windows::Forms::Label^ ovlinformation;
	private: System::Windows::Forms::ToolStripMenuItem^ exportOVLMakeXMLToolStripMenuItem;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::ListBox^ referencelist;
	private: System::Windows::Forms::GroupBox^ groupBox2;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::ToolStripMenuItem^ decompileAllItemsToolStripMenuItem;

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->openOVLToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->dumpOVLToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->commonToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->uniqueToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->bothToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exportOVLMakeXMLToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->itemlist = (gcnew System::Windows::Forms::ListBox());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->do_resolves = (gcnew System::Windows::Forms::CheckBox());
			this->do_showincomingrelocations = (gcnew System::Windows::Forms::CheckBox());
			this->stringlist = (gcnew System::Windows::Forms::ListBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->loaderlist = (gcnew System::Windows::Forms::ListBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->symbolresolvelist = (gcnew System::Windows::Forms::ListBox());
			this->ovlinformation = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->referencelist = (gcnew System::Windows::Forms::ListBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->decompileAllItemsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->menuStrip1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->openOVLToolStripMenuItem,
					this->dumpOVLToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->RenderMode = System::Windows::Forms::ToolStripRenderMode::System;
			this->menuStrip1->Size = System::Drawing::Size(835, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// openOVLToolStripMenuItem
			// 
			this->openOVLToolStripMenuItem->Name = L"openOVLToolStripMenuItem";
			this->openOVLToolStripMenuItem->Size = System::Drawing::Size(89, 20);
			this->openOVLToolStripMenuItem->Text = L"Open overlay";
			this->openOVLToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openOVLToolStripMenuItem_Click);
			// 
			// dumpOVLToolStripMenuItem
			// 
			this->dumpOVLToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(6) {
				this->commonToolStripMenuItem,
					this->uniqueToolStripMenuItem, this->toolStripSeparator1, this->bothToolStripMenuItem, this->exportOVLMakeXMLToolStripMenuItem,
					this->decompileAllItemsToolStripMenuItem
			});
			this->dumpOVLToolStripMenuItem->Name = L"dumpOVLToolStripMenuItem";
			this->dumpOVLToolStripMenuItem->Size = System::Drawing::Size(93, 20);
			this->dumpOVLToolStripMenuItem->Text = L"Dump overlay";
			this->dumpOVLToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::dumpOVLToolStripMenuItem_Click);
			// 
			// commonToolStripMenuItem
			// 
			this->commonToolStripMenuItem->Name = L"commonToolStripMenuItem";
			this->commonToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->commonToolStripMenuItem->Text = L"Common";
			this->commonToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::commonToolStripMenuItem_Click);
			// 
			// uniqueToolStripMenuItem
			// 
			this->uniqueToolStripMenuItem->Name = L"uniqueToolStripMenuItem";
			this->uniqueToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->uniqueToolStripMenuItem->Text = L"Unique";
			this->uniqueToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::uniqueToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(185, 6);
			// 
			// bothToolStripMenuItem
			// 
			this->bothToolStripMenuItem->Name = L"bothToolStripMenuItem";
			this->bothToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->bothToolStripMenuItem->Text = L"Both";
			this->bothToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::bothToolStripMenuItem_Click);
			// 
			// exportOVLMakeXMLToolStripMenuItem
			// 
			this->exportOVLMakeXMLToolStripMenuItem->Name = L"exportOVLMakeXMLToolStripMenuItem";
			this->exportOVLMakeXMLToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->exportOVLMakeXMLToolStripMenuItem->Text = L"Export OVLMake XML";
			this->exportOVLMakeXMLToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exportOVLMakeXMLToolStripMenuItem_Click);
			// 
			// itemlist
			// 
			this->itemlist->FormattingEnabled = true;
			this->itemlist->IntegralHeight = false;
			this->itemlist->Location = System::Drawing::Point(12, 52);
			this->itemlist->Name = L"itemlist";
			this->itemlist->Size = System::Drawing::Size(412, 181);
			this->itemlist->TabIndex = 1;
			this->itemlist->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::itemlist_SelectedIndexChanged);
			this->itemlist->DoubleClick += gcnew System::EventHandler(this, &Form1::itemlist_DoubleClick);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->do_resolves);
			this->groupBox1->Controls->Add(this->do_showincomingrelocations);
			this->groupBox1->Location = System::Drawing::Point(12, 380);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(742, 45);
			this->groupBox1->TabIndex = 2;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Dump options";
			// 
			// do_resolves
			// 
			this->do_resolves->AutoSize = true;
			this->do_resolves->Checked = true;
			this->do_resolves->CheckState = System::Windows::Forms::CheckState::Checked;
			this->do_resolves->Location = System::Drawing::Point(171, 19);
			this->do_resolves->Name = L"do_resolves";
			this->do_resolves->Size = System::Drawing::Size(241, 17);
			this->do_resolves->TabIndex = 1;
			this->do_resolves->Text = L"Resolve symbol resolves as incoming pointers";
			this->do_resolves->UseVisualStyleBackColor = true;
			// 
			// do_showincomingrelocations
			// 
			this->do_showincomingrelocations->AutoSize = true;
			this->do_showincomingrelocations->Checked = true;
			this->do_showincomingrelocations->CheckState = System::Windows::Forms::CheckState::Checked;
			this->do_showincomingrelocations->Location = System::Drawing::Point(6, 19);
			this->do_showincomingrelocations->Name = L"do_showincomingrelocations";
			this->do_showincomingrelocations->Size = System::Drawing::Size(152, 17);
			this->do_showincomingrelocations->TabIndex = 0;
			this->do_showincomingrelocations->Text = L"Show incoming relocations";
			this->do_showincomingrelocations->UseVisualStyleBackColor = true;
			// 
			// stringlist
			// 
			this->stringlist->FormattingEnabled = true;
			this->stringlist->IntegralHeight = false;
			this->stringlist->Location = System::Drawing::Point(431, 52);
			this->stringlist->Name = L"stringlist";
			this->stringlist->Size = System::Drawing::Size(392, 181);
			this->stringlist->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(428, 30);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(46, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Stringlist";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(9, 30);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(38, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"File list";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(684, 27);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(139, 19);
			this->button3->TabIndex = 8;
			this->button3->Text = L"Copy stringlist to clipboard";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(285, 27);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(139, 19);
			this->button4->TabIndex = 9;
			this->button4->Text = L"Copy itemlist to clipboard";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// loaderlist
			// 
			this->loaderlist->FormattingEnabled = true;
			this->loaderlist->IntegralHeight = false;
			this->loaderlist->Location = System::Drawing::Point(158, 264);
			this->loaderlist->Name = L"loaderlist";
			this->loaderlist->Size = System::Drawing::Size(267, 113);
			this->loaderlist->TabIndex = 10;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(155, 242);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(55, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Loader list";
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(273, 239);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(149, 19);
			this->button5->TabIndex = 12;
			this->button5->Text = L"Copy loaderlist to clipboard";
			this->button5->UseVisualStyleBackColor = true;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::button5_Click);
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(552, 239);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(57, 19);
			this->button6->TabIndex = 15;
			this->button6->Text = L"Copy";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::button6_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(428, 242);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(93, 13);
			this->label4->TabIndex = 14;
			this->label4->Text = L"Symbol resolve list";
			// 
			// symbolresolvelist
			// 
			this->symbolresolvelist->FormattingEnabled = true;
			this->symbolresolvelist->IntegralHeight = false;
			this->symbolresolvelist->Location = System::Drawing::Point(431, 264);
			this->symbolresolvelist->Name = L"symbolresolvelist";
			this->symbolresolvelist->Size = System::Drawing::Size(178, 113);
			this->symbolresolvelist->TabIndex = 13;
			// 
			// ovlinformation
			// 
			this->ovlinformation->AutoSize = true;
			this->ovlinformation->Location = System::Drawing::Point(12, 242);
			this->ovlinformation->MaximumSize = System::Drawing::Size(150, 0);
			this->ovlinformation->Name = L"ovlinformation";
			this->ovlinformation->Size = System::Drawing::Size(115, 26);
			this->ovlinformation->TabIndex = 16;
			this->ovlinformation->Text = L"No overlay information available";
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(766, 239);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(57, 19);
			this->button1->TabIndex = 19;
			this->button1->Text = L"Copy";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(612, 242);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(62, 13);
			this->label5->TabIndex = 18;
			this->label5->Text = L"References";
			// 
			// referencelist
			// 
			this->referencelist->FormattingEnabled = true;
			this->referencelist->IntegralHeight = false;
			this->referencelist->Location = System::Drawing::Point(615, 264);
			this->referencelist->Name = L"referencelist";
			this->referencelist->Size = System::Drawing::Size(208, 113);
			this->referencelist->TabIndex = 17;
			this->referencelist->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::referencelist_SelectedIndexChanged);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->button2);
			this->groupBox2->Location = System::Drawing::Point(15, 431);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(742, 45);
			this->groupBox2->TabIndex = 3;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Offset changer";
			this->groupBox2->Visible = false;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(6, 19);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(149, 20);
			this->button2->TabIndex = 0;
			this->button2->Text = L"Change preset offsets";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(15, 364);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(63, 13);
			this->label6->TabIndex = 20;
			this->label6->Text = L"V2; build 88";
			// 
			// decompileAllItemsToolStripMenuItem
			// 
			this->decompileAllItemsToolStripMenuItem->Name = L"decompileAllItemsToolStripMenuItem";
			this->decompileAllItemsToolStripMenuItem->Size = System::Drawing::Size(188, 22);
			this->decompileAllItemsToolStripMenuItem->Text = L"Decompile all items";
			this->decompileAllItemsToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::decompileAllItemsToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(835, 431);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->referencelist);
			this->Controls->Add(this->ovlinformation);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->symbolresolvelist);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->loaderlist);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->stringlist);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->itemlist);
			this->Controls->Add(this->menuStrip1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Overlay inspector";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	public: String^ formtitle;

	private: bool ToChar(String^ source, char*& target)
	{
		pin_ptr<const wchar_t> wch = PtrToStringChars(source);
		int len = ((source->Length + 1) * 2);
		target = new char[len];
		return wcstombs(target, wch, len) != -1;
	}

	private: bool ToString(String^ source, std::string& target)
	{
		pin_ptr<const wchar_t> wch = PtrToStringChars(source);
		int len = ((source->Length + 1) * 2);
		char* ch = new char[len];
		bool result = wcstombs(ch, wch, len) != -1;
		target = ch;
		delete ch;
		return result;
	}

		   //Zonder pointer comment
	private: void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, ulong data, std::string comment, bool isreloc)
	{
		//TConvert tc;tc.ulong = data;
		fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s%d(=$%X) %s", pos, offset, "long", name.c_str(), (isreloc) ? "-> " : "", data, data, comment.c_str());
		if (isreloc)
			fprintf(dumpfile, "<P-OUT>");
		else if (do_resolves->Checked) {
			if (resolvedsymbols.offset <= offset)
			{
				fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
				PositionReturn pr = OVL.OffsetToPosition(offset);
				OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
				resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

			}
		}
		fprintf(dumpfile, "\r\n");
	}
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, ushort data, std::string comment, bool isreloc)
		   {
			   //TConvert tc;tc.ulong = data;
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s%d(=$%X) %s", pos, offset, "short", name.c_str(), (isreloc) ? "-> " : "", data, data, comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "<@P-OUT>");
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, float data, std::string comment, bool isreloc)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s%f %s", pos, offset, "float", name.c_str(), (isreloc) ? "-> " : "", data, comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "<P-OUT>");
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, std::string data, std::string comment, bool isreloc)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s %s", pos, offset, "text", name.c_str(), data.c_str(), comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "<P-OUT>");
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }

		   //Met pointer comment
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, ulong data, std::string comment, bool isreloc, std::string reloccomment)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s%d(=$%X) %s", pos, offset, "long", name.c_str(), (isreloc) ? "-> " : "", data, data, comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "(*P %s)", reloccomment.c_str());
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, ushort data, std::string comment, bool isreloc, std::string reloccomment)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s%d(=$%X) %s", pos, offset, "short", name.c_str(), (isreloc) ? "-> " : "", data, data, comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "(*P %s)", reloccomment.c_str());
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, float data, std::string comment, bool isreloc, std::string reloccomment)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s%f(=$%X) %s", pos, offset, "float", name.c_str(), (isreloc) ? "-> " : "", data, data, comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "(*P %s)", reloccomment.c_str());
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, std::string data, std::string comment, bool isreloc, std::string reloccomment)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s %s", pos, offset, "text", name.c_str(), data.c_str(), comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "(*P %s)", reloccomment.c_str());
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }

		   //Met pointercomment en type
		   void PostVar(FILE*& dumpfile, ulong pos, ulong offset, std::string name, ulong data, std::string type, std::string comment, bool isreloc, std::string reloccomment)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s%d(=$%X) %s", pos, offset, type.c_str(), name.c_str(), (isreloc) ? "-> " : "", data, data, comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "(*P %s)", reloccomment.c_str());
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }

		   //Unknown met pointercomment en type
		   void PostVarUnknown(FILE*& dumpfile, ulong pos, ulong offset, std::string name, TConvert data, std::string type, std::string comment, bool isreloc, std::string reloccomment)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  %s($%X ($%X $%X $%X $%X) == %d) %s", pos, offset, type.c_str(), name.c_str(), (isreloc) ? "-> " : "", data.ul, data.c[0], data.c[1], data.c[2], data.c[3], data.ul, comment.c_str());
			   if (isreloc)
				   fprintf(dumpfile, "(*P %s)", reloccomment.c_str());
			   else if (do_resolves->Checked) {
				   if (resolvedsymbols.offset <= offset)
				   {
					   fprintf(dumpfile, "<P-IN@%X: %s>", offset, resolvedsymbols.name.c_str());
					   PositionReturn pr = OVL.OffsetToPosition(offset);
					   OVLData OVLDT = OVL.GetOVLD(pr.currentOVL);
					   resolvedsymbols = ResolveSymbolResolves(OVLDT.symbolresolves, offset);

				   }
			   }
			   fprintf(dumpfile, "\r\n");
		   }

		   void DumpVertex(FILE*& dumpfile, ulong pos, ulong offset, std::string name, MorhpMeshVertex mmv, std::string type, std::string comment, bool isreloc)
		   {
			   fprintf(dumpfile, "%-5.5X  %-5.5X  %-5.5s  %40.40s  X%d Y%d Z%d (X$%X Y$%X Z$%X) %s", pos, offset, type.c_str(), name.c_str(), mmv.X, mmv.Y, mmv.Z, mmv.X, mmv.Y, mmv.Z, comment.c_str());
			   fprintf(dumpfile, "\r\n");
		   }

		   bool is_bit_set(ulong value, ulong bitindex)
		   {
			   return (value & (1 << bitindex)) != 0;
		   }

		   void DumpBitFlags(FILE*& html, ulong data)
		   {
			   for (int z = 0; z < 32; z++)
				   if (is_bit_set(data, z))
					   fprintf(html, "									Flag %d set\r\n", (z + 1));
			   fprintf(html, "\r\n");
		   }

		   void PostMDLEnum(FILE*& html, ulong data)
		   {
			   MDLUnknownEnum MDLUE;
			   if (data & MDLUE.Unknown_1)
				   fprintf(html, "                                                                               Flag 1 set\r\n");
			   if (data & MDLUE.Unknown_2)
				   fprintf(html, "                                                                               Flag 2 set\r\n");
			   if (data & MDLUE.Unknown_3)
				   fprintf(html, "                                                                               Flag 3 set\r\n");
			   if (data & MDLUE.Unknown_4)
				   fprintf(html, "                                                                               Flag 4 set\r\n");
			   if (data & MDLUE.Unknown_5)
				   fprintf(html, "                                                                               Flag 5 set\r\n");
			   if (data & MDLUE.Unknown_6)
				   fprintf(html, "                                                                               Flag 6 set\r\n");
			   if (data & MDLUE.Unknown_7)
				   fprintf(html, "                                                                               Flag 7 set\r\n");
			   if (data & MDLUE.Unknown_8)
				   fprintf(html, "                                                                               Flag 8 set\r\n");
			   if (data & MDLUE.Unknown_9)
				   fprintf(html, "                                                                               Flag 9 set\r\n");
			   if (data & MDLUE.Unknown_10)
				   fprintf(html, "                                                                               Flag 10 set\r\n");
			   if (data & MDLUE.Unknown_11)
				   fprintf(html, "                                                                               Flag 11 set\r\n");
			   if (data & MDLUE.Unknown_12)
				   fprintf(html, "                                                                               Flag 12 set\r\n");
			   if (data & MDLUE.Unknown_13)
				   fprintf(html, "                                                                               Flag 13 set\r\n");
			   if (data & MDLUE.Unknown_14)
				   fprintf(html, "                                                                               Flag 14 set\r\n");
			   if (data & MDLUE.Unknown_15)
				   fprintf(html, "                                                                               Flag 15 set\r\n");
			   if (data & MDLUE.Unknown_16)
				   fprintf(html, "                                                                               Flag 16 set\r\n");
			   if (data & MDLUE.Unknown_17)
				   fprintf(html, "                                                                               Flag 17 set\r\n");
			   if (data & MDLUE.Unknown_18)
				   fprintf(html, "                                                                               Flag 18 set\r\n");
			   if (data & MDLUE.Unknown_19)
				   fprintf(html, "                                                                               Flag 19 set\r\n");
			   if (data & MDLUE.Unknown_20)
				   fprintf(html, "                                                                               Flag 20 set\r\n");
			   for (int z = 19; z < 32; z++)
				   if (is_bit_set(data, z))
					   fprintf(html, "                                           Flag %d set\r\n", (z + 1));
			   fprintf(html, "\r\n");
		   }

		   std::string returnHeader(std::string naam) {
			   std::string tijdelijk = "\r\n======================================================\r\n" + naam + "\r\n======================================================\r\n";
			   return tijdelijk;
		   }

		   std::string GetStringA(FILE*& ovl)
		   {
			   ulong size = 0;
			   fpos_t readpos;
			   fgetpos(ovl, &readpos);
			   ulong temp = 0;
			   ulong i = 0;
			   std::string returnstring;

			   do {
				   fread(&temp, 1, 1, ovl);
				   size++;
			   } while (temp != 0 && i < 1000);

			   fseek(ovl, readpos, SEEK_SET);
			   char* str = new char[size + 1];
			   str[size] = 0;
			   fread(str, size, 1, ovl);

			   returnstring = str;
			   delete[] str;
			   return returnstring;
		   }

		   std::wstring GetStringW(FILE*& ovl)
		   {
			   ulong size = 0;
			   fpos_t readpos;
			   fgetpos(ovl, &readpos);
			   ulong temp = 0;
			   ulong i = 0;
			   std::wstring returnstring;

			   bool nulled = false;
			   bool nulled2 = false;
			   do {
				   fread(&temp, 1, 1, ovl);
				   if (temp == 0 && nulled == false)
				   {
					   nulled = true;
					   size++;
				   }
				   else if (temp == 0 && nulled == true)
				   {
					   nulled2 = true;
				   }
				   else {
					   nulled = false;
					   size++;
				   }
			   } while (!nulled2 && i < 1000);

			   fseek(ovl, readpos, SEEK_SET);
			   wchar_t* str = new wchar_t[size];
			   str[size] = 0;
			   fread(str, (size + 2), 1, ovl);

			   returnstring = str;
			   //delete[] str;
			   return returnstring;
		   }

		   void ValidateXMLString(std::string& str)
		   {
			   size_t pos;
			   pos = str.find("'");
			   while (pos != std::string::npos)
			   {
				   str.replace(pos, 1, "&apos;");
				   pos = str.find("'");
			   }

			   /*pos = str.find("\"");
			   while (pos != std::string::npos)
			   {
			   str.replace(pos, 1, "&quot;");
			   pos = str.find("\"");
			   }

			   pos = str.find("&");
			   while (pos != std::string::npos)
			   {
			   str.replace(pos, 1, "&amp;");
			   pos = str.find("&");
			   }

			   pos = str.find("<");
			   while (pos != std::string::npos)
			   {
			   str.replace(pos, 1, "&lt;");
			   pos = str.find("<");
			   }

			   pos = str.find(">");
			   while (pos != std::string::npos)
			   {
			   str.replace(pos, 1, "&gt;");
			   pos = str.find(">");
			   }*/
		   }

	private: System::Void openOVLToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		OpenFileDialog^ ofd = gcnew OpenFileDialog();
		ofd->CheckFileExists = true;
		std::string safefilename;
		std::string filepathandname;
		try {
			ofd->Filter = "Overlay|*.common.ovl|Overlay v6|*.ovl";
			ofd->Title = "Selecteer een Overlay bestand";
			if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
				ToString(ofd->FileName->Replace(".common.ovl", ""), filepathandname);
				ToString(ofd->SafeFileName, safefilename);
				this->Text = formtitle + " | " + ofd->SafeFileName;
			}
			else {
				return;
			}
		}
		finally {
			delete ofd;
		}

		OVL = OVLReader(filepathandname);
		OVL.safefilename = safefilename;
		if (!OVL.IsValid())
		{
			std::string err = OVL.GetLastReadingError();
			MessageBox::Show("Error: Could not open Overlay.\r\r\n" + gcnew String(err.c_str()));
			return;
		}

		itemlist->Items->Clear();
		itemlist->BeginUpdate();
		indexunique = 0;
		for (ulong j = 0; j < 2; j++)
		{
			lf = OVL.GetRawLinkedFiles(j);
			for (ulong i = 0; i < lf.size(); i++)
			{
				String^ str;
				std::string symbol = OVL.GetStringFromOffset(lf[i].symbolresolve.stringpointer);
				Loader ldr = OVL.GetLoaderByID(lf[i].loaderreference.loadernumber, C_OVL);
				str = gcnew String(ldr.tag.c_str()) + "  |  " + gcnew String(symbol.c_str());

				/*if(symbol == "STRINGNOTFOUND")
				{
				Debug::WriteLine("Stringpointer " + lf[i].loaderreference.symbolstructpointer + gcnew String(ldr.tag.c_str()));
				}*/
				itemlist->Items->Add(str);
				if (j == 0)
				{
					indexunique = i;
				}
			}
		}
		itemlist->EndUpdate();
		DoAfterLoading();
	}



	private: System::Void itemlist_DoubleClick(System::Object^ sender, System::EventArgs^ e) {
		ulong selectedindex = itemlist->SelectedIndex;
		DecompileItem(selectedindex);
	}

		   void DecompileItem(ulong selectedindex)
		   {
			   if (selectedindex == -1)
			   {
				   Debug::WriteLine("Selected index error");
				   return;
			   }
			   //Debug::WriteLine(selectedindex + " / " + indexunique);
			   Debug::WriteLine("Common? " + (selectedindex <= indexunique && indexunique != 0));
			   if (selectedindex <= indexunique && indexunique != 0)
				   lf = OVL.GetRawLinkedFiles(0);
			   else {
				   lf = OVL.GetRawLinkedFiles(1);
				   selectedindex -= indexunique;
				   //Debug::WriteLine("File unique");
			   }

			   std::string symbol = OVL.GetStringFromOffset(lf[selectedindex].symbolresolve.stringpointer);

			   Loader ldr = OVL.GetLoaderByID(lf[selectedindex].loaderreference.loadernumber, C_OVL);

			   PositionReturn pr = OVL.OffsetToPosition(lf[selectedindex].loaderreference.datapointer);
			   OVLData OVLD = OVL.GetOVLD(pr.currentOVL);
			   //Debug::WriteLine("currentOVL position " + pr.position + " in OVL " + pr.currentOVL + "@" + gcnew String(symbol.c_str()));

			   FILE* ovl;
			   FILE* dmp;
			   std::string dumpname = symbol;
			   dumpname.erase((dumpname.end() - ldr.tag.length() - 1), dumpname.end());

			   if (fopen_s(&ovl, OVLD.ovlname.c_str(), "rb") == EINVAL)
			   {
				   MessageBox::Show("Overlay could not be opened");
				   return;
			   }
			   Debug::WriteLine(gcnew String(dumpname.c_str()));

			   fseek(ovl, pr.position, SEEK_SET);

			   //Data dump
			   if (ldr.tag == "txt")
			   {
				   dumpname.append(".txt");
				   if (fopen_s(&dmp, dumpname.c_str(), "wb") != EINVAL)
				   {
					   std::wstring tstr = GetStringW(ovl);
					   fwrite(tstr.c_str(), tstr.length() * 2, 1, dmp);
					   //MessageBox::Show("File saved");
					   fclose(dmp);
				   }
				   else {
					   MessageBox::Show("Dumpfile could not be opened");
				   }
			   }
			   else if (ldr.tag == "snd") {
				   Sound snd;
				   FILE* newovl;
				   bool fail = false;
				   std::vector<short> channel1data;
				   std::vector<short> channel2data;

				   fread(&snd, sizeof(Sound), 1, ovl);
				   Debug::WriteLine("Size 1: " + snd.channel1_size);
				   Debug::WriteLine("Size 2: " + snd.channel2_size);
				   Debug::WriteLine("Channels: " + snd.fmt.numchannels);
				   Debug::WriteLine("Loop: " + snd.loop);
				   dumpname.append(".wav");
				   PositionReturn pr = OVL.OffsetToPosition(snd.channel1);
				   OVLD = OVL.GetOVLD(pr.currentOVL);
				   if (fopen_s(&newovl, OVLD.ovlname.c_str(), "rb") != EINVAL)
				   {
					   fseek(newovl, pr.position, SEEK_SET);
					   for (long z = 0; z < snd.channel1_size / 2; z++)
					   {
						   short tshort;
						   fread(&tshort, 2, 1, newovl);
						   channel1data.push_back(tshort);
					   }
				   }
				   fclose(newovl);
				   pr = OVL.OffsetToPosition(snd.channel2);
				   OVLD = OVL.GetOVLD(pr.currentOVL);
				   if (snd.fmt.numchannels == 2)
				   {
					   if (fopen_s(&newovl, OVLD.ovlname.c_str(), "rb") != EINVAL)
					   {
						   fseek(newovl, pr.position, SEEK_SET);
						   for (long z = 0; z < snd.channel2_size / 2; z++)
						   {
							   short tshort;
							   fread(&tshort, 2, 1, newovl);
							   channel1data.push_back(tshort);
						   }
					   }
				   }
				   fclose(newovl);
				   if (fopen_s(&dmp, dumpname.c_str(), "wb") != EINVAL && fail == false)
				   {
					   ulong utemp = 36 + snd.channel1_size + snd.channel2_size;
					   fprintf(dmp, "RIFF");
					   fwrite(&utemp, 4, 1, dmp);
					   fprintf(dmp, "WAVE");
					   fprintf(dmp, "fmt ");
					   utemp = 16;
					   fwrite(&utemp, 4, 1, dmp);
					   fwrite(&snd.fmt.tag, 2, 1, dmp);
					   fwrite(&snd.fmt.numchannels, 2, 1, dmp);
					   fwrite(&snd.fmt.samplerate, 4, 1, dmp);
					   fwrite(&snd.fmt.byterate, 4, 1, dmp);
					   fwrite(&snd.fmt.blockalign, 2, 1, dmp);
					   fwrite(&snd.fmt.bitspersample, 2, 1, dmp);

					   fprintf(dmp, "data");
					   utemp = snd.channel1_size + snd.channel2_size;
					   fwrite(&utemp, 4, 1, dmp);
					   if (snd.fmt.numchannels == 1)
					   {
						   for (unsigned long z = 0; z < channel1data.size(); z++)
						   {
							   fwrite(&channel1data[z], 2, 1, dmp);
						   }
					   }
					   else {
						   ulong writtena = 0;
						   ulong writtenb = 0;
						   for (long z = 0; z < (snd.channel2_size / (snd.fmt.blockalign * snd.fmt.bitspersample)); z++)
						   {
							   for (long y = 0; y < snd.fmt.blockalign * (snd.fmt.bitspersample / 8); y++)
							   {
								   fwrite(&channel1data[writtena], 2, 1, newovl);
								   writtena++;
							   }
							   for (long y = 0; y < snd.fmt.blockalign * (snd.fmt.bitspersample / 8); y++)
							   {
								   fwrite(&channel2data[writtenb], 2, 1, newovl);
								   writtenb++;
							   }
						   }
					   }
					   //MessageBox::Show("File saved");
					   fclose(dmp);
				   }
				   else {
					   MessageBox::Show("Dumpfile could not be opened or an error was thrown");
				   }
			   }
			   else {
				   MessageBox::Show("This type currently can't be exported");
			   }
			   fclose(ovl);
		   }
	private: System::Void dumpOVLToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		//DumpOVL(0);
		//DumpOVL(1);
	}

		   void DumpOVL(ulong currentOVL)
		   {
			   if (!OVL.IsValid())
			   {
				   std::string err = OVL.GetLastReadingError();
				   MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
				   return;
			   }
			   FILE* html;
			   FILE* ovl;
			   ulong i, j, k;
			   fpos_t position;
			   OVLData OVLD = OVL.GetOVLD(currentOVL);
			   ulong ovlversion = OVLD.h1.version;
			   PositionReturn pr = OVL.OffsetToPosition(0);

			   std::string filename = "OverlayDump_";
			   std::string ovlname = OVL.safefilename;
			   ovlname.erase(ovlname.end() - 11, ovlname.end());
			   filename.append(ovlname);
			   if (currentOVL == 0)
				   filename.append("_common.txt");
			   else
				   filename.append("_unique.txt");
			   //filename.append(".txt");

			   if (fopen_s(&ovl, OVLD.ovlname.c_str(), "rb") == EINVAL)
			   {
				   MessageBox::Show("Overlay could not be opened");
				   return;
			   }
			   if (fopen_s(&html, filename.c_str(), "wb") == EINVAL)
			   {
				   fclose(ovl);
				   Debug::WriteLine(gcnew String(filename.c_str()));
				   MessageBox::Show("OverlayDump could not be opened");
				   return;
			   }

			   fseek(ovl, pr.position, SEEK_SET);
			   //Offsets
			   ulong reloc = GetNearestRelocation(OVLD.relocations, 0);
			   ulong relocpointto = GetNearestRelocation(OVLD.relocationspointingto, 0);
			   LinkedFiles data = GetNearestData(OVLD.linkedfiles, 0);
			   LoadReference stringlessdata = GetNearestStringlessData(OVLD.loaderreference, 0);//FLIC, BmpTbl etc
			   PreResolved presolveddata = GetNearestFurtherData(OVLD.presolvedfurtherdata, 0);//Extended dada
			   resolvedsymbols = ResolveSymbolResolves(OVLD.symbolresolves, 0);

			   ulong currentoffset = 0;
			   //fprintf(html, "<pre>\r\n");
			   fprintf(html, "\r\nOverlay dump of %s\r\n======================================================\r\n", OVLD.ovlname.c_str());
			   for (j = 0; j < OVLD.references.size(); j++)
			   {
				   fprintf(html, "Reference to %s\r\n", OVLD.references[j].file.c_str());
			   }
			   if (OVLD.references.size() > 0)
				   fprintf(html, "\r\n");
			   for (i = 0; i < 9; i++)
			   {
				   for (j = 0; j < OVLD.chunks[i].num_blocks; j++)
				   {
					   ulong offsetend = OVLD.chunks[i].blocks[j].internal_offset + OVLD.chunks[i].blocks[j].size;
					   ulong positionend = OVLD.chunks[i].blocks[j].position + OVLD.chunks[i].blocks[j].size;
					   fprintf(html, "Chunk %d/9 part %d/%d: $%X(%d)-$%X(%d)\r\n", (i + 1), (j + 1), OVLD.chunks[i].num_blocks, OVLD.chunks[i].blocks[j].internal_offset, OVLD.chunks[i].blocks[j].internal_offset, offsetend, offsetend);
					   //fprintf(html, "//Internal offset: $%X(%d) - $%X(%d)\r\n", OVLD.chunks[i].blocks[j].internal_offset, OVLD.chunks[i].blocks[j].internal_offset,offsetend,offsetend);
					   //fprintf(html, "//Part position: $%X(%d) - $%X(%d)\r\n", position, position, positionend, positionend);
					   //fprintf(html, "//Block length: $%X(%d)\r\n", offsetend-OVLD.chunks[i].blocks[j].internal_offset,offsetend-OVLD.chunks[i].blocks[j].internal_offset);
				   }
			   }

			   fgetpos(ovl, &position);
			   for (i = 0; i < 9; i++)
			   {
				   for (j = 0; j < OVLD.chunks[i].num_blocks; j++)
				   {
					   if (currentoffset != OVLD.chunks[i].blocks[j].internal_offset)
					   {
						   Debug::WriteLine("Offset mismatch");// + (OVLD.chunks[i].blocks[j].internal_offset - currentoffset) + " bytes (-= teveel += teweinig");
					   }
					   currentoffset = OVLD.chunks[i].blocks[j].internal_offset;

					   data = GetNearestData(OVLD.linkedfiles, currentoffset - 1);
					   stringlessdata = GetNearestStringlessData(OVLD.loaderreference, currentoffset - 1);
					   relocpointto = GetNearestRelocationPointTo(OVLD.relocationspointingto, currentoffset - 1);
					   presolveddata = GetNearestFurtherData(OVLD.presolvedfurtherdata, currentoffset - 1);
					   resolvedsymbols = ResolveSymbolResolves(OVLD.symbolresolves, currentoffset - 1);

					   ulong ultemp;
					   ushort ustemp;
					   float ftemp;
					   ulong currentblocksize = OVLD.chunks[i].blocks[j].size;
					   fseek(ovl, OVLD.chunks[i].blocks[j].position, SEEK_SET);
					   fgetpos(ovl, &position);
					   fprintf(html, "%sChunk %d/9 part %d/%d%s", "\r\n======================================================\r\n", (i + 1), (j + 1), OVLD.chunks[i].num_blocks, "\r\n======================================================\r\n");
					   ulong offsetend = OVLD.chunks[i].blocks[j].internal_offset + OVLD.chunks[i].blocks[j].size;
					   ulong positionend = OVLD.chunks[i].blocks[j].position + OVLD.chunks[i].blocks[j].size;
					   fprintf(html, "//Internal offset: $%X(%d) - $%X(%d)\r\n", OVLD.chunks[i].blocks[j].internal_offset, OVLD.chunks[i].blocks[j].internal_offset, offsetend, offsetend);
					   fprintf(html, "//Part position: $%X(%d) - $%X(%d)\r\n", OVLD.chunks[i].blocks[j].position, OVLD.chunks[i].blocks[j].position, positionend, positionend);
					   fprintf(html, "//Block length: $%X(%d)\r\n", offsetend - OVLD.chunks[i].blocks[j].internal_offset, offsetend - OVLD.chunks[i].blocks[j].internal_offset);
					   //PostVar(FILE *&dumpfile, ulong pos, ulong offset, std::string name, ulong data, std::string comment, bool isreloc, std::string relocdata)

					   if (i == 0 && j == 0 && currentoffset < stringlessdata.datapointer && currentoffset < presolveddata.offset && currentoffset < data.loaderreference.datapointer)
						   fprintf(html, "STRINGTABLE\r\n");
					   else if (i == 2 && j == 0)
						   fprintf(html, "SYMBOL REFERENCES\r\n");
					   else if (i == 2 && j == 1)
						   fprintf(html, "LOADER REFERENCES\r\n");
					   else if (i == 2 && j == 2 && OVLD.hassymbolresolves)
						   fprintf(html, "SYMBOL RESOLVES\r\n");
					   fprintf(html, "~~~~~~~~~~~\r\n");

					   //----------------
					   //READ DATA
					   //---------------
					   //Stringtable
					   if (i == 0 && j == 0 && currentoffset < stringlessdata.datapointer && currentoffset < presolveddata.offset && currentoffset < data.loaderreference.datapointer)
					   {
						   do {
							   fgetpos(ovl, &position);
							   std::string str = GetStringA(ovl);
							   PostVar(html, position, currentoffset, "String", str, "", (currentoffset == reloc));
							   currentoffset += str.length() + 1;
						   } while (currentoffset < currentblocksize);
					   }
					   else if (i == 2 && j == 0)
						   //Symbol references
					   {
						   ulong count = OVLD.chunks[i].blocks[j].size / 12;
						   if (ovlversion == 4 || ovlversion == 5)
							   count = OVLD.chunks[i].blocks[j].size / 16;
						   for (k = 0; k < count; k++)
						   {
							   fread(&ultemp, 4, 1, ovl);
							   PostVar(html, position, currentoffset, "Stringpointer", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   if (currentoffset == reloc)
								   reloc = GetNearestRelocation(OVLD.relocations, reloc);
							   currentoffset += 4;
							   fgetpos(ovl, &position);

							   fread(&ultemp, 4, 1, ovl);
							   PostVar(html, position, currentoffset, "Datapointer", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   if (currentoffset == reloc)
								   reloc = GetNearestRelocation(OVLD.relocations, reloc);
							   currentoffset += 4;
							   fgetpos(ovl, &position);

							   if (ovlversion == 4 || ovlversion == 5)
							   {
								   fread(&ustemp, 2, 1, ovl);
								   PostVar(html, position, currentoffset, "Pointer flag", ustemp, "(If not a pointer, string is a variable)", (currentoffset == reloc));
								   if (currentoffset == reloc)
									   reloc = GetNearestRelocation(OVLD.relocations, reloc);
								   currentoffset += 2;
								   fgetpos(ovl, &position);

								   fread(&ustemp, 2, 1, ovl);
								   PostVar(html, position, currentoffset, "Loader pointer", ustemp, "", (currentoffset == reloc));
								   if (currentoffset == reloc)
									   reloc = GetNearestRelocation(OVLD.relocations, reloc);
								   currentoffset += 2;
								   fgetpos(ovl, &position);
							   }
							   else {
								   fread(&ultemp, 4, 1, ovl);
								   PostVar(html, position, currentoffset, "Pointer flag", ultemp, "(If not a pointer, string is a variable)", (currentoffset == reloc));
								   if (currentoffset == reloc)
									   reloc = GetNearestRelocation(OVLD.relocations, reloc);
								   currentoffset += 4;
								   fgetpos(ovl, &position);
							   }

							   if (ovlversion == 4 || ovlversion == 5)
							   {
								   fread(&ultemp, 4, 1, ovl);
								   PostVar(html, position, currentoffset, "String hash", ultemp, "", (currentoffset == reloc));
								   if (currentoffset == reloc)
									   reloc = GetNearestRelocation(OVLD.relocations, reloc);
								   currentoffset += 4;
								   fgetpos(ovl, &position);
							   }

						   }
					   }
					   else if (i == 2 && j == 1)
						   //Loader references
					   {
						   ulong count = OVLD.chunks[i].blocks[j].size / 20;
						   for (k = 0; k < count; k++)
						   {
							   fread(&ultemp, 4, 1, ovl);
							   Loader ldr = OVL.GetLoaderByID(ultemp, currentOVL);
							   std::string loader = "(loader "; loader.append(ldr.name); loader.append(")");
							   PostVar(html, position, currentoffset, "Loader number", ultemp, loader, (currentoffset == reloc));
							   if (currentoffset == reloc)
								   reloc = GetNearestRelocation(OVLD.relocations, reloc);
							   currentoffset += 4;
							   fgetpos(ovl, &position);

							   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Data pointer", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   if (currentoffset == reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

							   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Extra data flag", ultemp, "", (currentoffset == reloc));
							   if (currentoffset == reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

							   fread(&ultemp, 4, 1, ovl);
							   if (ultemp == 0)
								   PostVar(html, position, currentoffset, "Symbol struct pointer", ultemp, "(loader is extra/stringlessdata)", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   else
								   PostVar(html, position, currentoffset, "Symbol struct pointer", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   if (currentoffset == reloc)
								   reloc = GetNearestRelocation(OVLD.relocations, reloc);
							   currentoffset += 4;
							   fgetpos(ovl, &position);

							   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Number of symbol resolves", ultemp, "", (currentoffset == reloc));
							   if (currentoffset == reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
						   }
					   }
					   else if (i == 2 && j == 2 && OVLD.hassymbolresolves)
						   //Symbol Resolves
					   {
						   ulong count = OVLD.chunks[i].blocks[j].size / 12;
						   if (ovlversion == 4 || ovlversion == 5)
							   count = OVLD.chunks[i].blocks[j].size / 16;
						   for (k = 0; k < count; k++)
						   {
							   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Data pointer", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   if (currentoffset == reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

							   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "String pointer", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   if (currentoffset == reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

							   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Load pointer", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
							   if (currentoffset == reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
							   if (ovlversion == 4 || ovlversion == 5)
							   {
								   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "String hash", ultemp, "", (currentoffset == reloc), OVL.PointerdataAtOffset(ultemp));
								   if (currentoffset == reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
							   }
						   }
					   }
					   else {
						   do {
							   std::string type = "long";
							   //currentoffset
							   ulong firstrelockind = min(reloc, min(relocpointto, data.loaderreference.datapointer));
							   if (position + 4 > (OVLD.chunks[i].blocks[j].size + OVLD.chunks[i].blocks[j].position) /*|| currentoffset + 4 > firstrelockind*/)//UNCOMMENT ALS FOUT IS GEFIXT
							   {
								   if (currentoffset + 2 == firstrelockind)
								   {
									   ulong readsize = 2;
									   ulong times = 1;
									   type = "short";
								   }
								   else {
									   ulong readsize = 1;
									   ulong times = reloc - currentoffset;
									   type = "byte";
								   }
							   }
							   ulong readsize = 4;
							   ulong times = 1;
							   for (ulong z = 0; z < times; z++)
							   {
								   ultemp = 0;
								   if (currentoffset >= relocpointto)
								   {
									   if (do_showincomingrelocations->Checked)
										   fprintf(html, "-==  INCOMING RELOCATION TO OFFSET $%x(%d)  ==-\r\n", relocpointto, relocpointto);
									   relocpointto = GetNearestRelocationPointTo(OVLD.relocationspointingto, currentoffset/*relocpointto*/);//Relocto als paramater omdat je zo de volgende relocto krijgt										 
								   }
								   //ulong blockbytesize = (min(min(presolveddata.offset,reloc), data.loaderreference.datapointer)-currentoffset);
								   if (currentoffset >= data.loaderreference.datapointer)
								   {
									   Loader ldr = OVL.GetLoaderByID(data.loaderreference.loadernumber, currentOVL);
									   std::string symbol = OVL.GetStringFromOffset(data.symbolresolve.stringpointer);
									   fprintf(html, "DATA OF %s (type:%s)\r\n", symbol.c_str(), ldr.name.c_str());
									   fprintf(html, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
									   ulong lastoffset = currentoffset;
									   if (ldr.tag == "svd")
									   {
#pragma region SVD
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SIVFlags", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Sway", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Brightness", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Scale", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Lodcount", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Lodpointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Structure version", ultemp, "(If -1, than structure is Soaked or Wild)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "mdl")
									   {
#pragma region MDL
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1", ultemp, "(Flags? Always 65536/0x10000)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 2", ultemp, "(=Flags)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   PostMDLEnum(html, ultemp);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 3", ftemp, "(Higher values seen at adults)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 4", ftemp, "(Higher values seen at adults)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 5", ftemp, "(Higher values seen at adults)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 6", ftemp, "(Higher values seen at adults)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Position-X offset?", ftemp, "(Based this on the MorhpMesh structure, testing required. Also smaller values were seen at babies)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Position-Y offset?", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Position-Z offset?", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Scale-X?", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Scale-Y?", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Scale-Z?", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 13", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 14", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 15", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 16", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 17", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 18", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 19", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 20", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "was")
									   {
#pragma region WAS
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Internal name", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Speciesname (Male?)", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Speciesname again (Female?)", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Description", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Datafile-filename", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Adultdata male", ultemp, "(Data usually consists out of a :mdl resolve and some unknown part)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Adultdata female", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Babydata (Male?)", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Babydata (Female?)", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknowndata A", ultemp, "(Seems to always be 24 bytes)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknowndata B", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Required shelter", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Icon(:gsi) resolve", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ftemp, "(Seen 0.0, 1.0, 2.0 and 3.0)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "NameForAnimalNaming", ultemp, "(Category name for in the animal selection list?)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "ppg")
									   {
#pragma region PPG
										   unsigned long tmpv = ovlversion;
										   //ovlversion = 4;
										   if (ovlversion == 4 || ovlversion == 5)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1 (V4/5)", ultemp, "(Seen 1)", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 2 (V4/5)", ultemp, "(Seen 0)", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 3 (V4/5)", ultemp, "(Seen 0 and 1, adult/child?)", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
										   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Contains bitflag", ultemp, "(Seems like to identify which kind of parts the model contains?)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   DumpBitFlags(html, ultemp);
										   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Group bitflag", ultemp, "(0 = head, 1 = body, 2 = legs)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   DumpBitFlags(html, ultemp);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   if (ovlversion == 4 || ovlversion == 5)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "PPG data", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown float", ftemp, "(LOD distance?)", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
										   /*if(ovlversion == 4 || ovlversion == 5)
										   {
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Number of hi-lods", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Number of med-lods", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Number of low-lods", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);*/
										   //} else if(ovlversion == 1)
										   //{
										   ultemp = 0;
										   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of hi-lods", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
										   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of hi-lods items", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
										   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown short", ultemp, "short", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   ultemp = 0;
										   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of med-lods", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
										   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of med-lods items", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
										   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown short", ultemp, "short", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   ultemp = 0;
										   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of low-lods", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
										   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of low-lods items", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
										   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown short", ultemp, "short", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   if (ovlversion == 1) {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "PPG data", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "PHD-resolve", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown float", ftemp, "(LOD distance?)", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
										   if (ovlversion == 4 || ovlversion == 5)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   ultemp = 0;
											   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of hi-swimlods", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
											   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of hi-swimlods items", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
											   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown short", ultemp, "short", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

											   ultemp = 0;
											   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of med-swimlods", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
											   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of med-swimlods items", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
											   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown short", ultemp, "short", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

											   ultemp = 0;
											   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of low-swimlods", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
											   fread(&ultemp, 1, 1, ovl); PostVar(html, position, currentoffset, "Number of low-swimlods items", ultemp, "byte", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 1; fgetpos(ovl, &position);
											   fread(&ultemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown short", ultemp, "short", "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "LOD-pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "PHD-resolve", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

											   /*fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "V4/5 unknown", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
											   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);*/
										   }
										   ovlversion = tmpv;
#pragma endregion
									   }
									   else if (ldr.tag == "asd")
									   {
#pragma region ASD
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Shelter sid", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown flag", ultemp, "(Seen -1)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Wardensshed sid", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown float", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "(Seen 1, 2)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown float", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "(Seen 1, 4)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Number of eatsplines", ultemp, "(Always 2)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Eatspline list", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Number of drinksplines", ultemp, "(Always 1 or 2)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "drinksplines list", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Number of sleepsplines", ultemp, "(Always 5)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "sleepsplines list", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Transition splines", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "(Seen 0, 2)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Sizeindicator", ultemp, "(Seen 15, 20, 6, 13. Size of unknown block = indicator*2)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown data pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Malespline", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Femalespline", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "mms")
									   {
#pragma region MMS
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Vertexcount", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Indexcount", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Algorithm Unknown 1", ultemp, "(Seems to be very important in the algorithm of how vertices are readed. Lower values at models with less vertices/indices. Cannot be zero! Vertices seem to 'leak' if not set right)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Algorithm Unknown 2", ultemp, "(Usually = unknown 1. Seems to be very important in the algorithm of how vertices are readed. Lower values at models with less vertices/indices. Cannot be zero!)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Type unknown", ultemp, "(Usually 0, 1 at some attachments)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Morphcount", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VertexUV's", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Indices", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 2", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Morphs", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "vwg")
									   {
#pragma region VWG
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Pointer to real data", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "enc")
									   {
#pragma region ENC
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Internal name", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Ingame name", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Icon resolve", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID full", ultemp, "// String is the internal SID name", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID end", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID corner", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID edge", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID corner I", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID gate", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID viewleft", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID viewmid", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SID viewright", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Cost fencepart?", ultemp, "// Seen 500", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Refund fencepart?", ultemp, "// Seen -400", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Cost viewinggallery?", ultemp, "//Seen 4000", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Refund viewinggallery?", ultemp, "//Seen -3500", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Spline right-right top", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Spline ?", ultemp, "// Can be empty spline (:spl)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Spline left-left top", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Spline ?", ultemp, "// Can be empty spline", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown float", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown float", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "psi")
									   {
#pragma region PSI
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Spritecount", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SpriteUVList", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Texture", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Type", ultemp, "(0 = normal, 1 = special. Has effect on how a particle handles colour)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Mode", ultemp, "(0 = Animated, 1 = Static)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "AnimationSpeed", ftemp, "(Seen 1.0 & 2.0 & 0.2 & 0.15 & 0.75)", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "(Always 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (ldr.tag == "ent")
									   {
#pragma region ENT
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "UnknownA", ultemp, "(Always 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "FloatCount", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "FloatPointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "BodypartCount", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "BodypartPointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Entertainer name", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "UnknownB", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SubtypeTXT", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "CostumeGSI", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   for (ulong z = 0; z < 16; z++)
										   {
											   char* str = (char*)malloc((sizeof(int) * 8 + 1));
											   _itoa(z, str, 10);
											   std::string varname = "LowerUniformColour";
											   varname.append(str);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, varname, ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   delete[] str;
										   }
										   for (ulong z = 0; z < 16; z++)
										   {
											   char* str = (char*)malloc((sizeof(int) * 8 + 1));
											   _itoa(z, str, 10);
											   std::string varname = "UpperUniformColour";
											   varname.append(str);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, varname, ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   delete[] str;
										   }

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SettingCount", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "SettingPointer", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkC", ultemp, "(Seen 329)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkD", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkE", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkF", ultemp, "(Seen 18)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkG", ultemp, "(Seen 19)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkH", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkI", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkJ", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkK", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkL", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkM", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkN", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkO", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkP", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkQ", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkR", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkS", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkT", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkU", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkV", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "VIPUnkW", ultemp, "(Seen 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);


										   /*fread(&ftemp, 4, 1, ovl);PostVar(html, position, currentoffset, "AnimationSpeed", ftemp, "(Seen 1.0 & 2.0 & 0.2 & 0.15 & 0.75)", (currentoffset == reloc));
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Unknown", ultemp, "(Always 0)", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);*/
#pragma endregion
									   }
									   /*else if(ldr.tag == "sid")
									   {
									   #pragma region SID
									   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Unknown 1", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
									   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
									   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Flags", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
									   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
									   DumpBitFlags(html, ultemp);
									   #pragma endregion
									   }*/
									   data = GetNearestData(OVLD.linkedfiles, lastoffset);//Data als paramater omdat je zo de volgende data krijgt
								   }
								   else if (currentoffset >= stringlessdata.datapointer) {
									   Loader ldr = OVL.GetLoaderByID(stringlessdata.loadernumber, currentOVL);
									   fprintf(html, "STRINGLESSDATA STRUCTURE %s\r\n", ldr.name.c_str());
									   fprintf(html, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
									   stringlessdata = GetNearestStringlessData(OVLD.loaderreference, currentoffset);
									   if (ldr.tag == "btbl")
									   {
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Count", ultemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
									   }
								   }
								   else if (currentoffset >= presolveddata.offset) {
									   fprintf(html, "EXTENDED DATA OF %s\r\n", presolveddata.name.c_str());
									   fprintf(html, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
									   ulong lastoffset = currentoffset;
									   if (presolveddata.name.find("SVD LODPOINTER LIST OF") != std::string::npos)
									   {
#pragma region SVD LODPOINTER
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "LOD pointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("WAS UNKNOWN-A DATABLOCK") != std::string::npos)
									   {
#pragma region WAS DATABLOCK-A
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 2", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 3", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 4", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 5", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 6", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (presolveddata.name.find("WAS UNKNOWN-B DATABLOCK") != std::string::npos)
									   {
#pragma region WAS DATABLOCK-B
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Number of animals", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Animal list", ultemp, "(Seems to be a list of animals the current animal will be aggressive to and attack them)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1", ultemp, "(Mostly ~95)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 2", ultemp, "(Mostly ~90-120)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 3", ftemp, "", (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 4", ultemp, "(Mostly ~95)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 5", ultemp, "(Mostly ~1-4)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 6", ultemp, "(Mostly ~1-4)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 7", ultemp, "(Mostly ~1-4)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (presolveddata.name.find("WAS UNKNOWN-B ANIMALLIST") != std::string::npos)
									   {
#pragma region WAS UNKNOWN-B ANIMALLIST
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Animal name", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("ASD SPLINELIST") != std::string::npos)
									   {
#pragma region ASD SPLINELIST
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Splinepointer", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("ASD UNKNOWN DATABLOCK-A") != std::string::npos)
									   {
#pragma region ASD DATABLOCK-A
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Data-A", ultemp, "(Seen -1 to 3, at Elephant also to 4)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Data-B", ultemp, "(Seen -1 to 3, at Elephant also to 4)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("SID SOUNDSCRIPT LIST") != std::string::npos)
									   {
#pragma region SID SOUNDSCRIPT
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Script reference", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("SID SOUNDDATA") != std::string::npos)
									   {
#pragma region SID SOUNDDATA
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Sound count", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Soundreferences list", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Script count", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Scriptreferences list", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (presolveddata.name.find("SID SOUNDEVENT SCRIPT") != std::string::npos)
									   {
#pragma region SID SOUNDEVENT SCRIPT
										   float time;
										   ulong instruction;
										   bool stop = false;
										   do {
											   fread(&time, 4, 1, ovl); PostVar(html, position, currentoffset, "Time", time, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&instruction, 4, 1, ovl); PostVar(html, position, currentoffset, "Instruction", instruction, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   if (instruction >= 3) {
												   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Value", ftemp, "", (currentoffset == reloc));
												   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
												   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Endtime", ftemp, "", (currentoffset == reloc));
												   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   }
											   if (instruction == 0)
												   stop = true;
										   } while (!stop);
#pragma endregion
									   }
									   else if (presolveddata.name.find("MMS VERTEXUV'S") != std::string::npos)
									   {
#pragma region MMS VERTEXUVS
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   ushort ustemp;
											   fread(&ustemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1", ustemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
											   fread(&ustemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Unknown 2", ustemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "U", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "V", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("MMS INDICES") != std::string::npos)
									   {
#pragma region MMS INDICES
										   bool even = true;
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   ushort ustemp;
											   even = !even;
											   fread(&ustemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Indice", ustemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
										   }
										   if ((presolveddata.size * 2) % 4 != 0)
										   {
											   fread(&ustemp, 2, 1, ovl); PostVar(html, position, currentoffset, "Padding for long", ustemp, "(Only if (indicecount*2)%4 != 0)", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 2; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("MMS MORPH DATA") != std::string::npos)
									   {
#pragma region MMS MORPHDATA
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   if (z)
												   fprintf(html, "~~~~~~~~~~~\r\n");
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "X-translate", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Y-translate", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Z-translate", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 1", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "X-scale", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Y-scale", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Z-scale", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown'2", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Morphname", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Times-count", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Timeslist", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Vertexdata", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown attachment extra's", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 4", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 5", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown 6", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("MMS MORPH TIMES LIST ANIMATION") != std::string::npos)
									   {
#pragma region MMS MORHP TIMES LIST
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Time", ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("MMS MORPH VERTEXPOSITIONS ANIMATION") != std::string::npos)
									   {
#pragma region MMS MORPH VERTEXPOSITIONS
										   presolveddata = GetNearestFurtherData(OVLD.presolvedfurtherdata, lastoffset);
										   fprintf(html, "Animation info (for research): Vertices:%d Indices:%d Timescount:%d Expectedsize:%d\r\n", presolveddata.count1, presolveddata.count2, presolveddata.count3, (min(min(presolveddata.offset, reloc), data.loaderreference.datapointer) - currentoffset));
										   fprintf(html, "~~~~~~~~~~~\r\n");
										   /*for(ulong x = 0; x < presolveddata.count3; x++)
										   {
										   if(x)
										   fprintf(html, "~~~~~~~~~~~\r\n");
										   for(ulong z = 0; z < presolveddata.count1; z++)
										   {
										   MorhpMeshVertex mmv;
										   //DumpVertex(FILE *&dumpfile, ulong pos, ulong offset, std::string name, TConvert data, std::string type, std::string comment, bool isreloc)
										   fread(&mmv, sizeof(MorhpMeshVertex), 1, ovl);DumpVertex(html, position, currentoffset, "MorphmeshVertex", mmv, "vrtx", "", (currentoffset == reloc));
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += sizeof(MorhpMeshVertex);fgetpos(ovl, &position);
										   }
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   }*/
#pragma endregion
									   }
									   else if (presolveddata.name.find("MMS MORPH ATTACHMENT UNKNOWNS") != std::string::npos)
									   {
#pragma region MMS MORPH ATTACHMENT
										   presolveddata = GetNearestFurtherData(OVLD.presolvedfurtherdata, lastoffset);
										   fprintf(html, "Unknown info (for research): Vertices:%d Indices:%d Timescount:%d Expectedsize:%d\r\n", presolveddata.count1, presolveddata.count2, presolveddata.count3, (min(min(presolveddata.offset, reloc), data.loaderreference.datapointer) - currentoffset));
										   fprintf(html, "~~~~~~~~~~~\r\n");
										   /*for(ulong x = 0; x < presolveddata.count3; x++)
										   {
										   if(x)
										   fprintf(html, "~~~~~~~~~~~\r\n");
										   for(ulong z = 0; z < presolveddata.count1; z++)
										   {
										   MorhpMeshVertex mmv;
										   //DumpVertex(FILE *&dumpfile, ulong pos, ulong offset, std::string name, TConvert data, std::string type, std::string comment, bool isreloc)
										   fread(&mmv, sizeof(MorhpMeshVertex), 1, ovl);DumpVertex(html, position, currentoffset, "MorphmeshVertex", mmv, "vrtx", "", (currentoffset == reloc));
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += sizeof(MorhpMeshVertex);fgetpos(ovl, &position);
										   }
										   fread(&ultemp, 4, 1, ovl);PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc), (currentoffset == reloc)?OVL.PointerdataAtOffset(ultemp):"");
										   if(currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc);currentoffset += 4;fgetpos(ovl, &position);
										   }*/
#pragma endregion
									   }
									   else if (presolveddata.name.find("VIEWINGGALLERY DATA") != std::string::npos)
									   {
#pragma region VIEWINGGALLERY DATA
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "ViewingGallery name 1", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "ViewingGallery name 2", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "GSI resolve", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 4960", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Spline resolve", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen -1", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 2", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Unknown ", ultemp, "// Seen 0", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
#pragma endregion
									   }
									   else if (presolveddata.name.find("PSI UV-COORDLIST OF") != std::string::npos)
									   {
#pragma region PSI UV-COORDLIST
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "PSI UV-Coords", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("PSI UV-COORDINATE") != std::string::npos)
									   {
#pragma region PSI UV-COORDINATE
										   fread(&ftemp, 4, 1, ovl); std::string breuk = "("; int factor = ftemp / 0.125000; char* str = (char*)malloc((sizeof(int) * 8 + 1)); _itoa(factor, str, 10); breuk.append(str); breuk.append("/8)");
										   PostVar(html, position, currentoffset, "Left", ftemp, breuk, (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); breuk = "("; factor = ftemp / 0.125000; _itoa(factor, str, 10); breuk.append(str); breuk.append("/8)");
										   PostVar(html, position, currentoffset, "Top", ftemp, breuk, (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); breuk = "("; factor = ftemp / 0.125000; _itoa(factor, str, 10); breuk.append(str); breuk.append("/8)");
										   PostVar(html, position, currentoffset, "Right", ftemp, breuk, (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   fread(&ftemp, 4, 1, ovl); breuk = "("; factor = ftemp / 0.125000; _itoa(factor, str, 10); breuk.append(str); breuk.append("/8)");
										   PostVar(html, position, currentoffset, "Bottom", ftemp, breuk, (currentoffset == reloc));
										   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

										   delete[] str;
#pragma endregion
									   }
									   else if (presolveddata.name.find("ENT UNKNOWNFLOATLIST OF") != std::string::npos)
									   {
#pragma region ENT UNKNOWNFLOATLIST
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   char* str = (char*)malloc((sizeof(int) * 8 + 1));
											   _itoa(z, str, 10);
											   std::string varname = "UnknownFloat";
											   varname.append(str);
											   fread(&ftemp, 4, 1, ovl); PostVar(html, position, currentoffset, varname, ftemp, "", (currentoffset == reloc));
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);

											   delete[] str;
										   }
#pragma endregion
									   }
									   else if (presolveddata.name.find("ENT BODYPART PPG-LIST OF") != std::string::npos)
									   {
#pragma region ENT BODYPART
										   for (ulong z = 0; z < presolveddata.size; z++)
										   {
											   fread(&ultemp, 4, 1, ovl); PostVar(html, position, currentoffset, "Bodypart", ultemp, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(ultemp) : "");
											   if (currentoffset >= reloc)reloc = GetNearestRelocation(OVLD.relocations, reloc); currentoffset += 4; fgetpos(ovl, &position);
										   }
#pragma endregion
									   }

									   presolveddata = GetNearestFurtherData(OVLD.presolvedfurtherdata, lastoffset);
								   }
								   else if (OVLD.chunks[i].blocks[j].size > 0) {
									   TConvert tc = { 0 };
									   fgetpos(ovl, &position);
									   fread(&tc.ul, readsize, 1, ovl);
									   PostVarUnknown(html, position, currentoffset, "Unknown data", tc, type, "", (currentoffset == reloc), (currentoffset == reloc) ? OVL.PointerdataAtOffset(tc.ul) : "");

									   if (currentoffset >= reloc)
										   reloc = GetNearestRelocation(OVLD.relocations, reloc);//Reloc als paramater omdat je zo de volgende reloc krijgt									 

									   currentoffset += readsize;
								   }
							   }

							   //Systeem inbouwen om onbekende relocs te tonen (PreResolveOVL())
							   //currentoffset += 4;
							   fgetpos(ovl, &position);
						   } while (position < (OVLD.chunks[i].blocks[j].size + OVLD.chunks[i].blocks[j].position));
					   }
					   Debug::WriteLine("Blocksize left [" + (i + 1) + "/9][" + (j + 1) + "/" + OVLD.chunks[i].num_blocks + "] " + ((OVLD.chunks[i].blocks[j].size + OVLD.chunks[i].blocks[j].position) - position));
					   fflush(html);
				   }
			   }

			   /*fprintf(html, "%sRelocations%s", "\r\n======================================================\r\n", "\r\n======================================================\r\n");
			   ulong tmp;
			   fread(&tmp, 4, 1, ovl);
			   fprintf(html, "Relocation count %d\r\n", tmp);
			   for(ulong i = 0; i < tmp; i++)
			   {
			   ulong tmp2;
			   fread(&tmp2, 4, 1, ovl);
			   }

			   if(ovlversion == 4 || (ovlversion == 5 && OVLD.extendedheaders.ovlv5extra.subversion))
			   {
			   fread(&tmp, 4, 1, ovl);
			   if(tmp > 0)
			   {
			   MessageBox::Show("Warning: Post relocation unknown is larger than 0");
			   }
			   }*/

			   //fprintf(html, "</pre>");
			   fclose(html);
			   fclose(ovl);
			   Beep(500, 50);
		   }

		   ulong GetNearestRelocation(std::vector<ulong> relocations, ulong currentoffset)
		   {
			   ulong temp = 0xFEFFFFFF;
			   ulong i;
			   for (i = 0; i < relocations.size(); i++)
			   {
				   if (relocations[i] < temp && relocations[i] > currentoffset)
				   {
					   temp = relocations[i];
					   //Debug::WriteLine("New relocation: " + temp);
				   }
			   }
			   return temp;
		   }

		   ulong GetNearestRelocationPointTo(std::vector<ulong> relocationspointingto, ulong currentoffset)
		   {
			   ulong temp = 0xFEFFFFFF;
			   ulong i;
			   for (i = 0; i < relocationspointingto.size(); i++)
			   {
				   if (relocationspointingto[i] < temp && relocationspointingto[i] > currentoffset)
				   {
					   temp = relocationspointingto[i];
					   //Debug::WriteLine("New relocation: " + temp);
				   }
			   }
			   return temp;
		   }

		   LinkedFiles GetNearestData(std::vector<LinkedFiles> linkedfiles, ulong currentoffset)
		   {
			   LinkedFiles lf;
			   ulong temp = 0xFEFFFFFF;
			   ulong i;
			   for (i = 0; i < linkedfiles.size(); i++)
			   {
				   if (linkedfiles[i].loaderreference.datapointer != 0)
					   if (linkedfiles[i].loaderreference.datapointer < temp && linkedfiles[i].loaderreference.datapointer > currentoffset)
					   {
						   temp = linkedfiles[i].loaderreference.datapointer;
						   lf = linkedfiles[i];
					   }
			   }
			   return lf;
		   }

		   PreResolved GetNearestFurtherData(std::vector<PreResolved> presolved, ulong currentoffset)
		   {
			   PreResolved pr;
			   ulong temp = 0xFEFFFFFF;
			   pr.offset = temp;
			   pr.name = "NO DATA HERE";
			   ulong i;
			   for (i = 0; i < presolved.size(); i++)
			   {
				   if (presolved[i].offset < temp && presolved[i].offset > currentoffset)
				   {
					   temp = presolved[i].offset;
					   pr = presolved[i];
				   }
			   }
			   return pr;
		   }

		   PreResolved ResolveSymbolResolves(std::vector<SymbolResolve> symbolresolves, ulong currentoffset)
		   {
			   PreResolved pr;
			   ulong temp = 0xFEFFFFFF;
			   pr.offset = temp;
			   pr.name = "NO DATA HERE";
			   ulong i;
			   for (i = 0; i < symbolresolves.size(); i++)
			   {
				   if (symbolresolves[i].pointer < temp && symbolresolves[i].pointer > currentoffset)
				   {
					   temp = symbolresolves[i].pointer;
					   pr.offset = symbolresolves[i].pointer;
					   pr.name = OVL.GetStringFromOffset(symbolresolves[i].stringpointer);
				   }
			   }
			   return pr;
		   }

		   LoadReference GetNearestStringlessData(std::vector<LoadReference> loaderrefence, ulong currentoffset)
		   {
			   LoadReference lf;
			   ulong temp = 0xFEFFFFFF;
			   lf.datapointer = temp;
			   ulong i;
			   for (i = 0; i < loaderrefence.size(); i++)
			   {
				   if (loaderrefence[i].datapointer != 0 && loaderrefence[i].symbolstructpointer == 0)
					   if (loaderrefence[i].datapointer < temp && loaderrefence[i].datapointer > currentoffset)
					   {
						   temp = loaderrefence[i].datapointer;
						   lf = loaderrefence[i];
					   }
			   }
			   return lf;
		   }

	private: System::Void commonToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		DumpOVL(0);
		Beep(500, 50);
		Beep(500, 50);
		Beep(2000, 50);
	}
	private: System::Void uniqueToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		DumpOVL(1);
		Beep(500, 50);
		Beep(500, 50);
		Beep(2000, 50);
	}
	private: System::Void bothToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		DumpOVL(0);
		DumpOVL(1);
		Beep(500, 50);
		Beep(500, 50);
		Beep(2000, 50);
	}
	private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!OVL.IsValid())
		{
			std::string err = OVL.GetLastReadingError();
			MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
			return;
		}
		String^ strlist = "List of overlay strings:\r\n";
		strlist += "-- Common --\r\n";
		OVLData OVLD = OVL.GetOVLD(0);
		for (ulong i = 0; i < OVLD.symbolstring.size(); i++)
		{
			strlist += gcnew String(OVLD.symbolstring[i].data.c_str()) + "\r\n";
		}
		strlist += "-- Unique --\r\n";
		OVLD = OVL.GetOVLD(1);
		for (ulong i = 0; i < OVLD.symbolstring.size(); i++)
		{
			strlist += gcnew String(OVLD.symbolstring[i].data.c_str()) + "\r\n";
		}
		Clipboard::SetText(strlist);
	}
	private: System::Void button4_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!OVL.IsValid())
		{
			std::string err = OVL.GetLastReadingError();
			MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
			return;
		}
		String^ strlist = "List of overlay files:\r\n";
		for (ulong j = 0; j < 2; j++)
		{
			lf = OVL.GetRawLinkedFiles(j);
			for (ulong i = 0; i < lf.size(); i++)
			{
				String^ str;
				std::string symbol = OVL.GetStringFromOffset(lf[i].symbolresolve.stringpointer);
				Loader ldr = OVL.GetLoaderByID(lf[i].loaderreference.loadernumber, C_OVL);
				str = "Loader: '" + gcnew String(ldr.tag.c_str()) + "'  File: '" + gcnew String(symbol.c_str()) + "'\r\n";
				strlist += str;
			}
		}
		if (strlist != "List of overlay files:\r\n")
			Clipboard::SetText(strlist);
	}

		   void UpdateStringlist(int currentOVL)
		   {
			   if (!OVL.IsValid())
			   {
				   std::string err = OVL.GetLastReadingError();
				   MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
				   return;
			   }
			   stringlist->Items->Clear();
			   stringlist->BeginUpdate();
			   OVLData OVLD = OVL.GetOVLD(currentOVL);
			   for (ulong i = 0; i < OVLD.symbolstring.size(); i++)
			   {
				   stringlist->Items->Add(gcnew String(OVLD.symbolstring[i].data.c_str()));
			   }
			   stringlist->EndUpdate();
		   }
		   void DoAfterLoading()
		   {
			   if (!OVL.IsValid())
			   {
				   std::string err = OVL.GetLastReadingError();
				   MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
				   return;
			   }
			   UpdateStringlist(0);

			   loaderlist->Items->Clear();
			   loaderlist->BeginUpdate();
			   OVLData OVLD = OVL.GetOVLD(0);
			   for (ulong i = 0; i < OVLD.loaders.size(); i++)
			   {
				   loaderlist->Items->Add("Name: " + gcnew String(OVLD.loaders[i].name.c_str()) + "  Tag: " + gcnew String(OVLD.loaders[i].tag.c_str()) + " Loader: " + gcnew String(OVLD.loaders[i].loader.c_str()));
			   }
			   loaderlist->EndUpdate();

			   symbolresolvelist->Items->Clear();
			   symbolresolvelist->BeginUpdate();
			   for (ulong j = 0; j < 2; j++)
			   {
				   OVLData OVLD = OVL.GetOVLD(j);
				   for (ulong i = 0; i < OVLD.symbolresolves.size(); i++)
				   {
					   std::string strptr = OVL.GetStringFromOffset(OVLD.symbolresolves[i].stringpointer);
					   symbolresolvelist->Items->Add("Stringpointer: " + gcnew String(strptr.c_str()) + "  Pointing to: " + OVLD.symbolresolves[i].pointer + "\r\n");
				   }
			   }
			   symbolresolvelist->EndUpdate();

			   OVLD = OVL.GetOVLD(0);
			   ovlinformation->Text = "File version: " + OVLD.h1.version + "\r\n";
			   if (OVLD.h1.version == 5)
			   {
				   ovlinformation->Text += "File subversion (v5): " + OVLD.extendedheaders.ovlv5extra.subversion + "\r\n";
			   }
			   ovlinformation->Text += "Header version: " + OVLD.h1.headerversion + "\r\n";

			   referencelist->Items->Clear();
			   referencelist->BeginUpdate();
			   for (ulong j = 0; j < 2; j++)
			   {
				   OVLData OVLD = OVL.GetOVLD(j);
				   for (ulong i = 0; i < OVLD.references.size(); i++)
				   {
					   referencelist->Items->Add(gcnew String(OVLD.references[i].file.c_str()));
				   }
			   }
			   referencelist->EndUpdate();
		   }
	private: System::Void button5_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!OVL.IsValid())
		{
			std::string err = OVL.GetLastReadingError();
			MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
			return;
		}
		String^ strlist = "List of overlay loaders:\r\n";
		OVLData OVLD = OVL.GetOVLD(0);
		for (ulong i = 0; i < OVLD.loaders.size(); i++)
		{
			strlist += "Name: " + gcnew String(OVLD.loaders[i].name.c_str()) + "  Tag: " + gcnew String(OVLD.loaders[i].tag.c_str()) + " Loader: " + gcnew String(OVLD.loaders[i].loader.c_str()) + "\r\n";
		}
		Clipboard::SetText(strlist);
	}//symbolresolvelist
	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!OVL.IsValid())
		{
			std::string err = OVL.GetLastReadingError();
			MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
			return;
		}
		String^ strlist = "List of overlay symbolresolves:\r\n";
		for (ulong j = 0; j < 2; j++)
		{
			OVLData OVLD = OVL.GetOVLD(j);
			for (ulong i = 0; i < OVLD.symbolresolves.size(); i++)
			{
				std::string strptr = OVL.GetStringFromOffset(OVLD.symbolresolves[i].stringpointer);
				strlist += "Stringpointer: " + gcnew String(strptr.c_str()) + "  Pointing to: " + OVLD.symbolresolves[i].pointer + "\r\n";
			}
		}
		Clipboard::SetText(strlist);
	}

		   void MakeIndents(FILE*& XML, ulong indentlevel)
		   {
			   for (ulong i = 0; i < indentlevel; i++)
				   fprintf(XML, "	");
		   }
	private: System::Void exportOVLMakeXMLToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!OVL.IsValid())
		{
			std::string err = OVL.GetLastReadingError();
			MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
			return;
		}
		ulong indentlevel = 0;
		ulong errornum = 1;
		FILE* COVL;
		FILE* UOVL;
		FILE* XML;

		std::string filename = "OverlayXML_";
		std::string ovlname = OVL.safefilename;
		ovlname.erase(ovlname.end() - 11, ovlname.end());
		filename.append(ovlname);
		filename.append(".xml");

		OVLData OVLD = OVL.GetOVLD(0);
		if (fopen_s(&COVL, OVLD.ovlname.c_str(), "rb") == EINVAL || NULL)
		{
			MessageBox::Show("Overlay[Common] could not be opened");
			return;
		}
		OVLD = OVL.GetOVLD(1);
		if (fopen_s(&UOVL, OVLD.ovlname.c_str(), "rb") == EINVAL || NULL)
		{
			MessageBox::Show("Overlay[Unique] could not be opened");
			fclose(COVL);
			return;
		}
		if (fopen_s(&XML, filename.c_str(), "wb") == EINVAL || NULL)
		{
			MessageBox::Show("XML could not be opened");
			fclose(COVL);
			fclose(UOVL);
			return;
		}
		fprintf(XML, "<?xml version='1.0' encoding='UTF-8'?>\r\n");
		fprintf(XML, "<rawovl basedir='%s' installdir='' xmlns='http://rct3.sourceforge.net/rct3xml/raw'>\r\n", ovlname.c_str());
		indentlevel++;

		for (ulong i = 0; i < 2; i++)
		{
			lf = OVL.GetRawLinkedFiles(i);
			for (ulong j = 0; j < lf.size(); j++)
			{
				std::string symbol = OVL.GetStringFromOffset(lf[j].symbolresolve.stringpointer);
				Loader ldr = OVL.GetLoaderByID(lf[j].loaderreference.loadernumber, C_OVL);
				PositionReturn pr = OVL.OffsetToPosition(lf[j].loaderreference.datapointer);
				ulong startoffset = lf[j].loaderreference.datapointer;
				std::string symbolwol = symbol;
				symbolwol.erase((symbolwol.end() - ldr.tag.length() - 1), symbolwol.end());
				ValidateXMLString(symbolwol);

				FILE* ovl = (pr.currentOVL == 1) ? UOVL : COVL;
				fseek(ovl, pr.position, SEEK_SET);
				bool found = true;
				if (ldr.tag == "txt")
				{
					std::wstring tstr = GetStringW(ovl);
					MakeIndents(XML, indentlevel);
					fprintf(XML, "<txt name='%s' type='text'>", symbolwol.c_str());
					fwrite(tstr.c_str(), tstr.length() * 2, 1, XML);
					fprintf(XML, "</txt>\r\n");
				}
				else if (ldr.tag == "snd") {
					Sound snd;
					fread(&snd, sizeof(Sound), 1, ovl);
					MakeIndents(XML, indentlevel);
					fprintf(XML, "<snd name='%s' loop='%d'>\r\n", symbolwol.c_str(), snd.loop);
					MakeIndents(XML, indentlevel + 1);
					fprintf(XML, "<sound><data type='file'>%s.wav</data></sound>\r\n", symbolwol.c_str());
					MakeIndents(XML, indentlevel);
					fprintf(XML, "</snd>\r\n");
				}
				else if (ldr.tag == "gsi") {
					ulong tmp = 0;
					ulong pos[4];
					fseek(ovl, 8, SEEK_CUR);
					fread(&tmp, 4, 1, ovl);
					//Debug::WriteLine("TMP: "+tmp);
					pr = OVL.OffsetToPosition(tmp);
					FILE* ovl2 = (pr.currentOVL == 1) ? UOVL : COVL;
					fseek(ovl2, pr.position, SEEK_SET);
					fread(&pos, 4, 4, ovl2);

					std::string texref = OVL.ReturnDatablocknameFromOffset((startoffset + 4), true);
					texref.erase((texref.end() - 4), texref.end());

					MakeIndents(XML, indentlevel);
					fprintf(XML, "<gsi name='%s' texture='%s' left='%d' top='%d' right='%d' bottom='%d'>\r\n", symbolwol.c_str(), texref.c_str(), pos[0], pos[1], pos[2], pos[3]);
				}
				else if (ldr.tag == "tex") {
					Sound snd;
					fread(&snd, sizeof(Sound), 1, ovl);
					MakeIndents(XML, indentlevel);
					fprintf(XML, "<tex name='%s' format='%d'>\r\n", symbolwol.c_str(), 18);
					MakeIndents(XML, indentlevel + 1);
					fprintf(XML, "<texture><data type='file'>%s.png</data></texture>\r\n", symbolwol.c_str());
					MakeIndents(XML, indentlevel);
					fprintf(XML, "</tex>\r\n");
				}
				else if (ldr.tag == "tks") {
					TrackSection tks;
					fread(&tks, sizeof(TrackSection), 1, ovl);
					MakeIndents(XML, indentlevel);
					std::string internalname = OVL.GetStringFromOffset(tks.internalname);
					std::string sidreference = OVL.ReturnDatablocknameFromOffset((startoffset + 4), true);
					sidreference.erase((sidreference.end() - 4), sidreference.end());
					fprintf(XML, "<tks name='%s' internalname='%s' sid='%s'>\r\n", symbolwol.c_str(), internalname.c_str(), sidreference.c_str());
					MakeIndents(XML, indentlevel + 1);
					fprintf(XML, "<basic special='%d' ><entry curve='%d' flags='%d'/><exit curve='%d' flags='%d' /></basic>\r\n", tks.specialcurves, tks.entrycurve, tks.entryflags, tks.exitcurve, tks.exitflags);

					MakeIndents(XML, indentlevel + 1);
					fprintf(XML, "<splines>\r\n");
					//Splinecode
					MakeIndents(XML, indentlevel + 2);
					std::string leftspline = OVL.ReturnDatablocknameFromOffset((startoffset + (8 * 4)), true);
					leftspline.erase((leftspline.end() - 4), leftspline.end());
					std::string rightspline = OVL.ReturnDatablocknameFromOffset((startoffset + (9 * 4)), true);
					rightspline.erase((rightspline.end() - 4), rightspline.end());
					fprintf(XML, "<car left='%s' right='%s' />\r\n", leftspline.c_str(), rightspline.c_str());

					MakeIndents(XML, indentlevel + 2);
					leftspline = OVL.ReturnDatablocknameFromOffset((startoffset + (10 * 4)), true);
					leftspline.erase((leftspline.end() - 4), leftspline.end());
					rightspline = OVL.ReturnDatablocknameFromOffset((startoffset + (11 * 4)), true);
					rightspline.erase((rightspline.end() - 4), rightspline.end());
					fprintf(XML, "<join left='%s' right='%s' />\r\n", leftspline.c_str(), rightspline.c_str());

					MakeIndents(XML, indentlevel + 2);
					leftspline = OVL.ReturnDatablocknameFromOffset((startoffset + (12 * 4)), true);
					leftspline.erase((leftspline.end() - 4), leftspline.end());
					rightspline = OVL.ReturnDatablocknameFromOffset((startoffset + (13 * 4)), true);
					rightspline.erase((rightspline.end() - 4), rightspline.end());
					fprintf(XML, "<extra left='%s' right='%s' />\r\n", leftspline.c_str(), rightspline.c_str());

					MakeIndents(XML, indentlevel + 1);
					fprintf(XML, "</splines>\r\n");

					MakeIndents(XML, indentlevel);
					fprintf(XML, "</tks>\r\n");
				}
				else if (ldr.tag == "psi") {
					ParticleSkin psi;
					fread(&psi, sizeof(ParticleSkin), 1, ovl);
					MakeIndents(XML, indentlevel);
					std::string texreference = OVL.ReturnDatablocknameFromOffset((startoffset + 8), true);
					texreference.erase((texreference.end() - 4), texreference.end());

					fprintf(XML, "<psi name='%s' texture='%s' type='%d' animationspeed='%f' unk1='%d'>\r\n", symbolwol.c_str(), texreference.c_str(), psi.flaga, psi.unknownmodifier, psi.unk1);
					pr = OVL.OffsetToPosition(psi.pos);
					FILE* ovl2 = (pr.currentOVL == 1) ? UOVL : COVL;
					fseek(ovl2, pr.position, SEEK_SET);

					for (unsigned long k = 0; k < psi.spritecount; k++)
					{
						ulong tmp = 0;
						pr = OVL.OffsetToPosition(psi.pos);
						fseek(ovl2, pr.position + (k * 4), SEEK_SET);
						fread(&tmp, 4, 1, ovl2);

						pr = OVL.OffsetToPosition(tmp);
						FILE* ovl3 = (pr.currentOVL == 1) ? UOVL : COVL;
						fseek(ovl3, pr.position, SEEK_SET);

						SpriteCoords sp;
						fread(&sp, sizeof(SpriteCoords), 1, ovl3);
						MakeIndents(XML, indentlevel + 1);
						fprintf(XML, "<spritecoords left='%f' top='%f' right='%f' bottom='%f' />\r\n", sp.left, sp.top, sp.right, sp.bottom);
					}
					MakeIndents(XML, indentlevel);
					fprintf(XML, "</psi>\r\n");
				}
				else if (ldr.tag == "sid") {
					SceneryItem sid;
					ulong tmp;
					std::string supports;
					std::string objecticon = OVL.ReturnDatablocknameFromOffset((startoffset + (22 * 4)), true);
					objecticon.erase((objecticon.end() - 4), objecticon.end());
					std::string groupicon = OVL.ReturnDatablocknameFromOffset((startoffset + (23 * 4)), true);
					groupicon.erase((groupicon.end() - 4), groupicon.end());
					std::string groupnametxt = OVL.ReturnDatablocknameFromOffset((startoffset + (24 * 4)), true);
					groupnametxt.erase((groupnametxt.end() - 4), groupnametxt.end());
					std::string nametxt = OVL.ReturnDatablocknameFromOffset((startoffset + (30 * 4)), true);
					nametxt.erase((nametxt.end() - 4), nametxt.end());
					fread(&sid, sizeof(SceneryItem), 1, ovl);

					pr = OVL.OffsetToPosition(sid.svdref);
					//Debug::WriteLine("SVDRef: "+ sid.svdref);
					FILE* ovl2 = (pr.currentOVL == 1) ? UOVL : COVL;
					fseek(ovl2, pr.position, SEEK_SET);
					fread(&tmp, 4, 1, ovl2);
					std::string svdname = OVL.ReturnDatablocknameFromOffset(tmp, true);


					std::string ovlpath = OVL.GetStringFromOffset(sid.ovlpath);

					ValidateXMLString(nametxt);
					ValidateXMLString(ovlpath);


					MakeIndents(XML, indentlevel);
					fprintf(XML, "<sid name='%s' nametxt='%s' icon='%s' ovlpath='%s' svd='%s'>\r\n", symbolwol.c_str(), nametxt.c_str(), objecticon.c_str(), ovlpath.c_str(), svdname.c_str());

					MakeIndents(XML, indentlevel + 1);
					fprintf(XML, "<type scenerytype='%d'/><!-- Type: %s -->\r\n", sid.type, SIDTypes[sid.type]);
					MakeIndents(XML, indentlevel + 1);
					fprintf(XML, "<position type='%d' xsize='%f' ysize='%f' zsize='%f' xsquares='%d' zsquares='%d'/><!-- Type: %s -->\r\n", sid.position, sid.sizex, sid.sizey, sid.sizez, sid.squares_x, sid.squares_z, SIDSizeTypes[sid.position]);

					MakeIndents(XML, indentlevel);
					fprintf(XML, "</sid>\r\n");
					//<type scenerytype='%d'/>
					//<position type='%d' xsize='%d' ysize='%d' zsize='%d' xsquares='%d' zsquares='%d'/>
				}
				else {
					found = false;
					MakeIndents(XML, indentlevel);
					fprintf(XML, "<!-- #%d::Error 1: Could not convert file %s, no converter for type '%s' build-in -->\r\n", errornum, symbol.c_str(), ldr.tag.c_str());
					errornum++;
				}
				if (found)
					fprintf(XML, "\r\n");
			}
		}
		fprintf(XML, "</rawovl>\r\n");
		indentlevel--;

		fclose(COVL);
		fclose(UOVL);
		fclose(XML);
		return;
	}
	private: System::Void itemlist_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void referencelist_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		ulong offsetlimit = 0x76F5;//30453;
		ulong offsetaddition = 24;
		if (!OVL.IsValid())
		{
			std::string err = OVL.GetLastReadingError();
			MessageBox::Show("Error: Overlay is not valid, reason: " + gcnew String(err.c_str()));
			return;
		}
		FILE* COVL;
		FILE* UOVL;

		OVLData OVLD = OVL.GetOVLD(0);
		if (fopen_s(&COVL, OVLD.ovlname.c_str(), "rb+") == EINVAL || NULL)
		{
			MessageBox::Show("Overlay[Common] could not be opened");
			return;
		}
		OVLD = OVL.GetOVLD(1);
		if (fopen_s(&UOVL, OVLD.ovlname.c_str(), "rb+") == EINVAL || NULL)
		{
			MessageBox::Show("Overlay[Unique] could not be opened");
			fclose(COVL);
			return;
		}

		for (ulong i = 0; i < 2; i++)
		{
			OVLD = OVL.GetOVLD(i);
			for (ulong j = 0; j < OVLD.relocations.size(); j++)
			{
				PositionReturn pr = OVL.OffsetToPosition(OVLD.relocations[j]);
				FILE* ovlf = (pr.currentOVL == 1) ? UOVL : COVL;
				fseek(ovlf, pr.position, SEEK_SET);
				ulong temp;
				fread(&temp, 4, 1, ovlf);
				if (pr.position != -1)
				{
					if (temp >= offsetlimit)
					{
						Debug::WriteLine("Changed: " + temp + "->" + (temp + offsetaddition) + " (From relocation " + OVLD.relocations[j] + ")");
						temp += offsetaddition;
						fseek(ovlf, pr.position, SEEK_SET);
						fwrite(&temp, 4, 1, ovlf);
					}
					else {
						Debug::WriteLine("Unchanged: " + temp);
					}
				}
				else
					MessageBox::Show("Error: Relocation is mapping out of bounds!");

			}
			Debug::WriteLine("- - - - - - - - - - - - -");
		}

		fclose(COVL);
		fclose(UOVL);
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void decompileAllItemsToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) {
		for (int i = 0; i < itemlist->Items->Count; i++)
		{
			DecompileItem(i);
		}
	}
	};
}

