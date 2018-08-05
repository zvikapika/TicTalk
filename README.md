# TicTalk
TicTalk is an object that connects to your google calendar, for tracking conversation of up to 4 participants dynamics via audio analysis.
Hardware is made up of an ESP8266 dev board, a 32 pixel led ring, an analog multiplexter, 4  microphones and a 3d-printred enclosure.

Using IFTTT, 15 miuntes prior to any meeting that is about to start, the MCU receives a notification via Blynk API. 
From that minute on, it first counts down the time until the meeting is about to start. Then, after signaling meeting start (via an ultra-cheesy RGB running rainbow effect), it collects audio samples and evaluates then in order to make an educated guess as to who is the chief speaker in the conversation on that moment. The result of this evaluation is displayed on the led ring via color and place coding. every speaker, sitting in front of a microphone is assigned a color by the system. At the end of the meeting, the collected data - scoring per color, according to the amount of time that person was found to be speaking, is uploaded to Google Sheets via IFTTT.

The purpose of this device is eventually to have a tchnology that will allow us to investigate human interaction during a meeting or conversation, as well as, perhaps, promote meetings where all participants are engaged, not only one dominant speaker. Such conversations, we suspect, are more creative, satisfying, and usefull than others, both on short and long term.

*** NO AUDIO IS RECORDED OR UPLOADED DURING THE OPERATION OF THIS DEVICE ***
 
![alt text](https://github.com/zvikapika/TicTalk/blob/master/TicTalk.png?raw=true)

TODO: 
add audio interpolation to support more than 4 participants sitting directly in front of their mics.
add quality of conversation metrics
act upon quality of conversation metrics, prolonging the meeting or other positive incentive

