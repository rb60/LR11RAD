//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------

void imgGraphClear(Graph <TStringList*, NodeData> *g)
{
	for (int i = 0; i < g->branches.size(); i++)
	{
		delete g->branches[i]->data;
	}
    g->Clear();
}

void GraphFromStr(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, TStringList* Str)
{
	imgGraphClear(g);

	TStringList* buf;
	int nodeCount = StrToInt(Str->Strings[0]);
	for (int i = 0; i < nodeCount; i++)
	{
		g->nodes.push_back(new Node<TStringList*, NodeData>({i,nullptr,nullptr}));
	}

	for (int i = 1, curNode = 0; i < Str->Count; i++)
	{
		if (Str->Strings[i] == "##Node")
		{
			curNode = StrToInt(Str->Strings[++i]);
		}
		else if(Str->Strings[i] == "##CURNode")
		{
			curNode = StrToInt(Str->Strings[++i]);
			cur = g->nodes[curNode];
		}
		else if(Str->Strings[i] == "##ConnStart")
		{
			buf = new TStringList();
			i++;
			int connTo = StrToInt(Str->Strings[i++]);
			g->addBranch(g->nodes[curNode],g->nodes[connTo],buf);
			for (; Str->Strings[i] != "##ConnEnd"; i++)
			{
				buf->Add(Str->Strings[i]);   
			}
		}
		else if(Str->Strings[i] == "##NodePrevNext")
		{
			i++;
			curNode = StrToInt(Str->Strings[i++]);
			int next = StrToInt(Str->Strings[i++]);
			int prev = StrToInt(Str->Strings[i]);
			for (int j = 0; j < g->nodes[curNode]->in.size(); j++)
			{
				if (g->nodes[curNode]->in[j]->start->data.number == prev)
				{
					g->nodes[curNode]->data.prev = g->nodes[curNode]->in[j];
					break;		
				}	
			}
			for (int j = 0; j < g->nodes[curNode]->out.size(); j++)
			{
				if (g->nodes[curNode]->out[j]->end->data.number == next)
				{
					g->nodes[curNode]->data.next = g->nodes[curNode]->out[j];
					break;		
				}	
			}
        }
	}
}

void BranchesToStr(Node<TStringList*, NodeData> *n, Node<TStringList*, NodeData>* cur, TStringList* Str)
{
	n->passed = true;
	Str->Add((n == cur ? "##CURNode" : "##Node") + IntToStr(n->data.number));
	Str->Add("Next#" + (n->data.next != nullptr ? IntToStr(n->data.next->end->data.number) : "NONE"));
	Str->Add("Prev#" + (n->data.prev != nullptr ? IntToStr(n->data.prev->start->data.number) : "NONE"));

	if (n->data.next != nullptr)
	{
		Str->Add("\t##Connection " + IntToStr(n->data.next->end->data.number));
		Str->AddStrings(n->data.next->data);
	}

	for (int i = 0; i < n->out.size(); i++)
	{
		if (n->out[i] != n->data.next)
		{
			Str->Add("\t##Connection " + IntToStr(n->out[i]->end->data.number));
			Str->AddStrings(n->out[i]->data);
		}

	}
	Str->Add("");

	if (n->data.next != nullptr && !n->data.next->end->passed)
	{
		BranchesToStr(n->data.next->end , cur, Str);
	}

	for (int i = 0; i < n->out.size(); i++)
	{
		if (n->out[i] != n->data.next  && !n->out[i]->end->passed)
		{
			BranchesToStr(n->out[i]->end , cur, Str);
		}
	}

	if (n->data.prev != nullptr && !n->data.prev->start->passed)
	{
		BranchesToStr(n->data.prev->start , cur, Str);
	}

	for (int i = 0; i < n->in.size(); i++)
	{
	   if (n->in[i] != n->data.prev && !n->in[i]->start->passed)
		{
			BranchesToStr(n->in[i]->start , cur, Str);
		}
	}

}

TStringList* GraphToStr(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur, bool revertable)
{
	TStringList* result = new TStringList();

	if (revertable)
	{
        	result->Add(IntToStr((int)g->nodes.size()));
		for (int i = 0; i < g->nodes.size(); i++)
		{
			result->Add(g->nodes[i] == cur ? "##CURNode" : "##Node");
			result->Add(IntToStr(i));
			for (int j = 0; j < g->nodes[i]->out.size(); j++)
			{
				result->Add("##ConnStart");
				result->Add(IntToStr(g->nodes[i]->out[j]->end->data.number));
				result->AddStrings(g->nodes[i]->out[j]->data);
				result->Add("##ConnEnd");
			}
		}

		for (int i = 0; i < g->nodes.size(); i++)
		{
			result->Add("##NodePrevNext");
			result->Add(IntToStr(i));
			result->Add(g->nodes[i]->data.next != nullptr ? IntToStr(g->nodes[i]->data.next->end->data.number) : "-1");
			result->Add(g->nodes[i]->data.prev != nullptr ? IntToStr(g->nodes[i]->data.prev->start->data.number) : "-1");
		}
	}
	else
	{
        BranchesToStr(g->nodes[0],cur, result);
		g->setAllnotPassed();
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
    bool parametric = false;
	do
	{
		currentStr = cur->data.prev->data->Strings[cur->data.prev->data->Count - 1];
		cur->data.next->data->Insert(0,currentStr);
		cur->data.prev->data->Delete(cur->data.prev->data->Count - 1);
		if (currentStr == "#ParametricEnd")
		{
            parametric = true;
		}


	}
	while(  (parametric ?
				currentStr != "#ParametricStart" :
			(currentStr != "#Move" &&
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
			currentStr != "#EraseStart" &&
			currentStr != "#Clear" &&
			currentStr != "#Rotate")) &&
			cur->data.prev->data->Count > 0);
}

void moveForward(Node<TStringList*, NodeData>* cur)
{
	String currentStr = cur->data.next->data->Strings[0];
    bool parametric = false;
	do
	{
		cur->data.prev->data->Add(currentStr);
		cur->data.next->data->Delete(0);
		if (currentStr == "#ParametricStart")
		{
            parametric = true;
		}

		if (currentStr == "#ParametricEnd")
		{
            parametric = false;
		}

		if (cur->data.next->data->Count > 0)
		{
			currentStr = cur->data.next->data->Strings[0];
		}
	}
	while(  (parametric ?
				true :
			(currentStr != "#ParametricStart" &&
			currentStr != "#Move" &&
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
			currentStr != "#EraseStart" &&
			currentStr != "#Clear" &&
			currentStr != "#Rotate")) &&
			cur->data.next->data->Count > 0);
}

void separateBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, bool before, bool replace)
{

	TStringList* newList = new TStringList();
	Node<TStringList*, NodeData> *newNode = new Node <TStringList*, NodeData>({replace ? (int)g->nodes.size() - 1 : (int)g->nodes.size(), nullptr, nullptr});
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
        cur->data.next->end->data.prev = cur->data.next->end->in.back();
        cur->data.next = cur->out.back();
		moveForward(newNode);
	}
	cur = newNode;

}

void unionBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, bool setToStart = true)
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
	delete cur;

    cur = setToStart ? start : end;
}

void undo(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur)
{
	if (cur->data.prev == nullptr)
	{
        return;
	}
	if (cur->data.next == nullptr || cur->out.size() > 1)
	{
		separateBranch(g,cur, true, false);
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
			separateBranch(g,buf, true, true);
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

	if (cur->data.prev == nullptr || cur->out.size() > 1)
	{
		separateBranch(g,cur,false, false);
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
	if (!g->isAncestorOf(end,cur) &&
		!g->isAncestorOf(cur,end))
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
