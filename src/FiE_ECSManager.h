/******************************************************************************
filename:	FiE_ECSManager.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that provides the user with the functionalities to use the ECS.

******************************************************************************/
#pragma once

namespace FireflyEngine::ECS
{
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
		// Interaction Functions
		// ------------------------------------------------------------------------

		// Creates a new entity
		template < typename... Components, tools::traits::is_empty_fn Callback = sharedinfo::empty_lambda_t >
		entity::Entity CreateEntity(Callback&& _callback = sharedinfo::empty_lambda_t{});

		// Registers the components defined by user
		template < typename... Components >
		void RegisterComponents() noexcept;

		// Registers the systems defined by user
		template < typename... Systems>
		void RegisterSystems() noexcept;



		// ------------------------------------------------------------------------
		// Other Functions
		// ------------------------------------------------------------------------

		// Updates manager every frame to run it 
		void Run() noexcept;

	private:

		std::unique_ptr<entity::Manager>		m_entityManager;	//<! Manager that handles entites
		std::unique_ptr<component::Manager>		m_componentManager; //<! Manager that handles components
		std::unique_ptr<system::Manager>		m_systemManager;    //<! Manager that handles systems
	};

}

#include <FiE_ECSManager.hpp>
