/******************************************************************************
filename:	Examples.cpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Entry point of the application.
******************************************************************************/
#include <FiE_ECS_includes.h>

// <TODO>: COMMNENT OUT
#define DEVELOPMENT_MODE

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
	int* ptr;

	Scale()
		: m_sx { 1 },
		  m_sy { 1 }, 
		  ptr { new int }
	{

	}

	~Scale()
	{
		m_sx = m_sy = 0;
		delete ptr;
		ptr = nullptr;
	}
};

struct Rotation
{
	float m_rx;
	float m_ry;
};

#define ALL_COMPONENTS \
	Position, Rotation, Scale

void TestCases()
{
	/* Test 01 - Registering components */
	{
		std::unique_ptr<FireflyEngine::component::Manager> compMgr =
			std::make_unique<FireflyEngine::component::Manager>();

		compMgr->RegisterComponents< ALL_COMPONENTS >();

		std::cout << FireflyEngine::component::info_v< Scale >.m_UID << std::endl;
		std::cout << FireflyEngine::component::info_v< Scale >.m_UID << std::endl;
	}

	/* Test 02 - Testing Archetype Pool */
	std::array< const FireflyEngine::component::Info* const, 1> arr{ &FireflyEngine::component::info_v< Scale > };
	FireflyEngine::pool::ArchetypePool pool{ std::span< const FireflyEngine::component::Info* const >{ arr.data(), arr.size() } };
	pool.Delete(pool.Append());
}

void RenderECS()
{

}

int main(void)
{
	// Enable run-time memory check for debug builds.
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef DEVELOPMENT_MODE
	TestCases();
#else
	RenderECS();
#endif
}
