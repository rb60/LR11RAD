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
TForm2 *Tree;
Graph <TStringList*, int> *history;
Node <TStringList*, int>* curNode;
int nodeCount = 0;
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
    Tree = new TForm2(this);

	imageLog = new TStringList();
	buffer = new TBitmap();

	history = new Graph<TStringList*, int>();
	Node <TStringList*, int>* start = new Node <TStringList*, int>(nodeCount++);
	Node <TStringList*, int>* end = new Node <TStringList*, int>(nodeCount++);
	TStringList* firstLog = new TStringList();
    firstLog->Add("#PenColor");
	firstLog->Add("0");
	firstLog->Add("#BrushColor");
	firstLog->Add("16777215");
	firstLog->Add("#PenWidth");
	firstLog->Add("1");
	history->CreateGraph(start,end, firstLog);
	curNode = end;
}
//---------------------------------------------------------------------------

TStringList* GraphString(Graph <TStringList*, int> *g)
{
   TStringList* result = new TStringList();
   for (int i = 0; i < g->nodes.size(); i++)
   {
		result->Add((g->nodes[i] == curNode ? "CURNode#" : "Node#") + IntToStr(g->nodes[i]->data));
		for (int j = 0; j < g->nodes[i]->out.size(); j++)
		{
			result->Add("\tConection to Node#" + IntToStr(g->nodes[i]->out[j]->end->data));
			result->AddStrings(g->nodes[i]->out[j]->data);
		}
        result->Add("");
   }
   return result;


}


void createBranch()
{
	if (curNode->out.size() != 0)
	{
		Node <TStringList*, int> *newNode = new Node <TStringList*, int>(nodeCount++);
		history->addNode(curNode, newNode, new TStringList());
		curNode = newNode;

	}
}

void separateBranch(Graph <TStringList*, int> *g, Node<TStringList*, int>*& cur, bool before = true)
{

	TStringList* newList = new TStringList();
	Node<TStringList*, int> *newNode = new Node <TStringList*, int>(nodeCount++);
	if (before)
	{
        String currentStr;
		do
		{
			currentStr = cur->in[0]->data->Strings[cur->in[0]->data->Count - 1];
			newList->Insert(0, currentStr);
			cur->in[0]->data->Delete(cur->in[0]->data->Count - 1);
		}
		while(	currentStr != "#Move" &&
				currentStr != "#PenColor" &&
				currentStr != "#BrushColor" &&
				currentStr != "#PenWidth" &&
				cur->in[0]->data->Count > 0);

		g->addNode(cur->in[0]->start ,newNode , cur, cur->in[0]->data, newList);

		for (int i = 1; i < cur->in.size(); i++)
		{
			g->addBranch(cur->in[i]->start,newNode, cur->in[i]->data);
		}
	}
	else
	{
		String currentStr = cur->out[0]->data->Strings[0];
		do
		{
			newList->Add(currentStr);
			cur->out[0]->data->Delete(0);
            if (cur->out[0]->data->Count > 0)
			{
				currentStr = cur->out[0]->data->Strings[0];
			}
		}
		while(	currentStr != "#Move" &&
				currentStr != "#PenColor" &&
				currentStr != "#BrushColor" &&
				currentStr != "#PenWidth" &&
				cur->out[0]->data->Count > 0);
		g->addNode(cur,newNode , cur->out[0]->end, newList, cur->out[0]->data);
	}
	cur = newNode;

}

void unionBranch(Graph <TStringList*, int> *g, Node<TStringList*, int>*& cur, bool setToStart = true)
{
	TStringList* newList = new TStringList();
	newList->AddStrings(cur->in[0]->data);
	newList->AddStrings(cur->out[0]->data);
	Node<TStringList*, int>* start = cur->in[0]->start;
	Node<TStringList*, int>* end = cur->out[0]->end;
	g->addBranch(start, end, newList);
	g->removeNode(cur);
    cur = setToStart ? start : end;
}

void undo(Graph <TStringList*, int> *g, Node<TStringList*, int>*& cur)
{
	if (cur->in.size() == 0)
	{
        return;
	}
	if (cur->out.size() != 1 || cur->in.size() != 1)
	{
		separateBranch(g,cur);
	}
	else if (cur->in[0]->data->Count != 0)
	{
		String currentStr;
		do
		{
			currentStr = cur->in[0]->data->Strings[cur->in[0]->data->Count - 1];
			cur->out[0]->data->Insert(0,currentStr);
			cur->in[0]->data->Delete(cur->in[0]->data->Count - 1);
		}
		while(	currentStr != "#Move" &&
				currentStr != "#PenColor" &&
				currentStr != "#BrushColor" &&
				currentStr != "#PenWidth" &&
				cur->in[0]->data->Count > 0);
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
		if (buf->in.size() != 0)
		{
			separateBranch(g,buf);
            cur = buf;
		}
    }
}

void redo(Graph <TStringList*, int> *g, Node<TStringList*, int>*& cur)
{
	if (cur->out.size() == 0)
	{
        return;
	}

	if (cur->out.size() != 1 || cur->in.size() != 1)
	{
        separateBranch(g,cur,false);
	}
	else if (cur->out[0]->data->Count != 0)
	{
		String currentStr = cur->out[0]->data->Strings[0];

		do
		{
			cur->in[0]->data->Add(currentStr);
			cur->out[0]->data->Delete(0);
			if (cur->out[0]->data->Count > 0)
			{
				currentStr = cur->out[0]->data->Strings[0];
			}
		}
		while(	currentStr != "#Move" &&
				currentStr != "#PenColor" &&
				currentStr != "#BrushColor" &&
				currentStr != "#PenWidth" &&
				cur->out[0]->data->Count > 0);
	}
	else
	{
		unionBranch(g,cur,false);
	}
}

TStringList* getCurLog(Node<TStringList*, int>* n)
{
	n->passed = true;
	TStringList* result = new TStringList();
    TStringList* buf;
	for (int i = n->in.size() - 1 ; i >= 0; i--)
	{
		if (!n->in[i]->start->passed)
		{
			buf = getCurLog(n->in[i]->start);
			result->AddStrings(buf);
            delete buf;
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

void TForm1::clearImg()
{
    Image1->Canvas->Brush->Color	= clWhite;
	Image1->Canvas->Pen->Color	= clWhite;
   	Image1->Canvas->Rectangle(0,0, Image1->Width ,Image1->Height);

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
		createBranch();
		curNode->in[0]->data->Add("#Move");
		curNode->in[0]->data->Add(IntToStr(X));
		curNode->in[0]->data->Add(IntToStr(Y));
	}
	if (sbLine->Down)
	{
		line = true;
		((TImage*)Sender)->Canvas->MoveTo(X,Y);
		if (curNode->out.size() != 0)
		createBranch();
        curNode->in[0]->data->Add("#Move");
		curNode->in[0]->data->Add(IntToStr(X));
		curNode->in[0]->data->Add(IntToStr(Y));
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
        if (curNode->out.size() != 0)
		{
			Node <TStringList*, int> *newNode = new Node <TStringList*, int>(nodeCount++);
			history->addNode(curNode, newNode, new TStringList());
            curNode = newNode;

		}
		createBranch();
		curNode->in[0]->data->Add("#Line");
		curNode->in[0]->data->Add(IntToStr(X));
		curNode->in[0]->data->Add(IntToStr(Y));
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
        createBranch();
		curNode->in[0]->data->Add("#Line");
		curNode->in[0]->data->Add(IntToStr(X));
		curNode->in[0]->data->Add(IntToStr(Y));
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
	Form2->Memo1->Lines->SetStrings(getCurLog());
	Form2->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorBox1Change(TObject *Sender)
{
	if (!programingChange)
	{
		PenColor = ((TColorBox*)Sender)->Selected;
		createBranch();
		curNode->in[0]->data->Add("#PenColor");
		curNode->in[0]->data->Add(IntToStr(((TColorBox*)Sender)->Selected));
	}


}
//---------------------------------------------------------------------------

void __fastcall TForm1::ColorBox2Change(TObject *Sender)
{
	if (!programingChange)
	{
		BrushColor = ((TColorBox*)Sender)->Selected;
		createBranch();
		curNode->in[0]->data->Add("#BrushColor");
		curNode->in[0]->data->Add(IntToStr(((TColorBox*)Sender)->Selected));
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::NumberBox1ChangeValue(TObject *Sender)
{
	if (!programingChange)
	{
		Image1->Canvas->Pen->Width = ((TNumberBox*)Sender)->Value;
        createBranch();
		curNode->in[0]->data->Add("#PenWidth");
		curNode->in[0]->data->Add(IntToStr((int)((TNumberBox*)Sender)->Value));
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm1::ree1Click(TObject *Sender)
{
	Tree->Memo1->Lines = GraphString(history);
	Tree->Show();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Undo1Click(TObject *Sender)
{
	undo(history, curNode);
	clearImg();
	readLog(getCurLog());
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Redo1Click(TObject *Sender)
{
    redo(history, curNode);
	clearImg();
	readLog(getCurLog());
}
//---------------------------------------------------------------------------

