/******************************************************************************
filename:	FiE_ECS_SharedInfo.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Shared information that is used across different files working with the ECS
	system.
******************************************************************************/
#pragma once

namespace FireflyEngine::sharedinfo
{
	// ------------------------------------------------------------------------
	// Expressions

	// Memory management
	constexpr auto virtual_page_size_v				= 4096;	 //<! 4k bytes (4Mb) as per specificed
															 
	// Component											 
	constexpr auto max_num_components_types_v		= 128;	 //<! Max number of components registered
	constexpr auto max_num_components_per_entity_v	= 32;	 //<! Max number of component each entity can have
															 
	// Entity												 
	constexpr auto max_num_entity_per_pool_v		= 1000;	 //<! Max number of entities in each memory pool
	constexpr auto max_num_entity_total_v			= 10000; //<! Max number of entities that can be generated at a time

	// ------------------------------------------------------------------------
	// Identifiers

	using entity_id_t	   = std::uint64_t;
	using entity_index_t   = std::uint32_t;
	using entity_valid_t   = entity_index_t;

	using component_uid_t  = std::uint16_t;					 //<! Based on max number of components
	using component_size_t = std::uint32_t;					 //<! For storing component size
}
