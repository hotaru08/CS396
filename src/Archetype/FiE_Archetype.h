/******************************************************************************
filename:	FiE_Archetype.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype defines the components that an entities has. It also provides
	functionality to interact with entities and their components.

******************************************************************************/
#pragma once

namespace FireflyEngine::entity
{
	struct Manager;
}

namespace FireflyEngine::archetype
{
	// ------------------------------------------------------------------------
	// Archetype Information
	// ------------------------------------------------------------------------
	struct Archetype final
	{
		//<! Aliases of type declarations
		using component_info_t	= Pool::component_info_t;


		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------

		Archetype(
			const std::span < component_info_t > _componentInfos,
			const tools::Bits& _bits,
			entity::Manager& _entityManager) noexcept;

		Archetype(const Archetype& _otherInst) noexcept				= delete; // Unable to copy archetype's data
		Archetype& operator=(const Archetype& _otherInst) noexcept	= delete;

		Archetype(const Archetype&& _otherInst) noexcept			= delete; // Unable to move archetype's data
		Archetype& operator=(const Archetype&& _otherInst) noexcept = delete;


		// ------------------------------------------------------------------------
		// Archetype Functions
		// ------------------------------------------------------------------------

		// Appends a new entity ( the components infos that the entities has to pool)
		sharedinfo::entity_index_t AddNewEntity() noexcept;

		// Gets the component of an Entity
		template < typename Component >
		Component& GetComponent(const sharedinfo::entity_index_t _entityIndex) const noexcept;

		// Destroys an entity of current archetype
		void DestroyEntity(entity::Entity& _entity) noexcept;

		// Safety check of the signature of archetype for setting the correct bits of components
		template < typename Component >
		constexpr bool CheckArchetypeSignature() const noexcept;



		// ------------------------------------------------------------------------
		// Structural Changes Functions
		// ------------------------------------------------------------------------

		// Update changes that would cause structural updates, leading to UDB
		void UpdateStructuralChanges() noexcept;

		// Ensure that only when this archetype is not being used by anything else,
		// then update changes that disrupts structure of systems using this archetype
		template < typename CallbackType = sharedinfo::empty_lambda_t >
		requires
			tools::traits::has_functor< CallbackType > &&
			std::is_same_v < typename tools::traits::fn_traits< CallbackType >::return_type_t, void > &&
			(tools::traits::fn_traits< CallbackType >::args_count_v == 0)
		void AccessGuard(CallbackType&& _callbackFunc = sharedinfo::empty_lambda_t{}) noexcept;

	private:

		tools::Bits					     m_compSignature;	  //<! Bit signature of flagged components that archetype has
		std::vector<entity::Entity>      m_toDeleteEntities;  //<! Stores the entities that are to be deleted (prevent structural changes)
									     
		entity::Manager&			     m_entityManager;	  //<! Reference to manager that handles the entities
		archetype::Pool					 m_currPool;		  //<! Pool that manages the components of archetype
		std::uint32_t				     m_processesRunning;  //<! Number of systems running, to ensure only update after all systems

	};
}
