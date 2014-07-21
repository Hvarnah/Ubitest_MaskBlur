/*
 * SceneDrawer.h
 *
 *  Created on: Jul 20, 2014
 *      Author: Hvarnah
 */

#include <ISceneDrawer.h>
namespace ubitest {

class SceneDrawer: public ISceneDrawer
{
public:
	virtual void init();
	virtual void timer();
	virtual void draw(unsigned int framebuffer, unsigned int screen_width, unsigned int screen_height);
	virtual void keyboard(unsigned char  key);

	SceneDrawer();
	~SceneDrawer();

private:
	float m_angle;
	const float m_rotation_speed;
	bool m_enable_lighting;
};

} /* namespace ubitest */