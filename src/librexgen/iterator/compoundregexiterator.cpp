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


#include <librexgen/iterator/compoundregexiterator.h>
#include <librexgen/debug.h>
#include <librexgen/unicode.h>
#include <assert.h>
#include <algorithm>
#include <cstdlib>
#include <set>

CompoundRegexIterator::CompoundRegexIterator(int _id, bool rnd)
  : Iterator(_id), randomize(rnd) {

  if (randomize) {
    for (unsigned int n = 0; n<iterators.size(); ++n) {
      rnd_iterators.push_back(n);
    }
    shuffle();
  }
}

CompoundRegexIterator::~CompoundRegexIterator() {
  for_each(iterators.begin(), iterators.end(),
    [](Iterator* i) { if (! i->isSingleton()) { delete i; } } );
}

void CompoundRegexIterator::updateReferences(IteratorState* iterState) {
  for_each(iterators.begin(), iterators.end(), [iterState](Iterator* i){i->updateReferences(iterState);});
}

bool CompoundRegexIterator::next() {
  if (state == resetted) { state = usable; return true; }
  if (randomize) {
    for (auto rnd_i = rnd_iterators.begin(); rnd_i != rnd_iterators.end(); ++rnd_i) {
      if (iterators[*rnd_i]->next()) {
        return true;
      }
    }
    return false;
  }
  for (auto i = iterators.begin(); i != iterators.end(); ++i) {
    if ((*i)->next()) {
      return true;
    }
  }
  return false;
}

void CompoundRegexIterator::value(SimpleString& dst) const {
  //assert(canUseValue());
  for_each(iterators.begin(), iterators.end(),
  [&dst](Iterator* i) { i->value(dst); });
}

bool CompoundRegexIterator::hasNext() const {
  bool has_next = false;
  if (state == not_usable) {
    return false;
  }
  for_each(iterators.begin(), iterators.end(), [&has_next](Iterator* i) {
    has_next |= i->hasNext();
  });
  return has_next;
}

void CompoundRegexIterator::addChild(Iterator* i) {
  rnd_iterators.push_back(iterators.size());
  if (randomize) {
    shuffle();
  }
  iterators.push_back(i);
  //i->reset();
  i->next();
}



