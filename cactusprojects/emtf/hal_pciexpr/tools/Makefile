include ../Makefile.in

# all only contains the general targets since we cannot know if the user
# has the SBS driver installed or not.

all : checksum asciiToInt

checksum : checksum.cc
	g++ -Wall $< -o $@ 

asciiToInt : asciiToInt.cc
	g++ -Wall $< -o $@ 

makeSBSSemaphore : makeSBSSemaphore.cc
	g++ -Wall -DBT1003 -I../busAdapter/include -I../generic/include -I$(SBSINCDIR) $< -o $@

printSemaphore : printSemaphore.cc
	g++ -Wall -DBT1003 -I../busAdapter/include -I../generic/include -I$(SBSINCDIR) $< -o $@

