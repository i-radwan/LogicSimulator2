#include "Gate.h"

//Gate Constructor
//Parameters:
//r_Inputs: no. of gate's input pins
//r_FanOut: Fan out of the gate's output pin
Gate::Gate(int r_Inputs, int r_FanOut, GraphicsInfo r_GfxInfo) :Component(r_GfxInfo, r_FanOut)
{
	//Allocate number of input pins (equals r_Inputs)
	m_InputPins = new InputPin[r_Inputs];
	setNumOfInputs(r_Inputs);	//set no. of inputs of that gate
	//Associate all input pins to this gate
	for (int i = 0; i < getNumOfInputs(); i++)
		m_InputPins[i].setComponent(this);
	for (size_t i = 0; i < r_Inputs; i++)
	{
		m_InputPins[i].setConnection(NULL);
	}

	m_OutputPin.setComponent(this);
}
Gate::~Gate() {
	delete[] m_InputPins;
}