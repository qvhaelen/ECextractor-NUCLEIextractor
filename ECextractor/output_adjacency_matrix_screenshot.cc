#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include<iomanip>
#include<sstream>
#include"output_adjacency_matrix_screenshot.h"

using namespace std;

void output_adjacency_matrix_screenshot(double** adjacency_matrix,int& strong_component, int& time_point, int& number_vertices, int& SCC_active, double*  X  )
{
 string OUTPUT_FILE_NAME ="output_data/screenshot/";
std::ostringstream tp_shot;
tp_shot << time_point;
std::string label_name1 = tp_shot.str();
std::ostringstream part_nbre;
part_nbre << strong_component;
std::string label_name2 = part_nbre.str();
std::ostringstream SCC_zero ;
SCC_zero << SCC_active;
std::string label_name3 = SCC_zero.str();
string  filename =OUTPUT_FILE_NAME+ label_name1 +"-"+label_name2 + "-"+label_name3+"-DIRECTED-GRAPH-EDGES-LIST";
ofstream output_file(filename.c_str());
if (output_file.is_open()){
  for ( int i=1; i<= number_vertices ;++i)
  {
     for (int j=1;j<=number_vertices ;++j)
     {
        if( adjacency_matrix[i][j] < 0.0 )
        {
          if(X[j] != 0.0)
          { output_file << j<< "   "<< i  << "  "<<  -1 << "   "<< 1 << endl;
         }else{output_file << j<< "   "<< i  << "  "<<  -1 << "   "<< 0 << endl;} 
        }
        if( adjacency_matrix[i][j] > 0.0 )
       {
          if(X[j] != 0.0)
           { output_file << j<< "  "<< i  << "  "<<  1 << "   "<< 1 <<  endl;
           }else{output_file << j<< "   "<< i  << "  "<<  1 << "   "<< 0 <<  endl;}
       }
     }
  }
 output_file.close();
}else{cout << "file can not be opened"  << endl;}
return;}










