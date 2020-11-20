#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>

#include "array.h"
#include "buffer.h"
#include "camera.h"
#include "model.h"
#include "program.h"
#include "scene.h"
#include "shader.h"
#include "typedef.h"
using namespace std;
int main()
{
	Scene scene;
	//init scene
	// ---------
	scene.init(
	    "../model/sprite/sprite.fbx",
	    "../resource/model.vert",
	    "../resource/model.frag",
	    "Animation");
	//setting camera
	// -------------------
	scene.attach_camera(
	    Vec3_t(0.0f, 0.0f, 3.0f),
	    Vec3_t(0.0f, 0.0f, -3.0f));
	/*the render loop*/
	while (!scene.is_closing()) {
		scene.update();
		scene.render();
		scene.handle_event();
		scene.swap_buffers();
	}
	/*de-allocate all resources once they've outlived their purpose */
	return 0;
}
