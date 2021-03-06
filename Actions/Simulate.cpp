#include "Simulate.h"
#include"..\ApplicationManager.h"
#include "..\Components\LED.h"
#include "..\Components\SWITCH.h"
#include "..\Components\Connection.h"
#include "..\Components\Component.h"
#include <algorithm>
#include "Validate.h"
Simulate::Simulate(ApplicationManager*pApp, bool v) : Action(pApp), validateFirst(v)
{
}

bool Simulate::ReadActionParameters(image *, Component* c)
{
	return false;
}
void Simulate::Execute()
{
	// Check validaity
	bool isValid = false;
	if (validateFirst)
	{
		Validate validateAction(pManager);
		validateAction.Execute();
		isValid = validateAction.getValid();
	} else{
		isValid = true;
	}
	if (!isValid)
	{
		return;
	}
	// reset all input pins to avoid old pins state
	for (size_t i = 0; i < pManager->getCompCount(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<SWITCH*>(comp)) || (dynamic_cast<Connection*>(comp) || comp->getDelete())) continue;
		for (size_t i = 0; i < comp->getNumOfInputs(); i++)
		{
			comp->setInputPinStatus(i, UNDEFINED);
		}
	}
	int totalComponentsCount = 0, operatedItemsCount = 0;
	vector<Component*> toBeOperatedComponents, operatedComponents;
	// Send switches signals to all next components
	for (size_t i = 0; i <pManager->getCompCount(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<Connection*>(comp) || comp->getDelete())) continue;
		if (dynamic_cast<SWITCH*>(comp))
		{
			for (size_t i = 0; i < comp->getOutputPin()->connectedConnectionsCount(); i++)
			{
				comp->getOutputPin()->getConnection(i)->getDestPin()->setStatus(static_cast<STATUS>(comp->GetOutPinStatus()));
			}
		}
		else if (!dynamic_cast<LED*>(comp)) { toBeOperatedComponents.push_back(comp); totalComponentsCount++; }
	}
	//Go to all LEDS and calcaulate the pins values
	while (operatedItemsCount < totalComponentsCount) //iterate until all components pins get status
	{
		for (size_t i = 0; i < toBeOperatedComponents.size(); i++)
		{
			Component* comp = toBeOperatedComponents[i];
			if (find(operatedComponents.begin(), operatedComponents.end(), comp) != operatedComponents.end())
			{
				continue;
			}
			// if not all pins have value skip this component
			bool isAllPinsHaveStatus = true;
			for (size_t j = 0; j < comp->getNumOfInputs(); j++)
			{
				if (comp->getInputPin(j)->getStatus() == UNDEFINED)
				{
					isAllPinsHaveStatus = false;
				}
			}
			if (!isAllPinsHaveStatus)
			{
				continue;
			}
			else {
				// operate the component and send its value to connected connections
				comp->Operate();
				for (size_t k = 0; k < comp->getOutputPin()->connectedConnectionsCount(); k++)
				{
					comp->getOutputPin()->getConnection(k)->getDestPin()->setStatus(static_cast<STATUS>(comp->GetOutPinStatus()));
				}
				operatedComponents.push_back(comp);
				operatedItemsCount++;
			}
		}

	}
	//Light up leds
	for (size_t i = 0; i <pManager->getCompCount(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if ((dynamic_cast<Connection*>(comp) || comp->getDelete())) continue;
		if (dynamic_cast<LED*>(comp)) { comp->Draw(pManager->GetOutput(), false); }
	}
	//light up connections
	for (size_t i = 0; i < pManager->getCompCount(); i++)
	{
		Component* comp = pManager->getComponent(i);
		if (comp->getDelete()) continue;
		if (dynamic_cast<Connection*>(comp))
		{
			if (((Connection*)comp)->getSourcePin()->getStatus() == HIGH)
			{
				((Connection*)comp)->selectYourSelf(pManager->GetOutput(), UI.OneValueConnection);
			}
			else {
				((Connection*)comp)->selectYourSelf(pManager->GetOutput(), UI.DrawColor);
			}
		}
	}
}
void Simulate::Undo()
{

}
void Simulate::Redo()
{

}
Simulate::~Simulate()
{

}
