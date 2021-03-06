#include "Move.h"
#include "..\ApplicationManager.h"
#include "..\Components\NOR3.h"
#include "..\Components\AND2.h"
#include "..\Components\AND3.h"
#include "..\Components\BUFFER.h"
#include "..\Components\LED.h"
#include "..\Components\NAND2.h"
#include "..\Components\NOR2.h"
#include "..\Components\NOT.h"
#include "..\Components\OR2.h"
#include "..\Components\SWITCH.h"
#include "..\Components\XNOR2.h"
#include "..\Components\XOR3.h"
#include "..\Components\XOR2.h"
#include "..\Components\Connection.h"
#include<iostream>
using namespace std;

Move::Move(ApplicationManager *pApp) :Action(pApp)
{
	int compIdx;
	newSmallImageForGate = new image;
}

bool Move::ReadActionParameters(image * smallImageBeforeAddingComponent, Component* c)
{
	return true;
}

void Move::Execute()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	Component* Comp = NULL;

	int x, y;
	// While the user is clicking
	while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
		Comp = pManager->getComponentByCoordinates(x, y, false, true, compIdx);
		if (Comp != NULL &&Comp->getDelete()) Comp = NULL;
		if (Comp != NULL && !dynamic_cast<Connection*>(Comp) && compIdx != -1) {
			// get old preimage for undo
			oldSmallCleanImage = Comp->getSmallCleanImageBeforeAddingComp();
			//Delete the component
			Comp->setDelete(true);
			Comp->Draw(pManager->GetOutput());
			oldGraphicsInfo.x1 = Comp->getCenterLocation().x1;
			oldGraphicsInfo.y1 = Comp->getCenterLocation().y1;
			// Free related connections
			vector <Connection*> allIns, allOuts;
			Comp->getAllInputConnections(allIns);
			Comp->getAllOutputConnections(allOuts);

			//Get Action type
			ActionType ActType = Comp->getComponentActionType();
			//Drag
			if (pManager->GetOutput()->SetDragImage(ActType, newCoor, newSmallImageForGate, true, Comp)) {
				//set new center location
				Comp->setNewCenterLocation(newCoor);
				// redraw and set the parameters
				Comp->setDelete(false);
				Comp->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);
				Comp->Draw(pOut, false);

				// Select the connected with  normal color after releasing the moved gate
				vector<Connection*> allInConnections, allOutConnections;
				Comp->getAllInputConnections(allInConnections);
				Comp->getAllOutputConnections(allOutConnections);
				for (size_t i = 0; i < allOutConnections.size(); i++)
				{
					allOutConnections[i]->selectYourSelf(pOut, UI.ConnColor);
				}
				for (size_t i = 0; i < allInConnections.size(); i++)
				{
					allInConnections[i]->selectYourSelf(pOut, UI.ConnColor);
				}
				Utils::undoActions.push(this);
				Action::pA = Comp;
			}
			else {
				Comp->setDelete(false);
				Comp->Draw(pManager->GetOutput());
			}
		}
	}
	if (Comp != NULL) {
		Comp->setDelete(false);
		Comp->Draw(pManager->GetOutput());
	}


}

void Move::Undo()
{
	Output* pOut = pManager->GetOutput();
	//delete the comp
	pA->setDelete(true);
	pA->Draw(pOut, false);

	//remove new Connections
	vector<Connection*> allInputConnections, allOutputConnections;
	pA->getAllInputConnections(allInputConnections);
	pA->getAllOutputConnections(allOutputConnections);

	pOut->clearConnections(allInputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, true, false);
	pOut->clearConnections(allOutputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, false, false);

	//redraw and reassign data

	pA->setNewCenterLocation(oldGraphicsInfo);
	pA->setSmallCleanImageBeforeAddingComp(oldSmallCleanImage);

	// redraw after changing parameters
	pA->setDelete(false);
	pA->Draw(pOut, false);
	//redraw old connections
	for (size_t i = 0; i < allInputConnections.size(); i++)
	{
		GraphicsInfo currentGfx = allInputConnections[i]->getCornersLocation();
		currentGfx.x2 = currentGfx.x2 - (newCoor.x1 - oldGraphicsInfo.x1);
		currentGfx.y2 = currentGfx.y2 - (newCoor.y1 - oldGraphicsInfo.y1);
		allInputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
		pOut->DrawConnection(currentGfx, allInputConnections[i]->getDestPin()->getPosition(), { oldGraphicsInfo.x1, oldGraphicsInfo.y1,0,0 }, allInputConnections[i]->getCellsBeforeAddingConnection(), false, allInputConnections[i]);

	}
	for (size_t i = 0; i < allOutputConnections.size(); i++)
	{
		GraphicsInfo currentGfx = allOutputConnections[i]->getCornersLocation();
		currentGfx.x1 = currentGfx.x1 - (newCoor.x1 - oldGraphicsInfo.x1);
		currentGfx.y1 = currentGfx.y1 - (newCoor.y1 - oldGraphicsInfo.y1);
		allOutputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
		Component* dstComp = allOutputConnections[i]->getDestPin()->getComponent();
		pOut->DrawConnection(currentGfx, allOutputConnections[i]->getDestPin()->getPosition(), { dstComp->getCenterLocation().x1, dstComp->getCenterLocation().y1,0,0 }, allOutputConnections[i]->getCellsBeforeAddingConnection(), false, allOutputConnections[i]);
	}
}

void Move::Redo()
{
	Output* pOut = pManager->GetOutput();
	pA->setDelete(true);
	pA->Draw(pOut, false);

	//remove new Connections
	vector<Connection*> allInputConnections, allOutputConnections;
	pA->getAllInputConnections(allInputConnections);
	pA->getAllOutputConnections(allOutputConnections);

	pOut->clearConnections(allInputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, true, false);
	pOut->clearConnections(allOutputConnections, pA->getCenterLocation().x1, pA->getCenterLocation().y1, false, false);

	//redraw and reassign data

	pA->setNewCenterLocation(newCoor);
	pA->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);

	pA->setDelete(false);
	pA->Draw(pOut, false);

	//redraw old connections
	for (size_t i = 0; i < allInputConnections.size(); i++)
	{
		GraphicsInfo currentGfx = allInputConnections[i]->getCornersLocation();
		currentGfx.x2 = currentGfx.x2 + (newCoor.x1 - oldGraphicsInfo.x1);
		currentGfx.y2 = currentGfx.y2 + (newCoor.y1 - oldGraphicsInfo.y1);
		allInputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
		pOut->DrawConnection(currentGfx, allInputConnections[i]->getDestPin()->getPosition(), { newCoor.x1, newCoor.y1,0,0 }, allInputConnections[i]->getCellsBeforeAddingConnection(), false, allInputConnections[i]);

	}
	for (size_t i = 0; i < allOutputConnections.size(); i++)
	{
		GraphicsInfo currentGfx = allOutputConnections[i]->getCornersLocation();
		currentGfx.x1 = currentGfx.x1 + (newCoor.x1 - oldGraphicsInfo.x1);
		currentGfx.y1 = currentGfx.y1 + (newCoor.y1 - oldGraphicsInfo.y1);
		allOutputConnections[i]->setCornersLocation({ currentGfx.x1 ,currentGfx.y1,currentGfx.x2 ,currentGfx.y2 });
		Component* dstComp = allOutputConnections[i]->getDestPin()->getComponent();
		pOut->DrawConnection(currentGfx, allOutputConnections[i]->getDestPin()->getPosition(), { dstComp->getCenterLocation().x1, dstComp->getCenterLocation().y1,0,0 }, allOutputConnections[i]->getCellsBeforeAddingConnection(), false, allOutputConnections[i]);

	}
}
Move::~Move()
{
}