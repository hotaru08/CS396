/******************************************************************************
filename:	FiE_EntityManager.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype defines the components that an entities has. It also provides
	functionality to interact with entities and their components.

******************************************************************************/
#pragma once

namespace FireflyEngine::entity
{
	class Manager final
	{
		// Container of global entities
		// CreateEntity
		// RemoveEntity
		// 

		std::unique_ptr<GlobalEntity> m_globalEntities; //<! Container storing all global entities


	public:

		// Rule of 5 - constructor, copy, move
		Manager()												= default;
		Manager(const Manager& _otherMgr) noexcept				= delete; // Unable to copy, only 1 manager
		Manager& operator=(const Manager& _otherMgr) noexcept	= delete;

		Manager(const Manager&& _otherMgr) noexcept				= delete; // Unable to move, only 1 manager
		Manager& operator=(const Manager&& _otherMgr) noexcept	= delete;





	};
}
