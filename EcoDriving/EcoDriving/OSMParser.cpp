#include"OSMParser.h"
#include<fstream>
#include<string>
#include<iostream>

namespace EcoDriving {
	namespace Parsers {
		//Node	
		size_t Node::getNodeID()const {
			return this->nodeID;
		}

		double Node::getAltitude()const {
			return this->altitude;
		}

		double Node::getLatitude()const {
			return this->latitude;
		}

		double Node::getLongitude()const {
			return this->longitude;
		}

		bool Node::operator==(const Node &node)const {
			return (this->nodeID == node.getNodeID());
		}

		Node::Node(double latitude, double longitude, double altitude, size_t nodeID) {
			this->altitude = altitude;
			this->latitude = latitude;
			this->longitude = longitude;
			this->nodeID = nodeID;
		}

		//Way

		std::string Way::getName()const {
			return this->name;
		}

		size_t Way::getWayID()const {
			return this->wayID;
		}

		bool Way::isTwoWay()const {
			return this->twoWay;
		}

		bool Way::operator==(const Way &way)const {
			return (this->wayID == way.getWayID());
		}

		Way::Way(size_t wayID, std::string name, bool isTwoWay) {
			this->wayID = wayID;
			this->name = name;
			this->twoWay = isTwoWay;
		}

		//Conect

		size_t Conect::getWayID()const {
			return this->wayID;
		}

		size_t Conect::getDstID()const {
			return this->dstID;
		}

		size_t Conect::getSrcID()const {
			return this->srcID;
		}

		bool Conect::operator==(const Conect &conect)const {
			return (this->wayID == conect.getWayID());
		}

		Conect::Conect(size_t wayID, size_t srcID, size_t dstID) {
			this->wayID = wayID;
			this->srcID = srcID;
			this->dstID = dstID;
		}

	}
}

namespace EcoDriving {
	namespace Linker {
		void NodeParser(std::unordered_map<size_t,EcoDriving::Parsers::Node>& nodeTable, std::string filename){
			
			nodeTable.clear();

			double altitude, longitude, latitude;
			size_t ID;

			std::string help;
			std::ifstream file(filename);

			if (file.is_open()) {
				help = "";
				std::getline(file, help, ';');
				
				ID = std::stoi(help);
				
				std::getline(file, help, ';');

				longitude = std::stod(help);
				help = "";
				std::getline(file, help, ';');

				latitude = std::stod(help);
				help = "";
				std::getline(file, help, ';');//ignore the components in radians
				std::getline(file, help, ';');//ignore the components in radians
				help = "";
				altitude = 0;
				
				EcoDriving::Parsers::Node send(latitude, longitude, altitude, ID);

				//nodeTable.insert(std::make_pair(send.getNodeID(), send));

			}
			else {
				std::cout << "There Was An Issue Opening The Requested File" << std::endl;
			}
			file.close();
		}
	}
}
