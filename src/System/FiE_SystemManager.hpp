/******************************************************************************
filename:	FiE_SystemManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that handles the interaction with systems.
	Actions includes:
		- Registering of systems

******************************************************************************/
namespace FireflyEngine::system
{
	template < typename System >
	inline void Manager::RegisterSystem() noexcept
	{
		//// Reference to system's information
		//constexpr auto& uid = info_v< System >.m_UID;
		//if (uid != FireflyEngine::sharedinfo::invalid_info_v)
		//	return;

		//// Set UID of system, adding to unique system count
		//uid = m_numSystems++;
	}

	template < typename... Systems >
	inline void Manager::RegisterSystems() noexcept
	{
		(RegisterSystem< Systems >(), ...);
	}
}
