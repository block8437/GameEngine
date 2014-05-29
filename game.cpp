﻿#include "game.h"
namespace GameEngine {
	Game gameObject;
	GameContactListener myContactListenerInstance;
	void _display_redirect() {
		gameObject.display();
	}
 
	void _reshape_redirect(GLsizei width, GLsizei height) {
		gameObject.reshape(width, height);
	}
 
	void _exit_redirect() {
		gameObject.onExit();
	}

	void _timer_redirect(int value) {
		gameObject.timer(value);
	}

	void _keyboardUp_redirect(unsigned char key, int x, int y) {
		gameObject.keyboardUp(key, x, y);
	}

	void _keyboardDown_redirect(unsigned char key, int x, int y) {
		gameObject.keyboardDown(key, x, y);
	}

	void _specialKeys_redirect(int key, int x, int y) {
		gameObject.specialKeys(key, x, y);
	}
 
	void _mouse_redirect(int button, int state, int x, int y) {
		gameObject.mouse(button, state, x, y);
	}

	Game::Game() {

	}

	Game::Game(char* _title, int width, int height, int _argc, char** _argv) {
		title = _title;
	
		originalWindowWidth = width;
		originalWindowHeight = height;
	
		windowWidth  = originalWindowWidth;
		windowHeight = originalWindowHeight;

		windowPosX	= 50;
		windowPosY	= 50;

		argc = _argc;
		argv = _argv;
	
		refreshMillis = 15.0f;
		base_time = 0;
		fps = 0;
		frames = 0;

		fullScreenMode = false;
	}

	void Game::initGL() {
		keys[0] = false;
		keys[1] = false;
		keys[2] = false;
		keys[3] = false;
	
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE); 
		glutInitWindowSize(windowWidth, windowHeight);
		glutInitWindowPosition(windowPosX, windowPosY); 
		glutCreateWindow(title);
		glutDisplayFunc(_display_redirect);
		glutReshapeFunc(_reshape_redirect);
		glutTimerFunc(0, _timer_redirect, 0);
		glutSpecialFunc(_specialKeys_redirect);
		glutKeyboardUpFunc(_keyboardUp_redirect);	
		glutKeyboardFunc(_keyboardDown_redirect);
		printf("*** Initializing GL Version: %s\n", (const char*)glGetString​(GL_VERSION​));
		if(fullScreenMode)
			glutFullScreen();
		glutMouseFunc(_mouse_redirect);

		B2_NOT_USED(argc);
		B2_NOT_USED(argv);
	}

	void Game::begin() {
		
		universe.getWorld()->SetContactListener(&myContactListenerInstance);

		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glOrtho(0.0f,windowWidth,0.0f,windowHeight,0.0f,1.0f);
		glClearColor(0.3, 0.3, 0.3, 0.3);
		GLenum error = glGetError();
		if(error != GL_NO_ERROR) {
			printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) );
			exit(0);
		}
		glutMainLoop();
	}

	void Game::initGame() {
		loadLevel("level1.json", &universe, windowWidth, windowHeight);
		PlayerObject* player = new PlayerObject(universe.getSpritesheet("playersprites"), 2.0f, 50.0f, 300.0f, universe.getWorld(), 640, 480);
		universe.setActivatePlayer(player);
	}

	void Game::keyboardDown(unsigned char key, int x, int y) {
		switch (key) {
			case 'd':
				keys[3] = true;
				break;
			case 'a':
				keys[2] = true;
				break;
			case 'w':
				keys[0] = true;
				break;
			case 's':
				keys[1] = true; 
				break;
			case 27:	 // ESC key
				exit(0);
				break;
		}
	}
	
	void Game::keyboardUp(unsigned char key, int x, int y) {
		switch (key) {
			case 'd':
				keys[3] = false;
				break;
			case 'a':
				keys[2] = false;
				break;
			case 'w':
				keys[0] = false;
				break;
			case 's':
				keys[1] = false; 
				break;
		}
	}
	
	void Game::specialKeys(int key, int x, int y) {
		switch (key) {
			case GLUT_KEY_F1:
				fullScreenMode = !fullScreenMode;
				if (fullScreenMode) {
					originalWindowWidth = windowWidth;
					originalWindowHeight = windowHeight;
					windowPosX	= glutGet(GLUT_WINDOW_X);
					windowPosY	= glutGet(GLUT_WINDOW_Y);
					windowWidth  = glutGet(GLUT_WINDOW_WIDTH);
					windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
					reshape(windowWidth, windowHeight);
					glutFullScreen();
						
				}
				else {
					windowWidth = originalWindowWidth;
					windowHeight = originalWindowHeight;
					
					glutReshapeWindow(windowWidth, windowHeight);
					glutPositionWindow(windowPosX, windowPosX);
				}
				break;
		}
	}

	void Game::mouse(int button, int state, int x, int y) {
		if(!state) {
			//addMoreBlocks(x, windowHeight - y);
			printf("Mouse clicked %d (%d,%d)\n", state, x, y);
		}
	}
	
	void Game::timer(int value) {
		glutPostRedisplay();
		glutTimerFunc(refreshMillis, _timer_redirect, 0);
	}
	
	void Game::display() {
		frames++;
		calculateFrameRate();
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		universe.update(keys[0], keys[1], keys[2], keys[3]);
		universe.render();

		glutSwapBuffers();
	}
	
	void Game::reshape(GLsizei width, GLsizei height) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		windowWidth = width;
		windowHeight = height;
		glOrtho(0.0f,windowWidth,0.0f,windowHeight,0.0f,1.0f);
		glViewport(0, 0, width, height);
	}
	
	void Game::calculateFrameRate() {
		int time = glutGet(GLUT_ELAPSED_TIME);
		if ((time - base_time) > 1000.0)
		{
			fps = frames*1000.0/(time - base_time);
			base_time = time;
			frames = 0;
		}
	}
	
	float Game::getFrameRate() {
		return fps;
	}

	void Game::onExit() {
		universe.clean();
	}

	void Game::setShader(const char* veretx_file, const char* fragment_file) {
		char *vs, *fs;
		//vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	}

	Game* NewGame(char* title, int windowWidth, int windowHeight, int argc, char** argv) {
		gameObject = Game(title, windowWidth, windowHeight, argc, argv);
		return &gameObject;
	}
}