//MIT License

//Copyright 2023 (c) termdax-v.2.h

//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:

//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.

//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

#ifndef _TERMDAX_IMPL
#define _TERMDAX_IMPL

#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>
#include <sys/ioctl.h>

#define TRM_NULL ""
#define TRM_RECT_TYPE 0
#define TRM_TEXT_TYPE 1
#define TRM_RED "\e[31m"
#define TRM_BLUE "\e[32m"
#define TRM_ENDBG "\e[0m"
#define TRM_GREEN "\e[34m"
#define TRM_EVENT_RESIZE 1
#define TRM_BG_RED "\e[41m"
#define TRM_BG_BLUE "\e[44m"
#define TRM_ENDCOLOR "\e[0m"
#define TRM_BG_GREEN "\e[42m"

int TRM_WINDOW_X = 0;
int TRM_WINDOW_Y = 0;

namespace trm
{

	struct TRM_RECTANGLE_STRUCT
	{

		int type;
		bool fill;
		char _char;
		int x, y, w, h;
		const char *bg_color;
		const char *text_color;

	};

	struct TRM_TEXT_STRUCT
	{
	
		int type;
		int x, y;
		const char *text;
		const char *bg_color;
		const char *text_color;

	};

	std::vector<TRM_TEXT_STRUCT> TRM_TEXT_ARRAY;
	std::vector<TRM_RECTANGLE_STRUCT> TRM_RECTANGLE_ARRAY;

	struct Termdax
	{

		private:

			struct termios newterm, savedterm;

			int printcr ( struct TRM_RECTANGLE_STRUCT object )
			{
				if ( object.text_color != TRM_NULL || object.bg_color != TRM_NULL ) {
					std::cout << object.text_color << object.bg_color << object._char << TRM_ENDBG << TRM_ENDCOLOR << std::flush;
					return 1;
				};
				std::cout << object._char << std::flush;
				return 0;
			};

			int printct ( struct TRM_TEXT_STRUCT object )
			{
				if ( object.text_color != TRM_NULL || object.bg_color != TRM_NULL ) {
					std::cout << object.text_color << object.bg_color << object.text << TRM_ENDBG << TRM_ENDCOLOR << std::flush;
					return 1;
				};
				std::cout << object.text << std::flush;
				return 0;
			};

		public:
			
			Termdax ()
			{
				winsize window;
				ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
				TRM_WINDOW_X = (int)window.ws_col;
				TRM_WINDOW_Y = (int)window.ws_row;
				tcgetattr(STDIN_FILENO, &savedterm);
			}

			int Event ()
			{
				winsize Events_window;
				ioctl(STDOUT_FILENO, TIOCGWINSZ, &Events_window);
				if ( Events_window.ws_col != TRM_WINDOW_X || Events_window.ws_row != TRM_WINDOW_Y ) {
					TRM_WINDOW_X = Events_window.ws_col;
					TRM_WINDOW_Y = Events_window.ws_row;
					return 1;
				};
				return 0;
			};

			void Background ( const char *color )
			{
				utilities.cmoveto(0,0);
				for ( int y = 0; y < TRM_WINDOW_Y; y++ ) {
					for ( int x = 0; x < TRM_WINDOW_X; x++ ) {
						std::cout << color << " " << TRM_ENDBG;
					};
				};
				utilities.cmoveto(0,0);
			};

			void Draw ()
			{
				for ( auto rectangle : TRM_RECTANGLE_ARRAY ) {
					utilities.cmoveto(rectangle.x, rectangle.y);
					for ( int y = 0; y < rectangle.h; y++ ) {
						for ( int x = 0; x < rectangle.w; x++ ) {
							if ( rectangle.fill ) {
								printcr(rectangle);
								continue;
							};
							if ( y == 0 || y == (rectangle.h - 1)) {
								printcr(rectangle);
							} else {
								if ( x == 0 || x == (rectangle.w - 1)) {
									printcr(rectangle);
								} else {
									utilities.cmoveforward(1);
								};
							};
						};
						utilities.cmovedown(1);
						utilities.cmovebackwards(rectangle.w);
					};
					utilities.cmoveto(0,0);
					
				};
				for ( auto text : TRM_TEXT_ARRAY ) {
					utilities.cmoveto(text.x, text.y);
					printct(text);
					utilities.cmoveto(0,0);
				};

				TRM_TEXT_ARRAY = {};
				TRM_RECTANGLE_ARRAY = {};
			};

			void Endtermdax ()
			{
				tcsetattr(STDIN_FILENO, TCSANOW, &savedterm);
			};

			struct utilities
			{
			
				void clear () {system("clear");};
				void rawterm () {system("stty raw");};
				void noecho () {system("stty -echo");};
				void nocanon () {system("stty -icanon");};
				void hidecursor () {system("tput civis");};
				void showcursor () {system("tput cnorm");};
				void cookedterm () {system("stty cooked");};
				void reset_terminal () {system("tput reset");};
				void cmoveup( int times ) { std::cout << "\033[" << times << "A"; };
				void cmovedown( int times ) { std::cout << "\033[" << times << "B"; };
				void cmoveforward( int times ) { std::cout << "\033[" << times << "C"; };
				void cmovebackwards( int times ) { std::cout << "\033[" << times << "D"; };
				void cmoveto( int cord_x, int cord_y ) { std::cout << "\033[" << cord_y << ";" << cord_x << "H"; };
					
			} utilities;
	};

	struct TRM_RECTANGLE
	{

		private:

			bool temp_fill;
			char temp_char;
			const char *temp_bg_color;
			const char *temp_text_color;
			int temp_x, temp_y, temp_w, temp_h;

		public:

			int type;
			bool fill;
			char _char;
			int x, y, w, h;
			const char *bg_color;
			const char *text_color;

			void Push (int x, int y, int w, int h, bool fill, char _char, const char *text_color, const char *bg_color)
			{
				this->x = x;
				this->y = y;
				this->w = w;
				this->h = h;
				this->type = TRM_RECT_TYPE;
				this->fill = fill;
				this->_char = _char;
				this->text_color = text_color;
				this->bg_color = bg_color;
				this->temp_fill = this->fill;
				this->temp_char = this->_char;
				this->temp_bg_color = this->bg_color;
				this->temp_text_color = this->text_color;
				this->temp_x = this->x; 
				this->temp_y = this->y;
				this->temp_w = this->w;
				this->temp_h = this->h;
				TRM_RECTANGLE_ARRAY.push_back({TRM_RECT_TYPE, this->fill, this->_char, this->x, this->y, this->w, this->h, this->bg_color, this->text_color });
			};

			void Update ()
			{
				if ( this->temp_fill != this->fill || this->temp_char != this->_char || this->temp_bg_color != this->bg_color || this->temp_text_color != this->text_color || this->temp_x != this->x || this->temp_y != this->y || this->temp_w != this->w || this->temp_h != this->h) {
					this->temp_fill = this->fill;
					this->temp_char = this->_char;
					this->temp_bg_color = this->bg_color;
					this->temp_text_color = this->text_color;
					this->temp_x = this->x; 
					this->temp_y = this->y;
					this->temp_w = this->w;
					this->temp_h = this->h;
					TRM_RECTANGLE_ARRAY.push_back({TRM_RECT_TYPE, this->fill, this->_char, this->x, this->y, this->w, this->h, this->bg_color, this->text_color });
				};
			};

	};

	struct TRM_TEXT
	{

		private:

			int temp_x, temp_y;
			const char *temp_text;
			const char *temp_bg_color;
			const char *temp_text_color;

		public:

			int type;
			int x, y;
			const char *text;
			const char *bg_color;
			const char *text_color;

			void Push (int x, int y, const char *text, const char *text_color, const char *bg_color)
			{
				this->x = x;
				this->y = y;
				this->text = text;
				this->bg_color = bg_color;
				this->text_color = text_color;
				this->temp_x = this->x;
				this->temp_y = this->y;
				this->temp_text = this->text;
				this->temp_bg_color = this->bg_color;
				this->temp_text_color = this->text_color;
				TRM_TEXT_ARRAY.push_back({TRM_TEXT_TYPE, this->x, this->y, this->text, this->bg_color, this->text_color});
			};

			void Update ()
			{
				if (this->temp_x != this->x || this->temp_y != this->y || this->temp_text != this->text || this->temp_bg_color != this->bg_color || this->temp_text_color != this->text_color ) {
					this->temp_x = this->x;
					this->temp_y = this->y;
					this->temp_text = this->text;
					this->temp_bg_color = this->bg_color;
					this->temp_text_color = this->text_color;
					TRM_TEXT_ARRAY.push_back({TRM_TEXT_TYPE, this->x, this->y, this->text, this->bg_color, this->text_color});
				};
			};

	};
}

#endif //_TERMDAX_IMPL
