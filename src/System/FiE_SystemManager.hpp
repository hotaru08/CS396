/******************************************************************************
filename:	FiE_SystemManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with systems.

******************************************************************************/
namespace FireflyEngine::system
{
	template < typename System >
		requires std::derived_from< System, BaseSystem >
	void Manager::RegisterSystem(ECS::Manager& _ecsManager) noexcept
	{
		m_systems.emplace_back
		(
			// creating system info eg. Info
			Info
			{
			    .m_updateFn = 
					[](BaseSystem& _system) noexcept
					{
						static_cast< details::CompletedSystem< System >& >( _system ).Run();
					},
				.m_system	= std::make_unique< details::CompletedSystem< System > >(_ecsManager)
			}
		);
	}


	template < typename... Systems >
	void Manager::RegisterSystems(ECS::Manager& _ecsManager) noexcept
	{
		(RegisterSystem<Systems>(_ecsManager), ...);
	}

	void Manager::RunAllSystems() noexcept
	{
		for (const auto& system : m_systems)
			system.m_updateFn( *system.m_system );
	}
}
