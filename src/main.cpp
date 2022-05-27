/******************************************************************************
filename:	main.cpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Entry point of the application.
******************************************************************************/
#include <FiE_ECS_includes.h>

struct Vector2D
{
	float x, y;
};

struct Position
{
	float m_px;
	float m_py;
};

struct Scale
{
	float m_sx;
	float m_sy;
};

struct Rotation
{
	float m_rx;
	float m_ry;
};

int main(void)
{
	std::unique_ptr<FireflyEngine::component::ComponentMgr> compMgr = 
		std::make_unique<FireflyEngine::component::ComponentMgr>();

	compMgr->RegisterComponents<
		Position,
		Rotation,
		Scale
	>();


	std::cout << FireflyEngine::component::ref_info_v< Scale >.m_UID << std::endl;
	std::cout << FireflyEngine::component::ref_info_v< Scale >.m_UID << std::endl;


	



}
