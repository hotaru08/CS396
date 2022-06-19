/******************************************************************************
filename:	Vector2D.h
author:		Jolyn Wong Kaiyi, wong.k@digipen.edu
Project:	CS396 Assignment 01

Description:

	Vector2D.

******************************************************************************/
#pragma once

struct Vector2D_Test
{
    float x, y;

    Vector2D_Test(const float _x = 0.f, const float _y = 0.f)
        : x{ _x }, y{ _y }
    { }

    void operator()()
    {
    }
};

struct Velocity
{
    Vector2D m_Value;
};

struct Timer
{
    float m_Value;
};

struct Bullet
{
    entity::Entity m_ShipOwner;
};

struct Position
{
    Vector2D pos{ 5.f, 5.f };
};

struct Scale
{
    float m_sx;
    float m_sy;
    int* ptr;

    Scale()
        : m_sx{ 1.f },
        m_sy{ 1.f },
        ptr{ new int }
    {
        std::cout << "Called Scale ctor" << std::endl;
    }

    Scale& operator=(const Scale&& _rhs) noexcept
    {
        m_sx = _rhs.m_sx;
        m_sy = _rhs.m_sy;

        delete ptr;
        ptr = _rhs.ptr;

        std::cout << "Called Scale move" << std::endl;
        return *this;
    }

    ~Scale()
    {
        m_sx = m_sy = 0.f;
        delete ptr;
        ptr = nullptr;

        std::cout << "Called Scale dtor" << std::endl;
    }
};

struct Rotation
{
    float m_rz = 45.f;
};

