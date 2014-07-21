/*
 * SceneDrawer.cpp
 *
 *  Created on: Jul 20, 2014
 *      Author: Hvarnah
 */
#include <windows.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <SceneDrawer.h>

namespace ubitest {

	SceneDrawer::SceneDrawer():m_angle(0.0f), m_rotation_speed(1.0f), m_enable_lighting(true)
	{
	}

	SceneDrawer::~SceneDrawer()
	{
	}

	void SceneDrawer::init()
	{
	}

	void SceneDrawer::timer()
	{
		m_angle += m_rotation_speed;
	}

	void SceneDrawer::keyboard(unsigned char  key)
	{
		if (key == 'l')
		{
			m_enable_lighting = !m_enable_lighting;
		}
	}

	void SceneDrawer::draw(unsigned int framebuffer, unsigned int screen_width, unsigned int screen_height)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0,0,screen_width, screen_height);

		if (m_enable_lighting)
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_LIGHT0);
		}
		else
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_COLOR_MATERIAL);
			glDisable(GL_LIGHT0);
		}

		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); 
		glClearDepth(-1.0f);
		glDepthFunc(GL_GREATER);
		glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
		

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 1.0, 1.0);
		glPushMatrix();
			glTranslatef(0.0f, 0.0f, -3.0f);	
			glRotatef(-20, 1, 0, 0);
			glRotatef(m_angle, 0, 1, 0);
			glutSolidTeapot(0.5f);
		glPopMatrix(); 

	}
} /* namespace ubitest */
