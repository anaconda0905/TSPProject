#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <queue>
using namespace std;

class Node{
	
	int key;
	vector<Node *>child;
public:
	Node();
	Node(int);
	Node*  insert(Node*, int, Node*);
	void Inorder(Node*);
};

Node::Node(){
	key = 0;
}
Node::Node(int data){
	key = data;
}

Node* Node::insert(Node* root, int data, Node* node){
	Node* tempNode;
	if (!root){
		return new Node(data);
	}
	else{
		if (root->key == data){
			root->child.push_back(node);
		}

		else{
			for (auto i = root->child.begin(); i != root->child.end(); ++i){
				tempNode = *i;
				tempNode = insert(tempNode, data, node);
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
			cout << tempNode->key << endl;
		}
		
	}

}
unsigned int hash(const GeoCoord& g)
{
    return std::hash<string>()(g.latitudeText + g.longitudeText);
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
	Node b, *root = NULL;
	root = b.insert(root,30, new Node(0));
	b.insert(root, 30, new Node(26));
	b.insert(root, 30, new Node(27));
	b.insert(root, 27, new Node(40));
	b.insert(root, 27, new Node(41));
	
	b.insert(root, 30, new Node(28));
	b.insert(root, 28, new Node(25));
	b.insert(root, 25, new Node(24));
	b.insert(root, 24, new Node(26));
	b.insert(root, 26, new Node(31));
	b.Inorder(root);

	string line;
	ifstream myfile(mapFile);
	bool _flag_address = true;
	bool _flag_line_seg_num = false;
	bool _flag_data = false;
	
	vector<StreetSegment> streetseg;
	int a = 41;
	
	if (myfile.is_open())
	{
		int _num_line_seg = 0;
		StreetSegment temp;
		while (getline(myfile, line) && a>0)
		{
			--a;
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
					cout << "Output of begin and end: ";
					for (auto i = streetseg.begin(); i != streetseg.end(); ++i){
						temp = *i;
						cout << temp.start.latitude << ":" << std::hash<string>()(temp.start.latitudeText + temp.start.longitudeText) << " -> ";
					}
					cout << endl;


					_flag_address = true;
					streetseg.clear();
				}
			}
			//cout << line << '\n';
		}

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
