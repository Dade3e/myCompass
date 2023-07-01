bool compassCalib(){
  int annulla = 0;
  bool calibrated = false;
  int calibrationData[3][2];
  bool changed = false;
  bool done = false;
  int t = 0;
  int c = 0;

  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
  delay(100);

  while(calibrated == false && annulla == 0){
    if(digitalRead(A) == LOW)
      annulla = 1;
    if(digitalRead(B) == LOW)
      annulla = 2;
    
    // If not calibrated
    int xC, yC, zC;
  
    // Read compass values
    compass.read();

    // Return XYZ readings
    xC = compass.getX();
    yC = compass.getY();
    zC = compass.getZ();

    changed = false;

    if(xC < calibrationData[0][0]) {
      calibrationData[0][0] = xC;
      changed = true;
    }
    if(xC > calibrationData[0][1]) {
      calibrationData[0][1] = xC;
      changed = true;
    }

    if(yC < calibrationData[1][0]) {
      calibrationData[1][0] = yC;
      changed = true;
    }
    if(yC > calibrationData[1][1]) {
      calibrationData[1][1] = yC;
      changed = true;
    }

    if(zC < calibrationData[2][0]) {
      calibrationData[2][0] = zC;
      changed = true;
    }
    if(zC > calibrationData[2][1]) {
      calibrationData[2][1] = zC;
      changed = true;
    }

    if (changed && !done) {
      Serial.println("CALIBRATING... Keep moving your sensor around.");
      c = millis();
      CompassAnimation( calibrationData[0][0],calibrationData[0][1],
                        calibrationData[1][0],calibrationData[1][1],
                        calibrationData[2][0],calibrationData[2][1] );
    }
    t = millis();
    
    
    if ( (t - c > 5000) && !done) {
      done = true;
      Serial.println("DONE. Copy the line below and paste it into your projects sketch.);");
      Serial.println();
        
      Serial.print("compass.setCalibration(");
      Serial.print(calibrationData[0][0]);
      Serial.print(", ");
      Serial.print(calibrationData[0][1]);
      Serial.print(", ");
      Serial.print(calibrationData[1][0]);
      Serial.print(", ");
      Serial.print(calibrationData[1][1]);
      Serial.print(", ");
      Serial.print(calibrationData[2][0]);
      Serial.print(", ");
      Serial.print(calibrationData[2][1]);
      Serial.println(");"); 

      compass.setCalibration( calibrationData[0][0], calibrationData[0][1], calibrationData[1][0],
                            calibrationData[1][1], calibrationData[2][0], calibrationData[2][1]);
      calibrated = true;
    }
  }

  while(digitalRead(A) == LOW);
  while(digitalRead(B) == LOW);
  if(annulla == 0){
    String str_calib =  String(calibrationData[0][0]) +"\n"+ 
                        String(calibrationData[0][1]) +"\n"+ 
                        String(calibrationData[1][0]) +"\n"+ 
                        String(calibrationData[1][1]) +"\n"+ 
                        String(calibrationData[2][0]) +"\n"+ 
                        String(calibrationData[2][1]) +"\n";
    

    return writeFile(SD, "/calib.txt", str_calib.c_str());
  }
  
  if(annulla == 1){
    //Impostazione di default
    /*-942
      1426
      -1897
      1052
      -1592
      1546
    */
    calibrationData[0][0] = -942;
    calibrationData[0][1] = 1426;
    calibrationData[1][0] = -1897;
    calibrationData[1][1] = 1052;
    calibrationData[2][0] = -1592;
    calibrationData[2][1] = 1546;

    compass.setCalibration( calibrationData[0][0], calibrationData[0][1], calibrationData[1][0],
                            calibrationData[1][1], calibrationData[2][0], calibrationData[2][1]);

    String str_calib =  String(calibrationData[0][0]) +"\n"+ 
                        String(calibrationData[0][1]) +"\n"+ 
                        String(calibrationData[1][0]) +"\n"+ 
                        String(calibrationData[1][1]) +"\n"+ 
                        String(calibrationData[2][0]) +"\n"+ 
                        String(calibrationData[2][1]) +"\n";

    return writeFile(SD, "/calib.txt", str_calib.c_str());
  }
  return 0;
  
}

