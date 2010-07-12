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
*/

#ifndef __TOPLEVEL_TREE_H__
#define __TOPLEVEL_TREE_H__

#include <iostream>
#include "cqueue.h"

// forward declaration
class ConcatenableQueue;

struct point
{
	double x_coord, y_coord;
};

struct tree_node
{
	point p, bridge1_lc, bridge2_lc, bridge1_rc, bridge2_rc; //only leaf nodes have a point stored in them
	float label;//it is the y-coord of the point stored rightmost leaf of the subtree rooted on the left child 
	ConcatenableQueue Ql, Qr;
	tree_node *leftchild, *rightchild, *parent;
	int balance_factor;//, type;  //type=0 => leafe, type=1 => label
	//bool leftmost;
};

class Tree
{
public:
	Tree();
	void add_node(const point &newPoint);
	void delete_node(const point &pointToDelete);
	tree_node* search_node(const point &pointToSearch);
	void print();
	tree_node* root() const;
	void set_show(const bool &show);

private:
	tree_node *root_;
	bool show_;

	tree_node *create_label(double, tree_node*, tree_node*);
	tree_node *create_object(point);
	void clockwise_anticlockDoubleRotation(tree_node*);
	void anticlockwise_clockDoubleRotation(tree_node*);
	void clockwiseRotation(tree_node*);
	void anticlockwiseRotation(tree_node*);
	void updateBalanceFactor(tree_node*, bool);
	void updateBalanceFactor_delete(tree_node*, bool, bool, bool, double);
	void printValues(tree_node*, int);
	void updateBridge_Hull(tree_node*, int);
	void updateHull(tree_node*);
	void buildChildrensHulls(tree_node*);
	void get_points_righthalf(node*, point*, point*, point*, int*); 
	void get_points_lefthalf(node*, point*, point*, point*, int*);
	int right_turn(point, point, point);
	double case9(point, point, point, point, point, point);
	int bridge_cases_lcHull(point, point, point, point, point, point, double, int, int);
	int bridge_cases_rcHull(point, point, point, point, point, point, double, int, int);
	node *newSS_lh_left(node*, point*, point*, point*, bool*);
	node *newSS_uh_right(node*, point*, point*, point*, bool*);
	node *newSS_lh_right(node*, point*, point*, point*, bool*, bool*);
	node *newSS_uh_left(node*, point*, point*, point*, bool*, bool*);
};


#endif /*__TOPLEVEL_TREE_H__*/
