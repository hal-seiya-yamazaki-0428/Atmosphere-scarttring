#ifndef _MY_IMGUI_H_
#define _MY_IMGUI_H_

class CMyImGui
{
private:
	
public:
	static void Init();
	static void Uninit();

	static void Begin();
	static void End();

	static bool ColorPicker(float *col, bool alphabar);
	static bool ColorPicker3(float col[3]);
	static bool ColorPicker4(float col[4]);
};

#endif