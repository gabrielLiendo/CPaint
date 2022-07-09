#pragma once
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include "tinyfiledialogs.h"

#ifdef _MSC_VER
#pragma warning(disable:4996) // Silences warnings about strcpy strcat fopen
#endif

extern list<shared_ptr<CShape>> shapes;

void saveFile()
{
	char const* lTheSaveFileName;
	char const* lFilterPatterns[2] = { "*.txt", "*.text" };
	FILE* lIn;
	char lBuffer[1024];

#ifdef _WIN32
	tinyfd_winUtf8 = 1;
#endif

	lTheSaveFileName = tinyfd_saveFileDialog(
		"Save As",
		"Scene.txt",
		2,
		lFilterPatterns,
		nullptr);

	if (!lTheSaveFileName)
	{
		tinyfd_messageBox(
			"Error",
			"Save file name can't be empty",
			"Ok",
			"Error",
			1);
		return;
	}

#ifdef _WIN32
	if (tinyfd_winUtf8)
		lIn = _wfopen(tinyfd_utf8to16(lTheSaveFileName), L"w");
	else
#endif
		lIn = fopen(lTheSaveFileName, "w");

	if (!lIn)
	{
		tinyfd_messageBox(
			"Error",
			"Can not open this file in write mode",
			"Ok",
			"Error",
			1);
		return;
	}

	std::string background = "BACKGROUND " 
		+ std::to_string(0.0) + " " + std::to_string(0.0) + " " + std::to_string(0.0) + "\n";
	
	fputs(background.c_str(), lIn);

	for (auto const& s : shapes) {
		string info = s->getInfo();
		fputs(info.c_str(), lIn);
	}
		
		
	fclose(lIn);

}