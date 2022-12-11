//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
TForm3 *Form3;
TForm2 *Tree;
ImgGraph *history;
ImgNode* curNode;
bool programingChange = false;
bool pen = false;
bool line = false;
bool rect = false;
bool text = false;
bool elipse = false;
bool erase = false;
bool penColorSelected = true;
int imgx0, imgy0;
TStringList* imageLog;
TStringList* initialLog;
TBitmap* buffer;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	imageLog = new TStringList();
	buffer = new TBitmap();

	history = new Graph<TStringList*, NodeData>();
	ImgNode* start = new ImgNode({0, nullptr, nullptr});
	curNode = new ImgNode({1, nullptr, nullptr});
	initialLog = new TStringList();
	initialLog->Add("#PenColor");
	initialLog->Add("0x00000000");
	initialLog->Add("#BrushColor");
	initialLog->Add("0x00FFFFFF");
	initialLog->Add("#PenWidth");
	initialLog->Add("1");
	initialLog->Add("#PenStyle");
	initialLog->Add("0");
	initialLog->Add("#BrushStyle");
	initialLog->Add("0");
	initialLog->Add("#Font");
	initialLog->Add("Times New Roman");
	initialLog->Add("12");
	initialLog->Add("0x00000000");
	initialLog->Add("0");
	initialLog->Add("0");
	initialLog->Add("0");
	initialLog->Add("0");
    readLog(initialLog);

	history->CreateGraph(start, curNode, new TStringList());
	start->data.next = start->out.back();
    curNode->data.prev = curNode->in.back();

    Form3 = new TForm3(this);
	Form3->g = history;
	Form3->cur = &curNode;
}
//---------------------------------------------------------------------------

void TForm1::update()
{
	clearImg();
	imageLog->SetStrings(initialLog);
	imageLog->AddStrings(getCurLog(history,curNode));
	readLog(imageLog);
	Form3->update();
}

void TForm1::clearImg()
{
	Image1->Canvas->Brush->Color	= clWhite;
    Image1->Canvas->Brush->Style	= bsSolid;
	Image1->Canvas->Pen->Color	= clWhite;
	Image1->Canvas->Rectangle(0,0, Image1->Width ,Image1->Height);
    Image1->Canvas->Pen->Color = Shape1->Brush->Color;
	Image1->Canvas->Brush->Color = Shape2->Brush->Color;
	Image1->Canvas->Brush->Style = (TBrushStyle)ComboBox2->ItemIndex;

}

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
		}
		else if (imglog->Strings[i] == "#Line")
		{
			i++;
			int x = StrToInt(imglog->Strings[i]);
			i++;
			int y = StrToInt(imglog->Strings[i]);
			Image1->Canvas->LineTo(x,y);
		}
		else if (imglog->Strings[i] == "#PenColor")
		{
			i++;
			TColor c = (TColor)StrToInt(imglog->Strings[i]);
			PenColor = c;
		}
		else if (imglog->Strings[i] == "#BrushColor")
		{
			i++;
			TColor c = (TColor)StrToInt(imglog->Strings[i]);
			BrushColor = c;
		}
		else if (imglog->Strings[i] == "#PenWidth")
		{
			i++;
			int w = StrToInt(imglog->Strings[i]);
			PenWidth = w;
		}
		else if(imglog->Strings[i] == "#Rect")
		{
			int x1,y1,x2,y2;
			i++;
			x1 = StrToInt(imglog->Strings[i++]);
			y1 = StrToInt(imglog->Strings[i++]);
			x2 = StrToInt(imglog->Strings[i++]);
			y2 = StrToInt(imglog->Strings[i]);
			Image1->Canvas->Rectangle(x1,y1,x2,y2);
		}
		else if(imglog->Strings[i] == "#PenStyle")
		{
			i++;
			TPenStyle s = (TPenStyle)StrToInt(imglog->Strings[i]);
			PenStyle = s;
		}
        else if(imglog->Strings[i] == "#BrushStyle")
		{
			i++;
			TBrushStyle s = (TBrushStyle)StrToInt(imglog->Strings[i]);
			BrushStyle = s;
		}
        else if(imglog->Strings[i] == "#Text")
		{
            int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i++]);
			Image1->Canvas->TextOut(x,y,imglog->Strings[i]);
		}
        else if(imglog->Strings[i] == "#Font")
		{
			i++;
            TFont* newFont = new TFont();
			newFont->Name = imglog->Strings[i++];
			newFont->Size = StrToInt(imglog->Strings[i++]);
			newFont->Color = (TColor)StrToInt(imglog->Strings[i++]);
			newFont->Style = TFontStyles();
			if (imglog->Strings[i++] == "-1") newFont->Style = Font->Style << fsBold;
			if (imglog->Strings[i++] == "-1") newFont->Style = Font->Style << fsItalic;
			if (imglog->Strings[i++] == "-1") newFont->Style = Font->Style << fsUnderline;
			if (imglog->Strings[i]   == "-1") newFont->Style = Font->Style << fsStrikeOut;
            Font = newFont;
		}
        else if(imglog->Strings[i] == "#Ellipse")
		{
			int x1,y1,x2,y2;
			i++;
			x1 = StrToInt(imglog->Strings[i++]);
			y1 = StrToInt(imglog->Strings[i++]);
			x2 = StrToInt(imglog->Strings[i++]);
			y2 = StrToInt(imglog->Strings[i]);
			Image1->Canvas->Ellipse(x1,y1,x2,y2);
		}
        else if(imglog->Strings[i] == "#Pipette")
		{
			int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i++]);
			TColor c = Image1->Canvas->Pixels[x][y];
			if (imglog->Strings[i] == "Pen")
			{
                PenColor = c;
			}
			else
			{
				BrushColor = c;
            }
		}
		else if(imglog->Strings[i] == "#EraseStart" || imglog->Strings[i] == "#Erase")
		{
            if(imglog->Strings[i] == "#EraseStart")
			{
				Image1->Canvas->Pen->Color = clWhite;
				Image1->Canvas->Brush->Color = clWhite;
				Image1->Canvas->Brush->Style = bsSolid;
			}
			int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i++]);
			Image1->Canvas->Rectangle(x - PenWidth*5, y - PenWidth*5, x + PenWidth*5, y + PenWidth*5);

		}
		else if(imglog->Strings[i] == "#EraseEnd")
		{
			Image1->Canvas->Pen->Color = Shape1->Brush->Color;
			Image1->Canvas->Brush->Color = Shape2->Brush->Color;
			Image1->Canvas->Brush->Style = (TBrushStyle)ComboBox2->ItemIndex;
		}
		else if (imglog->Strings[i] == "#Clear")
		{
            clearImg();
		}
		else if (imglog->Strings[i] == "#Rotate")
		{
			int angle = StrToInt(imglog->Strings[++i]);
            rotate(angle);
		}
	}
}



void TForm1::rotate(int angle)
{
	TBitmap *newBitMap = new TBitmap();

	float radians=(2*3.1416*angle)/360;

	float cosine=(float)cos(radians);
	float sine=(float)sin(radians);

	float Point1x=(-Image1->Picture->Bitmap->Height*sine);
	float Point1y=(Image1->Picture->Bitmap->Height*cosine);
	float Point2x=(Image1->Picture->Bitmap->Width*cosine-Image1->Picture->Bitmap->Height*sine);
	float Point2y=(Image1->Picture->Bitmap->Height*cosine+Image1->Picture->Bitmap->Width*sine);
	float Point3x=(Image1->Picture->Bitmap->Width*cosine);
	float Point3y=(Image1->Picture->Bitmap->Width*sine);

	float minx=min(0,min(Point1x,min(Point2x,Point3x)));
	float miny=min(0,min(Point1y,min(Point2y,Point3y)));
	float maxx=max(Point1x,max(Point2x,Point3x));
	float maxy=max(Point1y,max(Point2y,Point3y));

	int newWidth=(int)ceil(fabs(maxx)-minx);
	int newHeight=(int)ceil(fabs(maxy)-miny);

	newBitMap->Height = newHeight;
    newBitMap->Width = newWidth;

	for(int x=0;x < newWidth;x++)
	{
		for(int y=0;y < newHeight;y++)
		{
			int SrcBitmapx=(int)((x+minx)*cosine+(y+miny)*sine);
			int SrcBitmapy=(int)((y+miny)*cosine-(x+minx)*sine);

			if(	SrcBitmapx >= 0 &&
				SrcBitmapx < Image1->Picture->Bitmap->Width &&
				SrcBitmapy >= 0 &&
				SrcBitmapy < Image1->Picture->Bitmap->Height)
			{
				newBitMap->Canvas->Pixels[x][y] = Image1->Picture->Bitmap->Canvas->Pixels[SrcBitmapx][SrcBitmapy];
			}
		}
	}
    Image1->Picture->Bitmap = newBitMap;
}


TFont* TForm1::getFont()
{
    return Image1->Canvas->Font;
}

TBrushStyle TForm1::getBrushStyle()
{
	return Image1->Canvas->Brush->Style;
}

TPenStyle TForm1::getPenStyle()
{
    return Image1->Canvas->Pen->Style;
}

int TForm1::getPenWidth()
{
	return Image1->Canvas->Pen->Width;
}

TColor TForm1::getPenColor()
{
	return Image1->Canvas->Pen->Color;
}

TColor TForm1::getBrushColor()
{
	return Image1->Canvas->Brush->Color;
}

void TForm1::setFont(TFont* value)
{
	Image1->Canvas->Font = value;
    Edit1->Font = value;
}

void TForm1::setBrushStyle(TBrushStyle value)
{
	Image1->Canvas->Brush->Style = value;
	programingChange = true;
	ComboBox2->ItemIndex = (int)value;
	programingChange = false;
}

void TForm1::setPenStyle(TPenStyle value)
{
	Image1->Canvas->Pen->Style = value;
	programingChange = true;
	ComboBox1->ItemIndex = (int)value;
    programingChange = false;
}

void TForm1::setPenWidth(int value)
{
	Image1->Canvas->Pen->Width = value;
	programingChange = true;
	NumberBox1->Value = value;
	programingChange = false;
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
	imgx0 = X;
	imgy0 = Y;
	if (sbPencil->Down)
	{
		pen = true;
		((TImage*)Sender)->Canvas->MoveTo(X,Y);
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Move");
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));
        Form3->update();
	}
	if (sbLine->Down)
	{
		line = true;
		((TImage*)Sender)->Canvas->MoveTo(X,Y);
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Move");
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));
		Form3->update();
		buffer->Assign(Image1->Picture->Bitmap);
	}
	if (sbRect->Down)
	{
		rect = true;
        buffer->Assign(Image1->Picture->Bitmap);
	}
	if (sbText->Down)
	{
        Edit1->Text = "";
		text = true;
		Edit1->Top = Y + Panel1->Height;
		Edit1->Left = X;
		Edit1->Enabled = true;
		Edit1->Visible = true;
        Edit1->SetFocus();

	}
	if (sbElipse->Down)
	{
		elipse = true;
        buffer->Assign(Image1->Picture->Bitmap);
	}
	if (sbPip->Down)
	{
		if (penColorSelected)
		{
			PenColor = Image1->Canvas->Pixels[X][Y];
		}
		else
		{
			BrushColor = Image1->Canvas->Pixels[X][Y];
		}
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Pipette");
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));
		curNode->data.prev->data->Add(penColorSelected ? "Pen" : "Brush");
	}
	if (sbErase->Down)
	{
		erase = true;
		if (curNode->out.size() != 0)
            createBranch(history,curNode);
		curNode->data.prev->data->Add("#EraseStart");
		curNode->data.prev->data->Add(IntToStr(X));
        curNode->data.prev->data->Add(IntToStr(Y));
		Image1->Canvas->Pen->Color = clWhite;
		Image1->Canvas->Brush->Color = clWhite;
		Image1->Canvas->Brush->Style = bsSolid;
		Image1->Canvas->Rectangle(X - PenWidth*5, Y - PenWidth*5, X + PenWidth*5, Y + PenWidth*5);
	}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	if (pen)
	{
		((TImage*)Sender)->Canvas->LineTo(X,Y);
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Line");
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));
	}
	if (line)
	{
		((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		((TImage*)Sender)->Canvas->MoveTo(imgx0,imgy0);
		if (Shift.Contains(ssShift))
		{
			((TImage*)Sender)->Canvas->LineTo(	abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0,
												abs(X - imgx0) > abs(Y - imgy0) ? imgy0 : Y);
		}
		else
		{
			((TImage*)Sender)->Canvas->LineTo(X,Y);
		}

	}
	if (rect)
	{
		((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		if (Shift.Contains(ssShift))
		{
			((TImage*)Sender)->Canvas->Rectangle(imgx0,imgy0,
												 abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0 + (Y - imgy0),
												 abs(X - imgx0) > abs(Y - imgy0) ? imgy0 + (X - imgx0) : Y);
		}
		else
		{
			((TImage*)Sender)->Canvas->Rectangle(imgx0,imgy0,X,Y);
		}

	}
	if (elipse)
	{
		((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		if (Shift.Contains(ssShift))
		{
			((TImage*)Sender)->Canvas->Ellipse(imgx0,imgy0,
												 abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0 + (Y - imgy0),
												 abs(X - imgx0) > abs(Y - imgy0) ? imgy0 + (X - imgx0) : Y);
		}
		else
		{
			((TImage*)Sender)->Canvas->Ellipse(imgx0,imgy0,X,Y);
		}
	}
	if (erase)
	{
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Erase");
		curNode->data.prev->data->Add(IntToStr(X));
        curNode->data.prev->data->Add(IntToStr(Y));
		Image1->Canvas->Rectangle(X - PenWidth*5, Y - PenWidth*5, X + PenWidth*5, Y + PenWidth*5);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	if (line)
	{
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Line");
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));
		Form3->update();
	}
	if (rect)
	{
        if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Rect");
		curNode->data.prev->data->Add(IntToStr(imgx0));
		curNode->data.prev->data->Add(IntToStr(imgy0));
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));

	}
	if (elipse)
	{
        if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Ellipse");
		curNode->data.prev->data->Add(IntToStr(imgx0));
		curNode->data.prev->data->Add(IntToStr(imgy0));
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));
	}
	if (erase)
	{
        if (curNode->out.size() != 0)
            createBranch(history,curNode);
		curNode->data.prev->data->Add("#EraseEnd");
		Image1->Canvas->Pen->Color = Shape1->Brush->Color;
		Image1->Canvas->Brush->Color = Shape2->Brush->Color;
		Image1->Canvas->Brush->Style = (TBrushStyle)ComboBox2->ItemIndex;
	}
    Form3->update();
    rect = false;
	line = false;
	pen = false;
	elipse = false;
    erase = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Save1Click(TObject *Sender)
{
	if (SaveDialog1->Execute() == mrOk)
	{
		GraphToStr(history,curNode,true)->SaveToFile(SaveDialog1->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Save2Click(TObject *Sender)
{
	if (OpenDialog1->Execute() == mrOk)
	{
		imageLog->LoadFromFile(OpenDialog1->FileName);
		imgGraphClear(history);
		GraphFromStr(history, curNode, imageLog);
		delete imageLog;
		imageLog = getCurLog(history, curNode);
		readLog(imageLog);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::View1Click(TObject *Sender)
{
	Form3->update();
	Form3->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Update1Click(TObject *Sender)
{
	update();
}

void __fastcall TForm1::ColorBox1Change(TObject *Sender)
{
	if (!programingChange)
	{
		PenColor = ((TColorBox*)Sender)->Selected;
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#PenColor");
		curNode->data.prev->data->Add("0x" + IntToHex(((TColorBox*)Sender)->Selected));
		Form3->update();
		penColorSelected = true;
	}


}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorBox2Change(TObject *Sender)
{
	if (!programingChange)
	{
		BrushColor = ((TColorBox*)Sender)->Selected;
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#BrushColor");
		curNode->data.prev->data->Add("0x" + IntToHex(((TColorBox*)Sender)->Selected));
		Form3->update();
        penColorSelected = false;
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::NumberBox1ChangeValue(TObject *Sender)
{
	if (!programingChange)
	{
		Image1->Canvas->Pen->Width = ((TNumberBox*)Sender)->Value;
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#PenWidth");
		curNode->data.prev->data->Add(IntToStr((int)((TNumberBox*)Sender)->Value));
		Form3->update();
	}

}

//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
	if (!programingChange)
	{
		PenStyle = (TPenStyle)((TComboBox*)Sender)->ItemIndex;
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#PenStyle");
		curNode->data.prev->data->Add(IntToStr(((TComboBox*)Sender)->ItemIndex));
		Form3->update();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComboBox2Change(TObject *Sender)
{
    if (!programingChange)
	{
		BrushStyle = (TBrushStyle)((TComboBox*)Sender)->ItemIndex;
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#BrushStyle");
		curNode->data.prev->data->Add(IntToStr(((TComboBox*)Sender)->ItemIndex));
		Form3->update();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == vkReturn)
	{
		text = false;
		Form1->SetFocus();
		((TEdit*)Sender)->Visible = false;
		((TEdit*)Sender)->Enabled = false;
		Image1->Canvas->TextOut(imgx0,imgy0,((TEdit*)Sender)->Text);
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Text");
		curNode->data.prev->data->Add(IntToStr(imgx0));
		curNode->data.prev->data->Add(IntToStr(imgy0));
		curNode->data.prev->data->Add(((TEdit*)Sender)->Text);
	}
	((TEdit*)Sender)->Width = Image1->Canvas->TextWidth(((TEdit*)Sender)->Text + "###");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Exit(TObject *Sender)
{
	if (text)
	{
        ((TEdit*)Sender)->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
	FontDialog1->Font = Font;
	if (FontDialog1->Execute() == mrOk)
	{
		Font = FontDialog1->Font;
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Font");
		curNode->data.prev->data->Add(Font->Name);
		curNode->data.prev->data->Add(IntToStr(Font->Size));
		curNode->data.prev->data->Add("0x" + IntToHex((int)Font->Color));
		curNode->data.prev->data->Add(BoolToStr(Font->Style.Contains(fsBold)));
		curNode->data.prev->data->Add(BoolToStr(Font->Style.Contains(fsItalic)));
		curNode->data.prev->data->Add(BoolToStr(Font->Style.Contains(fsUnderline)));
		curNode->data.prev->data->Add(BoolToStr(Font->Style.Contains(fsStrikeOut)));

	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Shape1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
    penColorSelected = true;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Shape2MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
    penColorSelected = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
	clearImg();
	if (curNode->out.size() != 0)
		createBranch(history,curNode);
	curNode->data.prev->data->Add("#Clear");

}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
	rotate(NumberBox2->Value);
    if (curNode->out.size() != 0)
		createBranch(history,curNode);
	curNode->data.prev->data->Add("#Rotate");
	curNode->data.prev->data->Add(IntToStr((int)NumberBox2->Value));

}
//---------------------------------------------------------------------------

void __fastcall TForm1::View2Click(TObject *Sender)
{
    if (SaveDialog1->Execute() == mrOk)
	{
		GraphToStr(history,curNode,true)->SaveToFile(SaveDialog1->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Open1Click(TObject *Sender)
{
    if (OpenDialog1->Execute() == mrOk)
	{
		imageLog->LoadFromFile(OpenDialog1->FileName);
		imgGraphClear(history);
		GraphFromStr(history, curNode, imageLog);
		delete imageLog;
		imageLog = getCurLog(history, curNode);
		readLog(imageLog);
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Open2Click(TObject *Sender)
{
	undo(history, curNode);
	update();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Redo2Click(TObject *Sender)
{
    redo(history, curNode);
	update();
}
//---------------------------------------------------------------------------

