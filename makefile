
# Makefile


# Declare the mysql_config_path
mysql_config_path = /usr/local/bin/mysql_config
files = filehasher.c files.c database.c hex.c
executable = filehasher


# all
all:
	gcc -o $(executable) $(files) \
	$(shell $(mysql_config_path) --cflags) $(shell $(mysql_config_path) --libs) -Wall
debug:
	gcc -o $(executable) $(files) \
        $(shell $(mysql_config_path) --cflags) $(shell $(mysql_config_path) --libs) -Wall -DDEBUG

clean:
	rm -rf *.o
	
