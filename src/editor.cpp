#include "editor.hpp"
#include <vector> 
#include <string>
#include <cctype> 

#include <ncurses.h>
#define KEY_ESC 27

void refreshLine(std::vector<std::string>, int);
void refreshAll(std::vector<std::string>);

void editor::edit(std::vector<std::string>& content) {
	clear();
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
	while(true) {
		if(refall) {
			refreshAll(content);	
			refall = false;
		}
		else if(refline) {
			refreshLine(content, cy);
			refline = false;
		}

		move(cy, cx);
		key = getch();
		if(std::isprint(key)) {
			content[cy].insert(content[cy].begin()+cx, key);
			cx++;
			refline = true;
		}
		else {
			switch(key) {
				case KEY_BACKSPACE: 
					if(cx > 0 && content[cy].size() > 0) {
						content[cy].erase(cx-1, 1);
						cx--;
						refline = true;
					}
					else if(cx == 0 && cy > 0) {
						content[cy-1] += content[cy];
						content.erase(content.begin()+cy);
						move(cy, 0); 
						clrtoeol();
						move(content.size(), 0);
						clrtoeol();
						cx = content[cy-1].size();
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
				case '\t': 
					for(int i = 0; i < 4; i++) {
						content[cy].insert(content[cy].begin()+cx, ' ');
						cx++;
					}
					refline = true;
					break;
				case KEY_HOME: 
					cx = 0;
					break;
				case KEY_END: 
					cx = content[cy].size();
					break;
				case KEY_LEFT: 
					if(cx > 0) 
						cx--;
					break;
				case KEY_RIGHT: 
					if(cx < content[cy].size()) 
						cx++;
					break;
				case KEY_UP: 
					if(cy > 0) 
						cy--;
					if(cx > content[cy].size() || cx == content[cy+1].size()) 
						cx = content[cy].size();
					break;
				case KEY_DOWN: 
					if(cy < content.size()-1) 
						cy++;
					if(cx > content[cy].size() || cx == content[cy-1].size())
						cx = content[cy].size();
					break;
				case KEY_ESC: 
					return;
			}
		}
	}
}

void refreshLine(std::vector<std::string> lines, int index) {
	move(index, 0);
	clrtoeol();
	addstr(lines[index].c_str());
}

void refreshAll(std::vector<std::string> lines) {
	for(int i = 0; i < lines.size(); i++)
		refreshLine(lines, i);
}



