# Objects/Targets
.PRECIOUS: %.cc
OPTIONS=-g -I../libnnet
LINK=-L ../libnnet -lnnet

all : 6_type_alcove

% : %.cc
	g++ $(OPTIONS) $< -o $@ $(LINK)

clean:
	rm *.o
