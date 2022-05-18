CXXFLAGS=-g -DSOLUTION -Wall

all: p3test01.out p3test02.out p3test03.out Life.out Life-gfx.out \
	Life-traditional.out

timetests:
	time ./Life-traditional.out 0 0 1024 1000 0 < pts02.txt
	time ./Life.out 0 0 1024 1000 0 < pts02.txt
	time ./Life.out 0 0 1048576 1000 0 < pts02.txt


Life.out: Point.o BBox.o QuadTree.o QuadTree_dump.o QTQuad.o QTQuad_dump.o Life.o
	g++ -g $^ -o $@

Life-gfx.out: Point.o BBox.o QuadTree.o QTQuad.o QTQuad_dump.o Life-gfx.o
	g++ -g $^ -o $@

Life-traditional.out: Point.o BBox.o Life-traditional.o
	g++ -g $^ -o $@

p3test01.out: Point.o BBox.o QuadTree.o QuadTree_dump.o QTQuad.o QTQuad_dump.o p3test01.o
	g++ -g $^ -o $@

p3test02.out: Point.o BBox.o QuadTree.o QuadTree_dump.o QTQuad.o QTQuad_dump.o p3test02.o
	g++ -g $^ -o $@

p3test03.out: Point.o BBox.o QuadTree.o QuadTree_dump.o QTQuad.o QTQuad_dump.o p3test03.o
	g++ -g $^ -o $@

p3test04.out: Point.o BBox.o QuadTree.o QuadTree_dump.o QTQuad.o QTQuad_dump.o p3test04.o
	g++ -g $^ -o $@

Life.o: QuadTree.h QTQuad.h BBox.h Point.h
Life-gfx.o: QuadTree.h QTQuad.h BBox.h Point.h
Life-traditional.o: BBox.h Point.h
p3test01.o: QuadTree.h QTQuad.h BBox.h Point.h
p3test02.o: QuadTree.h QTQuad.h BBox.h Point.h
p3test03.o: QuadTree.h QTQuad.h BBox.h Point.h
p3test04.o: QuadTree.h QTQuad.h BBox.h Point.h
Point.o: Point.h
BBox.o: BBox.h Point.h
QuadTree.o: QuadTree.h QTQuad.h BBox.h Point.h
QTQuad.o: QTQuad.h BBox.h Point.h
