#ifndef _COMPONENT_H
#define _COMPONENT_H
#include "..\GUI\Output.h"
#include "InputPin.h"
#include "OutputPin.h"

class Component
{
private:
	string m_Label;
	bool deleted;
	image* smallCleanImageBeforeAddingComp;
	int m_Inputs;		//No. of input pins of that Gate.
	static int lastID;
	int ID;
protected:
	GraphicsInfo m_GfxInfo;	//The parameters required to draw a component
	GraphicsInfo m_CenterInfo;
	InputPin* m_InputPins;	//Array of input pins of the Gate
	OutputPin m_OutputPin;	//The Gate output pin
public:
	static int CreatedComponents, DestructedComponents;
	Component(const GraphicsInfo &r_GfxInfo, int r_FanOut);
	Component(const GraphicsInfo &r_GfxInfo);

	virtual void Operate() = 0;	//Calculates the output according to the inputs
	virtual void Draw(Output* pOut, bool highlight = false) = 0;	//for each component to Draw itself
	
	virtual int GetOutPinStatus() = 0;	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n) = 0;	//returns status of Inputpin # n if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s) = 0;	//set status of Inputpin # n, to be used by connection class.
	
	virtual ActionType getComponentActionType() = 0;
	void refresh(Output* pOut)
	{
		pOut->DrawCleanImage(getSmallCleanImageBeforeAddingComp(), m_CenterInfo.x1+1, m_CenterInfo.y1, false);
		Draw(pOut, false);
	}
	void setLabel(string s);
	string getLabel();

	void setDelete(bool d);
	bool getDelete();
	
	void setID(int id);
	int getID();

	void setNewCenterLocation(GraphicsInfo GfxInfo);
	GraphicsInfo getCenterLocation();

	void setCornersLocation(GraphicsInfo GfxInfo);
	GraphicsInfo getCornersLocation();

	void setSmallCleanImageBeforeAddingComp(image* i);
	image* getSmallCleanImageBeforeAddingComp();

	virtual void selectYourSelf(Output* pOut, color Color) = 0;

	void Component::setNumOfInputs(int n);
	int getNumOfInputs()const;

	OutputPin* getOutputPin();
	InputPin* getInputPin(int);

	void getAllInputConnections(vector<Connection*>& allConnections);
	void getAllOutputConnections(vector<Connection*>& allOutputConnections);

	//Save and Load
	virtual void save(int, ofstream &)=0;
	virtual void load(ApplicationManager*,ifstream& )=0;

	//Destructor must be virtual
	virtual ~Component();
};

#endif
