/*
 * main.cpp
 *
 *  Created on: Jul 20, 2014
 *      Author: Hvarnah
 */

#include "MainWindow.h"
#include "SceneDrawer.h"

#include <Windows.h>
#include <Wincon.h>
#include <memory>
#include <string>
#include <iostream>
using namespace std; 
using namespace ubitest;

int main(int argc, char *argv[])
{
	FreeConsole();

	shared_ptr<ISceneDrawer> drawer = make_shared<SceneDrawer>();

	try
	{
		MainWindow::instance()->run(argc, argv, 800, 800, "ubitest", drawer);
	}
	catch (string & msg)
	{
		AllocConsole();
		cout << "program terminated with the following message:\n" << msg << "\n";
		system("PAUSE");
	}
	catch (...)
	{
		AllocConsole();
		cout << "program terminated with unknown exception\n";
		system("PAUSE");
	}
}

