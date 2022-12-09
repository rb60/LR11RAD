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

TStringList* GraphString(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur);
void createBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur);
void moveBackward(Node<TStringList*, NodeData>* cur);
void moveForward(Node<TStringList*, NodeData>* cur);
void separateBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, bool before = true);
void unionBranch(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur, bool setToStart = true);
void undo(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur);
void redo(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>*& cur);
void merge(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur, Node<TStringList*, NodeData>* end);
TStringList* getCurLog(Node<TStringList*, NodeData>* n);
TStringList* getCurLog(Graph <TStringList*, NodeData> *g, Node<TStringList*, NodeData>* cur);

typedef Graph<TStringList*, NodeData> ImgGraph;
typedef Node<TStringList*, NodeData> ImgNode;

#endif
