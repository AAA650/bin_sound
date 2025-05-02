#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
using namespace filesystem;

vector<path>* get_res_dir(const path& search_dir) {
	if (!is_directory(search_dir))
		return nullptr;

	vector<path>* path_list = new vector<path>;

	for (auto const& dir_it : directory_iterator(search_dir)) {
		if (is_regular_file(dir_it.path() / "zero.wav")
			&& is_regular_file(dir_it.path() / "one.wav")) {
			path_list->push_back(dir_it.path());
		}
	}

	return path_list;
}

int main(int argc, char* argv[])
{
	// init vars
	path in_bin_file = "res.bin", res_path;
	bool is_res_set = false;

	// process args
	for (int i = 0; i < argc; i++) {
		// set in_bin_file
		if (strcmp(argv[i], "-f") == 0)
			in_bin_file = argv[++i];
		// set res_dir
		else if (strcmp(argv[i], "-p") == 0)
		{
			res_path = argv[++i];
			is_res_set = true;
		}
	}

	if (!is_res_set) {
		// if res dir was not set in args
		// find dirs in current_path/res
		vector<path>* res_list = get_res_dir(current_path() / "res");

		// check res_list
		if (res_list == nullptr) {
			cerr << "No legal directories detected" << endl;
			return -1;
		}

		cout << "Res was not set! Choose following folders detected." << endl;
		
		// output dir detected
		for (int i = 0; i < res_list->size(); i++) {
			cout << i << ") " << res_list->at(i) << endl;
		}

		//accept users input
		for (int input;;) {
			// input res offset
			cout << "Pls choose one: ";
			cin >> input;

			// check input
			if (cin.fail()) {
				cin.clear();
				cin.ignore();
				continue;
			}

			// if out_of_range or other errors occered
			try{
				res_path = res_list->at(input);
			}// then redo for loop
			catch (exception ex) {
				continue;
			}

			break;
		}

		delete res_list;
	}
	else if (!is_directory(res_path)) {
		cerr << "Res is not a directory" << endl;
		return -1;
	}

	// set zero/one audio
	path zero_audio = res_path / "zero.wav",
		one_audio = res_path / "one.wav";

	// check if file existing
	if (!is_regular_file(in_bin_file)) {
		cerr << "Input " << in_bin_file.c_str() << " is not a regular file" << endl;
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

	// read input_bin_file
	ifstream* bin_file = new ifstream(in_bin_file, ios::in | ios::binary);

	// count down to start
	cout << "Ready? 3... ";
	Sleep(1000);
	cout << "2... ";
	Sleep(1000);
	cout << "1... " << endl;
	Sleep(1000);

	for (int byte_data = 0; !bin_file->eof();) {
		// process each byte
		byte_data = bin_file->get();

		for (int i = (sizeof byte_data) * 2 - 1; i >= 0; i--) {
			// process each bit
			int bit_data = (byte_data >> i) & 1;
			cout << bit_data;

			// play sound
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

	// free data
	bin_file->close();
	delete bin_file;

	return 0;
}