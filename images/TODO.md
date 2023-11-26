# TODO
- Music and SFX

- Move screen back when car is going faster
- Integrate into a FX image with multiple games

# Bugs
- Save dazta fails to save first time (or maybe when there is a new record)
    - FX Busy error
    - Save file at startup if it's not found
- Car is too fast at high frame rates    
- Work out why flashing text causes slow downs

# DONE
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


# Features
- Locked 60 fps gameplay
- More levels
- Bigger levels, 60% bigger and 3x for some.
- Modify car stats for each race
- Save data on FX, no more overrites
- Double the keyframes for car
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
- Getting to 4 colour grayscale

