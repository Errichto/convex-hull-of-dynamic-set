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

convexHullr_OVL:	updatehull.cc cqueue.h toplevel_tree.h
	g++ -o convexHullr_OVL updatehull.cc

all:
	${MAKE} convexHullr_OVL

clean:
	rm -f convexHullr_OVL
