//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include "Graph.h"
#include <System.Classes.hpp>

struct NodeData
{
	int number;
	Branch <TStringList*, NodeData>* prev;
    Branch <TStringList*, NodeData>* next;
};

void imgGraphClear(Graph <TStringList*, NodeData> *g);
void GraphFromStr(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, TStringList* Str);
TStringList* GraphToStr(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur, bool revertable = false);
void createBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur);
void undo(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur);
void redo(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur);
void merge(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur, Node<TStringList*, NodeData>* end);
TStringList* getCurLog(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur);

typedef Graph<TStringList*, NodeData> ImgGraph;
typedef Node<TStringList*, NodeData> ImgNode;

#endif
