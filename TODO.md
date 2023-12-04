# TODO
- Play game through on Arduboy making times for every level
    - Once without car mods, best time
    - Set 3rd place time, okay time, no mods, no shortcuts
    - Set better first place times
- Write README.md for github repo with gameplay instructions
    - Web Player - https://tiberiusbrown.github.io/Ardens/player.html?file=https://raw.githubusercontent.com/tonym128/ArduRacerFx/main/Extra/ArduRacerFx.hex&file=https://raw.githubusercontent.com/tonym128/ArduRacerFx/main/Extra/fxdata.bin
- Write the Arduboy community topic
- Add to http://www.bloggingadeadhorse.com/cart/Cart.html
- Can I submit to ? https://arduboy.ried.cl/
- Video demo

# Bugs

# DONE
- Run in online emulator
- Github action
    - Build app
    - Build release
- Fix zoom offset for tile zoomer
- Arduino CLI compile works
    - Removed bootscreen for a couple hundred bytes
    - Used base for a bunch of space savings!
    - arduino-cli compile -e --fqbn=arduboy-homemade:avr:arduboy-homemade:based_on=leonardo,boot=cathy3k,core=arduino-core ArduRacerFx.ino
- Play game through on Arduboy making times for every level
    - First place, car tuning, everything goes, best possible time
- Save data fails to save first time in Ardens (not an issue on device)
    - FX Busy error
    - Save file at startup if it's not found
- Simplify any levels too hard to get 99.99 sec
- Reset maxlevel to 1
- Integrate into a FX image with multiple games
- Music and SFX
    - Music repeats on level select change
    - Music for Zoom needs work
    - Add Win music
- Add points to the main screen (levels 1st = 5 points, 2nd = 3 points, 3rd = 1, haven't finished = 0)
    - 20 levels = 100 points
- Work out why flashing text causes slow downs (Text is just slow, but fine for perf)
- Car is too fast at high frame rates    
- Update trophy screen for 20 levels
- Increase max time to 99.99 seconds (overflow, change to 16-bit save and 32-bit gameplay to match millis)
- Issues with saving 32 bit indexed int, changed to uint16t

- Configure car performance
    - Max Speed
    - Accelerate
    - Turn

- Increase level size
- Add more levels

- Add an ingame map screen with current location
- Move level times into FX
- Rewrite Number Zoom to use FX
- Rewrite Stage Zoom to use FX
- Move levels into FX
    - Single level var to store current level layout
    - Max level size increase 30x30 ?
- Use FX Save
- Resolved performance issues
    - Change ArdBitmap to another effect 
        - scroll the number tiles across the screen
        - Grayscale race starter

    - Remove ArdBitmap
        - Number zoom, create tiles and stream
- Use fewer loads 
    - Smaller tiles for maps

# Skip
- Add missing tiles to minimize rendering (game logic changes)
- Make newline detection optional (minimal benefit)
- Use FX Arduboy Font (my impl is simpler)
- Update all graphics to grayscale (not fast enough)
- Add a win video (waste of space)
- Add new car models (not without grayscale)
- Use Grayscale (not rendering fast enough)
- Move screen back when car is going faster
- Last lap isn't clear for changing lanes (just gonna do it)


# Features
- Locked 60 fps gameplay
- More levels
- Bigger levels, 60% bigger and 3x for some.
- Modify car stats for each race
- Save data on FX, no more overrites
- Double the frames for car direction
- Map display
- Better sounds
- Music
- Beat the Dev (my best times are the best times)
    - First place, any method, any car
    - 2nd place, follow the track, default car
    - 3rd place, a few seconds off the pace to allow easier progress.
- 99% Arduboy Memory used.
- >50kb of Arduboy FX Data!

# Technical callouts
- Levels on FX
- Tile sets on FX
- Updated Draw routines for FX
- ArduBitmap ported to FX for 1 use case
- MipMap zooming using FX draw
- Dynamic tilemap caching and windowing
- Adding ArduTonesFX
- Debugging
- Performance Tuning Display Routine, finding map issue
- Arderns
    - Undoing my cross platform code

# Failures
- Getting framerate sufficient for grayscale

# Skipped for release
- Map explorer - Right press on level select
    - Up down left right to pan
    - A change zoom level 4x ?
    - B exit back to level select
- Custom map builder
