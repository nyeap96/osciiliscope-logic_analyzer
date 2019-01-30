oscilloscope: oscilloscope.c
	gcc -I/usr/include/libusb-1.0 -lusb-1.0 -I/opt/vc/include -I/usr/include -lshapes -o oscilloscope oscilloscope.c
	
expression.o: logicExpression.h logicExpression.c
	gcc -c logicExpression.c
	
logic: logic.c logicExpression.o
	gcc -I/usr/include/libusb-1.0 -lusb-1.0 -I/opt/vc/include -I/usr/include -lshapes -o logic logicAnalyzer.c logicExpression.o