/* $Id$
 * 
 * Copyright (C) 2007 Jose Alberto Cisneros Perez. All rights reserved.
 *
 * This file may be used under the terms of the GNU General Public License
 * versions 3.0 as published by the Free Software Foundation and
 * appearing in the COPYING file included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 *  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  This file implements a concatenable queue where the information is 
 *  stored in the leaves of a 2-3 Tree.
*/

#ifndef __CQUEUE_H__
#define __CQUEUE_H__


#include <iostream>

using namespace std;

struct node 
{
	int type; // type 0 is for leaf, 1 is for 2node, 2 is for 3node
	double data1, data2, x_coord; // on leaf nodes, data2 is the y-coordinate of the point 
	node *left, *middle, *right, *parent, *biggest_y, *smallest_y;//
}; 

class ConcatenableQueue 
{
public:
	node *root;
	ConcatenableQueue();
	ConcatenableQueue(double, double);
	void add_node(double, double);
	void delete_node(double);
	bool is_empty();
	void print();
	void search(double);
	int height();
	void addChild(node*, node*, double);
	node *rightmostNodeAtLevel(int, node*);
	node *leftmostNodeAtLevel(int, node*);	
private:
	node *create_LeafNode(double, double);
	node *create_2Node(double, double, node*, node*);
	node *searchForInsert(node*, node*);
	node *searchLeafNode(double, node*);
	void deleteNode(node*);
	void updateValuesInsert(node*, double);
	void updateValuesDelete(node*, double, double);
	void printValues(node*, int);
};

//Procedures concatenate and split concatenable queues
ConcatenableQueue concatenate(ConcatenableQueue CQ1, ConcatenableQueue CQ2);

//split_left, CQ1 keeps the node with value val 
void split_left (ConcatenableQueue CQ, ConcatenableQueue *CQ1, ConcatenableQueue *CQ2, double val);

//split_right, CQ2 keeps the node with value val 
void split_right (ConcatenableQueue CQ, ConcatenableQueue *CQ1, ConcatenableQueue *CQ2, double val);

#endif /*__CQUEUE_H__*/
