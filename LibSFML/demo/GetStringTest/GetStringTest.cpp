// GetStringTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>

#include "..\MTMode.h"
#include <SFML/Graphics.hpp>
#include <GL/glew.h>


int _tmain(int argc, _TCHAR* argv[])
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Application");

	GLenum err = glewInit();
	if (GLEW_OK != err){
		std::cout << "GLew Init Failed!" << std::endl;
		return 0;
	}

	std::cout << "+GL_VERSION:" << glGetString(GL_VERSION) << std::endl;
	std::cout << "+GL_SHADING_LANGUAGE_VERSION:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;	
	std::cout << "+GL_RENDERER:" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "+GL_VENDOR:" << glGetString(GL_VENDOR) << std::endl;
	//std::cout << "+GL_EXTENSIONS:" << glGetString(GL_EXTENSIONS) << std::endl;

	GLint num_exts{ 0 };
	glGetIntegerv(GL_NUM_EXTENSIONS, &num_exts);

	for (int i = 0; i < num_exts; i++)
		std::cout <<i<<":"<< glGetStringi(GL_EXTENSIONS, i) << std::endl;

	return 0;
}

