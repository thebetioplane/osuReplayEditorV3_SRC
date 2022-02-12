#include "common.hpp"
#include "../thirdparty/glfw/glfw3.h"
#include "../thirdparty/AntTweakBar/AntTweakBar.h"
#include "config.hpp"
#include "breader.hpp"
#include "audioengine.hpp"
#include "osudb.hpp"
#include "replayengine.hpp"
#include "beatmapengine.hpp"
#include "texture.hpp"
#include "ui.hpp"

static void load_replay(const char *fname)
{
	replayengine::init(fname);
	const char *osu_path;
	const char *song_path;
	bool found = osudb::get_entry(replayengine::beatmap_hash, &osu_path, &song_path);
	if (! found)
		fatal("cannot find song in your osu!.db");
	audioengine::load(song_path);
	beatmapengine::init(osu_path);
	audioengine::play();
	audioengine::set_playback_speed(1.0f);
	/*
	audioengine::jump_to(36.7618f);
	audioengine::pause();
	*/
}

static inline void init()
{
	std::ios_base::sync_with_stdio(false);
	config::init();
	audioengine::init();
	osudb::init();
	replayengine::init(nullptr);
	beatmapengine::init(nullptr);
}

static void deinit()
{
}

namespace bounds {
	static int x, y, w, h;
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
	int x = 0;
	int y = 0;
	int w = height * 16 / 10;
	int h = width * 10 / 16;
	if (w > width) {
		w = width;
		h = w * 10 / 16;
		y = (height - h) / 2;
	} else {
		h = height;
		w = h * 16 / 10;
		x = (width - w) / 2;
	}
	glViewport(x, y, w, h);
	TwWindowSize(width, height);
	bounds::x = x;
	bounds::y = y;
	bounds::w = w;
	bounds::h = h;
	static char new_title[256];
	sprintf_s(new_title, 256, "Window %dx%d; Viewport %dx%d", width, height, w, h);
	glfwSetWindowTitle(window, new_title);
}

enum scroll_t { PLAYING, PAUSED, FORWARD, BACKWARD };

static scroll_t scroll_state = PLAYING;

static void key_callback(GLFWwindow* window, int key, int, int action, int)
{
	if (TwEventKeyGLFW(key, action))
		return;
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			ui::reset_tw();
			break;
		case 'Q':
			trace(replayengine::replay_hash);
			trace(beatmapengine::stack_leniency);
			trace(beatmapengine::hp);
			trace(beatmapengine::cs);
			trace(beatmapengine::od);
			trace(beatmapengine::ar);
			trace(beatmapengine::slider_mult);
			break;
		case 'W':
			trace(audioengine::get_time());
			break;
		case 'S':
			audioengine::toggle_pause();
			scroll_state = audioengine::is_playing() ? PLAYING : PAUSED;
			break;
		case 'D':
			audioengine::pause();
			scroll_state = FORWARD;
			break;
		case 'A':
			audioengine::pause();
			scroll_state = BACKWARD;
			break;
		default:
			ui::key_down(key);
		}
	} else if (action == GLFW_RELEASE) {
		switch (key) {
		case 'D':
			if (scroll_state == FORWARD) {
				audioengine::pause();
				scroll_state = PAUSED;
			}
			break;
		case 'A':
			if (scroll_state == BACKWARD) {
				audioengine::pause();
				scroll_state = PAUSED;
			}
			break;
		}
	}
}
static void mouse_button_callback(GLFWwindow*, int button, int action, int)
{
	TwMouseAction tw_action;
	TwMouseButtonID tw_button;
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		tw_button = TW_MOUSE_LEFT;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		tw_button = TW_MOUSE_RIGHT;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		tw_button = TW_MOUSE_MIDDLE;
		break;
	default:
		return;
	}
	switch (action) {
	case GLFW_PRESS:
		tw_action = TW_MOUSE_PRESSED;
		break;
	case GLFW_RELEASE:
		tw_action = TW_MOUSE_RELEASED;
		break;
	default:
		return;
	}
	TwMouseButton(tw_action, tw_button);
}

static void mouse_move_callback(GLFWwindow *, double x, double y)
{
	TwEventMousePosGLFW(static_cast<int>(x), static_cast<int>(y));
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error(%d): %s\n", error, description);
	fatal("glfw error");
}

static void drop_callback(GLFWwindow*, int count, const char** paths)
{
	if (count == 0)
		return;
	if (count > 1)
		not_fatal("more than one dropped file, only processing the first");
	load_replay(paths[0]);
}

#define window_width 1000
#define window_height 625

#ifdef _WIN32
#define sleep_ms(ms)(Sleep(ms));
#endif

//#define FPS
#define VSYNC

int main()
{
	init();
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		fatal("glfwInit() failed");
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Window's Window", nullptr, nullptr);
	if (!window)
		fatal("glfwCreateWindow() failed");
	glfwMakeContextCurrent(window);
#ifdef VSYNC
	glfwSwapInterval(1);
#else
	glfwSwapInterval(0);
#endif
	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetDropCallback(window, drop_callback);
	glfwSetCursorPosCallback(window, mouse_move_callback);
	resize_callback(window, window_width, window_height);
	ui::init_tw();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	//glClearColor(0.392157f, 0.584314f, 0.929412f, 1.0f);
	glClearColor(.0f, .0f, .0f, 1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1600, 1000, 0, 0, 2);
	textures::init();
	bool cursor_is_hand = false;
#ifdef _DEBUG
	load_replay("samples/Replays/ban.osr");
#endif
	glColor3f(1.0f, 1.0f, 1.0f);
	float last_time = (float)glfwGetTime();
#ifdef FPS
	int frame_count = 0;
	float frame_sum = 0.0f;
#endif
	while (!glfwWindowShouldClose(window)) {
		float curr_time = (float)glfwGetTime();
		float dt = curr_time - last_time;
#ifdef FPS
		++frame_count;
		frame_sum += dt;
		if (frame_sum >= 1.0f) {
			const float fps = frame_count / frame_sum;
			trace(fps);
			frame_count = 0;
			frame_sum = 0.0f;
		}
#endif
		last_time = curr_time;
		if (scroll_state == FORWARD) {
			audioengine::rel_jump(dt * audioengine::get_playback_speed());
		}
		else if (scroll_state == BACKWARD) {
			audioengine::rel_jump(-dt * audioengine::get_playback_speed());
		}
		glClear(GL_COLOR_BUFFER_BIT);
		float t = audioengine::get_time();
		beatmapengine::draw(t);
		replayengine::draw(t);
		ui::draw(t);
		glfwSwapBuffers(window);
		glfwPollEvents();
#ifndef VSYNC
		sleep_ms(1);
#endif
	}
	TwTerminate();
	glfwTerminate();
	deinit();
	return 0;
}
