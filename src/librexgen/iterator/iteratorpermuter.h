/*
    rexgen - a tool to create words based on regular expressions    
    Copyright (C) 2012-2013  Jan Starke <jan.starke@outofbed.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the Free
    Software Foundation; either version 2 of the License, or (at your option)
    any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
    more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin St, Fifth Floor, Boston, MA 02110, USA
*/


#ifndef ITERATORPERMUTER_H
#define ITERATORPERMUTER_H

#include <librexgen/iterator/iterator.h>
#include <librexgen/unicode.h>
#include <vector>
#include <algorithm>

using namespace std;

class Regex;

class IteratorPermuter : public Iterator
{

public:
  void value(SimpleString& dst) const;
    bool hasNext() const;
    IteratorPermuter(int _id, const Regex* re, IteratorState* is, unsigned int min, unsigned int max);
    ~IteratorPermuter();
    bool next();
    void init();
    void updateReferences(IteratorState* iterState);
private:
  bool existsIteratorWithNextElement() const;
  
  const unsigned int min_occurs, max_occurs;
  const Regex* regex;
  vector<Iterator *> iterators;
  IteratorState* iteratorState;
  bool hasNextElement;
  
  unsigned int current;
  unsigned int occurs;
};


#endif // ITERATORPERMUTER_H
