# Evan Stark - February 23rd 2025 - ITCS 4145 001
# Make file for the Graph Crawler Assignment.

LDFLAGS=-lcurl
LD=g++
CXXFLAGS= -I ~/rapidjson/include

graph-crawler: graph-crawler.o
	$(LD) -o $@ $< $(LDFLAGS)

graph-crawler.o: graph-crawler.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
