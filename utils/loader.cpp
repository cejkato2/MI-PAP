#include <iostream>
#include <fstream>
#include <getopt.h>
#include <math.h>
#include <stdlib.h>
#include "array.h"
#include "2Darray.h"
#include "loader.h"


void help()
{
	std::cout << "Usage:" << std::endl;
	std::cout << "-f|--file <filename>	input file name"<< std::endl;
	std::cout << "-t|--threads <number>	amount of threads (default 2)"<< std::endl;
	std::cout << "-n|--amount <X - number>	set exponent of amount - 2^X (default 10)"<< std::endl;

}

void getSettings(char **filename, int *amount, int *threads, int argc, char **argv)
{
	bool got_file = false;
	bool got_lgamount = false;
	bool got_threads = false;
	int lgamount;

	static struct option long_options[] = {
		/* These options set a flag. */
		{
			"file", required_argument, 0, 'f'
		},
		{
			"threads", required_argument, 0, 't'
		},
		{
			"amount", required_argument, 0, 'n'
		},
		{
			"help", no_argument, 0, 'h'
		},
		{
			0, 0, 0, 0
		}
	};
	/* getopt_long stores the option index here. */
	int option_index = 0;
	int c;

	while (1) {
		c = getopt_long(argc, argv, "f:t:n:h",
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
			got_file = true;
			break;
		case 'n':
			lgamount = atoi(optarg);
			got_lgamount = true;
			break;
		case 't':
			*threads = atoi(optarg);
			got_threads = true;
			break;
		case 'h':
			help();
			exit(0);
		default:
			return;
		}
	}
	if (got_file == false) {
		filename = NULL;
	}

	if (got_threads == false) {
		*threads = THREADS; //implicit value
	}
	if (got_lgamount == false) {
		lgamount = ARRAY_SIZE_EXP; //implicit value
	}
	//set arraysize
	*amount = (int)pow(2, lgamount);

}

void getFilename(char **filename, int argc, char **argv)
{
	bool got_file = false;
	static struct option long_options[] = {
		/* These options set a flag. */
		{
			"file", required_argument, 0, 'f'
		},
		{
			0, 0, 0, 0
		}
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
			got_file = true;
			break;
		default:
			return;
		}
	}
	if (got_file == false) {
		filename = NULL;
	}
}

void loadValues(char *filename, Array &array, int amount)
{
	std::ifstream in;

	if (amount > 0) {
		for (int i = 0; i < amount; ++i) {
			array.addValue((int)random());
		}
	} else {
		if (filename != NULL) {
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
			in.close();
		} else {
			std::cerr << "0 amount and no filename" << std::endl;
		}
	}
}

void loadValues(char *filename, Array2D *a2d, int amount)
{
	std::ifstream in;
	Array a;

	loadValues(filename, a, amount);

	if (a.isEmpty() == true) {
		return;
	}

	int x, y;
	int size = a.getSize();
	int n = (int)sqrt((double)size);
	if (n * n != size) {
		//neni ctvercova :-(
		x = n;
		while ((size % x) != 0) {
			x--;
		}
		y = size / x;
	} else {
		x = y = n;
	}
	int index = 0;
	int *data = a.getData();

	#ifdef DEBUG_OUTPUT
	std::cout << "Size of matrix: " << x << "x" << y << std::endl;
	#endif

	a2d->addRow(y);
	a2d->addColumn(x);
	for (int i = 0; i < y; i++) {
		for (int j = 0; j < x; j++) {
			a2d->setValueAt(data[index++], i, j);
		}
	}
}


