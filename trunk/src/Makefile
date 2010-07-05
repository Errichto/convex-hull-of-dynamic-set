# $Id$
# 
# Copyright (C) 2007 Jose Alberto Cisneros Perez. All rights reserved.
#
# This file may be used under the terms of the GNU General Public License
# versions 3.0 as published by the Free Software Foundation and
# appearing in the COPYING file included in the packaging of this file.
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#

convexHullr_OVL:	updatehull.cc cqueue.o toplevel_tree.o
	g++ -o convexHullr_OVL updatehull.cc cqueue.o toplevel_tree.o

cqueue.o: cqueue.h cqueue.cc
	g++ -c cqueue.cc -o cqueue.o

toplevel_tree.o: toplevel_tree.h toplevel_tree.cc
	g++ -c toplevel_tree.cc -o toplevel_tree.o

all:
	${MAKE} convexHullr_OVL

clean:
	rm -f convexHullr_OVL cqueue.o toplevel_tree.o
