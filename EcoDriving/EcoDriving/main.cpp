
#include "OSMParser.h"
#include <unordered_map>
#include "Graph.h"
#include "Location.h"
#include "ElectricVehicle.h"
#include "PROJECT_SETTINGS_MACROS.h"
#include "CoordinateSystem.h"
#include "MATH_CONSTANTS_MACROS.h"
#include <cstdio>
#include "graphviewer.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace EcoDriving::Linker;

void main(void) {
	GraphViewer *gv = new GraphViewer(600, 600, true);

	gv->setBackground("background.jpg");

	gv->createWindow(600, 600);


	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

	Linker a;
	std::cout << "Nodes:" << a.nodes.size() << std::endl;
	std::cout << "Connections:" << a.conections.size() << std::endl;
	std::cout << "Ways:" << a.ways.size() << std::endl;
	std::cout << "Node Locations:" << a.locationNodes.size() << std::endl;

	EcoDriving::EcoVehicle::ElectricVehicle car(0,0,0,100,1000);

	Graph<EcoDriving::Location::Location> locationGraph;
	for (auto it = a.locationNodes.begin(); it != a.locationNodes.end(); ++it) {
		locationGraph.addVertex(it->second);
		gv->addNode(it->first,it->second.getCoordinates().x, it->second.getCoordinates().y);
	}
	int c = 0;
	for (auto it = a.conections.begin(); it != a.conections.end(); ++it) {
		std::vector<std::pair<size_t, size_t>> edges = it->second.getEdges();
		for (int i = 0; i < edges.size(); i++) {
			c++;
			EcoDriving::Location::Location src = a.locationNodes[edges[i].first];
			EcoDriving::Location::Location dst = a.locationNodes[edges[i].second];

			double distance = src.getCoordinates().distanceCalculation(dst.getCoordinates());
			double timeToArrival = distance * car.getVelocity();
			double heightDiff = dst.getCoordinates().altitude - src.getCoordinates().altitude;

			double usedBattery = 0;
			if (heightDiff == 0) {
				usedBattery = distance*FLAT_ENERGY_COST;
			}
			else if (heightDiff < 0) {
				usedBattery = -distance*DOWN_HILL_REFENERATION;
			}
			else if (heightDiff > 0) {
				usedBattery = distance*UP_HILL_ENERGY_COST;
			}

			double weight = usedBattery;
			//cout << "Weight:" << weight << endl;
			gv->addEdge(c, src.getNodeID(), dst.getNodeID(), EdgeType::DIRECTED);

			locationGraph.addEdge(src, dst, weight);
		}
	}

	locationGraph.dijkstraShortestPath(a.locationNodes.begin()->second);
	EcoDriving::Location::Location end = (--a.locationNodes.end())->second;
	vector<EcoDriving::Location::Location> res = locationGraph.getPath(a.locationNodes.begin()->second, end);

	for (int i = 0; i < res.size(); i++) {
		cout << "ID:" << res[i].getNodeID() << endl;
	}
	gv->rearrange();

	std::cout << "Number of Vertexes:" << locationGraph.getNumVertex() << std::endl;

	getchar();
	return;
}