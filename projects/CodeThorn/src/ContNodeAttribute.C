#ifndef CONT_NODE_ATTRIBUTE_C
#define CONT_NODE_ATTRIBUTE_C

#include"sage3basic.h"

#include"ContNodeAttribute.h"
#include"Analyzer.h"
#include"Flow.h"
#include<sstream>

using namespace std;

string ContNodeAttribute::toString()
{
	stringstream s;
	s << contLabel;
	return s.str();
} 


#endif
