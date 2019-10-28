#pragma once
#include "stdio.h"
#include "ffmpeg.h"

int main() {
	char* filename = "C:\\Users\\ivc\\Documents\\Visual Studio 2015\\Projects\\ConsoleApplication2\\test_1080p.mp4";

	ffmpeg decode;
	decode.initffmpeg();
	decode.OpenUrl(filename);
	decode.decode();
	return 0;
}