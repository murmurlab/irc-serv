#include "common.hpp"
#include "errors.hpp"

int main(int argc, char const *argv[]) try {
	if (argc != 3)
		throw (runtime_error(E_ARG));
	
	return 0;
} catch (exception& e) {
	cerr << e.what() << endl;
}
