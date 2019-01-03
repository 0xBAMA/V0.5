#include "../CImg.h"		//single-header library for image display and saving
//License below

//IMPORTANT LINKS RE CIMG LIBRARY docs
//http://cimg.eu/reference/group__cimg__overview.html


#include "../json.hpp"  	//single-header library for output file formatting
//MIT License


#include "../linalg.h"      //single-header library for linear algebra (vectors)
//Public Domain License

//Cimg license - from Cimg.eu - CImg is a free, open-source library distributed
//under the CeCILL-C (close to the GNU LGPL) or CeCILL (compatible with the GNU GPL)
//licenses. It can be used in commercial applications.

#include <cstdlib>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>

//#include <bitset>
//not in use right now - potentially useful for a memory-saving optimization

using vec = linalg::aliases::double3;
//using vec2 = linalg::aliases::double2;
using mat = linalg::aliases::double3x3;

using std::cout;
using std::endl;

class Voraldo;
//forward declaration of Voraldo, so that it can be used in the IO and Draw

struct RGBA{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
 unsigned char alpha;
};

struct Vox{
	unsigned char state;
	unsigned char lighting_intensity;
	bool mask;
};

class Voraldo_IO{
public:
	Voraldo *parent;

	Voraldo_IO(Voraldo *p);
	~Voraldo_IO();

	void load(){return;}
	void save(){return;}

	void display();

};


class Voraldo_Draw{
public:
	Voraldo *parent;

	Voraldo_Draw(Voraldo *p);
	~Voraldo_Draw();

	void init_block(int x, int y, int z, bool noisefill);

};


class Voraldo{
//top level class - instantiates IO and Draw classes
public:
		Voraldo();
		~Voraldo();

		Voraldo_IO *io;
		Voraldo_Draw *draw;

		Vox *data;
		RGBA *palette;

//---block dimensions----
		int x_dim;
		int y_dim;
		int z_dim;

		int num_cells;

		unsigned char get_data_by_vector_index(vec index);
		//return the state value for the selected index

		void set_data_by_vector_index(vec index, unsigned char set, bool mask);
		//set the data at the selected index, also setting the mask if the
		//mask variable is true
};
