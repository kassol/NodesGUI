#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>  
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
using namespace std;
using namespace boost::uuids; 

int main(int argc, char* argv[])
{
	random_generator rgen;  
	uuid ranUUID = rgen();
	string id = boost::lexical_cast<string>(ranUUID);
	return 0;
}

