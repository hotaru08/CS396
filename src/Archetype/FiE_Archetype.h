/******************************************************************************
filename:	FiE_Archetype.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype defines the components that an entities has. It also provides
	functionality to interact with entities and their components.

******************************************************************************/
namespace FireflyEngine::archetype
{
	// ------------------------------------------------------------------------
	// Archetype Information
	// ------------------------------------------------------------------------
	struct Archetype final
	{
		// Aliases of type declarations
		using component_info_t = const component::Info* const;
		using info_array_t		= std::array< std::byte*, sharedinfo::max_num_components_per_entity_v >;


		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------


		Archetype() noexcept;
		Archetype(std::span < component_info_t > _componentInfos) noexcept;
		~Archetype() noexcept;

		Archetype(const Archetype& _otherInst) noexcept				= delete; // Unable to copy archetype's data
		Archetype& operator=(const Archetype& _otherInst) noexcept	= delete;

		Archetype(const Archetype&& _otherInst) noexcept			= delete; // Unable to move archetype's data
		Archetype& operator=(const Archetype&& _otherInst) noexcept = delete;


	private:

		info_array_t		m_pCompInfos;	//<! Container storing pointers to components' info
		pool::ArchetypePool m_pool;			//<! Pool that manages the components and entities of archetype

	};
}

//#include <Archetype\FiE_Archetype.hpp>