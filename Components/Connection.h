#ifndef _CONNECTIONS_H
#define _CONNECTIONS_H
#include "Component.h"
#include "OutputPin.h"
#include "InputPin.h"
class Connection :	public Component
{
	//Component*	SrcCmpnt;	//Connection source component
	//Component*	DstCmpnt;	//Connection Destination component
	//int		DstPin;		//The Input pin to which this connection is linked
	OutputPin* SrcPin;	//The Source pin of this connection (an output pin of certain Component)
	InputPin* DstPin;	//The Destination pin of this connection (an input pin of certain Component)
	vector<Cell> cellsBeforeAddingConnection;
	bool isDrawn;
public:
	//Connection(const GraphicsInfo &r_GfxInfo, Component *pS=NULL,Component *pD=NULL, int Pin=0);
	Connection(const GraphicsInfo &r_GfxInfo, OutputPin *pSrcPin,InputPin *pDstPin);

	virtual void Operate() ;	//Calculates the output according to the inputs
	virtual void Draw(Output* pOut, bool highlight);	//for each component to Draw itself

	virtual ActionType getComponentActionType();
	
	void setSourcePin(OutputPin *pSrcPin);
	void setDestPin(InputPin *pDstPin);
	OutputPin* getSourcePin();
	InputPin* getDestPin();

	void setIsDrawn(bool isDrawn);
	bool getIsDrawn();

	void deleteConnection(Output* pOut);

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s); 
	virtual void selectYourSelf(Output* pOut, color Color);

	void setCellsBeforeAddingConnection(vector<Cell> cellsBeforeAddingConnection);
	vector<Cell>& getCellsBeforeAddingConnection();

	virtual void save(int, ofstream &);
	virtual void load(ApplicationManager*,ifstream& in);
};
#endif