#ifndef TelaOLED_h
#define TelaOLED_h

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LARGURA_OLED 128
#define ALTURA_OLED 64
#define RESET_OLED -1

//Pinos do OLED na ESP32
#define OLED_SDA 5
#define OLED_SCL 4

//Para o cado do NODEMCU com OLED 0.96 buildin
//#define OLED_SDA D5
//#define OLED_SCL D6

typedef struct Menu
{
  char id[64];
  char texto[64];
};


class TelaOLED
{
public:
   TelaOLED() : display(LARGURA_OLED, ALTURA_OLED, &Wire, RESET_OLED), accents("àaâäcçeèé12ëïîioöoôûuUüùÂA34ÄÊ5EË6ÎÏ7ÔÖ8UÛÜ9ÀÇÉÈÊ0Ô")
	{
		clearMenuOptions();
	}

   ~TelaOLED()
	{
	}

	void Start()
	{
		Wire.begin(OLED_SDA,OLED_SCL);
		if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
			Serial.println("Falha ao abrir display");
		else
    {
      display.setRotation(2);
			display.clearDisplay();
    }
	}

	bool addMenuOption(const char* id,const char* option)
	{
		if(n_options<10)
		{
      strcpy(menus[n_options].id,id);
			strcpy(menus[n_options].texto,option);
			n_options++;
			return true;
		}
		else
			return false;
	}

  char* getSelectedId()
  {
      if(n_options>0)
        return menus[selected].id;
      else
        return NULL;
  }

	void clearMenuOptions()
	{
		n_options=0;
		selected=0;
	}

	void drawMenu()
	{
	  display.clearDisplay();
	  display.setTextSize(1);             
	  display.setTextColor(SSD1306_WHITE);
	  display.setCursor(0,0);
	  display.println(F("Selecione o app:"));
	  display.println("");
	  for(int i=0;i < n_options; i++)
	  {
		if(i == selected)
		  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
		else
		  display.setTextColor(SSD1306_WHITE);
		display.println(menus[i].texto);
	  }
	  display.display();
	}

  void drawBitmap(const unsigned char* bitmap)
	{
	  display.clearDisplay();
	  display.drawXBitmap(0, 0,  bitmap, 128, 64, WHITE);
	  display.display();
	}

  void drawSimpleText(char* message)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextWrap(true);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,20);
    display.println(message);
    display.display();
  }

  void drawSimpleText(char* message1,char* message2)
  {
    int16_t x1, y1;
    uint16_t w, h;

    display.clearDisplay();
    display.setTextSize(1);             
    display.setTextColor(SSD1306_WHITE);
    display.getTextBounds(message1, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((LARGURA_OLED - w) / 2,20);
    display.println(message1);
    display.println("");

    display.getTextBounds(message2, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((LARGURA_OLED - w) / 2, (ALTURA_OLED/2)+20);
             
    display.println(message2);
    display.display();
  }

	void drawLongText(char* message)
	{
	  // start
	  removeAccents(message);
	  display.setTextWrap(false);
	  display.setTextColor(SSD1306_WHITE);    
	  int x = display.width();
	  int minX = -12 * strlen(message); // 12  = 6 pixels/char * text_size * 2

	  //loop
	  while(true)
	  {
		display.clearDisplay();
		display.setTextSize(2);
		display.setCursor(x,20);
		display.print(message);
		display.display();
		x=x-4; // scroll speed
		if(x < minX)
		  break;
		delay(30);
	  }
	}

	void removeAccents(char Text[])
	{
	  int TextLength = strlen(Text);
	  int i = 0;
	  do {
		char c = Text[i];
		if ((uint8_t)c == 195) {
		  char c2 = Text[i + 1];
		  switch ((uint8_t)c2) {
			case 160 ... 166:
			  Text[i] = 'a';
			  break;
			case 167:
			  Text[i] = 'c';
			  break;
			case 168 ... 171:
			  Text[i] = 'e';
			  break;
			case 172 ... 175:
			  Text[i] = 'i';
			  break;
			case 177:
			  Text[i] = 'n';
			  break;
			case 178 ... 182:
			  Text[i] = 'o';
			  break;
			case 185 ... 188:
			  Text[i] = 'u';
			  break;
			case 189 ... 191:
			  Text[i] = 'y';
			  break;
			case 128 ... 134:
			  Text[i] = 'A';
			  break;
			case 135:
			  Text[i] = 'C';
			  break;
			case 136 ... 139:
			  Text[i] = 'E';
			  break;
			case 140 ... 143:
			  Text[i] = 'I';
			  break;
			case 146 ... 150:
			  Text[i] = 'O';
			  break;
			case 153 ... 156:
			  Text[i] = 'U';
			  break;
			default:
			  break;
		  }
		  for (int j = i + 1; j < TextLength; j++) Text[j] = Text[j + 1];
		  TextLength --;
		}
		i++;
	  } while (i < TextLength);
	}

private:

  Adafruit_SSD1306 display;
  Menu menus[10];
  char accents[102];

public:

  int selected;
  int n_options;
};

#endif
