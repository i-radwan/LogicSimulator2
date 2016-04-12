#pragma once
#include "Action.h"
class Move :
	public Action
{
public:
	Move(ApplicationManager *pApp);
	virtual ~Move();

	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters();
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

