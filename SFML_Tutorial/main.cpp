#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>

using namespace sf;


const int H = 20; // Visota
const int W = 10; // Shirina
int field[H][W] = { 0 }; // gamefield
int Nextfield[4][4] = { 0 }; // Nexttetramino

//for music
float timerm = 0;
bool musico = false;
//for score
int score = 0;
// timer and deley 
float timer = 0, delay = 0.8;

//Horizontal
int hr = 0;
//rotation
bool rotate = false;

//Color
int colorNum = rand()%7+1;
int nextColor = 0;

//while not GameOver -false
bool GameOver = true;
bool GameExit = false;
//for tetraminos
int n = rand() % 7;
int n1 = rand() % 7;
// Tetramino array
int figures[7][4] =
{
	1,3,5,7, // i
	0,2,3,5, // z
	1,3,2,4, // s
	1,3,2,5, // t
	0,1,3,5, // l
	1,3,5,4, // j
	0,1,2,3, // o
};

struct Point
{
	int x, y;
} a[4], b[4], c[4];

//start foo
void forStart(bool &Gameover) {
	if (GameOver)
	{
		GameOver = false;
		n = rand() % 7;
		for (int i = 0; i < 4; i++)
		{
			a[i].x = figures[n][i] % 2+4;
			a[i].y = figures[n][i] / 2;
		}

		n1 = rand() % 7;
		for (int i = 0; i < 4; i++)
		{
			c[i].x = figures[n1][i] % 2 ;
			c[i].y = figures[n1][i] / 2;
		}
		nextColor = rand() % 7+1;
		for (int i = 0; i < 4; i++)
		{
			Nextfield[c[i].y][c[i].x] = nextColor;
		}
	}
}
//Vishli za granicu(proverka) ili natknulsa na kordinati kot zanati
int Check()
{
	for (int i = 0; i < 4; i++)
	{
		if (a[i].x < 0 || a[i].x >= W || a[i].y >= H)
		{
			return 0;
		}
		else if (field[a[i].y][a[i].x]!=0)
		{
			return 0;
		}
		
	}
	return 1;
};

//Horisontal moving foo
void Horisont(int &hr) {
	
	for (int i = 0; i < 4; i++) 
	{
		b[i] = a[i];
		a[i].x += hr;
	}
	hr = 0;
	bool check = Check();
	if (check == false) 
	{
		for (int i = 0; i < 4; i++) 
		{ 
			a[i] = b[i]; 
		}
	}
}

//Rotate foo
void Rotate(bool&rotate)
{
	if (rotate)
	{
		Point p = a[1]; // задаем центр вращения
		for (int i = 0; i < 4; i++)
		{
			int x = a[i].y - p.y; //y-y0
			int y = a[i].x - p.x; //x-x0
			a[i].x = p.x - x;
			a[i].y = p.y + y;
		}
		bool check = Check();
		if (check==false)
		{
			for (int i = 0; i < 4; i++)
			{ 
				a[i] = b[i]; 
			}
		}
	}
	rotate = false;
}

//Vertical moving foo
void Vertical(float&timer,float &delay) {
	
	if (timer > delay)
	{
		for (int i = 0; i < 4; i++) 
		{
			b[i] = a[i]; a[i].y += 1; 
		}
		bool check = Check();
		if (check==false)
		{
			//ochisheniye next polya
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					Nextfield[i][j] = 0;
				}
			}
			score += 20;
			for (int i = 0; i < 4; i++)
			{
				field[b[i].y][b[i].x] = colorNum;
			}

			colorNum = nextColor;
			nextColor = rand() % 7 + 1;
			n = n1;
			for (int i = 0; i < 4; i++)
			{
				a[i].x = figures[n][i] % 2 + 4;
				a[i].y = figures[n][i] / 2;
			}

			n1 = rand() % 7;
			for (int j = 0; j < 4; j++)
			{
				c[j].x = figures[n1][j] % 2;
				c[j].y = figures[n1][j] / 2;
			}		
			for (int i = 0; i < 4; i++)
			{
				Nextfield[c[i].y][c[i].x] = nextColor;				
			}

			
		}
		timer = 0; delay = 0.8;
	}
}



int main()
{	srand(time(0));
	


	RenderWindow window(VideoMode(280, 365), "Rustam's Tetris:)");

	//Sozdaniye Texturi i frama
	Texture texture, frame;
	texture.loadFromFile("images\\tiles.png");
	frame.loadFromFile("images\\Myframe7.png");
	Sprite sprite(texture);
	sprite.setTextureRect(IntRect(0, 0, 18, 18));
	Sprite sprite1(frame);

	//For music
	Music music;
	music.openFromFile("Music\\TetrisMusic1.ogg");
	music.setVolume(35);
	music.play();

	//For sound
	SoundBuffer buffer;
	buffer.loadFromFile("Music\\sound1.ogg");
	Sound sound;
	sound.setBuffer(buffer);

	//For sound gameover
	SoundBuffer buffer1;
	buffer1.loadFromFile("Music\\GameOver.ogg");
	Sound sound1;
	sound1.setBuffer(buffer1);		
				
	//For text Next:
	Font font;
	font.loadFromFile("Text\\file.ttf");
	Text text;
	text.setFont(font);
	text.setString("Next:");
	text.setPosition(210, 50);
	text.setCharacterSize(22);
	text.setFillColor(Color::Blue);
	text.setStyle(Text::Bold| Text::Underlined);


	//For text GameOver:
	
	font.loadFromFile("Text\\file.ttf");
	Text gameover;
	gameover.setFont(font);
	gameover.setString("Game Over:(");
	gameover.setPosition(8, 130);
	gameover.setCharacterSize(45);
	gameover.setFillColor(Color::Black);
	gameover.setStyle(Text::Bold );



	//For text Score:	
	Text text1;
	text1.setFont(font);
	text1.setString("Score:");
	text1.setPosition(205,220);
	text1.setCharacterSize(22);
	text1.setFillColor(Color::Red);
	text1.setStyle(Text::Bold|Text::Underlined);

	Text scoreCurrent;
	scoreCurrent.setFont(font);
	scoreCurrent.setFillColor(sf::Color::Black);
	scoreCurrent.setStyle(Text::Bold | Text::Underlined);
	scoreCurrent.setCharacterSize(20);
	scoreCurrent.setPosition(205, 250);

	


	

	
	//Class For time
	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		
		timerm += time;


		std::stringstream s;
		s << score;
		scoreCurrent.setString(s.str());



		if (musico==true)
		{
			if (timerm > 180.000)
			{
				music.openFromFile("Music\\TetrisMusic1.ogg");
				music.play();
				music.setVolume(35);
				timerm = 0;
				musico = false;
			}
		}
		else
		{
			if (timerm > 83.000)
			{
				music.openFromFile("Music\\TetrisMusic2.ogg");
				music.play();
				music.setVolume(40);
				timerm = 0;
				musico = true;
			}
		}
		
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{

				window.close();
			}
			if (event.type == Event::KeyPressed)
			{

				if (event.key.code == Keyboard::Up)
				{
					if (n != 6)
					{
						rotate = true;
					}
				}
				if (event.key.code == Keyboard::Space)
				{
					if (n != 6)
					{
						rotate = true;
					}
				}
				else if (event.key.code == Keyboard::Left)
				{
					hr = -1;
				}
				else if (event.key.code == Keyboard::D)
				{
					hr = 1;
				}
				else if (event.key.code == Keyboard::A)
				{
					hr = -1;
				}
				else if (event.key.code == Keyboard::Right)
				{
					hr = 1;
				}
			}
		}
		if (score >= 1000&&score<=2500)
		{
			delay = 0.6;
		}
		else if (score >= 2500 && score <= 4500)
		{
			delay = 0.5;
		}
		else if (score >= 4500 && score <= 9000)
		{
			delay = 0.4;
		}
		else if (score >= 9100 && score <= 18000)
		{
			delay = 0.3;
		}
		
		if (Keyboard::isKeyPressed(Keyboard::Down))
		{
			delay = 0.03;
		}
		//moyi foo
		forStart(GameOver);
		Horisont(hr);
		Rotate(rotate);
		Vertical(timer, delay);
				
		
		int k = H - 1;
		for (int i = H - 1; i > 0; i--)
		{
			int count = 0;
			for (int j = 0; j < W; j++)
			{
				if (field[i][j] != 0)
				{
					count++;
				}
				field[k][j] = field[i][j];
				
			}
			if (count < W)
			{
				k--;
			}
			else
			{
				score += 100;
				sound.setVolume(100);
				sound.play();				
			}			
		}
		

		
		window.clear(Color::White);
		
		window.draw(sprite1);
		
		for (int i = 0; i < H; i++)
		{
			for (int j = 0; j < W; j++)
			{
				if (field[i][j] != 0)
				{
					sprite.setTextureRect(IntRect(field[i][j] * 18, 0, 18, 18));
					sprite.setPosition(j * 18, i * 18);
					sprite.move(8,0);
					window.draw(sprite);
				}				
			}
		}
		for (int i = 0; i < 4; i++)
		{			
			sprite.setTextureRect(IntRect(colorNum * 18, 0, 18, 18));
			sprite.setPosition(a[i].x * 18, a[i].y * 18);	
			sprite.move(8, 0);
			window.draw(sprite);
		}			

		//next:
		window.draw(text);

		////score:
		window.draw(text1);
		window.draw(scoreCurrent);
		for (int i = 0; i < 4; i++)
		{
			for (int d = 0; d < 4; d++)
			{
				
				if (Nextfield[i][d] != 0)
				{
					sprite.setTextureRect(IntRect(Nextfield[i][d] * 18, 0, 18, 18));
					sprite.setPosition(d * 18, i * 18);
					if (n1!=0)
					{
						
					  sprite.move(220, 90);
					  window.draw(sprite);
					}
					else
					{
						sprite.move(212, 110);
						window.draw(sprite);
					}
					
				}
				
			}			
		}

		for (size_t i = 3; i < 8; i++)
		{
			if (field[0][i] != 0)
			{
				sound1.play();
				window.draw(gameover);
				window.display();
				music.setVolume(0);
				for (int i = 0; i < 4; i++)
				{
					field[1][i] = 0;
					while (!field[1][i])
					{
						
					}
				}
				
			}			
		}
		
		
	     window.display();		
	}

	return 0;
}






