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

ConcatenableQueue::ConcatenableQueue()
{
	root = NULL;
}

ConcatenableQueue::ConcatenableQueue(double x_coord, double y_coord)
{
	root = NULL;
	add_node(x_coord, y_coord);
}

node *ConcatenableQueue::rightmostNodeAtLevel(int level, node *bigger_y)
{
	node *currentNode;
	currentNode = root;
	currentNode->biggest_y =  bigger_y;
	for (int i=1; i<level; ++i)
	{
		currentNode = currentNode->right;
		currentNode->biggest_y =  bigger_y;
	}
	return currentNode;
}

node *ConcatenableQueue::leftmostNodeAtLevel(int level, node *smaller_y)
{
	node *currentNode;
	currentNode = root;
	currentNode->smallest_y = smaller_y;
	for (int i=1; i<level; i++)
	{
		currentNode = currentNode->left;
		currentNode->smallest_y = smaller_y;
	}
	return currentNode;
}

int ConcatenableQueue::height()
{
	int h = 0;
	node *currentNode;
	currentNode = root;
	while ( currentNode != NULL )
	{
		++h;
		currentNode = currentNode->right;
	}
	return h;
}

void ConcatenableQueue::updateValuesDelete(node *updateNode, double val, double newVal)
{
	while ( (updateNode != root) && (updateNode != NULL) )
	{
		if ( (updateNode == updateNode->parent->left) && (updateNode->parent->data1 == val) )
			updateNode->parent->data1 = newVal;
		else if ( (updateNode == updateNode->parent->middle) && (updateNode->parent->data2 == val) )
			updateNode->parent->data2 = newVal;
		else if ( (updateNode == updateNode->parent->right) && (updateNode->parent->middle == NULL) && (updateNode->parent->data2 == val) )
			updateNode->parent->data2 = newVal;
		updateNode = updateNode->parent;
	}
}

void ConcatenableQueue::updateValuesInsert(node *updateNode, double val)
{
	while ( updateNode != root )
	{
		if ( (updateNode == updateNode->parent->left) && (updateNode->parent->data1 < val) )
			updateNode->parent->data1 = val;
		else if ( (updateNode == updateNode->parent->middle) && (updateNode->parent->data2 < val) )
			updateNode->parent->data2 = val;
		else if ( (updateNode == updateNode->parent->right) && (updateNode->parent->middle == NULL) && (updateNode->parent->data2 < val) )
			updateNode->parent->data2 = val;
		updateNode = updateNode->parent;
		val = updateNode->data2;
	}
}

void ConcatenableQueue::deleteNode(node *nodeToDelete)
{
	node *father, *brother, *fathersBrother;
	double auxValue1, auxValue2;
	bool leftbrother;
	if ( nodeToDelete == root )
		root = NULL;
	else if ( (nodeToDelete->parent == root) && (root->type == 1) )
	{
		if ( nodeToDelete == root->left )
		{
			root->right->parent = NULL;
			root = root->right;
		}
		else
		{
			root->left->parent = NULL;
			root = root->left;
		}
	}
	else
	{
		father = new node;
		father = nodeToDelete->parent;
		if ( father->type == 2 )
		{
			if ( father->left == nodeToDelete )
			{
				father->left = father->middle;
			}
			else if ( father->right == nodeToDelete )
				father->right = father->middle;
			father->middle = NULL;
			father->type = 1;
			father->data1 = father->left->data2;
			father->data2 = father->right->data2;
			updateValuesDelete(father, nodeToDelete->data2, father->data2);
		}
		else
		{
			brother = new node;
			fathersBrother = new node;
			if ( father->left == nodeToDelete )
				brother = father->right;
			else
				brother = father->left;
			if ( father->parent->right == father )
			{
				if ( father->parent->type == 2 )
					fathersBrother = father->parent->middle;
				else
					fathersBrother = father->parent->left;
				leftbrother = true;
			}
			else
				if ( father->parent->left == father )
				{
					if ( father->parent->type == 2 )
						fathersBrother = father->parent->middle;
					else
						fathersBrother = father->parent->right;
					leftbrother = false;
				}
				else
				{
					fathersBrother = father->parent->left;
					leftbrother = true;
				}
			if ( fathersBrother->type == 2 )
			{
				auxValue1 = father->data2;
				if (leftbrother)
				{
					father->right = brother;
					auxValue2 = fathersBrother->right->data2;
					father->left = fathersBrother->right;
					father->left->parent = father;
					father->data1 = father->left->data2;
					father->data2 = father->right->data2;
					fathersBrother->right = fathersBrother->middle;
					father->data1 = father->left->data2;
					father->data2 = father->right->data2;
					fathersBrother->middle = NULL;
					fathersBrother->type = 1;
					updateValuesDelete(father, auxValue1, father->right->data2);
					updateValuesDelete(fathersBrother, auxValue2, fathersBrother->right->data2);
				}
				else
				{
					father->left = brother;
					father->right = fathersBrother->left;
					father->right->parent = father;
					fathersBrother->left = fathersBrother->middle;
					fathersBrother->data1 = fathersBrother->left->data2;
					fathersBrother->data2 = fathersBrother->right->data2;
					fathersBrother->middle = NULL;
					father->data1 = father->left->data2;
					father->data2 = father->right->data2;
					fathersBrother->type = 1;
					updateValuesDelete(father, auxValue1, father->right->data2);
				}
			}
			else
			{
				if (leftbrother)
				{
					fathersBrother->middle = fathersBrother->right;
					fathersBrother->right = brother;
					fathersBrother->right->parent = fathersBrother;
					updateValuesInsert(fathersBrother, fathersBrother->right->data2);
				}
				else
				{
					fathersBrother->middle = fathersBrother->left;
					fathersBrother->left = brother;
					fathersBrother->right->parent = fathersBrother; 
				}
				brother->parent = fathersBrother;
				fathersBrother->data1 = fathersBrother->left->data2;
				fathersBrother->data2 = fathersBrother->middle->data2;
				fathersBrother->type = 2;
				father->right = NULL;
				father->left = NULL;
				deleteNode(father);
				updateValuesInsert(fathersBrother, fathersBrother->right->data2);
			}
		}
	}
}

void ConcatenableQueue::delete_node(double val)
{
	node *nodeToDelete;
	nodeToDelete = new node;
	nodeToDelete = searchLeafNode(val, root);
	if ( nodeToDelete == NULL )
		cout << "The leaf with data " << val << " does not exist in the tree.\n";
	else
	{
		deleteNode(nodeToDelete);
		cout << "The leaf with data " << val << " was deleted from the tree.\n";
	}
	delete nodeToDelete;
}

bool ConcatenableQueue::is_empty() 
{
	if ( root == NULL )
		return true;
	else
		return false;
}

node *ConcatenableQueue::searchLeafNode(double val, node *current)
{
	if ( current->type == 0 )
		if ( current->data2 == val)
			return current;
		else
			return NULL;
	else if ( val <= current->data1 )
		return searchLeafNode(val, current->left);
	else if ( (val <= current->data2) && (current->middle != NULL) )
		return searchLeafNode(val, current->middle);
	else
		return searchLeafNode(val, current->right);
}

void ConcatenableQueue::addChild(node *v, node *aux, double val)
{
	node *v1, *aux1;
	double d1, d2;
	v1 = new node;
	if ( v->left->type == 0 )
	{
		v1 = create_2Node(aux->data2, v->right->data2, aux, v->right);
		v->biggest_y = v->middle;
	}
	else
	{
		//d1 = rightmostValue(aux);
		//d2 = rightmostValue(v->right);
		//v1 = create_2Node(d1, d2, aux, v->right);
		v1 = create_2Node(aux->biggest_y->data2, v->right->biggest_y->data2, aux, v->right);
		v->biggest_y = v->middle->biggest_y;
	}
	v1->left->parent = v1;
	v1->right->parent = v1;
	v->right = v->middle;
	v->middle = NULL;
	v->type = 1;
	if ( v->parent == NULL )
	{
		node *newRoot;
		newRoot = new node;
		newRoot = create_2Node(root->data2, v1->data2, v, v1);
		root = newRoot;
		v->parent = root;
		v1->parent = root;
	}
	else
	{
		v1->parent = v->parent;
		if ( v->parent->type == 1 )
		{
			if (v == v->parent->left)
			{
				v->parent->middle = v1;
				v->parent->data2 = v1->data2;
				v->parent->data1 = v->data2;
			}
			else
			{
				v->parent->middle = v;
				v->parent->right = v1;
				v->parent->data2 = v->data2;
			}
			v->parent->type = 2;
		}
		else
		{
			if ( v == v->parent->left)
			{
				aux1 = v->parent->middle;
				v->parent->middle = v1;
			}
			else
				if ( v == v->parent->middle )
					aux1 = v1;
				else
				{
					aux1 = v;
					v->parent->right = v1;
				}

			if ( v->parent->left->type == 0 )
			{
				v->parent->data1 = v->parent->left->data2;
				v->parent->data2 = v->parent->middle->data2;
			}
			else
			{
				v->parent->data1 = v->parent->left->biggest_y->data2;
				v->parent->data2 = v->parent->middle->biggest_y->data2;   
			}
			//v->parent->data1 = rightmostValue(v->parent->left);
			//v->parent->data2 = rightmostValue(v->parent->middle);
			addChild(v->parent, aux1, val);
		}
	}
}

node *ConcatenableQueue::searchForInsert(node *newNode, node *r)
{
	if ( newNode->data2 < r->smallest_y->data2 )
		r->smallest_y = newNode;
	else if ( newNode->data2 > r->biggest_y->data2 )
		r->biggest_y = newNode;

	if ( r->left->type == 0 )
		return r;
	else
	{
		if ( newNode->data2 <= r->data1)
			return searchForInsert(newNode, r->left);
		else
			if ((r->middle != NULL) && (newNode->data2 <= r->data2))
				return searchForInsert(newNode, r->middle);
			else
				return searchForInsert(newNode, r->right);
	}
}

node *ConcatenableQueue::create_LeafNode(double xcoord, double ycoord)
{
	node *newNode;
	newNode = new node;
	newNode->data1 = -1;
	newNode->data2 = ycoord;
	newNode->x_coord = xcoord;
	newNode->type = 0;
	newNode->left = NULL;
	newNode->middle = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
	return newNode;
}

node *ConcatenableQueue::create_2Node(double lValue, double mValue, node *leftSon, node *rightSon)
{
	node *newNode;
	newNode = new node;
	newNode->data1 = lValue;
	newNode->data2 = mValue;
	newNode->type = 1;
	newNode->left = leftSon;
	newNode->middle = NULL;
	newNode->right = rightSon;
	newNode->parent = NULL;
	if ( leftSon->type == 0 )
	{
		newNode->smallest_y = leftSon;
		newNode->biggest_y = rightSon;
	}
	else
	{
		newNode->smallest_y = leftSon->smallest_y;
		newNode->biggest_y = rightSon->biggest_y;
	}
	return newNode;
}

void ConcatenableQueue::add_node(double xcoord, double ycoord) 
{
	node *newNode, *aux;
	newNode = new node;
	if ( root == NULL)
	{
		newNode = create_LeafNode(xcoord, ycoord);
		root = newNode;
	}
	else
		if ( root->type == 0 )
		{
			node *newNode1;
			node *newNode2;
			newNode1 = new node;
			newNode2 = new node;
			newNode1 = create_LeafNode(xcoord, ycoord);
			newNode2 = create_LeafNode(root->x_coord,root->data2);
			if ( ycoord < root->data2 )
				newNode = create_2Node(ycoord, root->data2, newNode1, newNode2);
			else
				newNode = create_2Node(root->data2, ycoord, newNode2, newNode1);
			root = newNode;
			newNode1->parent = root;
			newNode2->parent = root;
	}
	else
	{
		node *father;
		father = new node;
		newNode = create_LeafNode(xcoord, ycoord);
		father = searchForInsert(newNode, root);
		if ( father->type == 1 )
		{
			newNode->parent = father;
			if ( (father->data1 <= ycoord) && (ycoord <= father->data2) )
			{
				father->middle = newNode;
				father->data2 = ycoord;
			}
		else
			if ( father->data1 > ycoord )
			{
				father->middle = father->left;
				father->left = newNode;
				father->data1 = ycoord;
				father->data2 = father->middle->data2;
			}
			else
			{
				father->middle = father->right;
				father->right = newNode;
				father->data2 = father->middle->data2;
			}
			father->type = 2;
			updateValuesInsert(father, father->right->data2);
		}
		else
		{
			newNode->parent = father;
			if ( newNode->data2 > father->data2 )
				if ( newNode->data2 > father->right->data2 )
				{
					aux = father->right;
					father->right = newNode;
				}
				else
					aux = newNode;
			else
			{
				aux = father->middle;
				if ( newNode->data2 > father->data1 )
					father->middle = newNode;
				else
				{
					father->middle = father->left;
					father->left = newNode;
				}
			}
			father->data1 = father->left->data2;
			father->data2 = father->middle->data2;
			addChild(father, aux, ycoord);
		}
	}
}

void ConcatenableQueue::print()
{
	if ( is_empty() )
		cout << "Empty Queue\n";
	else
		printValues(root, 0);
}

void ConcatenableQueue::printValues(node *printNode, int indent)
{
	int i;
	if ( printNode != NULL )
	{
		if ( printNode->type == 0)
			cout << "(" << printNode->x_coord << ", " << printNode->data2 << "), ";
		printValues(printNode->left, indent+1);	
		printValues(printNode->middle, indent+1);
		printValues(printNode->right, indent+1);
	}
}
	

void ConcatenableQueue::search(double val)
{
	node *res;
	res = new node;
	res = searchLeafNode(val, root);
	if (res == NULL)
		cout << val << " is not a data of the tree\n";
	else
		cout << res->data2 << " was found\n";
}

//Procedures concatenate and split concatenable queues
ConcatenableQueue concatenate(ConcatenableQueue CQ1, ConcatenableQueue CQ2)
{
	ConcatenableQueue newCQ;
	int heightCQ1, heightCQ2, level;
	node *attachNode, *currentNode, *aux;
	if ( CQ1.is_empty() )
		return CQ2;
	else if ( CQ2.is_empty() )
		return CQ1;
	else
	{
		heightCQ1 = CQ1.height();
		heightCQ2 = CQ2.height();
		if ( (heightCQ1 == heightCQ2) && (heightCQ1 > 0) )
		{
			node *newRoot;
			newRoot = new node;
			newRoot->left = CQ1.root;
			newRoot->left->parent = newRoot;
			newRoot->right = CQ2.root;
			newRoot->right->parent = newRoot;
			newRoot->type = 1;
			if ( CQ1.root->type != 0 )
			{
				newRoot->data1 = CQ1.root->biggest_y->data2;
				newRoot->data2 = CQ2.root->biggest_y->data2;
				newRoot->smallest_y = CQ1.root->smallest_y;
				newRoot->biggest_y = CQ2.root->biggest_y;
			}
			else
			{
				newRoot->smallest_y = CQ1.root;
				newRoot->biggest_y = CQ2.root;
				newRoot->data1 = CQ1.root->data2;
				newRoot->data2 = CQ2.root->data2;
			}
			newCQ.root = newRoot;
			newCQ.root->middle = NULL;
		}
		else if ( heightCQ1 > heightCQ2 )
		{
			newCQ = CQ1;
			newCQ.root->parent = NULL;
			level = heightCQ1-heightCQ2;
			if ( CQ2.root->type != 0 )
				attachNode = newCQ.rightmostNodeAtLevel(level, CQ2.root->biggest_y);
			else
				attachNode = newCQ.rightmostNodeAtLevel(level, CQ2.root);
			if ( attachNode->type == 1 )
			{
				attachNode->type = 2;
				attachNode->middle = attachNode->right;
				attachNode->right = CQ2.root;
				attachNode->right->parent = attachNode;
				currentNode = attachNode->parent;
				while ( currentNode != NULL )
				{
					if ( currentNode->type == 1 )
					{
						if ( CQ2.root->type == 0 )
							currentNode->data2 = CQ2.root->data2;
						else
							currentNode->data2 = CQ2.root->biggest_y->data2;
					}
					currentNode = currentNode->parent;
				}
			}
			else if ( attachNode->type == 2 )
			{
				aux = attachNode->right;
				attachNode->right = CQ2.root;
				attachNode->right->parent = attachNode;
				if ( attachNode == CQ1.root )
					attachNode->parent = NULL;
				newCQ.addChild(attachNode, aux, attachNode->right->data2);
			}
		}
		else
		{
			newCQ = CQ2;
			newCQ.root->parent = NULL;
			level = heightCQ2-heightCQ1;
			if ( CQ1.root->type != 0 )
				attachNode = newCQ.leftmostNodeAtLevel(level, CQ1.root->smallest_y);
			else
				attachNode = newCQ.leftmostNodeAtLevel(level, CQ1.root);
			if ( attachNode->type == 1 )
			{
				attachNode->type = 2;
				attachNode->middle = attachNode->left;
				attachNode->left = CQ1.root;
				attachNode->left->parent = newCQ.root;
				if ( CQ1.root->type == 0 )
				{
					attachNode->data1 = attachNode->left->data2;
					attachNode->data2 = attachNode->middle->data2;
				}
				else
				{
					attachNode->data1 = attachNode->left->biggest_y->data2;
					attachNode->data2 = attachNode->middle->biggest_y->data2;
				}
			}
			else if ( attachNode->type == 2 )
			{
				aux = attachNode->middle;
				attachNode->middle = attachNode->left;
				attachNode->left = CQ1.root;
				attachNode->left->parent = attachNode;
				attachNode->data2 = attachNode->data1;
				if ( CQ1.root->type == 0 )
					attachNode->data1 = attachNode->left->data2;
				else
					attachNode->data1 = attachNode->left->biggest_y->data2;
				if ( attachNode == CQ2.root )
					attachNode->parent = NULL;
				newCQ.addChild(attachNode, aux, attachNode->left->data2);
			}
		}
		return newCQ;
	}
}

//split_left, CQ1 keeps the node with value val 
void split_left (ConcatenableQueue CQ, ConcatenableQueue *CQ1, ConcatenableQueue *CQ2, double val)
{
	ConcatenableQueue cqr1, cqr2, cqr, cql1, cql2, cql, aux1, aux2;
	node *currentNode;
	currentNode = CQ.root;
	while ( currentNode->type != 0 )
	{
		if ( val <= currentNode->data1 )
		{
			cqr1.root = currentNode->right;
			cqr1.root->parent = NULL;
			if ( currentNode->type == 1 )
			{
				if ( cqr.root == NULL )
					cqr = cqr1;
				else
				{
					aux1 = cqr;
					cqr = concatenate(cqr1, aux1);
				}
			}
			else
			{
				cqr2.root = currentNode->middle;
				cqr2.root->parent = NULL;
				if ( cqr.root == NULL )
					cqr = concatenate(cqr2, cqr1);
				else
				{
					aux1 = cqr;
					aux2 = concatenate(cqr2, cqr1);
					cqr = concatenate(aux2, aux1);
				}
			}
			currentNode = currentNode->left;
		}
		else if( (currentNode->type == 2) && (val <= currentNode->data2) )
		{
			cql1.root = currentNode->left;
			cql1.root->parent = NULL;
			cqr1.root = currentNode->right;
			cqr1.root->parent = NULL;
			if ( cqr.root == NULL )
				cqr = cqr1;
			else
			{
				aux1 = cqr;
				cqr = concatenate(cqr1, aux1);
			}
			aux1 = cql;
			if ( cql.root = NULL )
				cql = cql1;
			else
				cql = concatenate(aux1, cql1);
			currentNode = currentNode->middle;
		}
		else
		{
			cql1.root = currentNode->left;
			cql1.root->parent = NULL; 
			if ( currentNode->type == 1 )
			{
				if ( cql.root == NULL )
					cql = cql1;
				else
				{
					aux1 = cql;
					cql = concatenate(aux1, cql1);
				}
			}
			else
			{
				cql2.root = currentNode->middle;
				cql2.root->parent = NULL;
				if ( cql.root == NULL )
					cql = concatenate(cql1, cql2);
				else
				{
					aux1 = cql;
					aux2 = concatenate(cql1, cql2);
					cql = concatenate(aux1, aux2);
				}
			}
			currentNode = currentNode->right;
		}
	}
	if ( cql.root == NULL) 
	{
		cql.root = currentNode;
		cql.root->parent = NULL;
	}
	else
	{
		aux1.root = currentNode;
		aux1.root->parent = NULL;
		aux2 = cql;
		cql = concatenate(aux2, aux1);
	}
	*CQ1 = cql;
	*CQ2 = cqr;
}

//split_right, CQ2 keeps the node with value val 
void split_right (ConcatenableQueue CQ, ConcatenableQueue *CQ1, ConcatenableQueue *CQ2, double val)
{
	ConcatenableQueue cqr1, cqr2, cqr, cql1, cql2, cql, aux1, aux2;
	node *currentNode = CQ.root;
	while ( currentNode->type != 0 )
	{
		if ( val <= currentNode->data1 )
		{
			cqr1.root = currentNode->right;
			cqr1.root->parent = NULL;
			if ( currentNode->type == 1 )
			{
				if ( cqr.root == NULL )
					cqr.root = 	cqr1.root;
				else
				{
					aux1.root = cqr.root;
					cqr = concatenate(cqr1, aux1);
				}
			}
			else
			{
				cqr2.root = currentNode->middle;
				cqr2.root->parent = NULL;
				if ( cqr.root == NULL )
					cqr = concatenate(cqr2, cqr1);
				else
				{
					//aux1 = cqr;
					aux1.root = cqr.root;
					aux2 = concatenate(cqr2, cqr1);
					cqr = concatenate(aux2, aux1);
				}
			}
			currentNode = currentNode->left;
		}
		else if( (currentNode->type == 2) && (val <= currentNode->data2) )
		{
			cql1.root = currentNode->left;
			cql1.root->parent = NULL;
			cqr1.root = currentNode->right;
			cqr1.root->parent = NULL;
			if ( cqr.root == NULL )
				//cqr = cqr1;
				cqr.root = cqr1.root;
			else
			{
				aux1.root = cqr.root;
				//aux1 = cqr;
				cqr = concatenate(cqr1, aux1);
			}
			aux1 = cql;
			aux1.root = cql.root;
			if ( cql.root = NULL )
			//cql = cql1;
				cql.root = cql1.root;
			else
				cql = concatenate(aux1, cql1); 
			currentNode = currentNode->middle;
		}
		else
		{
			cql1.root = currentNode->left;
			cql1.root->parent = NULL;
			if ( currentNode->type == 1 )
			{
				if ( cql.root == NULL )
					//cql = cql1;
					cql.root = cql1.root;
				else
				{
					//aux1 = cql;
					aux1.root = cql.root;
					cql = concatenate(aux1, cql1);
				}
			}
			else
			{
				cql2.root = currentNode->middle;
				cql2.root->parent = NULL;
				if ( cql.root == NULL )
					cql = concatenate(cql1, cql2);
				else
				{
					aux1.root = cql.root;
					//aux1 = cql;
					aux2 = concatenate(cql1, cql2);
					cql = concatenate(aux1, aux2);
				}
			}
			currentNode = currentNode->right;
		}
	}
	if ( cqr.root == NULL) 
	{
		cqr.root = currentNode;
		cqr.root->parent = NULL;
	}
	else
	{
		aux1.root = currentNode;
		aux1.root->parent = NULL;
		aux2 = cqr;
		cqr = concatenate(aux1, aux2);
	}
	*CQ1 = cql;
	*CQ2 = cqr;
}
