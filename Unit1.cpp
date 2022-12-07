//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
bool programingChange = false;
bool pen = false;
TStringList* imageLog;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	imageLog = new TStringList();
}
//---------------------------------------------------------------------------

void TForm1::readLog(TStringList* imglog)
{
	for (int i = 0; i < imglog->Count; i++)
	{
		if (imglog->Strings[i] == "#Move")
		{
			i++;
			int x = StrToInt(imglog->Strings[i]);
			i++;
			int y = StrToInt(imglog->Strings[i]);
			Image1->Canvas->MoveTo(x,y);
            continue;
		}
		else if (imglog->Strings[i] == "#Line")
		{
            i++;
			int x = StrToInt(imglog->Strings[i]);
			i++;
			int y = StrToInt(imglog->Strings[i]);
			Image1->Canvas->LineTo(x,y);
			continue;
		}
        else if (imglog->Strings[i] == "#PenColor")
		{
			i++;
			TColor c = (TColor)StrToInt(imglog->Strings[i]);
			PenColor = c;
			continue;
		}
        else if (imglog->Strings[i] == "#BrushColor")
		{
			i++;
			TColor c = (TColor)StrToInt(imglog->Strings[i]);
            BrushColor = c;
			continue;
		}
	}
}

TColor TForm1::getPenColor()
{
    return Shape1->Brush->Color;
}

TColor TForm1::getBrushColor()
{
    return Shape2->Brush->Color;
}

void TForm1::setPenColor(TColor value)
{
	Image1->Canvas->Pen->Color = value;
	Shape1->Brush->Color = value;
	programingChange = true;
	ColorBox1->Selected = value;
    programingChange = false;
}

void TForm1::setBrushColor(TColor value)
{
	Image1->Canvas->Brush->Color = value;
	Shape2->Brush->Color = value;
    programingChange = true;
	ColorBox2->Selected = value;
	programingChange = false;
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
        readLog(imageLog);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::View1Click(TObject *Sender)
{
	Form2->Memo1->Lines = imageLog;
	Form2->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorBox1Change(TObject *Sender)
{
	if (!programingChange)
	{
        PenColor = ((TColorBox*)Sender)->Selected;
		imageLog->Add("#PenColor");
		imageLog->Add(IntToStr(((TColorBox*)Sender)->Selected));
	}


}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorBox2Change(TObject *Sender)
{
	if (!programingChange)
	{
    	BrushColor = ((TColorBox*)Sender)->Selected;
		imageLog->Add("#BrushColor");
		imageLog->Add(IntToStr(((TColorBox*)Sender)->Selected));
	}

}
//---------------------------------------------------------------------------

