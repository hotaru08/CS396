/******************************************************************************
filename:	FiE_ECSManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Manager that provides the user with the functionalities to use the ECS.

******************************************************************************/
namespace FireflyEngine::ECS
{
	template<typename ...Components, tools::traits::is_empty_fn Callback>
	inline entity::Entity Manager::CreateEntity(Callback&& _callback)
	{


	}

	void Manager::Run()
	{

	}
}
