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

ConcatenableQueue::ConcatenableQueue() : root_(NULL)
{
}

ConcatenableQueue::ConcatenableQueue(const double &x_coord, const double &y_coord) : root_(NULL)
{
	addNode(x_coord, y_coord);
}

ConcatenableQueue ConcatenableQueue::clone()
{
	ConcatenableQueue newQueue;
	if (!isEmpty())
	{
		addQueuePointsToQueue(&newQueue, root_);
	}
	return newQueue;
}

void ConcatenableQueue::addQueuePointsToQueue(ConcatenableQueue *CQ, node *current_node)
{
	if (current_node != NULL)
	{
		if (current_node->type == LEAF_NODE)
			CQ->addNode(current_node->x_coord, current_node->data2);
 
		addQueuePointsToQueue(CQ, current_node->left);
		addQueuePointsToQueue(CQ, current_node->middle);
		addQueuePointsToQueue(CQ, current_node->right);
	}
	/*if (current_node->type == LEAF_NODE)
		return createLeafNode(current_node->x_coord, current_node->data2);
		else if (current_node->type == TWO_NODE)
			return create2Node(current_node->data1, current_node->data2, cloneNode(current_node->left), cloneNode(current_node->right));
 
	node *new_node = create2Node(current_node->data1, current_node->data2, cloneNode(current_node->left), cloneNode(current_node->right));
	new_node->middle = cloneNode(current_node->middle);
	return new_node;*/
}

node *ConcatenableQueue::rightmostNodeAtLevel(const int &level, node *bigger_y)
{
	node *currentNode;
	currentNode = root_;
	currentNode->biggest_y =  bigger_y;
	for (int i=1; i<level; ++i)
	{
		currentNode = currentNode->right;
		currentNode->biggest_y =  bigger_y;
	}
	return currentNode;
}

node *ConcatenableQueue::leftmostNodeAtLevel(const int &level, node *smaller_y)
{
	node *currentNode;
	currentNode = root_;
	currentNode->smallest_y = smaller_y;
	for (int i=1; i<level; i++)
	{
		currentNode = currentNode->left;
		currentNode->smallest_y = smaller_y;
	}
	return currentNode;
}

int ConcatenableQueue::height() const
{
	int h = 0;
	node *currentNode;
	currentNode = root_;
	while ( currentNode != NULL )
	{
		++h;
		currentNode = currentNode->right;
	}
	return h;
}

void ConcatenableQueue::updateValuesDelete(node *current_node, const double &current_value, const double &new_value)
{
	while ( (current_node != root_) && (current_node != NULL) )
	{
		if ( (current_node == current_node->parent->left) && (current_node->parent->data1 == current_value) )
		{
			current_node->parent->data1 = new_value;
		}
		else if ( (current_node == current_node->parent->middle) && (current_node->parent->data2 == current_value) )
		{
			current_node->parent->data2 = new_value;
		}
		else if ( (current_node == current_node->parent->right) &&
							(current_node->parent->middle == NULL) &&
							(current_node->parent->data2 == current_value) )
		{
			current_node->parent->data2 = new_value;
		}
		current_node = current_node->parent;
	}
}

void ConcatenableQueue::updateValuesInsert(node *current_node, double value)
{
	while ( current_node != root_ )
	{
		if ( (current_node == current_node->parent->left) && (current_node->parent->data1 < value) )
			current_node->parent->data1 = value;
		else if ( (current_node == current_node->parent->middle) && (current_node->parent->data2 < value) )
			current_node->parent->data2 = value;
		else if ( (current_node == current_node->parent->right) && 
							(current_node->parent->middle == NULL) && 
							(current_node->parent->data2 < value) )
			current_node->parent->data2 = value;
		current_node = current_node->parent;
		value = current_node->data2;
	}
}

void ConcatenableQueue::deleteNode(node *node_to_delete)
{
	node *father, *brother, *fathersBrother;
	double auxValue1, auxValue2;
	bool leftbrother;
	if ( node_to_delete == root_ )
		root_ = NULL;
	else if ( (node_to_delete->parent == root_) && (root_->type == TWO_NODE) )
	{
		if ( node_to_delete == root_->left )
		{
			root_->right->parent = NULL;
			root_ = root_->right;
		}
		else
		{
			root_->left->parent = NULL;
			root_ = root_->left;
		}
	}
	else
	{
		father = new node;
		father = node_to_delete->parent;
		if ( father->type == THREE_NODE )
		{
			if ( father->left == node_to_delete )
			{
				father->left = father->middle;
			}
			else if ( father->right == node_to_delete )
				father->right = father->middle;
			father->middle = NULL;
			father->type = TWO_NODE;
			father->data1 = father->left->data2;
			father->data2 = father->right->data2;
			updateValuesDelete(father, node_to_delete->data2, father->data2);
		}
		else
		{
			brother = new node;
			fathersBrother = new node;
			if ( father->left == node_to_delete )
				brother = father->right;
			else
				brother = father->left;
			if ( father->parent->right == father )
			{
				if ( father->parent->type == THREE_NODE )
					fathersBrother = father->parent->middle;
				else
					fathersBrother = father->parent->left;
				leftbrother = true;
			}
			else
				if ( father->parent->left == father )
				{
					if ( father->parent->type == THREE_NODE )
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
			if ( fathersBrother->type == THREE_NODE )
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
					fathersBrother->type = TWO_NODE;
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
					fathersBrother->type = TWO_NODE;
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
				fathersBrother->type = THREE_NODE;
				father->right = NULL;
				father->left = NULL;
				deleteNode(father);
				updateValuesInsert(fathersBrother, fathersBrother->right->data2);
			}
		}
	}
}

void ConcatenableQueue::deleteNode(const double &value)
{
	node *node_to_delete = searchLeafNode(value, root_);
	if ( node_to_delete == NULL )
	{
		std::cout << "The leaf with data " << value << " does not exist in the tree.\n";
	}
	else
	{
		deleteNode(node_to_delete);
		std::cout << "The leaf with data " << value << " was deleted from the tree.\n";
	}
	delete node_to_delete;
}

bool ConcatenableQueue::isEmpty() const
{
	if ( root_ == NULL )
		return true;
	else
		return false;
}

node *ConcatenableQueue::searchLeafNode(const double &value, node *current)
{
	if ( current->type == LEAF_NODE )
	{
		if ( current->data2 == value)
			return current;
		else
			return NULL;
	}
	else if ( value <= current->data1 )
	{
		return searchLeafNode(value, current->left);
	}
	else if ( (value <= current->data2) && (current->middle != NULL) )
	{
		return searchLeafNode(value, current->middle);
	}
	return searchLeafNode(value, current->right);
}

void ConcatenableQueue::addChildNode(node *child, node *aux, const double &value)
{
	node *v1, *aux1;
	v1 = new node;
	if ( child->left->type == LEAF_NODE )
	{
		v1 = create2Node(aux->data2, child->right->data2, aux, child->right);
		child->biggest_y = child->middle;
	}
	else
	{
		//d1 = rightmostValue(aux);
		//d2 = rightmostValue(v->right);
		//v1 = create2Node(d1, d2, aux, v->right);
		v1 = create2Node(aux->biggest_y->data2, child->right->biggest_y->data2, aux, child->right);
		child->biggest_y = child->middle->biggest_y;
	}
	v1->left->parent = v1;
	v1->right->parent = v1;
	child->right = child->middle;
	child->middle = NULL;
	child->type = TWO_NODE;
	if ( child->parent == NULL )
	{
		node *newRoot = create2Node(root_->data2, v1->data2, child, v1);
		root_ = newRoot;
		child->parent = root_;
		v1->parent = root_;
	}
	else
	{
		v1->parent = child->parent;
		if ( child->parent->type == TWO_NODE )
		{
			if (child == child->parent->left)
			{
				child->parent->middle = v1;
				child->parent->data2 = v1->data2;
				child->parent->data1 = child->data2;
			}
			else
			{
				child->parent->middle = child;
				child->parent->right = v1;
				child->parent->data2 = child->data2;
			}
			child->parent->type = THREE_NODE;
		}
		else
		{
			if ( child == child->parent->left)
			{
				aux1 = child->parent->middle;
				child->parent->middle = v1;
			}
			else
			{
				if ( child == child->parent->middle )
				{
					aux1 = v1;
				}
				else
				{
					aux1 = child;
					child->parent->right = v1;
				}
			}
			if ( child->parent->left->type == LEAF_NODE )
			{
				child->parent->data1 = child->parent->left->data2;
				child->parent->data2 = child->parent->middle->data2;
			}
			else
			{
				child->parent->data1 = child->parent->left->biggest_y->data2;
				child->parent->data2 = child->parent->middle->biggest_y->data2;   
			}
			//v->parent->data1 = rightmostValue(v->parent->left);
			//v->parent->data2 = rightmostValue(v->parent->middle);
			addChildNode(child->parent, aux1, value);
		}
	}
}

node *ConcatenableQueue::searchForInsert(node *new_node, node *current_node)
{
	if ( new_node->data2 < current_node->smallest_y->data2 )
		current_node->smallest_y = new_node;
	else if ( new_node->data2 > current_node->biggest_y->data2 )
		current_node->biggest_y = new_node;

	if ( current_node->left->type == LEAF_NODE )
		return current_node;
	else
	{
		if ( new_node->data2 <= current_node->data1)
			return searchForInsert(new_node, current_node->left);
		else
			if ((current_node->middle != NULL) && (new_node->data2 <= current_node->data2))
				return searchForInsert(new_node, current_node->middle);
			else
				return searchForInsert(new_node, current_node->right);
	}
}

node *ConcatenableQueue::createLeafNode(const double &x_coord, const double &y_coord)
{
	node *newNode = new node;
	newNode->data1 = -1.0;
	newNode->data2 = y_coord;
	newNode->x_coord = x_coord;
	return newNode;
}

node *ConcatenableQueue::create2Node(const double &l_value, const double &m_value, node *left_child, node *right_child)
{
	node *newNode = new node;
	newNode->data1 = l_value;
	newNode->data2 = m_value;
	newNode->type = TWO_NODE;
	newNode->left = left_child;
	newNode->right = right_child;
	if ( left_child->type == LEAF_NODE )
	{
		newNode->smallest_y = left_child;
		newNode->biggest_y = right_child;
	}
	else
	{
		newNode->smallest_y = left_child->smallest_y;
		newNode->biggest_y = right_child->biggest_y;
	}
	return newNode;
}

void ConcatenableQueue::addNode(const double &x_coord, const double &y_coord) 
{
	node *newNode, *aux;
	newNode = new node;
	if ( root_ == NULL)
	{
		newNode = createLeafNode(x_coord, y_coord);
		root_ = newNode;
	}
	else
	{
		if ( root_->type == LEAF_NODE )
		{
			node *newNode1;
			node *newNode2;
			newNode1 = new node;
			newNode2 = new node;
			newNode1 = createLeafNode(x_coord, y_coord);
			newNode2 = createLeafNode(root_->x_coord,root_->data2);

			if ( y_coord < root_->data2 )
				newNode = create2Node(y_coord, root_->data2, newNode1, newNode2);
			else
				newNode = create2Node(root_->data2, y_coord, newNode2, newNode1);

			delete root_;
			root_ = NULL;

			root_ = newNode;
			newNode1->parent = root_;
			newNode2->parent = root_;
		}
		else
		{
			node *father;
			father = new node;
			newNode = createLeafNode(x_coord, y_coord);
			father = searchForInsert(newNode, root_);
			if ( father->type == TWO_NODE )
			{
				newNode->parent = father;
				if ( (father->data1 <= y_coord) && (y_coord <= father->data2) )
				{
					father->middle = newNode;
					father->data2 = y_coord;
				}
				else
				{
					if ( father->data1 > y_coord )
					{
						father->middle = father->left;
						father->left = newNode;
						father->data1 = y_coord;
						father->data2 = father->middle->data2;
					}
					else
					{
						father->middle = father->right;
						father->right = newNode;
						father->data2 = father->middle->data2;
					}
				}
				father->type = THREE_NODE;
				updateValuesInsert(father, father->right->data2);
			}
			else
			{
				newNode->parent = father;
				if ( newNode->data2 > father->data2 )
				{
					if ( newNode->data2 > father->right->data2 )
					{
						aux = father->right;
						father->right = newNode;
					}
					else
					{
						aux = newNode;
					}
				}
				else
				{
					aux = father->middle;
					if ( newNode->data2 > father->data1 )
					{
						father->middle = newNode;
					}
					else
					{
						father->middle = father->left;
						father->left = newNode;
					}
				}
				father->data1 = father->left->data2;
				father->data2 = father->middle->data2;
				addChildNode(father, aux, y_coord);
			}
		}
	}
}

void ConcatenableQueue::print()
{
	if ( isEmpty() )
		std::cout << "Empty Queue\n";
	else
		printValues(root_);
}

void ConcatenableQueue::printValues(node *current_node)
{
	if ( current_node != NULL )
	{
		if ( current_node->type == LEAF_NODE)
			std::cout << "(" << current_node->x_coord << ", " << current_node->data2 << "), ";
		printValues(current_node->left);	
		printValues(current_node->middle);
		printValues(current_node->right);
	}
}
	

void ConcatenableQueue::search(const double &value)
{
	node *res = searchLeafNode(value, root_);
	if (res == NULL)
		std::cout << value << " is not a data of the tree\n";
	else
		std::cout << res->data2 << " was found\n";
}

node *ConcatenableQueue::root()
{
	return root_;
}

void ConcatenableQueue::set_root(node *new_root)
{
	root_ = new_root;
}

//Procedures concatenate and split concatenable queues
ConcatenableQueue ConcatenableQueue::concatenate(ConcatenableQueue CQ1, ConcatenableQueue CQ2)
{
	ConcatenableQueue newCQ;
	int heightCQ1, heightCQ2, level;
	node *attachNode, *currentNode, *aux;
	if ( CQ1.isEmpty() )
		return CQ2;
	else if ( CQ2.isEmpty() )
		return CQ1;
	else
	{
		heightCQ1 = CQ1.height();
		heightCQ2 = CQ2.height();
		if ( (heightCQ1 == heightCQ2) && (heightCQ1 > 0) )
		{
			node *newRoot;
			newRoot = new node;
			newRoot->left = CQ1.root();
			newRoot->left->parent = newRoot;
			newRoot->right = CQ2.root();
			newRoot->right->parent = newRoot;
			newRoot->type = TWO_NODE;
			if ( CQ1.root()->type != LEAF_NODE )
			{
				newRoot->data1 = CQ1.root()->biggest_y->data2;
				newRoot->data2 = CQ2.root()->biggest_y->data2;
				newRoot->smallest_y = CQ1.root()->smallest_y;
				newRoot->biggest_y = CQ2.root()->biggest_y;
			}
			else
			{
				newRoot->smallest_y = CQ1.root();
				newRoot->biggest_y = CQ2.root();
				newRoot->data1 = CQ1.root()->data2;
				newRoot->data2 = CQ2.root()->data2;
			}
			newCQ.set_root(newRoot);
			newCQ.root()->middle = NULL;
		}
		else if ( heightCQ1 > heightCQ2 )
		{
			newCQ = CQ1;
			newCQ.root()->parent = NULL;
			level = heightCQ1-heightCQ2;
			if ( CQ2.root()->type != LEAF_NODE )
				attachNode = newCQ.rightmostNodeAtLevel(level, CQ2.root()->biggest_y);
			else
				attachNode = newCQ.rightmostNodeAtLevel(level, CQ2.root());
			if ( attachNode->type == TWO_NODE )
			{
				attachNode->type = THREE_NODE;
				attachNode->middle = attachNode->right;
				attachNode->right = CQ2.root();
				attachNode->right->parent = attachNode;
				currentNode = attachNode->parent;
				while ( currentNode != NULL )
				{
					if ( currentNode->type == TWO_NODE )
					{
						if ( CQ2.root()->type == LEAF_NODE )
							currentNode->data2 = CQ2.root()->data2;
						else
							currentNode->data2 = CQ2.root()->biggest_y->data2;
					}
					currentNode = currentNode->parent;
				}
			}
			else if ( attachNode->type == THREE_NODE )
			{
				aux = attachNode->right;
				attachNode->right = CQ2.root();
				attachNode->right->parent = attachNode;
				if ( attachNode == CQ1.root() )
					attachNode->parent = NULL;
				newCQ.addChildNode(attachNode, aux, attachNode->right->data2);
			}
		}
		else
		{
			newCQ = CQ2;
			newCQ.root()->parent = NULL;
			level = heightCQ2-heightCQ1;
			if ( CQ1.root()->type != LEAF_NODE )
				attachNode = newCQ.leftmostNodeAtLevel(level, CQ1.root()->smallest_y);
			else
				attachNode = newCQ.leftmostNodeAtLevel(level, CQ1.root());
			if ( attachNode->type == TWO_NODE )
			{
				attachNode->type = THREE_NODE;
				attachNode->middle = attachNode->left;
				attachNode->left = CQ1.root();
				attachNode->left->parent = newCQ.root();
				if ( CQ1.root()->type == LEAF_NODE )
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
			else if ( attachNode->type == THREE_NODE )
			{
				aux = attachNode->middle;
				attachNode->middle = attachNode->left;
				attachNode->left = CQ1.root();
				attachNode->left->parent = attachNode;
				attachNode->data2 = attachNode->data1;
				if ( CQ1.root()->type == LEAF_NODE )
					attachNode->data1 = attachNode->left->data2;
				else
					attachNode->data1 = attachNode->left->biggest_y->data2;
				if ( attachNode == CQ2.root() )
					attachNode->parent = NULL;
				newCQ.addChildNode(attachNode, aux, attachNode->left->data2);
			}
		}
		return newCQ;
	}
}

//split_left, CQ1 keeps the node with value val 
void ConcatenableQueue::split_left (ConcatenableQueue CQ, ConcatenableQueue *CQ1, ConcatenableQueue *CQ2, const double &split_value)
{
	ConcatenableQueue cqr1, cqr2, cqr, cql1, cql2, cql, aux1, aux2;
	node *currentNode;
	currentNode = CQ.root();
	while ( currentNode->type != LEAF_NODE )
	{
		if ( split_value <= currentNode->data1 )
		{
			cqr1.set_root(currentNode->right);
			cqr1.root()->parent = NULL;
			if ( currentNode->type == TWO_NODE )
			{
				if ( cqr.root() == NULL )
					cqr = cqr1;
				else
				{
					aux1 = cqr;
					cqr = concatenate(cqr1, aux1);
				}
			}
			else
			{
				cqr2.set_root(currentNode->middle);
				cqr2.root()->parent = NULL;
				if ( cqr.root() == NULL )
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
		else if( (currentNode->type == THREE_NODE) && (split_value <= currentNode->data2) )
		{
			cql1.set_root(currentNode->left);
			cql1.root()->parent = NULL;
			cqr1.set_root(currentNode->right);
			cqr1.root()->parent = NULL;
			if ( cqr.root() == NULL )
				cqr = cqr1;
			else
			{
				aux1 = cqr;
				cqr = concatenate(cqr1, aux1);
			}
			aux1 = cql;
			if ( cql.root() == NULL )
				cql = cql1;
			else
				cql = concatenate(aux1, cql1);
			currentNode = currentNode->middle;
		}
		else
		{
			cql1.set_root(currentNode->left);
			cql1.root()->parent = NULL; 
			if ( currentNode->type == TWO_NODE )
			{
				if ( cql.root() == NULL )
					cql = cql1;
				else
				{
					aux1 = cql;
					cql = concatenate(aux1, cql1);
				}
			}
			else
			{
				cql2.set_root(currentNode->middle);
				cql2.root()->parent = NULL;
				if ( cql.root() == NULL )
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
	if ( cql.root() == NULL) 
	{
		cql.set_root(currentNode);
		cql.root()->parent = NULL;
	}
	else
	{
		aux1.set_root(currentNode);
		aux1.root()->parent = NULL;
		aux2 = cql;
		cql = concatenate(aux2, aux1);
	}
	*CQ1 = cql;
	*CQ2 = cqr;
}

//split_right, CQ2 keeps the node with value val 
void ConcatenableQueue::split_right (ConcatenableQueue CQ, ConcatenableQueue *CQ1, ConcatenableQueue *CQ2, const double &split_value)
{
	ConcatenableQueue cqr1, cqr2, cqr, cql1, cql2, cql, aux1, aux2;
	node *currentNode = CQ.root();
	while ( currentNode->type != LEAF_NODE )
	{
		if ( split_value <= currentNode->data1 )
		{
			cqr1.set_root(currentNode->right);
			cqr1.root()->parent = NULL;
			if ( currentNode->type == TWO_NODE )
			{
				if ( cqr.root() == NULL )
					cqr.set_root(cqr1.root());
				else
				{
					aux1.set_root(cqr.root());
					cqr = concatenate(cqr1, aux1);
				}
			}
			else
			{
				cqr2.set_root(currentNode->middle);
				cqr2.root()->parent = NULL;
				if ( cqr.root() == NULL )
					cqr = concatenate(cqr2, cqr1);
				else
				{
					//aux1 = cqr;
					aux1.set_root(cqr.root());
					aux2 = concatenate(cqr2, cqr1);
					cqr = concatenate(aux2, aux1);
				}
			}
			currentNode = currentNode->left;
		}
		else if( (currentNode->type == THREE_NODE) && (split_value <= currentNode->data2) )
		{
			cql1.set_root(currentNode->left);
			cql1.root()->parent = NULL;
			cqr1.set_root(currentNode->right);
			cqr1.root()->parent = NULL;
			if ( cqr.root() == NULL )
			{
				//cqr = cqr1;
				cqr.set_root(cqr1.root());
			}
			else
			{
				aux1.set_root(cqr.root());
				//aux1 = cqr;
				cqr = concatenate(cqr1, aux1);
			}
			aux1 = cql;
			aux1.set_root(cql.root());
			if ( cql.root() == NULL )
			//cql = cql1;
				cql.set_root(cql1.root());
			else
				cql = concatenate(aux1, cql1); 
			currentNode = currentNode->middle;
		}
		else
		{
			cql1.set_root(currentNode->left);
			cql1.root()->parent = NULL;
			if ( currentNode->type == TWO_NODE )
			{
				if ( cql.root() == NULL )
				{
					//cql = cql1;
					cql.set_root(cql1.root());
				}
				else
				{
					//aux1 = cql;
					aux1.set_root(cql.root());
					cql = concatenate(aux1, cql1);
				}
			}
			else
			{
				cql2.set_root(currentNode->middle);
				cql2.root()->parent = NULL;
				if ( cql.root() == NULL )
				{
					cql = concatenate(cql1, cql2);
				}
				else
				{
					aux1.set_root(cql.root());
					//aux1 = cql;
					aux2 = concatenate(cql1, cql2);
					cql = concatenate(aux1, aux2);
				}
			}
			currentNode = currentNode->right;
		}
	}
	if ( cqr.root() == NULL) 
	{
		cqr.set_root(currentNode);
		cqr.root()->parent = NULL;
	}
	else
	{
		aux1.set_root(currentNode);
		aux1.root()->parent = NULL;
		aux2 = cqr;
		cqr = concatenate(aux1, aux2);
	}
	*CQ1 = cql;
	*CQ2 = cqr;
}
