/******************************************************************************
filename:	FiE_EntityManager.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that manages the entities and archetypes of the ECS. It provides
	functionalities to interact with entities, which would modify the archetype
	the entity belongs to if neccessary.

******************************************************************************/
#pragma once

namespace FireflyEngine::entity
{
	// ------------------------------------------------------------------------
	// Manager that handles the entities and archetypes
	// ------------------------------------------------------------------------
	struct Manager final
	{
		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------
		
		Manager() noexcept;
		Manager(const Manager& _otherMgr) noexcept				= delete; // Unable to copy, only 1 manager
		Manager& operator=(const Manager& _otherMgr) noexcept	= delete;

		Manager(const Manager&& _otherMgr) noexcept				= delete; // Unable to move, only 1 manager
		Manager& operator=(const Manager&& _otherMgr) noexcept	= delete;


		// ------------------------------------------------------------------------
		// Entities Functions
		// ------------------------------------------------------------------------

		// Creates a new entity with the specified components
		template
		<
			typename... Components, 
			tools::traits::is_return_void_fn CallbackType = sharedinfo::empty_lambda_t
		>
		entity::Entity CreateEntity(CallbackType&& _callbackFunc = sharedinfo::empty_lambda_t{}) noexcept;
		
		

	private:

		//<! Alias to component's imfo
		using component_info_t = const component::Info* const;							

		std::vector< std::unique_ptr< archetype::Archetype > > m_archetypes;         //<! Container storing all archetypes of entities
		std::vector< tools::Bits >							   m_bits;			     //<! Container storing all archetypes signatures
		std::unique_ptr<EntityInfo[]>						   m_entityInfos;		 //<! Pointer to container storing all actual entities information
		sharedinfo::entity_index_t							   m_nextEmptyInfoIndex; //<! Index of next free slot in container of entities information


		// ------------------------------------------------------------------------
		// Helper Functions
		// ------------------------------------------------------------------------

		// Retrieves the details of the specified entity
		const entity::EntityInfo& GetEntityInfo(const entity::Entity& _entity) const noexcept;

		// Retrieves archetype that will be interacted with of entities
		// If archetype does not exist, create a new archetype and retrieves the newly created archetype
		//archetype::Archetype RetrieveArchetype(const std::span< component_info_t > _componentInfos) noexcept;

		// Wrapper for retrieving archetype based on components' type
		//template < typename... Components >
		//archetype::Archetype RetrieveArchetype() noexcept;
	};
}

#include <Entity\FiE_EntityManager.hpp>
