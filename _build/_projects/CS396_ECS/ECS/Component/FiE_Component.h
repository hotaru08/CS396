/******************************************************************************
filename:	FiE_Component.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:

Description:

	Contains information regarding components and functionalities to interact
	with them.

	Questions:
		- std::function<void(std::byte*)> vs void(std::byte*)?
		- std::variant vs union?
		- info_v, why xECS has 2 of them?
******************************************************************************/
namespace FireflyEngine::component
{
	// ------------------------------------------------------------------------
	// Holds important information shared among components to work with
	// ------------------------------------------------------------------------
	class info final
	{
		// Aliases of function type declaractions
		using constructor_fn	 = void(std::byte* _src) noexcept;
		using destructor_fn		 = void(std::byte* _src) noexcept;
		using move_fn			 = void(std::byte* _src, std::byte* _dest) noexcept;

	public:

		// Aliases of pointers to functions
		using constructor_ptr_fn = constructor_fn*;
		using destructor_ptr_fn  = destructor_fn*;
		using move_ptr_fn		 = move_fn*;

		// Constant definitions
		static constexpr    std::uint64_t INVALID_ID_V = -1; //<! UID used to define a invalid id

		std::uint64_t		m_UID;							 //<! Unique ID of component (for tracking)
		std::uint64_t		m_size;							 //<! Size of component (memory space usage)
															 
		constructor_ptr_fn	m_pConstructor;					 //<! Pointer to a constructor function
		destructor_ptr_fn	m_pDestructor;					 //<! Pointer to a destructor function
		move_ptr_fn			m_pMoveFunc;					 //<! Pointer to a move function
	};


	// ------------------------------------------------------------------------
	// Creating and retrieving info of components
	// ------------------------------------------------------------------------
	template < typename Component >
	consteval info CreateInfo() noexcept
	{
		info newInfo{ INVALID_ID_V, sizeof(Component) };

		// Handling for trivially constructable (primitive types) - no need to call special functions
		newInfo.m_pConstructor = 
			std::is_trivially_constructible<Component> ? nullptr : 
			[](std::byte* _src)
			{ 
				new (_src) Component
			};

		newInfo.m_pDestructor  = 
			std::is_trivially_constructible<Component> ? nullptr : 
			[](std::byte* _src)
			{ 
				std::destroy_at(reinterpret_cast<Component*>(_src))
			};

		newInfo.m_pMoveFunc    =
			std::is_trivially_constructible<Component> ? nullptr :
			[](std::byte* _src, std::byte* _dest) 
			{ 
				reinterpret_cast<Component*>(_src) = std::move(reinterpret_cast<Component*>(_dest)
			};

		return newInfo;
	}

	// Information value retrieved from component
	// std::decay to retrieve base type for using as index to retrieve info
	template < typename T >
	static constexpr auto info_v = CreateInfo< std::decay< T > >();


	// ------------------------------------------------------------------------
	// Entity information
	// ------------------------------------------------------------------------




}
