//#include <iostream>

//#include <glad/glad.h>


//using namespace std;

//int main(int argc, char *argv[])
//{
//    cout << "Hello World!" << endl;
//    return 0;
//}


#include <windows.h>
#include <iostream>

//#include <glad/glad.h>
#include <obs.h>

static const int cx = 800;
static const int cy = 600;

class DisplayContext {
    obs_display_t *display;

public:
    inline DisplayContext(obs_display_t *display) : display(display) {}
    inline ~DisplayContext() {obs_display_destroy(display);}
    inline operator obs_display_t*() {return display;}
};

class SourceContext {
    obs_source_t *source;

public:
    inline SourceContext(obs_source_t *source) : source(source) {}
    inline ~SourceContext() {obs_source_release(source);}
    inline operator obs_source_t*() {return source;}
};

/* --------------------------------------------------- */

class SceneContext {
    obs_scene_t *scene;

public:
    inline SceneContext(obs_scene_t *scene) : scene(scene) {}
    inline ~SceneContext() {obs_scene_release(scene);}
    inline operator obs_scene_t*() {return scene;}
};


static void do_log(int log_level, const char *msg, va_list args, void *param)
{
    char bla[4096];
    vsnprintf(bla, 4095, msg, args);

    OutputDebugStringA(bla);
    OutputDebugStringA("\n");

    if (log_level < LOG_WARNING)
        __debugbreak();

    FILE *fptr;
    fptr = fopen("testLog.txt", "a");
    fprintf(fptr, bla);


    UNUSED_PARAMETER(param);
}

static DisplayContext CreateDisplay(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);


    gs_init_data info = {};
    info.cx = rc.right;
    info.cy = rc.bottom;
    info.format = GS_RGBA;
    info.zsformat = GS_ZS_NONE;
    info.window.hwnd = hwnd;

    return obs_display_create(&info);
}

static void CreateOBS(HWND hwnd)
{
    std::cout << "try test";
    RECT rc;
    GetClientRect(hwnd, &rc);

//    obs_startup("", "", nullptr);
//    obs_initialized();
//    obs_startup("en-US", R"_(C:\Users\varder\AppData\Roaming\obs-studio/plugin_config)_", nullptr);
//        throw "Couldn't create OBS";
////    throw "obs started ";
    struct obs_video_info ovi;
    ovi.adapter         = 0;
    ovi.base_width      = rc.right;
    ovi.base_height     = rc.bottom;
    ovi.fps_num         = 30000;
    ovi.fps_den         = 1001;
    ovi.graphics_module = "libobs-d3d11.dll";//DL_D3D11;
    ovi.output_format   = VIDEO_FORMAT_I420;
    ovi.output_width    = rc.right;
    ovi.output_height   = rc.bottom;

    char szBuff[64];
////        throw "try to reset video";




//    struct obs_video_info ovi;
    ovi.fps_num = 30;
    ovi.fps_den = 1;
    ovi.graphics_module = "libobs-d3d11.dll";
    ovi.base_width     = (uint32_t)1280;
    ovi.base_height    = (uint32_t)720;
    ovi.output_width   = (uint32_t)1280;
    ovi.output_height  = (uint32_t)720;
    ovi.output_format  = VIDEO_FORMAT_I420;
    ovi.colorspace     = VIDEO_CS_709 ;
    ovi.range          = VIDEO_RANGE_FULL;
    ovi.adapter        = 0;
    ovi.gpu_conversion = true;
    ovi.scale_type     = OBS_SCALE_BILINEAR;


    if (int res = obs_reset_video(&ovi) != 0){
        sprintf(szBuff, "res %d " , res);
        throw szBuff;
    }
    FILE *fptr;
    fptr = fopen("testLog.txt", "a");
    fprintf(fptr,"%s", "sentence test");

}


static LRESULT CALLBACK sceneProc(HWND hwnd, UINT message, WPARAM wParam,
        LPARAM lParam)
{
    switch (message) {

    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }

    return 0;
}

static HWND CreateTestWindow(HINSTANCE instance)
{
    WNDCLASS wc;

    memset(&wc, 0, sizeof(wc));
    wc.lpszClassName = TEXT("bla");
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hInstance     = instance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc   = (WNDPROC)sceneProc;
    if (!RegisterClass(&wc)){
        return 0;
    }

    HWND hwnd = CreateWindow(TEXT("bla"), TEXT("bla"),
            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
            1000, 800, cx, cy,
            NULL, NULL, instance, NULL);

    return hwnd;
}

//static DisplayContext CreateDisplay(HWND hwnd)
//{
//    RECT rc;
//    GetClientRect(hwnd, &rc);


//    gs_init_data info = {};
//    info.cx = rc.right;
//    info.cy = rc.bottom;
//    info.format = GS_RGBA;
//    info.zsformat = GS_ZS_NONE;
//    info.window.hwnd = hwnd;

//    return obs_display_create(&info);
//}

//#pragma comment (lib, "opengl32.lib")

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//int WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd )
static void SourceLoaded(void *data, obs_source_t *source)
{
       blog(LOG_INFO, "loaded source 11111");
   obs_scene_t *scene = obs_scene_from_source(source);

   if(strcmp(obs_source_get_name(source), "scene5")==0){
        obs_source_inc_showing(source);
        obs_scene_addref(scene);
        obs_set_output_source(0, source);
   }

}

void Load(const char *file1)
{

    const char *file = R"_(C:\Users\v.chubar\AppData\Roaming\obs-studio/basic/scenes/varder.json)_";
//        const char *file = R"_(C:\Users\varder\AppData\Roaming\obs-studio/basic/scenes/varder.json)_";
    obs_data_t *data = nullptr;
    data = obs_data_create_from_json_file_safe(file, "bak");
    if(!data){
        return;
    }
    const char *sceneName = obs_data_get_string(data, "current_scene");
    obs_data_array_t *sources    = obs_data_get_array(data, "sources");

    obs_load_sources(sources, SourceLoaded, nullptr);

}

static void AddTestItems(obs_scene_t *scene, obs_source_t *source)
{
    obs_sceneitem_t *item = NULL;
    struct vec2 scale;

    vec2_set(&scale, 20.0f, 20.0f);

    item = obs_scene_add(scene, source);
    obs_sceneitem_set_scale(item, &scale);
}

static void RenderWindow(void *data, uint32_t cx, uint32_t cy)
{
    obs_render_main_view();

    UNUSED_PARAMETER(data);
    UNUSED_PARAMETER(cx);
    UNUSED_PARAMETER(cy);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int numCmd)
{

    HWND hwnd = NULL;
    std::cout << hwnd;

    hwnd = CreateTestWindow(instance);

    obs_startup("en-US",  R"_(C:\Users\varder\AppData\Roaming\obs-studio/plugin_config)_", nullptr);
    blog(LOG_INFO, "--------------------------------- ALL MODULES");
    obs_load_all_modules();
    blog(LOG_INFO, "--------------------------------- LOG MODULES");
    obs_log_loaded_modules();
    blog(LOG_INFO, "--------------------------------- POST MODULES");

    CreateOBS(hwnd);
//    obs_post_load_modules();

//    SourceContext source = obs_source_create("random",
//                    "some randon source", NULL, nullptr);

//    SourceContext filter = obs_source_create("test_filter",
//                    "a nice green filter", NULL, nullptr);

//    obs_source_filter_add(source, filter);

//    SceneContext scene = obs_scene_create("test scene");

//    AddTestItems(scene, source);
    Load("");
//    obs_set_output_source(0, obs_scene_get_source(scene));


    DisplayContext display = CreateDisplay(hwnd);
    obs_display_add_draw_callback(display, RenderWindow, nullptr);


    MSG msg          = {0};

    while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
        DispatchMessage( &msg );

    return 0;
}


//int WINAPI WinMain1(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int numCmd)
//{
//    base_set_log_handler(do_log, nullptr);
//    HWND hwnd = NULL;
//    std::cout << hwnd;

//    hwnd = CreateTestWindow(instance);

//    CreateOBS(hwnd);

//    Load("");

//    MSG msg          = {0};

//    while( GetMessage( &msg, NULL, 0, 0 ) > 0 )
//        DispatchMessage( &msg );

//    return 0;
//}

//LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
//{
//    switch(message)
//    {
//    case WM_CREATE:
//        {
//        PIXELFORMATDESCRIPTOR pfd =
//        {
//            sizeof(PIXELFORMATDESCRIPTOR),
//            1,
//            PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
//            PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
//            32,                        //Colordepth of the framebuffer.
//            0, 0, 0, 0, 0, 0,
//            0,
//            0,
//            0,
//            0, 0, 0, 0,
//            24,                        //Number of bits for the depthbuffer
//            8,                        //Number of bits for the stencilbuffer
//            0,                        //Number of Aux buffers in the framebuffer.
//            PFD_MAIN_PLANE,
//            0,
//            0, 0, 0
//        };

//        HDC ourWindowHandleToDeviceContext = GetDC(hWnd);

//        int  letWindowsChooseThisPixelFormat;
//        letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
//        SetPixelFormat(ourWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

//        HGLRC ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
//        wglMakeCurrent (ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);

//        MessageBoxA(0,(char*)glGetString(GL_VERSION), "OPENGL VERSION",0);

//        wglDeleteContext(ourOpenGLRenderingContext);
//        PostQuitMessage(0);
//        }
//        break;
//    default:
//        return DefWindowProc(hWnd, message, wParam, lParam);
//    }
//    return 0;

//}
