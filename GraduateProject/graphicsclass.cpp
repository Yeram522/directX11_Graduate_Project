////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_Scene = 0;

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_Scene = new Scene("Main");
	if(!m_Scene)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_Scene->Initialize(screenWidth, screenHeight, hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Scene.", L"Error", MB_OK);
		return false;
	}

	

	return true;
}


void GraphicsClass::Shutdown()
{
	if (m_Scene)
	{
		m_Scene->Shutdown();
		delete m_Scene;
		m_Scene = 0;
	}
	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	static float rotation = 0.0f;


	// Update the rotation variable each frame.
	rotation += XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
	
	m_Scene->Render();
	// Present the rendered scene to the screen.
	return true;
}


