#pragma once
#include <vector>
#include <list>

template<typename TBranch, typename TNode>
class Node;

template<typename TBranch, typename TNode>
class Branch
{
public:
	Branch(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end);
	Branch(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end, TBranch data);
	Node<TBranch, TNode>*  start;
	Node<TBranch, TNode>* end;
	TBranch data;
};

template<typename TBranch, typename TNode>
class Node
{
public:
	Node(TNode data);
	void removeBranch(Branch<TBranch, TNode>* branch);
	std::vector<Branch<TBranch, TNode>*> in;
	std::vector<Branch<TBranch, TNode>*> out;
	TNode data;
	bool passed;

};

template<typename TBranch, typename TNode>
class Graph
{
public:
	Graph();
	void CreateGraph(Node<TBranch, TNode>* n1, Node<TBranch, TNode>* n2, TBranch b);
	void addNode(	Node<TBranch, TNode>* addTo,
					Node<TBranch, TNode>* newNode,
					TBranch bData);

	void addNode(	Node<TBranch, TNode>* previus,
					Node<TBranch, TNode>* newNode,
					Node<TBranch, TNode>* next,
					TBranch bData1,
					TBranch bData2);

	void addBranch(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end, TBranch data);
	void removeBranch(Branch<TBranch, TNode>* branch);
	void removeNode(Node<TBranch, TNode>* node);
	void setAllnotPassed();
	bool isAncestorOf(Node<TBranch, TNode>* n1, Node<TBranch, TNode>* n2);
	std::list<Node<TBranch, TNode>*> Path(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end);
	std::vector<Branch<TBranch, TNode>*> branches;
	std::vector<Node<TBranch, TNode>*> nodes;


};

template<typename TBranch, typename TNode>
inline Branch<TBranch, TNode>::Branch(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end)
{
	this->start = start;
	this->end = end;
}

template<typename TBranch, typename TNode>
inline Branch<TBranch, TNode>::Branch(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end, TBranch data)
{
	this->start = start;
	this->end = end;
	this->data = data;
}

template<typename TBranch, typename TNode>
inline Node<TBranch, TNode>::Node(TNode data)
{
	this->data = data;
	passed = false;
}

template<typename TBranch, typename TNode>
inline void Node<TBranch, TNode>::removeBranch(Branch<TBranch, TNode>* branch)
{
	for (int i = 0; i < in.size(); i++)
	{
		if (in[i] == branch)
		{
			in.erase(in.begin() + i);
			return;
		}
	}

	for (int i = 0; i < out.size(); i++)
	{
		if (out[i] == branch)
		{
			out.erase(out.begin() + i);
			return;
		}
	}
}

template<typename TBranch, typename TNode>
inline Graph<TBranch, TNode>::Graph()
{

}

template<typename TBranch, typename TNode>
inline void Graph<TBranch, TNode>::CreateGraph(Node<TBranch, TNode>* n1, Node<TBranch, TNode>* n2, TBranch b)
{
	Branch<TBranch, TNode> *branch = new Branch<TBranch, TNode>(n1, n2, b);
	n1->out.push_back(branch);
	n2->in.push_back(branch);
	nodes.push_back(n1);
	nodes.push_back(n2);
	branches.push_back(branch);

}

template<typename TBranch, typename TNode>
inline void Graph<TBranch, TNode>::addNode(Node<TBranch, TNode>* addTo, Node<TBranch, TNode>* newNode, TBranch bData)
{

	Branch<TBranch, TNode> *branch = new Branch<TBranch, TNode>(addTo, newNode, bData);
	addTo->out.push_back(branch);
	newNode->in.push_back(branch);
	nodes.push_back(newNode);
	branches.push_back(branch);

}

template<typename TBranch, typename TNode>
inline void Graph<TBranch, TNode>::addNode(Node<TBranch, TNode>* previus, Node<TBranch, TNode>* newNode, Node<TBranch, TNode>* next, TBranch bData1, TBranch bData2)
{
	for (int i = 0; i < previus->out.size(); i++)
	{
		if (previus->out[i]->end == next)
		{
			removeBranch(previus->out[i]);
			break;
		}
	}
	Branch<TBranch, TNode >* b1 = new Branch<TBranch, TNode>(previus, newNode, bData1);
	Branch<TBranch, TNode >* b2 = new Branch<TBranch, TNode>(newNode, next, bData2);
	previus->out.push_back(b1);
    newNode->in.push_back(b1);
	next->in.push_back(b2);
    newNode->out.push_back(b2);
	branches.push_back(b1);
	branches.push_back(b2);
	nodes.push_back(newNode);


}

template<typename TBranch, typename TNode>
inline void Graph<TBranch, TNode>::addBranch(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end, TBranch data)
{
	for (int i = 0; i < branches.size(); i++)
	{
		if (branches[i]->start == start && branches[i]->end == end)
		{
			return;
		}
	}
	Branch<TBranch, TNode>* newBranch = new Branch<TBranch, TNode>(start, end, data);
	start->out.push_back(newBranch);
	end->in.push_back(newBranch);
	branches.push_back(newBranch);
}

template<typename TBranch, typename TNode>
inline void Graph<TBranch, TNode>::removeBranch(Branch<TBranch, TNode>* branch)
{
	branch->start->removeBranch(branch);
	branch->end->removeBranch(branch);
	for (int i = 0; i < branches.size(); i++)
	{
		if (branches[i] == branch)
		{
			branches.erase(branches.begin() + i);
		}
	}
}

template<typename TBranch, typename TNode>
inline void Graph<TBranch, TNode>::removeNode(Node<TBranch, TNode>* node)
{
	for (int i = node->in.size() - 1; i >= 0; i--)
	{
		removeBranch(node->in[i]);
	}

	for (int i = node->out.size() - 1; i >= 0; i--)
	{
		removeBranch(node->out[i]);
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i] == node)
		{
			nodes.erase(nodes.begin() + i);
			break;
		}
	}
}

template<typename TBranch, typename TNode>
inline void Graph<TBranch, TNode>::setAllnotPassed()
{
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i]->passed = false;
	}
}

template<typename TBranch, typename TNode>
inline bool Graph<TBranch, TNode>::isAncestorOf(Node<TBranch, TNode>* n1, Node<TBranch, TNode>* n2)
{
	if (n1 == n2)
	{
		return true;
	}
	bool result = false;
	n1->passed = true;
	for (int i = 0; i < n1->out.size(); i++)
	{
		if (!n1->out[i]->end->passed && !result)
		{
			result = isAncestorOf(n1->out[i]->end, n2);
		}
	}
	n1->passed = false;
	return result;
}

template<typename TBranch, typename TNode>
inline std::list<Node<TBranch, TNode>*> Graph<TBranch, TNode>::Path(Node<TBranch, TNode>* start, Node<TBranch, TNode>* end)
{
	start->passed = true;
	std::list<Node<TBranch, TNode>*> result;
	if (start == end)
	{
		result.push_back(end);
		start->passed = false;
		return result;
	}

	for (int i = 0; i < start->out.size(); i++)
	{
		if (!start->out[i]->end->passed)
		{
			result = Path(start->out[i]->end, end);
			if (result.back() == end)
			{
				break;
			}
		}
	}

	result.push_front(start);
	start->passed = false;
	return result;
}

