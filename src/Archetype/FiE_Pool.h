/******************************************************************************
filename:	FiE_Pool.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype's memory pool storing an array of pointers that points to the
	respective components the archetype refers to.

******************************************************************************/
namespace FireflyEngine::pool
{
	// ------------------------------------------------------------------------
	// Archetype's Pool Information
	// ------------------------------------------------------------------------
	struct ArchetypePool final
	{
		// Aliases of type declarations
		using component_info_t	= const component::Info* const;
		using memory_array_t	= std::array< std::byte*, sharedinfo::max_num_components_per_entity_v >;


		// ------------------------------------------------------------------------
		// Constructors / Destructor
		// ------------------------------------------------------------------------

		ArchetypePool() noexcept;
		ArchetypePool(const std::span < component_info_t >& _componentInfos) noexcept;
		~ArchetypePool() noexcept;

		ArchetypePool(const ArchetypePool& _otherInst) noexcept			    = delete; // Unable to copy pool data to other pools
		ArchetypePool& operator=(const ArchetypePool& _otherInst) noexcept  = delete;

		ArchetypePool(const ArchetypePool&& _otherInst) noexcept			= delete; // Unable to move pool data to other pools
		ArchetypePool& operator=(const ArchetypePool&& _otherInst) noexcept = delete;


		// ------------------------------------------------------------------------
		// Pool Functions
		// ------------------------------------------------------------------------
		
		// Initializes pool with defined archetype utilizing this pool
		void Initialize(std::span < component_info_t > _componentInfos) noexcept;

		// Appends and initialize new components to components' pool, returning index of new entity
		sharedinfo::entity_index_t Append() noexcept;

		// Deletes and removes components from components' pool
		void Delete(const sharedinfo::entity_index_t _entityIndex) noexcept;

		// Clear and removes all components in the pools
		void Clear() noexcept;

		// Retrieves the current number of components used
		constexpr std::uint32_t GetSize() const noexcept;


		// ------------------------------------------------------------------------
		// Components functions
		// ------------------------------------------------------------------------
		
		// Finds for the index where the component to be found is located in this pool
		constexpr std::int32_t FindComponentType(const sharedinfo::component_uid_t& _uid) const noexcept;

		// Retrieves the specified component at specified entity index
		template < typename Component >
			requires std::is_same_v< Component, std::decay_t< Component > >
		Component& GetComponent(const sharedinfo::entity_index_t& _entityIndex) const noexcept;


	private:

		memory_array_t					m_pComponents;	//<! Array of pointers that points to the components of archetype
		std::span < component_info_t >  m_pCompInfos;	//<! Container storing pointers to components' info
		std::uint32_t					m_numEntities;	//<! Number of entities in pool
	};
}

#include <Archetype\FiE_Pool.hpp>
