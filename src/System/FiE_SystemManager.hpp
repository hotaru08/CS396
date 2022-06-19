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
	inline void Manager::RegisterSystem() noexcept
	{
	}

	template < typename... Systems >
	inline void Manager::RegisterSystems() noexcept
	{
		(RegisterSystem< Systems >(), ...);
	}
}
