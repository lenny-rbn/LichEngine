#include <pch.h>
#include <Maths.h>
#include <Core/WrapperWindow.h>


namespace WindowAPI
{
	double GetTimer()
	{
		return glfwGetTime();
	}

	void PollEvents()
	{
		glfwPollEvents();
	}

	int WindowShouldClose(LichWindow* window)
	{
		return glfwWindowShouldClose(window);
	}

	int GetKey(LichWindow* window,int key)
	{
		return glfwGetKey(window,key);
	}

	int GetMouseButton(LichWindow* window, int key)
	{
		return glfwGetMouseButton(window, key);
	}

	void SwipBuffers(LichWindow* window)
	{
		glfwSwapBuffers(window);
	}

	void SetInputMode(LichWindow* window,int mode,int value)
	{
		glfwSetInputMode(window,mode,value);
	}

	void GetCursorPosition(LichWindow* window,double* xpos,double* ypos)
	{
		glfwGetCursorPos(window,xpos,ypos);
	}

	GLFWglproc GetProcdAddress(const char* procname)
	{
		return glfwGetProcAddress(procname);
	}

	void framebuffer_size_callback(LichWindow* window,int width,int height)
	{
		glViewport(0,0,width,height);
	}

	LichWindow* Init(int width,int height,const char* name)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,5);
		glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,true);

		LichWindow* window = glfwCreateWindow(width,height,name,NULL,NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create window" << std::endl;
			glfwTerminate();
			return nullptr;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

		return window;
	}

	Vector2D GetWindowSize(LichWindow* window)
	{
		int width,height;
		glfwGetWindowSize(window,&width,&height);
		return Vector2D(width,height);
	}

	void SetErrorCallback(ErrorFunction function)
	{
		glfwSetErrorCallback(function);
	}

	void SetCurrentContext(LichWindow* window)
	{
		glfwMakeContextCurrent(window);
	}

	int StartWindowAPI()
	{
		return glfwInit();
	}

	void Terminate()
	{
		glfwTerminate();
	}
}