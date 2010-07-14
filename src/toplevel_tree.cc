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

#include "cqueue.h"
#include "toplevel_tree.h"


Tree::Tree() : root_(NULL), show_(false)
{
}

void Tree::updateHull(tree_node *current_node)
{
	ConcatenableQueue upperLcHull, upperRcHull, lowerLcHull, lowerRcHull, completeLcHull, completeRcHull, aux1, aux2, aux3, aux4;
	updateBridgeHull(current_node, 0);
	updateBridgeHull(current_node, 1);
	split_left(current_node->leftchild->Ql,&lowerLcHull, &aux1, current_node->bridge1_lc.y_coord);
	split_left(current_node->leftchild->Qr,&lowerRcHull, &aux3, current_node->bridge1_rc.y_coord);
	current_node->leftchild->Ql = aux1;
	current_node->leftchild->Qr = aux3;
	split_right(current_node->rightchild->Ql, &aux2, &upperLcHull, current_node->bridge2_lc.y_coord);
	split_right(current_node->rightchild->Qr, &aux4, &upperRcHull, current_node->bridge2_rc.y_coord);
	current_node->rightchild->Ql = aux2;
	current_node->rightchild->Qr = aux4;
	completeLcHull = concatenate(lowerLcHull,upperLcHull);
	completeRcHull = concatenate(lowerRcHull,upperRcHull);
	current_node->Ql = completeLcHull;   
	current_node->Qr = completeRcHull;
}

//bridge lc hull operations

void Tree::getPointsRightHalf(node *v_node, point &p, point &p0, point &p1, int &flag)
{
	if ( v_node->left->type != LEAF_NODE )
	{
		flag = 0;
		p.x_coord = v_node->right->smallest_y->x_coord;
		p.y_coord = v_node->right->smallest_y->data2;
		if ( v_node->type == THREE_NODE )
		{
			p0.x_coord = v_node->middle->biggest_y->x_coord;
			p0.y_coord = v_node->middle->biggest_y->data2;
		}
		else
		{
			p0.x_coord = v_node->left->biggest_y->x_coord;
			p0.y_coord = v_node->left->biggest_y->data2;
		}
		if ( v_node->right->smallest_y->parent->type == THREE_NODE )
		{
			p1.x_coord = v_node->right->smallest_y->parent->middle->x_coord;
			p1.y_coord = v_node->right->smallest_y->parent->middle->data2;
		}
		else
		{
		p1.x_coord = v_node->right->smallest_y->parent->right->x_coord;
		p1.y_coord = v_node->right->smallest_y->parent->right->data2;
		}
	}
	else
	{
		p0.x_coord = v_node->left->x_coord;
		p0.y_coord = v_node->left->data2;
		if ( v_node->type == THREE_NODE )
		{
			p.x_coord = v_node->middle->x_coord;
			p.y_coord = v_node->middle->data2;
			p1.x_coord = v_node->right->x_coord;
			p1.y_coord = v_node->right->data2;
			flag = 0;
		}
		else
		{
			p.x_coord = v_node->right->x_coord;
			p.y_coord = v_node->right->data2;
			if ( v_node->parent == NULL )
				flag = 1;
			else
			{
				if ( v_node->parent->right == v_node )
					flag = 1;
				else if ( (v_node->parent->middle == v_node) || (v_node->parent->type == TWO_NODE) )
				{
					p1.x_coord = v_node->parent->right->smallest_y->x_coord;
					p1.y_coord = v_node->parent->right->smallest_y->data2;
					flag = 0;
				}
				else
				{
					p1.x_coord = v_node->parent->middle->smallest_y->x_coord;
					p1.y_coord = v_node->parent->middle->smallest_y->data2;
					flag = 0;
				}
			}
		}
	}
}

void Tree::getPointsLeftHalf(node *v_node, point &q, point &q0, point &q1, int &flag)
{
	if ( v_node->left->type != LEAF_NODE )
	{
		flag = 0;
		q.x_coord = v_node->left->biggest_y->x_coord;
		q.y_coord = v_node->left->biggest_y->data2;
		if ( v_node->type == THREE_NODE )
		{
			q1.x_coord = v_node->middle->smallest_y->x_coord;
			q1.y_coord = v_node->middle->smallest_y->data2;
		}
		else
		{
			q1.x_coord = v_node->right->smallest_y->x_coord;
			q1.y_coord = v_node->right->smallest_y->data2;
		}
		if ( v_node->left->biggest_y->parent->type == THREE_NODE )
		{
			q0.x_coord = v_node->left->biggest_y->parent->middle->x_coord;
			q0.y_coord = v_node->left->biggest_y->parent->middle->data2;
		}
		else
		{
			q0.x_coord = v_node->left->biggest_y->parent->left->x_coord;
			q0.y_coord = v_node->left->biggest_y->parent->left->data2;
		}
	}
	else
	{
		q1.x_coord = v_node->right->x_coord;
		q1.y_coord = v_node->right->data2;
		if ( v_node->type == THREE_NODE )
		{
			q.x_coord = v_node->middle->x_coord;
			q.y_coord = v_node->middle->data2;
			q0.x_coord = v_node->left->x_coord;
			q0.y_coord = v_node->left->data2;
			flag = 0;
		}
		else
		{
			q.x_coord = v_node->left->x_coord;
			q.y_coord = v_node->left->data2;
			if ( v_node->parent == NULL )
				flag = 1;
			else
			{
				if ( v_node->parent->left == v_node )
					flag = 1;
				else if ( (v_node->parent->middle == v_node) || (v_node->parent->type == TWO_NODE) )
				{
					q0.x_coord = v_node->parent->left->biggest_y->x_coord;
					q0.y_coord = v_node->parent->left->biggest_y->data2;
					flag = 0;
				}
				else
				{
					q0.x_coord = v_node->parent->middle->biggest_y->x_coord;
					q0.y_coord = v_node->parent->middle->biggest_y->data2;
					flag = 0;
				}
			}
		}
	}
}

int Tree::rightTurn(const point &p0, const point &p1, const point &p2) const
{
	double cross_prod;
	cross_prod = ((p2.x_coord - p0.x_coord)*(p1.y_coord - p0.y_coord))-((p2.y_coord - p0.y_coord)*(p1.x_coord - p0.x_coord));
	if ( cross_prod == 0 )
		return 2; //no turn (colinear points)
	else if ( cross_prod < 0 )
		return 0;  // turn left
	else
		return 1;  //turn right
}

double Tree::case9(const point &p, const point &p0, const point &p1,
				const point &q, const point &q0, const point &q1) const
{
	double m1, m2, mp, mq, bp, bq;
	m2 = (p.y_coord-p1.y_coord)/(p.x_coord-p1.x_coord);
	if ( p0.x_coord != p.x_coord )
	{
		m1 = (p0.y_coord-p.y_coord)/(p0.x_coord-p.x_coord);
		if ( m1 > 0 )
			mp = m1 - ((m1-m2)/2);
		else
			mp = m1 + ((m2-m1)/2);
	}
	else
		if ( m2 > 0 )
			mp = m2 + 0.05;
		else
			mp = m2 - 0.05;
	m1 = (q0.y_coord-q.y_coord)/(q0.x_coord-q.x_coord);
	if ( q.x_coord != q1.x_coord )
	{
		m2 = (q.y_coord-q1.y_coord)/(q.x_coord-q1.x_coord);
		if ( m2 < 0 )
			mq = m1 - ((m1-m2)/2);
		else
			mq = m1 + ((m2-m1)/2);
	}
	else
		if ( m2 < 0 )
			mq = m1 - 0.05;
		else
			mq = m1 + 0.05;
	bp = p.y_coord - (mp*p.x_coord);
	bq = q.y_coord - (mq*q.x_coord);
	return ((mq*bp)-(mp*bq))/(mq-mp);
}

int Tree::bridgeCasesLcHull(const point &p, const point &p0, const point &p1,
					const point &q, const point &q0, const point &q1,
					const double &half, const int &flag_lh, const int &flag_uh) const
{
	int chck_p0, chck_p1, chck_q0, chck_q1;
	double intersec;
	if ( flag_lh == 0 )
	{
		chck_q0 = rightTurn(p,q,q0);
		chck_q1 = rightTurn(p,q,q1);
	}
	else if ( flag_lh == 1 )
	{
		chck_q0 = 0;
		chck_q1 = rightTurn(p,q,q1);
	}
	else
	{
		chck_q0 = 0;
		chck_q1 = 0;
	}
	if ( flag_uh == 0 )
	{
		chck_p0 = rightTurn(q,p,p0);
		chck_p1 = rightTurn(q,p,p1);
	}
	else if ( flag_uh == 1 )
	{
		chck_p1 = 1;
		chck_p0 = rightTurn(q,p,p0);
	}
	else
	{
		chck_p0 = 1;
		chck_p1 = 1;
	}
	if ( ((chck_p1 == 1) || (chck_p1 == 2)) && (chck_p0 == 1) && (chck_q1 == 0) && ((chck_q0 == 0) || (chck_q0 == 2)) )
		return 1;
	else if ( ((chck_p1 == 1) || (chck_p1 == 2)) && (chck_p0 == 1) && (chck_q1 == 0) && (chck_q0 == 1) )
		return 2;
	else if ( ((chck_p1 == 1) || (chck_p1 == 2)) && (chck_p0 == 1) && ((chck_q1 == 1) || (chck_q1 == 2)) && ((chck_q0 == 0) || (chck_q0 == 2)) )
		return 3;
	else if ( (chck_p1 == 0) && (chck_p0 == 1) && (chck_q1 == 0) && ((chck_q0 == 0) || (chck_q0 == 2)) )
		return 4;
	else if ( ((chck_p1 == 1) || (chck_p1 == 2)) && ((chck_p0 == 0) || (chck_p0 == 2)) && (chck_q1 == 0) && ((chck_q0 == 0) || (chck_q0 == 2)) )
		return 5;
	else if ( (chck_p1 == 0) && (chck_p0 == 1) && (chck_q1 == 0) && (chck_q0 == 1) )
		return 6;
	else if ( (chck_p1 == 1) && (chck_p0 == 0) && (chck_q1 == 0) && (chck_q0 == 1) )
		return 7;
	else if ( (chck_p1 == 0) && (chck_p0 == 1) && (chck_q1 == 1) && (chck_q0 == 0) )
		return 8;
	else if ( (chck_p1 == 1) && (chck_p0 == 0) && (chck_q1 == 1) && (chck_q0 == 0) )
	{
		intersec = case9(p,p0,p1,q,q0,q1);
		if (intersec < half)
			return 9;
		else 
			return 10;
	}
}

int Tree::bridgeCasesRcHull(const point &p, const point &p0, const point &p1,
					const point &q, const point &q0, const point &q1,
					const double &half, const int &flag_lh, const int &flag_uh) const
{
	int chck_p0, chck_p1, chck_q0, chck_q1;
	double intersec;
	if ( flag_lh == 0 )
	{
		chck_q0 = rightTurn(p,q,q0);
		chck_q1 = rightTurn(p,q,q1);
	}
	else if ( flag_lh == 1 )
	{
		chck_q0 = 1;
		chck_q1 = rightTurn(p,q,q1);
	}
	else
	{
		chck_q0 = 1;
		chck_q1 = 1;
	}
	if ( flag_uh == 0 )
	{
		chck_p0 = rightTurn(q,p,p0);
		chck_p1 = rightTurn(q,p,p1);
	}
	else if ( flag_uh == 1 )
	{
		chck_p1 = 0;
		chck_p0 = rightTurn(q,p,p0);
	}
	else
	{
		chck_p0 = 0;
		chck_p1 = 0;
	}
	if ( ((chck_p1 == 0) || (chck_p1 == 2)) && (chck_p0 == 0) && (chck_q1 == 1) && ((chck_q0 == 1) || (chck_q0 == 2)) )
		return 1;
	else if ( ((chck_p1 == 0) || (chck_p1 == 2)) && (chck_p0 == 0) && (chck_q1 == 1) && (chck_q0 == 0) )
		return 2;
	else if ( ((chck_p1 == 0) || (chck_p1 == 2)) && (chck_p0 == 0) && ((chck_q1 == 0) || (chck_q1 == 2)) && ((chck_q0 == 1) || (chck_q0 == 2)) )
		return 3;
	else if ( (chck_p1 == 1) && (chck_p0 == 0) && (chck_q1 == 1) && ((chck_q0 == 1) || (chck_q0 == 2)) )
		return 4;
	else if ( ((chck_p1 == 0) || (chck_p1 == 2)) && ((chck_p0 == 1) || (chck_p0 == 2)) && (chck_q1 == 1) && ((chck_q0 == 1) || (chck_q0 == 2)) )
		return 5;
	else if ( (chck_p1 == 1) && (chck_p0 == 0) && (chck_q1 == 1) && (chck_q0 == 0) )
		return 6;
	else if ( (chck_p1 == 0) && (chck_p0 == 1) && (chck_q1 == 1) && (chck_q0 == 0) )
		return 7;
	else if ( (chck_p1 == 1) && (chck_p0 == 0) && (chck_q1 == 0) && (chck_q0 == 1) )
		return 8;
	else if ( (chck_p1 == 0) && (chck_p0 == 1) && (chck_q1 == 0) && (chck_q0 == 1) )
	{
		intersec = case9(p,p0,p1,q,q0,q1);
		if (intersec < half)
			return 9;
		else 
			return 10;
	}
}

node *Tree::newSSLhLeft(node *current, point &q, point &q0, point &q1, bool &onepoint)
{
	if ( current->type == LEAF_NODE )
	{
		if ( !onepoint )
		{
			onepoint = true;
			if ( current->parent != NULL )
			{
				if ( current->parent->right == current )
				{
					q1.y_coord = q.y_coord;
					q1.x_coord = q.x_coord;
					if ( current->parent->type == THREE_NODE )
					{
						q.y_coord = current->parent->middle->data2;
						q.x_coord = current->parent->middle->x_coord;
					}
					else
					{
						q.y_coord = current->parent->left->data2;
						q.x_coord = current->parent->left->x_coord;
					}
					onepoint = false;
				}
			}
		}
		return NULL;
	}
	else if ( current->left->type == LEAF_NODE )
	{
		if ( current->type == THREE_NODE )
		{
			q1 = q;
			onepoint = false;
		}
		else
			onepoint = true;
		q.x_coord = current->left->x_coord;
		q.y_coord = current->left->data2;
		return current->left;
	}
	else if ( q.y_coord == current->left->biggest_y->data2 )
		return current->left;
	else if ( q.y_coord == current->right->smallest_y->data2 )
	{
		q1 = q;
		q = q0;
		onepoint = false;
		return current->right->smallest_y;
	}
	else
	{
		if ( q.y_coord == current->middle->smallest_y->data2 )
		{
			q1 = q;
			q = q0;
			onepoint = false;
			return current->right->smallest_y;
		}
		else
			return current->middle;
	}
}

node *Tree::newSSUhRight(node *current, point &p, point &p0, point &p1, bool &onepoint)
{
	if ( current->type == LEAF_NODE )
	{
		if ( !onepoint )
		{
			onepoint = true;
			if ( current->parent != NULL )
			{
				if ( current->parent->left == current )
				{
					p0 = p;
					if ( current->parent->type == THREE_NODE )
					{
						p.y_coord = current->parent->middle->data2;
						p.x_coord = current->parent->middle->x_coord;
					}
					else
					{
						p.y_coord = current->parent->right->data2;
						p.x_coord = current->parent->right->x_coord;
					}
					onepoint = false;
				}
			}
		}
		return NULL;
	}
	else if ( current->right->type == LEAF_NODE )
	{
		if ( current->type == THREE_NODE )
		{
			p0 = p;
			onepoint = false;
		}
		else
		{
			onepoint = true;
		}
		p.x_coord = current->right->x_coord;
		p.y_coord = current->right->data2;
		return current->right;
	}
	else if ( p.y_coord == current->right->smallest_y->data2 )
		return current->right;
	else if ( p.y_coord == current->left->biggest_y->data2 )
	{
		p0 = p;
		p = p1;
		onepoint = false;
		return current->left->biggest_y;
	}
	else
	{
		if ( p.y_coord == current->middle->biggest_y->data2 )
		{
			p0 = p;
			p = p1;
			onepoint = false;
			return current->middle->biggest_y;
		}
		else
			return current->middle;
	}
}

node *Tree::newSSLhRight(node *current, point &q, point &q0, point &q1, bool &onepoint, bool &out)
{
	node *newcurrent = NULL;
	if ( current->type == LEAF_NODE )
	{
		out = true;
	}
	else if ( current->left->type == LEAF_NODE )
	{
		onepoint = false;
		out = true;
		newcurrent = current->right;
	}
	else if ( q.y_coord == current->right->smallest_y->data2 )
	{
		newcurrent = current->right;
		out = true;
	}
	else if ( current->type == THREE_NODE )
	{
		point aux = q1;
		if ( q.y_coord == current->left->biggest_y->data2 )
		{
			if ( current->middle->smallest_y->parent->type == THREE_NODE )
			{
				q1.x_coord = current->middle->smallest_y->parent->middle->x_coord;
				q1.y_coord = current->middle->smallest_y->parent->middle->data2;
			}
			else
			{
				q1.x_coord = current->middle->smallest_y->parent->right->x_coord;
				q1.y_coord = current->middle->smallest_y->parent->right->data2;
			}
			q0 = q;
			q = aux;
		}
		else if ( q.y_coord == current->middle->smallest_y->data2 )
		{
			q1.x_coord = current->right->smallest_y->x_coord;
			q1.y_coord = current->right->smallest_y->data2;
			q.x_coord = current->middle->biggest_y->x_coord;
			q.y_coord = current->middle->biggest_y->data2;
			if ( current->middle->biggest_y->parent->type == THREE_NODE )
			{
				q0.x_coord = current->middle->biggest_y->parent->middle->x_coord;
				q0.y_coord = current->middle->biggest_y->parent->middle->data2;
			}
			else
			{
				q0.x_coord = current->middle->biggest_y->parent->left->x_coord;
				q0.y_coord = current->middle->biggest_y->parent->left->data2;
			}
		}
		else
		{
			if ( current->right->smallest_y->parent->type == THREE_NODE )
			{
				q1.x_coord = current->right->smallest_y->parent->middle->x_coord;
				q1.y_coord = current->right->smallest_y->parent->middle->data2;
			}
			else
			{
				q1.x_coord = current->right->smallest_y->parent->right->x_coord;
				q1.y_coord = current->right->smallest_y->parent->right->data2;
			}
			q0 = q;
			q = aux;
		}
	}
	else
	{
		point aux = q1;
		if ( current->right->smallest_y->parent->type == THREE_NODE )
		{
			q1.x_coord = current->right->smallest_y->parent->middle->x_coord;
			q1.y_coord = current->right->smallest_y->parent->middle->data2;
		}
		else
		{
			q1.x_coord = current->right->smallest_y->parent->right->x_coord;
			q1.y_coord = current->right->smallest_y->parent->right->data2;
		}
		q0 = q;
		q = aux;
	}
	return newcurrent;
}

node *Tree::newSSUhLeft(node *current, point &p, point &p0, point &p1, bool &onepoint, bool &out)
{
	node *newcurrent;
	newcurrent = NULL;
	if ( current->type == LEAF_NODE )
		out = true;
	else if ( current->left->type == LEAF_NODE )
	{
		onepoint = false;
		out = true;
		newcurrent = current->left;
	}
	else if ( p.y_coord == current->left->biggest_y->data2 )
	{
		newcurrent = current->left;
		out = true;
	}
	else if ( current->type == THREE_NODE )
	{
		point aux = p0;
		if ( p.y_coord == current->right->smallest_y->data2 )
		{
			if ( current->middle->biggest_y->parent->type == THREE_NODE )
			{
				p0.x_coord = current->middle->biggest_y->parent->middle->x_coord;
				p0.y_coord = current->middle->biggest_y->parent->middle->data2;
			}
			else
			{
				p0.x_coord = current->middle->biggest_y->parent->left->x_coord;
				p0.y_coord = current->middle->biggest_y->parent->left->data2;
			}

			p1 = p;
			p = aux;
		}
		else if ( p.y_coord == current->middle->biggest_y->data2 )
		{
			p0.x_coord = current->left->biggest_y->x_coord;
			p0.y_coord = current->left->biggest_y->data2;
			p.x_coord = current->middle->smallest_y->x_coord;
			p.y_coord = current->middle->smallest_y->data2;
			if ( current->middle->smallest_y->parent->type == THREE_NODE )
			{
				p1.x_coord = current->middle->smallest_y->parent->middle->x_coord;
				p1.y_coord = current->middle->smallest_y->parent->middle->data2;
			}
			else
			{
				p1.x_coord = current->middle->smallest_y->parent->right->x_coord;
				p1.y_coord = current->middle->smallest_y->parent->right->data2;
			}
		}
		else
		{
			if ( current->left->biggest_y->parent->type == THREE_NODE )
			{
				p0.x_coord = current->left->biggest_y->parent->middle->x_coord;
				p0.y_coord = current->left->biggest_y->parent->middle->data2;
			}
			else
			{
				p0.x_coord = current->left->biggest_y->parent->left->x_coord;
				p0.y_coord = current->left->biggest_y->parent->left->data2;
			}
			p1 = p;
			p = aux;
		}
	}
	else
	{
		point aux = p0;
		if ( current->left->smallest_y->parent->type == THREE_NODE )
		{
			p0.x_coord = current->left->biggest_y->parent->middle->x_coord;
			p0.y_coord = current->left->biggest_y->parent->middle->data2;
		}
		else
		{
			p0.x_coord = current->left->smallest_y->parent->left->x_coord;
			p0.y_coord = current->left->smallest_y->parent->left->data2;
		}
		p1 = p;
		p = aux;
	}
	return newcurrent;
}


void Tree::buildChildrensHulls(tree_node *parent_node)
{
	ConcatenableQueue upperLcHull, upperRcHull, lowerLcHull, lowerRcHull, completeUpperLcHull, completeUpperRcHull, completeLowerLcHull, completeLowerRcHull;
	split_right(parent_node->Ql,&lowerLcHull, &upperLcHull, parent_node->bridge2_lc.y_coord);
	split_right(parent_node->Qr,&lowerRcHull, &upperRcHull, parent_node->bridge2_rc.y_coord);
	completeUpperLcHull = concatenate(parent_node->rightchild->Ql,upperLcHull);
	completeUpperRcHull = concatenate(parent_node->rightchild->Qr,upperRcHull);
	completeLowerLcHull = concatenate(lowerLcHull, parent_node->leftchild->Ql);
	completeLowerRcHull = concatenate(lowerRcHull, parent_node->leftchild->Qr);
	parent_node->rightchild->Ql = completeUpperLcHull;
	parent_node->leftchild->Ql = completeLowerLcHull; 
	parent_node->rightchild->Qr = completeUpperRcHull;
	parent_node->leftchild->Qr = completeLowerRcHull;
}

void Tree::updateBridgeHull(tree_node *v_node, const int &hull)
{
	bool bridgefound, onepoint_lh, onepoint_uh, out;
	double half;
	int chk_case, flag_lh, flag_uh;
	point p, p0, p1, q, q0, q1;
	node *current_lh, *current_uh, *aux;
	bridgefound = false;
	onepoint_lh = true;
	onepoint_uh = true;
	if ( hull == 0 )
	{
		current_lh = v_node->leftchild->Ql.root();
		current_uh = v_node->rightchild->Ql.root();
	}
	else
	{
		current_lh = v_node->leftchild->Qr.root();
		current_uh = v_node->rightchild->Qr.root();
	}
	if ( (current_lh->type != LEAF_NODE) && (current_uh->type != LEAF_NODE) )
		half = current_uh->smallest_y->data2 - ((current_uh->smallest_y->data2 - current_lh->biggest_y->data2)/2);
	else if ( current_lh->type != LEAF_NODE )
		half = current_uh->data2 - ((current_uh->data2 - current_lh->biggest_y->data2)/2);
	else if ( current_uh->type != LEAF_NODE )
		half = current_uh->smallest_y->data2 - ((current_uh->smallest_y->data2 - current_lh->data2)/2);
	else
		half = current_uh->data2 - ((current_uh->data2 - current_lh->data2)/2);
	p.x_coord = current_uh->x_coord;
	p.y_coord = current_uh->data2;
	q.x_coord = current_lh->x_coord;
	q.y_coord = current_lh->data2;
	while ( !bridgefound )
	{
		if ( (current_lh->type == LEAF_NODE) && (current_uh->type == LEAF_NODE) )
		{
			if ( (onepoint_lh) && (onepoint_uh) )
				bridgefound = true;
			else if ( onepoint_lh )
			{
				if ( hull == 0 )
					chk_case = bridgeCasesLcHull(p,p0,p1,q,q0,q1,half,2,1);
				else
					chk_case = bridgeCasesRcHull(p,p0,p1,q,q0,q1,half,2,1);
				if ( chk_case != 1 )
					p = p0;
				bridgefound = true;
			}
			else if ( onepoint_uh )
			{
				if ( hull == 0 )
					chk_case = bridgeCasesLcHull(p,p0,p1,q,q0,q1,half,1,2);
				else
					chk_case = bridgeCasesRcHull(p,p0,p1,q,q0,q1,half,1,2);
				if ( chk_case != 1 )
					q = q1;
				bridgefound = true;
			}
			else
			{
				if ( hull == 0 )
					chk_case = bridgeCasesLcHull(p,p0,p1,q,q0,q1,half,1,1);
				else
					chk_case = bridgeCasesRcHull(p,p0,p1,q,q0,q1,half,1,1);
				if ( chk_case != 1 )
				{
					if ( hull == 0 )
						chk_case = bridgeCasesLcHull(p0,p,p1,q,q0,q1,half,1,2);
					else
						chk_case = bridgeCasesRcHull(p0,p,p1,q,q0,q1,half,1,2);
					if ( chk_case != 1 )
					{
						if ( hull == 0 )
							chk_case = bridgeCasesLcHull(p,p0,p1,q1,q0,q,half,2,1);
						else
							chk_case = bridgeCasesRcHull(p,p0,p1,q1,q0,q,half,2,1);
						if ( chk_case != 1 )
						{
							p = p0;
							q = q1;
						}
						else
							q = q1;
					}
					else
						p = p0;
				}
				bridgefound = true; 
			}
		}
		else
		{
			if ( current_uh->type == LEAF_NODE )
			{
				getPointsLeftHalf(current_lh, q, q0, q1, flag_lh);
				if ( onepoint_uh )
					flag_uh = 2;
				else
					flag_uh = 1;
			}
			else if ( current_lh->type == LEAF_NODE )
			{
				getPointsRightHalf(current_uh, p, p0, p1, flag_uh);
				if ( onepoint_lh )
					flag_lh = 2;
				else
					flag_lh = 1;
			}   
			else
			{
				getPointsLeftHalf(current_lh, q, q0, q1, flag_lh);
				getPointsRightHalf(current_uh, p, p0, p1, flag_uh);
			}
			out = false;
			while (!out)
			{
				if ( hull == 0 )
					chk_case = bridgeCasesLcHull(p,p0,p1,q,q0,q1,half,flag_lh,flag_uh);
				else
					chk_case = bridgeCasesRcHull(p,p0,p1,q,q0,q1,half,flag_lh,flag_uh);
				switch (chk_case)
				{
					case 1:
						bridgefound = true;
						out = true;
						break;
					case 2:
					case 4:
					case 6:
						aux = newSSLhLeft(current_lh, q, q0, q1, onepoint_lh);
						if ( aux != NULL )
							current_lh = aux;
						aux = newSSUhRight(current_uh, p, p0, p1, onepoint_uh);
						if ( aux != NULL )
							current_uh = aux;
						out = true;
						break;
					case 7:
						aux = newSSLhLeft(current_lh, q, q0, q1, onepoint_lh);
						if ( aux != NULL )
							current_lh = aux;
						out = true;
						break;
					case 8:
						aux = newSSUhRight(current_uh, p, p0, p1, onepoint_uh);
						if ( aux != NULL )
							current_uh = aux;
						out = true;
						break;
					case 3:
						aux = newSSLhRight(current_lh, q, q0, q1, onepoint_lh, out);
						if ( aux != NULL )
							current_lh = aux;
						if ( out )
						{
							aux = newSSUhRight(current_uh, p, p0, p1, onepoint_uh);
							if ( aux != NULL )
								current_uh = aux;
						}
						break;
					case 5:
						aux = newSSUhLeft(current_uh, p, p0, p1, onepoint_uh, out);
						if ( aux != NULL )
							current_uh = aux;
						if ( out )
						{
							aux = newSSLhLeft(current_lh, q, q0, q1, onepoint_lh);
							if ( aux != NULL )
								current_lh = aux;
						}
						break;
					case 9:
						aux = newSSLhRight(current_lh, q, q0, q1, onepoint_lh, out);
						if ( aux != NULL )
							current_lh = aux;
						if ( (current_lh->type == LEAF_NODE) && (current_uh->type != LEAF_NODE) )
						{
							out = false;
							flag_lh = 2;
							q.y_coord = q1.y_coord;
							q.x_coord = q1.x_coord;
						}
						break;
					case 10:
						aux = newSSUhLeft(current_uh, p, p0, p1, onepoint_uh, out);
						if ( aux != NULL )
							current_uh = aux;
						if ( (current_uh->type == LEAF_NODE) && (current_lh->type != LEAF_NODE) )
						{
							out = false;
							flag_uh = 2;
							p.y_coord = p0.y_coord;
							p.x_coord = p0.x_coord;
						}
						break;
				}
			}
		}
	}
	if ( hull == 0 )
	{
		v_node->bridge1_lc = q;
		v_node->bridge2_lc = p;
	}
	else
	{
		v_node->bridge1_rc = q;
		v_node->bridge2_rc = p;
	}
}

tree_node *Tree::createLabel(const double &new_label, tree_node *left_child, tree_node *right_child)
{
	tree_node *new_node = new tree_node;
	new_node->label = new_label;
	new_node->Ql = concatenate(left_child->Ql, right_child->Ql);
	new_node->Qr = concatenate(left_child->Qr, right_child->Qr);
	new_node->bridge1_lc = left_child->p;
	new_node->bridge2_lc = right_child->p;
	new_node->bridge1_rc = left_child->p;
	new_node->bridge2_rc = right_child->p;
	left_child->Ql.set_root(NULL);
	right_child->Ql.set_root(NULL);
	left_child->Qr.set_root(NULL);
	right_child->Qr.set_root(NULL);
	new_node->leftchild = NULL;
	new_node->rightchild = NULL;
	new_node->parent = NULL;
	new_node->balance_factor = 0;
	return new_node;
}

tree_node *Tree::createObject(const point &new_point)
{
	ConcatenableQueue lcHull(new_point.x_coord, new_point.y_coord);
	ConcatenableQueue rcHull(new_point.x_coord, new_point.y_coord);
	tree_node *new_node = new tree_node;
	new_node->p = new_point;
	new_node->label = new_point.y_coord;
	new_node->Ql = lcHull;
	new_node->Qr = rcHull;
	new_node->leftchild = NULL;
	new_node->rightchild = NULL;
	new_node->parent = NULL;
	new_node->balance_factor = 0;
	return new_node;
}

void Tree::anticlockwiseRotation(tree_node *z_node)
{
	tree_node *y_node;
	y_node = z_node->rightchild;
	z_node->rightchild = y_node->leftchild;
	y_node->leftchild->parent = z_node;
	y_node->leftchild = z_node;
	if ( z_node == root_ )
	{
		y_node->parent = NULL;
		root_ = y_node;
	}
	else if ( z_node == z_node->parent->leftchild )
	{
		z_node->parent->leftchild = y_node;
		y_node->parent = z_node->parent;
	}
	else
	{
		y_node->parent = z_node->parent;
		z_node->parent->rightchild = y_node;
	}
	z_node->parent = y_node;
	if ( y_node->balance_factor == 1 )
	{
		z_node->balance_factor = 0;
		y_node->balance_factor = 0;
	}
	else
	{
		z_node->balance_factor = 1;
		y_node->balance_factor = -1;
	}
}

void Tree::clockwiseRotation(tree_node *z_node)
{
	tree_node *y_node;
	y_node = z_node->leftchild;
	z_node->leftchild = y_node->rightchild;
	y_node->rightchild->parent = z_node;
	y_node->rightchild = z_node;
	if ( z_node == root_ )
	{
		y_node->parent = NULL;
		root_ = y_node;
	}
	else if ( z_node == z_node->parent->leftchild )
	{
		z_node->parent->leftchild = y_node;
		y_node->parent = z_node->parent;
	}
	else
	{
		z_node->parent->rightchild = y_node;
		y_node->parent = z_node->parent;
	}
	z_node->parent = y_node;
	if ( y_node->balance_factor == -1 )
	{
		z_node->balance_factor = 0;
		y_node->balance_factor = 0;
	}
	else
	{
		z_node->balance_factor = -1;
		y_node->balance_factor = 1;
	}
}

void Tree::clockwiseAnticlockDoubleRotation(tree_node *z_node)
{
	tree_node *y_node, *x_node;
	y_node = z_node->rightchild;
	x_node = y_node->leftchild;
	z_node->rightchild = x_node->leftchild;
	x_node->leftchild->parent = z_node;
	y_node->leftchild = x_node->rightchild;
	x_node->rightchild->parent = y_node;
	x_node->rightchild = y_node;
	y_node->parent = x_node;
	x_node->parent = z_node->parent;
	x_node->leftchild = z_node;
	if ( z_node == root_ )
		root_ = x_node;
	else if ( z_node->parent->leftchild == z_node )
		z_node->parent->leftchild = x_node;
	else
		z_node->parent->rightchild = x_node;
	z_node->parent = x_node;
	if ( x_node->balance_factor == 0 )
	{
		y_node->balance_factor = 0;
		z_node->balance_factor = 0;
	}
	else if ( x_node->balance_factor == 1 )
	{
		y_node->balance_factor = 0;
		z_node->balance_factor = -1;
	}
	else
	{
		y_node->balance_factor = 1;
		z_node->balance_factor = 0;
	}
	x_node->balance_factor = 0;
}

void Tree::anticlockwiseClockDoubleRotation(tree_node *z_node)
{
	tree_node *y_node, *x_node;
	y_node = z_node->leftchild;
	x_node = y_node->rightchild;
	z_node->leftchild = x_node->rightchild;
	x_node->rightchild->parent = z_node;
	y_node->rightchild = x_node->leftchild;
	x_node->leftchild->parent = y_node;
	x_node->leftchild = y_node;
	y_node->parent = x_node;
	x_node->parent = z_node->parent;
	x_node->rightchild = z_node;
	if ( z_node == root_ )
		root_ = x_node;
	else if ( z_node->parent->leftchild == z_node )
		z_node->parent->leftchild = x_node;
	else
		z_node->parent->rightchild = x_node;
	z_node->parent = x_node;
	if ( x_node->balance_factor == 0 )
	{
		y_node->balance_factor = 0;
		z_node->balance_factor = 0;
	}
	else if ( x_node->balance_factor == 1 )
	{
		y_node->balance_factor = -1;
		z_node->balance_factor = 0;
	}
	else
	{
		y_node->balance_factor = 0;
		z_node->balance_factor = 1;
	}
	x_node->balance_factor = 0;
}

void Tree::addNode(const point &new_point) 
{
	tree_node *new_node(NULL), *parent(NULL), *current(NULL), *new_label_node(NULL);
	bool right;
	int i;
	new_node = createObject(new_point);
	if ( root_ == NULL )
		root_ = new_node;
	else
	{
		current = root_;
		i = 1;
		while ( current->leftchild !=NULL )
		{
			buildChildrensHulls(current);
			if (show_)
			{
				std::cout << "The lc hull of the lower half at level " << i<< "\n";
				current->leftchild->Ql.print();
				std::cout << "\n";
				std::cout << "The rc hull of the lower half at level " << i<< "\n";
				current->leftchild->Qr.print();
				std::cout << "\n";
				std::cout << "The lc hull of the upper half at level " << i<< "\n";
				current->rightchild->Ql.print();
				std::cout << "\n";
				std::cout << "The rc hull of the upper half at level " << i<< "\n";
				current->rightchild->Qr.print();
				std::cout << "\n";
			}
			if ( new_node->label <= current->label )
				current = current->leftchild;
			else 
				current = current->rightchild;
			i++;
		}
		if ( current == root_ )
		{
			if ( current->label <= new_node->label )
			{
				new_label_node = createLabel(current->label, current, new_node);
				new_label_node->leftchild = current;
				new_label_node->rightchild = new_node;
				new_node->parent = new_label_node;
				new_label_node->leftchild->parent = new_label_node;
			}
			else
			{
				new_label_node = createLabel(new_node->label, new_node, current);
				new_label_node->rightchild = current;
				new_label_node->leftchild = new_node;
				new_node->parent = new_label_node;
				new_label_node->rightchild->parent = new_label_node;
			}
			root_ = new_label_node;
		}
		else
		{
			parent = current->parent;
			if ( current->label <= new_node->label )
			{
				new_label_node = createLabel(current->label, current, new_node);
				new_label_node->leftchild = current;
				new_label_node->rightchild = new_node;
				new_node->parent = new_label_node;
				new_label_node->leftchild->parent = new_label_node;
			}
			else
			{
				new_label_node = createLabel(new_node->label, new_node, current);
				new_label_node->rightchild = current;
				new_label_node->leftchild = new_node;
				new_node->parent = new_label_node;
				new_label_node->rightchild->parent = new_label_node;
			}
			if ( parent->rightchild == current )
			{
				parent->rightchild = new_label_node;
				right = true;
			}
			else
			{
				parent->leftchild = new_label_node;
				right = false;
			}
			current->parent = new_label_node;
			new_label_node->parent = parent;
		}
		updateBalanceFactor(new_label_node->parent, right);
	}
}

void Tree::updateBalanceFactor(tree_node *current, bool right)
{
	bool updatelabel;
	updatelabel = true;
	while ( current != NULL )
	{
		if ( right )
		{
			if ( updatelabel )
				current->balance_factor ++;
			if ( current->balance_factor == 2)
			{
				if ( current->rightchild->balance_factor == -1 )
				{
					buildChildrensHulls(current->rightchild);
					buildChildrensHulls(current->rightchild->leftchild);
					clockwiseAnticlockDoubleRotation(current);
					updateHull(current);
					updateHull(current->parent->rightchild);
				}
				else
				{
					buildChildrensHulls(current->rightchild);
					anticlockwiseRotation(current);
					updateHull(current);
				}
				updatelabel = false;
				current = current->parent;
				updateHull(current);
			}
			else
				updateHull(current);
		}
		else
		{
			if ( updatelabel )
				current->balance_factor --;
			if ( current->balance_factor == -2)
			{
				if ( current->leftchild->balance_factor == 1 )
				{
					buildChildrensHulls(current->leftchild);
					buildChildrensHulls(current->leftchild->rightchild);
					anticlockwiseClockDoubleRotation(current);
					updateHull(current);
					updateHull(current->parent->leftchild);
				}
				else
				{
					buildChildrensHulls(current->leftchild);
					clockwiseRotation(current);
					updateHull(current);
				}
				updatelabel = false;
				current = current->parent;
				updateHull(current);
			}
			else
				updateHull(current);
		}
		if ( (current == root_) || (current->balance_factor == 0) )
			updatelabel = false;
		else if ( current == current->parent->rightchild )
			right = true;
		else
			right = false;
		if (show_)
		{
			std::cout << "The updated lc-hull of the node with label " << current->label<< "\n";
			current->Ql.print();
			std::cout << "\n";
			std::cout << "The updated rc-hull of the node with label " << current->label<< "\n";
			current->Qr.print();
			std::cout << "\n";
		}
		current = current->parent;
	}
}

void Tree::deleteNode(const point &point_to_delete)
{
	bool right, uLabel, update;
	tree_node *node_to_delete(NULL), *parent(NULL), *brother(NULL);
	node_to_delete = searchNode(point_to_delete);
	if ( node_to_delete != NULL )
	{
		if ( node_to_delete == root_ )
			// TODO: (check if root has to be deleted
			root_ = NULL;
		else 
		{
			parent = node_to_delete->parent;
			if ( parent == root_ )
			{
				if ( parent->rightchild ==  node_to_delete )
					root_ = parent->leftchild;
				else
					root_ = parent->rightchild;
				root_->parent = NULL;
			}
			else
			{
				if ( parent->rightchild ==  node_to_delete )
				{
					brother = parent->leftchild;
					uLabel =  true;
					update = false;
				}
				else
				{
					brother = parent->rightchild;
					uLabel = false;
					update = true;
				}
				if ( parent->parent->rightchild == parent )
				{
					parent->parent->rightchild = brother;
					right = true;
				}
				else
				{
					right = false;
					parent->parent->leftchild = brother;
				}
				brother->parent = parent->parent;
				if ( parent->parent->parent != NULL )
				{
					if ( (parent->parent->parent->rightchild ==  parent->parent) && (right) )
						uLabel = false;
					else if ( !right )
						uLabel = true;
					else
						uLabel = false;
				}
				if ( brother->rightchild == NULL )
					updateBalanceFactorDelete(parent->parent, right, uLabel, update, brother->label);
				else
					updateBalanceFactorDelete(parent->parent, right, uLabel, update, brother->rightchild->label);
			}       
			delete node_to_delete;
			delete parent;
		}
	}
}

void Tree::updateBalanceFactorDelete(tree_node *current, bool right, bool update_label, bool update_label_done, const double &label)
{
	bool updateBF, balanced;
	updateBF = true;    
	while ( current != NULL )
	{
		balanced = false;
		if ( update_label && !update_label_done )
		{
			current->label = label;
			update_label_done = true;
		}
		if ( right )
		{
			if ( updateBF )
				current->balance_factor --;
			if ( current->balance_factor == -2)
			{
				if ( current->leftchild->balance_factor == 1 )
				{
					buildChildrensHulls(current->leftchild);
					buildChildrensHulls(current->leftchild->rightchild);
					anticlockwiseClockDoubleRotation(current);
					updateHull(current);
					updateHull(current->parent->leftchild);
					balanced = true;
				}
				else if ( current->leftchild->balance_factor == -1 )
				{
					buildChildrensHulls(current->leftchild);
					clockwiseRotation(current);
					updateHull(current);
					balanced = true;
				}
				else
				{
					buildChildrensHulls(current->leftchild);
					clockwiseRotation(current);
					updateHull(current);
				}
				current = current->parent;
				updateHull(current);
			}
			else
				updateHull(current);
		}
		else
		{
			if ( updateBF )
				current->balance_factor ++;
			if ( current->balance_factor == 2)
			{
				if ( current->rightchild->balance_factor == -1 )
				{
					buildChildrensHulls(current->rightchild);
					buildChildrensHulls(current->rightchild->leftchild);
					clockwiseAnticlockDoubleRotation(current);
					updateHull(current);
					updateHull(current->parent->rightchild);
					balanced = true;
				}
				else if ( current->rightchild->balance_factor == 1 )
				{
					buildChildrensHulls(current->rightchild);
					anticlockwiseRotation(current);
					updateHull(current);
					balanced = true;
				}
				else
				{
					buildChildrensHulls(current->rightchild);
					anticlockwiseRotation(current);
					updateHull(current);
				}
				current = current->parent;
				updateHull(current);
			}
			else
				updateHull(current);
		}
		if ( current->parent != NULL )
		{
			if ( current->parent->rightchild == current )
				right = true;
			else
				right = false;
		}
		if ( (current->balance_factor != 0) && (!balanced) )
			updateBF = false;
		if (show_)
		{
			std::cout << "The updated lc-hull of the node with label " << current->label<< "\n";
			current->Ql.print();
			std::cout << "\n";
			std::cout << "The updated rc-hull of the node with label " << current->label<< "\n";
			current->Qr.print();
			std::cout << "\n";
		}
		current = current->parent;
		if ( current != NULL )
		{
			if ( current->parent != NULL )
			{
				if ( (current->parent->rightchild == current) && (right) )
					update_label = false;
				else if ( !right )
					update_label = true;
				else
					update_label = false;
			}
			else if ( !right )
			{
				update_label = true;
			}
			else
			{
				update_label = false;
			}
		}
	}
}

tree_node *Tree::searchNode(const point &point_to_search)
{
	tree_node *current = root_;
	int i(1);
	while ( current->rightchild != NULL )
	{
		buildChildrensHulls(current);
		if (show_)
		{
			std::cout << "The lc-hull of the lower half at level " << i<< "\n";
			current->leftchild->Ql.print();
			std::cout << "\n";
			std::cout << "The rc-hull of the lower half at level " << i<< "\n";
			current->leftchild->Qr.print();
			std::cout << "\n";
			std::cout << "The lc-hull of the upper half at level " << i<< "\n";
			current->rightchild->Ql.print();
			std::cout << "\n";
			std::cout << "The rc-hull of the upper half at level " << i<< "\n";
			current->rightchild->Qr.print();
			std::cout << "\n";
		}
		if (point_to_search.y_coord <= current->label)
			current = current->leftchild;
		else
			current = current->rightchild;
		i++;
	}
	if ( current->p == point_to_search)
		return current;
	else 
		return NULL;
}

void Tree::print()
{
	printValues(root_, 0);
}

void Tree::printValues(tree_node *current_node, const int &indent)
{
	int i;
	if ( current_node != NULL )
	{
		for (i = 0; i < indent; ++i)
			std::cout << "---";
		if ( current_node->rightchild == NULL )
			std::cout << "-- (" << current_node->p.x_coord << ", " << current_node->p.y_coord << ")\n";
		else
			std::cout << "-- " << current_node->label << "\n";
		std::cout << "\n";
		printValues(current_node->leftchild, indent + 1);
		printValues(current_node->rightchild, indent + 1);
	}
}

tree_node* Tree::root() const
{
	return root_;
}

void Tree::set_show(const bool &show)
{
	show_ = show;
}
