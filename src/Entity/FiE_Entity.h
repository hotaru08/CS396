/******************************************************************************
filename:	FiE_Entity.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Contains information regarding entities and functionalities.

******************************************************************************/
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
				sharedinfo::entity_valid_t	m_generatedID : 31,	//<! ID of generated entity
											m_isAlive	  : 1;	//<! Flag checking if entity is alive
			};
		};
		static_assert(sizeof(sharedinfo::entity_valid_t) == sizeof(validation));

		// Defines the index of entity
		sharedinfo::entity_id_t				m_entityID;			//<! Index of entity
		struct
		{
			sharedinfo::entity_index_t		m_globalIndex;		//<! Global index of entity (for entity container)
			validation						m_validation;		//<! Check for validness of entity
		};

		// Compare between entities
		bool operator==(const Entity& _otherEnt) const noexcept
		{
			return m_entityID == _otherEnt.m_entityID;
		}

		// Check if entity is alive
		bool IsAlive() const noexcept
		{
			return m_validation.m_isAlive;
		}
	};
	static_assert(sizeof(sharedinfo::entity_id_t) == sizeof(Entity));


	// ------------------------------------------------------------------------
	// Global Entity Information ( wrapper to refer to internal Entity Data) 
	// ------------------------------------------------------------------------
	struct GlobalEntity final
	{

	};
}