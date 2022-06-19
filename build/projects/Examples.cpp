/******************************************************************************
filename:	Examples.cpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Entry point of the application.

******************************************************************************/
#include <FiE_ECS_includes.h>

//#define FREEGLUT_STATIC
#include "GL/freeglut.h"
#include <random>

static struct Window
{
	std::unique_ptr<FireflyEngine::ECS::Manager> m_ecsManager = 
		std::make_unique<FireflyEngine::ECS::Manager>();

	std::uint32_t m_width	= 1024;
	std::uint32_t m_height	= 800;
	std::uint32_t m_posX	= 0;
	std::uint32_t m_posY	= 0;

} sg_game;

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
void TestCase1()
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
	const std::int32_t findCompIndex = poolInst.FindComponentTypeIndex(arr[1]->m_uid);

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

	std::cout << "\033[1m\033[34m" << "\n----- Function Traits -----\n" << "\033[0m\033[37m" << std::endl;

	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< int() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< void(*)() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< void* (*)() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< int* (*)() >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits< int& (*)() >::return_type_t).name() << std::endl;

	std::cout << "lambda" << std::endl;
	auto l = []()
	{
	};
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(l) > ::return_type_t).name() << std::endl;

	Vector2D vec2;
	Vector2D* pVec2;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(&Vector2D::operator()) > ::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < Vector2D > ::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < Vector2D* > ::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < Vector2D& > ::return_type_t).name() << std::endl;

	std::cout << "------" << std::endl;
	std::cout << typeid(decltype(vec2)).name() << std::endl;
	std::cout << typeid(decltype(pVec2)).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(vec2) >::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(pVec2) >::return_type_t).name() << std::endl;

	std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
}

/* Test 03 - Archetype, Queries */
void TestCase3()
{
	std::cout << "\033[1m\033[33m" << "\n----- START TEST 03 -----\n" << "\033[0m\033[37m" << std::endl;
	FireflyEngine::tools::Bits bits;
	bits.SetBitsFromComponents < Position >();
	std::unique_ptr<FireflyEngine::entity::Manager> inst =
		std::make_unique<FireflyEngine::entity::Manager>();

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
		*inst
	};

	// ------------------------------------------------------------------------
	FireflyEngine::query::instance testQuery, testQuery1;

	testQuery.AddToQueryFromFunction(
		[](Position& pos, Rotation& rot, Scale* scale)
		{
		}
	);

	std::tuple
	<
		FireflyEngine::query::MustHave< Position >,
		FireflyEngine::query::OneOf   < Rotation >
	> testTuple;
	testQuery1.AddToQueryFromTuple(&testTuple);

	std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
}

/* Test 04 - Creating Entities  */
void TestCase4()
{
	std::cout << "\033[1m\033[33m" << "\n----- START TEST 04 -----\n" << "\033[0m\033[37m" << std::endl;
	sg_game.m_ecsManager->RegisterComponents<
		Position,
		Rotation,
		Scale
	>();

	FireflyEngine::entity::Entity ent1 = sg_game.m_ecsManager->CreateEntity<
		Position,
		Rotation
	>();

	FireflyEngine::entity::Entity ent2 = sg_game.m_ecsManager->CreateEntity<
		Rotation,
		Position
	>();

	FireflyEngine::entity::Entity ent3 = sg_game.m_ecsManager->CreateEntity<
		Rotation,
		Scale
	>();

	sg_game.m_ecsManager->CreateEntity(
		[](Position&, Rotation&, Scale&)
		{
			std::cout << "Hello from the other sideeeeee" << std::endl;
		}
	);

	std::cout << "Ent1: " << ent1.m_infoIndex << " / " << ent1.m_validation.m_generation << std::endl;
	std::cout << "Ent2: " << ent2.m_infoIndex << " / " << ent2.m_validation.m_generation << std::endl;
	std::cout << "Ent3: " << ent3.m_infoIndex << " / " << ent3.m_validation.m_generation << std::endl;

	std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
}



void TestCase5()
{

}

void TestCases()
{
	//TestCase1();
	TestCase2();
	TestCase2n5();
	TestCase4();
	TestCase3();
}

int main(int argc, char** argv)
{
	// Enable run-time memory check for debug builds.
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	srand(100);
	TestCases();

	// Setup Window Instance, Graphics and GameLoop
	{
		glutInitWindowSize(sg_game.m_width, sg_game.m_height);
		glutInitWindowPosition(sg_game.m_posX, sg_game.m_posY);
	
		glutInit(&argc, argv);
		glutCreateWindow("CS396 Assignment 01");
		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutReshapeFunc
		(
			[](int w, int h)
			{
				float fw = static_cast<float>(w);
				float fh = static_cast<float>(h);

				sg_game.m_width	= w;
				sg_game.m_height	= h;
				glViewport(0, 0, w, h);
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(0, fw, 0, fh, -1, 1);
				glScalef(1, -1, 1);
				glTranslatef(0, -fh, 0);
			}
		);
		glutDisplayFunc
		(
			[]()
			{
				sg_game.m_ecsManager->Run();
			}
		);

		glutMainLoop();
	}

	std::cout << "Exiting application ..." << std::endl;
}
