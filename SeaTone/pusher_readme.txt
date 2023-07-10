
This program extracts and imports sounds from the game Doukutsu Monogotari. Using it is pretty simple. (If you've already extracted the sounds, you can skip to step 4; you don't have to do the first part over again every time).


Step 1: Open it up. Tell it where the game is, and tell it where you want the sounds to go. The folder you pick for the latter should not have any other PXT's in it, and I recommend it to be empty because it's about to get filled up with 86 tiny files and you don't want to make a big mess in some work folder of yours.

Step 2: Push the button that says Pull. It should say it was successful. The first time you extract sounds from the game (before you import them back in) you will get some warning messages because some of the sounds in the game were actually glitched up in the way they were stored. But that's OK! I noticed that and put code in there that fixes it for you.

Step 3: Go edit your sounds in whatever program you like to use to edit PXTs. I highly recommend CTone.exe. I may be biased. You can also use PixTone, it'll work just fine too. Don't rename the files, and don't add or delete any files from the directory.*

Step 4: Push Allocate. During this step Pusher tries to figure out a way to cram all the PXTs back into the executable. It will actually re-order the sounds etc within the available space, so you can for example add channels to one sound if you take away channels from another. Doukutsu had 4 sounds saved within the executable that were never loaded or used in-game, so you already have 4 extra channels free to start with.

Step 5: Push Write. Now it goes inside your nice pretty game and scrambles the bits all up, and hopefully, your computer will still be able to read them afterwards (don't worry, it always works for me).

Step 6: If you didn't figure out that step 6 is to play the game, well....

Step 7: there is no step 7.

Step 8: there is no step 8, either.

Step 9: there IS a step 9, but it's a secret.

Step 10: Geez, I said it was simple to use. Quit reading the stupid readme, already.



Sound ID 7 is blank. If you put a sound in this slot, it will start playing incessantly as soon as the game gets about halfway through the loading screen, and it will never, ever, stop through the whole game. I think it's sort of like the Idle process in Windows XP: the PXT code is written in such a way that it always has to be playing SOMETHING, so ID 7 is there as a dummy sound to play when there's nothing else.


* Actually, you CAN rename the files, or add/delete files from the directory. Sort of.

When you import the sounds back in, Pusher rewrites the sound loader to use the sound ID that comes right after "ID" in the filename. So for "ID0e_snd_get_xp.pxt", it sets that sound to be loaded as ID 0x0E. So, if you're tricky about it, you could actually add or remove brand new sound IDs to the game. The catch is, while you can pick any IDs for them you like, you must have exactly 86 sounds loaded and no more and no less. So to add a new sound ID you would have to delete some other sound ID to make room, even if there was physically enough space to allocate the new sound data in the exe. And if that old sound ID ever were to play in-game, you could easily be injured by the flying electrons as it general-protection-faults all over you.

So if you have to delete a sound to add a new one, and if that old sound couldn't ever play anyway, why not just replace the old sound with your new sound and not mess with changing the IDs.

If you do change the IDs, you can't pick any IDs over 0x80. I know, the drum sounds used for the Org player have IDs in the 0x90 range. But those are a special case--they use a DWORD-sized PUSH, and there are only a few of those in the sound loader routine, and they're all already used by the drum sounds. Unless you want to delete one of the drums. Note: that is not a good idea. Remember what I said above about flying electrons? Well how you would like that massive explosion triggered by the very background music itself?

So, anyway, you probably shouldn't add/delete files from the directory.


--sshsigi
