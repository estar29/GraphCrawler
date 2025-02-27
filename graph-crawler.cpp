// Evan Stark - February 20th 2025 - ITCS 4145 001
// This program demonstrates the process of Breadth-First Search (BFS)
// on a graph hosted on a web-based server.

// SOURCES USED
// https://curl.se/libcurl/c/libcurl-tutorial.html          libcurl tutorial
// https://curl.se/libcurl/c/curl_free.html                 curl_free operation documentation
// https://curl.se/libcurl/c/curl_easy_escape.html          curl_easy_escape operation documentation
// https://en.cppreference.com/w/cpp/container/vector       doucmentation for std::vector
// https://www.geeksforgeeks.org/vector-of-strings-in-cpp/  creating string vectors in C++
// https://rapidjson.org/md_doc_tutorial.html               rapidjson document tutorial; used for creating the array
// https://cplusplus.com/reference/string/string/           C++ string documentation
// https://cplusplus.com/forum/beginner/250860/             Debugging an error with rapidjson
// https://github.com/Tencent/rapidjson/issues/1235         Debugging ConstValueIterator issue for array traversal
// https://stackoverflow.com/questions/18468229/concatenate-two-char-strings-in-a-c-program   How to concatenate char* strings
// 

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
  int max_level = std::atoi(argv[2]);
  int current_level = 0;

  // Run the following when current_level is in the bounds of max_level.
  while (current_level <= max_level)
  {
    // Print out the current level of the traversal.
    std::cout << "Level " << current_level << "\n";
    // Initializing a new curl handle.
    CURL *curl = curl_easy_init();

    // Taking in initial case.
    char* initial = (char*) malloc(sizeof(argv[1]) + 1);
    initial = argv[1];
    char* domain = (char*) malloc(sizeof(char) * 200);
    domain = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";

    // Vectors to hold the next nodes to traverse and the visited nodes.
    std::vector <char*> next_nodes;
    std::vector <char*> is_visited;

    // Adding the initial case to next_nodes.
    next_nodes.push_back(initial);
  
    // Run while next_nodes has elements in it.
    while (next_nodes.size() != 0)
    {
      // Adding the temp string and converting to a URL friendly string.
      char* temp = (char*) malloc(sizeof(char) * 200);
      temp = (char*) next_nodes.front();
      char* dest = (char*) malloc(sizeof(temp) + sizeof(domain) + 1);
      strcpy(dest, domain);

      char* url_string = curl_easy_escape(curl, temp, 0);
      strcat(dest, url_string);

      // Making curl call using the url string.
      curl_easy_setopt(curl, CURLOPT_URL, dest);
      std::string output;
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&output);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_neighbors);
      curl_easy_perform(curl);

      // Free the url_string..
      curl_free( (void*) url_string);

      // Remove temp/front from next_nodes, add to is_visited.
      next_nodes.erase(next_nodes.begin());
      is_visited.push_back(temp);
      
      // From the output, perform BFS to get the immediate children of the root's children nodes, etc.
      using namespace rapidjson;
      Document neighbors;
      neighbors.Parse(output.c_str());

      if (neighbors.GetParseError() != 0) 
      {
        std::cout << "Parsing error with code: " << neighbors.GetParseError() << " exiting.";
        return 1;
      }

      // Array for children nodes.
      Value& children = neighbors["neighbors"];
      assert(children.IsArray());

      // For all the children, print out their value if not in the is_visited vector.
      for (rapidjson::Value::ConstValueIterator iter1 = children.Begin(); iter1 != children.End(); iter1++)
      {
	      // Iterate through is_visited to check if the element exists; remove element and break if yes.
        for (size_t j = 0; j < is_visited.size(); j++)
        {
          if (iter1->GetString() == is_visited[j])
          {
            children.Erase(iter1);
          }
        }
        
        char* new_element = (char*) iter1->GetString();
        is_visited.push_back(new_element);
        next_nodes.push_back(new_element);
      }
    
    // For all of the remaining children, print them out and empty the array.
    for (rapidjson::Value::ConstValueIterator iter2 = children.Begin(); iter2 != children.End(); iter2++) 
    {
      std::cout << iter2->GetString() << "\n";
      children.Erase(iter2);
    }
    
    // Cleanup curl, free temp and dest, increment current_level.
    curl_easy_cleanup(curl);
    free(temp);
    free(dest);
    current_level++;
  }

  // Freeing the remaining pointers.
  free(initial);
  free(domain);
  }

}