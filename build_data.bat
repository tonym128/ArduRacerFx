echo "Build Data File"
cd fxdata
python .\fxdata-build.py .\fxdata.txt
cd ..

echo "Build application"
arduino-cli compile -e --fqbn=arduboy-homemade:avr:arduboy-homemade:based_on=leonardo,boot=cathy3k,core=arduino-core ArduRacerFx.ino

echo "Copy builds to final folder"
copy .\build\arduboy-homemade.avr.arduboy-homemade\ArduRacerFx.ino.hex .\Extra\ArduRacerFx.hex
copy .\fxdata\fxdat*.bin .\Extra
