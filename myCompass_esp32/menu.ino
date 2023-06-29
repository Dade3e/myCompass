void menu(){
  draw_s("MENU");
  delay(650);
  draw_s("");
  delay(50);
  uint8_t select = 1;
  int i = 2;
  int wait = 0;
  int text_vel = 4;
  while(select != 99){
    
    if(digitalRead(LEFT) == LOW){
      select += 1;
      wait = 0;
      i = 2;      
    }
      
    while(digitalRead(LEFT) == LOW);

    switch(select){
      case(1):
        i = draw_scroll("   SELECT POINT   ", i);
        wait ++;
        if(wait == text_vel){
          wait = 0;
          i++;          
        }
        if(digitalRead(RIGHT) == LOW)
          select = 10;
        while(digitalRead(RIGHT) == LOW);
      break;
      case(2):
      case(4):
        i = 2;
        wait = 0;
        select +=1;
      break;
      case(3):
        i = draw_scroll("   CALIB   ", i);
        wait ++;
        if(wait == text_vel){
          wait = 0;
          i++;          
        }
        if(digitalRead(RIGHT) == LOW)
          select = 11;
        while(digitalRead(RIGHT) == LOW);
      break;
      case(5):
        draw_s("EXIT");
        if(digitalRead(RIGHT) == LOW)
          select = 99;
        while(digitalRead(RIGHT) == LOW);
      break;
      case(10):
        select_coords();
        i = 2;
        wait = 0;
        select = 1;
      break;
      case(11):
        //screen()
        compassCalib();
        i = 2;
        wait = 0;
        select = 3;
      break;
      default:
        select = 1;
        break;
      
    }
    delay(50);
  }

}

void select_coords(){
  bool exit = false;
  uint8_t select = s_coords;
  int i = 2;
  int wait = 0;
  while(digitalRead(RIGHT) == HIGH){
    if(digitalRead(LEFT) == LOW){
      select += 1;
      i = 2;
      wait = 0;
    }
      
    while(digitalRead(LEFT) == LOW);
    if(select >= (sizeof(TARGET_NAME) / sizeof(TARGET_NAME[0])))
      select = 0;
    i = draw_scroll(TARGET_NAME[select], i);
    wait ++;
    if(wait == 4){
      wait = 0;
      i++;          
    }
    delay(50);
  }
  
  s_coords = select;
  draw_s("OK ");
  delay(1000);
}

