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
	struct Manager final
	{
		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------
		
		// Rule of 5 - constructor, copy, move
		Manager()												= default;
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
		
		// DeleteEntity
		
		

	private:



		std::vector< std::unique_ptr< archetype::Archetype > > m_archetypes;             //<! Container storing all archetypes of entities
		std::vector< tools::Bits >							   m_bits;			         //<! Container storing all archetypes signatures
		std::unique_ptr<GlobalEntity[]>						   m_globalEntities;         //<! Pointer to container storing all global entities
		sharedinfo::entity_index_t							   m_emptyGlobalEntityIndex; //<! Index of next free slot in container of global entities

	};
}

#include <Entity\FiE_EntityManager.hpp>
