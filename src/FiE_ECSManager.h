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
		entity::Manager		m_entityManager;
		component::Manager	m_componentManager;
		system::Manager		m_systemManager;

		template < typename... Components, tools::traits::is_empty_fn Callback = sharedinfo::empty_lambda_t >
		entity::Entity CreateEntity(Callback&& _callback = sharedinfo::empty_lambda_t{});

		//void DeleteEntity();


		void Run();
	};

}

#include <FiE_ECSManager.hpp>
