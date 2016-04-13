#ifndef LED_Header
#define LED_Header
#include"Action.h"
#include"..\Components\LED.h"
#include"..\ApplicationManager.h"
class AddLED:public Action
{
private:
	 string gateLabel;
	GraphicsInfo GInfo;
public:
	AddLED(ApplicationManager *pApp);
	virtual ~AddLED(void);
	//Reads parameters required for action to execute
	virtual bool  ReadActionParameters(image * );
	//Execute action (code depends on action type)
	virtual void Execute();

	virtual void Undo();
	virtual void Redo();

};

#endif