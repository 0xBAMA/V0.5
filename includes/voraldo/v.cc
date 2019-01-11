#include "../voraldo/v.h"

using std::cout;
using std::endl;

//---------------------------
Voraldo_IO::Voraldo_IO(Voraldo *p)
{
 parent = p;
}

Voraldo_IO::~Voraldo_IO()
{

}

void Voraldo_IO::display()
{

}

//---------------------------
Voraldo_Draw::Voraldo_Draw(Voraldo *p)
{
 parent = p;
}

Voraldo_Draw::~Voraldo_Draw()
{

}

void Voraldo_Draw::init_block(int x, int y, int z, bool noisefill)
{

 if(data != NULL)
	{
		delete[] data;
 }

 x_dim = x;
	y_dim = y;
	z_dim = z;

	num_cells = x_dim * y_dim * z_dim;

 parent->data = new Vox[num_cells];

 for(int i = 0; i < num_cells; i++)
  data[i].mask = false;
  if(noise_fill)
 	{
 		if(std::rand%696 == 69)
   {
    data[i].state = std::rand%255;//this is a little different
   }
  }
  else
  {
   data[i].state = 0;
  }
}

void Voraldo_Draw::clear_all()
{
 for(int i = 0; i < num_cells; i++)
 {
  data[i].state = 0;
 }
}

void Voraldo_Draw::mask_unmask_all()
{
 for(int i = 0; i < num_cells; i++)
 {
  data[i].mask = false;
 }
}

void Voraldo_Draw::mask_invert_mask()
{
 for(int i = 0; i < num_cells; i++)
 {
  data[i].mask = data[i].mask ? false:true;
 }
}

void Voraldo_Draw::mask_all_nonzero()
{
 for(int i = 0; i < num_cells; i++)
 {
  data[i].mask = false;
 }
}

void Voraldo_Draw::mask_by_state(unsigned char s)
{

}

void Voraldo_Draw::draw_point(vec point, unsigned char set, bool draw=true, bool mask=false)
{

}

void Voraldo_Draw::draw_line_segment(vec point1, vec point2, unsigned char set, bool draw=true, bool mask=false)
{

}

void Voraldo_Draw::draw_triangle(vec point1, vec point2, vec point3, unsigned char set, bool draw=true, bool mask=false)
{

}

void Voraldo_Draw::draw_sphere(vec center, double radius, unsigned char set, bool draw=true, bool mask=false)
{

}

void Voraldo_Draw::draw_ellipsoid(vec center, vec radii, unsigned char set, bool draw=true, bool mask=false)
{

}

void Voraldo_Draw::draw_cylinder(vec bvec, vec tvec, double radius,unsigned char set, bool draw=true, bool mask=false)
{

}

void Voraldo_Draw::draw_tube(vec bvec, vec tvec, double inner_radius, double outer_radius,unsigned char set, bool draw=true, bool mask=false)
{

}

void Voraldo_Draw::draw_quadrilateral_hexahedron(vec a, vec b, vec c, vec d, vec e, vec f, vec g, vec h, unsigned char set, bool draw=true, bool mask=false)
{

}

//---------------------------
Voraldo::Voraldo()
{
  io = new Voraldo_IO(this);
  draw = new Voraldo_Draw(this);

  palette = new RGBA[256];
  //need to fill in all the data for colors

  data = NULL;//call draw.init_block(x,y,z,noise_fill)
  //to populate the data array
}

Voraldo::~Voraldo()
{

  delete[] io;
  delete[] draw;
  delete[] palette;
  delete[] data;

}

unsigned char Voraldo::get_data_by_vector_index(vec index)
{
  int data_index = index[2]*y_dim*x_dim + index[1]*x_dim + index[0];

  bool x_valid = x < x_dim && x >= 0;
  bool y_valid = y < y_dim && y >= 0;
  bool z_valid = z < z_dim && z >= 0;

  bool point_valid = x_valid && y_valid && z_valid;


  if(point_valid)
   return data[data_index].state;
  else
   return 0;
}

void Voraldo::set_data_by_vector_index(vec index, unsigned char set, bool mask)
{
 int data_index = index[2]*y_dim*x_dim + index[1]*x_dim + index[0];

 bool x_valid = x < x_dim && x >= 0;
 bool y_valid = y < y_dim && y >= 0;
 bool z_valid = z < z_dim && z >= 0;

 bool point_valid = x_valid && y_valid && z_valid;

 if(point_valid)
  if(!data[data_index].mask)
  {
   data[data_index].state = set;
   data[data_index].mask = mask;
  }
}

bool Voraldo::planetest(vec plane_point, vec plane_normal, vec test_point)
{
 //return false if the point is above the plane
	//return true if the point is below the plane

	double result = 0.0;

	//equation of plane

	// a (x-x1) + b (y-y1) + c (z-z1) = 0

	double a = plane_normal[0];
	double b = plane_normal[1];
	double c = plane_normal[2];

	double x1 = plane_point[0];
	double y1 = plane_point[1];
	double z1 = plane_point[2];

	double x = test_point[0];
	double y = test_point[1];
	double z = test_point[2];

	result = a * (x - x1) + b * (y - y1) + c * (z - z1);

	return (result < 0)?true:false;
}

bool Voraldo::intersect_ray_bbox(vec bbox_min, vec bbox_max, vec ray_org, vec ray_dir, double &tmin, double &tmax, double t0=0, double t1=9999)
{/*
 * Ray-box intersection using IEEE numerical properties to ensure that the
 * test is both robust and efficient, as described in:
 *
 *      Amy Williams, Steve Barrus, R. Keith Morley, and Peter Shirley
 *      "An Efficient and Robust Ray-Box Intersection Algorithm"
 *      Journal of graphics tools, 10(1):49-54, 2005
 *
 */
//I pulled this code after three attempts at my own implementation didn't work
 vec bbox[2];
	int sign[3];

	vec inv_direction = vec(1/ray_dir[0],1/ray_dir[1],1/ray_dir[2]);

	sign[0] = (inv_direction[0] < 0);
	sign[1] = (inv_direction[1] < 0);
	sign[2] = (inv_direction[2] < 0);

	bbox[0] = bbox_min;
	bbox[1] = bbox_max;


	//already declared (passed in by reference so that they can be used)
  tmin = (bbox[sign[0]][0] - ray_org[0]) * inv_direction[0];
  tmax = (bbox[1-sign[0]][0] - ray_org[0]) * inv_direction[0];

  double tymin = (bbox[sign[1]][1] - ray_org[1]) * inv_direction[1];
  double tymax = (bbox[1-sign[1]][1] - ray_org[1]) * inv_direction[1];

  if ( (tmin > tymax) || (tymin > tmax) )
    return false;
  if (tymin > tmin)
    tmin = tymin;
  if (tymax < tmax)
    tmax = tymax;

  double tzmin = (bbox[sign[2]][2] - ray_org[2]) * inv_direction[2];
  double tzmax = (bbox[1-sign[2]][2] - ray_org[2]) * inv_direction[2];

  if ( (tmin > tzmax) || (tzmin > tmax) )
    return false;
  if (tzmin > tmin)
    tmin = tzmin;
  if (tzmax < tmax)
    tmax = tzmax;
  return ( (tmin < t1) && (tmax > t0) );

}
