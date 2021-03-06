#include "Input.h"
#include "Output.h"
#include "..\Utils.h"
#include <iostream>
#include "..\Components\SWITCH.h"
#include "..\Components\LED.h"
#include "..\Components\Gate.h"
#include "..\Actions\ChangeSwitch.h"
Input::Input(window* pW)
{
	pWind = pW; //point to the passed window
	isSelectMode = isSelectionContainConnections = false;
	toBeChangedSwitch = NULL;
	toBeAddedToSelected = toBeRemovedFromSelected = NULL;
	toBeSelectedConnection = NULL;
}
// Get point clicked and correct it 
void Input::GetPointClicked(int &x, int &y, bool DrawGate, bool DrawConnection)
{
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
	Utils::correctPointClicked(x, y, DrawGate, DrawConnection);
}
void Input::getSelectionPoint(int & x, int & y)
{
	pWind->GetMouseClick(x, y);
	pWind->FlushMouseQueue();
}
void Input::getMouseCoordinates(int & x, int & y)
{
	pWind->GetMouseCoord(x, y);
	pWind->FlushMouseQueue();
}
void Input::DrawAfterMenu(image * img, int x, int y, int type, int w)
{
	int Height;
	switch (type)
	{
	case 1:
		Height = UI.RightClickMenuHeight;
		break;
	case 2:
		Height = UI.RightClickCMenuH + UI.RightClickCMenuH / 2;
		break;
	case 3:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 5:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 7:
		//HoveringBar
		Height = UI.HoverHeight;
		break;
	case 9:
		//It's the status Bar
		Height = UI.StatusBarHeight;
		break;
	}
	if (type == 7) {
		//It's an hover part
		pWind->DrawImage(img, x, y, w, Height);
	}
	else if (type == 9) {
		//It's the status Bar
		pWind->DrawImage(img, x, y, UI.StatusBoxWidth, UI.StatusBoxHeight);
	}
	else pWind->DrawImage(img, x, y, UI.RightClickMenuLength, Height);
	if (type != 7)
		delete img;
}
void Input::clearHoveringImage(image * img, int J, int K, int widthh)
{
	if (img != NULL)
		this->DrawAfterMenu(img, J, K, 7, widthh);
}
void Input::correctHoverStartpoint(int & x, int & y, int width)
{
	//If Point (x,y) in the Right or Middle of the Drawing Area
	if (x + width < UI.width) {
		if (y + UI.HoverHeight > UI.height) {
			// y must be corrected
			y = y - UI.HoverHeight;
		}
		else {
			//The Point(x,y) is ready for drawing HoverBar 
		}
	}
	//If Point (x,y) in the Right of the Drawing Area
	if (x + width > UI.width) {
		// x must be corrected
		x = UI.width - width - 40;
		if (y + UI.HoverHeight > UI.height - UI.StatusBarHeight) {
			// y must be corrected
			y = y - UI.HoverHeight;
		}
		else {
			//The Point(x,y) is ready for drawing HoverBar 
		}
	}
}
image * Input::StoreBeforeMenu(int x, int y, int type, int w)
{
	int Height;
	switch (type)
	{
	case 1:
		Height = UI.RightClickMenuHeight;
		break;
	case 2:
		Height = UI.RightClickCMenuH + UI.RightClickCMenuH / 2;
		break;
	case 3:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 5:
		Height = UI.RightClickCMenuH / 2;
		break;
	case 7:
		//HoveringBar
		Height = UI.HoverHeight;
		break;
	}
	image * ptr = new image;
	if (type == 7) {
		//It's an hover part
		pWind->StoreImage(ptr, x, y, w, Height);
	}
	else if (type == 9) {
		//It's the status Bar
		pWind->StoreImage(ptr, x, y, UI.StatusBoxWidth, UI.StatusBoxHeight);
	}
	else pWind->StoreImage(ptr, x, y, UI.RightClickMenuLength, Height);
	return ptr;
}

image * Input::printHovering(int & x, int & y, string s, int & w, Component * C, int pin)
{
	image* img = NULL;
	int width, height;
	pWind->SetFont(20, BOLD | ITALICIZED, BY_NAME, "Arial");
	//checking whether it's in the design Mode or 
	if (UI.AppMode == DESIGN) {
		int h;
		pWind->getStringWidth(s, width, h);
		//the Clearnace
		width += 16;
		height = UI.HoverHeight;
		if (!dynamic_cast<Connection*>(C)) {
			//if it's a connection for better appearnace
			x = C->getCenterLocation().x1 + 10;
			y = C->getCenterLocation().y1 + 5;
		}
		//Correct the Points
		this->correctHoverStartpoint(x, y, width);
		//Store Before Hovering
		img = this->StoreBeforeMenu(x, y, 7, width);
		//Drawing the Hover Bar
		pWind->SetPen(BLACK, 2);
		pWind->SetBrush(AZURE);
		pWind->DrawRectangle(x, y, x + width, y + height, FILLED, 3, 4);
		pWind->DrawString(x + 7, y + 2, s);
		w = width;
		return img;
	}
	else {
		//Simulation Mode Hovering
		Connection *ptr = NULL;

		//Checking if it's a connection Hover or other componenets hover
		if (!dynamic_cast<Connection*> (C))
		{
			//Check if hovering pin: Set the component to the meant connection of the hovered pin to enter the next if as connection and display hover box
			//output pin
			GraphicsInfo compCorner = C->getCornersLocation();
			if (pin == 3)
			{
				C = C->getOutputPin()->getConnection(0);
			}
			else if (pin == 0) { C = C->getInputPin(0)->getConnection(); }
			else if (pin == 2) { C = C->getInputPin((C->getNumOfInputs() == 2) ? 1 : 2)->getConnection(); }
			else if (pin == 1) { C = C->getInputPin((C->getNumOfInputs() == 1) ? 0 : 1)->getConnection(); }
			else if (pin == -1 && C->getLabel().size() > 0) {
				//Other Component Hover
				int h;
				height = UI.HoverHeight;
				pWind->getStringWidth(s, width, h);
				//the Clearnace
				width += 16;
				//Correct the Points
				x = C->getCenterLocation().x1 + 10;
				y = C->getCenterLocation().y1 + 5;
				this->correctHoverStartpoint(x, y, width);
				//Store Before Hovering
				img = this->StoreBeforeMenu(x, y, 7, width);
				//Drawing the Hover Bar
				pWind->SetPen(BLACK, 2);
				pWind->SetBrush(LIGHTBLUE);
				pWind->DrawRectangle(x, y, x + width, y + height, FILLED, 4, 4);
				pWind->DrawString(x + 7, y + 2, s);
				w = width;
			}
		}
		if (ptr = dynamic_cast<Connection*> (C)) {
			string hoverLabel = "";
			width = 20;
			height = UI.HoverHeight;
			//Correct the Points
			this->correctHoverStartpoint(x, y, width);
			//Store Before Hovering
			img = this->StoreBeforeMenu(x, y, 7, width);
			if (ptr->GetOutPinStatus()) { //It's a logic 1 output -> green Colour
				hoverLabel = "1";
				pWind->SetPen(BLACK, 2);
				pWind->SetBrush(GREEN);
			}
			else { //It's a logic 0 output -> grey Colour
				hoverLabel = "0";
				pWind->SetPen(BLACK, 2);
				pWind->SetBrush(GREY);
			}
			pWind->DrawRectangle(x, y, x + width, y + height, FILLED, 4, 4);
			pWind->DrawString(x + 5, y + 1, hoverLabel);
			w = width;
		}

	}
	return img;
}
//This function reads the position where the user clicks to determine the desired action
ActionType Input::GetUserAction()
{
	int x = 0, y = 0, xT, yT, hoverXOld = 0, hoverYOld = 0, oldTopHoveredItemOrder = -1, oldLeftHoverItem = -1;

	//For Hovering Bar
	int widthh = 0;
	image* imgh = NULL;
	int J = 0, K = 0;

	clicktype s = LEFT_CLICK;
	Component* preComp = NULL;
	int prePin = -1;
	while (true) {
		// Check for keyboard hotkeys
		char c1;
		keytype key = pWind->GetKeyPress(c1);
		if ((int)c1 == 26) {
			//Always Clear hover Bar if found
			clearHoveringImage(imgh, J, K, widthh);
			return UNDOACTION;
		}
		else if ((int)c1 == 25) {
			//Always Clear hover Bar if found
			clearHoveringImage(imgh, J, K, widthh);
			return REDOACTION;
		}
		else if ((int)c1 == 24) {
			//Always Clear hover Bar if found
			image* i = new image;
			pWind->StoreImage(i, UI.LeftToolBarWidth, UI.TopToolBarHeight, UI.width - UI.LeftToolBarWidth - 20, UI.height - UI.TopToolBarHeight - 20);
			i->SaveImage("ScreenShot.bmp", pWind);
		}
		else if ((int)c1 == 4) {
			return DSN_MODE;
		}
		else if ((int)c1 == 18) {
			//Always Clear hover Bar if found
			clearHoveringImage(imgh, J, K, widthh);
			return SimulateAction;
		}
		else if ((int)c1 == 19) {
			//Always Clear hover Bar if found
			clearHoveringImage(imgh, J, K, widthh);
			return SAVE;
		}
		// Double check that there is no pressed key
		if (key == 0)
		{
			key = pWind->GetKeyPress(c1);
		}
		// Working with mouse
		bool drawConnection = false;
		//Check if button clicked and within the borders
		if (pWind->GetButtonState(LEFT_BUTTON, xT, yT) == BUTTON_DOWN && yT >= UI.ToolBarHeight + 20 && xT >= UI.LeftToolBarWidth && yT < UI.height - UI.StatusBarHeight) {
			if (UI.AppMode == DESIGN)
			{
				//Get the component
				Component* comp = Utils::getArrayOfComponents(yT / UI.GRID_SIZE, xT / UI.GRID_SIZE);
				if (comp != NULL && !(dynamic_cast<Connection*>(comp) || comp->getDelete() || dynamic_cast<LED*>(comp)))
				{
					GraphicsInfo compCorner = comp->getCornersLocation();
					//Check if the user clicked the output pin
					if (xT >= compCorner.x1&&xT <= compCorner.x2&& yT >= compCorner.y1&&yT <= compCorner.y2)
					{
						if (xT >= compCorner.x2 - UI.GATE_Width / 2 + 8 && yT > (compCorner.y1 + UI.GATE_Height / 2) - 6 && yT < (compCorner.y1 + UI.GATE_Height / 2) + 6)
						{
							pWind->SetPen(UI.ErrorColor, 5);
							pWind->DrawLine(compCorner.x2 - UI.GATE_Width / 2 + 11, compCorner.y1 + UI.GATE_Height / 2, compCorner.x2 - UI.GATE_Width / 2 + 12, compCorner.y1 + UI.GATE_Height / 2, FRAME);
							startXPointForConnections = xT;
							startYPointForConnections = yT;
							drawConnection = true;
						}
					}
				}
				if (drawConnection)
				{
					//Always Clear hover Bar if found
					clearHoveringImage(imgh, J, K, widthh);
					return ADD_CONNECTION;
				}
				if (comp != NULL &&comp->getDelete()) comp = NULL;
				if (comp != NULL && !dynamic_cast<Connection*>(comp)) {
					if (isSelectMode)
					{
						for (size_t i = 0; i < selectedComponents.size(); i++)
						{
							// check if user selects one of the selected components
							if (!dynamic_cast<Connection*>(selectedComponents[i].second) && selectedComponents[i].second == comp) {
								if (key == CTRL) // remove this component from selected ones
								{
									toBeRemovedFromSelected = comp;
									clearHoveringImage(imgh, J, K, widthh);
									return MULTI_SELECT; // re-execute multi-select action in order to update the selection list
								}
								else { // Double check if the user clicked CTRL key
									key = pWind->GetKeyPress(c1);
									if (key == CTRL || (int)c1 == 13) {
										toBeRemovedFromSelected = comp;
										clearHoveringImage(imgh, J, K, widthh);
										return MULTI_SELECT;// re-execute multi-select action in order to update the selection list
									}
								}
								//Always Clear hover Bar if found //CHECKTHISSAMRA
								clearHoveringImage(imgh, J, K, widthh);
								return MULTI_MOVE; // user clicked already selected components :: he wants to move them all
							}
						}
						// Ctrl
						if (key == CTRL) // add this component to selected ones
						{
							toBeAddedToSelected = comp;
							clearHoveringImage(imgh, J, K, widthh);
							return MULTI_SELECT;
						}
					}
					else {
						Sleep(90); // Sleep to distinguish between select and move
						int tmX, tmY;
						if (pWind->GetButtonState(LEFT_BUTTON, tmX, tmY) == BUTTON_DOWN) {
							//Always Clear hover Bar if found
							clearHoveringImage(imgh, J, K, widthh);
							return MOVE;
						}
						else {
							clearHoveringImage(imgh, J, K, widthh);
							return SELECT;
						}
					}
				}
				else {
					if (!isSelectMode) {  // If user clicked on empty space the multiselect mode will start
						int tmpX = xT, tmpY = yT;
						Utils::correctPointClicked(tmpX, tmpY, true, false);
						Component* c = Utils::getArrayOfComponents(tmpY / UI.GRID_SIZE, tmpX / UI.GRID_SIZE);
						if (c == NULL || isSelectMode) {
							clearHoveringImage(imgh, J, K, widthh);
							return MULTI_SELECT;
						}
						else if (c != NULL && dynamic_cast<Connection*>(c)) { // if clicked on connection , this connection is going to be selected
							pWind->DownButtons(false);
							clearHoveringImage(imgh, J, K, widthh);
							toBeSelectedConnection = (Connection*)c;
							return SELECT_CONNECTION;
						}
					}
					clearHoveringImage(imgh, J, K, widthh);
					return MULTI_SELECT; // Else start the multi select action
				}
			}
		}
		else {
			//Get the coordinates of the user click 
			// We Called it with false argument inorder not to delete the click to be used in RightSelect
			//Otherwise we call that function agian with true to delete that click 
			if ((s = pWind->GetMouseClick(x, y, false)) != NO_CLICK)
			{
				break;
			}
			else { // This block covers the hovering part
				int hoverX, hoverY;
				pWind->GetMouseCoord(hoverX, hoverY);
				//If top bar and design mode and top bar down
				if (hoverY >= 5 && hoverY < UI.ToolBarHeight - 20 && hoverX > UI.LeftToolBarWidth && UI.AppMode == DESIGN)
				{ // Hovering the top bar
					//Always Clear hover Bar if found
					clearHoveringImage(imgh, J, K, widthh);
					vector<pair<int, int> > TopItemsRanges;
					TopItemsRanges.push_back(make_pair(107, 170));
					TopItemsRanges.push_back(make_pair(190, 245));
					TopItemsRanges.push_back(make_pair(275, 332));
					TopItemsRanges.push_back(make_pair(360, 415));
					TopItemsRanges.push_back(make_pair(443, 496));
					TopItemsRanges.push_back(make_pair(527, 592));
					TopItemsRanges.push_back(make_pair(612, 662));
					TopItemsRanges.push_back(make_pair(697, 753));
					TopItemsRanges.push_back(make_pair(782, 836));
					TopItemsRanges.push_back(make_pair(867, 922));
					TopItemsRanges.push_back(make_pair(953, 1010));
					TopItemsRanges.push_back(make_pair(1035, 1091));
					TopItemsRanges.push_back(make_pair(1121, 1166));

					int HoveredItemOrder = -1;

					for (size_t i = 0; i < TopItemsRanges.size(); i++)
					{
						if (hoverX >= TopItemsRanges[i].first - 5 && hoverX <= TopItemsRanges[i].second + 3) {
							HoveredItemOrder = i;
						}
					}
					if (HoveredItemOrder != oldTopHoveredItemOrder)
					{ // Draw specific photo for each hovered icon

						switch (HoveredItemOrder)
						{
						case D2AND:  pWind->DrawImage("images\\Menu\\top_bar_and2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D3AND:  pWind->DrawImage("images\\Menu\\top_bar_and3_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2OR: pWind->DrawImage("images\\Menu\\top_bar_or2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2XOR: pWind->DrawImage("images\\Menu\\top_bar_xor2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D3XOR: pWind->DrawImage("images\\Menu\\top_bar_xor3_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DBUFFER:  pWind->DrawImage("images\\Menu\\top_bar_buffer_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DNOT: pWind->DrawImage("images\\Menu\\top_bar_not_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2NAND:  pWind->DrawImage("images\\Menu\\top_bar_nand2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2NOR:  pWind->DrawImage("images\\Menu\\top_bar_nor2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D3NOR: pWind->DrawImage("images\\Menu\\top_bar_nor3_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case D2XNOR: pWind->DrawImage("images\\Menu\\top_bar_xnor2_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DSWITCH: pWind->DrawImage("images\\Menu\\top_bar_switch_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						case DLED: pWind->DrawImage("images\\Menu\\top_bar_led_hovered.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						default:  pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight); break;
						}
					}
					oldTopHoveredItemOrder = HoveredItemOrder;
				}
				else if (hoverY >= 0 && hoverY < UI.height && UI.AppMode == DESIGN && hoverX <= UI.LeftToolBarWidth - 12 && hoverX >= 5) {
					//Left toolbar hovering icons
					//Always Clear hover Bar if found
					clearHoveringImage(imgh, J, K, widthh);
					vector<pair<int, int> > LeftItemsRanges;
					LeftItemsRanges.push_back(make_pair(6, 85));
					LeftItemsRanges.push_back(make_pair(103, 180));
					LeftItemsRanges.push_back(make_pair(197, 273));
					LeftItemsRanges.push_back(make_pair(286, 366));
					LeftItemsRanges.push_back(make_pair(382, 460));
					LeftItemsRanges.push_back(make_pair(475, 550));

					int HoveredLeftItemOrder = -1;

					for (size_t i = 0; i < LeftItemsRanges.size(); i++)
					{
						if (hoverY >= LeftItemsRanges[i].first && hoverY <= LeftItemsRanges[i].second) {
							HoveredLeftItemOrder = i;
						}
					}
					if (HoveredLeftItemOrder != oldLeftHoverItem)
					{ // Draw specific icons for left hovered bar in design mode
						switch (HoveredLeftItemOrder + 14)
						{
						case DSIMULATION: pWind->DrawImage("images\\Menu\\left_bar_simulate_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DNEW:pWind->DrawImage("images\\Menu\\left_bar_new_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DSAVE: pWind->DrawImage("images\\Menu\\left_bar_save_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DLOAD: pWind->DrawImage("images\\Menu\\left_bar_load_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case DEXIT:pWind->DrawImage("images\\Menu\\left_bar_exit_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						default: pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						}
					}
					oldLeftHoverItem = HoveredLeftItemOrder;
				}
				else if (hoverY >= 0 && hoverY < UI.height && UI.AppMode == SIMULATION && hoverX <= UI.LeftToolBarWidth - 12 && hoverX >= 5) {
					// Left bar hovering in simulation mode
					//Always Clear hover Bar if found
					clearHoveringImage(imgh, J, K, widthh);
					vector<pair<int, int> > LeftItemsRanges;
					LeftItemsRanges.push_back(make_pair(6, 85));
					LeftItemsRanges.push_back(make_pair(103, 180));
					LeftItemsRanges.push_back(make_pair(197, 273));

					int HoveredLeftItemOrder = -1;

					for (size_t i = 0; i < LeftItemsRanges.size(); i++)
					{
						if (hoverY >= LeftItemsRanges[i].first && hoverY <= LeftItemsRanges[i].second) {
							HoveredLeftItemOrder = i;
						}
					}
					if (HoveredLeftItemOrder != oldLeftHoverItem)
					{
						switch (HoveredLeftItemOrder)
						{
						case SDESIGN: pWind->DrawImage("images\\Menu\\left_bar_simulation_design_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case STT:pWind->DrawImage("images\\Menu\\left_bar_simulation_tt_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						case SEXIT: pWind->DrawImage("images\\Menu\\left_bar_simulation_exit_hovered.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						default: pWind->DrawImage("images\\Menu\\left_bar_simulation_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
						}
					}
					oldLeftHoverItem = HoveredLeftItemOrder;
				}
				else {

					oldTopHoveredItemOrder = oldLeftHoverItem = -1;
					// if not hovering any toolbar redraw the normal toolbars
					if (UI.AppMode == DESIGN)
					{
						pWind->DrawImage("images\\Menu\\top_bar_normal.jpg", UI.LeftToolBarWidth, 0, UI.width - UI.LeftToolBarWidth - 14, UI.TopToolBarHeight);
						pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
					}
					else {
						pWind->DrawImage("images\\Menu\\left_bar_simulation_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height);
					}

					//Check if hovering a component
					Utils::correctPointClicked(hoverX, hoverY, true, false);
					if (Utils::CheckPointInBorders(hoverX, hoverY)) {
						Component* comp = Utils::getArrayOfComponents(hoverY / UI.GRID_SIZE, hoverX / UI.GRID_SIZE);
						int pin = -1;

						if (comp != NULL && !comp->getDelete())
						{
							//if hovering pins, get pin number and sent to print hovering function
							GraphicsInfo compCorner = comp->getCornersLocation();
							if (hoverX >= compCorner.x2 - UI.GATE_Width / 4 && hoverY > (compCorner.y1 + UI.GATE_Height / 2) - 6 && hoverY < (compCorner.y1 + UI.GATE_Height / 2) + 6)
							{
								pin = 3;//output pin 
							}
							else if (hoverX < (compCorner.x1 + UI.GATE_Width / 4) && hoverY <= compCorner.y2 - UI.GATE_Height / 2 - 6 && (comp->getNumOfInputs() == 2 || comp->getNumOfInputs() == 3)) { pin = 0; }
							else if (hoverX < (compCorner.x1 + UI.GATE_Width / 4) && hoverY >= compCorner.y2 - UI.GATE_Height / 2 + 6 && (comp->getNumOfInputs() == 2 || comp->getNumOfInputs() == 3)) { pin = 2; }
							else if (hoverX < (compCorner.x1 + UI.GATE_Width / 4) && hoverY >= compCorner.y2 - UI.GATE_Height / 2 - 6 && hoverY <= compCorner.y2 - UI.GATE_Height / 2 + 6 && (comp->getNumOfInputs() == 1 || comp->getNumOfInputs() == 3)) { pin = 1; }

							// check if on the same component or not on the same pin
							if (comp != preComp || (UI.AppMode == SIMULATION && comp == preComp && pin != prePin))
							{
								//Always Clear hover Bar if found at the transition between one component and the other
								clearHoveringImage(imgh, J, K, widthh);
								if ((UI.AppMode == DESIGN && comp->getLabel().size() > 0) || (UI.AppMode == SIMULATION && ((dynamic_cast<Connection*>(comp)) || (!(dynamic_cast<Connection*>(comp)))))) { //The Gate Worth Drawing A hover Part
									pWind->GetMouseCoord(J, K);
									delete imgh;
									imgh = NULL;
									imgh = printHovering(J, K, comp->getLabel(), widthh, comp, pin);
								}
							}
						}
						else if (comp == NULL) {
							//Always Clear hover Bar if found at the transition between one component and the other
							clearHoveringImage(imgh, J, K, widthh);
						}
						preComp = comp;
						prePin = pin;
					}
				}
				hoverYOld = hoverY;
				hoverXOld = hoverX;
			}
		}
	}
	//Get user clicks
	if (UI.AppMode == DESIGN)	//application is in design mode
	{
		//[1] If user clicks on the Toolbar
		if (y >= 5 && y < UI.ToolBarHeight - 20 && x > UI.LeftToolBarWidth)
		{
			pWind->GetMouseClick(x, y);
			//Check whick Menu item was clicked
			//==> This assumes that menu items are lined up horizontally <==

			int ClickedItemOrder = -1;
			vector<pair<int, int> > TopItemsRanges;
			TopItemsRanges.push_back(make_pair(107, 170));
			TopItemsRanges.push_back(make_pair(190, 245));
			TopItemsRanges.push_back(make_pair(275, 332));
			TopItemsRanges.push_back(make_pair(360, 415));
			TopItemsRanges.push_back(make_pair(443, 496));
			TopItemsRanges.push_back(make_pair(527, 592));
			TopItemsRanges.push_back(make_pair(612, 662));
			TopItemsRanges.push_back(make_pair(697, 753));
			TopItemsRanges.push_back(make_pair(782, 836));
			TopItemsRanges.push_back(make_pair(867, 922));
			TopItemsRanges.push_back(make_pair(953, 1010));
			TopItemsRanges.push_back(make_pair(1035, 1091));
			TopItemsRanges.push_back(make_pair(1121, 1166));

			for (size_t i = 0; i < TopItemsRanges.size(); i++)
			{
				if (x >= TopItemsRanges[i].first - 5 && x <= TopItemsRanges[i].second + 3) {
					ClickedItemOrder = i;
				}
			}

			//Divide x coord of the point clicked by the menu item width (int division)
			//if division result is 0 ==> first item is clicked, if 1 ==> 2nd item and so on
			pWind->FlushMouseQueue();
			if (s == LEFT_CLICK)
			{
				switch (ClickedItemOrder)
				{
				case D2AND:  return ADD_AND_GATE_2;
				case D3AND:  return ADD_AND_GATE_3;
				case D2OR:  return ADD_OR_GATE_2;
				case D2XOR: return ADD_XOR_GATE_2;
				case D3XOR:  return ADD_XOR_GATE_3;
				case DBUFFER:  return ADD_Buff;
				case DNOT:  return ADD_INV;
				case D2NAND:  return ADD_NAND_GATE_2;
				case D2NOR:  return ADD_NOR_GATE_2;
				case D3NOR:  return ADD_NOR_GATE_3;
				case D2XNOR:  return ADD_XNOR_GATE_2;
				case DSWITCH:  return ADD_Switch;
				case DLED: return ADD_LED;
				default: return DSN_TOOL;	//A click on empty place in desgin toolbar
				}
			}

		}
		//Clicking the left bar icons
		else if (y >= 0 && y < UI.height && UI.AppMode == DESIGN && x <= UI.LeftToolBarWidth - 12 && x >= 5) {
			pWind->GetMouseClick(x, y);
			vector<pair<int, int> > LeftItemsRanges;
			LeftItemsRanges.push_back(make_pair(6, 85));
			LeftItemsRanges.push_back(make_pair(103, 180));
			LeftItemsRanges.push_back(make_pair(197, 273));
			LeftItemsRanges.push_back(make_pair(286, 366));
			LeftItemsRanges.push_back(make_pair(382, 460));
			LeftItemsRanges.push_back(make_pair(475, 550));

			// get item number depending on its order
			int ClickedItemOrder = -1;

			for (size_t i = 0; i < LeftItemsRanges.size(); i++)
			{
				if (y >= LeftItemsRanges[i].first && y <= LeftItemsRanges[i].second) {
					ClickedItemOrder = i;
				}
			}

			//return actions
			switch (ClickedItemOrder + 14)
			{
			case DSIMULATION: {
				// Check if there exits a component in the grid and not deleted to start simulation
				bool returnValidation = false;
				for (size_t i = 0; i < 44; i++)
				{
					for (size_t j = 0; j < 77; j++) {
						if (Utils::getArrayOfComponents(i, j) != NULL && !Utils::getArrayOfComponents(i, j)->getDelete())
						{
							returnValidation = true;
						}
					}
				}

				if (returnValidation) { return SimulateAction; };
				break; }
			case DNEW:return NEW; break;
			case DSAVE:return SAVE; break;
			case DLOAD:return LOAD; break;
			case DEXIT:return EXIT; break;
			default:  pWind->DrawImage("images\\Menu\\left_bar_normal.jpg", 0, 0, UI.LeftToolBarWidth, UI.height); break;
			}
		}
		//[2] User clicks on the drawing area
		if (y > UI.ToolBarHeight && y < UI.height - UI.StatusBarHeight)
		{
			//user want to select/unselect a statement i;
			if (s == RIGHT_CLICK) {
				//Always Clear hover Bar if found
				clearHoveringImage(imgh, J, K, widthh);
				return RIGHT_CLICKSELECT;
			}
			else {
				//Always Clear hover Bar if found
				clearHoveringImage(imgh, J, K, widthh);
				pWind->GetMouseClick(x, y, true); //Remove the last Saved Click
			}
		}

		//[3] User clicks on the status bar
		return STATUS_BAR;
	}
	else	//Application is in Simulation mode
	{
		pWind->GetMouseClick(x, y);

		if (y >= 0 && y < UI.height && x <= UI.LeftToolBarWidth - 12 && x >= 5) {
			//Get action depending on the number of the icon
			vector<pair<int, int> > LeftItemsRanges;
			LeftItemsRanges.push_back(make_pair(6, 85));
			LeftItemsRanges.push_back(make_pair(103, 180));
			LeftItemsRanges.push_back(make_pair(197, 273));

			int ClickedItemOrder = -1;

			for (size_t i = 0; i < LeftItemsRanges.size(); i++)
			{
				if (y >= LeftItemsRanges[i].first && y <= LeftItemsRanges[i].second) {
					ClickedItemOrder = i;
				}
			}
			switch (ClickedItemOrder) {
			case SDESIGN: {return DSN_MODE;; }
			case STT: return Create_TruthTable;
			case SEXIT: return EXIT;
			default: return DSN_TOOL;
			}
		}
		else if (y >= UI.TopToolBarHeight && y < UI.height && x >= UI.LeftToolBarWidth && x < UI.width) {
			// if user clicks a switch in simulation mode we execute change switch action
			Component* comp = Utils::getArrayOfComponents(y / UI.GRID_SIZE, x / UI.GRID_SIZE);
			if (comp != NULL)
			{
				GraphicsInfo compCorners = comp->getCornersLocation();
				if (!((!dynamic_cast<SWITCH*>(comp) || comp->getDelete()) && x >= compCorners.x1&&x <= compCorners.x2&& y >= compCorners.y1&&y <= compCorners.y2)) {
					toBeChangedSwitch = (SWITCH*)comp;
					pWind->FlushMouseQueue();
					clearHoveringImage(imgh, J, K, widthh);
					return Change_Switch;
				}
			}
		}
		//Always Clear hover Bar if found
		clearHoveringImage(imgh, J, K, widthh);
		return SELECT_SIM;
	}
	pWind->FlushMouseQueue();
}

string Input::getStringBox(string s)
{
	return pWind->getStringBox(s);
}
string Input::getSavePath()
{
	return pWind->getSaveFilePath();
}
string Input::getLoadPath()
{
	return pWind->getLoadFilePath();
}
SWITCH * Input::getToBeChangedSwitch()
{
	return toBeChangedSwitch;
}
void Input::setComponentToBeAddedToSelected(Component* C)
{
	toBeAddedToSelected = C;
}
Component * Input::getComponentToBeAddedToSelected()
{
	return 	toBeAddedToSelected;
}
Component * Input::getComponentToBeRemovedFromSelected()
{
	return toBeRemovedFromSelected;
}
void Input::setComponentToBeRemovedFromSelected(Component * C)
{
	toBeRemovedFromSelected = C;
}
buttonstate Input::GetButtonStatus(const button btMouse, int &iX, int &iY) const {
	return pWind->GetButtonState(btMouse, iX, iY);
}
void Input::WaitSelectionPoint(int &X, int &Y)
{
	pWind->WaitMouseClick(X, Y);
}

void Input::setisSelectionContainConnections(bool b)
{
	isSelectionContainConnections = b;
}

void Input::setSelectMode(bool b)
{
	isSelectMode = b;
}
bool Input::getisSelectionContainConnections()
{
	return isSelectionContainConnections;
}
bool Input::getSelectMode()
{
	return isSelectMode;
}

void Input::setSelectedComponents(vector<pair<int, Component*> > comp)
{
	selectedComponents = comp;
}

vector<pair<int, Component*> >& Input::getSelectedComponents()
{
	return selectedComponents;
}

void Input::CorrectPointClickedSilent(int& x, int& y, bool drawImage, bool drawConnection)
{
	Utils::correctPointClicked(x, y, drawImage, drawConnection);
}

void Input::getExactConnectionLocation(int & x, int & y)
{
	Utils::correctPointClicked(x, y, true, false);
}

void Input::clearSelectedComponents()
{
	selectedComponents.clear();
}
Input::~Input()
{
}
