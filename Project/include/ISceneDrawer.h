/*
 * ISceneDrawer.h
 *
 *  Created on: Jul 20, 2014
 *      Author: Hvarnah
 */
#ifndef _ISCENEDRAWER_H_
#define _ISCENEDRAWER_H_

namespace ubitest {

class ISceneDrawer 
{
public:
	virtual void init() = 0;
	virtual void timer() = 0;
	virtual void keyboard(unsigned char  key) = 0;
	virtual void draw(unsigned int framebuffer, unsigned int screen_width, unsigned int screen_height) = 0;

	ISceneDrawer(){}
	virtual ~ISceneDrawer(){}
};

} /* namespace ubitest */

#endif /* ISCENEDRAWER_H_ */