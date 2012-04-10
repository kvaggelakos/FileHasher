
# Makefile


# Declare the mysql_config_path
mysql_config_path = /usr/local/bin/mysql_config

# all
all:
	gcc -o filehasher filehasher.c files.c database.c \
	$(shell $(mysql_config_path) --cflags) $(shell $(mysql_config_path) --libs) -Wall
debug:
	gcc -o filehasher filehasher.c files.c database.c \
        $(shell $(mysql_config_path) --cflags) $(shell $(mysql_config_path) --libs) -Wall -DDEBUG

clean:
	rm -rf *.o
	
