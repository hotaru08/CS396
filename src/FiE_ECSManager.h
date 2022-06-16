
#pragma once
#include <GL\freeglut.h>

namespace FireflyEngine::ECS
{
	class Manager final
	{


	public:

		void Update()
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set background color to black and opaque
			glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

			// Draw a Red 1x1 Square centered at origin
			glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
				glColor3f(1.0f, 0.0f, 0.0f); // Red
				glVertex2f(-0.5f, -0.5f);    // x, y
				glVertex2f(0.5f, -0.5f);
				glVertex2f(0.5f, 0.5f);
				glVertex2f(-0.5f, 0.5f);
			glEnd();

			glFlush();  // Render now
		}
	};
}