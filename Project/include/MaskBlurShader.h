/*
 * MaskBlurShader.h
 *
 *  Created on: Jul 20, 2014
 *      Author: Hvarnah
 */

#ifndef MASKBLURSHADER_H_
#define MASKBLURSHADER_H_

namespace ubitest {

class MaskBlurShader {
public:
	void activate();
	void deactivate();
	void setMaskTexture(unsigned int textureId, unsigned int textureUnit);
	void setSceneTexture(unsigned int textureId, unsigned int textureUnit);
	void setScreenSize(int screen_width, int screen_height);

	MaskBlurShader();
	~MaskBlurShader();

private:
	void attach_Shader(GLint type, const char * source, GLint program);

	GLuint	m_program;
	GLuint	m_mask_texture_unit;
	GLuint	m_mask_texture_id;
	GLuint	m_scene_texture_unit;
	GLuint	m_scene_texture_id;
	int		m_screen_width;
	int		m_screen_height;

	static const char s_vertex_shader[], s_fragment_shader[];
};

} /* namespace ubitest */
#endif /* MASKBLURSHADER_H_ */
