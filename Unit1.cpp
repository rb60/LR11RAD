//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
bool pen = false;
TStringList* imageLog;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	imageLog = new TStringList();
}
//---------------------------------------------------------------------------

void readLog(TStringList* imglog, TImage* img)
{
	for (int i = 0; i < imglog->Count; i++)
	{
		if (imglog->Strings[i] == "#Move")
		{
			i++;
			int x = StrToInt(imglog->Strings[i]);
			i++;
			int y = StrToInt(imglog->Strings[i]);
			img->Canvas->MoveTo(x,y);
            continue;
		}
		else if (imglog->Strings[i] == "#Line")
		{
            i++;
			int x = StrToInt(imglog->Strings[i]);
			i++;
			int y = StrToInt(imglog->Strings[i]);
			img->Canvas->LineTo(x,y);
			continue;
		}
	}
}

void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	if (sbPencil->Down)
	{
		pen = true;
		((TImage*)Sender)->Canvas->MoveTo(X,Y);
		imageLog->Add("#Move");
		imageLog->Add(IntToStr(X));
		imageLog->Add(IntToStr(Y));
	}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y)
{
	if (pen)
	{
		((TImage*)Sender)->Canvas->LineTo(X,Y);
        imageLog->Add("#Line");
		imageLog->Add(IntToStr(X));
		imageLog->Add(IntToStr(Y));
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
    pen = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Save1Click(TObject *Sender)
{
	if (SaveDialog1->Execute() == mrOk)
	{
		imageLog->SaveToFile(SaveDialog1->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Save2Click(TObject *Sender)
{
	if (OpenDialog1->Execute() == mrOk)
	{
		imageLog->LoadFromFile(OpenDialog1->FileName);
        readLog(imageLog, Image1);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::View1Click(TObject *Sender)
{
	Form2->Memo1->Lines = imageLog;
	Form2->Show();
}
//---------------------------------------------------------------------------

