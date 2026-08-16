#ifndef CLMACHINEVIEWSTRUCT_H
#define CLMACHINEVIEWSTRUCT_H

#undef slots
#include <Python.h>
#define slots Q_SLOTS

#include <algorithm>
#include <exception>
#include <string>
#include <iostream>

using namespace std;

	struct struct_method
	{
		QString name;
		QString source;
		QString path;
	};
	
	struct struct_widget
	{
		QString name;
		QString row;
		QString coll;
		QString object_id;
		QString object_class;
		QString width;
		QString height;
		QString value;
		QString refresh;
		vector <struct_method> methods;
	};

#endif
