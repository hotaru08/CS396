/******************************************************************************
filename:	FiE_Pool.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype's memory pool storing an array of pointers that points to the
	respective components the archetype refers to.

	Actions includes:
		- Identifies components that might be in this pool
		- Retrieving the component information for specified entity

******************************************************************************/
namespace FireflyEngine::pool
{
	// ------------------------------------------------------------------------
	// Archetype's Pool Information
	// ------------------------------------------------------------------------
	struct ArchetypePool final
	{
		// Aliases of function type declarations
		using component_infos_t = std::span < component::Info* const >;
		using memory_array_t	= std::array< std::byte*, sharedinfo::max_num_components_per_entity_v >;

	private:

		std::uint32_t		m_size;       //<! Current size / number of entities in pool
		memory_array_t		m_components; //<! Array of pointers that points to the different components of archetype
		component_infos_t	m_compInfos;  //<! Container storing the components' info


		ArchetypePool(const ArchetypePool& _otherInst) noexcept			    = delete; // Unable to copy pool data to other pools
		ArchetypePool& operator=(const ArchetypePool& _otherInst) noexcept  = delete;

		ArchetypePool(const ArchetypePool&& _otherInst) noexcept			= delete; // Unable to move pool data to other pools
		ArchetypePool& operator=(const ArchetypePool&& _otherInst) noexcept = delete;


	public:

		// Rules of 5 - construct, destruct, copy
		ArchetypePool() noexcept											= default;
		ArchetypePool(component_infos_t _componentInfos) noexcept;
		~ArchetypePool() noexcept;

		// ------------------------------------------------------------------------
		// Pool Functions
		// ------------------------------------------------------------------------
		
		// Initializes pool with defined archetype utilizing this pool
		void Initialize(component_infos_t _componentInfos) noexcept;

		// Appends and initialize new components to components' pool
		void Append() noexcept;

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
		constexpr std::int32_t FindComponentTypeInPool(const sharedinfo::component_uid_t& _uid) const noexcept;

		// Retrieves the specified component at specified entity index
		template < typename Component >
		Component& GetComponent(const sharedinfo::entity_index_t& _entityIndex) const noexcept;
	};
}

#include <Archetype\FiE_Pool.hpp>
