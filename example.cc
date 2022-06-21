#include <iostream>
#include <fstream>

#include "include/rs/multi_map.h"

using namespace std;

unsigned long long N = 1000000;
std::vector<uint32_t> gaps;
// vector<int> keys(1e6);
vector<int> keys;
int epsilon = 32; // space-time trade-off parameter

void RadixSplineExample() {

  // Create random keys.
  // generate(keys.begin(), keys.end(), rand);
  // keys.push_back(8128);
  // sort(keys.begin(), keys.end());

  keys.reserve(N);

  // Build RadixSpline.
  int min = keys.front();
  int max = keys.back();
  rs::Builder<uint64_t> rsb(min, max, 18, epsilon); 
  for (const auto& key : keys) 
  {
    // std::cout << key << std::endl;
    rsb.AddKey(key);
  }
  rs::RadixSpline<uint64_t> rs = rsb.Finalize();

  printf("\n************************************************************ ACTUAL STRUCTURE (RadixSpline) ************************************************************\n");
  rs.GetSize();

  // Search using RadixSpline.
  int entries_read = 0;
  for(int i = 0; i<keys.size(); i++)
  {
    rs::SearchBound bound = rs.GetSearchBound(keys[i]);
    // cout << "The search key " << keys[i] << " is in the range: [" << bound.begin << ", "
    //    << bound.end << ")" << endl;
    auto start = begin(keys) + bound.begin, last = begin(keys) + bound.end;
    entries_read += (bound.end - bound.begin);
    // std::cout << "IOs: " << (double)(bound.end - bound.begin)/32.0 << std::endl;
    // cout << "The key is at position: "
    //    << std::lower_bound(start, last, keys[i]) - begin(keys) << endl;
  }
  std::cout << "\nIO/read: " << (double)(entries_read)/(32.0*(double)keys.size()) << std::endl;
  // rs::SearchBound bound = rs.GetSearchBound(min+1);
  // cout << "The search key " << min+1 << " is in the range: [" << bound.begin << ", "
  //      << bound.end << ")" << endl;
  // auto start = begin(keys) + bound.begin, last = begin(keys) + bound.end;
  // cout << "The key is at position: "
  //      << std::lower_bound(start, last, 8128) - begin(keys) << endl;
}

int main(int argc, char** argv) {
  double standard_deviation;
  double mean;
  unsigned int a, b;
  if(argc <= 1)
  {
      standard_deviation = 5.0;
      mean = 3.0;
  }
  else if(strcmp(argv[1], "0") == 0)  // UNIFORM
  {
      N = atol(argv[2]);
      a = atoi(argv[3]);
      b = atoi(argv[4]);
      printf("distribution: %s data_entries: %llu a: %d b: %d epsilon: %d \n", argv[1], N, a, b, epsilon);
  }
  else if(strcmp(argv[1], "1") == 0)  // NORMAL
  {
      N = atol(argv[2]);
      mean = atof(argv[3]);
      standard_deviation = atof(argv[4]);
      printf("distribution: %s data_entries: %llu mean: %.3f standard_deviation: %.3f epsilon: %d \n", argv[1], N, mean, standard_deviation, epsilon);
  } 

  // read from file
  std::ifstream inputFile("/Users/subarnachatterjee/Dropbox/Cosine_Learned_Component/code_fun/data.txt");
  // test file open   
  if (inputFile) {        
      int key;

      // read the elements in the file into a vector  
      while ( inputFile >> key ) {
          // std::cout << key << std::endl;
          keys.push_back(key);
      }
  }
  else
  {
      printf("ERROR OPENING FILE\n");
  }
  // close the file
  std::sort(keys.begin(), keys.end());

  RadixSplineExample();

  return 0;
}
