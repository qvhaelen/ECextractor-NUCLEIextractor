 #include <fstream>
#include <iostream>
#include <string>
#include<iomanip>
#include<sstream>
#include<ctime>
#include"output_stat.h"
using namespace std;
void output_stat(int& tp_start, int& tp_end ,double& duration ){
 string OUTPUT_FILE_NAME ="output_data/";
string  filename =OUTPUT_FILE_NAME+ "Simulation-General-Information.txt";
time_t rawtime;
struct tm *timeinfo;
char format[32];
time(&rawtime);
timeinfo = localtime(&rawtime);
ofstream output_file(filename.c_str());
if (output_file.is_open()){
  output_file<<"***********************************GENERAL INFORMATION ******************************************"<< endl;
  output_file<< "Date when the simulation was performed : "; 
  output_file<< asctime(timeinfo) <<endl;
  output_file<<"*************************************************************************************************"<< endl;
  output_file<<"Elapsed CPU time for the  completion of the simulation is: " <<duration << " seconds"<< " = "<<  duration/60.0 <<" minutes " << endl;
  output_file<< "Initial time point for the analysis: "<<tp_start << " Final  time point for the analysis: "<<tp_end <<endl;
  output_file<< "Total  number of time points used for the analysis:   "<<tp_end -tp_start+1<<endl;
  output_file<<"*************************************************************************************************" << endl;
  output_file.close();
}else{cout << "file can not be opened"  << endl;}
return;}























