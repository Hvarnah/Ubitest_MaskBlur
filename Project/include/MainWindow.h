/*
 * MainWindow.h
 *
 *  Created on: Jul 20, 2014
 *      Author: Hvarnah
 */

#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <string>
#include <memory>
#include <map>
#include <ISceneDrawer.h>

namespace ubitest {
class MaskBlurShader;

class MainWindow {
public:
	enum MaskType {MT_MASK_HALF, MT_MASK_CHESSBOARD}; 

	static MainWindow * instance();
	void run(int argc, char * argv[], int width, int height, std::string caption, std::shared_ptr<ISceneDrawer> drawer);
private:
	MainWindow();
	~MainWindow();

	void draw();
	void timer(int timerId);
	void keyboard(unsigned char key, int x, int y);
	void init();
	void close();
	void release();

	void create_Mask_Texture(MaskType type);

	static void s_draw();
	static void s_timer(int timerId);
	static void s_keyboard(unsigned char key, int x, int y);

	static MainWindow * m_instance;

	unsigned int		m_screen_width;
	unsigned int		m_screen_height;
	unsigned int		m_mask_texture;
	unsigned int		m_scene_texture;
	unsigned int		m_framebuffer;

	static const unsigned int timer_delay = 25;


	std::shared_ptr<MaskBlurShader> m_shader;
	std::shared_ptr<ISceneDrawer>	m_scene_drawer;

	std::map<MaskType, unsigned char *> m_mask_data_map;
};

} /* namespace ubitest */

#endif /* _MAINWINDOW_H_ */
