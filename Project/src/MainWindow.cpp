/*
 * MainWindow.cpp
 *
 *  Created on: Jul 20, 2014
 *      Author: Hvarnah
 */

#define GL_GLEXT_PROTOTYPES
#include "windows.h"
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <math.h>

#include <MaskBlurShader.h>
#include <MainWindow.h>

using namespace std;

namespace ubitest 
{

class ExitSuccess {};

class ExitFailure 
{
public:
	string msg;
	ExitFailure(string _msg):msg(_msg){}
};

MainWindow * MainWindow::m_instance = nullptr;

MainWindow::MainWindow()
{
	// TODO Auto-generated constructor stub
}

MainWindow::~MainWindow() 
{
	release();
}

void MainWindow::release()
{
	for(map<MaskType, unsigned char *>::iterator it = m_mask_data_map.begin(); it != m_mask_data_map.end(); ++ it)
	{
		delete [] it->second;
	}
}

MainWindow * MainWindow::instance() 
{
	if (m_instance == nullptr)
		m_instance = new MainWindow();
	return m_instance;
}

void MainWindow::run(int argc, char * argv[], int width, int height, std::string caption, shared_ptr<ISceneDrawer> drawer) 
{
	try
	{
		m_screen_width = width;
		m_screen_height = height;
		m_scene_drawer = drawer;

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH); 
		glutInitWindowSize(width, height);
		glutInitWindowPosition(0, 0);
		glutCreateWindow(caption.c_str());

		init();

		glutDisplayFunc(MainWindow::s_draw);
		glutTimerFunc(timer_delay, MainWindow::s_timer, 0);
		glutKeyboardFunc(MainWindow::s_keyboard);
		glutMainLoop();
	}
	catch(ExitSuccess & e)
	{
		release();
	}
	catch(ExitFailure & e)
	{
		release();
		throw e.msg;
	}
	catch(...)
	{
		release();
		throw;
	}
}

void MainWindow::init() 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable( GL_TEXTURE_2D );

	create_Mask_Texture(MT_MASK_HALF);

	GLint GlewInitResult = glewInit();
	if (GLEW_OK != GlewInitResult) 
	{
		throw (ExitFailure("glew init fail"));
	}

	try	
	{
		m_shader = make_shared<MaskBlurShader>();
	}
	catch(const char * msg)
	{
		throw ExitFailure(msg);
	}

	glGenTextures(1, &m_scene_texture);
	glBindTexture(GL_TEXTURE_2D, m_scene_texture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, m_screen_width, m_screen_height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	
	glGenFramebuffers(1, &m_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_screen_width, m_screen_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_scene_texture, 0);
 
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainWindow::draw() 
{
	m_scene_drawer->draw(m_framebuffer, m_screen_width, m_screen_height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glEnable( GL_TEXTURE_2D );
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);

	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glViewport(0,0,m_screen_width, m_screen_height);
	m_shader->setSceneTexture(m_scene_texture, 0);
	m_shader->setMaskTexture(m_mask_texture, 1);
	m_shader->setScreenSize(m_screen_width, m_screen_height);
	m_shader->activate();

	glBegin(GL_TRIANGLES);
		glVertex2f(-1.0, -1.0);
		glTexCoord2f(-1.0, -1.0);

		glVertex2f(1.0, -1.0);
		glTexCoord2f(1.0, -1.0);

		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 1.0);

		glVertex2f(-1.0, -1.0);
		glTexCoord2f(-1.0, -1.0);

		glVertex2f(1.0, 1.0);
		glTexCoord2f(1.0, 1.0);

		glVertex2f(-1.0, 1.0);
		glTexCoord2f(-1.0, 1.0);
	//glBindVertexArray(m_screen_mesh_VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnd();

	m_shader->deactivate();

	glFlush();
}

void MainWindow::close()
{
	throw ExitSuccess();
}

void MainWindow::timer(int timerId) 
{
	m_scene_drawer->timer();
	draw();
}

void MainWindow::keyboard(unsigned char key, int x, int y) 
{
	switch (key)
	{
	case 27: //ESC key
		close();
		break;
	case '1':
		create_Mask_Texture(MT_MASK_HALF);
		break;
	case '2':
		create_Mask_Texture(MT_MASK_CHESSBOARD);
		break;
	default:
		m_scene_drawer->keyboard(key);
	}
}

void MainWindow::create_Mask_Texture(MaskType type)
{
	unsigned char * data;

	if (m_mask_data_map.find(type) != m_mask_data_map.end())
	{
		data = m_mask_data_map.find(type)->second;
	}
	else
	{
		data = new unsigned char[m_screen_width * m_screen_height];

		switch (type)
		{
		case MT_MASK_HALF:
			for (int y = 0; y < m_screen_height; ++ y)
				for (int x = 0; x < m_screen_width; ++ x)
					data[(x+y*m_screen_width)] = (x > m_screen_width*0.5f)?255:0;
			break;
			
		case MT_MASK_CHESSBOARD:
			default:
			for (int y = 0; y < m_screen_height; ++ y)
				for (int x = 0; x < m_screen_width; ++ x)
					data[x+y*m_screen_width] = ((x/50 + y/50)%2 == 1)?255:0;
			break;
		}
		m_mask_data_map.insert(make_pair(type, data));
	}

	if (m_mask_texture != 0)
		glDeleteTextures(1, &m_mask_texture);

	glGenTextures( 1, &m_mask_texture );
    glBindTexture( GL_TEXTURE_2D + 1, m_mask_texture );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                     GL_LINEAR_MIPMAP_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    gluBuild2DMipmaps( GL_TEXTURE_2D, 1, m_screen_width, m_screen_height,
                       GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
}

//*****Static functions for glut retranslation**************
void MainWindow::s_draw() 
{
	instance()->draw();
}

void MainWindow::s_timer(int timerId) 
{
	instance()->timer(timerId);
	glutTimerFunc(timer_delay, MainWindow::s_timer, 0);
}

void MainWindow::s_keyboard(unsigned char key, int x, int y) 
{
	instance()->keyboard(key, x, y);
}

} /* namespace ubitest */
