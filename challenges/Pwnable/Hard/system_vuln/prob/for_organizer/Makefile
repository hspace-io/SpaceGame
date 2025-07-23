all: exploit chall
	@echo "All done!"
exploit:
	gcc -o hack hack.c -masm=intel
chall:
	gcc -o system system.c -lseccomp -no-pie -fomit-frame-pointer

