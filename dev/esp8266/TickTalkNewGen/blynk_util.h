#define MAX_BUFFER_LEN 100

// http://188.166.206.43/18e7be4ec98d4f8b85a24db905a81982/update/V0?value=
// String sent: May 30, 2018 at 02:33PM-May 30, 2018 at 03:18PM

// http://188.166.206.43/18e7be4ec98d4f8b85a24db905a81982/update/V0?value=Aug 1, 2018 at 02:40PM-Aug 1, 2018 at 02:45PM

void onAppointmentTriggered(int startDay, int startMonth, int startYear, int startHour, int startMinute, int endHour, int endMinute, int duration);

//char auth[] = "01dee24a40f747d4ad8b528cc82b224f";
char auth[] = "18e7be4ec98d4f8b85a24db905a81982";


MatchState ms;
char buff[MAX_BUFFER_LEN];
char* monthNames[] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

void blynkSetup() {
  Blynk.config(auth);
}


// Assuming String received in the format of:
// "May 30, 2018 at 02:33PM-May 30, 2018 at 03:18PM"
// (defined in IFTTT applet)
BLYNK_WRITE(V0) {
  String data = param.asString();
  Serial.println(String("got: ") + data);
  data.toCharArray(buff, constrain(data.length(), 0, MAX_BUFFER_LEN));
  int startDay = 0;
  int startMonth = 0;
  int startYear = 0;
  for(int i = 0; i < 12; i++) {
    int mc = 0;
    for(int j = 0; j < 3; ++j) {
      if(buff[j] == monthNames[i][j]) {
        mc++;
      }
    }
    if(mc == 3) {
      startMonth = i + 1;
      break;
    }
  }
  ms.Target(buff);
  int result = ms.Match(" ");
  if(result > 0) {
    // date is double digit?
    if(buff[ms.MatchStart+3] == ',') {
//      Serial.println(data.substring(ms.MatchStart+1, ms.MatchStart+3));
//      Serial.println(data.substring(ms.MatchStart+5, ms.MatchStart+9));
      startDay = data.substring(ms.MatchStart+1, ms.MatchStart+3).toInt();
      startYear = data.substring(ms.MatchStart+5, ms.MatchStart+9).toInt();
    }
    else {
//      Serial.println(data.substring(ms.MatchStart+1, ms.MatchStart+2));
//      Serial.println(data.substring(ms.MatchStart+4, ms.MatchStart+8));
      startDay = data.substring(ms.MatchStart+1,  ms.MatchStart+2).toInt();
      startYear = data.substring(ms.MatchStart+4, ms.MatchStart+8).toInt();
    }
  }
  else {
    Serial.println("funky doodle");
  }
  result = ms.Match(" at ");
  if (result > 0) {
    int startHour = data.substring(ms.MatchStart + 4, ms.MatchStart + 6).toInt();
    int startMinute = data.substring(ms.MatchStart + 7, ms.MatchStart + 9).toInt();
    if (data.charAt(ms.MatchStart + 9) == 'P') {
      startHour += 12;
    }
    data = data.substring(ms.MatchStart + 10);
    data.toCharArray(buff, constrain(data.length(), 0, MAX_BUFFER_LEN));
    ms.Target(buff);
    result = ms.Match(" at ");
    if (result > 0) {
      int endHour = data.substring(ms.MatchStart + 4, ms.MatchStart + 6).toInt();
      int endMinute = data.substring(ms.MatchStart + 7, ms.MatchStart + 9).toInt();
      if (data.charAt(ms.MatchStart + 9) == 'P') {
        endHour += 12;
      }
      else {
        if(endHour == 12) endHour = 0;
      }
      // TODO: handle special case when meeting statr date is different than end date
      int duration = (endHour * 60 + endMinute - (startHour * 60 + startMinute));
      if(duration > 0) {
        onAppointmentTriggered(startDay, startMonth, startYear, startHour, startMinute, endHour, endMinute, duration);
      }
    }
    else {
      Serial.println("no match found for ending hour");
    }
  }
  else {
    Serial.println("no match found for starting hour");
  }
}




