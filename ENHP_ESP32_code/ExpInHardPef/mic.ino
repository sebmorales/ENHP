bool listenMic() {
  mic_event=false;
  if(mic_current_reading<mic_readings){
    int val = analogRead(mic);
    mic_min = min(mic_min, val);
    mic_max = max(mic_max, val); 
    mic_current_reading++;
  }else{
    int delta = mic_max - mic_min;
    //reset the readings
    mic_max=0;
    mic_min=1024;
    mic_current_reading=0;
    
    if (delta > mic_th) {
      // Serial.println(delta);
      mic_event=true;
      // digitalWrite(out, HIGH);
      old_mic=mic_event;
      return true;
    }
    if(mic_event!=old_mic){
      old_mic=mic_event;
      return true;
    }

  }
  return false;

}