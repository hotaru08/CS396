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
		using component_info_t	= const component::Info*;
		using info_array_t		= std::array< component_info_t, sharedinfo::max_num_components_per_entity_v >;


		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------

		//Archetype() noexcept;
		Archetype(
			const std::span < component_info_t >& _componentInfos,
			const tools::Bits& _bits) noexcept;

		Archetype(const Archetype& _otherInst) noexcept				= delete; // Unable to copy archetype's data
		Archetype& operator=(const Archetype& _otherInst) noexcept	= delete;

		Archetype(const Archetype&& _otherInst) noexcept			= delete; // Unable to move archetype's data
		Archetype& operator=(const Archetype&& _otherInst) noexcept = delete;


		// ------------------------------------------------------------------------
		// Archetype Functions
		// ------------------------------------------------------------------------

		// Creates a new entity of current archetype

		//entity::Entity& CreateEntity() noexcept;


		// Destroys an entity of current archetype
		void DestroyEntity(entity::Entity& _entity) noexcept;


		// ------------------------------------------------------------------------
		// Structural Changes Functions
		// ------------------------------------------------------------------------

		void UpdateStructuralChanges() noexcept;


		//void AccessGuard() noexcept;

	private:

		info_array_t				  m_pCompInfos;		  //<! Container storing pointers to components' info
		tools::Bits					  m_compSignature;	  //<! Bit signature of flagged components that archetype has
		std::vector< entity::Entity > m_toDeleteEntities; //<! Stores the entities that are to be deleted (prevent structural changes)
		
		pool::ArchetypePool			  m_pool;			  //<! Pool that manages the components and entities of archetype
		std::uint32_t				  m_processesRunning; //<! Number of systems running, to ensure only update after all systems
	};
}

#include <Archetype\FiE_Archetype.hpp>
