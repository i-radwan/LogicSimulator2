#include "New.h"
#include"..\Components\Component.h"
#include"..\Components\Gate.h"
#include"..\ApplicationManager.h"
#include"..\Components\Connection.h"
#include <iostream>
using namespace std;
Clear::Clear(ApplicationManager*pApp) : Action(pApp)
{
	Loading = false;
}
bool Clear::ReadActionParameters(image * i, Component* c)
{
	Output *pOut = pManager->GetOutput();
	int selected = pOut->printPopUpMessage("",'N');
	if (selected == 1) return true;
	return false;
}
void Clear::Execute()
{
	if (Loading || ReadActionParameters()) {
		Output *pOut = pManager->GetOutput();
		for (int i = 0; i <pManager->getCompCount(); i++) {
			Component *C = pManager->getComponent(i);
			if (!dynamic_cast<Connection*> (C)) {
				C->setDelete(true);
				C->Draw(pOut);
				//Removing Connections
				vector<Connection*> allInConnections, allOutConnections;
				C->getAllInputConnections(allInConnections);
				C->getAllOutputConnections(allOutConnections);
				pOut->clearConnections(allInConnections, C->getCenterLocation().x1, C->getCenterLocation().y1, true, true);
				pOut->clearConnections(allOutConnections, C->getCenterLocation().x1, C->getCenterLocation().y1, false, true);
			}
		}		
		// clear status bar
		
		//Clear Drawing Area
		pOut->ClearDrawingArea();
		//Resetting Interface
		pOut->resetInterfaceArrays();
		pManager->setCompCount(0);
		if (Loading)
			Loading = false;
	}
}
void Clear::Undo()
{

}
void Clear::Redo()
{

}
void Clear::setLoading(bool T)
{
	Loading = T;
}
Clear::~Clear()
{
}
