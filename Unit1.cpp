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
bool parametric = false;
bool copy = false;
int imgx0, imgy0;
TStringList* imageLog;
TStringList* parametricLog;
TStringList* initialLog;
TBitmap* buffer;
TBitmap* copyBuffer;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	imageLog = new TStringList();
	parametricLog = new TStringList();
	buffer = new TBitmap();
    copyBuffer = new TBitmap();

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

void TForm1::findMinMax(int& minx, int& maxx, int& miny, int& maxy, TStringList* imglog)
{
	minx = miny = maxx = maxy = -1;
	int copyWidth, copyHeight;
	for (int i = 0; i < imglog->Count; i++)
	{
		if (	imglog->Strings[i] == "#Move" ||
				imglog->Strings[i] == "#Line" ||
				imglog->Strings[i] == "#Paste"
				)
		{
			bool mmPaste = imglog->Strings[i] == "#Paste";
			int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i]);

			if (mmPaste)
			{
				if (x + copyWidth > maxx || maxx == -1) maxx = x + copyWidth;
				if (y + copyHeight > maxy || maxy == -1) maxy = y + copyHeight;
			}

			if (x < minx || minx == -1) minx = x;
			if (x > maxx || maxx == -1) maxx = x;
			if (y < miny || miny == -1) miny = y;
			if (y > maxy || maxy == -1) maxy = y;
		}
		else if(imglog->Strings[i] == "#Rect" ||
				imglog->Strings[i] == "#Ellipse" ||
				imglog->Strings[i] == "#Copy")
		{
			bool mmPaste = imglog->Strings[i] == "#Copy";
			int x1,y1,x2,y2;
			i++;
			x1 = StrToInt(imglog->Strings[i++]);
			y1 = StrToInt(imglog->Strings[i++]);
			x2 = StrToInt(imglog->Strings[i++]);
			y2 = StrToInt(imglog->Strings[i]);

			if (mmPaste)
			{
				copyWidth = x2 - x1;
				copyHeight = y2 - y1;
			}

			if (x1 < minx || minx == -1) minx = x1;
			if (x1 > maxx || maxx == -1) maxx = x1;
			if (y1 < miny || miny == -1) miny = y1;
			if (y1 > maxy || maxy == -1) maxy = y1;

			if (x2 < minx || minx == -1) minx = x2;
			if (x2 > maxx || maxx == -1) maxx = x2;
			if (y2 < miny || miny == -1) miny = y2;
			if (y2 > maxy || maxy == -1) maxy = y2;
		}
		else if(imglog->Strings[i] == "#Pipette" ||
				imglog->Strings[i] == "#Flood")
		{
            int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i]);

            if (x < minx || minx == -1) minx = x;
			if (x > maxx || maxx == -1) maxx = x;
			if (y < miny || miny == -1) miny = y;
			if (y > maxy || maxy == -1) maxy = y;
        }
	}
}

TStringList* TForm1::toParametric(TStringList* imglog)
{
	int minx, maxx, miny, maxy;
	findMinMax(minx, maxx, miny, maxy, imglog);
	TStringList* result = new TStringList;
	result->Add(IntToStr(maxx - minx));
	result->Add(IntToStr(maxy - miny));
	for (int i = 0; i < imglog->Count; i++)
	{
		if (imglog->Strings[i] == "#Move" ||
            imglog->Strings[i] == "#Line" ||
			imglog->Strings[i] == "#EraseStart" ||
			imglog->Strings[i] == "#Erase" ||
			imglog->Strings[i] == "#Paste")
		{
			result->Add(imglog->Strings[i++]);
			int x = StrToInt(imglog->Strings[i++]);
			int y = StrToInt(imglog->Strings[i]);
			result->Add(IntToStr(x - minx));
			result->Add(IntToStr(y - miny));
		}
		else if (	imglog->Strings[i] == "#PenColor" ||
					imglog->Strings[i] == "#BrushColor" ||
					imglog->Strings[i] == "#PenWidth" ||
					imglog->Strings[i] == "#PenStyle" ||
					imglog->Strings[i] == "#BrushStyle")
		{
			result->Add(imglog->Strings[i++]);
            result->Add(imglog->Strings[i]);
		}
		else if(imglog->Strings[i] == "#Rect" ||
				imglog->Strings[i] == "#Ellipse" ||
				imglog->Strings[i] == "#Copy")
		{
            result->Add(imglog->Strings[i++]);
			int x1,y1,x2,y2;
			x1 = StrToInt(imglog->Strings[i++]);
			y1 = StrToInt(imglog->Strings[i++]);
			x2 = StrToInt(imglog->Strings[i++]);
			y2 = StrToInt(imglog->Strings[i]);
			result->Add(IntToStr(x1 - minx));
			result->Add(IntToStr(y1 - miny));
			result->Add(IntToStr(x2 - minx));
			result->Add(IntToStr(y2 - miny));
		}
		else if(imglog->Strings[i] == "#Pipette" ||
				imglog->Strings[i] == "#Flood")
		{
			result->Add(imglog->Strings[i++]);
			int x,y;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i++]);
            result->Add(IntToStr(x - minx));
			result->Add(IntToStr(y - miny));
			result->Add(imglog->Strings[i]);

		}
		else if(imglog->Strings[i] == "#EraseEnd")
		{
			result->Add(imglog->Strings[i]);
		}
	}
	result->Add("#ParametricEnd");
    return result;
}

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
	int pX0, pY0, pPenWidth;
	float pDx, pDy;
	TColor pBrushColor, pPenColor;
	TPenStyle pPenStyle;
	TBrushStyle pBrushStyle;
	bool parametricLog = false;
	for (int i = 0; i < imglog->Count; i++)
	{
		if (imglog->Strings[i] == "#Move")
		{
			i++;
			int x = StrToInt(imglog->Strings[i++]);
			int y = StrToInt(imglog->Strings[i]);
			if (parametricLog)
			{
				x = pX0 + pDx*x;
				y = pY0 + pDy*y;
            }
			Image1->Canvas->MoveTo(x,y);
		}
		else if (imglog->Strings[i] == "#Line")
		{
			i++;
			int x = StrToInt(imglog->Strings[i++]);
			int y = StrToInt(imglog->Strings[i]);
			if (parametricLog)
			{
				x = pX0 + pDx*x;
				y = pY0 + pDy*y;
            }
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
			PenWidth = parametric ? w*min(pDx, pDy) : w;

		}
		else if(imglog->Strings[i] == "#Rect")
		{
			int x1,y1,x2,y2;
			i++;
			x1 = StrToInt(imglog->Strings[i++]);
			y1 = StrToInt(imglog->Strings[i++]);
			x2 = StrToInt(imglog->Strings[i++]);
			y2 = StrToInt(imglog->Strings[i]);
			if (parametricLog)
			{
				x1 = pX0 + pDx*x1;
				y1 = pY0 + pDy*y1;
				x2 = pX0 + pDx*x2;
				y2 = pY0 + pDy*y2;
            }
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
			if (parametricLog)
			{
				x1 = pX0 + pDx*x1;
				y1 = pY0 + pDy*y1;
				x2 = pX0 + pDx*x2;
				y2 = pY0 + pDy*y2;
			}
			Image1->Canvas->Ellipse(x1,y1,x2,y2);
		}
        else if(imglog->Strings[i] == "#Pipette")
		{
			int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i++]);
			if (parametricLog)
			{
				x = pX0 + pDx*x;
				y = pY0 + pDy*y;
			}
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
			if (parametricLog)
			{
				x = pX0 + pDx*x;
				y = pY0 + pDy*y;
			}
			Image1->Canvas->Rectangle(x - PenWidth*3, y - PenWidth*3, x + PenWidth*3, y + PenWidth*3);


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
		else if (imglog->Strings[i] == "#ParametricStart")
		{
			int x2, y2, W, H;
			i++;
			pX0 = StrToInt(imglog->Strings[i++]);
			pY0 = StrToInt(imglog->Strings[i++]);
			x2 = StrToInt(imglog->Strings[i++]);
			y2 = StrToInt(imglog->Strings[i++]);
			W  = StrToInt(imglog->Strings[i++]);
			H  = StrToInt(imglog->Strings[i]);
			pDx = (x2 - pX0)/(float)W;
			pDy = (y2 - pY0)/(float)H;
			parametricLog = true;
			pPenWidth = PenWidth;
			pPenColor = PenColor;
			pPenStyle = PenStyle;
			pBrushColor = BrushColor;
			pBrushStyle = BrushStyle;
		}
		else if (imglog->Strings[i] == "#ParametricEnd")
		{
			parametricLog = false;
			PenWidth = pPenWidth;
			PenColor = pPenColor;
			PenStyle = pPenStyle;
			BrushColor = pBrushColor;
			BrushStyle = pBrushStyle;
		}
		else if (imglog->Strings[i] == "#Copy")
		{
            int x1,y1,x2,y2;
			i++;
			x1 = StrToInt(imglog->Strings[i++]);
			y1 = StrToInt(imglog->Strings[i++]);
			x2 = StrToInt(imglog->Strings[i++]);
			y2 = StrToInt(imglog->Strings[i]);
			if (parametricLog)
			{
				x1 = pX0 + pDx*x1;
				y1 = pY0 + pDy*y1;
				x2 = pX0 + pDx*x2;
				y2 = pY0 + pDy*y2;
			}
			TRect source(x1, y1, x2, y2);
			TRect dest(0,0, x2 - x1, y2 - y1);
			copyBuffer->Height = dest.Height();
			copyBuffer->Width = dest.Width();
			copyBuffer->Canvas->CopyRect(dest, Image1->Canvas, source);

		}
        else if (imglog->Strings[i] == "#Paste")
		{
			int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i]);
			if (parametricLog)
			{
				x = pX0 + pDx*x;
				y = pY0 + pDy*y;
			}
			TRect source(0,0, copyBuffer->Width, copyBuffer->Height);
			TRect dest(x,y, x + copyBuffer->Width, y + copyBuffer->Height);
			Image1->Canvas->CopyRect(dest, copyBuffer->Canvas, source);

		}
		else if(imglog->Strings[i] == "#Flood")
		{
            int x,y;
			i++;
			x = StrToInt(imglog->Strings[i++]);
			y = StrToInt(imglog->Strings[i++]);
			if (parametricLog)
			{
				x = pX0 + pDx*x;
				y = pY0 + pDy*y;
			}

			if (imglog->Strings[i] == "Surface")
			{
				Image1->Canvas->FloodFill(x,y,Image1->Canvas->Pixels[x][y],fsSurface);
			}
			else
			{
                Image1->Canvas->FloodFill(x,y,PenColor,fsBorder);
            }
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
	if (sbParametric->Down && parametricLog->Count > 0)
	{
		parametric = true;
		buffer->Assign(Image1->Picture->Bitmap);
	}
	if (sbCopy->Down)
	{
        buffer->Assign(Image1->Picture->Bitmap);
        copy = true;
	}
	if (sbPaste->Down)
	{
		TRect source(0,0, copyBuffer->Width, copyBuffer->Height);
		TRect dest(X,Y, X + copyBuffer->Width, Y + copyBuffer->Height);
		((TImage*)Sender)->Canvas->CopyRect(dest, copyBuffer->Canvas, source);
        if (curNode->out.size() != 0)
            createBranch(history,curNode);
		curNode->data.prev->data->Add("#Paste");
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));
	}
	if (sbFlood->Down)
	{
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Flood");
		curNode->data.prev->data->Add(IntToStr(X));
		curNode->data.prev->data->Add(IntToStr(Y));

		if (ComboBox3->ItemIndex == 0)
		{
			((TImage*)Sender)->Canvas->FloodFill(X,Y,((TImage*)Sender)->Canvas->Pixels[X][Y],fsSurface);
			curNode->data.prev->data->Add("Surface");

		}
		else
		{
			((TImage*)Sender)->Canvas->FloodFill(X,Y,PenColor,fsBorder);
            curNode->data.prev->data->Add("Border");
        }
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
	if (parametric)
	{
		float ratio = (float)StrToInt(parametricLog->Strings[0]) / (float)StrToInt(parametricLog->Strings[1]);
		((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		imageLog->Clear();
		imageLog->Add("#ParametricStart");
		imageLog->Add(IntToStr(imgx0));
		imageLog->Add(IntToStr(imgy0));
        if (Shift.Contains(ssShift))
		{
			imageLog->Add(IntToStr((int)(abs(X - imgx0) > abs(Y - imgy0)*ratio ? X : imgx0 + (Y - imgy0)*ratio)));
			imageLog->Add(IntToStr((int)(abs(X - imgx0) > abs(Y - imgy0)*ratio ? imgy0 + (X - imgx0)/ratio : Y)));
		}
		else
		{
			imageLog->Add(IntToStr(X));
			imageLog->Add(IntToStr(Y));
		}
		imageLog->AddStrings(parametricLog);
		readLog(imageLog);
	}
	if (copy)
	{
		((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		TRect source(imgx0,imgy0, X, Y);
		((TImage*)Sender)->Canvas->DrawFocusRect(source);
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	if (line)
	{
        ((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Line");
        ((TImage*)Sender)->Canvas->MoveTo(imgx0,imgy0);
        if (Shift.Contains(ssShift))
		{
			curNode->data.prev->data->Add(IntToStr(abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0));
			curNode->data.prev->data->Add(IntToStr(abs(X - imgx0) > abs(Y - imgy0) ? imgy0 : Y));
            ((TImage*)Sender)->Canvas->LineTo(	abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0,
												abs(X - imgx0) > abs(Y - imgy0) ? imgy0 : Y);
		}
		else
		{
			curNode->data.prev->data->Add(IntToStr(X));
			curNode->data.prev->data->Add(IntToStr(Y));
            ((TImage*)Sender)->Canvas->LineTo(X,Y);
		}
	}
	if (rect)
	{
    	((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
        if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Rect");
		curNode->data.prev->data->Add(IntToStr(imgx0));
		curNode->data.prev->data->Add(IntToStr(imgy0));
		if (Shift.Contains(ssShift))
		{
            curNode->data.prev->data->Add(IntToStr(abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0 + (Y - imgy0)));
			curNode->data.prev->data->Add(IntToStr(abs(X - imgx0) > abs(Y - imgy0) ? imgy0 + (X - imgx0) : Y));
			((TImage*)Sender)->Canvas->Rectangle(imgx0,imgy0,
												 abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0 + (Y - imgy0),
												 abs(X - imgx0) > abs(Y - imgy0) ? imgy0 + (X - imgx0) : Y);
		}
		else
		{
			curNode->data.prev->data->Add(IntToStr(X));
			curNode->data.prev->data->Add(IntToStr(Y));
            ((TImage*)Sender)->Canvas->Rectangle(imgx0,imgy0,X,Y);
		}


	}
	if (elipse)
	{
		((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
        if (curNode->out.size() != 0)
			createBranch(history,curNode);
		curNode->data.prev->data->Add("#Ellipse");
		curNode->data.prev->data->Add(IntToStr(imgx0));
		curNode->data.prev->data->Add(IntToStr(imgy0));
		if (Shift.Contains(ssShift))
		{
            curNode->data.prev->data->Add(IntToStr(abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0 + (Y - imgy0)));
			curNode->data.prev->data->Add(IntToStr(abs(X - imgx0) > abs(Y - imgy0) ? imgy0 + (X - imgx0) : Y));
            ((TImage*)Sender)->Canvas->Ellipse(imgx0,imgy0,
												 abs(X - imgx0) > abs(Y - imgy0) ? X : imgx0 + (Y - imgy0),
												 abs(X - imgx0) > abs(Y - imgy0) ? imgy0 + (X - imgx0) : Y);
		}
		else
		{
			curNode->data.prev->data->Add(IntToStr(X));
			curNode->data.prev->data->Add(IntToStr(Y));
            ((TImage*)Sender)->Canvas->Ellipse(imgx0,imgy0,X,Y);
		}
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
	if (parametric)
	{
		float ratio = (float)StrToInt(parametricLog->Strings[0]) / (float)StrToInt(parametricLog->Strings[1]);
        ((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		imageLog->Clear();
		imageLog->Add("#ParametricStart");
		imageLog->Add(IntToStr(imgx0));
		imageLog->Add(IntToStr(imgy0));
         if (Shift.Contains(ssShift))
		{
			imageLog->Add(IntToStr((int)(abs(X - imgx0) > abs(Y - imgy0)*ratio ? X : imgx0 + (Y - imgy0)*ratio)));
			imageLog->Add(IntToStr((int)(abs(X - imgx0) > abs(Y - imgy0)*ratio ? imgy0 + (X - imgx0)/ratio : Y)));
		}
		else
		{
			imageLog->Add(IntToStr(X));
			imageLog->Add(IntToStr(Y));
		}
		imageLog->AddStrings(parametricLog);
        readLog(imageLog);
        if (curNode->out.size() != 0)
            createBranch(history,curNode);
		curNode->data.prev->data->AddStrings(imageLog);
	}
	if (copy)
	{
        ((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		TRect source(imgx0,imgy0, X, Y);
		TRect dest(0,0, X - imgx0, Y - imgy0);
		copyBuffer->Height = dest.Height();
		copyBuffer->Width = dest.Width();
		copyBuffer->Canvas->CopyRect(dest, ((TImage*)Sender)->Canvas, source);
        if (curNode->out.size() != 0)
            createBranch(history,curNode);
		curNode->data.prev->data->Add("#Copy");
		curNode->data.prev->data->Add(IntToStr(imgx0));
		curNode->data.prev->data->Add(IntToStr(imgy0));
		curNode->data.prev->data->Add(IntToStr(X));
        curNode->data.prev->data->Add(IntToStr(Y));

	}
	Form3->update();
	rect = false;
	line = false;
	pen = false;
	elipse = false;
	erase = false;
	parametric = false;
	copy = false;
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
		imageLog->SetStrings(initialLog);
		imageLog->AddStrings(getCurLog(history, curNode));
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

void __fastcall TForm1::Export2Click(TObject *Sender)
{
	if (SaveDialog1->Execute() == mrOk)
	{
    	imageLog->SetStrings(initialLog);
		imageLog->AddStrings(getCurLog(history, curNode));
		toParametric(imageLog)->SaveToFile(SaveDialog1->FileName);
        imageLog->SetStrings(initialLog);
	}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::SpeedButton5Click(TObject *Sender)
{
	if (OpenDialog1->Execute() == mrOk)
	{
		parametricLog->LoadFromFile(OpenDialog1->FileName);
	}
}
//---------------------------------------------------------------------------






