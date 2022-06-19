/******************************************************************************
filename:	FiE_EntityManager.hpp
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Archetype defines the components that an entities has. It also provides
	functionality to interact with entities and their components.

******************************************************************************/
namespace FireflyEngine::entity
{
	// ------------------------------------------------------------------------
	// Helper Functions
	// ------------------------------------------------------------------------

	template < typename... Components >
	archetype::Archetype& Manager::RetrieveArchetype() noexcept
	{
		static_assert
		(
			(!std::is_same_v< entity::Entity, std::decay_t<Components> > && ...),
			"Entity has been internally registered. Please do not explicitly register."
		);

		// Create an array storing the compnents to deduce the archetype they belong to
		static constexpr auto compInfoList = std::array
		{
			&component::info_v< entity::Entity >, &component::info_v< Components >...
		};

		// Set signature for the received set of component information
		tools::Bits receivedCompSignature;
		for (const auto& compInfo : compInfoList)
		{
			assert
			(
				compInfo->m_uid != sharedinfo::invalid_info_v &&
				"Component has invalid uid, has not been registered."
			);
			receivedCompSignature.SetBit(compInfo->m_uid);
		}

		// Find Archetype based on component signature
		for (const tools::Bits& signature : m_archetypeSignatures)
		{
			if (receivedCompSignature != signature)
				continue;

			const unsigned index = (&signature) - m_archetypeSignatures.data();
			return *m_archetypes[index];
		}

		// Create archetype if not found
		m_archetypeSignatures.emplace_back(receivedCompSignature);
		return *m_archetypes.emplace_back
		(
			std::make_unique<archetype::Archetype>(compInfoList, receivedCompSignature, *this)
		);
	}

	entity::Entity Manager::InitializeEntityInfo(
		const sharedinfo::entity_index_t _entityIndex,
		archetype::Archetype& _archetype)
	{
		// Used all slots in global entity container
		assert
		(
			m_nextEmptyInfoIndex < sharedinfo::max_num_entity_total_v &&
			"Invalid index. Check if index is within range of container size."
		);

		// Take index of current free slot and update entity index to be that index
		// Update the free index to be the next free index
		const sharedinfo::entity_index_t entityIndex = m_nextEmptyInfoIndex;
		EntityInfo& entityInfo = m_entityInfos[entityIndex];
		m_nextEmptyInfoIndex = entityInfo.m_poolIndex;

		entityInfo.m_poolIndex = _entityIndex;
		entityInfo.m_pArchetype	= &_archetype;

		// Create a new Entity that has the index and generation
		return
		{
			.m_infoIndex	= entityIndex,
			.m_validation	= entityInfo.m_validation
		};
	}

	void Manager::UpdateEntityInfoAfterDelete(const Entity _entity) noexcept
	{
		EntityInfo& entityInfo = GetEntityInfo(_entity);

		// Update entity information, setting it to be an empty state for next generation
		++entityInfo.m_validation.m_generation;
		entityInfo.m_validation.m_isDead = false;
		entityInfo.m_pArchetype = nullptr;

		// Set pool index to store next free index, and replace free with current entity index
		entityInfo.m_poolIndex = m_nextEmptyInfoIndex;
		m_nextEmptyInfoIndex = _entity.m_infoIndex;
	}

	void Manager::UpdateEntityInfoAfterDelete(const Entity _entity, Entity& _swapEntity) noexcept
	{
		// Get deleted entity and swap with to be swapped entity
		EntityInfo& entityInfo = GetEntityInfo(_entity);
		GetEntityInfo(_swapEntity).m_poolIndex = entityInfo.m_poolIndex;

		++entityInfo.m_validation.m_generation;
		entityInfo.m_validation.m_isDead = false;
		entityInfo.m_pArchetype = nullptr;

		entityInfo.m_poolIndex = m_nextEmptyInfoIndex;
		m_nextEmptyInfoIndex = _entity.m_infoIndex;
	}

	// ------------------------------------------------------------------------
	// Member Exposed Functions
	// ------------------------------------------------------------------------

	Manager::Manager() noexcept
		: m_archetypes			{ },
		  m_archetypeSignatures	{ },
		  m_entityInfos			{ std::make_unique< EntityInfo[] >(sharedinfo::max_num_entity_total_v) },
		  m_nextEmptyInfoIndex	{ 0 }
	{
		// Initialize container that would store the entity informations
		for (unsigned i = 0, end = sharedinfo::max_num_entity_total_v - 2; i < end; ++i)
		{
			m_entityInfos[i].m_poolIndex = i + 1;
		}
	}

	template 
	<
		typename... Components,
		typename CallbackType
	>
		requires tools::traits::has_functor< CallbackType > &&
				 std::is_same_v< typename tools::traits::fn_traits< CallbackType >::return_type_t, void >
	entity::Entity Manager::CreateEntity(CallbackType && _callback) noexcept
	{
		archetype::Archetype& arch = RetrieveArchetype< Components... >();
		assert(arch.CheckArchetypeSignature< Components >() && ...); //eg. passing a pointer to fn, pointer

		// Add a new entity to archetype
		sharedinfo::entity_index_t indexInPool = arch.AddNewPoolEntity();
		const Entity entity = InitializeEntityInfo(indexInPool, arch);
		arch.GetComponent< Entity >(indexInPool) = entity;

		// Call the callback to initialize the created entity
		if constexpr (!std::is_same_v< tools::traits::empty_lambda_t, CallbackType >)
			_callback(arch.GetComponent< std::remove_reference_t< Components > >(indexInPool) ...);
		
		// return the newly created entity in the component pool
		return entity;
	}

	entity::EntityInfo& Manager::GetEntityInfo(const entity::Entity& _entity) const noexcept
	{
		entity::EntityInfo& entityInfo = m_entityInfos[_entity.m_infoIndex];
		assert
		(
			entityInfo.m_validation == _entity.m_validation &&
			"Incorrect generation of entity, invalid entity."
		);
		return entityInfo;
	}

	void Manager::DeleteEntity(entity::Entity& _entity) noexcept
	{
		assert(!_entity.IsDead() && "Current Entity is not alive, unable to delete.");
		EntityInfo& entityInfo = GetEntityInfo(_entity);

		assert(_entity.m_validation == entityInfo.m_validation && 
			   "Correct index,but wrong generation. Unable to delete entity.");
		
		// Set entity and its stored information to be dead (not alive)
		entityInfo.m_validation.m_isDead =
		_entity.m_validation.m_isDead    = true;
		entityInfo.m_pArchetype->DestroyEntity(_entity);
	}

	template < typename CallbackType >
		requires tools::traits::has_functor< CallbackType > && 
				 std::is_same_v< typename tools::traits::fn_traits< CallbackType >::return_type_t, void >
	bool Manager::FindEntity(const entity::Entity& _entity, CallbackType&& _callback) const noexcept
	{
		// Entity is not alive, do not need to find the entity
		if constexpr (_entity.IsDead())
			return false;

		// Retrieve info of entity to be found and check if same entity (same generation)
		const EntityInfo& entityInfo = m_entityInfos[_entity.m_infoIndex];
		if (entityInfo.m_validation != _entity.m_validation)
			return false;
		
		// There is no callback to be called, returns
		if constexpr (std::same_v< CallbackType, sharedinfo::empty_lambda_t >)
			return true;

		entityInfo.m_pArchetype->AccessGuard
		(
			[&] < typename... Components > ( std::tuple< Components... >* )
			{
				_callback(entityInfo.m_pArchetype->GetComponent< 
					std::remove_reference_t< Components > 
				>(entityInfo.m_poolIndex) ...);

			} (tools::traits::null_tuple_v < tools::traits::fn_traits< CallbackType >::args_types_t >);
		);
		return true;
	}

	template < typename... Components >
	std::vector<archetype::Archetype*> Manager::SearchForArchetypeWith() const noexcept
	{
		static_assert
		(
			(!std::is_same_v< entity::Entity, std::decay_t<Components> > && ...),
			"Entity is internally searched for. Please do not explictly search for Entity."
		);

		// Create an array storing the compnents to deduce the archetype they belong to
		static constexpr auto compInfoList = std::array
		{
			&component::info_v< entity::Entity >, &component::info_v< Components >...
		};

		// Set signature for the received set of component information
		tools::Bits receivedCompSignature;
		for (const auto& compInfo : compInfoList)
		{
			assert
			(
				compInfo->m_uid != sharedinfo::invalid_info_v &&
				"Component has invalid uid, has not been registered."
			);
			receivedCompSignature.SetBit(compInfo->m_uid);
		}

		// Search for the archetypes that contains the conponents being searched for
		std::vector<archetype::Archetype*> foundArchs;
		for (const tools::Bits& archSignature : m_archetypeSignatures)
		{
			if (archSignature != receivedCompSignature)
				continue;

			// Found an archetype that matches, add to container
			foundArchs.push_back(m_archetypes[&archSignature - m_archetypeSignatures.data()].get());
		}

		return std::move(foundArchs);
	}

	std::vector<archetype::Archetype*>
	Manager::SearchForArchetypeWith(const query::instance& _compBitQuery) const noexcept
	{
		// Search for the archetypes that contains the conponents being searched for
		std::vector<archetype::Archetype*> foundArchs;
		for (const tools::Bits& archSignature : m_archetypeSignatures)
		{
			if (_compBitQuery != archSignature)
				continue;

			// Found an archetype that matches, add to container
			foundArchs.push_back(m_archetypes[&archSignature - m_archetypeSignatures.data()].get());
		}

		return std::move(foundArchs);
	}
}
