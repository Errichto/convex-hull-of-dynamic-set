/* $Id$
 * 
 * Copyright (C) 2007 Jose Alberto Cisneros Perez. All rights reserved.
 *
 * This file may be used under the terms of the GNU General Public License
 * versions 3.0 as published by the Free Software Foundation and
 * appearing in the COPYING file included in the packaging of this file.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
*/


#include <iostream>
#include <fstream>
#include <ctime>

#include "toplevel_tree.h"


void printMenu()
{
	std::cout << "\n";
	std::cout << "1. Add a point\n";
	std::cout << "2. Delete a point\n";
	std::cout << "3. Exit\n";
	std::cout << ">> ";
}

int main(int argc, char *argv[])
{
	int i=1;
	char toptree, lc_rchulls;
	double time_dif;
	point p;
	Tree tree1;
	std::ifstream inFile, inFile1;
	inFile.open(argv[1]);
	inFile1.open(argv[2]);
	clock_t start, end;

	if (!inFile || !inFile1)
	{
	}
	else
	{
		while (inFile >> p.x_coord && inFile1 >> p.y_coord)
		{
			tree1.add_node(p);
			std::cout << i << " ";
			std::cout.flush();
			++i;
			//tree1.print();
			std::cout << "\n";
		}
		std::cout << "\n";
		std::cout << "See the structure of the top-level storing the current set y/n: ";
		std::cin >> toptree;
		if ( (toptree == 'y') || (toptree == 'Y') )
		{
			tree1.print();
			std::cout << "\n";
		}
		inFile.close();
		inFile1.close();
	}

	tree_node *root = tree1.root();
	if ( root == NULL )
		std::cout << "The current set is empty.\n ";
	else
	{
		root->Ql.print();
		std::cout << "\n";
		root->Qr.print();
		std::cout << "\n";
	}
	printMenu();
	while ( i != 3 )
	{
		std::cin >> i;
		//scanf("%d", &i);
		switch (i)
		{
			case 1: std::cout << "Enter the x-coordinate of the point: ";
						std::cin >> p.x_coord;
						std::cout << "Enter the y-coordinate of the point: ";
						std::cin >> p.y_coord;
						std::cout << "See the structure of the top level tree after and after each update y/n: ";
						std::cin >> toptree;
						std::cout << "See the lc/rc-hull splitting at each level of the tree while going down before the insertion y/n: ";
						std::cin >> lc_rchulls;
						if ( (toptree == 'y') || (toptree == 'Y') )
						{
								std::cout << "Top level tree before inserting the point (" << p.x_coord << ", " << p.y_coord << ")\n";
								tree1.print();
								std::cout << "\n";
						}
						if ( (lc_rchulls == 'y') || (lc_rchulls == 'Y') )
							tree1.set_show(true);
						else
							tree1.set_show(false);
						start=clock();
						tree1.add_node(p);
						end = clock();
						time_dif = end-start;
						std::cout << time_dif << " milliseconds spent updating the convex hull\n";
						std::cout << "The new convex hull is:\n";
						root = tree1.root();
						root->Ql.print();
						std::cout << "\n";
						root->Qr.print();
						std::cout << "\n";
						if ( (toptree == 'y') || (toptree == 'Y') )
						{
							std::cout << "Top level tree after inserting the point (" << p.x_coord << ", " << p.y_coord << ")\n";
							tree1.print();
							std::cout << "\n";
						}
						printMenu();
						break;
		case 2: std::cout << "Enter the x-coordinate of the point: ";
						std::cin >> p.x_coord;
						std::cout << "Enter the y-coordinate of the point: ";
						std::cin >> p.y_coord;
						std::cout << "Do you want to see the structure of the top level tree after and after each update y/n: ";
						std::cin >> toptree;
						std::cout << "See the lc/rc-hull splitting at each level of the tree while going down before the deletion y/n: ";
						std::cin >> lc_rchulls;
						if ( (toptree == 'y') || (toptree == 'Y') )
						{
							std::cout << "Top level tree before deleting the point (" << p.x_coord << ", " << p.y_coord << ")\n";
							tree1.print();
							std::cout << "\n";
						}
						if ( (lc_rchulls == 'y') || (lc_rchulls == 'Y') )
							tree1.set_show(true);
						else
							tree1.set_show(false);
						start=clock();
						tree1.delete_node(p);
						end=clock();
						time_dif = end-start;
						std::cout << time_dif << " milliseconds spent updating the convex hull\n";
						std::cout << "The new convex hull is:\n";
						root = tree1.root();
						root->Ql.print();
						std::cout << "\n";
						root->Qr.print();
						std::cout << "\n";
						if ( (toptree == 'y') || (toptree == 'Y') )
						{
							std::cout << "Top level tree after deleting the point (" << p.x_coord << ", " << p.y_coord << ")\n";
							tree1.print();
							std::cout << "\n";
						}
						printMenu();
						break;
		}
	}
}
