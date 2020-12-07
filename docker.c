#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	char **new_argv = malloc(sizeof(char *) * (argc + 5));
	assert(new_argv);
	int i = 1;
	int injected = 0;
	new_argv[0] = argv[0];
	for (; i < argc; i++) {
		new_argv[i + injected * 4] = argv[i];
		if (!strcmp("run", argv[i]) && !injected) {
			new_argv[i + 1] = "--userns";
			new_argv[i + 2] = "keep-id";
			new_argv[i + 3] = "--user";
			new_argv[i + 4] = "root:root";
			injected = 1;
		}
	}
	new_argv[i + injected * 4] = NULL;
	assert(execvp("podman", new_argv) == 0);
	free(new_argv);
	return 0;
}
