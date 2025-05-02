#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
using namespace filesystem;

int main(int argc, char* argv[])
{
	path input_file = "src.bin", zero_audio = "zero.wav", one_audio = "one.wav";
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-f") == 0)
			input_file = argv[++i];
		else if (strcmp(argv[i], "-z") == 0)
			zero_audio = argv[++i];
		else if (strcmp(argv[i], "-o") == 0)
			one_audio = argv[++i];
	}

	if (!is_regular_file(input_file)) {
		cerr << "Input is not a regular file" << endl;
		system("pause");
		return -1;
	}

	if (!is_regular_file(zero_audio)) {
		cerr << "Zero is not a regular file" << endl;
		system("pause");
		return -1;
	}

	if (!is_regular_file(one_audio)) {
		cerr << "One is not a regular file" << endl;
		system("pause");
		return -1;
	}

	ifstream* bin_file = new ifstream(input_file, ios::in | ios::binary);

	for (int byte_data = 0; !bin_file->eof();) {
		byte_data = bin_file->get();
		for (int i = (sizeof byte_data) * 2 - 1; i >= 0; i--) {
			int bit_data = (byte_data >> i) & 1;
			cout << bit_data;
			switch (bit_data) {
			case 0:
				PlaySound(zero_audio.c_str(), NULL, SND_FILENAME);
				break;
			case 1:
				PlaySound(one_audio.c_str(), NULL, SND_FILENAME);
				break;
			}
		}
		cout << endl;
	}

	bin_file->close();
	delete bin_file;

	return 0;
}