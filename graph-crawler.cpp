// Evan Stark - February 20th 2025 - ITCS 4145 001
// This program demonstrates the process of Breadth-First Search (BFS)
// on a graph hosted on a web-based server.

// SOURCES USED
// 

// Importing all necessary libraries.
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#include "Bridges.h"
#include "DataSource.h"
#include "data_src/MovieActorWikidata.h"

using namespace bridges;

int main(int argc, char* argv[]) 
{
    // Creating new BRIDGES object.
    Bridges bridges(350, "estark3", "75927143440");
    bridges.setTitle("GraphCrawler Test");
    bridges.setDescription("Accessing Movie Wikidata");
    
    // Setting the data source to the bridges object.
    DataSource ds (&bridges);

    std::vector<MovieActorWikiData> wiki_data = ds.getMovieActorWikiData(2004, 2010);

    std::cout << "Movies from 2004 to 2010: " << wiki_data.size() << "\n";

    for (int i=0; i <5; i++) 
    {
        std::cout "MOVIE: " << wiki_data[i].getMovieURI() << " " << wiki_data[i].getMovieName();
        std::cout "ACTOR: " << wiki_data[i].getActorURI() << " " << wiki_data[i].getActorName() << "\n";
    }

    return 0;
}