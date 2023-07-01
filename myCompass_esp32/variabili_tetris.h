static const unsigned char PROGMEM mantex_logo [] = {
  0x00, 0x00, 0x18, 0x06, 0x01, 0xc0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
  };

  const char pieces_S_l[2][2][4] = {{
                                      {0, 0, 1, 1}, {0, 1, 1, 2}
                                    },
                                    {
                                      {0, 1, 1, 2}, {1, 1, 0, 0}
                                    }};

  const char pieces_S_r[2][2][4]{{
                                    {1, 1, 0, 0}, {0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2}, {0, 0, 1, 1}
                                  }};

const char pieces_L_l[4][2][4] = {{
                                    {0, 0, 0, 1}, {0, 1, 2, 2}
                                  },
                                  {
                                    {0, 1, 2, 2}, {1, 1, 1, 0}
                                  },
                                  {
                                    {0, 1, 1, 1}, {0, 0, 1, 2}
                                  },
                                  {
                                    {0, 0, 1, 2}, {1, 0, 0, 0}
                                  }};

const char pieces_Sq[1][2][4] = {{
                                    {0, 1, 0, 1}, {0, 0, 1, 1}
                                  }};

const char pieces_T[4][2][4] = {{
                                    {0, 0, 1, 0},{0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2},{1, 0, 1, 1}
                                  },
                                  {
                                    {1, 0, 1, 1},{0, 1, 1, 2}
                                  },
                                  {
                                    {0, 1, 1, 2},{0, 0, 1, 0}
                                  }};

const char pieces_l[2][2][4] = {{
                                    {0, 1, 2, 3}, {0, 0, 0, 0}
                                  },
                                  {
                                    {0, 0, 0, 0}, {0, 1, 2, 3}
                                  }};

#define WIDTH 64
#define HEIGHT 128

const short MARGIN_TOP = 19; //- declares a constant variable MARGIN_TOP with a value of 19 of type short.
const short MARGIN_LEFT = 3; //- declares a constant variable MARGIN_LEFT with a value of 3 of type short.
const short SIZE = 5; //- declares a constant variable SIZE with a value of 5 of type short.
const short TYPES = 6;// - declares a constant variable TYPES with a value of 6 of type short.
#define SPEAKER_PIN 32// - creates a macro with the name SPEAKER_PIN and a value of 3. This allows the code to refer to SPEAKER_PIN throughout the rest of the code and substitute it with the value 3.
const int MELODY_LENGTH = 10; //- declares a constant variable MELODY_LENGTH with a value of 10 of type int.
const int MELODY_NOTES[MELODY_LENGTH] = {262, 294, 330, 262}; //- declares an array MELODY_NOTES of size MELODY_LENGTH (which is 10), and initializes it with four integer values.
const int MELODY_DURATIONS[MELODY_LENGTH] = {500, 500, 500, 500}; //- declares an array MELODY_DURATIONS of size MELODY_LENGTH (which is 10), and initializes it with four integer values.
int click[] = { 1047 };// - declares an array click of size 1, and initializes it with one integer value.
int click_duration[] = { 100 }; //- declares an array click_duration of size 1, and initializes it with one integer value.
int erase[] = { 2093 }; //- declares an array erase of size 1, and initializes it with one integer value.
int erase_duration[] = { 100 }; //- declares an array erase_duration of size 1, and initializes it with one integer value.
word currentType, nextType, rotation; //- declares three variables of type word, named currentType, nextType, and rotation.
short pieceX, pieceY; //- declares two variables of type short, named pieceX and pieceY.
short piece[2][4];// - declares a two-dimensional array piece of size 2x4 with elements of type short.
int interval = 20, score; //- declares two variables, interval and score, of type int. interval is initialized with a value of 20.
long timer, delayer; //- declares two variables, timer and delayer, of type long.
boolean grid[10][18];// - declares a two-dimensional array grid of size 10x18 with elements of type boolean.
boolean b1, b2, b3;// - declares three variables of type boolean, named b1, b2, and b3.