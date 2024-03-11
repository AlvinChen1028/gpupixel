gcc -g -I../../src/raw/ -L../../output/library/linux/debug/ test.c -o test -lgpupixel -Wl,-rpath,'../../output/library/linux/debug/'
