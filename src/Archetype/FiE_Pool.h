/******************************************************************************
filename:	FiE_Pool.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype's memory Pool storing an array of pointers that points to the
	respective components the archetype refers to.

******************************************************************************/
#pragma once

namespace FireflyEngine::archetype
{
	// ------------------------------------------------------------------------
	// Archetype's Pool Information
	// ------------------------------------------------------------------------
	struct Pool final
	{
		// Aliases of type declarations
		using component_info_t	= const component::Info* const;
		using memory_array_t	= std::array< std::byte*, sharedinfo::max_num_components_per_entity_v >;


		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------

		Pool(const std::span < component_info_t > _componentInfos) noexcept;
		~Pool() noexcept;

		Pool() noexcept										= delete;
		Pool(const Pool& _otherInst) noexcept			    = delete; // Unable to copy Pool data to other pools
		Pool& operator=(const Pool& _otherInst) noexcept    = delete;

		Pool(const Pool&& _otherInst) noexcept				= delete; // Unable to move Pool data to other pools
		Pool& operator=(const Pool&& _otherInst) noexcept	= delete;


		// ------------------------------------------------------------------------
		// Pool Functions
		// ------------------------------------------------------------------------
		
		// Initializes Pool with defined archetype utilizing this Pool
		void Initialize(std::span < component_info_t > _componentInfos) noexcept;

		// Appends and initialize new components to components' Pool, returning index of new entity
		sharedinfo::entity_index_t Append() noexcept;

		// Deletes and removes components from components' Pool
		void Delete(const sharedinfo::entity_index_t _entityIndex) noexcept;

		// Clear and removes all components in the pools
		void Clear() noexcept;

		// Retrieves the current number of components used
		constexpr std::uint32_t GetSize() const noexcept;


		// ------------------------------------------------------------------------
		// Components functions
		// ------------------------------------------------------------------------
		
		// Finds for the index where the component to be found is located in this Pool
		constexpr std::int32_t FindComponentTypeIndex(const sharedinfo::component_uid_t& _uid) const noexcept;

		// Retrieves the specified component at specified entity index
		template < typename Component >
			requires std::is_same_v< Component, std::decay_t< Component > >
		Component& GetComponent(const sharedinfo::entity_index_t _entityIndex) const noexcept;


	private:

		memory_array_t					m_pComponents;	//<! Array of pointers that points to the components of archetype
		std::span < component_info_t >  m_pCompInfos;	//<! Container storing pointers to components' info
		std::uint32_t					m_numEntities;	//<! Number of entities in Pool
	};
}

#include <Archetype\FiE_Pool.hpp>
