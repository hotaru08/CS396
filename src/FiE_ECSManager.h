
#pragma once

namespace FireflyEngine::ECS
{
	class Manager final
	{


	public:

		entity::Manager		m_entityManager;
		component::Manager	m_componentManager;

		void Update()
		{

		}
	};
}