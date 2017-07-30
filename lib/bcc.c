#include <alfwrapper/die.h>        // for die
#include <alfwrapper/fd.h>         // for fd_close
#include <alfwrapper/path.h>       // for path_change, path_basename, path_d...
#include <alfwrapper/socket.h>     // for socket_option
#include <bcc/bpf_common.h>        // for bpf_table_fd, bpf_function_size
#include <bcc/compat/linux/bpf.h>  // for bpf_prog_type::BPF_PROG_TYPE_SOCKE...
#include <bcc/libbpf.h>            // for bpf_lookup_elem, bpf_prog_load
#include <stddef.h>                // for NULL, size_t
#include <sys/socket.h>            // for SOL_SOCKET, SO_ATTACH_BPF, SO_LOCK...

void bcc_attach (int sockfd, void* module, const char* function) {
	/* Load the BPF filter into the kernel */
	void* start = bpf_function_start (module, function);
	size_t size = bpf_function_size (module, function);
	int progfd = bpf_prog_load (BPF_PROG_TYPE_SOCKET_FILTER, start, size, "GPL", 0, NULL, 0);

	/* Attach the filter to the socket and prevent the filter from being removed */
	socket_option (sockfd, SOL_SOCKET, SO_ATTACH_BPF, &progfd, sizeof(progfd));
	socket_option (sockfd, SOL_SOCKET, SO_LOCK_FILTER, &(int){1}, sizeof (int));

	/* Close the program fd */
	fd_close (progfd);
}

/* bpf_table_fd() and bpf_update_elem() with error checking */
void bcc_get (void* module, const char* name, void* key, void* value) {
	int table_fd = bpf_table_fd (module, name);

	if (table_fd < 0) {
		die ("bpf_table_fd(%p, %s) failed", module, name);
	}

	int r = bpf_lookup_elem (table_fd, key, value);

	if (r != 0) {
		die ("bpf_lookup_elem(%d, %p, %p) failed", table_fd, key, value);
	}
}

/* bpf_table_fd() and bpf_update_elem() with error checking */
void bcc_set (void* module, const char* name, void* key, void* value){
	int table_fd = bpf_table_fd (module, name);

	if (table_fd < 0) {
		die ("bpf_table_fd(%p, %s) failed", module, name);
	}

	int r = bpf_update_elem (table_fd, key, value, 0);

	if (r != 0) {
		die ("bpf_update_elem(%d, %p, %p, %d) failed", table_fd, key, value, 0);
	}
}

/* bpf_module_create_c() with error checking */
void* bcc_load (const char* file) {
	/* Save the current directory for later */
	const char* cwd = path_get_current ();

	/* Change to the directory of the file so relative includes work */
	path_change (path_dirname (file));

	/* Load the file */
	void* module = bpf_module_create_c (path_basename (file), 0, NULL, 0);

	if (module == NULL) {
		die ("bpf_module_create_c (%s, 0, NULL, 0) failed", file);
	}

	/* Change back to the original directory */
	path_change (cwd);

	return module;
}
