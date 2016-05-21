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

	int x, y;
	Component* Comp = NULL;
	while (pIn->GetButtonStatus(LEFT_BUTTON, x, y) == BUTTON_DOWN) {
		for (int i = 0; i < Utils::allComponentsCorners.size(); i++)
		{
			if (x >= Utils::allComponentsCorners[i].x1 && x <= Utils::allComponentsCorners[i].x2 && y >= Utils::allComponentsCorners[i].y1&&y <= Utils::allComponentsCorners[i].y2 && !dynamic_cast<Connection*> (pManager->getComponent(i)))
			{
				compIdx = i;
				Comp = pManager->getComponent(i);
			}
		}
		//
		if (Comp != NULL &&Comp->getDelete()) Comp = NULL;
		if (Comp != NULL && compIdx != -1) {
			oldSmallCleanImage = Comp->getSmallCleanImageBeforeAddingComp();

			Comp->setDelete(true);
			Comp->Draw(pManager->GetOutput());
			//Free gate location
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
				Comp->setNewCenterLocation(newCoor);
				Comp->setDelete(false);
				Utils::allComponentsCorners[compIdx].x1 = newCoor.x1 - UI.GATE_Width / 2;
				Utils::allComponentsCorners[compIdx].y1 = newCoor.y1 - UI.GATE_Height / 2;
				Utils::allComponentsCorners[compIdx].x2 = newCoor.x1 + UI.GATE_Width / 2;
				Utils::allComponentsCorners[compIdx].y2 = newCoor.y1 + UI.GATE_Height / 2;
				Comp->setSmallCleanImageBeforeAddingComp(newSmallImageForGate);
				Comp->Draw(pOut, false);
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
	Utils::allComponentsCorners[compIdx].x1 = oldGraphicsInfo.x1 - UI.GATE_Width / 2;
	Utils::allComponentsCorners[compIdx].y1 = oldGraphicsInfo.y1 - UI.GATE_Height / 2;
	Utils::allComponentsCorners[compIdx].x2 = oldGraphicsInfo.x1 + UI.GATE_Width / 2;
	Utils::allComponentsCorners[compIdx].y2 = oldGraphicsInfo.y1 + UI.GATE_Height / 2;
	pA->setSmallCleanImageBeforeAddingComp(oldSmallCleanImage);

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
	Utils::allComponentsCorners[compIdx].x1 = newCoor.x1 - UI.GATE_Width / 2;
	Utils::allComponentsCorners[compIdx].y1 = newCoor.y1 - UI.GATE_Height / 2;
	Utils::allComponentsCorners[compIdx].x2 = newCoor.x1 + UI.GATE_Width / 2;
	Utils::allComponentsCorners[compIdx].y2 = newCoor.y1 + UI.GATE_Height / 2;
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
