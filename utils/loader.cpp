#include <iostream>
#include <fstream>
#include <getopt.h>
#include "../array.h"
#include "loader.h"

void getFilename(char **filename, int argc, char **argv)
{
	static struct option long_options[] = {
		/* These options set a flag. */
		{ "file", required_argument, 0,			'f'										       },
		{ 0,	  0,		     0,			0										       }
	};
	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;

	while (1) {
		c = getopt_long(argc, argv, "f:",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c) {
		case 0:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0)
				break;
			std::cout << "option " << long_options[option_index].name << std::endl;
			if (optarg)
				std::cout << " with arg " << optarg;
			std::cout << std::endl;
			break;

		case 'f':
			*filename = optarg;
			break;
		default:
			return;
		}
	}
}

void loadValues(char *filename, Array &array)
{
	std::ifstream in;

	if (filename == NULL) {
		std::cerr << "No input file specified\n" << std::endl;
		return;
	}

	in.open(filename);
	if (in.good() != true) {
		std::cerr << "File not found" << std::endl;
		return;
	}

	while (in.eof() != true) {
		int val;

		in >> val;
		if (in.eof() != true) {
			array.addValue(val);
		}
	}

}


