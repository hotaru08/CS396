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
	struct instance final
	{
		// Rules of 3 - construct, destruct, copy
		instance() noexcept = default;
		~instance() noexcept;

		instance(const instance& _otherInst) noexcept			 = delete; // Unable to copy same pool data to other pools
		instance& operator=(const instance& _otherInst) noexcept = delete;

		// ------------------------------------------------------------------------
		// Pool Functions
		// ------------------------------------------------------------------------
		
		// Initializes pool with defined archetype utilizing this pool
		void Initialize() noexcept;

		// Adds memory 
		void Append() noexcept;

		// Deletes 
		void Delete() noexcept;

		// 
		void Clear() noexcept;

		std::uint32_t GetSize() const noexcept;


		// ------------------------------------------------------------------------
		// Components functions
		// ------------------------------------------------------------------------
		
		// Finds for the index where the component to be found is located in this pool
		constexpr std::int32_t FindComponentTypeInPool(const FireflyEngine::sharedinfo::component_uid_t& _uid) const noexcept;

		// Retrieves the 
		template < typename Component >
		Component& GetComponent(const FireflyEngine::sharedinfo::entity_index_t& _entityIndex) const noexcept;

		std::uint32_t																		 m_size;       //<! Current size / number of entities in pool
		std::array< std::byte*, FireflyEngine::sharedinfo::max_num_components_per_entity_v > m_components; //<! Array of pointers that points to the different components of archetype
		std::span< FireflyEngine::component::info* const >									 m_compInfos;  //<! Container storing the components' info 
	};
}
