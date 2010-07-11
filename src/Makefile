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

CXX=g++
CXX_FLAGS=-Wall

SOURCES= cqueue.h \
	toplevel_tree.h \
	cqueue.cc \
	toplevel_tree.cc

OBJS=$(SOURCES:%.cc=%.o)

convexHull_OVL_SOURCES=updatehull.cc
convexHull_OVL_OBJS=$(convexHull_OVL_SOURCES:%.cc=%.o)


convexHull_OVL:	$(convexHull_OVL_OBJS) libconvexHull.a
	g++ -o convexHull_OVL $(convexHull_OVL_OBJS) libconvexHull.a

libconvexHull.a:	$(OBJS)
	ar rc libconvexHull.a $(OBJS)
	ranlib libconvexHull.a

all:
	${MAKE} convexHull_OVL

.cc.o:
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -f convexHull_OVL *.o libconvexHull.a
