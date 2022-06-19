/******************************************************************************
filename:	FiE_Entity.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Contains information regarding entity and its functionalities.

******************************************************************************/
#pragma once

namespace FireflyEngine::archetype
{
	struct Archetype;
}

namespace FireflyEngine::entity
{
	// ------------------------------------------------------------------------
	// Entity Information
	// ------------------------------------------------------------------------
	union Entity final
	{
		// Defines the validation used for checking if entity is valid
		union validation final
		{
			sharedinfo::entity_valid_t		m_validationID;		//<! Validation ID of entity
			struct
			{
				sharedinfo::entity_valid_t	m_generation  : 31,	//<! ID of generated entity
											m_isDead	  : 1;	//<! Flag checking if entity is alive
			};

			constexpr bool operator==(const validation _otherValidation) const noexcept
			{
				return m_validationID == _otherValidation.m_validationID;
			}
		};
		static_assert
		(
			sizeof(sharedinfo::entity_valid_t) == sizeof(validation),
			"Size of Validation is not what is expected ( 4 bytes ). Please check."
		);

		// Defines the index of entity
		sharedinfo::entity_id_t				m_entityID;			//<! Index of entity
		struct
		{
			sharedinfo::entity_index_t		m_infoIndex;		//<! Index of where entity is stored in container of all entities
			validation						m_validation;		//<! Check for validness of entity
		};

		// Compare between entities
		constexpr bool operator==(const Entity& _otherEnt) const noexcept
		{
			return m_entityID == _otherEnt.m_entityID;
		}

		// Check if entity is alive
		constexpr bool IsDead() const noexcept
		{
			return m_validation.m_isDead;
		}
	};
	static_assert
	(
		sizeof(sharedinfo::entity_id_t) == sizeof(Entity),
		"Size of Entity is not what is expected ( 8 bytes ). Please check."
	);


	// ------------------------------------------------------------------------
	// Global Entity Information ( wrapper to refer to internal Entity Data)
	// ------------------------------------------------------------------------
	struct EntityInfo final
	{
		archetype::Archetype*		m_pArchetype;		//<! Pointer to corresponding archetype of entity
		sharedinfo::entity_index_t	m_poolIndex;	//<! Entity index in archetype's Pool
		entity::Entity::validation	m_validation;		//<! Validation ID of entity
	};
}
