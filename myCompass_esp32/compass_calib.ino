bool compassCalib(){
  int annulla = false;
  while(calibrated == false && annulla == false){
    if(digitalRead(B) == LOW)
      annulla = true;
    // If not calibrated
    int x, y, z;

    // Read compass values
    compass.read();

    // Return XYZ readings
    x = compass.getX();
    y = compass.getY();
    z = compass.getZ();

    changed = false;

    if(x < calibrationData[0][0]) {
      calibrationData[0][0] = x;
      changed = true;
    }
    if(x > calibrationData[0][1]) {
      calibrationData[0][1] = x;
      changed = true;
    }

    if(y < calibrationData[1][0]) {
      calibrationData[1][0] = y;
      changed = true;
    }
    if(y > calibrationData[1][1]) {
      calibrationData[1][1] = y;
      changed = true;
    }

    if(z < calibrationData[2][0]) {
      calibrationData[2][0] = z;
      changed = true;
    }
    if(z > calibrationData[2][1]) {
      calibrationData[2][1] = z;
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
      Serial.println("DONE.");
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
  
  calibrated = false;
  if(annulla == false){
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