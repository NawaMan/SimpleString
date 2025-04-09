CXXFLAGS = -std=c++20 -I/usr/include
LDFLAGS = -L/usr/lib -lsstring_lib -lboost_locale -lboost_regex

all: test_app

test_app: test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f test_app
