#ifndef _STGROUP_HPP_
#define _STGROUP_HPP_

#include <iostream>
#include <algorithm>

#include "basicEdgeGroup.h"

class STGroup
{
public:
  STGroup(void);
  ~STGroup(void);
  
  void findMinSpanningTree(const basicEdgeGroup& graph);
  
  const basicEdgeGroup& returnMinSpanningTree() const; 
  
protected:
  basicEdgeGroup resultantTree;
  
private:
  
};

#endif