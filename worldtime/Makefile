.PHONY:all
all: worldtime select

worldtime:worldtime.c
	gcc $^ -o $@  -L /usr/lib64/mysql -l mysqlclient
select:select.c
	gcc $^ -o $@  -L /usr/lib64/mysql -l mysqlclient

.PHONY:clean
clean:
	rm worldtime
