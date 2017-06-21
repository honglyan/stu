CC=gcc
AFLAGS=-Wall -g
LDFLAGS= -lmysqlclient
OBJS= add.cgi del.cgi mod.cgi sel.cgi stu.cgi stusel.cgi stumod.cgi tea.cgi teac.cgi teag.cgi addc.cgi adds.cgi

all:$(OBJS)

$(OBJS):%.cgi:%.c
	$(CC) $(AFLAGS) $< cgic.c -o $@ $(LDFLAGS)

.PHONY:clean
clean:
	rm ./*.cgi
install:
	cp *.cgi /usr/lib/cgi-bin/sx
	cp head.html footer.html  stu.html teacher.html /usr/lib/cgi-bin/sx/
	sudo cp index.html  /var/www/html
