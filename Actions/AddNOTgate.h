#ifndef NOT_Header
#define NOT_Header
#include"Action.h"
#include"..\Components\NOT.h"
class AddNOTgate:public Action
{
public:
	AddNOTgate(ApplicationManager *pApp);
	virtual ~AddNOTgate(void);

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * , Component*);
	//Execute action (code depends on action type)
	virtual void Execute();
};

#endif