/******************************************************************************
filename:	Examples.cpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Entry point of the application.

******************************************************************************/
#include <FiE_ECS_includes.h>

#define GLUT_STATIC_LIB
#include "GL/glut.h"
#include <random>

static struct Game
{
	std::unique_ptr<FireflyEngine::ECS::Manager> m_ecsMgr = std::make_unique<FireflyEngine::ECS::Manager>();
	std::int32_t m_width	= 1024;
	std::int32_t m_height	= 800;

} sg_gameInst;;

struct Vector2D
{
	float x, y;
};

struct Position
{
	float m_px = 5.f;
	float m_py = 5.f;
};

struct Scale
{
	float m_sx;
	float m_sy;
	int* ptr;

	Scale()
		: m_sx { 1.f },
		  m_sy { 1.f }, 
		  ptr { new int }
	{
		std::cout << "Called Scale ctor" << std::endl;
	}

	~Scale()
	{
		m_sx = m_sy = 0.f;
		delete ptr;
		ptr = nullptr;

		std::cout << "Called Scale dtor" << std::endl;
	}
};

struct Rotation
{
	float m_rz = 45.f;
};

void TestCases()
{
	/* Test 01 - Registering components */
	{
		std::cout << "\033[1m\033[33m" << "\n----- START TEST 01 -----\n" << "\033[0m\033[37m" << std::endl;
		std::unique_ptr<FireflyEngine::component::Manager> compMgr =
			std::make_unique<FireflyEngine::component::Manager>();

		compMgr->RegisterComponents< 
			Position, 
			Rotation, 
			Scale 
		>();

		std::cout << FireflyEngine::component::info_v< Scale >.m_uid << std::endl;
		std::cout << FireflyEngine::component::info_v< Scale >.m_uid << std::endl;
		std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
	}

	/* Test 02 - Archetype Pool */
	{
		std::cout << "\033[1m\033[33m" << "\n----- START TEST 02 -----\n" << "\033[0m\033[37m" << std::endl;
		using info_type = FireflyEngine::pool::ArchetypePool::component_info_t;
		constexpr auto info_size_v = 3;

		std::array< info_type, info_size_v> arr
		{ 
			&FireflyEngine::component::info_v< Rotation >,
			&FireflyEngine::component::info_v< Scale >,
			&FireflyEngine::component::info_v< Position >
		};

		FireflyEngine::pool::ArchetypePool poolInst 
		{ 
			std::span< info_type >{ arr.data(), arr.size() }
		};

		const auto index = poolInst.Append();

		std::cout << "Finding Scale Component in pool ... " << std::endl;
		const std::int32_t findCompIndex = poolInst.FindComponentType(arr[1]->m_uid);
		
		findCompIndex > 0 ?
			std::cout << "Found at " << findCompIndex << std::endl :
			std::cout << "Component not found." << std::endl;

		//auto& comp = poolInst.GetComponent< Vector2D >(index); // Test assert
		auto& comp = poolInst.GetComponent< Rotation >(index);

		std::cout 
			<< "Rotation uid: "
			<< FireflyEngine::component::info_v < std::decay_t<decltype(comp)> >.m_uid
			<< std::endl; // shld be 1 (order of registration)

		std::cout << "Rotation along z-axis: " << comp.m_rz << std::endl;

		poolInst.Delete(index);
		std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
	}

	/* Test 03 - Archetype, Signatures, Pool */

	/* Test 04 - Registering Systems */

	/* Test 05 - Systems, Archetypes, Query */

	/* Test 06 - Overall Manager (ECS manager) */
}

void Init()
{
	//
	// Register all the elements of the game eg. components and systems


	//
	// Generate game entities
	//std::srand(100);
	//auto& SpaceShipArchetype = s_Game.m_GameMgr->getOrCreateArchetype< position, velocity, timer >();
	//for (int i = 0; i < 1000; i++)
	//{
	//	SpaceShipArchetype.CreateEntity([&](position& Position, velocity& Velocity, timer& Timer)
	//		{
	//			Position.m_Value.m_X = std::rand() % s_Game.m_W;
	//			Position.m_Value.m_Y = std::rand() % s_Game.m_H;

	//			Velocity.m_Value.m_X = (std::rand() / (float)RAND_MAX) - 0.5f;
	//			Velocity.m_Value.m_Y = (std::rand() / (float)RAND_MAX) - 0.5f;
	//			Velocity.m_Value.Normalize();

	//			Timer.m_Value = (std::rand() / (float)RAND_MAX) * 8;
	//		});
	//}
}

int main(int argc, char** argv)
{
	// Enable run-time memory check for debug builds.
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	TestCases();

	// Setup Window Instance, Graphics and GameLoop
	glutInitWindowSize(sg_gameInst.m_width, sg_gameInst.m_height);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutCreateWindow("CS396 Assignment 01");
	glutDisplayFunc([]
		{
			//sg_gameInst.m_ecsMgr->Run();
		}
	);
	glutReshapeFunc([](int w, int h)
		{
			sg_gameInst.m_width = w;
			sg_gameInst.m_height = h;
			glViewport(0, 0, w, h);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0, w, 0, h, -1, 1);
			glScalef(1, -1, 1);
			glTranslatef(0, -h, 0);
		}
	);
	//glutTimerFunc(0, timer, 0);
	glutMainLoop();
}
