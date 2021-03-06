#ifndef _LED_H
#define _LED_H
#include"Component.h"
#include "InputPin.h"
#include "OutputPin.h"
class LED : public Component
{
public:
	LED(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	virtual void Operate();	//Calculates the output according to the inputs
	virtual void Draw(Output* pOut, bool highlight);	//for each component to Draw itself


	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void save(int, ofstream &);
	virtual void load(ApplicationManager* pM, ifstream& in);

	virtual void setInputPinStatus(int n, STATUS s); virtual void selectYourSelf(Output* pOut, color Color);
	//Destructor must be virtual
	virtual ~LED();
	virtual ActionType getComponentActionType();

};

#endif