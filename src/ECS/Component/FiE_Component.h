/******************************************************************************
filename:	FiE_Component.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Contains information regarding components and functionalities.

	Questions:
		- component uid
		- what is size for again and how it is assigned / used
******************************************************************************/
namespace FireflyEngine::component
{
	// ------------------------------------------------------------------------
	// Information for managing data of a component
	// ------------------------------------------------------------------------
	class info final
	{
		// Aliases of function type declarations
		using constructor_fn	 = void(std::byte* _src) noexcept;
		using destructor_fn		 = void(std::byte* _src) noexcept;
		using move_fn			 = void(std::byte* _src, std::byte* _dest) noexcept;

	public:

		// Aliases of pointers to functions
		using constructor_ptr_fn = constructor_fn*;
		using destructor_ptr_fn  = destructor_fn*;
		using move_ptr_fn		 = move_fn*;

		// Constant definitions
		static constexpr sharedinfo::component_uid_t invalid_info_v = 
			static_cast<sharedinfo::component_uid_t>(-1);		//<! UID used to define a invalid id

		mutable sharedinfo::component_uid_t	 m_UID;				//<! Unique ID of component
		sharedinfo::component_size_t m_size;					//<! Size of memory used by component

		constructor_ptr_fn	m_pConstructor;						//<! Pointer to a constructor
		destructor_ptr_fn	m_pDestructor;						//<! Pointer to a destructor
		move_ptr_fn			m_pMoveFunc;				 		//<! Pointer to a move function
	};


	// ------------------------------------------------------------------------
	// Creating and retrieving info of components
	// ------------------------------------------------------------------------
	template < typename Component >
	consteval info CreateInfo() noexcept
	{
		return
		{
			.m_UID			= info::invalid_info_v,
			.m_size			= sizeof(Component),

			// Handling for trivially constructable (primitive types) - no need to call special functions
			.m_pConstructor =
				std::is_trivially_constructible_v< Component > ? nullptr :
				[](std::byte* _src) noexcept
				{
					new (_src) Component;
				},

			.m_pDestructor =
				std::is_trivially_destructible_v< Component > ? nullptr :
				[](std::byte* _src) noexcept
				{
					std::destroy_at(reinterpret_cast< Component* >(_src));
				},

			.m_pMoveFunc =
				std::is_trivially_move_assignable_v< Component > ? nullptr :
				[](std::byte* _src, std::byte* _dest) noexcept
				{
					*reinterpret_cast< Component* >(_dest) = std::move(*reinterpret_cast< Component* >(_src));
				}
		};
	}

	
	// Information of the component (only one instance per component)
	template < typename T >
	static constexpr auto info_v = CreateInfo< T >();

	// Reference to component's information
	template < typename T >
	constexpr auto& ref_info_v = info_v< T >;
}
