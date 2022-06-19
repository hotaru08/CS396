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

		// Creates a new entity that has the specified component types 
		template < typename... Components >
			requires (!std::is_pointer_v< Components > && ...) &&
					 (!std::is_reference_v< Components > && ...)
		entity::Entity CreateEntity() noexcept;

		// Creates a new entity with the component deduced from callback function
		template < typename CallbackType = sharedinfo::empty_lambda_t >
			requires tools::traits::has_functor< CallbackType > &&
					 std::is_same_v< typename tools::traits::fn_traits< CallbackType >::return_type_t, void >
		entity::Entity CreateEntity(CallbackType&& _callback = sharedinfo::empty_lambda_t{}) noexcept;


		void DeleteEntity(entity::Entity& _entity) noexcept;





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
