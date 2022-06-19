/******************************************************************************
filename:	FiE_System.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Contains information regarding systems and functionalities.

******************************************************************************/
#pragma once

namespace FireflyEngine::system
{
	// ------------------------------------------------------------------------
	// Base System that defines what a system would have
	// ------------------------------------------------------------------------
	struct BaseSystem
	{
		//<! The components the system will be working with
		using query = std::tuple<>;		
		
		BaseSystem(ECS::Manager& _ecsManager) noexcept
			: m_ecsManager { _ecsManager }
		{ }

		void OnUpdate() noexcept {}

	protected:

		ECS::Manager& m_ecsManager;	//<! Manager that handles the ECS
	};

	// ------------------------------------------------------------------------
	// Information regarding systems
	// ------------------------------------------------------------------------
	struct Info
	{
		using update_fn	 = void(BaseSystem& _systemInst);
		using update_fn_ptr = update_fn*;

		update_fn*                    m_updateFn; //<! Function to be called to update
		std::unique_ptr< BaseSystem > m_system;   //<! Pointer to system of this info
	};

	// ------------------------------------------------------------------------
	// Internal Implementions
	// ------------------------------------------------------------------------
	namespace details
	{
		// Completed system that contains the information to actually run the systems
		template< typename UserSystem >
			requires std::derived_from< UserSystem, BaseSystem >
		struct CompletedSystem final : UserSystem
		{
			CompletedSystem(ECS::Manager& _ecsManager) noexcept
				: UserSystem { _ecsManager }
			{ }

			// Update the archetypes that this system interact with to do logic
			void Run() noexcept
			{
				// If its a empty functor (meaning no arguments), just run the functor
				// tried tools::traits::fn_traits< UserSystem::operator()() >::args_count_v == 0
				if constexpr (&UserSystem::OnUpdate != &BaseSystem::OnUpdate)
				{
					UserSystem::OnUpdate();
				}
				else
				{
					// Generate a query ( what compoents and their states ) from query and functor
					query::instance query;
					query.AddToQueryFromTuple(tools::traits::null_tuple_v< UserSystem::query >);
					query.AddToQueryFromFunction(*this);

					// Iterate through the components that the system interacts with and update them
					UserSystem::m_ecsManager.ForEachEntity(query, *this);
				}
			}
		};
	}
}
