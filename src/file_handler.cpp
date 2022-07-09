#include "file_handler.h"

extern list<shared_ptr<CShape>> shapes;
extern PaintUI UI;

void saveScene()
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

void loadScene()
{
	char const* lTheOpenFileName;
	char const* lFilterPatterns[2] = { "*.txt", "*.text" };
	FILE* lIn;
	char lBuffer[1024];

	lTheOpenFileName = tinyfd_openFileDialog(
		"Open",
		"",
		2,
		lFilterPatterns,
		NULL,
		0);

	if (!lTheOpenFileName)
	{
		tinyfd_messageBox(
			"Error",
			"Open file name is empty",
			"Ok",
			"error",
			0);
		return;
	}

#ifdef _WIN32
	if (tinyfd_winUtf8)
		lIn = _wfopen(tinyfd_utf8to16(lTheOpenFileName), L"r"); // the UTF-8 filename is converted to UTF-16
	else
#endif
		lIn = fopen(lTheOpenFileName, "r");

	if (!lIn)
	{
		tinyfd_messageBox(
			"Error",
			"Can not open this file in read mode",
			"Ok",
			"error",
			1);
		return;
	}

	char* figureName[100];
	char* token;
	float color, bgColor[3];

	while (fgets(lBuffer, sizeof(lBuffer), lIn) != NULL) {

		token = strtok(lBuffer, " \t");
		if (strcmp(token, "BACKGROUND") == 0)
		{
			int i = 0;
			token = strtok(NULL, " \t");
			for (; token != NULL; i++)
			{
				color = atof(token);
				if (color > 1.0f || color < 0.0f)
				{
					i = 4;
					break;
				}
				bgColor[i] = color;
				token = strtok(NULL, " \t");
			}
			if (i != 3)
				cout << "Invalid Background Color" << endl;
			else
			{
				UI.bgColor[0] = bgColor[0]; UI.bgColor[1] = bgColor[1]; UI.bgColor[2] = bgColor[2];
			}
		}

	}

	fclose(lIn);

	//cout << "hola" << endl;
}