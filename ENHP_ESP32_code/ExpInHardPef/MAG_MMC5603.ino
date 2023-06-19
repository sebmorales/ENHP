void init_mag(){

  /* Initialise the sensor */
  if (!mmc.begin(MMC56X3_DEFAULT_ADDRESS, &Wire)) {  // I2C mode
    /* There was a problem detecting the MMC5603 ... check your connections */
    Serial.println("Ooops, no MMC5603 detected ... Check your wiring!");
    mag_success=false;
    // while (1) delay(10);
  }else{
    mag_success=true;
    /* Display some basic information on this sensor */
    mmc.printSensorDetails();
    mmc.setDataRate(1000); // in Hz, from 1-255 or 1000
  }
}  
  