#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


// This version of the overloaded funciton "parse" should deal with strings.
void parse(char filePath[], char fieldName[], char equiv = '=', char delim = ','){
		
	// File name
	// std::string filePath = filePath;
	printf("Function output: %s\n", filePath);
	
	// Open file
	std::ifstream file(filePath);
	
	// Declare line variable
	std::string line;
	
	// Read over the file
	while(std::getline(file, line)){
		std::istringstream iss(line);
		std::string result;
		if(std::getline(iss, result, equiv)){
			if(result == fieldName){
				std::string token;
				while(std::getline(iss, token, delim)){
					std::cout << token << std::endl;
				}
			}
		}
	}	
}

int main(int argc, char *argv[]){
	
	// File name
	char *filePath = argv[1];
	
	// Field name to read
	char *fieldName = argv[2];
	
	// char filePath2[] = argv[1];
	printf("Main output: %s\n", filePath);
	

		
	parse(filePath, fieldName);
	
	return(0);
}

