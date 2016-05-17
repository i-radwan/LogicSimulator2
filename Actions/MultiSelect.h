#ifndef _MULTI_SELECT
#define _MULTI_SELECT
#include "Action.h"
class MultiSelect :
	public Action
{
	vector<pair<int, Component*> > allSelectedComponents;

public:
	MultiSelect(ApplicationManager* pManager);
	virtual bool ReadActionParameters(image* img = NULL);
	virtual void Execute();
	//To undo this action (code depends on action type)
	virtual void Undo();
	//To redo this action (code depends on action type)
	virtual void Redo();
	virtual ~MultiSelect();
};

#endif