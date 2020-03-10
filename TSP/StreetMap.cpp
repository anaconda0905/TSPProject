#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <queue>
using namespace std;
	
class Node{
public:
	std::size_t key;
	vector<Node *>child;
	StreetSegment data;

	Node();
	Node(std::size_t, Node *);
	Node(std::size_t, StreetSegment);
	Node*  insert(Node*, std::size_t, Node*);
	void Inorder(Node*);
};


Node::Node(){
	
	key = 0;
}
Node::Node(std::size_t hashval, Node* node){
	key = hashval;
	data = node->data;
}

Node::Node(std::size_t hashval, StreetSegment streetseg){
	key = hashval;
	data = streetseg;
}


Node* Node::insert(Node* root, std::size_t hashval, Node* node){
	Node* tempNode;
	if (!root){
		Node *newnode = new Node(hashval, node);
		
		return newnode;
	}
	else{
		if (root->key == hashval || node->key != root->key){
			root->child.push_back(node);
		}

		else{
			for (auto i = root->child.begin(); i != root->child.end(); ++i){
				tempNode = *i;
				tempNode = insert(tempNode, hashval, node);
			}

		}
		return root;
	}
}

void Node::Inorder(Node* root){
	Node* tempNode;
	if (!root)
		return;
	else
	{
		for (auto i = root->child.begin(); i != root->child.end(); ++i){
			tempNode = *i;
			Inorder(tempNode);
			cout << tempNode->key <<"->" <<tempNode->data.end.latitudeText<<":"<<tempNode->data.end.longitudeText<< endl;
		}
		
	}

}
unsigned int hash(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
}

bool hasPath(Node *root, vector<GeoCoord>& arr, std::size_t x)
{
	Node* tempnode;
	bool _flag = false;
	if (!root)
		return false;
	arr.push_back(root->data.end);
	if (root->key == x){
		return true;
	}
	else{
		for (auto i = root->child.begin(); i != root->child.end(); ++i){
			tempnode = *i;
			_flag |= hasPath(tempnode, arr, x);
		}
		if(_flag)
			return _flag;
	}
	arr.pop_back();
	return false;
}

// function to print the path from root to the 
// given node if the node lies in the binary tree 
void printPath(Node *root, std::size_t x)
{
	vector<GeoCoord> arr;
	GeoCoord temp;
	if (hasPath(root, arr, x))
	{
		cout << "yes" << endl;
		for (auto i = arr.begin(); i != arr.end(); ++i){
			temp = *i;
			cout << temp.latitudeText << ":" << temp.longitudeText << "->";
		}
	}

	// 'x' is not present in the binary tree  
	else
		cout << "No Path";
}


class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
};

StreetMapImpl::StreetMapImpl()
{
}

StreetMapImpl::~StreetMapImpl()
{
}

bool StreetMapImpl::load(string mapFile)
{
	Node node, *root = NULL;
	root = node.insert(root, 1, new Node());
	string line;
	ifstream myfile(mapFile);
	bool _flag_address = true;
	bool _flag_line_seg_num = false;
	bool _flag_data = false;
	
	
	vector<StreetSegment> streetseg;
	
	
	if (myfile.is_open())
	{
		
		int _num_line_seg = 0;
		StreetSegment temp;
		
		while (getline(myfile, line))
		{
			if (_flag_address == true){
				
				temp.name = line;
				_flag_address = false;
				_flag_line_seg_num = true;
				_flag_data = false;
			}
			else if (_flag_line_seg_num == true)
			{
				_num_line_seg = std::stoi(line);
				_flag_data = true;
				_flag_line_seg_num = false;
			}
			else if(_flag_data == true)
			{
				--_num_line_seg;
				int geoindex = 0;
				string geovalue[4] = { "" };
				char* cstrptr = (char *)line.c_str();
				char* context = NULL;
				char *token = strtok_s(cstrptr, " ", &context);
				while (token != NULL) {
					geovalue[geoindex] = token;
					geoindex++;
					token = strtok_s(NULL, " ", &context);
				}
				
				temp.start = GeoCoord(geovalue[0],geovalue[1]);
				
				temp.end = GeoCoord(geovalue[2], geovalue[3]);
				streetseg.push_back(temp);
				
				delete[] token;
				if (_num_line_seg == 0){
					temp.start = GeoCoord();
					temp.end = GeoCoord(streetseg.front().start.latitudeText, streetseg.front().start.longitudeText);
					node.insert(root, 1, new Node(std::hash<string>()(temp.end.latitudeText + temp.end.longitudeText), temp));

					for (auto i = streetseg.begin(); i != streetseg.end(); ++i){
						temp = *i;
						node.insert(root, std::hash<string>()(temp.start.latitudeText + temp.start.longitudeText), new Node(std::hash<string>()(temp.end.latitudeText + temp.end.longitudeText), temp));
					}
					_flag_address = true;
					streetseg.clear();
				}
			}
		}
		//node.Inorder(root);
		GeoCoord tempGeo = GeoCoord("34.0290818", "-118.4859930");
		std::size_t point = std::hash<string>()(tempGeo.latitudeText + tempGeo.longitudeText);
		
		printPath(root, point);
		myfile.close();
		return true;
	}
	else {
		cout << "Unable to open file";
		return false;  // Delete this line and implement this function correctly
	}
}


bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
	
    return false;  // Delete this line and implement this function correctly
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
