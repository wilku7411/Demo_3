#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <string>

static float s_defBGcolor[] = { 0.7f, 0.4f, 0.01f, 0.0f };

class CWindow
{
public:
	CWindow()
		:
		m_name(),
		m_window(nullptr),
		m_desktopMode(nullptr),
		m_width(0), m_height(0),
		m_posx(0), m_posy(0),
		m_open(false)
    {};
	~CWindow() {};

    enum EScreenPos
    {
        INVALID = -1,
        UPPER_LEFT = 0,
        UPPER_RIGHT = 1, 
        LOWER_LEFT = 2,
        LOWER_RIGHT = 3,
        CENTER = 4
    };

	int Init(const std::string& windowName, const int width, const int height, bool fullscreen = false);
	void Destroy();

	void ClearUpdate();
	void Swap();
	
	void ProcessKeyboardInput();
	
	void SetCallbacks();
    void SetPosition(EScreenPos position);

	inline GLFWwindow* GetWindow() { return m_window; };
	inline operator GLFWwindow*() { return m_window; }; // added for simplicity

    static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void MouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    static void ViewPortChangeCallback(GLFWwindow* window, int w, int h);

	inline bool IsOpen() const { return m_open; };

    inline float* GetBackgroundColor() { return m_backgroundColor; }
    inline void SetBackgroundColor(float val[4])
    {
        m_backgroundColor[0] = val[0];
        m_backgroundColor[1] = val[1];
        m_backgroundColor[2] = val[2];
        m_backgroundColor[3] = val[3];
    }

    inline int Width() const { return m_width; }
    inline void Width(int val) { m_width = val; }
    
    inline int Height() const { return m_height; }
    inline void Height(int val) { m_height = val; }
private:
    int InitGLFW();
    int InitContext();
    void SetHints();

    std::string m_name;
    const GLFWvidmode* m_desktopMode;
    GLFWwindow* m_window;
    float m_backgroundColor[4] = { 0.52f, 0.8f, 0.92f, 0.0f };
    int m_width, m_height, m_posx, m_posy;
	bool m_open;
};

