#include "../voraldo/v.h"

using std::cout;
using std::endl;

Voraldo::Voraldo(){
  io = new Voraldo_IO(this);
  draw = new Voraldo_Draw(this);

  palette = new RGBA[256];
  //need to fill in all the data for colors

  data = NULL;//call draw.init_block(x,y,z,noise_fill)
  //to populate the data array
}

Voraldo::~Voraldo(){

  delete[] io;
  delete[] draw;
  delete[] palette;
  delete[] data;

}

unsigned char Voraldo::get_data_by_vector_index(vec index){
  int data_index = index[2]*y_dim*x_dim + index[1]*x_dim + index[0];

  bool x_valid = x < x_res && x >= 0;
  bool y_valid = y < y_res && y >= 0;
  bool z_valid = z < z_res && z >= 0;

  bool point_valid = x_valid && y_valid && z_valid;


  if(point_valid)
   return data[data_index].state;
  else
   return 0;
}

void Voraldo::set_data_by_vector_index(vec index, unsigned char set, bool mask){
 int data_index = index[2]*y_dim*x_dim + index[1]*x_dim + index[0];

 bool x_valid = x < x_res && x >= 0;
 bool y_valid = y < y_res && y >= 0;
 bool z_valid = z < z_res && z >= 0;

 bool point_valid = x_valid && y_valid && z_valid;

 if(point_valid){
  if(!data[data_index].mask){
   data[data_index].state = set;
   data[data_index].mask = mask;
  }
 }
}

//---------------------------
Voraldo_IO::Voraldo_IO(Voraldo *p){
 parent = p;
}

Voraldo_IO::~Voraldo_IO(){

}

void Voraldo_IO::display(){

}

//---------------------------
Voraldo_Draw::Voraldo_Draw(Voraldo *p){
 parent = p;
}

Voraldo_Draw::~Voraldo_Draw(){

}

void Voraldo::init_block(int x, int y, int z, bool noisefill){

}
