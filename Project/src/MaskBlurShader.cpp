/*
 * MaskBlurShader.cpp
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

#include <MaskBlurShader.h>

namespace ubitest {

	const char MaskBlurShader::s_fragment_shader[] =
		"uniform sampler2D mask_texture;\n"
		"uniform sampler2D scene_texture;\n"
		"uniform float screen_width;\n"
		"uniform float screen_height;\n"
		"const int radius = 5;\n"
		"void main()\n"
		"{\n"
		"	float mask = texture2D(mask_texture, vec2(gl_FragCoord.x/screen_width, gl_FragCoord.y/screen_height)).r;\n"
		"	if (mask == 0)\n"
		"		gl_FragColor = texture2D(scene_texture, vec2(gl_FragCoord.x/screen_width, gl_FragCoord.y/screen_height));\n"
		"	else \n"
		"	{\n"
		"		int d = (radius*2-1);\n"
		"		int size = d*d;\n"
		"		vec4 sum = vec4(0.0, 0.0, 0.0, 0.0);\n"
		"		for (int i = 0; i < size; ++i)\n"
		"			sum += texture2D(scene_texture, vec2((gl_FragCoord.x - radius + (i % d))/screen_width, (gl_FragCoord.y - radius + (i/d))/screen_height));\n"
		"		gl_FragColor = sum/size;\n"
		"	}\n"
		"}\n"; 

	MaskBlurShader::MaskBlurShader()
	{
		m_program = glCreateProgram();
				
		attach_Shader(GL_FRAGMENT_SHADER, s_fragment_shader, m_program);
		
		glLinkProgram(m_program);
		GLint result;
		glGetProgramiv(m_program, GL_LINK_STATUS, &result);
		if(result == GL_FALSE) 
		{
			glDeleteProgram(m_program);
			throw("MaskBlurShader program link fail");
		}
	}

	void MaskBlurShader::attach_Shader(GLint type, const char * source, GLint program)
	{
		GLuint shader = glCreateShader(type);
		GLint length = strlen(source);

		GLchar * source_array = new char[length];
		strcpy(source_array, source);

		glShaderSource(shader, 1, (const GLchar **)&source_array, NULL);
		glCompileShader(shader);

		GLint result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
		if(result == GL_FALSE)
		{
			char *buf = new char[500];
			glGetShaderInfoLog(shader, 500, NULL, buf);
			glDeleteProgram(m_program);
			throw(buf);
		}
		
		glAttachShader(m_program, shader);
        glDeleteShader(shader);
		//delete [] source_array;
	}

	MaskBlurShader::~MaskBlurShader()
	{
		glDeleteProgram(m_program);
	}

	void MaskBlurShader::activate()
	{
		glUseProgram(m_program);

		glBindTexture( GL_TEXTURE_2D+m_mask_texture_unit, m_mask_texture_id);
		glUniform1i(glGetUniformLocation(m_program, "mask_texture"), m_mask_texture_unit);

		glBindTexture( GL_TEXTURE_2D+m_scene_texture_unit, m_scene_texture_id);
		glUniform1i(glGetUniformLocation(m_program, "scene_texture"), m_scene_texture_unit);

		glUniform1f(glGetUniformLocation(m_program, "screen_width"), m_screen_width);
		glUniform1f(glGetUniformLocation(m_program, "screen_height"), m_screen_height);
	}

	void MaskBlurShader::deactivate()
	{
		glBindTexture( GL_TEXTURE_2D+m_mask_texture_unit, 0);
		glBindTexture( GL_TEXTURE_2D+m_scene_texture_unit, 0);
		glUseProgram(0);
	}

	void MaskBlurShader::setMaskTexture(unsigned int texture_id, unsigned int texture_unit)
	{
		m_mask_texture_id = texture_id;
		m_mask_texture_unit = texture_unit;
	}

	void MaskBlurShader::setSceneTexture(unsigned int texture_id, unsigned int texture_unit)
	{
		m_scene_texture_id = texture_id;
		m_scene_texture_unit = texture_unit;
	}

	void MaskBlurShader::setScreenSize(int screen_width, int screen_height)
	{
		m_screen_width = screen_width;
		m_screen_height = screen_height;
	}

} /* namespace ubitest */
