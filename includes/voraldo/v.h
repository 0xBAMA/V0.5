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
//	>>algorithm given to Andriy as a project independent of this one on 1/14

using vec = linalg::aliases::double3;
using mat = linalg::aliases::double3x3;

using std::cout;
using std::endl;

//forward declaration of Voraldo, so that it can be used in the IO and Draw
class Voraldo;

struct RGB{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};	//I am still researching how the alpha channel is implemented

struct Vox{
	unsigned int state;
	unsigned char alpha; //this is a better way to handle alpha, in this context
	bool mask;

	//unsigned char lighting_intensity;
//I think a second function that access lighting intensity would be relevant, as
	//in - you have a nonzero state, then you will try to access the lighting value
	//with this other function. That way I don't have to return extra/irrelevant
	//data for zero valued cells.
};

class Voraldo_IO{
public:
	Voraldo *parent;

	Voraldo_IO(Voraldo *p);
	~Voraldo_IO();

	void load(){return;}
	void save(){return;}

	void display(std::string filename, double x_rot, double y_rot, double z_rot, double scale, bool perspective);

};


class Voraldo_Draw{
public:
	Voraldo *parent;

	Voraldo_Draw(Voraldo *p);
	~Voraldo_Draw();

	void init_block(int x, int y, int z, bool noisefill);
	//initializes block with selected dimensions, with or without noise

	void clear_all();
	//sets all state and mask values to zero

// 	███▄ ▄███▓ ▄▄▄        ██████  ██ ▄█▀
// ▓██▒▀█▀ ██▒▒████▄    ▒██    ▒  ██▄█▒
// ▓██    ▓██░▒██  ▀█▄  ░ ▓██▄   ▓███▄░
// ▒██    ▒██ ░██▄▄▄▄██   ▒   ██▒▓██ █▄
// ▒██▒   ░██▒ ▓█   ▓██▒▒██████▒▒▒██▒ █▄
// ░ ▒░   ░  ░ ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░▒ ▒▒ ▓▒
// ░  ░      ░  ▒   ▒▒ ░░ ░▒  ░ ░░ ░▒ ▒░
// ░      ░     ░   ▒   ░  ░  ░  ░ ░░ ░
// 						░         ░  ░      ░  ░  ░

	void mask_unmask_all();
	//sets the value of mask to false for all voxels

	void mask_invert_mask();
	//inverts the value of mask for all voxels

	void mask_all_nonzero();
	//sets mask to true for all cells that have nonzero state values

	void mask_by_state(unsigned char s);
	//sets mask to true for all voxels which bear the given state value.

// 	▓█████▄  ██▀███   ▄▄▄       █     █░
// ▒██▀ ██▌▓██ ▒ ██▒▒████▄    ▓█░ █ ░█░
// ░██   █▌▓██ ░▄█ ▒▒██  ▀█▄  ▒█░ █ ░█
// ░▓█▄   ▌▒██▀▀█▄  ░██▄▄▄▄██ ░█░ █ ░█
// ░▒████▓ ░██▓ ▒██▒ ▓█   ▓██▒░░██▒██▓
//  ▒▒▓  ▒ ░ ▒▓ ░▒▓░ ▒▒   ▓▒█░░ ▓░▒ ▒
//  ░ ▒  ▒   ░▒ ░ ▒░  ▒   ▒▒ ░  ▒ ░ ░
//  ░ ░  ░   ░░   ░   ░   ▒     ░   ░
//    ░       ░           ░  ░    ░
//  ░   all of these have the same layout for the set, draw, and mask arguments

//set is the value to draw
//draw is whether or not you will draw in the voxels in the shape
//mask is whether or not you will mask the voxels in that shape
//draw and mask are exclusive, you might say independent of one another - i.e.
//	these functions can be used to mask without drawing if desired

	void draw_point(vec point, unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws a point at the location given by the input argument

	void draw_line_segment(vec v1, vec v2, unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws a line segment from point1 to point2

	void draw_triangle(vec v0, vec v1, vec v2, unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws a triangle between point 1, point 2 and point 3

	void draw_sphere(vec center, double radius, unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws a sphere centered at the point center, with a radius set by radius

	void draw_ellipsoid(vec center, vec radii, unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws an ellipsoid, which is a sphere that has three separate radii
		//this needs to be extended so that this shape can rotate. As we sit right now
		//this shape can change position but the axes can be only along the cardinal
		//axes of the block. It should be a relatively simple thing to use a rotation
		//matrix, but I will need to think about the architecture of the function.

	void draw_cylinder(vec bvec, vec tvec, double radius,unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws a cylinder with circular faces centered at bvec and tvec, with the
		//radius specified by the input argument.

	void draw_tube(vec bvec, vec tvec, double inner_radius, double outer_radius,unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws a cylinder that has a smaller cyilnder taken out of the center
		//	>>	make sure to add validation code for if inner >= outer

	void draw_quadrilateral_hexahedron(vec a, vec b, vec c, vec d, vec e, vec f, vec g, vec h, unsigned char set, bool draw=true, unsigned char alpha=255, bool mask=false);
	//draws a block shape that can be stretched, skewed, twisted and rotated.
		//the main issue comes in when the four points making up a face do not lie in
		//the same plane - there the algorithm has to choose between two ambigous
		//cases and the results will be less than predictable.
};


class Voraldo{
//top level class - instantiates IO and Draw classes
public:
		Voraldo();
		~Voraldo();

		Voraldo_IO *io;
		Voraldo_Draw *draw;

		Vox *data;
		RGB *palette;

//---block dimensions----
		int x_dim;
		int y_dim;
		int z_dim;

		int num_cells;

		unsigned int get_data_by_vector_index(vec index);
		//return the state value for the selected index

		unsigned char get_alpha_by_vector_index(vec index);
		//same as set_data_by_vector_index, but for the alpha

		void set_data_by_vector_index(vec index, unsigned char set, bool draw, bool mask, unsigned char alpha);
		//set the data at the selected index, also setting the mask if the
		//mask variable is true

		bool planetest(vec plane_point, vec plane_normal, vec test_point);
		//see docs

		bool intersect_ray_bbox(vec bbox_min, vec bbox_max, vec ray_org, vec ray_dir, double &tmin, double &tmax, double t0=0, double t1=9999);
		//see docs
};
