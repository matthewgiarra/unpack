#include <iostream>
#include <string>
#include <fstream>
#include <regex>

#define KRED  "\x1B[31m"
#define KBLU  "\x1B[34m"
#define RESET "\033[0m"

int main(int argc, char *argv[]){
	
	// Check number of input args
	if(argc < 2){
		printf(KRED "Error: Specify at least two inputs\n(missing file path and regexp string)\n" RESET);
		return(-1);
	}
	
	if(argc < 3){
		printf(KRED "Error: Specify at least two inputs\n(missing regexp string)\n" RESET);
		return(-1);
	}
		
	// Read the input argument and assign it to the string variable "filename"
	std::string filePath = argv[1];
	
	// Read argument as string to be matched
	std::string string_to_match = argv[2];
	
	// Prefix / postfix for regexp
	std::string pre = "\\b(";
	std::string post = ")([^ ]*)";
	
	
	// "\\b(_STRING_)([^ ]*)";
	
	// Combine the prefix + string to match + postfix
	std::string reg_exp = pre + string_to_match + post;
	
	// Create the regular expression, passing the combined
	// string as an array of characters (instead of a string object).
	std::regex reg_exp_string (reg_exp.c_str());
	
	// Declare the match boolean variable
	std::smatch reg_exp_match;  
	
	// Declare a variable called "line" of type string
	std::string line;
	
	// Open a file
	std::ifstream myfile (filePath);
	
	// Check to make sure the file was open, then read from it
	if(myfile.is_open()){
		
		// Inform user that file was opened successfully.
		printf(KBLU "Opened file!\n\n" RESET);
		
		// Declare line number
		int line_num = 0;
		
		// Print each line of the file.
		while(getline (myfile, line)){
			if(std::regex_search(line, reg_exp_match, reg_exp_string)){
				
				// Inform the user that the string was matched.
				// Print the number of the line on which the match occurred.
				printf(KBLU "String matched on line %d\n" RESET, line_num);
				
				// Print the whole line
				std::cout << "Matched line: \"" << line << "\"\n";
			}
			
			// Increment line number
			line_num++;
		}
		
	}
	else{
		printf(KRED "Error: Unable to open file!\n" RESET);
	}
	
	// Close the file
	myfile.close();
	
	// GTFO
	return(0);
	
}








