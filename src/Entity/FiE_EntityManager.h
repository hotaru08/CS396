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
			typename CallbackType = sharedinfo::empty_lambda_t
		>
			requires tools::traits::has_functor< CallbackType > &&
					 std::is_same_v< typename tools::traits::fn_traits< CallbackType >::return_type_t, void >
		entity::Entity CreateEntity(CallbackType&& _callback = sharedinfo::empty_lambda_t{}) noexcept;
		
		// Retrieves the details of the specified entity
		const entity::EntityInfo& GetEntityInfo(const entity::Entity& _entity) const noexcept;

		// Find an entity and carry out actions on it
		template < typename CallbackType = sharedinfo::empty_lambda_t>
			requires tools::traits::has_functor< CallbackType >
		bool FindEntity(const entity::Entity& _entity, CallbackType&& _callback = sharedinfo::empty_lambda_t{}) const noexcept;

		// Deletes an entity, along with its respective information
		void DeleteEntity(entity::Entity& _entity) noexcept;

	private:

		//<! Alias to component's imfo
		using component_info_t = archetype::Archetype::component_info_t;							

		std::vector< std::unique_ptr< archetype::Archetype > > m_archetypes;			//<! Container storing all archetypes of entities
		std::vector< tools::Bits >							   m_archetypeSignatures;	//<! Container storing all archetypes signatures
		std::unique_ptr<EntityInfo[]>						   m_entityInfos;			//<! Pointer to container storing all actual entities information
		sharedinfo::entity_index_t							   m_nextEmptyInfoIndex;	//<! Index of next free slot in container of entities information


		// ------------------------------------------------------------------------
		// Helper Functions
		// ------------------------------------------------------------------------

		// Retrieves archetype that will be interacted with of entities
		// If archetype does not exist, create a new archetype and retrieves the newly created archetype
		archetype::Archetype& RetrieveArchetype(std::span< component_info_t > _componentInfos) noexcept;

		// Wrapper for retrieving archetype based on components' type
		template < typename... Components >
		archetype::Archetype& RetrieveArchetype() noexcept;

		// Initialize the information in the global entity pool
		entity::Entity InitializeEntityInfo(
			const sharedinfo::entity_index_t _entityIndex,
			archetype::Archetype& _archetype);

		// Internally, update the information of entities when they are allowed to update
		// with structural changes
		void UpdateEntityInfoAfterDelete(const Entity _entity) noexcept;
		void UpdateEntityInfoAfterDelete(const Entity _entity, Entity& _swapEntity) noexcept;
	};
}
