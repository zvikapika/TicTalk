# TicTalk
TicTalk is an object that connects to your google calendar and tracks meetings appearing on it.
During these meetings is gets activated automatically and collects and analyzes audio data of, currently, up to 4 participants.

Hardware consists of an ESP8266 dev board, a 32 pixel led ring, an analog multiplexter, 4  microphones and a 3d-printred enclosure.

The plumbing: Using IFTTT, 15 minutes prior to any meeting that is about to start, the ESP receives a notification via Blynk API. 
From that minute on, it  counts down the time until the meeting is about to start. Then, after signaling meeting start (via the rudimentary ultra-cheesy led effect), it collects audio samples and evaluates then in order to make an educated guess as to who is the chief speaker in the conversation in that moment. The result of this evaluation is displayed on the led ring via color and place coding: Every speaker, assumed to be sitting in front of a microphone, is assigned a color by the system. At the end of the meeting, the collected data - scoring per participant, according to the amount of time that person was found to be speaking, is uploaded to Google Sheets via IFTTT, for further logging and analysis.

The purpose of this device is to eventually have a tchnology that allows us to investigate human interaction during a meeting or conversation, as well as, perhaps, promote collaborative meetings where all participants are engaged, not only one dominant speaker. Such conversations, we suspect, are more creative, satisfying, and usefull than others, both on short and long term.

important: *** NO AUDIO IS RECORDED OR UPLOADED DURING THE OPERATION OF THIS DEVICE ***
 
![alt text](https://github.com/zvikapika/TicTalk/blob/master/TicTalk.png?raw=true)

TODO: 
add audio interpolation to support more than 4 participants sitting directly in front of their mics.
add quality of conversation metrics
act upon quality of conversation metrics, prolonging the meeting or other positive incentive

