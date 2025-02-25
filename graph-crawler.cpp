// Evan Stark - February 20th 2025 - ITCS 4145 001
// This program demonstrates the process of Breadth-First Search (BFS)
// on a graph hosted on a web-based server.

// SOURCES USED
// https://curl.se/libcurl/c/libcurl-tutorial.html

// Importing all necessary libraries.
#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include "rapidjson/document.h"

using namespace std;

size_t output_test(char *ptr, size_t size, size_t nmemb, void *userdata) {
  std::string* my_string = (std::string*)userdata;

  for (size_t i = 0; i < nmemb; i++) {
    my_string->push_back(ptr[i]);
  }

}

int main(int argc, char* argv[]) 
{
  // Initializing a new curl handle.
  CURL *curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/Tom%20Hanks");

  std::string output;
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&output);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, output_test);

  curl_easy_perform(curl);

  std::cout << output << "\n";
  
  // From the output, perform BFS to get the immediate children of the root's children nodes, etc.
  using namespace rapidjson;
  Document neighbors;
  neighbors.Parse(output.c_str());
  std::cout << neighbors.GetParseError() << "\n";

  std::string nn = neighbors["neighbors"].GetString();
  std::cout << nn << "\n";
  
  // Cleanup curl and return.
  curl_easy_cleanup(curl);
  return 0;
}
