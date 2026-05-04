
Hello.............




This program is a sort of in-house development tool. It was written to make sounds for my own games. However, the PXT file format is a perfectly good format so why re-invent the wheel, I thought. This means that SeaTone is backward compatible with PixTone, the program that originally used the PXT format. SeaTone can load and edit sounds created by PixTone, and vice versa as long as you don't use any of the extra features SeaTone adds to the format.

This means that SeaTone may also be of interest to those of you who are interested in making mods for games that used PixTone-based sound, namely Doukutsu Monogatari. I've included a seperate little app called Pusher that will make it easy to extract and import the PXT's in and out of the Doukutsu executable for those people that want it for that reason.

I've decided to release it for anyone who may be interested in arcade/SNES-style sound synthesis for whatever reason. Mostly because I want to see if you all can make anything neato with it. So if you like it please send me something you made with it or tell me about how you used it. That little ego boost makes me go "cool-o" and helps inspire me to release other stuff.



IMPORTANT:
TO MAKE SOUNDS FOR DOUKUTSU or other PixTone-based games you _cannot_ use the track "Pitch2" or the WhiteNoise channel (the last wavemodel icon on right). PixTone did not have these tracks and so Doukutsu cannot play them. They are a SeaTone extension to the PXT file format.

ALSO...when making sounds for PixTone, and in general, actually, I do not recommend you take the volume above 63. PixTone does not allow you to set it higher than this, and you may distort the waveform if you do. In general you can safely take it up to 127. Now, PixTone _WILL_ load and play sounds with a Top setting higher than 63, although it's scrollbar will be messed up. The ONLY reason I raised the Top limit to 255 for this program is because it allows higher freq sweeps, for example if you want a "siren" type sound that starts at a low freq and gets very high, you can do it by setting a Saw Up wave on your Pitch1 modulator and choosing 255 Top (you may glitch the scope display if you go over 128 because the waveform gets wrapped around it's 8-bit value. But the sound will still come out correct).



I have a widescreen monitor (1280x800). If you don't, the dock-like HUD at the bottom may not have enough room to appear below the main window like it's supposed to. If this happens, you can drag it around by it's client area to somewhere convenient. The program requires at least 1024x768, however. It really won't be usable with less.

If it should crash on you, let me know and attach the file "debug.stt". This is a debug log and will help me fix the problem.



The wave models in the icons are, from left to right: Sine, Triangle, Saw Up, Saw Down, Square Wave, Noise Channel, and White Noise.

"repts" is the number of times the wave will repeat over the length of the sound. If you're familiar with PixTone, PixTone called it "freq". However it is not really "frequency" in the sense of hertz because the actual frequency produced will be dependent on the length of the sound. For the "Main" tonegroup, SeaTone shows the final frequency in hertz in small font in the scope display. When you change the length of a sound, you can check "Keep Relative Freq" if you don't want your Main waveform's frequency to get messed up.

"Top" is synonymous with a waveform's volume. In PXT lingo, the volume of a TG (tonegroup) is called it's top. It's the most the sound will deviate from the centerline.

"size_blocks" defines the length of the sound in samples. SeaTone uses a 22050hz, 8-bit signed mono sample setting.

The Mute button next to each channel is only temporary and doesn't get saved. It's there for turning off individual tonegroups during testing the sound. Similarly for the Envelope disable button, saving with this on will just clear all your envelopes. If you want to turn off a tonegroup permanently, set it's volume and repts to 0.

The WhiteNoise channel is synthed by simply choosing a random value for each sample across it's length between the "Top" value, and the negative of the "Top" value. This generates a softer noise than what you will get from the regular noise channel. BUT, it means that the "Rept" setting will have no effect, and neither will any sort of pitch modulator you try to apply. The Volume modulator will work, however.



Ok, some shortcuts that may not be obvious:

 * You can right-click the mouse button to test-play the sound. This is actually how I intended you to test the sound; the "Play" button is mostly there just for looks.

 * You can loop the sound by pushing 'S'. You can also use 'S' to cut off playback of a sound immediately, if it turns out to sound like an evil ghoul from hyperspace dragging his teeth on a thousand interdimensional chalkboards.

 * You can adjust the parameters of the sound while it's looping. I've found this to be useful sometimes, particurly on short sounds, to get everything tweaked just right. Then right-click to stop the loop and right-click one more time to hear it by itself to make sure you got it just right. Using this technique tends to annoy anyone else in the room, though.



