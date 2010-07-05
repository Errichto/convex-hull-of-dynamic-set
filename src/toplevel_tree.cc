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


Tree::Tree() : root(NULL), show(0)
{
}

void Tree::updateHull(tree_node *current)
{
	ConcatenableQueue upperLcHull, upperRcHull, lowerLcHull, lowerRcHull, completeLcHull, completeRcHull, aux1, aux2, aux3, aux4;
	//printf("current label %f \n", current->label);
	updateBridge_Hull(current,0);
	//printf("dd\n");
	//printf("**********\n");
	updateBridge_Hull(current,1);
	//printf("b1.x = %f b1.y = %f b.x=%f b2.y = %f\n",current->bridge1_rc.x_coord,current->bridge1_rc.y_coord, current->bridge2_rc.x_coord,current->bridge2_rc.y_coord);
	split_left(current->leftchild->Ql,&lowerLcHull, &aux1, current->bridge1_lc.y_coord);
	split_left(current->leftchild->Qr,&lowerRcHull, &aux3, current->bridge1_rc.y_coord);
	current->leftchild->Ql = aux1;
	current->leftchild->Qr = aux3;
	split_right(current->rightchild->Ql, &aux2, &upperLcHull, current->bridge2_lc.y_coord);
	split_right(current->rightchild->Qr, &aux4, &upperRcHull, current->bridge2_rc.y_coord);
	current->rightchild->Ql = aux2;
	current->rightchild->Qr = aux4;
	completeLcHull = concatenate(lowerLcHull,upperLcHull);
	completeRcHull = concatenate(lowerRcHull,upperRcHull);
	current->Ql = completeLcHull;   
	current->Qr = completeRcHull;
}

//bridge lc hull operations

void Tree::get_points_righthalf(node *v, point *p, point *p0, point *p1, int *flag)
{
	if ( v->left->type != 0 )
	{
		*flag = 0;
		p->x_coord = v->right->smallest_y->x_coord;
		p->y_coord = v->right->smallest_y->data2;
		if ( v->type == 2 )
		{
			p0->x_coord = v->middle->biggest_y->x_coord;
			p0->y_coord = v->middle->biggest_y->data2;
		}
		else
		{
			p0->x_coord = v->left->biggest_y->x_coord;
			p0->y_coord = v->left->biggest_y->data2;
		}
		if ( v->right->smallest_y->parent->type == 2 )
		{
			p1->x_coord = v->right->smallest_y->parent->middle->x_coord;
			p1->y_coord = v->right->smallest_y->parent->middle->data2;
		}
		else
		{
		p1->x_coord = v->right->smallest_y->parent->right->x_coord;
		p1->y_coord = v->right->smallest_y->parent->right->data2;
		}
	}
	else
	{
		p0->x_coord = v->left->x_coord;
		p0->y_coord = v->left->data2;
		if ( v->type == 2 )
		{
			p->x_coord = v->middle->x_coord;
			p->y_coord = v->middle->data2;
			p1->x_coord = v->right->x_coord;
			p1->y_coord = v->right->data2;
			*flag = 0;
		}
		else
		{
			p->x_coord = v->right->x_coord;
			p->y_coord = v->right->data2;
			if ( v->parent == NULL )
				*flag = 1;
			else
			{
				if ( v->parent->right == v )
					*flag = 1;
				else if ( (v->parent->middle == v) || (v->parent->type == 1) )
				{
					p1->x_coord = v->parent->right->smallest_y->x_coord;
					p1->y_coord = v->parent->right->smallest_y->data2;
					*flag = 0;
				}
				else
				{
					p1->x_coord = v->parent->middle->smallest_y->x_coord;
					p1->y_coord = v->parent->middle->smallest_y->data2;
					*flag = 0;
				}
			}
		}
	}
}

void Tree::get_points_lefthalf(node *v, point *q, point *q0, point *q1, int *flag)
{
	if ( v->left->type != 0 )
	{
		*flag = 0;
		q->x_coord = v->left->biggest_y->x_coord;
		q->y_coord = v->left->biggest_y->data2;
		if ( v->type == 2 )
		{
			q1->x_coord = v->middle->smallest_y->x_coord;
			q1->y_coord = v->middle->smallest_y->data2;
		}
		else
		{
			q1->x_coord = v->right->smallest_y->x_coord;
			q1->y_coord = v->right->smallest_y->data2;
		}
		if ( v->left->biggest_y->parent->type == 2 )
		{
			q0->x_coord = v->left->biggest_y->parent->middle->x_coord;
			q0->y_coord = v->left->biggest_y->parent->middle->data2;
		}
		else
		{
			q0->x_coord = v->left->biggest_y->parent->left->x_coord;
			q0->y_coord = v->left->biggest_y->parent->left->data2;
		}
	}
	else
	{
		q1->x_coord = v->right->x_coord;
		q1->y_coord = v->right->data2;
		if ( v->type == 2 )
		{
			q->x_coord = v->middle->x_coord;
			q->y_coord = v->middle->data2;
			q0->x_coord = v->left->x_coord;
			q0->y_coord = v->left->data2;
			*flag = 0;
		}
		else
		{
			q->x_coord = v->left->x_coord;
			q->y_coord = v->left->data2;
			if ( v->parent == NULL )
				*flag = 1;
			else
			{
				if ( v->parent->left == v )
					*flag = 1;
				else if ( (v->parent->middle == v) || (v->parent->type == 1) )
				{
					q0->x_coord = v->parent->left->biggest_y->x_coord;
					q0->y_coord = v->parent->left->biggest_y->data2;
					*flag = 0;
				}
				else
				{
					q0->x_coord = v->parent->middle->biggest_y->x_coord;
					q0->y_coord = v->parent->middle->biggest_y->data2;
					*flag = 0;
				}
			}
		}
	}
}

int Tree::right_turn(point p0, point p1, point p2)
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

double Tree::case9(point p, point p0, point p1, point q, point q0, point q1)
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

int Tree::bridge_cases_lcHull(point p, point p0, point p1, point q, point q0, point q1, double half, int flag_lh, int flag_uh)
{
	int chck_p0, chck_p1, chck_q0, chck_q1;
	double intersec;
	if ( flag_lh == 0 )
	{
		chck_q0 = right_turn(p,q,q0);
		chck_q1 = right_turn(p,q,q1);
	}
	else if ( flag_lh == 1 )
	{
		chck_q0 = 0;
		chck_q1 = right_turn(p,q,q1);
	}
	else
	{
		chck_q0 = 0;
		chck_q1 = 0;
	}
	if ( flag_uh == 0 )
	{
		chck_p0 = right_turn(q,p,p0);
		chck_p1 = right_turn(q,p,p1);
	}
	else if ( flag_uh == 1 )
	{
		chck_p1 = 1;
		chck_p0 = right_turn(q,p,p0);
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

int Tree::bridge_cases_rcHull(point p, point p0, point p1, point q, point q0, point q1, double half, int flag_lh, int flag_uh)
{
	int chck_p0, chck_p1, chck_q0, chck_q1;
	double intersec;
	if ( flag_lh == 0 )
	{
		chck_q0 = right_turn(p,q,q0);
		chck_q1 = right_turn(p,q,q1);
	}
	else if ( flag_lh == 1 )
	{
		chck_q0 = 1;
		chck_q1 = right_turn(p,q,q1);
	}
	else
	{
		chck_q0 = 1;
		chck_q1 = 1;
	}
	if ( flag_uh == 0 )
	{
		chck_p0 = right_turn(q,p,p0);
		chck_p1 = right_turn(q,p,p1);
	}
	else if ( flag_uh == 1 )
	{
		chck_p1 = 0;
		chck_p0 = right_turn(q,p,p0);
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

node *Tree::newSS_lh_left(node *current, point *q, point *q0, point *q1, bool *onepoint)
{
	if ( current->type == 0 )
	{
		if ( !*onepoint )
		{
			*onepoint = true;
			if ( current->parent != NULL )
			{
				if ( current->parent->right == current )
				{
					q1->y_coord = q->y_coord;
					q1->x_coord = q->x_coord;
					if ( current->parent->type == 2 )
					{
						q->y_coord = current->parent->middle->data2;
						q->x_coord = current->parent->middle->x_coord;
					}
					else
					{
						q->y_coord = current->parent->left->data2;
						q->x_coord = current->parent->left->x_coord;
					}
					*onepoint = false;
				}
			}
		}
		return NULL;
	}
	else if ( current->left->type == 0 )
	{
		if ( current->type == 2 )
		{
			q1->x_coord = q->x_coord;
			q1->y_coord = q->y_coord;
			*onepoint = false;
		}
		else
			*onepoint = true;
		q->x_coord = current->left->x_coord;
		q->y_coord = current->left->data2;
		return current->left;
	}
	else if ( q->y_coord == current->left->biggest_y->data2 )
		return current->left;
	else if ( q->y_coord == current->right->smallest_y->data2 )
	{
		q1->y_coord = q->y_coord;
		q1->x_coord = q->x_coord;
		q->y_coord = q0->y_coord;
		q->x_coord = q0->x_coord;
		*onepoint = false;
		return current->right->smallest_y;
	}
	else
	{
		if ( q->y_coord == current->middle->smallest_y->data2 )
		{
			q1->y_coord = q->y_coord;
			q1->x_coord = q->x_coord;
			q->y_coord = q0->y_coord;
			q->x_coord = q0->x_coord;
			*onepoint = false;
			return current->right->smallest_y;
		}
		else
			return current->middle;
	}
}

node *Tree::newSS_uh_right(node *current, point *p, point *p0, point *p1, bool *onepoint)
{
	if ( current->type == 0 )
	{
		if ( !*onepoint )
		{
			*onepoint = true;
			if ( current->parent != NULL )
			{
				if ( current->parent->left == current )
				{
					p0->y_coord = p->y_coord;
					p0->x_coord = p->x_coord;
					if ( current->parent->type == 2 )
					{
						p->y_coord = current->parent->middle->data2;
						p->x_coord = current->parent->middle->x_coord;
					}
					else
					{
						p->y_coord = current->parent->right->data2;
						p->x_coord = current->parent->right->x_coord;
					}
					*onepoint = false;
				}
			}
		}
		return NULL;
	}
	else if ( current->right->type == 0 )
	{
		if ( current->type == 2 )
		{
			p0->x_coord = p->x_coord;
			p0->y_coord = p->y_coord;
			*onepoint = false;
		}
		else
			*onepoint = true;
		p->x_coord = current->right->x_coord;
		p->y_coord = current->right->data2;
		return current->right;
	}
	else if ( p->y_coord == current->right->smallest_y->data2 )
		return current->right;
	else if ( p->y_coord == current->left->biggest_y->data2 )
	{
		p0->y_coord = p->y_coord;
		p0->x_coord = p->x_coord;
		p->y_coord = p1->y_coord;
		p->x_coord = p1->x_coord;
		*onepoint = false;
		return current->left->biggest_y;
	}
	else
	{
		if ( p->y_coord == current->middle->biggest_y->data2 )
		{
			p0->y_coord = p->y_coord;
			p0->x_coord = p->x_coord;
			p->y_coord = p1->y_coord;
			p->x_coord = p1->x_coord;
			*onepoint = false;
			return current->middle->biggest_y;
		}
		else
			return current->middle;
	}
}

node *Tree::newSS_lh_right(node *current, point *q, point *q0, point *q1, bool *onepoint, bool *out)
{
	double x,y;
	node *newcurrent;
	newcurrent = NULL;
	if ( current->type == 0 )
	{
		*out = true;
	}
	else if ( current->left->type == 0 )
	{
		*onepoint = false;
		*out = true;
		newcurrent = current->right;
	}
	else if ( q->y_coord == current->right->smallest_y->data2 )
	{
		newcurrent = current->right;
		*out = true;
	}
	else if ( current->type == 2 )
	{
		x = q1->x_coord;
		y = q1->y_coord;
		if ( q->y_coord == current->left->biggest_y->data2 )
		{
			if ( current->middle->smallest_y->parent->type == 2 )
			{
				q1->x_coord = current->middle->smallest_y->parent->middle->x_coord;
				q1->y_coord = current->middle->smallest_y->parent->middle->data2;
			}
			else
			{
				q1->x_coord = current->middle->smallest_y->parent->right->x_coord;
				q1->y_coord = current->middle->smallest_y->parent->right->data2;
			}
			q0->x_coord = q->x_coord;
			q0->y_coord = q->y_coord;
			q->x_coord = x;
			q->y_coord = y;
		}
		else if ( q->y_coord == current->middle->smallest_y->data2 )
		{
			q1->x_coord = current->right->smallest_y->x_coord;
			q1->y_coord = current->right->smallest_y->data2;
			q->x_coord = current->middle->biggest_y->x_coord;
			q->y_coord = current->middle->biggest_y->data2;
			if ( current->middle->biggest_y->parent->type == 2 )
			{
				q0->x_coord = current->middle->biggest_y->parent->middle->x_coord;
				q0->y_coord = current->middle->biggest_y->parent->middle->data2;
			}
			else
			{
				q0->x_coord = current->middle->biggest_y->parent->left->x_coord;
				q0->y_coord = current->middle->biggest_y->parent->left->data2;
			}
		}
		else
		{
			if ( current->right->smallest_y->parent->type == 2 )
			{
				q1->x_coord = current->right->smallest_y->parent->middle->x_coord;
				q1->y_coord = current->right->smallest_y->parent->middle->data2;
			}
			else
			{
				q1->x_coord = current->right->smallest_y->parent->right->x_coord;
				q1->y_coord = current->right->smallest_y->parent->right->data2;
			}
			q0->x_coord = q->x_coord;
			q0->y_coord = q->y_coord;
			q->x_coord = x;
			q->y_coord = y;
		}
	}
	else
	{
		x = q1->x_coord;
		y = q1->y_coord;
		if ( current->right->smallest_y->parent->type == 2 )
		{
			q1->x_coord = current->right->smallest_y->parent->middle->x_coord;
			q1->y_coord = current->right->smallest_y->parent->middle->data2;
		}
		else
		{
			q1->x_coord = current->right->smallest_y->parent->right->x_coord;
			q1->y_coord = current->right->smallest_y->parent->right->data2;
		}
		q0->x_coord = q->x_coord;
		q0->y_coord = q->y_coord;
		q->x_coord = x;
		q->y_coord = y;
	}
	return newcurrent;
}

node *Tree::newSS_uh_left(node *current, point *p, point *p0, point *p1, bool *onepoint, bool *out)
{
	double x,y;
	node *newcurrent;
	newcurrent = NULL;
	if ( current->type == 0 )
		*out = true;
	else if ( current->left->type == 0 )
	{
		*onepoint = false;
		*out = true;
		newcurrent = current->left;
	}
	else if ( p->y_coord == current->left->biggest_y->data2 )
	{
		newcurrent = current->left;
		*out = true;
	}
	else if ( current->type == 2 )
	{
		x = p0->x_coord;
		y = p0->y_coord;
		if ( p->y_coord == current->right->smallest_y->data2 )
		{
			if ( current->middle->biggest_y->parent->type == 2 )
			{
				p0->x_coord = current->middle->biggest_y->parent->middle->x_coord;
				p0->y_coord = current->middle->biggest_y->parent->middle->data2;
			}
			else
			{
				p0->x_coord = current->middle->biggest_y->parent->left->x_coord;
				p0->y_coord = current->middle->biggest_y->parent->left->data2;
			}

			p1->x_coord = p->x_coord;
			p1->y_coord = p->y_coord;
			p->x_coord = x;
			p->y_coord = y;
		}
		else if ( p->y_coord == current->middle->biggest_y->data2 )
		{
			p0->x_coord = current->left->biggest_y->x_coord;
			p0->y_coord = current->left->biggest_y->data2;
			p->x_coord = current->middle->smallest_y->x_coord;
			p->y_coord = current->middle->smallest_y->data2;
			if ( current->middle->smallest_y->parent->type == 2 )
			{
				p1->x_coord = current->middle->smallest_y->parent->middle->x_coord;
				p1->y_coord = current->middle->smallest_y->parent->middle->data2;
			}
			else
			{
				p1->x_coord = current->middle->smallest_y->parent->right->x_coord;
				p1->y_coord = current->middle->smallest_y->parent->right->data2;
			}
		}
		else
		{
			if ( current->left->biggest_y->parent->type == 2 )
			{
				p0->x_coord = current->left->biggest_y->parent->middle->x_coord;
				p0->y_coord = current->left->biggest_y->parent->middle->data2;
			}
			else
			{
				p0->x_coord = current->left->biggest_y->parent->left->x_coord;
				p0->y_coord = current->left->biggest_y->parent->left->data2;
			}
			p1->x_coord = p->x_coord;
			p1->y_coord = p->y_coord;
			p->x_coord = x;
			p->y_coord = y;
		}
	}
	else
	{
		x = p0->x_coord;
		y = p0->y_coord;
		if ( current->left->smallest_y->parent->type == 2 )
		{
			p0->x_coord = current->left->biggest_y->parent->middle->x_coord;
			p0->y_coord = current->left->biggest_y->parent->middle->data2;
		}
		else
		{
			p0->x_coord = current->left->smallest_y->parent->left->x_coord;
			p0->y_coord = current->left->smallest_y->parent->left->data2;
		}
		p1->x_coord = p->x_coord;
		p1->y_coord = p->y_coord;
		p->x_coord = x;
		p->y_coord = y;
	}
	return newcurrent;
}

//

void Tree::buildChildrensHulls(tree_node *parent)
{
	ConcatenableQueue upperLcHull, upperRcHull, lowerLcHull, lowerRcHull, completeUpperLcHull, completeUpperRcHull, completeLowerLcHull, completeLowerRcHull;
	split_right(parent->Ql,&lowerLcHull, &upperLcHull, parent->bridge2_lc.y_coord);
	split_right(parent->Qr,&lowerRcHull, &upperRcHull, parent->bridge2_rc.y_coord);
	completeUpperLcHull = concatenate(parent->rightchild->Ql,upperLcHull);
	completeUpperRcHull = concatenate(parent->rightchild->Qr,upperRcHull);
	completeLowerLcHull = concatenate(lowerLcHull, parent->leftchild->Ql);
	completeLowerRcHull = concatenate(lowerRcHull, parent->leftchild->Qr);
	parent->rightchild->Ql = completeUpperLcHull;
	parent->leftchild->Ql = completeLowerLcHull; 
	parent->rightchild->Qr = completeUpperRcHull;
	parent->leftchild->Qr = completeLowerRcHull;
}

void Tree::updateBridge_Hull(tree_node *v, int hull)
{
	bool bridgefound, onepoint_lh, onepoint_uh, out;
	double half;
	int chk_case, flag_lh, flag_uh, i=1;
	point p, p0, p1, q, q0, q1;
	node *current_lh, *current_uh, *aux;
	bridgefound = false;
	onepoint_lh = true;
	onepoint_uh = true;
	if ( hull == 0 )
	{
		current_lh = v->leftchild->Ql.root;
		current_uh = v->rightchild->Ql.root;
	}
	else
	{
		current_lh = v->leftchild->Qr.root;
		current_uh = v->rightchild->Qr.root;
	}
	if ( (current_lh->type != 0) && (current_uh->type != 0) )
		half = current_uh->smallest_y->data2 - ((current_uh->smallest_y->data2 - current_lh->biggest_y->data2)/2);
	else if ( current_lh->type != 0 )
		half = current_uh->data2 - ((current_uh->data2 - current_lh->biggest_y->data2)/2);
	else if ( current_uh->type != 0 )
		half = current_uh->smallest_y->data2 - ((current_uh->smallest_y->data2 - current_lh->data2)/2);
	else
		half = current_uh->data2 - ((current_uh->data2 - current_lh->data2)/2);
	p.x_coord = current_uh->x_coord;
	p.y_coord = current_uh->data2;
	q.x_coord = current_lh->x_coord;
	q.y_coord = current_lh->data2;
	while ( !bridgefound )
	{
		if ( (current_lh->type == 0) && (current_uh->type == 0) )
		{
			if ( (onepoint_lh) && (onepoint_uh) )
				bridgefound = true;
			else if ( onepoint_lh )
			{
				if ( hull == 0 )
					chk_case = bridge_cases_lcHull(p,p0,p1,q,q0,q1,half,2,1);
				else
					chk_case = bridge_cases_rcHull(p,p0,p1,q,q0,q1,half,2,1);
				if ( chk_case != 1 )
					p = p0;
				bridgefound = true;
			}
			else if ( onepoint_uh )
			{
				if ( hull == 0 )
					chk_case = bridge_cases_lcHull(p,p0,p1,q,q0,q1,half,1,2);
				else
					chk_case = bridge_cases_rcHull(p,p0,p1,q,q0,q1,half,1,2);
				if ( chk_case != 1 )
					q = q1;
				bridgefound = true;
			}
			else
			{
				if ( hull == 0 )
					chk_case = bridge_cases_lcHull(p,p0,p1,q,q0,q1,half,1,1);
				else
					chk_case = bridge_cases_rcHull(p,p0,p1,q,q0,q1,half,1,1);
				if ( chk_case != 1 )
				{
					if ( hull == 0 )
						chk_case = bridge_cases_lcHull(p0,p,p1,q,q0,q1,half,1,2);
					else
						chk_case = bridge_cases_rcHull(p0,p,p1,q,q0,q1,half,1,2);
					if ( chk_case != 1 )
					{
						if ( hull == 0 )
							chk_case = bridge_cases_lcHull(p,p0,p1,q1,q0,q,half,2,1);
						else
							chk_case = bridge_cases_rcHull(p,p0,p1,q1,q0,q,half,2,1);
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
			if ( current_uh->type == 0 )
			{
				get_points_lefthalf(current_lh, &q, &q0, &q1, &flag_lh);
				if ( onepoint_uh )
					flag_uh = 2;
				else
					flag_uh = 1;
			}
			else if ( current_lh->type == 0 )
			{
				get_points_righthalf(current_uh, &p, &p0, &p1, &flag_uh);
				if ( onepoint_lh )
					flag_lh = 2;
				else
					flag_lh = 1;
			}   
			else
			{
				get_points_lefthalf(current_lh, &q, &q0, &q1, &flag_lh);
				get_points_righthalf(current_uh, &p, &p0, &p1, &flag_uh);
			}
			out = false;
			while (!out)
			{
				if ( hull == 0 )
					chk_case = bridge_cases_lcHull(p,p0,p1,q,q0,q1,half,flag_lh,flag_uh);
				else
					chk_case = bridge_cases_rcHull(p,p0,p1,q,q0,q1,half,flag_lh,flag_uh);
				switch (chk_case)
				{
					case 1:
						bridgefound = true;
						out = true;
						break;
					case 2:
					case 4:
					case 6:
						aux = newSS_lh_left(current_lh, &q, &q0, &q1, &onepoint_lh);
						if ( aux != NULL )
							current_lh = aux;
						aux = newSS_uh_right(current_uh, &p, &p0, &p1, &onepoint_uh);
						if ( aux != NULL )
							current_uh = aux;
						out = true;
						break;
					case 7:
						aux = newSS_lh_left(current_lh, &q, &q0, &q1, &onepoint_lh);
						if ( aux != NULL )
							current_lh = aux;
						out = true;
						break;
					case 8:
						aux = newSS_uh_right(current_uh, &p, &p0, &p1, &onepoint_uh);
						if ( aux != NULL )
							current_uh = aux;
						out = true;
						break;
					case 3:
						aux = newSS_lh_right(current_lh, &q, &q0, &q1, &onepoint_lh, &out);
						if ( aux != NULL )
							current_lh = aux;
						if ( out )
						{
							aux = newSS_uh_right(current_uh, &p, &p0, &p1, &onepoint_uh);
							if ( aux != NULL )
								current_uh = aux;
						}
						break;
					case 5:
						aux = newSS_uh_left(current_uh, &p, &p0, &p1, &onepoint_uh, &out);
						if ( aux != NULL )
							current_uh = aux;
						if ( out )
						{
							aux = newSS_lh_left(current_lh, &q, &q0, &q1, &onepoint_lh);
							if ( aux != NULL )
								current_lh = aux;
						}
						break;
					case 9:
						aux = newSS_lh_right(current_lh, &q, &q0, &q1, &onepoint_lh, &out);
						if ( aux != NULL )
							current_lh = aux;
						if ( (current_lh->type == 0) && (current_uh->type != 0) )
						{
							out = false;
							flag_lh = 2;
							q.y_coord = q1.y_coord;
							q.x_coord = q1.x_coord;
						}
						break;
					case 10:
						aux = newSS_uh_left(current_uh, &p, &p0, &p1, &onepoint_uh, &out);
						if ( aux != NULL )
							current_uh = aux;
						if ( (current_uh->type == 0) && (current_lh->type != 0) )
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
		v->bridge1_lc = q;
		v->bridge2_lc = p;
	}
	else
	{
		v->bridge1_rc = q;
		v->bridge2_rc = p;
	}
}

tree_node *Tree::create_label(double newLabel, tree_node *leftchild, tree_node *rightchild)
{
	tree_node *newNode;
	newNode = new tree_node;
	newNode->label = newLabel;
	newNode->Ql = concatenate(leftchild->Ql, rightchild->Ql);
	newNode->Qr = concatenate(leftchild->Qr, rightchild->Qr);
	newNode->bridge1_lc = leftchild->p;
	newNode->bridge2_lc = rightchild->p;
	newNode->bridge1_rc = leftchild->p;
	newNode->bridge2_rc = rightchild->p;
	leftchild->Ql.root = NULL;
	rightchild->Ql.root = NULL;
	leftchild->Qr.root = NULL;
	rightchild->Qr.root = NULL;
	newNode->leftchild = NULL;
	newNode->rightchild = NULL;
	newNode->parent = NULL;
	newNode->balance_factor = 0;
	return newNode;
}

tree_node *Tree::create_object(point newPoint)
{
	tree_node *newNode;
	ConcatenableQueue lcHull(newPoint.x_coord, newPoint.y_coord);
	ConcatenableQueue rcHull(newPoint.x_coord, newPoint.y_coord);
	newNode = new tree_node;
	newNode->p = newPoint;
	newNode->label = newPoint.y_coord;
	newNode->Ql = lcHull;
	newNode->Qr = rcHull;
	newNode->leftchild = NULL;
	newNode->rightchild = NULL;
	newNode->parent = NULL;
	newNode->balance_factor = 0;
	return newNode;
}

void Tree::anticlockwiseRotation(tree_node *zNode)
{
	tree_node *yNode;
	yNode = zNode->rightchild;
	zNode->rightchild = yNode->leftchild;
	yNode->leftchild->parent = zNode;
	yNode->leftchild = zNode;
	if ( zNode == root )
	{
		yNode->parent = NULL;
		root = yNode;
	}
	else if ( zNode == zNode->parent->leftchild )
	{
		zNode->parent->leftchild = yNode;
		yNode->parent = zNode->parent;
	}
	else
	{
		yNode->parent = zNode->parent;
		zNode->parent->rightchild = yNode;
	}
	zNode->parent = yNode;
	if ( yNode->balance_factor == 1 )
	{
		zNode->balance_factor = 0;
		yNode->balance_factor = 0;
	}
	else
	{
		zNode->balance_factor = 1;
		yNode->balance_factor = -1;
	}
}

void Tree::clockwiseRotation(tree_node *zNode)
{
	tree_node *yNode;
	yNode = zNode->leftchild;
	zNode->leftchild = yNode->rightchild;
	yNode->rightchild->parent = zNode;
	yNode->rightchild = zNode;
	if ( zNode == root )
	{
		yNode->parent = NULL;
		root = yNode;
	}
	else if ( zNode == zNode->parent->leftchild )
	{
		zNode->parent->leftchild = yNode;
		yNode->parent = zNode->parent;
	}
	else
	{
		zNode->parent->rightchild = yNode;
		yNode->parent = zNode->parent;
	}
	zNode->parent = yNode;
	if ( yNode->balance_factor == -1 )
	{
		zNode->balance_factor = 0;
		yNode->balance_factor = 0;
	}
	else
	{
		zNode->balance_factor = -1;
		yNode->balance_factor = 1;
	}
}

void Tree::clockwise_anticlockDoubleRotation(tree_node *zNode)
{
	tree_node *yNode, *xNode;
	yNode = zNode->rightchild;
	xNode = yNode->leftchild;
	zNode->rightchild = xNode->leftchild;
	xNode->leftchild->parent = zNode;
	yNode->leftchild = xNode->rightchild;
	xNode->rightchild->parent = yNode;
	xNode->rightchild = yNode;
	yNode->parent = xNode;
	xNode->parent = zNode->parent;
	xNode->leftchild = zNode;
	if ( zNode == root )
		root = xNode;
	else if ( zNode->parent->leftchild == zNode )
		zNode->parent->leftchild = xNode;
	else
		zNode->parent->rightchild = xNode;
	zNode->parent = xNode;
	if ( xNode->balance_factor == 0 )
	{
		yNode->balance_factor = 0;
		zNode->balance_factor = 0;
	}
	else if ( xNode->balance_factor == 1 )
	{
		yNode->balance_factor = 0;
		zNode->balance_factor = -1;
	}
	else
	{
		yNode->balance_factor = 1;
		zNode->balance_factor = 0;
	}
	xNode->balance_factor = 0;
}

void Tree::anticlockwise_clockDoubleRotation(tree_node *zNode)
{
	tree_node *yNode, *xNode;
	yNode = zNode->leftchild;
	xNode = yNode->rightchild;
	zNode->leftchild = xNode->rightchild;
	xNode->rightchild->parent = zNode;
	yNode->rightchild = xNode->leftchild;
	xNode->leftchild->parent = yNode;
	xNode->leftchild = yNode;
	yNode->parent = xNode;
	xNode->parent = zNode->parent;
	xNode->rightchild = zNode;
	if ( zNode == root )
		root = xNode;
	else if ( zNode->parent->leftchild == zNode )
		zNode->parent->leftchild = xNode;
	else
		zNode->parent->rightchild = xNode;
	zNode->parent = xNode;
	if ( xNode->balance_factor == 0 )
	{
		yNode->balance_factor = 0;
		zNode->balance_factor = 0;
	}
	else if ( xNode->balance_factor == 1 )
	{
		yNode->balance_factor = -1;
		zNode->balance_factor = 0;
	}
	else
	{
		yNode->balance_factor = 0;
		zNode->balance_factor = 1;
	}
	xNode->balance_factor = 0;
}

void Tree::add_node(point newPoint) 
{
	tree_node *newNode, *parent, *current, *newLabel;
	bool right;
	int i;
	newNode = create_object(newPoint);
	if ( root == NULL )
		root = newNode;
	else
	{
		current = root;
		i = 1;
		while ( current->leftchild !=NULL )
		{
			buildChildrensHulls(current);
			if ( show == 1 )
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
			if ( newNode->label <= current->label )
				current = current->leftchild;
			else 
				current = current->rightchild;
			i++;
		}
		if ( current == root )
		{
			if ( current->label <= newNode->label )
			{
				newLabel = create_label(current->label, current, newNode);
				newLabel->leftchild = current;
				newLabel->rightchild = newNode;
				newNode->parent = newLabel;
				newLabel->leftchild->parent = newLabel;
			}
			else
			{
				newLabel = create_label(newNode->label, newNode, current);
				newLabel->rightchild = current;
				newLabel->leftchild = newNode;
				newNode->parent = newLabel;
				newLabel->rightchild->parent = newLabel;
			}
			root = newLabel;
		}
		else
		{
			parent = current->parent;
			if ( current->label <= newNode->label )
			{
				newLabel = create_label(current->label, current, newNode);
				newLabel->leftchild = current;
				newLabel->rightchild = newNode;
				newNode->parent = newLabel;
				newLabel->leftchild->parent = newLabel;
			}
			else
			{
				newLabel = create_label(newNode->label, newNode, current);
				newLabel->rightchild = current;
				newLabel->leftchild = newNode;
				newNode->parent = newLabel;
				newLabel->rightchild->parent = newLabel;
			}
			if ( parent->rightchild == current )
			{
				parent->rightchild = newLabel;
				right = true;
			}
			else
			{
				parent->leftchild = newLabel;
				right = false;
			}
			current->parent = newLabel;
			newLabel->parent = parent;
		}
		updateBalanceFactor(newLabel->parent, right);
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
					clockwise_anticlockDoubleRotation(current);
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
					anticlockwise_clockDoubleRotation(current);
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
		if ( (current == root) || (current->balance_factor == 0) )
			updatelabel = false;
		else if ( current == current->parent->rightchild )
			right = true;
		else
			right = false;
		if ( show == 1 )
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

void Tree::delete_node(point pointToDelete)
{
	bool right, uLabel, update;
	double newLabel;
	tree_node *nodeToDelete, *parent, *brother, *aux, *aux1;
	nodeToDelete = search_node(pointToDelete);
	if ( nodeToDelete != NULL )
	{
		if ( nodeToDelete == root )
			root = NULL;
		else 
		{
			parent = nodeToDelete->parent;
			if ( parent == root )
			{
				if ( parent->rightchild ==  nodeToDelete )
					root = parent->leftchild;
				else
					root = parent->rightchild;
				root->parent = NULL;
			}
			else
			{
				if ( parent->rightchild ==  nodeToDelete )
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
					if ( (parent->parent->parent->rightchild ==  parent->parent) && (right) )
						uLabel = false;
					else if ( !right )
						uLabel = true;
					else
						uLabel = false;
				if ( brother->rightchild == NULL )
					updateBalanceFactor_delete(parent->parent, right, uLabel, update, brother->label);
				else
					updateBalanceFactor_delete(parent->parent, right, uLabel, update, brother->rightchild->label);
			}       
			delete nodeToDelete;
			delete parent;
		}
	}
}

void Tree::updateBalanceFactor_delete(tree_node *current, bool right, bool updateLabel, bool uLabelDone, double label)
{
	bool updateBF, balanced;
	updateBF = true;    
	while ( current != NULL )
	{
		balanced = false;
		if ( updateLabel && !uLabelDone )
		{
			current->label = label;
			uLabelDone = true;
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
					anticlockwise_clockDoubleRotation(current);
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
				//balanced = true;
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
					clockwise_anticlockDoubleRotation(current);
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
			if ( current->parent->rightchild == current )
				right = true;
			else
				right = false;
		if ( (current->balance_factor != 0) && (!balanced) )
			updateBF = false;
		if ( show == 1 )
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
			if ( current->parent != NULL )
				if ( (current->parent->rightchild == current) && (right) )
					updateLabel = false;
				else if ( !right )
					updateLabel = true;
				else
					updateLabel = false;
			else if ( !right )
				updateLabel = true;
			else
				updateLabel = false;
	}
}

tree_node *Tree::search_node(point pointToSearch)
{
	tree_node *current;
	current = root;
	int i;
	i = 1;
	while ( current->rightchild != NULL )
	{
		buildChildrensHulls(current);
		if ( show == 1 )
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
		if (pointToSearch.y_coord <= current->label)
			current = current->leftchild;
		else
			current = current->rightchild;
		i++;
	}
	if ( (current->p.y_coord == pointToSearch.y_coord) && (current->p.x_coord == pointToSearch.x_coord) )
		return current;
	else 
		return NULL;
}

void Tree::print()
{
	printValues(root, 0);
}

void Tree::printValues(tree_node *printNode, int indent)
{
	int i;
	if ( printNode != NULL )
	{
		for (i=0; i<indent; i++)
			std::cout << "---";
		//printf("-- label=%f, bf=%i //\n", printNode->label, printNode->balance_factor);
		if ( printNode->rightchild == NULL )
			std::cout << "-- (" << printNode->p.x_coord << ", " << printNode->p.y_coord << ")\n";
		else
			std::cout << "-- " << printNode->label << "\n";
		//printNode->Ql.print();
		std::cout << "\n";
		printValues(printNode->leftchild, indent+1);
		printValues(printNode->rightchild, indent+1);
	}
}
