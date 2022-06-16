/******************************************************************************
filename:	FiE_ECS_includes.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Common includes utilized amongst the project.

******************************************************************************/
#pragma once

// ----------------------------------------------------------------------------
// system includes
// ----------------------------------------------------------------------------
#include <Windows.h>

#include <iostream>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <type_traits>
#include <memory>
#include <utility>
#include <array>
#include <vector>
#include <span>
#include <cassert>
#include <concepts>


// ----------------------------------------------------------------------------
// ECS includes
// ----------------------------------------------------------------------------

/* Shared Info */
#include <Info\FiE_ECS_SharedInfo.h>
#include <Info\FiE_ECS_Bits.h>
#include <Info\FiE_Type_Traits.h>

/* Entities */
#include <Entity\FiE_Entity.h>

/* Components */
#include <Component\FiE_Component.h>
#include <Component\FiE_ComponentManager.h>

/* Archetype */
#include <Archetype\FiE_Pool.h>
#include <Archetype\FiE_Archetype.h>

/* Systems */


/* Managers */
#include <Entity\FiE_EntityManager.h>
#include <System\FiE_SystemManager.h>
#include <FiE_ECSManager.h>