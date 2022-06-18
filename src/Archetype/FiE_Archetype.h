/******************************************************************************
filename:	FiE_Archetype.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype defines the components that an entities has. It also provides
	functionality to interact with entities and their components.

******************************************************************************/
#pragma once

namespace FireflyEngine::entity
{
	struct Manager;
}

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

		Archetype(
			const std::span < component_info_t > _componentInfos,
			const tools::Bits& _bits,
			entity::Manager& _entityManager) noexcept;

		Archetype(const Archetype& _otherInst) noexcept				= delete; // Unable to copy archetype's data
		Archetype& operator=(const Archetype& _otherInst) noexcept	= delete;

		Archetype(const Archetype&& _otherInst) noexcept			= delete; // Unable to move archetype's data
		Archetype& operator=(const Archetype&& _otherInst) noexcept = delete;


		// ------------------------------------------------------------------------
		// Archetype Functions
		// ------------------------------------------------------------------------

		// Appends a new entity ( the components infos that the entities has to pool)
		sharedinfo::entity_index_t AddNewEntity() noexcept;

		// Gets the component of an Entity
		template < typename Component >
		Component& GetComponent(const sharedinfo::entity_index_t _entityIndex) const noexcept;
		
		// Destroys an entity of current archetype
		void DestroyEntity(entity::Entity& _entity) noexcept;


		// ------------------------------------------------------------------------
		// Structural Changes Functions
		// ------------------------------------------------------------------------	

		// Update changes that would cause structural updates, leading to UDB
		void UpdateStructuralChanges() noexcept;

		// Ensure that only when this archetype is not being used by anything else,
		// then update changes that disrupts structure of systems using this archetype
		template < tools::traits::is_empty_fn CallbackType = sharedinfo::empty_lambda_t >
		void AccessGuard(CallbackType&& _callbackFunc = sharedinfo::empty_lambda_t{}) noexcept;

	private:

		info_array_t				     m_pCompInfos;		  //<! Container storing pointers to components' info
		tools::Bits					     m_compSignature;	  //<! Bit signature of flagged components that archetype has
		std::vector<entity::Entity>      m_toDeleteEntities;  //<! Stores the entities that are to be deleted (prevent structural changes)
									     
		entity::Manager&			     m_entityManager;	  //<! Reference to manager that handles the entities
		archetype::Pool					 m_currPool;		  //<! Pool that manages the components of archetype
		std::uint32_t				     m_processesRunning;  //<! Number of systems running, to ensure only update after all systems

	};
}

#include <Archetype\FiE_Archetype.hpp>
