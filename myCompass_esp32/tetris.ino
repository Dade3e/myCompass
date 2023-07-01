void SetupTetris(){
  display.setRotation(1);
  display.clearDisplay();
  //display.drawBitmap(3, 23, mantex_logo, 64, 82,  WHITE);
  String a = "TETRIS";
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  int len = a.length();
  int x_text = (128-(len*6)) /2;
  display.setCursor(x_text, 36);
  display.print(a);
  display.display();
  delay(100);
  display.clearDisplay();
  drawLayout();
  display.display();
  randomSeed(analogRead(0));
  nextType = random(TYPES);
  generate();
  timer = millis();
  Tetris();
}  
  
void Tetris() {
  while(digitalRead(B) == HIGH){
    if(millis() - timer > interval){
      checkLines();
      refresh();
      if(nextCollision()){
        for(short i = 0; i < 4; i++)
          grid[pieceX + piece[0][i]][pieceY + piece[1][i]] = 1;
        generate();
      }else
        pieceY++;
      timer = millis();
    }
    if(!digitalRead(DOWN)){
      tone(SPEAKER_PIN, click[0], 1000 / click_duration[0]);
      delay(100);
      noTone(SPEAKER_PIN);
      if(b1){
        if(!nextHorizontalCollision(piece, -1)){
          pieceX--;
          refresh();
        }
        b1 = false;
      }
    }else{
      b1 = true;
    }
    if(!digitalRead(UP)){
      tone(SPEAKER_PIN, click[0], 1000 / click_duration[0]);
      delay(100);
      noTone(SPEAKER_PIN);
      if(b2){
        if(!nextHorizontalCollision(piece, 1)){
          pieceX++;
          refresh();
        }
        b2 = false;
      }
    }else{
      b2 = true;
    }
    if(!digitalRead(RIGHT)){
      interval = 20;
    } else{
      interval = 400;
      }
    if(!digitalRead(LEFT)){
      tone(SPEAKER_PIN, click[0], 1000 / click_duration[0]);
      delay(100);
      noTone(SPEAKER_PIN);
      if(b3){
        if(rotation == getMaxRotation(currentType) - 1 && canRotate(0)){
          rotation = 0;
        }else if(canRotate(rotation + 1)){
          rotation++;
        }  
        copyPiece(piece, currentType, rotation);
        refresh();
        b3 = false;
        delayer = millis();
      }
    }else if(millis() - delayer > 50){
      b3 = true;
    }
  }
}

  void drawLayout(){
    display.drawLine(0, 15, WIDTH, 15, WHITE);
    display.drawRect(0, 0, WIDTH, HEIGHT, WHITE);
    drawNextPiece();
    char text[6];
    itoa(score, text, 10);
    drawText(text, getNumberLength(score), 7, 4);
  }
  short getNumberLength(int n){
    short counter = 1;
    while(n >= 10){
      n /= 10;
      counter++;
    }
    return counter;
  }
  void drawText(char text[], short length, int x, int y){
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(WHITE); // Draw white text
    display.setCursor(x, y);     // Start at top-left corner
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
    for(short i = 0; i < length; i++)
      display.write(text[i]);
  }

    void copyPiece(short piece[2][4], short type, short rotation){
    switch(type){
    case 0: //L_l
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_L_l[rotation][0][i];
        piece[1][i] = pieces_L_l[rotation][1][i];
      }
      break;
    case 1: //S_l
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_S_l[rotation][0][i];
        piece[1][i] = pieces_S_l[rotation][1][i];
      }
      break;
    case 2: //S_r
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_S_r[rotation][0][i];
        piece[1][i] = pieces_S_r[rotation][1][i];
      }
      break;
    case 3: //Sq
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_Sq[0][0][i];
        piece[1][i] = pieces_Sq[0][1][i];
      }
      break;
      case 4: //T
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_T[rotation][0][i];
        piece[1][i] = pieces_T[rotation][1][i];
      }
      break;
      case 5: //l
      for(short i = 0; i < 4; i++){
        piece[0][i] = pieces_l[rotation][0][i];
        piece[1][i] = pieces_l[rotation][1][i];
      }
      break;
    }
  }
  short getMaxRotation(short type){
    if(type == 1 || type == 2 || type == 5)
      return 2;
    else if(type == 0 || type == 4)
      return 4;
    else if(type == 3)
      return 1;
    else
      return 0;
  }
  boolean canRotate(short rotation){
    short piece[2][4];
    copyPiece(piece, currentType, rotation);
    return !nextHorizontalCollision(piece, 0);
  }

  void generate(){
    currentType = nextType;
    nextType = random(TYPES);
    if(currentType != 5)
      pieceX = random(9);
    else
      pieceX = random(7);
    pieceY = 0;
    rotation = 0;
    copyPiece(piece, currentType, rotation);
  }
  void drawPiece(short type, short rotation, short x, short y){
    for(short i = 0; i < 4; i++)
      display.fillRect(MARGIN_LEFT + (SIZE + 1)*(x + piece[0][i]), MARGIN_TOP + (SIZE + 1)*(y + piece[1][i]), SIZE, SIZE, WHITE);
  }
  void drawNextPiece(){
    short nPiece[2][4];
    copyPiece(nPiece, nextType, 0);
    for(short i = 0; i < 4; i++)
      display.fillRect(50 + 3*nPiece[0][i], 4 + 3*nPiece[1][i], 2, 2, WHITE);
  }

    void refresh(){
      display.clearDisplay();
      drawLayout();
      drawGrid();
      drawPiece(currentType, 0, pieceX, pieceY);
      display.display();
  }
  void drawGrid(){
    for(short x = 0; x < 10; x++)
      for(short y = 0; y < 18; y++)
        if(grid[x][y])
          display.fillRect(MARGIN_LEFT + (SIZE + 1)*x, MARGIN_TOP + (SIZE + 1)*y, SIZE, SIZE, WHITE);
  }
  boolean nextHorizontalCollision(short piece[2][4], int amount){
    for(short i = 0; i < 4; i++){
      short newX = pieceX + piece[0][i] + amount;
      if(newX > 9 || newX < 0 || grid[newX][pieceY + piece[1][i]])
        return true;
    }
    return false;
  }
  boolean nextCollision(){
    for(short i = 0; i < 4; i++){
      short y = pieceY + piece[1][i] + 1;
      short x = pieceX + piece[0][i];
      if(y > 17 || grid[x][y])
        return true;
    }
    return false;
  }

  void checkLines(){
    boolean full;
    for(short y = 17; y >= 0; y--){
      full = true;
      for(short x = 0; x < 10; x++){
        full = full && grid[x][y];
      }
      if(full){
        breakLine(y);
        y++;
      }
    }
  }
  void breakLine(short line){
      tone(SPEAKER_PIN, erase[0], 1000 / erase_duration[0]); 
      delay(100);
      noTone(SPEAKER_PIN);
    for(short y = line; y >= 0; y--){
      for(short x = 0; x < 10; x++){
        grid[x][y] = grid[x][y-1];
      }
    }
    for(short x = 0; x < 10; x++){
      grid[x][0] = 0;
    }
    display.invertDisplay(true);
    delay(50);
    display.invertDisplay(false);
    score += 10;
  }