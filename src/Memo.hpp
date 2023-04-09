#include <vector> 
#include <string> 

class Memo {
private: 
	std::string title;
	std::string path;
	std::vector<std::string> content;
public: 
	Memo(std::string path, std::string title) : path{path}, title{title} {}
   	const static int maxTitleLen = 25;
};
