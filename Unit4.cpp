//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
TStringList* GraphString(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur)
{
	TStringList* result = new TStringList();
	for (int i = 0; i < g->nodes.size(); i++)
	{
		result->Add((g->nodes[i] == cur ? "CURNode#" : "Node#") + IntToStr(g->nodes[i]->data.number));
		result->Add("\tNext Node#" + (g->nodes[i]->data.next != nullptr ? IntToStr(g->nodes[i]->data.next->end->data.number) : "NONE"));
        result->Add("\tPrev Node#" + (g->nodes[i]->data.prev != nullptr ? IntToStr(g->nodes[i]->data.prev->start->data.number) : "NONE"));
		for (int j = 0; j < g->nodes[i]->out.size(); j++)
		{
			result->Add("\tConection to Node#" + IntToStr(g->nodes[i]->out[j]->end->data.number));
			result->AddStrings(g->nodes[i]->out[j]->data);
		}
		result->Add("");
	}
	return result;


}

void createBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur)
{

	Node <TStringList*, NodeData> *newNode = new Node <TStringList*, NodeData>({(int)g->nodes.size(), nullptr, nullptr});
	g->addNode(cur, newNode, new TStringList());
    newNode->data.prev = newNode->in[0];
	cur = newNode;
}


void moveBackward(Node<TStringList*, NodeData>* cur)
{
	String currentStr;
	do
	{
		currentStr = cur->data.prev->data->Strings[cur->data.prev->data->Count - 1];
		cur->data.next->data->Insert(0,currentStr);
		cur->data.prev->data->Delete(cur->data.prev->data->Count - 1);
	}
	while(	currentStr != "#Move" &&
			currentStr != "#PenColor" &&
			currentStr != "#BrushColor" &&
			currentStr != "#PenWidth" &&
			currentStr != "#Rect" &&
			currentStr != "#PenStyle" &&
			currentStr != "#BrushStyle" &&
			currentStr != "#Text" &&
			currentStr != "#Font" &&
			currentStr != "#Ellipse" &&
			currentStr != "#Pipette" &&
			cur->data.prev->data->Count > 0);
}

void moveForward(Node<TStringList*, NodeData>* cur)
{
	String currentStr = cur->data.next->data->Strings[0];
	do
	{
		cur->data.prev->data->Add(currentStr);
		cur->data.next->data->Delete(0);
		if (cur->data.next->data->Count > 0)
		{
			currentStr = cur->data.next->data->Strings[0];
		}
	}
	while(	currentStr != "#Move" &&
			currentStr != "#PenColor" &&
			currentStr != "#BrushColor" &&
			currentStr != "#PenWidth" &&
			currentStr != "#Rect" &&
			currentStr != "#PenStyle" &&
			currentStr != "#BrushStyle" &&
			currentStr != "#Text" &&
			currentStr != "#Font" &&
			currentStr != "#Ellipse" &&
            currentStr != "#Pipette" &&
			cur->data.next->data->Count > 0);
}

void separateBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, bool before)
{

	TStringList* newList = new TStringList();
	Node<TStringList*, NodeData> *newNode = new Node <TStringList*, NodeData>({(int)g->nodes.size(), nullptr, nullptr});
	if (before)
	{
		g->addNode(cur->data.prev->start ,newNode , cur, cur->data.prev->data, newList);
		newNode->data.prev = newNode->in[0];
		newNode->data.next = newNode->out[0];
        cur->data.prev->start->data.next = cur->data.prev->start->out.back();
        cur->data.prev = cur->in.back();
		for (int i = 0; i < cur->in.size(); i++)
		{
			if (cur->in[i]->start != newNode)
			{
				g->addBranch(cur->in[i]->start,newNode, cur->in[i]->data);
			}
		}
		moveBackward(newNode);

	}
	else
	{
		g->addNode(cur,newNode , cur->data.next->end, newList, cur->data.next->data);
        newNode->data.prev = newNode->in[0];
		newNode->data.next = newNode->out[0];
        cur->data.next->end->data.prev = cur->data.next->end->out.back();
        cur->data.next = cur->out.back();
		moveForward(newNode);
	}
	cur = newNode;

}

void unionBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, bool setToStart)
{
	TStringList* newList = new TStringList();
	newList->AddStrings(cur->data.prev->data);
	newList->AddStrings(cur->data.next->data);
	Node<TStringList*, NodeData>* start = cur->data.prev->start;
	Node<TStringList*, NodeData>* end = cur->data.next->end;
	g->addBranch(start, end, newList);
	start->data.next  = start->out.back();
    end->data.prev  = end->in.back();
	g->removeNode(cur);
    cur = setToStart ? start : end;
}

void undo(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur)
{
	if (cur->data.prev == nullptr)
	{
        return;
	}
	if (cur->out.size() != 1)
	{
		separateBranch(g,cur);
	}
	else if (cur->data.prev->data->Count != 0)
	{
		moveBackward(cur);
	}
	else
	{
		Node<TStringList*, NodeData>* buf = cur->data.prev->start;
		for (int i = 0; i < cur->in.size(); i++)
		{
		   if (cur->data.prev->start != cur->in[i]->start && g->isAncestorOf(cur->data.prev->start, cur->in[i]->start))
			{
				buf = cur->in[i]->start;
				break;
			}
		}

		if (buf->in.size() != 0)
		{
			separateBranch(g,buf);
			for (int i = 0; i < cur->in.size(); i++)
			{
				if (cur->data.prev->start != cur->in[i]->start &&
					buf->data.next->end	 != cur->in[i]->start)
				{
                   g->addBranch(cur->in[i]->start, buf, cur->in[i]->data);
				}
			}
		}
		unionBranch(g,cur);
		cur = buf;
	}
}

void redo(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur)
{
	if (cur->data.next == nullptr)
	{
		return;
	}

	if (cur->out.size() != 1 || cur->in.size() != 1)
	{
		separateBranch(g,cur,false);
	}
	else
	{
		if (cur->data.next->data->Count != 0)
		{
			moveForward(cur);
		}

		if (cur->data.next->data->Count == 0)
		{
			unionBranch(g,cur,false);
		}
	}
}

TStringList* getCurLog(Node<TStringList*, NodeData>* n)
{

	n->passed = true;
	TStringList* result = new TStringList();
	TStringList* buf;
	for (int i = n->in.size() - 1 ; i >= 0; i--)
	{
		if (n->in[i]->start	!= n->data.prev->start)
		{
			if (!n->in[i]->start->passed)
			{
				buf = getCurLog(n->in[i]->start);
				result->AddStrings(buf);
				delete buf;
			}
			result->AddStrings(n->in[i]->data);
		}

	}

	if (n->data.prev != nullptr)
	{
		if (!n->data.prev->start->passed)
		{
			buf = getCurLog(n->data.prev->start);
			result->AddStrings(buf);
			delete buf;
		}
		result->AddStrings(n->data.prev->data);
	}



	return result;

}

void merge(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur, Node<TStringList*, NodeData>* end)
{
	if (!g->isAncestorOf(end,cur))
	{
        g->addBranch(cur,end,new TStringList());
	}

}

TStringList* getCurLog(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur)
{
   TStringList* result = getCurLog(cur);
   g->setAllnotPassed();
   return result;
}

#pragma package(smart_init)
