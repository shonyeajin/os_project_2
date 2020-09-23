ssu_shell: ssu_shell.c pps ttop
	gcc ssu_shell.c -o ssu_shell
pps: pps.c
	gcc pps.c -o pps
ttop: ttop.c
	gcc ttop.c -o ttop
clean:
	rm ssu_shell
	rm ttop
	rm pps
