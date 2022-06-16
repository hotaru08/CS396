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

static struct Window
{
	std::unique_ptr<FireflyEngine::ECS::Manager> m_ecsManager = 
		std::make_unique<FireflyEngine::ECS::Manager>();

	std::uint32_t m_width	= 1024;
	std::uint32_t m_height	= 800;
	std::uint32_t m_posX	= 0;
	std::uint32_t m_posY	= 0;

} sg_gameWindow;

struct Vector2D
{
	float x, y;

	Vector2D(const float _x = 0.f, const float _y = 0.f)
		: x{ _x }, y{ _y }
	{ }

	void operator()()
	{
	}
};

struct Position
{
	Vector2D pos{ 5.f, 5.f };
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

	Scale& operator=(const Scale&& _rhs) noexcept
	{
		m_sx = _rhs.m_sx;
		m_sy = _rhs.m_sy;

		delete ptr;
		ptr = _rhs.ptr;

		std::cout << "Called Scale move" << std::endl;
		return *this;
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

void Foo()
{
	std::cout << "called in function" << std::endl;
}

/* Test 01 - Registering components */
void TestCases1()
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
void TestCase2()
{
	std::cout << "\033[1m\033[33m" << "\n----- START TEST 02 -----\n" << "\033[0m\033[37m" << std::endl;
	using info_type = FireflyEngine::archetype::Pool::component_info_t;
	constexpr auto info_size_v = 3;

	std::array< info_type, info_size_v> arr
	{
		&FireflyEngine::component::info_v< Rotation >,
		&FireflyEngine::component::info_v< Scale >,
		&FireflyEngine::component::info_v< Position >
	};

	FireflyEngine::archetype::Pool poolInst
	{
		std::span< info_type >{ arr.data(), arr.size() }
	};

	const auto index = poolInst.Append();
	const auto index2 = poolInst.Append();
	const auto index3 = poolInst.Append();

	std::cout << "Finding Scale Component in Pool ... " << std::endl;
	const std::int32_t findCompIndex = poolInst.FindComponentType(arr[1]->m_uid);

	findCompIndex > 0 ?
		std::cout << "Found at " << findCompIndex << std::endl :
		std::cout << "Component not found." << std::endl;

	//auto& comp = poolInst.GetComponent< Vector2D >(index); // Test assert
	auto& comp = poolInst.GetComponent< Rotation >(index);

	std::cout
		<< "Rotation uid: "
		<< FireflyEngine::component::info_v < decltype(comp) >.m_uid
		<< std::endl; // shld be 1 (order of registration)

	std::cout << "Rotation along z-axis: " << comp.m_rz << std::endl;

	poolInst.Delete(index);
	std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
}

/* Test 02.5 - Function traits */
void TestCase2n5()
{
	std::cout << "\033[1m\033[33m" << "\n----- START TEST 02.5 -----\n" << "\033[0m\033[37m" << std::endl;

	std::cout << "\033[1m\034[33m" << "\n----- Function Traits -----\n" << "\033[0m\033[37m" << std::endl;

	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< int() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< void(*)() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< void* (*)() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< int* (*)() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< int& (*)() >::return_type_t).name() << std::endl;

	auto l = []()
	{
		return []()
		{
			return true;
		};
	};
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(l) > ::return_type_t).name() << std::endl;

	Vector2D vec2;
	Vector2D* pVec2;
	std::cout << typeid(Vector2D).name() << std::endl;
	std::cout << typeid(decltype(&Vector2D::operator())).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(&Vector2D::operator()) > ::return_type_t).name() << std::endl;

	std::cout << "------" << std::endl;
	std::cout << typeid(decltype(vec2)).name() << std::endl;
	std::cout << typeid(decltype(pVec2)).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(vec2) >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(pVec2) >::return_type_t).name() << std::endl;

	std::cout << "\033[1m\034[33m" << "\n----- Tuple Traits -----\n" << "\033[0m\033[37m" << std::endl;


	std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
}

/* Test 03 - Archetype, Signatures, Pool */
void TestCase3()
{
	std::cout << "\033[1m\033[33m" << "\n----- START TEST 03 -----\n" << "\033[0m\033[37m" << std::endl;
	FireflyEngine::tools::Bits bits;
	bits.SetBitsFromComponents < Position >();
	FireflyEngine::entity::Manager inst;

	using info_type = FireflyEngine::archetype::Archetype::component_info_t;
	constexpr auto info_size_v = 3;

	std::array< info_type, info_size_v> arr
	{
		&FireflyEngine::component::info_v< Rotation >,
		&FireflyEngine::component::info_v< Scale >,
		&FireflyEngine::component::info_v< Position >
	};

	FireflyEngine::archetype::Archetype archetype
	{
		std::span< info_type >{ arr.data(), arr.size() },
		bits,
		inst
	};
	//archetype.CreateEntity([]() {});

	std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
}

void TestCases()
{
	TestCase1();
	TestCase2();
	TestCase2n5();
	TestCase3();
}

void Init()
{
	//
	// Register all the elements of the game eg. components and systems


	//
	// Generate game entities
	std::srand(100);
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
	//{
	//	glutInitWindowSize(sg_gameWindow.m_width, sg_gameWindow.m_height);
	//	glutInitWindowPosition(sg_gameWindow.m_posX, sg_gameWindow.m_posY);
	//
	//	glutInit(&argc, argv);
	//	glutCreateWindow("CS396 Assignment 01");

	//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	//	glutReshapeFunc
	//	(
	//		[](int w, int h)
	//		{
	//			sg_gameWindow.m_width	= w;
	//			sg_gameWindow.m_height	= h;
	//			glViewport(0, 0, w, h);
	//			glMatrixMode(GL_PROJECTION);
	//			glLoadIdentity();
	//			glOrtho(0, w, 0, h, -1, 1);
	//			glScalef(1, -1, 1);
	//			glTranslatef(0, -h, 0);
	//		}
	//	);
	//	glutDisplayFunc
	//	(
	//		[]()
	//		{
	//			sg_gameWindow.m_ecsManager->Update();
	//		}
	//	);

	//	glutMainLoop();
	//}

	std::cout << "Exiting application ..." << std::endl;
}
