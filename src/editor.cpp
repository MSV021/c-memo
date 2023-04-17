#include "editor.hpp"
#include <vector> 
#include <string>
#include <cctype> 

#include <ncurses.h>

void editor::edit(std::vector<std::string>& content) {
	int cy = 0, cx = 0;
	if(content.size() == 0) 
		content.push_back(std::string());
	if(content.size() > 0) {
		for(auto line : content) {	
			addstr(line.c_str());
			addch('\n');
		} 
		cy = content.size()-1;
		cx = content.back().size();
	}

	int key;
	bool refline = false, refall = false;
	while(1) {
		if(refall) {
			for(int i = 0; i < content.size(); i++) {
				move(i, 0);
				clrtoeol();
				addstr(content[i].c_str());
			}
			refall = false;
		}

		if(refline) {
			move(cy, 0);
			clrtoeol();
			addstr(content[cy].c_str());
			refline = false;
		}

		move(cy, cx);
		key = getch();
		if(std::isprint(key)) {
			content[cy].insert(content[cy].begin()+cx, key);
			refline = true;
			cx++;
		}
		else {
			switch(key) {
				case KEY_BACKSPACE: 
					if(cx > 0 && content[cy].size() > 0) {
						content[cy].erase(cx-1, 1);
						refline = true;
						cx--;
					}
					else if(cx == 0 && cy > 0) {
						cx = content[cy-1].size();
						content[cy-1] += content[cy];
						content.erase(content.begin()+cy);
						move(cy, 0); 
						clrtoeol();
						move(content.size(), 0);
						clrtoeol();
						cy--;
						refall = true;
					}
					break;
				case '\n':
					content.insert(content.begin()+cy+1, std::string());
					content[cy+1] += content[cy].substr(cx);
					content[cy].erase(cx);
					cy++;
					cx = 0;
					refall = true;	
					break;
				case KEY_HOME: 
					cx = 0;
					break;
				case KEY_END: 
					cx = content[cy].size();
					break;
				case KEY_LEFT: 
					if(cx > 0) cx--;
					break;
				case KEY_RIGHT: 
					if(cx < content[cy].size()) cx++;
					break;
				case KEY_UP: 
					if(cy > 0) cy--;
					if(cx > content[cy].size()) cx = content[cy].size();
					break;
				case KEY_DOWN: 
					if(cy < content.size()-1) cy++;
					if(cx > content[cy].size()) cx = content[cy].size();
					break;
				case 27: 
					return;
			}
		}
	}
}



