ECE 330L Microprocessors Lab 
Final Project 
Spring 2026 

In this project you will implement an electronic version of the classic game 
Battleship in the STM32F407. 

Game Play Sequency: 
1) The game starts with the 7 segment displays displaying the name of the game. 
Note that the marquee message scroller function available in the interrupt handler 
is a handy way to display messages that are more than 8 characters in length for 
different stages of the game. 
2) Player one places their boats first. Each player will place three single segment 
boats, and two double segment boats on the map. The map consists of two separate 
bitmaps, one for the vertical segments (vertically aligned boats) which is an array 
of 2 by 16 elements (32 total), and the other for horizontal segments (horizontally 
aligned boats) which is an array of 3 by 8 elements (24 total).  
Note that while you could choose to use an actual array variable for each of these 
maps, the most memory efficient method is to use two unsigned integers. The 
vertical segments can be allocated to the upper 16 bits of a 32 bit integer for the top 
row of vertical segments, and the lower 16 bits of a 32 bit integer for the bottom 
row of vertical segments. The three rows of horizontal segments can be represented 
by three 8 bit bytes withing a 32 bit integer. 1’s would represent a boat and 0’s 
empty space. 
3) After player one finishes placing their boats, player two places their boats in 
their boat maps. Thus, if two unsigned integers are used to store the boat maps, 
there will be two such integers for player one’s maps and two for player two’s 
maps. 
4) A key element of the game is the use of a cursor control to move a selector 
around the maps. This is best accomplished using two of the potentiometers, one 
for vertical movement and the other for horizontal movement. There also must be 
a switch allocated to select between the cursor being on the vertical or the 
horizontal segment maps. Having the cursor blinking is a good way to track where 
it is on the map since during placement and gameplay, shots and boats will be 
shown on the map with the cursor. 
ECE 330L Microprocessors Lab 
5) For each of the boat maps for each player, there also needs to be a shot map that 
records where shots were placed by the opposing player. During game play, shots 
that missed should be shown at 50% brighness and shots that hit should be shown 
at 100% brightness. 1’s represent shot placement with 0’s representing non shots. 
6) Player one and player two will alternate turns taking one shot each until one of 
the players successfully hits all 7 of their opponent’s boats, then a game over 
message should be shown that declares the winner. 
7) A display buffer is highly recommended to create an image of what is to be 
written to the 7 segment displays. This will make it easier to manage the displays 
rather than trying to update them directly. The Systik handler is a great place to 
manage the display buffer and display updating, as well as implementing the PWM 
for brightness control and cursor blinking functions. Since the Systik handler 
executes once per millisecond, it is fast enough to provide flicker free dimming 
since 100Hz is an adequate frame rate for dimming. This leaves 10 levels of 
possible dimming if a 10 millisecond frame rate is used. 
8) Sound effects and music are strongly encouraged. Sound can be used to 
punctuate hits and misses (explosions vs. splashes), and a song can be played to 
celebrate the winner (nautical themes are recommended). Timer 7 is the best place 
to manage sounds and you can use lab 6 as a basis to aid with this. 
9) The message marquee is a great way to announce the player turns, current score 
as the game progresses, and to announce the winner at the end. 
Feel free to use your creativity to make the game as playable and interesting as 
possible. 
Deliverables: 
1) Submit your C program files (.C and .H files that you modified) to Canvas 
in a ZIP file. Be sure to comment your code or you will be docked points! This 
submittal is a team effort. 
2) Include a paragraph in your report discussing what you felt was the most 
challenging part of your project and how you met this challenge. 
ECE 330L Microprocessors Lab 
3) Obtain check-off by showing the operation of your program to your instructor or 
lab assistant.  
4) Individually answer the three ABET questions found in the Assignments section 
in Canvas. You will get points for answering these questions. 
