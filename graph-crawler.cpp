// Evan Stark - February 20th 2025 - ITCS 4145 001
// This program demonstrates the process of Breadth-First Search (BFS)
// on a graph hosted on a web-based server.

// SOURCES USED
// https://curl.se/libcurl/c/libcurl-tutorial.html : libcurl tutorial
// https://en.cppreference.com/w/cpp/container/vector : doucmentation for std::vector
// https://www.geeksforgeeks.org/vector-of-strings-in-cpp/ : creating string vectors in C++
// https://rapidjson.org/md_doc_tutorial.html : rapidjson document tutorial; used for creating the array.
// https://cplusplus.com/reference/string/string/ : C++ string documentation

// Importing all necessary libraries.
#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include "rapidjson/document.h"

using namespace std;

size_t print_neighbors(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  std::string* my_string = (std::string*)userdata;

  for (size_t i = 0; i < nmemb; i++)
  {
    my_string->push_back(ptr[i]);
  }

}



int main(int argc, char* argv[]) 
{
  // max_level = the max level the BFS will travel to.
  // current_level = level of the traversal the BFS is currently at.
  int max_level = (int) argv[2];
  int current_level = 0;

  // Run the following when current_level is in the bounds of max_level.
  while (current_level <= max_level)
  {
    // Print out the current level of the traversal.
    std::cout << "Level " << current_level;
  // Initializing a new curl handle.
  CURL *curl = curl_easy_init();

  // Taking in initial case.
  std::string initial = (std::string) argv[1];
  std::string domain = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";

  // Vectors to hold the next nodes to traverse and the visited nodes.
  std::vector <std::string> next_nodes;
  std::vector <std::string> is_visited;

  // Adding the initial case to next_nodes.
  next_nodes.insert(0, initial);
  
  // Run while next_nodes has elements in it.
  while (next_nodes.size() != 0)
    {
      std::string temp = next_nodes.front();

      // Adding the temp string to the domain.
      domain.append(temp);
      curl_easy_setopt(curl, CURLOPT_URL, domain);
      std::string output;
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&output);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_neighbors);
      curl_easy_perform(curl);

      // Remove temp/front from next_nodes, add to is_visited.
      next_nodes.erase(next_nodes.front());
      is_visited.insert(0, temp);
      
      // From the output, perform BFS to get the immediate children of the root's children nodes, etc.
      using namespace rapidjson;
      Document neighbors;
      neighbors.Parse(output.c_str());
      std::cout << neighbors.GetParseError() << "\n";

      // 
      const Value& children = neighbors["neighbors"];
      assert(children.IsArray());  // Delete later?

      // For all the children, print out their value if not in the is_visited vector.
      for (size_t i=0; i < arr.Size(); i++)
      {
	if (is_visited
	std::cout << arr[i].GetString() << "\n";
      }
  
  // Cleanup curl and return.
  curl_easy_cleanup(curl);
  return 0;

  // Increment current_level.
  current_level++;
  }

}
