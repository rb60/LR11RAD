//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Graph.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
Graph <TStringList*, int> *history;
Node <TStringList*, int>* curNode;
bool programingChange = false;
bool pen = false;
bool line = false;
int imgx0, imgy0;
TStringList* imageLog;
TBitmap* buffer;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	imageLog = new TStringList();
	buffer = new TBitmap();

	history = new Graph<TStringList*, int>();
	Node <TStringList*, int>* start = new Node <TStringList*, int>(0);
	Node <TStringList*, int>* end = new Node <TStringList*, int>(0);
	TStringList* firstLog = new TStringList();
	history->CreateGraph(start,end, firstLog);
	curNode = end;
}
//---------------------------------------------------------------------------


void separateBranch(Graph <TStringList*, int> *g, Node<TStringList*, int>*& cur, bool before = true)
{

	TStringList* newList = new TStringList();
	Node<TStringList*, int> *newNode = new Node <TStringList*, int>(0);
	if (before)
	{
		newList->Add(cur->in[0]->data->Strings[cur->in[0]->data->Count - 1]);
		cur->in[0]->data->Delete(cur->in[0]->data->Count - 1);
		g->addNode(cur->in[0]->start	,newNode , cur, cur->in[0]->data, newList);

		for (int i = 1; i < cur->in.size(); i++)
		{
			g->addBranch(cur->in[i]->start,newNode, cur->in[i]->data);
		}
	}
	else
	{
		newList->Add(cur->out[0]->data->Strings[0]);
		cur->out[0]->data->Delete(0);
		g->addNode(cur,newNode , cur->out[0]->end, newList, cur->out[0]->data);
	}
	cur = newNode;

}

void unionBranch(Graph <TStringList*, int> *g, Node<TStringList*, int>*& cur)
{
	TStringList* newList = new TStringList();
	newList->AddStrings(cur->in[0]->data);
	newList->AddStrings(cur->out[0]->data);
	Node<TStringList*, int>* buf = cur->in[0]->start;
	g->addBranch(cur->in[0]->start, cur->out[0]->end, newList);
	g->removeNode(cur);
    cur = buf;
}

void undo(Graph <TStringList*, int> *g, Node<TStringList*, int>*& cur)
{

	if (cur->out.size() != 1)
	{
		separateBranch(g,cur);
	}
	else if (cur->in[0]->data->Count != 0)
	{
		cur->out[0]->data->Insert(0,cur->in[0]->data->Strings[cur->in[0]->data->Count - 1]);
		cur->in[0]->data->Delete(cur->in[0]->data->Count - 1);
	}
	else
	{
    	Node<TStringList*, int>* buf = cur->in[0]->start;
		for (int i = 1; i < cur->in.size(); i++)
		{
		   if (g->isAncestorOf(cur->in[0]->start,cur->in[i]->start))
			{
				buf = cur->in[i]->start;
                break;
			}
		}
		unionBranch(g,cur);
		separateBranch(g,buf);
    }
}


TStringList* getCurLog(Node<TStringList*, int>* n)
{
	n->passed = true;
	TStringList* result = new TStringList();
	for (int i = n->in.size() - 1; i >= 0; i++)
	{
		if (!n->in[i]->start->passed)
		{
            result->AddStrings(getCurLog(n->in[i]->start));
		}
		result->AddStrings(n->in[i]->data);
	}
    return result;

}

TStringList* getCurLog()
{
   TStringList* result = getCurLog(curNode);
   history->setAllnotPassed();
   return result;
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
        else if (imglog->Strings[i] == "#PenWidth")
		{
			i++;
			int w = StrToInt(imglog->Strings[i]);
			PenWidth = w;
			continue;
		}
	}
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
		imageLog->Add("#Move");
		imageLog->Add(IntToStr(X));
		imageLog->Add(IntToStr(Y));
	}
	if (sbLine->Down)
	{
		line = true;
		((TImage*)Sender)->Canvas->MoveTo(X,Y);
		imageLog->Add("#Move");
		imageLog->Add(IntToStr(X));
		imageLog->Add(IntToStr(Y));
        buffer->Assign(Image1->Picture->Bitmap);
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
	if (line)
	{
		((TImage*)Sender)->Picture->Bitmap->Assign(buffer);
		((TImage*)Sender)->Canvas->MoveTo(imgx0,imgy0);
		((TImage*)Sender)->Canvas->LineTo(X,Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	if (line)
	{
        imageLog->Add("#Line");
		imageLog->Add(IntToStr(X));
		imageLog->Add(IntToStr(Y));
	}
    line = false;
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

void __fastcall TForm1::NumberBox1ChangeValue(TObject *Sender)
{
	if (!programingChange)
	{
        Image1->Canvas->Pen->Width = ((TNumberBox*)Sender)->Value;
		imageLog->Add("#PenWidth");
		imageLog->Add(IntToStr((int)((TNumberBox*)Sender)->Value));
	}

}
//---------------------------------------------------------------------------

