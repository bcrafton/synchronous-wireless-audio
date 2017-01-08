import ctypes

testlib = ctypes.CDLL('/home/brian/Desktop/testlib1.so')
testlib.myprint()

x = testlib.add(1,2)
print x


p = ctypes.create_string_buffer(10);
testlib.set(p, 10)
print p[0]
