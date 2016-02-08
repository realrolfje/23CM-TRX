# Tips, Tricks and Hardware Modifications

Building the 23cm transceiver kit will get you a working transceiver, but you want
more of course. So here are my notes on modifications we've done on some of the
transceivers.

## Gotchas, problems and solutions

## Modifications and improvements.

### Remove receiver audio rumbling

![C24 Receive Audio](images/receive-audio.png)

When receiving a strong FM signal on a decent speaker in a case, you'll notive a low 
rumble in the audio. This is the MC3362 detecting all audio frequencies. To stop the
lower frequencies from reaching the speaker, replace C24 with a 1nF capacitor.