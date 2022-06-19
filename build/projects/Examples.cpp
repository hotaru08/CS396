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
#include <Misc\Vector2D.h>
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

// ----------------------------------------------------------------------------

struct Vector2D_Test
{
	float x, y;

	Vector2D_Test(const float _x = 0.f, const float _y = 0.f)
		: x{ _x }, y{ _y }
	{ }

	void operator()()
	{
	}
};

struct Velocity
{
	Vector2D m_Value;
};

struct Timer
{
	float m_Value;
};

struct Bullet
{
	FireflyEngine::entity::Entity m_ShipOwner;
};

struct Position
{
	Vector2D m_Value;
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

// --------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// SYSTEM
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------

struct UpdateMovement : FireflyEngine::system::BaseSystem
{
	void operator()(Position& _position, Velocity& _vel) const
	{
		_position.m_Value += _vel.m_Value;

		// Bounce on edges
		if (_position.m_Value.x < 0)
		{
			_position.m_Value.x = 0;
			_vel.m_Value.x = -_vel.m_Value.x;
		}
		else if (_position.m_Value.x >= sg_game.m_width)
		{
			_position.m_Value.x = sg_game.m_width - 1;
			_vel.m_Value.x = -_vel.m_Value.x;
		}

		if (_position.m_Value.y < 0)
		{
			_position.m_Value.y = 0;
			_vel.m_Value.y = -_vel.m_Value.y;
		}
		else if (_position.m_Value.y >= sg_game.m_height)
		{
			_position.m_Value.y = sg_game.m_height - 1;
			_vel.m_Value.y = -_vel.m_Value.y;
		}
	}
};

//---------------------------------------------------------------------------------------

struct BulletLogic : FireflyEngine::system::BaseSystem
{
	void operator()(
		FireflyEngine::entity::Entity& entity, 
		Position& position, Timer& timer, Bullet& bullet) const
	{
		// If I am dead because some other bullet killed me then there is nothing for me to do...
		if (entity.IsDead()) return;

		// Update my timer
		timer.m_Value -= 0.01f;
		if (timer.m_Value <= 0)
		{
			m_ecsManager.DeleteEntity(entity);
			return;
		}

		// Check for collisions
		FireflyEngine::query::instance query;
		query.m_mustHaveBits.SetBitsFromComponents<Position>();

		m_ecsManager.ForEachEntity
		(
			query,
			[&](FireflyEngine::entity::Entity& E, Position& Pos)  -> bool
			{
				assert(entity.IsDead() == false);

				// Our we checking against my self?
				if (&entity == &E) return false;

				// Is this bullet or ship already dead?
				if (E.IsDead()) return false;

				// Are we colliding with our own ship?
				// If so lets just continue
				if (bullet.m_ShipOwner.m_entityID == E.m_entityID) return false;

				constexpr auto distance_v = 3;
				if (MagnitudeSquared(Pos.m_Value - position.m_Value) < distance_v * distance_v)
				{
					m_ecsManager.DeleteEntity(entity);
					m_ecsManager.DeleteEntity(E);
					return true;
				}

				return false;
			}
		);
	}
};

//---------------------------------------------------------------------------------------

struct SpaceShipLogic : FireflyEngine::system::BaseSystem
{
	using query = std::tuple
	<
		FireflyEngine::query::NoneOf<Bullet>
	>;

	void operator()(FireflyEngine::entity::Entity& entity, Position& position, Timer& time) const
	{
		if (time.m_Value > 0)
		{
			time.m_Value -= 0.01f;
			return;
		}

		FireflyEngine::query::instance    query;
		query.m_noneOfBits.SetBitsFromComponents<Bullet>();
		m_ecsManager.ForEachEntity
		(
			query,
			[&](Position& Pos) -> bool
			{
				// Don't shoot myself
				if (&Pos == &position) return false;

				auto        Direction = Pos.m_Value - position.m_Value;
				const auto  DistanceSquare = FiE_Math::MagnitudeSquared(Direction);

				// Shoot a bullet if close enough
				constexpr auto min_distance_v = 30;
				if (DistanceSquare < min_distance_v * min_distance_v)
				{
					time.m_Value = 8;
					m_ecsManager.CreateEntity(
						[&](Position& pos, Velocity& vel, Bullet& bullet, Timer& timer)
						{
							Direction /= std::sqrt(DistanceSquare);
							vel.m_Value = Direction * 2.0f;
							pos.m_Value = position.m_Value + vel.m_Value;

							bullet.m_ShipOwner = entity;
							timer.m_Value = 10;
						});
					return true;
				}
				return false;
			}
		);
	}
};

//---------------------------------------------------------------------------------------

struct RenderBullets : FireflyEngine::system::BaseSystem
{
	using query = std::tuple
	<
		FireflyEngine::query::MustHave< Bullet >
	>;

	void operator()(Position& position, Velocity& velocity) const
	{
		constexpr auto SizeX = 1;
		constexpr auto SizeY = SizeX * 3;
		glBegin(GL_TRIANGLES);
		glColor3f(1.0, 0.5, 0.0);
		glVertex2i(position.m_Value.x + velocity.m_Value.x * SizeY, position.m_Value.y + velocity.m_Value.y * SizeY);
		glVertex2i(position.m_Value.x + velocity.m_Value.y * SizeX, position.m_Value.y - velocity.m_Value.x * SizeX);
		glVertex2i(position.m_Value.x - velocity.m_Value.y * SizeX, position.m_Value.y + velocity.m_Value.x * SizeX);
		glEnd();
	}
};

//---------------------------------------------------------------------------------------

struct RenderShips : FireflyEngine::system::BaseSystem
{
	using query = std::tuple
	<
		FireflyEngine::query::NoneOf<Bullet>
	>;

	void operator()(Position& position, Timer& timer) const
	{
		constexpr auto Size = 3;
		glBegin(GL_QUADS);
		if (timer.m_Value > 0) glColor3f(1.0, 1.0, 1.0);
		else                   glColor3f(0.5, 1.0, 0.5);
		glVertex2i(position.m_Value.x - Size, position.m_Value.y - Size);
		glVertex2i(position.m_Value.x - Size, position.m_Value.y + Size);
		glVertex2i(position.m_Value.x + Size, position.m_Value.y + Size);
		glVertex2i(position.m_Value.x + Size, position.m_Value.y - Size);
		glEnd();
	}
};

//---------------------------------------------------------------------------------------

struct PageFlip : FireflyEngine::system::BaseSystem
{
	void OnUpdate() noexcept
	{
		glutSwapBuffers();
		glClear(GL_COLOR_BUFFER_BIT);
	}
};

// ----------------------------------------------------------------------------

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

	Vector2D_Test vec2;
	Vector2D_Test* pVec2;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < decltype(&Vector2D_Test::operator()) > ::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < Vector2D_Test > ::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < Vector2D_Test* > ::return_type_t).name() << std::endl;
	std::cout << typeid(FireflyEngine::tools::traits::fn_traits < Vector2D_Test& > ::return_type_t).name() << std::endl;

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
		[](Position& pos)
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

	FireflyEngine::entity::Entity ent4 = sg_game.m_ecsManager->CreateEntity(
		[](Position&, Rotation&)
		{
			std::cout << "Hello from the other sideeeeee" << std::endl;
		}
	);

	sg_game.m_ecsManager->DeleteEntity(ent2);

	ent2 = sg_game.m_ecsManager->CreateEntity<
		Rotation,
		Position
	>();

	std::cout << "Ent1: " << ent1.m_infoIndex << " / " << ent1.m_validation.m_generation << std::endl;
	std::cout << "Ent2: " << ent2.m_infoIndex << " / " << ent2.m_validation.m_generation << std::endl;
	std::cout << "Ent3: " << ent3.m_infoIndex << " / " << ent3.m_validation.m_generation << std::endl;
	std::cout << "Ent4: " << ent4.m_infoIndex << " / " << ent4.m_validation.m_generation << std::endl;

	std::cout << "\033[1m\033[33m" << "\n----- END TEST -----\n" << "\033[0m\033[37m" << std::endl;
}

/* Test 05 - Iterating through entities  */
void TestCase5()
{
	FireflyEngine::query::instance testQuery, testQuery1;

	testQuery.AddToQueryFromFunction(
		[](Position& pos)
		{
		}
	);
	std::tuple
	<
		FireflyEngine::query::MustHave< Position >,
		FireflyEngine::query::OneOf   < Rotation >
	> testTuple;
	testQuery1.AddToQueryFromTuple(&testTuple);

	sg_game.m_ecsManager->ForEachEntity(
		testQuery1, 
		[](Position&)
		{
			std::cout << "Entered function here" << std::endl;
			return false;
		}
	);
}

void TestCases()
{
	//TestCase1();
	TestCase2();
	TestCase2n5();
	TestCase4();
	TestCase3();
	TestCase5();
}

// ----------------------------------------------------------------------------

/* Initialize the game by registering components and system */
void InitializeGame() noexcept
{
	srand(100);

	// Register Components
	sg_game.m_ecsManager->RegisterComponents
	< 
		Position,
		Rotation,
		Scale, 
		Velocity,
		Bullet,
		Timer
	>();

	sg_game.m_ecsManager->RegisterSystems
	<
		UpdateMovement,		  // Structural: No
		SpaceShipLogic,		  // Structural: Can Create Bullets
		BulletLogic,		  // Structural: Can Destroy Bullets and Ships
		RenderShips,		  // Structural: No
		RenderBullets,		  // Structural: No
		PageFlip			  // Structural: No
	>();

	// Initialize the game
	for (int i = 0; i < 1000; ++i)
	{
		sg_game.m_ecsManager->CreateEntity(
			[&](Position& _position, Velocity& _velocity, Timer& _timer)
			{
				_position.m_Value.x = std::rand() % sg_game.m_width;
				_position.m_Value.y = std::rand() % sg_game.m_height;

				_velocity.m_Value.x = (std::rand() / (float)RAND_MAX) - 0.5f;
				_velocity.m_Value.y = (std::rand() / (float)RAND_MAX) - 0.5f;
				FiE_Math::NormalizeVector(_velocity.m_Value);

				_timer.m_Value = (std::rand() / (float)RAND_MAX) * 8;
			}
		);
	}
}

void timer(int value)
{
	// Post re-paint request to activate display()
	glutPostRedisplay();

	// next timer call milliseconds later
	glutTimerFunc(15, timer, 0);
}

int main(int argc, char** argv)
{
	// Enable run-time memory check for debug builds.
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//TestCases();
	InitializeGame();

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

		glutTimerFunc(0, timer, 0);
		glutMainLoop();
	}

	std::cout << "Exiting application ..." << std::endl;
}
