
all: user hubert resto 


resto: resto.c shared.h struct.h message.c message.h resto.h f_co_resto.c
	gcc  -Wall resto.c message.c f_co_resto.c -o resto -lm

user: user.c message.c message.h user.h f_co_user.c
	gcc  -Wall user.c message.c f_co_user.c -o user -lm

hubert: hubert.c message.c message.h hubert.h f_co_hubert.c
	gcc -Wall hubert.c message.c f_co_hubert.c -o hubert -lm 

clean: 
	\rm -rf *~  *.o 

tar:
	make clean
