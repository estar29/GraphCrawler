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
// https://chatgpt.com/share/67c13d21-8054-8001-971c-f693fa675598   ChatGPT conversation about fixing a rapidjson array error.
// https://www.geeksforgeeks.org/cpp-malloc/?ref=header_outind      Reviewing how malloc works in C++
// https://www.geeksforgeeks.org/strdup-strdndup-functions-c/       Strdup and Strdndup function allocation documentation
// https://www.geeksforgeeks.org/vector-erase-in-cpp-stl/           Vector erase function to clear all vector elements.

// Importing all necessary libraries.
#include <iostream>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <chrono>
#include "rapidjson/document.h"

using namespace std;

// Function to print out the libcurl calls.
size_t print_neighbors(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  std::string* my_string = (std::string*)userdata;

  for (size_t i = 0; i < nmemb; i++)
  {
    my_string->push_back(ptr[i]);
  }

}



// Main function, containing the actual BFS algorithm.
int main(int argc, char* argv[]) 
{
  // Starting the run-time clock.
  using std::chrono::high_resolution_clock;
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::seconds;

  auto start_time = high_resolution_clock::now();

  // max_level = the max level the BFS will travel to.
  // current_level = level of the traversal the BFS is currently at.
  int max_level = std::atoi(argv[2]);
  int current_level = 0;

  // Vectors to hold the next nodes to traverse and the visited nodes.
  std::vector <char*> next_nodes;
  std::vector <char*> is_visited;

  // Temp_child array to add nodes to each pass.
  std::vector <char*> temp_child;



  // Run the following when current_level is in the bounds of max_level.
  while (current_level <= max_level)
  {
    // Print out the current level of the traversal.
    std::cout << "---LEVEL " << current_level << "---\n" << "\n";
    
    // Initializing a new curl handle.
    CURL *curl = curl_easy_init();

    // Curl initialization check.
    if (!curl) 
    {
      std::cout << "Error allocating memory for libcurl.  Exiting...";
      return 1;
    }

    // Taking in initial case.
    // strdup --> copies the string and allocates memory for it.
    char* initial = strdup(argv[1]);
    const char* domain = "http://hollywood-graph-crawler.bridgesuncc.org/neighbors/";
    
    // Adding the initial case to next_nodes.
    next_nodes.push_back(initial);

    // Run while next_nodes has elements in it.
    while (next_nodes.size() != 0)
    {
      // Adding the temp string.
      char* temp = strdup(next_nodes.front());
      char dest[256];
      strcpy(dest, domain);

      // Remove temp from next_nodes, add to is_visited.
      next_nodes.erase(next_nodes.begin());
      is_visited.push_back(temp);

      // Converting temp to a URL friendly string.
      char* url_string = curl_easy_escape(curl, temp, 0);
      strcat(dest, url_string);

      // Making curl call using the url string.
      curl_easy_setopt(curl, CURLOPT_URL, dest);
      std::string output;
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&output);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, print_neighbors);
      curl_easy_perform(curl);

      // Free the url_string.
      curl_free(url_string);

      // From the output, perform BFS to get the immediate children of the root's children nodes, etc.
      using namespace rapidjson;
      Document doc;
      doc.Parse(output.c_str());

      // Checking any potential parsing errors.
      if (doc.GetParseError() != 0) 
      {
        std::cout << "Parsing error with code: " << doc.GetParseError() << " exiting.";
        return 1;
      }

      // Validate the JSON object.
      if (!doc.IsObject() || !doc.HasMember("neighbors"))
      {
        continue;
      }

      // Creating a new array to hold the neighbor nodes.
      Value& children = doc["neighbors"];
      if (!children.IsArray())
      {
        continue;
      }

      // For all the children, remove them if they have already been visited.
      for (auto& iter1 : children.GetArray())
      {
        bool been_visited = false;

        // Checking all nodes in the is_visited vector.
        for (auto& visit_iter : is_visited)
        {
          if (iter1.GetString() == std::string(visit_iter))
          {
            been_visited = true;
            break;
          }
          
        }
        
        // If the nodes has been visited, delete it from the children array; else add it.
        if (been_visited == true) 
        {
          children.Erase(children.Begin());
        }
        // Else, add the new element to temp_child and the is_visited vectors.
        else {
          char* new_element = strdup(iter1.GetString());
          is_visited.push_back(new_element);
          temp_child.push_back(new_element);
        }

      }

  }



  // Print and add the children from the previous pass to next_nodes.
  for (auto& temp_iter : temp_child)
  {
    std::cout << temp_iter << "\n";
    next_nodes.push_back(temp_iter);
  }

  // New-line break to improve formatting.
  std::cout << "\n";

  // Clear the temp_child array.
  temp_child.clear();

  // Freeing the remaining pointers.
  curl_easy_cleanup(curl);
  free(initial);
  current_level++;
  }

  // Getting the end time of the program run.
  auto end_time = high_resolution_clock::now();
  duration<double> run_time = end_time - start_time;

  std::cout << "\nTIME TAKEN: " << run_time.count() << " seconds";

  // Exit program.
  return 0;
}