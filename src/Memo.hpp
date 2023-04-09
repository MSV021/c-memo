#include <vector> 
#include <string> 

struct Memo {
	std::string title;
	std::string path;
	std::vector<std::string> content;
   	const static int maxTitleLen = 25;

	Memo(std::string path, std::string title) : path{path}, title{title} {}
};
