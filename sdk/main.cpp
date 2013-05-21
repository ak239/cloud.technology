#include <iostream>

#include "GLApp.h"
#include "GlutWindow.h"

#include "clouds/clouds.h"

#include "FSShaderProgramLoader.h"

void setupWindow(const GlutWindow& window, ContextCallbacks* callbacks, std::string shaderFolder)
{
	FSShaderProgramLoader shader_loader(shaderFolder);
	GLContext context = window.getContext();
	context.registerCallbacks(callbacks);
	if (!shader_loader.loadProgram(context)){
		std::cerr << shader_loader.getLastError() << std::endl;
	}else{
		shader_loader.getProgram().use();
	}
}

int main(int args, char* argv[])
{
	GLApp app;

	app.glutInitialize(args, argv);

	//GlutWindow wnd = GlutWindow::Fullscreen("1920x1080");
	GlutWindow wnd("Clouds window");
	wnd.setGeometry(800, 600);
	wnd.setPosition(300, 300);
	
	// after window create!!! (need context)
	if (!app.glewInitialize())
	{
		std::cerr << "App Init Error: " << app.getLastError() << std::endl;
		return -1;
	}

	GLContext context = wnd.getContext();
	context.initTweakBars();
	context.registerCallbacks(new Clouds());

	return app.exec();
}
